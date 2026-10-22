# 超级烟感 · G3 钩子验收报告 · 宿主 sim v0.1

**日期**：2026-10-22（Asia/Shanghai）  
**性质**：报告骨架。结论 **引用** 验收单，本文件不新跑、不改数字。  
**验收单**：[`../验收单/超级烟感_G3钩子验收_fw-hush-relay-0.1.0-evt_v0.1.md`](../验收单/超级烟感_G3钩子验收_fw-hush-relay-0.1.0-evt_v0.1.md)

本报告 **不是** 实机 G3 全过，也 **不是** UL/EN 实验室过线。实机空表见 [`EVT实机验收报告_待填_v0.1.md`](EVT实机验收报告_待填_v0.1.md)。

---

## 1. 结论（摘自验收单 §0、§2）

| 项 | 值 |
|---|---|
| 结论 | **有条件通过** |
| Fail | **0** |
| `make -C firmware test` | **pass 28 / fail 0 / skip 1** |
| 制品 | `fw-hush-relay-0.1.0-evt` |
| 源码包 | `firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz` |
| SHA256 | `8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d`（期望 = 实测，Pass） |
| Head（验收时） | `ba6f34ea36ffe125258aa11be010e8d240042bfd` |
| PR | https://github.com/Leo2442926161/test1/pull/14 |

计数 28/0/1 的 skip 为 `TC-CASCADE-PWR-01`（EVT 硬件 Vbat/电流；未跑、不声称结果）。

---

## 2. 条件（摘自验收单 §0，须同时成立）

| 条件 ID | 条件 | 验收单状态 |
|---|---|---|
| C1 | 范围锁定为 **宿主 sim**（`image=host-sim`，`ncs_board_build=false`）；**不是** nRF 烧录镜像 | 已声明并实测 |
| C2 | `TC-CASCADE-PWR-01`（EVT N 台同响 Vbat/电流）**SKIP**，无硬件实测声称 | 已 SKIP |
| C3 | Matter/Claim 文档抽检仍走资料包人工抽检；本包仅 **policy lock** 单测 | 已标明 |
| C4 | 本机构建 `build/fw_sim` SHA 与 PR 自述 Linux 二进制 SHA 可不一致（重编）；以 **源码包 SHA256** 为准 | 已核 |

---

## 3. 本报告不覆盖

- nRF54L15 烧录镜像与样机回读。
- 灯环 / 峰值墙电流、N 台同响压降。
- 声压、实验室限值、送检结论。

上述项保持空白，直到 [Issue #15](https://github.com/Leo2442926161/test1/issues/15) 关闭并另填 EVT 实机验收报告。
