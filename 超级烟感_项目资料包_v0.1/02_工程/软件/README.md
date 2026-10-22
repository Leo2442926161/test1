# 超级烟感 · 软件索引 v0.1

**日期**：2026-10-22（Asia/Shanghai）  
**本目录**：软件设计包（角色与工作流、索引、详细设计、缺口清单、ICD v0.1.7、FSM v0.3、语音短句 v0.1.1）。  
**可执行代码**：仓库顶层 [`firmware/src/`](../../../firmware/src)，包名 `fw-hush-relay-0.1.0-evt`。  
**实机**：nRF54L15 烧录镜像与 EVT 电流样归 [Issue #15](https://github.com/Leo2442926161/test1/issues/15)。本目录不放 `.hex` / `.elf`，也不放实验室通过证书。

`firmware/README.md` 已写明：这是对齐 ICD v0.1.7 的宿主 C 行为模型，不是 nRF54L15 烧录镜像，也没有链接 nRF Connect SDK。源码包身份以 `firmware/MANIFEST.json` 的 `primary_sha256` 为准（当前 `8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d`）。G3 钩子验收单核对的是这一包，本索引不改那些字节。

## 从这里去哪

| 要看的东西 | 路径 |
|---|---|
| 角色与工作流 | [`超级烟感_软件_角色与工作流_v0.1.md`](超级烟感_软件_角色与工作流_v0.1.md) |
| 本目录详细设计 | [`软件详细设计_v0.1.md`](软件详细设计_v0.1.md) |
| 本目录缺口（待补 + Issue） | [`目录与缺口清单_软件_v0.1.md`](目录与缺口清单_软件_v0.1.md) |
| ICD 现行版 v0.1.7 | [`超级烟感_软件硬件_ICD_v0.1.7.md`](超级烟感_软件硬件_ICD_v0.1.7.md) |
| 路径光 / 语音 FSM v0.3 | [`超级烟感_路径光语音_FSM短句草案_v0.3.md`](超级烟感_路径光语音_FSM短句草案_v0.3.md) |
| 语音短句 v0.1.1（文案，无播放器） | [`超级烟感_语音短句_v0.1.1.md`](超级烟感_语音短句_v0.1.1.md) |
| 固件包说明 | [`../../../firmware/README.md`](../../../firmware/README.md) |
| NCS 骨架（无 `.hex` / `.elf`） | [`../../../firmware/nrf/README.md`](../../../firmware/nrf/README.md) · [`HARDWARE_NEEDED.md`](../../../firmware/nrf/HARDWARE_NEEDED.md)。`ncs_board_build` 仍为 false |
| 固件源码 | [`../../../firmware/src/`](../../../firmware/src)（`fw_node.c` `fw_ux.c` `fw_status.c` `fw_sim.c`） |
| 公共头文件 | [`../../../firmware/include/fw.h`](../../../firmware/include/fw.h) |
| 文案策略锁 | [`../../../firmware/policy/claims.txt`](../../../firmware/policy/claims.txt) |
| 源码包 | [`../../../firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz`](../../../firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz) |
| G3 钩子验收（宿主 sim，有条件通过） | [`../../04_测试/验收单/超级烟感_G3钩子验收_fw-hush-relay-0.1.0-evt_v0.1.md`](../../04_测试/验收单/超级烟感_G3钩子验收_fw-hush-relay-0.1.0-evt_v0.1.md) |
| Hush / 级联可复现测项 v0.1.2 | [`../../04_测试/测项/超级烟感_Hush与级联_可复现测项_v0.1.2.md`](../../04_测试/测项/超级烟感_Hush与级联_可复现测项_v0.1.2.md) |
| SED 超限步骤 v0.1.1 | [`../../04_测试/测项/超级烟感_SED超限额拒绝降级_可复现步骤_v0.1.1.md`](../../04_测试/测项/超级烟感_SED超限额拒绝降级_可复现步骤_v0.1.1.md) |
| 宿主测试 | `make -C firmware test`（CI：[`.github/workflows/firmware-host-test.yml`](../../../.github/workflows/firmware-host-test.yml)） |
| 已关闭的钩子包 Issue | [#3](https://github.com/Leo2442926161/test1/issues/3) · [PR #14](https://github.com/Leo2442926161/test1/pull/14) |
| 未关闭的实机 Issue | [#15](https://github.com/Leo2442926161/test1/issues/15) |
| 本目录整编 Issue | [#19](https://github.com/Leo2442926161/test1/issues/19) |

G3 验收单在 `04_测试/验收单/`。这里只做链接，不复制正文，也不把它写成 UL/EN 或 EVT 电流通过记录。单上的结论是宿主钩子 **有条件通过**（28 pass / 0 fail / 1 skip）。Skip 项是 `TC-CASCADE-PWR-01`。

## 真实代码与待补

| 位置 | 状态 |
|---|---|
| `firmware/include/fw.h`、`firmware/src/fw_node.c`、`fw_ux.c`、`fw_status.c` | **已实现**。Hush、离线级联计数、SED L1–L9 钳位、Matter 通知计数。可在 Linux 上编译。 |
| `firmware/src/fw_sim.c`、`firmware/test/` | **已实现，仅宿主**。CLI 与 `test_fw`。不进芯片。 |
| `firmware/policy/claims.txt` | **已实现**。锁 `matter_role=notify_only`，并锁掉有线互联 / UL·EN 多站互联声称。 |
| `firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz` | **已实现**。可移植源码包。`build/fw_sim` 的 SHA 随编译器变化，不是烧录哈希。 |
| nRF Connect SDK 骨架 | **骨架在** [`firmware/nrf/`](../../../firmware/nrf)。`west.yml` 指向 NCS v3.4.1。无 `.hex` / `.elf`，未跑 `west build`，`ncs_board_build=false`。脚号未写入 overlay。负责人：软件。跟踪 [#15](https://github.com/Leo2442926161/test1/issues/15)。 |
| Matter 协议栈、手机 App、OTA / MCUboot | **待补**。负责人：软件。见缺口清单。 |
| EVT 电流 / Vbat 样 | **待补**。宿主套件对 `TC-CASCADE-PWR-01` 打印 SKIP，不填毫安读数。负责人：软件交样、测试出通过/退回单。跟踪 [#15](https://github.com/Leo2442926161/test1/issues/15)。 |

## 并行目录（本索引不写入）

下列路径由对应岗位维护。软件 PR 不在里面放原理图、STEP、Gerber 或实机报告。

| 岗位 | 约定路径 |
|---|---|
| 硬件 | `超级烟感_项目资料包_v0.1/02_工程/硬件设计/` |
| 结构 | `超级烟感_项目资料包_v0.1/02_工程/结构/` |
| 测试 | `超级烟感_项目资料包_v0.1/04_测试/` |

## `02_工程/` 根上剩余平铺稿（OUT OF SCOPE）

软件现行稿只留在本目录：ICD **v0.1.7**、FSM **v0.3**、语音短句 **v0.1.1**。根上的 ICD v0.1.3、v0.1.4、v0.1.4a、v0.1.5、v0.1.6 和 FSM v0.1、v0.2 已删除，没有第二份，也没有 `_archive`。

硬件扁平稿已由硬件岗收进 `02_工程/硬件设计/`。测试扁平稿已由测试岗收进 `04_测试/`。结构根上六份 `超级烟感_结构_*` 转发已删，正文只在 `02_工程/结构/`。本索引不改这些目录，也不重建转发。

| 岗位 | 状态 |
|---|---|
| 结构 | 根上六份已删。正文只在 `02_工程/结构/` |
| 认证 | 根上重复认证清单已删。现行主稿在 [`02_工程/硬件设计/电源射频/超级烟感_UL_EN认证流_标准与资料清单_v0.1.md`](../硬件设计/电源射频/超级烟感_UL_EN认证流_标准与资料清单_v0.1.md)。归硬件，软件不改 |
| 交付物缺口 | `超级烟感_交付物缺口清单_v0.1.md` 仍在根上。测试/DQE 盘点，软件不改 |
| 产品 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` 仍在根上 |

## 宿主构建

在仓库根目录：

```sh
make -C firmware test
make -C firmware verify
```

`test` 编译并运行 `firmware/test/test_fw.c` 与 `firmware/test/cli_hooks.sh`。现行宿主结果是 **pass=28 fail=0 skip=1**。`verify` 核对源码包字节，不烧录。
