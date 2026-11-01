#!/usr/bin/env python3
"""Write MANIFEST.json and a deterministic source tarball.

--check rebuilds the tarball in memory and compares source hashes plus the
committed tarball bytes. host_sim_sha256 is informational (compiler output).
"""

from __future__ import annotations

import argparse
import gzip
import hashlib
import io
import json
import tarfile
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VERSION = "fw-hush-relay-0.1.0-evt"
PRIMARY_NAME = f"dist/{VERSION}-src.tar.gz"
MTIME = int(datetime(2026, 10, 22, tzinfo=timezone.utc).timestamp())
SOURCES = [
    "include/fw.h",
    "src/fw_node.c",
    "src/fw_ux.c",
    "src/fw_status.c",
    "src/fw_sim.c",
    "test/test_fw.c",
    "test/cli_hooks.sh",
    "policy/claims.txt",
    "Makefile",
    "README.md",
    "tools/gen_manifest.py",
]
HOOKS = [
    "hush_active",
    "hush_remaining_s",
    "hush_remain_s",
    "hush_reject",
    "hush_reject_reason",
    "alarm_relay_tx",
    "alarm_relay_rx",
    "relay_enable",
    "HUSH_SMOKE_S",
    "ux_light_set",
    "ux_light_get_status",
]


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def build_tar_bytes() -> bytes:
    raw = io.BytesIO()
    with tarfile.open(fileobj=raw, mode="w", format=tarfile.USTAR_FORMAT) as tar:
        for rel in sorted(SOURCES):
            path = ROOT / rel
            info = tar.gettarinfo(str(path), arcname=f"{VERSION}/{rel}")
            info.uid = 0
            info.gid = 0
            info.uname = ""
            info.gname = ""
            info.mtime = MTIME
            with path.open("rb") as handle:
                tar.addfile(info, handle)
    gzbuf = io.BytesIO()
    with gzip.GzipFile(filename="", mode="wb", fileobj=gzbuf, mtime=MTIME) as gz:
        gz.write(raw.getvalue())
    return gzbuf.getvalue()


def source_artifacts() -> list[dict[str, str]]:
    rows = []
    for rel in SOURCES:
        path = ROOT / rel
        if not path.is_file():
            raise SystemExit(f"missing source {rel}")
        rows.append({"path": rel, "sha256": sha256_file(path)})
    return rows


def optional_binary(rel: str) -> dict[str, str] | None:
    path = ROOT / rel
    if not path.is_file():
        return None
    return {"path": rel, "sha256": sha256_file(path), "note": "host build, not an nRF image"}


def load_manifest() -> dict:
    path = ROOT / "MANIFEST.json"
    if not path.is_file():
        raise SystemExit("MANIFEST.json missing")
    return json.loads(path.read_text(encoding="utf-8"))


def check() -> None:
    manifest = load_manifest()
    listed = {row["path"]: row["sha256"] for row in manifest["artifacts"] if row["path"] in SOURCES}
    for row in source_artifacts():
        if listed.get(row["path"]) != row["sha256"]:
            raise SystemExit(f"hash mismatch {row['path']}")
    tar_bytes = build_tar_bytes()
    tar_path = ROOT / PRIMARY_NAME
    if not tar_path.is_file():
        raise SystemExit(f"missing {PRIMARY_NAME}")
    committed = tar_path.read_bytes()
    if committed != tar_bytes:
        raise SystemExit("source tarball bytes differ from a fresh pack of the sources")
    digest = sha256_bytes(tar_bytes)
    if manifest.get("primary_sha256") != digest:
        raise SystemExit("primary_sha256 does not match the source tarball")
    if manifest.get("version") != VERSION or manifest.get("icd_baseline") != "v0.1.7":
        raise SystemExit("manifest version or ICD baseline drifted")
    print(f"OK {PRIMARY_NAME} {digest}")


def write() -> None:
    tar_bytes = build_tar_bytes()
    tar_path = ROOT / PRIMARY_NAME
    tar_path.parent.mkdir(parents=True, exist_ok=True)
    tar_path.write_bytes(tar_bytes)
    artifacts = source_artifacts()
    artifacts.append({"path": PRIMARY_NAME, "sha256": sha256_bytes(tar_bytes)})
    for rel in ("build/fw_sim", "build/test_fw"):
        row = optional_binary(rel)
        if row is not None:
            artifacts.append(row)
    manifest = {
        "version": VERSION,
        "icd_baseline": "v0.1.7",
        "image": "host-sim",
        "mcu_target": "nRF54L15",
        "ncs_board_build": False,
        "build_time_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "primary_artifact": PRIMARY_NAME,
        "primary_sha256": sha256_bytes(tar_bytes),
        "hush_smoke_s": 540,
        "test_hooks": HOOKS,
        "artifacts": artifacts,
        "notes": [
            "primary_sha256 identifies the portable source package.",
            "build/fw_sim sha256 is the linux host binary from this build and is not a flash image.",
            "Current numbers inside the sim are design-assumption clamps, not lab measurements.",
        ],
    }
    (ROOT / "MANIFEST.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"primary_artifact": PRIMARY_NAME, "primary_sha256": manifest["primary_sha256"]}, indent=2))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if args.check:
        check()
    else:
        write()


if __name__ == "__main__":
    main()
