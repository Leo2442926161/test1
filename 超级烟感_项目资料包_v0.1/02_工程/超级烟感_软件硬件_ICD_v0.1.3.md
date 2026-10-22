# 超级烟感 · 软件-硬件 ICD v0.1.3

**相对 v0.1.2**：写入 Nest IP 工程规避（光/语音/互联）；非法律意见。

**基线**  
主控 nRF54L15（差分 nRF52840 / MG24）。CO：TGS5141 主、NAP-508 备（电化学，**无加热丝**）。光学 EVT 双轨：ADPD-I2C 性能 / TI 分立 ADC 成本（$89 量产默认意向）。CR123A×2。Matter=状态/事件；本地声光优先。

## 已冻配置（市场产品签字）
| 项 | 值 |
|---|---|
| 确认窗 | 默认 20s，工程 10–40s，**可回读**；对外不写秒 |
| 灵敏度 | Sensitive / Default出厂 / Quiet → `SmokeSensitivityLevel` |
| N-03 | n=10；未验完禁 zero/never |
| 文案禁区 | 禁 Fewer false alarms、优于 Nest、Matter=消防互联 |

## 信号 ICD（逻辑名）
| 信号 | 方向 | 接口 | 软件用途 | 备注 |
|---|---|---|---|---|
| OPT_I2C_SDA/SCL | 双向 | TWIM/I2C 3.3V | ADPD 性能轨 | 与分立互斥装配 |
| OPT_INT | IN | GPIO 下降沿 | 光学就绪/故障 | 判决后仍走本地 FSM |
| OPT_DISC_ADC | IN | ADC | TI 分立成本轨 | 互斥装配 |
| OPT_RAIL_SEL | IN | GPIO（0Ω/电阻） | 启动识别光路轨 | **v0.1.1 新增**；固件读轨选后端 |
| CO_SIG | IN | **模拟** TIA→ADC | TGS5141 浓度/剂量 | NAP-508 同形态 |
| CO_BIAS_EN | OUT | GPIO | 偏置运放供电/使能 | **原 CO_HEAT 作废**（无加热丝） |
| HORN_DRV | OUT | 高驱 GPIO（可经 MOS） | 本地蜂鸣 P0 | **不得**与 Thread WAKE 共用；不经 Thread 门控 |
| LED_STATUS/RING | OUT | 高驱 GPIO/PWM | 指示/光环 | 遮光防串腔；同上不得共 WAKE |
| BTN_TEST_MUTE | IN | GPIO 上拉+消抖 | SelfTest/消音 | 安装态单手可达 |
| VBAT_SENSE | IN | 独立 ADC | BatteryAlert | 阈值待测；与 CO/分立光分 ADC 通道优先 |
| COVER_DET | IN | 微动/霍尔 GPIO | 仓盖检测 | **必选**（非「若有」）；对齐开盖不断电 |
| HW_FAULT_SNS | IN | GPIO/ADC | HardwareFault | 可测点；EVT 可先 NC |
| CONTAM_TP | 测点 | 光电窗目视+工装口 | Contamination | 对齐结构 |
| SWD/UART_DBG | 调试 | — | 产测日志 | 量产可关 |
| RF_RESET/WAKE | OUT/IN | 模组 | Thread | 天线净空 ≥5mm；**勿与 HORN/LED 脚复用** |


## 三条锁定测法 · 探针表（测试认可）
| 测法 | 刺激/条件 | 探针 | 通过判据 |
|---|---|---|---|
| 硬路径 ≤50ms | 烟/CO 刺激 | `HORN_DRV`/`LED_*` GPIO + 刺激端时间戳 | 端到端 ≤50ms；不经 Thread |
| 断网完整声光 | 断 Thread/Matter 后刺激 | 同上 + 网络态 | 声光完整；上报可丢 |
| OTA Alarm 拒升 | Alarm 态下发 Apply | OTA 状态/分区 | 拒绝切槽；声光不中断 |
| 开盖不断电 | 开仓盖 | `COVER_DET` **必选** | 检测有效；供电不断（硬件） |
| 光路轨识别 | 上电 | `OPT_RAIL_SEL` | 正确绑定 ADPD 或分立后端 |
| CO 通路 | 偏置+气体/模拟注入 | `CO_BIAS_EN` + 模拟 `CO_SIG` | 剂量路径可复现 |

## 路径光 / 语音 / 互联（Nest IP 工程规避 · 非法律意见）
1. **路径光**：细环或离散状态点；禁 Nest 大环面光语与可识别「Nightly Promise」类绿闪节奏照搬。
2. **语音**：自研文案与状态机；禁复刻 Nest Heads-Up /「烟雾在×房间」类公开句式与交互剧本。
3. **互联**：Matter **仅**状态/通知；不做有线或专有语音位置互联卖点。
4. **体验**：路径光/语音=应达体验目标（不上第四卖点）；外观更好看≠外形抄 Nest。
软件实现自研 FSM + 文案表；对照 Nest 只保留内部事实笔记，不写进对外话术或固件默认文案。


## Pin 草案（Raytac AN54LQ-15 / nRF54L15）— 网表前可改
| 逻辑名 | SoC Pin | 电气 | 备注 |
|---|---|---|---|
| OPT_I2C_SCL | P2.01 | I2C TWIM | ADPD 轨 |
| OPT_I2C_SDA | P2.02 | I2C | 同上 |
| OPT_INT | P2.03 | GPIO IN | 下降沿唤醒 |
| OPT_DISC_ADC | P1.04 / AIN0 | ADC | TI 轨；与 ADPD 互斥 DNP |
| OPT_RAIL_SEL | P2.05 | GPIO IN | 上拉/0Ω 识轨 |
| CO_SIG | P1.05 / AIN1 | ADC | TIA 后模拟 |
| CO_BIAS_EN | P1.08 | GPIO OUT | 偏置运放使能 |
| VBAT_SENSE | P1.06 / AIN2 | ADC | 分压；阈值待测 |
| HORN_DRV | P0.00 | GPIO OUT 高驱 | →MOS→压电；禁 Thread 门控 |
| LED_STATUS | P0.01 | GPIO/PWM | 指示 |
| LED_RING | P2.06 | GPIO/PWM | 细环/状态点；遮光防串 |
| BTN_TEST_MUTE | P1.09 | GPIO IN 上拉 | 安装态可达 |
| COVER_DET | P1.10 | GPIO IN | 必选微动/霍尔 |
| HW_FAULT_SNS | P1.07 / AIN3 | ADC/GPIO | 可测点 |
| CONTAM_TP | TP | 工装 | 不占 GPIO |
| SWD* | 模组脚 | 调试 | 量产可关 |
| RF | 模组天线 | — | 净空≥5mm；无外挂 RF_RESET |

布线遇 RF/电源冲突可换同组 AIN/GPIO；**逻辑名与双轨互斥规则不变**。绝对 Pin 以原理图网表为准回写。

## 硬路径与时序
- 判决 → HORN/LED：**≤50ms**；独立 ISR/任务
- Matter 镜像异步；边沿队列 8～16
- 启动：读 `OPT_RAIL_SEL` 绑定 ADPD 或分立后端

## OTA（nRF54L15）
Boot + A/B + NVS + DAC。Alarm/Testing 拒 Apply；低电禁 OTA。

## 故障映射
光学失败→HardwareFault；污染→Contamination；低电→BatteryAlert；烟/CO→P0 + State/Events。

## SoC 差分
主实现 nRF54L15；52840/MG24 仅差分表。绝对球脚：**待原理图网表 v0.1.1 再冻**。

## 互检状态
| 角色 | 结论 |
|---|---|
| 市场产品 | 配置/文案禁区 — 过 |
| 测试 | N 用例接口 — 认可；Pin 后冻探针表；低电阈值待测 |
| 结构 | 净空/声腔/遮光/按键 — 过；COVER_DET 必选已采纳 |
| 硬件 | 逻辑对齐通过；AN54LQ-15 Pin 草案已并入；网表后再冻 |

## 风险与工期
硬路径 1.5–2 周；光学双后端 +0.5 周；MVP 骨架约 3–4 周。不下单。
