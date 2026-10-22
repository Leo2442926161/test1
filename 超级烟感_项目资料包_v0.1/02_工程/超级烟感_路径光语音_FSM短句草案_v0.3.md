# 超级烟感 · 路径光 / 语音 FSM + 短句草案 v0.3

**相对 v0.2**：对齐 `路径光语音_自研体验规格_v0.3` §5（Leo 补强）——白壳主 SKU；**RGB 全彩**；细环 **自研旋转/环扫**（路径 wow + 告警脉动之上）；默认 **64 分段**可寻址时序实现，不加到 96。  
**短句**：沿用已锁 **v0.1.1**（本文件仅扩光效；文案表不改）。  
**约束**：禁 Nest 大环面色块扫场 / 绿闪 Nightly 节奏 / Heads-Up 句式。语音可关，**声光不可关**。

## 光通道（对齐网表骨架 v0.1.1）
| 通道 | 用途 |
|---|---|
| LED_STATUS | 中心状态点；GPIO/PWM **直驱**（健康/告警主语义） |
| LED_RING[0..63] | 细环 64 分段；**可寻址 RGB**（单线/SPI 默认；I2C RGB 多片备） |
| LED_PATH | 底边线型导光；可与环共用灯驱通道或独立条（结构草排） |

驱动：可寻址协议帧（固件时序）；`ux_light_set(mode, rgb?, period_ms?, duty?)`；报警态强制最高优先，盖过夜间路径。

## 色语义（RGB · 盲测可区分）
| 事件 | 主色 | 辅色/节奏 | 禁区 |
|---|---|---|---|
| Smoke P0 | 红 `#FF1A1A` | 快脉动 2–4 Hz | — |
| CO P0 | 橙 `#FF7A00` | 与红交替或橙脉动（异于烟） | 勿用纯红同节奏 |
| Fault/Contam | 黄 `#FFCC00` | 双闪 / 慢双峰 | — |
| Path / Clear / OK | 冷白～浅青 | 非绿闪 Nightly | **禁** Nest 绿闪节奏 |
| LowBat | 琥珀黄慢闪 | 中心点为主，环可不亮 | — |

## 旋转 / 环扫动效（自研 · 64 分段时序）
| 模式 | 触发 | 行为（细环） | 中心点 | 底边路径 | 优先级 |
|---|---|---|---|---|---|
| `RingSweep_Night` | 夜间路径触发 | 单向环扫：亮窗 4–8 段 chase，周期 **1.0–1.5 s**（默认 1.2 s）；冷白低占空 | 极暗或跟随 | 同步单向流动 | P2（可被报警盖） |
| `Spin_Short` | SelfTest / FindMe | 短旋 0.8–1.5 圈后收束；白/浅青 | 白点亮 | 可选同向一波 | P2 |
| `Pulse_Smoke` | SmokeAlarm | 全环红脉动 + 可选慢旋强调（非整圈色块扫场） | 红快闪 | 红加速流动 | **P0** |
| `Pulse_CO` | COAlarm | 橙脉动；相位/占空与烟可区分 | 橙/红交替 | 橙流动 | **P0** |
| `Pulse_Fault` | Fault | 黄双闪；环可不旋 | 黄双闪 | 灭或慢闪 | P1（不压 P0） |
| `MuteHold` | MuteWindow | 环灭或极慢提示 | 红常亮 | 慢闪「临时静音」 | 安规窗内 |
| `ClearSweep` | Clear | 环收束一圈（短旋收束） | 绿/青一闪 | 收束到中心 | 过渡 |
| `Idle` | 待机 | 近熄；可选极慢暗蓝呼吸（中心） | 极慢呼吸 | 灭 | — |

**酷炫≠抄 Nest**：旋转靠 **64 段地址时序**（亮窗 chase / 短旋），**不用**大环面色块扫场；Nightly 类绿闪节奏不进固件默认表。

## 状态机（摘要）
`Idle → PathGuide(RingSweep_Night) → EventAnnounce → AlarmP0(Pulse_*) → MuteWindow → Clear(ClearSweep) → Idle`  
并行：`LowBat` / `Fault` / `Eos` 不压制 P0。  
测试短旋：`SelfTest` → `Spin_Short` → OK/Fail 反馈。

| 状态 | 光效模式 | 本地声 |
|---|---|---|
| Idle 夜间 | Idle | 无 |
| PathGuide | RingSweep_Night | 可选短嘀 |
| SmokeAlarm P0 | Pulse_Smoke | 火警音型（不可关） |
| COAlarm P0 | Pulse_CO | CO 音型（不可关） |
| MuteWindow | MuteHold | 安规允许窗内静音 |
| Clear | ClearSweep | 短确认音 |
| LowBat | 黄点慢闪 | 间歇啁啾 |
| Fault | Pulse_Fault | 故障音 |
| SelfTest / Find | Spin_Short | 见短句表 |

应达：起亮 ≤0.5 s；暗室路径 ≥4 m；三套告警盲测可区分。

## 语音短句
**锁定引用**：`超级烟感_语音短句_v0.1.1.md`（不在本版改文案）。

## 软件接口
- `ux_light_set(mode)` / `ux_voice_say(event_id)`；`voice_enable` NVS 可关
- 报警强制 `horn+light`；语音失败不影响声光
- 旋转参数（窗宽、周期、色）表驱动，OTA 可调；**不**写 Nest 对照进固件
- SED：非 P0 严格限占空比；限额等硬件寿命表冻后回写 ICD

## 风险与工期
- FSM+64 段旋转表驱动：**+2–3 日**（相对 v0.2）
- 风险：RGB+旋转抬峰值 → 与 CR123A / Thread SED 冲突；报警外限占空；通道冻后跟硬件寿命表
- 不加 96 段，除非成本过线且 Leo 再批

## v0.3 变更摘要
- RGB 全彩色语义表；旋转态：`RingSweep_Night` / `Spin_Short` / `Pulse_*` / `ClearSweep`
- 对齐体验规格 v0.3 §5；壳体白盘；64 分段不变
- 短句仍 v0.1.1

## v0.3.1 补丁（硬件改口）
- 灯驱默认：**可寻址 RGB×64**（单线/SPI）；非纯 I2C 单色级联
- PathGuide 环扫周期锁 **1.0–1.5 s**（对齐市场/测试判据草案）

## 占空比 / SED（对齐寿命表 v0.1 · ICD v0.1.5）
| ID | 限额 |
|---|---|
| L1 | RingSweep 同时亮 ≤8 段 |
| L2 | 非报警单段 ≤25% 冷白；周期 1.0–1.5 s |
| L3 | 连续环扫 ≤60 s，冷却 ≥5 min |
| L4 | Idle：`LED_RGB_EN=0`；STATUS ≤0.5 mA avg |
| L5 | 报警 ≤16 段、红/橙 ≤70%；守 600/1200 mA 墙 |
| L6 | 禁全环 64 满幅（产测外电除外） |
| L7 | 射频高峰非 P0 光效减半 |
| L8 | 声光限额与 voice_enable 无关 |

RingSweep 默认：window=6–8、brightness≤25%、period=1.2 s、max_continuous=60 s。
