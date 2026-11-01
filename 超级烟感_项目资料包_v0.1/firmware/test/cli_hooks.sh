#!/bin/sh
# CLI hook dump. Virtual time only; this script must not sleep for the hush window.
set -eu
cd "$(dirname "$0")/.."
SIM=./build/fw_sim
STATE=$(mktemp)
cleanup() { rm -f "$STATE"; }
trap cleanup EXIT

python3 - "$STATE" "$SIM" <<'PY'
import json, subprocess, sys
state, sim = sys.argv[1], sys.argv[2]

def call(*args):
    out = subprocess.check_output([sim, "--state", state, *args], text=True)
    return json.loads(out)

def need(cond, msg):
    if not cond:
        raise SystemExit("FAIL CLI " + msg)

call("reset")
smoke = call("smoke")
for key in (
    "hush_active", "hush_remaining_s", "hush_remain_s", "hush_reject",
    "hush_reject_reason", "alarm_relay_tx", "alarm_relay_rx", "relay_enable",
    "HUSH_SMOKE_S",
):
    need(key in smoke, "missing " + key)
need(smoke["HUSH_SMOKE_S"] == 540, "HUSH_SMOKE_S")
need(smoke["horn_on"] is True, "smoke horn")
need(smoke["relay_enable"] is True, "default relay")

muted = call("mute")
need(muted["hush_active"] is True, "hush_active")
need(muted["hush_remaining_s"] == 540 and muted["hush_remain_s"] == 540, "remain")
need(muted["horn_on"] is False and muted["sensing_active"] is True, "mute horn/sense")
need(muted["ux"]["mode"] == "mute_hold", "mutehold")

restored = call("tick", "540")
need(restored["hush_active"] is False and restored["horn_on"] is True, "auto restore")

call("reset")
call("co")
rejected = call("mute")
need(rejected["hush_active"] is False, "co hush")
need(rejected["hush_reject"] == "co_forbidden", "co reason")
need(rejected["hush_reject_reason"] == "co_forbidden", "co alias")
need(rejected["horn_on"] is True, "co horn stays")

call("reset")
call("pair")
call("smoke")
call("hop")
multi = call("mute")
need(multi["hush_active"] is True and multi["horn_on"] is False, "origin hush")
need(multi["peer"]["horn_on"] is True, "peer still alarming")
need(multi["peer"]["alarm_type"] == "smoke", "peer type")
need(multi["peer"]["silenced_by_remote_hush"] is False, "peer not suppressed")
need(multi["peer"]["alarm_relay_rx"] >= 1, "peer rx")

call("reset")
call("pair")
call("relay-enable", "0")
off = call("smoke")
call("hop")
off = call("status")
need(off["relay_enable"] is False, "relay off")
need(off["alarm_relay_tx"] == 0, "no tx")
need(off["horn_on"] is True, "local horn")
need(off["peer"]["alarm_type"] == "none", "no follow")

call("reset")
clamped = call("ux", "path_guide", "16", "50", "16", "0")
need(clamped["ux"]["limit_action"] == "clamp", "l1 action")
need(clamped["ux"]["applied_window"] <= 8, "l1 window")
need(clamped["ux"]["applied_brightness"] <= 25, "l2 brightness")

print("PASS CLI hooks")
PY
