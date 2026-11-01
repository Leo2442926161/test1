# 超级烟感 EVT 固件可测包 `fw-hush-relay-0.1.0-evt`

宿主可编译的 C 行为模型，对齐 ICD **v0.1.7** 的 Hush、离线级联和 SED L1–L9 钩子。  
这不是 nRF54L15 烧录镜像，也没有链接 nRF Connect SDK。本机没有板级 `west flash` 环境时，G3 用下面的命令验证状态机。

## 路径

| 项 | 位置 |
|---|---|
| 本包 | 仓库顶层 `firmware/` |
| 资料包 | PR #1 的 `超级烟感_项目资料包_v0.1/`（本 PR 不合并它） |
| 日后板级工程 | 仍可把 `firmware/src` 挪到 `超级烟感_项目资料包_v0.1/02_工程/firmware/`，接口不变 |

选顶层 `firmware/` 是为了这一包能单独克隆、单独 `make test`。

## 构建与钩子

在 `firmware/` 目录：

```sh
make test
make            # build/fw_sim 与 build/test_fw
./build/fw_sim --state /tmp/dut.state reset
./build/fw_sim --state /tmp/dut.state smoke
./build/fw_sim --state /tmp/dut.state mute
./build/fw_sim --state /tmp/dut.state status
```

`status`（以及每条命令）向 stdout 打一行 JSON。测试要读的字段：

| 钩子 | JSON |
|---|---|
| 是否在消音窗 | `hush_active` |
| 剩余秒 | `hush_remaining_s`，别名 `hush_remain_s` |
| 拒绝原因 | `hush_reject`，别名 `hush_reject_reason`（CO 为 `co_forbidden`） |
| 级联发送/接收计数 | `alarm_relay_tx` / `alarm_relay_rx` |
| 级联开关 | `relay_enable`（内存 NVS，默认 `true`） |
| 默认窗 | `HUSH_SMOKE_S` = **540**（常量）。可配 8–10 分钟见 `hush_cfg_s` |
| 灯限额 | `ux.applied_window` / `applied_brightness` / `applied_segments` / `led_rgb_en` / `limit_action` / `reject_reason` / `rf_half` / `sweep_elapsed_s` / `cooldown_remain_s` |

对应 ICD 的 `ux_light_set` / `ux_light_get_status`：C API 是 `fw_ux_set` / `fw_ux`，CLI 是 `ux` 与 `status`。

常用命令：`smoke` `co` `mute` `critical` `tick N` `relay-enable 0|1` `pair` `hop` `tx-fail 1` `cloud 0` `matter 0` `ux-tick N`。  
`tick` 是虚拟秒，不睡眠。9 分钟恢复是 `tick 540`，不是等 540 秒。  
模型拒绝（CO 消音、满幅白、冷却期）仍打印 JSON，退出码 0；命令写错才是非 0。

双机示例：

```sh
./build/fw_sim --state /tmp/dut.state reset
./build/fw_sim --state /tmp/dut.state pair
./build/fw_sim --state /tmp/dut.state smoke
./build/fw_sim --state /tmp/dut.state hop
./build/fw_sim --state /tmp/dut.state mute
./build/fw_sim --state /tmp/dut.state status
```

此时本机 `hush_active=true`、`horn_on=false`，`peer.horn_on` 仍为 true，`peer.silenced_by_remote_hush=false`。

## `make test` 覆盖

自动通过的是软件钩子，不是实验室证书：

- 烟 Hush、540 s 虚拟时间恢复、危急打断、CO `co_forbidden`、本机 Hush 不压他机
- 烟/CO 同类型跟报、断云仍级联、`relay_enable=0` 不跟、异组不跟、级联失败不挡本机声光
- Matter 仅通知（`matter_role=notify_only`，会话断开仍级联）
- SED L1–L8 与峰值模型钳位、L9（关级联 / 失败不挡本地 / TX 预算算进 600 mA 模型）

`TC-CASCADE-PWR-01` 打印 **SKIP**。没有 EVT 电流/Vbat 采样，本包不填写、也不宣称该项通过。  
`TC-MATTER-BOUND-02` 与 `TC-CLAIM-BOUND-01` 只锁本包 `policy/claims.txt` 和固件标志。PR #1 的 Listing/说明书抽检仍是人工项。

电流数字来自寿命表里的设计假设（满幅约 45 mA/段、蜂鸣按 120 mA、级联 TX 预算 40 mA），用来驱动钳位逻辑。它们不是钳表读数。L7 的 u8 百分比上限写成 **12**（规格 ≤12.5%，整数百分比放不下 0.5）。

## 版本与哈希

```sh
make manifest   # 重写 MANIFEST.json 与 dist/*.tar.gz
make verify     # 核对源文件与源码包字节
sha256sum dist/fw-hush-relay-0.1.0-evt-src.tar.gz
sha256sum build/fw_sim
```

主制品是 `dist/fw-hush-relay-0.1.0-evt-src.tar.gz`（可移植源码包）。`build/fw_sim` 的 SHA256 随编译器和平台变化，只标识这一次 Linux 宿主构建，不能当作 nRF 镜像哈希。

## 以后怎么落到 nRF54L15

1. 把 `include/fw.h` 和 `src/fw_*.c`（不含 `fw_sim.c`）编进 NCS 应用。
2. `BTN_TEST_MUTE` 短按调用 `fw_mute`；烟/CO 判决调用 `fw_set_alarm`；1 Hz 定时调用 `fw_tick(1)`。
3. `relay_enable` 改存 Zephyr settings，默认开。现在的 NVS 只是节点里的一个布尔量。
4. 级联帧只带报警类型和组号。不要带「把对方消音」位。`fw_mute` 不会发这种帧。
5. Matter 事件沿用现有通知计数。通知失败不得回退 `horn_on`，也不得代替 `alarm_relay_tx`。
6. 产测口把 `fw_status_json` 打到 UART/RTT，字段名与本 CLI 相同。
7. 本包没有 board overlay、分区或 `west` 工程。在 NCS 板级构建落地之前，不要把 `build/fw_sim` 写进芯片。

## 不做的事

- 不把 Matter 或无线跟报写成有线消防互联，也不声称 UL/EN multiple-station interconnect。
- 不实现 Nest 房间名或 Heads-Up 剧本。
- 不编造声压、毫安或认证通过记录。
