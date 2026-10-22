# 超级烟感 · 仓库地图

住宅 Smoke+CO（SKU-EX）资料与宿主固件。资料在 `超级烟感_项目资料包_v0.1/`，代码在 `firmware/`。总索引：[超级烟感_项目资料包_v0.1/README.md](超级烟感_项目资料包_v0.1/README.md)。

## 产品锁定

- 美欧旗舰 **Smoke+CO**、可测抗误报、**Matter/Thread（仅状态与通知）**；同品牌无线级联。声光报警不可被用户永久关闭。
- 价带 **$89 主推 / $99 旗舰 / $79 促销底**。G2 有条件不过，主阻塞是 CO OEM 实价（[#8](https://github.com/Leo2442926161/test1/issues/8)）。
- 光学：量产默认 **TI 分立双波段**；ADPD 只做 EVT 备选。主控 **nRF54L15**。电源 **CR123A×2**。
- 白壳薄盘；**64 段 RGB** 自研路径光。Hush **仅烟、默认 9 min**；**CO 禁止静音**。
- 外形草排外径 **Ø135**；安装螺柱 **R=41.6×3**，坐标 **(41.6, 0) / (−20.8, 36.0) / (−20.8, −36.0)**。说明书 N-06 为 v0.1.2。

## 锁定目录

| 路径 | 现在 |
|---|---|
| [01_市场产品/](超级烟感_项目资料包_v0.1/01_市场产品/) | PRD、范围、Listing、认证主稿 |
| `02_工程/硬件设计/` | 硬件设计包。由 [PR #21](https://github.com/Leo2442926161/test1/pull/21) 归位，本索引不搬文件 |
| `02_工程/结构/` | 结构包。由 [#17](https://github.com/Leo2442926161/test1/issues/17) 归位，本索引不新建该目录 |
| [02_工程/软件/](超级烟感_项目资料包_v0.1/02_工程/软件/README.md) | 详设、索引、缺口（[#19](https://github.com/Leo2442926161/test1/issues/19) / [#22](https://github.com/Leo2442926161/test1/pull/22)，已在 main） |
| [04_测试/](超级烟感_项目资料包_v0.1/04_测试/README.md) | 测项、验收单、报告、证据（[#23](https://github.com/Leo2442926161/test1/pull/23)，已在 main） |
| [firmware/](firmware/README.md) | 宿主 sim `fw-hush-relay-0.1.0-evt`（`image=host-sim`） |

## 现行入口

| 主题 | 文件 |
|---|---|
| PRD | [PRD-v0.1_正式版.md](超级烟感_项目资料包_v0.1/01_市场产品/PRD-v0.1_正式版.md) |
| ICD v0.1.7 | [超级烟感_软件硬件_ICD_v0.1.7.md](超级烟感_项目资料包_v0.1/02_工程/超级烟感_软件硬件_ICD_v0.1.7.md) |
| Hush 测项 v0.1.2 | [04_测试/测项/](超级烟感_项目资料包_v0.1/04_测试/测项/超级烟感_Hush与级联_可复现测项_v0.1.2.md) |
| 网表 v0.1.2 / 坐标 v0.1.1 | 仍在 `02_工程/` 根上，归位目标是 `02_工程/硬件设计/`（PR #21） |
| 结构绝对坐标 + G3 套 | 仍在 `02_工程/超级烟感_结构_*.md`，归位目标是 `02_工程/结构/`（#17） |
| 固件 | [firmware/README.md](firmware/README.md) |

nRF54L15 烧录镜像与 EVT 电流样跟踪 [#15](https://github.com/Leo2442926161/test1/issues/15)。`firmware/` 里的 `build/fw_sim` 是宿主行为模型。

## 未关闭

| Issue | 事项 |
|---|---|
| [#6](https://github.com/Leo2442926161/test1/issues/6) | ID 定稿与效果图 ≥6（现仅 2 张概念 JPG） |
| [#8](https://github.com/Leo2442926161/test1/issues/8) | CO OEM 实价，G2 仍锁 |
| [#9](https://github.com/Leo2442926161/test1/issues/9) | 实验室 UL 217 / UL 2034 / EN 限值与排期 |
| [#12](https://github.com/Leo2442926161/test1/issues/12) | RoHS / REACH / UN38.3 |
| [#15](https://github.com/Leo2442926161/test1/issues/15) | nRF54L15 镜像与 EVT 电流样 |
| [#17](https://github.com/Leo2442926161/test1/issues/17) | 结构目录与 STEP |
| [#20](https://github.com/Leo2442926161/test1/issues/20) | 硬件设计包：原理图工程 / Gerber / 锁定 BOM（目录见 PR #21） |

[#19](https://github.com/Leo2442926161/test1/issues/19) 已关闭，软件索引在 `02_工程/软件/`。

```sh
make -C firmware test
```
