# nRF54L15 NCS 骨架（不是烧录镜像）

给 [Issue #15](https://github.com/Leo2442926161/test1/issues/15) 的板级工程空壳。  
`west build` 的步骤写在下面。本目录 **没有** `.hex` / `.elf`，也没有把宿主 sim 写成芯片镜像。

| 项 | 现在 |
|---|---|
| `firmware/MANIFEST.json` 的 `image` | `host-sim`（本骨架不改这个文件） |
| `ncs_board_build` | **false** |
| 本机是否跑过 `west build` | **否**。2026-10-22 检查：无 `west`、无 `arm-zephyr-eabi-gcc`、无 Zephyr SDK |
| 可烧录 EVT 镜像 | **没有**。DK 冒烟目标即使将来编过，也不是样机镜像 |
| 脚号 | 草案五处在 `boards/evt_pins_placeholder.overlay`（不自动套用）。`app.overlay` 无脚号。确认单：[HARDWARE_NEEDED.md](HARDWARE_NEEDED.md) |

宿主包 `fw-hush-relay-0.1.0-evt` 的说明仍在 [`firmware/README.md`](../README.md)。那份文件进了源码包哈希，本骨架不改它。那里第 7 点仍写「没有 west 工程」——指的是当时的宿主包。west 工程在本目录，且 **不** 进 `dist/*.tar.gz`。

行为基线是 ICD v0.1.7，文件在软件目录（PR #30 之后，不在 `02_工程/` 根上）：

[`超级烟感_项目资料包_v0.1/02_工程/软件/超级烟感_软件硬件_ICD_v0.1.7.md`](../../超级烟感_项目资料包_v0.1/02_工程/软件/超级烟感_软件硬件_ICD_v0.1.7.md)

索引：[`超级烟感_项目资料包_v0.1/02_工程/软件/README.md`](../../超级烟感_项目资料包_v0.1/02_工程/软件/README.md)。

## 本目录是什么

| 文件 | 作用 |
|---|---|
| `west.yml` | 指向 nRF Connect SDK **v3.4.1**（`nrfconnect/sdk-nrf`，2026-09-17 的发行标签）。不把 SDK 放进本仓库 |
| `CMakeLists.txt` `prj.conf` `Kconfig` `VERSION` `src/main.c` | 可被 `west build` 指向的应用。`VERSION` 的 `EXTRAVERSION=skeleton` 只标识这个空壳，不是 `fw-hush-relay-0.1.0-evt` |
| `app.overlay` | Zephyr 会自动带上。不写脚号，DK 冒烟不复用 EVT 草案脚 |
| `boards/evt_pins_placeholder.overlay` | **不会**被自动套用。只记四只灯脚 + `VBAT_SENSE` AIN2。无极性、无分压电阻 |
| `src/main.c` | 打印三行说明后返回。不调用 `fw_*`，不打钩子 JSON |

`CONFIG_CHAOJI_SMOKE_EVT_IMAGE` 默认关闭。打开它会在编译期 `#error`，避免把空壳说成 EVT 镜像。

产品模组方向是 Raytac AN54LQ-15。下面的板目标 `nrf54l15dk/nrf54l15/cpuapp` 是 Nordic DK（PCA10156）的 **工具链冒烟目标**，取自 [NCS 3.4.1 板级说明](https://docs.nordicsemi.com/bundle/ncs-3.4.1/page/nrf/app_dev/board_names.html)。DK 的引脚不是 EVT 网表。没有自定义板目标。草案脚号只在不被自动套用的 overlay 里，见 `HARDWARE_NEEDED.md`。 |

未做、本 Issue 的关闭条件也还没满足：

- 不链接 `firmware/src/fw_node.c`、`fw_ux.c`、`fw_status.c`（`fw_sim.c` 永远不进镜像）
- 不把 `fw_status_json` 打到 UART 或 RTT
- 不保存 `relay_enable`（无 settings / NVS）
- 无 MCUboot、分区、Matter、Thread
- 不运行 `west flash`，不把 `build/` 里的产物提交回仓库

## 怎样 west build

需要本机已有 NCS v3.4.1 工具链（Toolchain Manager 或同等环境里的 `west` 与 Zephyr SDK）。只装 Python 包 `west` 不够。

在含有本 `west.yml` 的分支上，于仓库外建工作区（会克隆 SDK，体积大）：

```sh
west init -m https://github.com/Leo2442926161/test1.git --mr <branch> --mf firmware/nrf/west.yml ncs-ws
cd ncs-ws
west update
west build -b nrf54l15dk/nrf54l15/cpuapp --no-sysbuild test1/firmware/nrf
```

`<branch>` 用含 `firmware/nrf/west.yml` 的分支。`self.path` 在 `west.yml` 里是 `test1`，所以应用路径是 `ncs-ws/test1/firmware/nrf`。

已有 NCS v3.4.1 工作区时，不必再 `west init`。在该工作区里把 `ZEPHYR_BASE` 指到它的 `zephyr/`，然后：

```sh
west build -b nrf54l15dk/nrf54l15/cpuapp --no-sysbuild --build-dir /tmp/chaoji-smoke-dk-smoke /path/to/test1/firmware/nrf
```

`--no-sysbuild` 若被当前 west 拒绝，去掉该参数再编。sysbuild 默认产物一般在 `build/<工程名>/zephyr/zephyr.hex` 与同目录的 `zephyr.elf`。这两种产物都只是 DK 冒烟文件：

- 不要 `west flash` 到 EVT 样机
- 不要拷回本仓库
- 不要把它的 SHA256 写进 `firmware/MANIFEST.json`
- 不要把 `ncs_board_build` 改成 true

冒烟成功只说明工具链能编过这个空壳。钩子回读和 `TC-CASCADE-PWR-01` 仍然未做。

## 计划中的 MANIFEST 字段（尚未写入）

`firmware/MANIFEST.json` 保持 `image=host-sim`、`ncs_board_build=false`。只有同时满足下面三条，才另写烧录身份（不要覆盖宿主源码包那一行 `primary_sha256`，除非当时的流程明确把主制品改成镜像）：

1. 同一次 `west build` 在本机或 CI 真实完成，产物是这次编译写出的 `.hex` 或 `.elf`
2. 板目标已换成 @硬件 确认过的 EVT 板，而不是 `nrf54l15dk/nrf54l15/cpuapp`
3. 镜像里能回读与 ICD v0.1.7 同语义的 `hush_*`、`alarm_relay_*`、`relay_enable`

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

本目录不参与 `make -C firmware test` / `make verify`。改宿主 `firmware/src`、`firmware/README.md` 或 `firmware/Makefile` 才会碰源码包哈希。
