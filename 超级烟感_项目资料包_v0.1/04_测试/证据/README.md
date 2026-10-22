# 证据

本目录收 **原始日志、SHA256、录屏索引**。结论写在 `../验收单/` 与 `../报告/`，不在这里改写。

入库约定：

- 日志：原文件名 + 对应 TC / 命令，不贴截图式改写。
- 哈希：对制品或日志做 `sha256sum`，一行一个文件。
- 录屏：文件名、时长、覆盖的 TC；视频本体可另存，这里留索引。

尚无实机录屏。宿主 sim 原始日志当时在验收工作区 `/workspace/g3-logs/`，**本提交未收入日志正文**。可核对的索引如下（数字均摘自验收单，不是新测）：

| 证据 | 值 | 出处 |
|---|---|---|
| 制品 | `fw-hush-relay-0.1.0-evt` | 验收单文首 |
| 源码包 | `firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz` | 验收单 §1 |
| SHA256 | `8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d` | 验收单 §1，期望=实测 |
| `make test` | `01-make-test.log`；pass=28 fail=0 skip=1 | 验收单 §2 |
| CLI / 场景 | `02-cli-reset.json` … `15-relay-default.json` | 验收单 §3–§4 |
