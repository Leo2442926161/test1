# nRF54L15 NCS 骨架（不是烧录镜像）

给 [Issue #15](https://github.com/Leo2442926161/test1/issues/15) 的板级工程。  
行为源已经列进 `CMakeLists.txt`，钩子 JSON 有一条还没接到样机的路径。本目录 **没有** `.hex` / `.elf`。`west build` 在这台机器上没跑过。

| 项 | 现在 |
|---|---|
| `firmware/MANIFEST.json` 的 `image` | `host-sim`（本目录不改这个文件） |
| `ncs_board_build` | **false** |
| `primary_sha256` | 仍是宿主源码包 `8e0aad9b…70ea3d`。钩子会话不覆盖它 |
| 本机是否跑过 `west build` | **否**。2026-11-01：无 `west`、无 `arm-zephyr-eabi-gcc`、无 Zephyr SDK。只装 Python 包 `west` 不够，本轮没有克隆 NCS |
| 可烧录 EVT 镜像 | **没有**。`VERSION` 的 `EXTRAVERSION=hooks-unflashed` 只标记「钩子接上了、没有烧录文件」。DK 冒烟目标即使将来编过，也不是样机镜像 |
| 脚号 | 草案五处仍在 `boards/evt_pins_placeholder.overlay`（不自动套用）。`app.overlay` 无脚号。确认单：[HARDWARE_NEEDED.md](HARDWARE_NEEDED.md) |
| 钩子 | `hook_session` 调用 `fw_status_json`。RTT / UART 写函数返回 -1 |

宿主包 `fw-hush-relay-0.1.0-evt` 的说明仍在 [`firmware/README.md`](../README.md)。那份文件进了源码包哈希，本骨架不改它。那里第 7 点仍写「没有 west 工程」——指的是当时的宿主包。west 工程在本目录，且 **不** 进 `dist/*.tar.gz`。

行为基线是 ICD v0.1.7：

[`超级烟感_项目资料包_v0.1/02_工程/软件/超级烟感_软件硬件_ICD_v0.1.7.md`](../../02_工程/软件/超级烟感_软件硬件_ICD_v0.1.7.md)

索引：[`超级烟感_项目资料包_v0.1/02_工程/软件/README.md`](../../02_工程/软件/README.md)。

## 本目录是什么

| 文件 | 作用 |
|---|---|
| `west.yml` | 指向 nRF Connect SDK **v3.4.1**（`nrfconnect/sdk-nrf`，2026-09-17 的发行标签）。不把 SDK 放进本仓库 |
| `CMakeLists.txt` | 编进 `src/main.c`、`src/hook_session.c`、宿主 `fw_node.c` / `fw_ux.c` / `fw_status.c`。不编宿主 CLI |
| `prj.conf` `Kconfig` `VERSION` | DK 控制台打开。`CONFIG_USE_SEGGER_RTT` 与 `CONFIG_SETTINGS` 保持关闭。`CONFIG_CHAOJI_SMOKE_EVT_IMAGE` 打开会在 `main.c` 里 `#error` |
| `include/hook_session.h` `src/hook_session.c` | 单节点会话。上电 `fw_node_init`（`relay_enable` 默认 true）。JSON 不改写宿主里的 `image` 字符串 |
| `src/main.c` | 把空闲态 JSON 按 64 字节分片打到 `printk`，并调用两个仍会失败的写函数 |
| `hostcheck/` | 宿主 gcc 把会话和三份行为源链在一起。不是 `west build` |
| `app.overlay` | Zephyr 会自动带上。不写脚号，DK 冒烟不复用 EVT 草案脚 |
| `boards/evt_pins_placeholder.overlay` | **不会**被自动套用。只记四只灯脚 + `VBAT_SENSE` AIN2。无极性、无分压电阻 |

产品模组方向是 Raytac AN54LQ-15。板目标 `nrf54l15dk/nrf54l15/cpuapp` 是 Nordic DK（PCA10156）的 **工具链冒烟目标**，取自 [NCS 3.4.1 板级说明](https://docs.nordicsemi.com/bundle/ncs-3.4.1/page/nrf/app_dev/board_names.html)。DK 的引脚不是 EVT 网表。没有自定义板目标。草案脚号只在不被自动套用的 overlay 里，见 `HARDWARE_NEEDED.md`。

未做、本 Issue 的关闭条件也还没满足：

- 没有真实 `west build`，仓库里没有 `.hex` / `.elf`，`ncs_board_build` 仍是 false
- RTT 通道 1 没打开，`hook_rtt_write` 返回 -1
- `UART_DBG` 仍是 `PIN_TBD`，波特率未填（`HOOK_UART_BAUD_PLAN` 为 0），`hook_uart_write` 返回 -1
- `relay_enable` 只在 RAM。计划中的 settings 键是 `chaoji/relay_enable`，`CONFIG_SETTINGS` 没开，复位后回到默认 true
- 没有 1 Hz 线程、按键、烟/CO 驱动、灯脚、无线。`fw_tick` 不在 `main` 里调用
- 无 MCUboot、分区、Matter、Thread
- 不运行 `west flash`，不把 `build/` 里的产物提交回仓库
- `TC-CASCADE-PWR-01` 空表不填

## 钩子怎么出去

样机要回读的名字与 ICD v0.1.7 / 宿主 CLI 相同：`hush_active`、`hush_remaining_s`（别名 `hush_remain_s`）、`hush_reject`（别名 `hush_reject_reason`）、`alarm_relay_tx`、`alarm_relay_rx`、`relay_enable`。

| 步 | 谁 | 现在 |
|---|---|---|
| 1 | `hook_session_json` | 调用 `fw_status_json(node, NULL, …)`。对端是 null：板上没有第二节点，也没有射频 |
| 2 | `hook_session_frame` | 在这份 JSON 后加一个 `\n`。对象内部没有换行。这是将来 RTT/UART 的字节合同 |
| 3 | `hook_rtt_write` | 计划写到 RTT 通道 `HOOK_RTT_CHANNEL_PLAN`（1）。**返回 -1**。不碰通道 0 |
| 4 | `hook_uart_write` | 等 `UART_DBG` 离开 `PIN_TBD` 再绑。**返回 -1**。不把 Zephyr 控制台 UART 当成产测口 |
| 5 | `main` 的 `printk` | 活跃观测口，名字是 `console`。`HOOK_JSON_BEGIN` / 若干行 `HOOK_JSON ` / `HOOK_JSON_END`。把每行前缀去掉再拼回去，得到第 1 步的对象。这是调试分片，不是产测记录 |

`fw_status_json` 仍写出 `"image": "host-sim"` 和 `"ncs_board_build": false`。会话另有 `hook_session_image_claim()`，固定 `"none"`。烧录身份只按下面的 MANIFEST 规则另写，不在这条 JSON 里改字。

`relay_enable`：`hook_session_set_relay_enable` 只改节点内存，`hook_session_relay_store_name()` 为 `ram`，`hook_session_relay_persisted()` 为 false。上电走 `fw_node_init`，默认 true，对齐 `TC-NVS-RELAY-DEFAULT` 的默认，**不是**掉电保持。

宿主侧可以先证明这三份行为源和会话链得起来（不产生镜像）：

```sh
make -C 超级烟感_项目资料包_v0.1/firmware/nrf/hostcheck test
```

## 怎样 west build

需要本机已有 NCS v3.4.1 工具链（Toolchain Manager 或同等环境里的 `west` 与 Zephyr SDK）。

在含有本 `west.yml` 的分支上，于仓库外建工作区（会克隆 SDK，体积大）：

```sh
west init -m https://github.com/Leo2442926161/test1.git --mr <branch> --mf 超级烟感_项目资料包_v0.1/firmware/nrf/west.yml ncs-ws
cd ncs-ws
west update
west build -b nrf54l15dk/nrf54l15/cpuapp --no-sysbuild test1/超级烟感_项目资料包_v0.1/firmware/nrf
```

`<branch>` 用含 `超级烟感_项目资料包_v0.1/firmware/nrf/west.yml` 的分支。`self.path` 在 `west.yml` 里是 `test1`，所以应用路径是 `ncs-ws/test1/超级烟感_项目资料包_v0.1/firmware/nrf`。

已有 NCS v3.4.1 工作区时，不必再 `west init`。在该工作区里把 `ZEPHYR_BASE` 指到它的 `zephyr/`，然后：

```sh
west build -b nrf54l15dk/nrf54l15/cpuapp --no-sysbuild --build-dir /tmp/chaoji-smoke-dk-smoke /path/to/test1/超级烟感_项目资料包_v0.1/firmware/nrf
```

`--no-sysbuild` 若被当前 west 拒绝，去掉该参数再编。sysbuild 默认产物一般在 `build/<工程名>/zephyr/zephyr.hex` 与同目录的 `zephyr.elf`。这两种产物都只是 DK 冒烟文件：

- 不要 `west flash` 到 EVT 样机
- 不要拷回本仓库
- 不要把它的 SHA256 写进 `firmware/MANIFEST.json`
- 不要把 `ncs_board_build` 改成 true

冒烟成功只说明工具链能编过这个应用。钩子回读和 `TC-CASCADE-PWR-01` 仍然未做。换 EVT 板目标要等 `HARDWARE_NEEDED.md` 里的脚位确认，而不是在 DK 冒烟之后自动改。

## 计划中的 MANIFEST 字段（尚未写入）

`firmware/MANIFEST.json` 保持 `image=host-sim`、`ncs_board_build=false`。只有同时满足下面三条，才另写烧录身份（不要覆盖宿主源码包那一行 `primary_sha256`，除非当时的流程明确把主制品改成镜像）：

1. 同一次 `west build` 在本机或 CI 真实完成，产物是这次编译写出的 `.hex` 或 `.elf`
2. 板目标已换成 @硬件 确认过的 EVT 板，而不是 `nrf54l15dk/nrf54l15/cpuapp`
3. 镜像里能回读与 ICD v0.1.7 同语义的 `hush_*`、`alarm_relay_*`、`relay_enable`（RTT 或 UART 写函数返回 0，而不是现在的 -1）

届时计划字段：

```json
{
  "image": "nrf54l15-evt",
  "mcu_target": "nRF54L15",
  "ncs_board_build": true,
  "ncs_revision": "v3.4.1",
  "board_target": "<EVT board confirmed in HARDWARE_NEEDED.md>",
  "hooks_transport": "<uart or rtt>",
  "flash_artifact": "<path of the hex or elf this build wrote>",
  "flash_sha256": "<sha256 of that file>"
}
```

在那之前，这些键不出现在 `firmware/MANIFEST.json`。

## 宿主回归

`hostcheck` 不参与 `make -C 超级烟感_项目资料包_v0.1/firmware test` / `make verify`。改宿主 `超级烟感_项目资料包_v0.1/firmware/src`、`超级烟感_项目资料包_v0.1/firmware/README.md` 或 `超级烟感_项目资料包_v0.1/firmware/Makefile` 才会碰源码包哈希。本轮不改那些文件。
