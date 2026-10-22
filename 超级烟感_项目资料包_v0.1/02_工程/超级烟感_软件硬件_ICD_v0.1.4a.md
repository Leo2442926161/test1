# 超级烟感 · 软件-硬件 ICD v0.1.4a

**相对 v0.1.3**：写入 Leo 补强——白壳主 SKU；光圈 **RGB + 64 分段可寻址**；光效 FSM 含自研旋转/环扫（FSM v0.3）。**v0.1.4a**：灯驱默认改 **可寻址 RGB（单线/SPI）**，非纯 I2C 单色；环扫周期 1.0–1.5 s。绝对 Pin 等网表 v0.1.2。

**基线**  
主控 nRF54L15（差分 nRF52840 / MG24）。CO：TGS5141 主、NAP-508 备（电化学，**无加热丝**）。光学 EVT 双轨：ADPD-I2C / TI 分立 ADC。CR123A×2。Matter=状态/事件；本地声光优先。

## 已冻配置
| 项 | 值 |
|---|---|
| 壳体 | 浅色薄盘（白）主 SKU |
| 光圈 | **64 分段**可寻址；RGB 全彩；旋转靠时序，**不加 96** |
| 灯驱 | **可寻址 RGB×64**（单线/SPI 默认；I2C RGB 多片备）；`LED_STATUS` **直驱** |
| 确认窗 | 默认 20s，工程 10–40s，可回读；对外不写秒 |
| 灵敏度 | Sensitive / Default出厂 / Quiet |
| N-03 | n=10；未验完禁 zero/never |
| 短句 | v0.1.1 锁定 |
| 文案禁区 | 禁 Fewer false alarms、优于 Nest、Matter=消防互联 |

## 信号 ICD（逻辑名）
| 信号 | 方向 | 接口 | 软件用途 | 备注 |
|---|---|---|---|---|
| OPT_I2C_SDA/SCL | 双向 | TWIM/I2C 3.3V | ADPD 性能轨 | 与分立互斥装配 |
| OPT_INT | IN | GPIO 下降沿 | 光学就绪/故障 | 判决后仍走本地 FSM |
| OPT_DISC_ADC | IN | ADC | TI 分立成本轨 | 互斥装配 |
| OPT_RAIL_SEL | IN | GPIO（0Ω/电阻） | 启动识别光路轨 | 固件读轨选后端 |
| CO_SIG | IN | **模拟** TIA→ADC | TGS5141 浓度/剂量 | NAP-508 同形态 |
| CO_BIAS_EN | OUT | GPIO | 偏置运放供电/使能 | 原 CO_HEAT 作废 |
| HORN_DRV | OUT | 高驱 GPIO（可经 MOS） | 本地蜂鸣 P0 | **不得**与 Thread WAKE 共用 |
| LED_STATUS | OUT | GPIO/PWM **直驱** | 中心状态点 | 禁与 RF_WAKE 复用 |
| LED_RGB_DATA | OUT | 单线/SPI MOSI | 可寻址 RGB 数据 | 默认轨；禁与 HORN/RF_WAKE 复用 |
| LED_RGB_CLK | OUT | SPI SCK（若 SPI） | 时钟 | 单线方案 DNP |
| LED_RGB_EN | OUT | GPIO | 灯串供电/使能 | 可选；Idle 可关电省电 |
| BTN_TEST_MUTE | IN | GPIO 上拉+消抖 | SelfTest/消音 | 安装态单手可达 |
| VBAT_SENSE | IN | 独立 ADC | BatteryAlert | 阈值待测 |
| COVER_DET | IN | 微动/霍尔 GPIO | 仓盖检测 | **必选** |
| HW_FAULT_SNS | IN | GPIO/ADC | HardwareFault | EVT 可先 NC |
| CONTAM_TP | 测点 | 光电窗+工装口 | Contamination | 对齐结构 |
| SWD/UART_DBG | 调试 | — | 产测日志 | 量产可关 |
| RF_RESET/WAKE | OUT/IN | 模组 | Thread | 净空 ≥5mm；**勿与 HORN/LED 复用** |

## 光效 FSM 摘要（详 FSM v0.3）
| 模式 | 用途 | 优先级 |
|---|---|---|
| RingSweep_Night | 夜间路径单向环扫（冷白低占空）；周期 **1.0–1.5 s** | P2 |
| Spin_Short | 测试/找设备短旋 | P2 |
| Pulse_Smoke / Pulse_CO / Pulse_Fault | 告警色+脉动（可带慢旋强调） | P0 / P0 / P1 |
| ClearSweep / MuteHold / Idle | 清除收束 / 静音窗 / 待机 | — |

禁：Nest 大环面色块扫场、绿闪 Nightly 节奏。白壳亮度标定一版（不加通道）。

## 三条锁定测法 · 探针表
| 测法 | 刺激/条件 | 探针 | 通过判据 |
|---|---|---|---|
| 硬路径 ≤50ms | 烟/CO 刺激 | `HORN_DRV`/`LED_*` + 刺激时间戳 | 端到端 ≤50ms；不经 Thread |
| 断网完整声光 | 断 Thread/Matter 后刺激 | 同上 + 网络态 | 声光完整；上报可丢 |
| OTA Alarm 拒升 | Alarm 态下发 Apply | OTA 状态/分区 | 拒绝切槽；声光不中断 |
| 开盖不断电 | 开仓盖 | `COVER_DET` | 检测有效；供电不断 |
| 光路轨识别 | 上电 | `OPT_RAIL_SEL` | 正确绑定后端 |
| CO 通路 | 偏置+气体/模拟 | `CO_BIAS_EN` + `CO_SIG` | 剂量路径可复现 |
| 64 段旋转可区分 | Path / Test / 三告警 | 灯驱帧 + 目视/工装 | 环扫/短旋/三色脉动盲测可分；非 Nest 绿闪 |

## 路径光 / 语音 / 互联（Nest IP 工程规避 · 非法律意见）
1. 路径光：细环 64 段 + 中心点 + 底边线型；自研旋转/环扫时序；禁大环面与 Nightly 绿闪照搬。
2. 语音：短句 v0.1.1；禁 Heads-Up /「烟雾在×房间」句式。
3. 互联：Matter 仅状态/通知。
4. 体验：应达目标，不上第四卖点。

## Pin 草案（网表前可改）
沿用 v0.1.3 AN54LQ-15 草案；**新增/调整**：
| 逻辑名 | SoC Pin（草案） | 电气 | 备注 |
|---|---|---|---|
| LED_STATUS | P0.01 | GPIO/PWM | 直驱中心点 |
| LED_RGB_DATA | P2.06（草案） | 单线/SPI | 可寻址协议；网表 v0.1.2 回写 |
| LED_RGB_CLK | P2.07（草案） | SPI SCK | 单线方案 DNP |
| LED_RING 位映射 | — | ch0–63 RGB | 软件地址表；结构开孔对齐 |

其余脚同 v0.1.3。绝对 Pin 以原理图网表为准。

## 硬路径与时序
- 判决 → HORN/LED：**≤50ms**；P0 可先刷缓存帧/全局亮度，忌阻塞长串刷新
- Matter 镜像异步；边沿队列 8～16
- SED/占空比限额：等硬件寿命表（可寻址 RGB+CR123A）冻后回写；Idle 低占空不变

## OTA / 故障 / SoC 差分
同 v0.1.3。Alarm/Testing 拒 Apply；低电禁 OTA。

## 互检状态
| 角色 | 结论 |
|---|---|
| 市场产品 | 白壳+64+短句锁 — 过；体验规格 v0.3 §5 |
| 测试 | 安全五条不变；增 64 段旋转/三色可区分测项 |
| 结构 | 白盘+64 细环开孔草排 |
| 硬件 | 网表 v0.1.2 改可寻址 RGB 总线脚；寿命表重做 |
| 软件 | FSM v0.3.1 / ICD v0.1.4a：可寻址协议 + 1.0–1.5 s 环扫 |

## 风险与工期
硬路径 1.5–2 周；光效旋转表驱动 +2–3 日；光学双后端 +0.5 周；MVP 骨架约 3–4 周。  
风险：可寻址 RGB 峰值与 SED/换电寿命；P0 快路径 + Idle 关电。BOM 增须守 **$32**。不下单。
