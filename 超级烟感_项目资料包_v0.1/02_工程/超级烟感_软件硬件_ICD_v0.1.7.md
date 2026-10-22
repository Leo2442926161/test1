# 超级烟感 · 软件-硬件 ICD v0.1.7（正式）

**相对 v0.1.6 / +L9**：SED 增 **L9**（级联突发+常开组网开销归软件）；峰值墙仍 600/1200 mA。原：对齐市场 `互联与Hush_产品范围锁定_v0.1.1`——Hush 烟-only / `MuteWindow` 默认 9 min（可配 8–10）/ CO 禁；Thread·自研报文无线离线级联；Matter 仅通知。

**基线**  
主控 nRF54L15（差分 nRF52840 / MG24）。CO：TGS5141 主、NAP-508 备。光学 EVT 双轨。CR123A×2 并联。Matter=状态/事件；本地声光优先。到仓 **$32** 不动。

## 已冻配置
| 项 | 值 |
|---|---|
| 壳体 | 浅色薄盘（白）主 SKU |
| 光圈 | **64×可寻址 RGB**；旋转靠时序，不加 96 |
| 灯驱 | 单线默认 / SPI 备；`LED_STATUS` 直驱；`LED_RGB_EN` 关电 |
| 环扫 | 周期 **1.0–1.5 s**（默认 1.2）；窗 **≤8**；亮 **≤25%**；连续 **≤60 s** |
| 短句 | v0.1.1 锁定 |
| Hush | 烟-only `MuteWindow` 默认 **9 min**（可配 8–10）；**CO 禁** |
| 级联 | 同品牌 Thread/自研报文离线跟报；Matter≠级联；无有线主轨 |
| 确认窗 / 灵敏度 / N-03 / 文案禁区 | 同 v0.1.4a |

## SED / 占空比限额（必须执行 · 来源寿命表 v0.1）
| ID | 限额 | 适用 |
|---|---|---|
| **L1** | 同时点亮 **≤ 8 段**；窗外全灭 | RingSweep_Night / PathGuide |
| **L2** | 单段亮度 **≤ 25%**（冷白）；周期 1.0–1.5 s | 非报警 |
| **L3** | 连续环扫 **≤ 60 s**，冷却 ≥5 min（或累计 5 min/小时） | 防演示耗电 |
| **L4** | Idle：**`LED_RGB_EN=0`**；仅 `LED_STATUS` ≤0.5 mA avg 呼吸 | 夜间待机 |
| **L5** | 报警：同时亮 **≤ 16 段**，红/橙 **≤ 70%**；可叠蜂鸣，总电流守峰值墙 | Pulse_* P0 |
| **L6** | **禁止**全环 64 段 RGB 满幅；产测全亮仅工装外电 | 工厂 |
| **L7** | OTA/Thread 高峰时非 P0：**亮度减半**（≤12.5%）；窗仍守 L1≤8 | 与射频错峰 |
| **L8** | `voice_enable` 无关：声光限额不变；语音功放 Idle 关 | — |
| **L9** | **无线级联/常开组网**：跟报占空软件限；报警态级联 TX 不破峰值墙；**级联失败不阻断本地声光**；常开组网开销归软件 SED（Idle）；`relay_enable=0` 可降 | 一期跟报 |

**峰值墙（并联 CR123A）**  
- 持续 >100 ms：**≤ 600 mA**（含灯+蜂鸣+级联 TX）  
- 短脉冲 <20 ms：**≤ 1200 mA**  
超限固件限流/降亮。P0 守 L5+峰值墙。级联**无新脚**；失败只丢 L1 跟报，**L0 声光不挡**。常开组网开销守 **L9**（与 L1–L8 一并执行）。

**RingSweep 默认参数**：window=6–8、brightness≤25%、period=1.2 s、max_continuous=60 s。

## 信号 ICD（逻辑名）
| 信号 | 方向 | 接口 | 软件用途 | 备注 |
|---|---|---|---|---|
| OPT_I2C_SDA/SCL | 双向 | TWIM/I2C | ADPD 性能轨 | 与分立互斥 |
| OPT_INT | IN | GPIO 下降沿 | 光学就绪/故障 | |
| OPT_DISC_ADC | IN | ADC | TI 分立轨 | 互斥装配 |
| OPT_RAIL_SEL | IN | GPIO | 识光路轨 | |
| CO_SIG | IN | 模拟 ADC | CO 剂量 | |
| CO_BIAS_EN | OUT | GPIO | 偏置使能 | |
| HORN_DRV | OUT | 高驱 GPIO | 蜂鸣 P0 | 禁与 RF_WAKE 复用 |
| LED_STATUS | OUT | GPIO/PWM | 中心点直驱 | P0.01 |
| LED_RGB_DATA | OUT | 单线/SPI MOSI | 可寻址数据 | P2.06；禁 HORN/WAKE |
| LED_RGB_CLK | OUT | SPI SCK | 时钟 | P2.07；单线 **DNP** |
| LED_RGB_EN | OUT | GPIO→负载开关 | 灯串供电 | P2.08；**Idle=0**；Alarm 先置 1 再出帧 |
| BTN_TEST_MUTE | IN | GPIO | SelfTest/消音 | |
| VBAT_SENSE | IN | ADC | BatteryAlert | |
| COVER_DET | IN | GPIO | 仓盖 | 必选 |
| HW_FAULT_SNS | IN | GPIO/ADC | HardwareFault | |
| CONTAM_TP | 测点 | — | Contamination | |
| SWD/UART_DBG | 调试 | — | 产测 | |
| RF_* | — | 模组 | Thread | 净空≥5mm |

已删除默认轨：`LED_I2C_*` / 单色 I2C 灯驱。

## 光效 FSM 摘要（详 FSM v0.3.1）
| 模式 | 限额 | 优先级 |
|---|---|---|
| RingSweep_Night | L1+L2+L3 | P2 |
| Spin_Short | L1+L2（短） | P2 |
| Pulse_Smoke/CO/Fault | L5 + 峰值墙 | P0/P0/P1 |
| Idle | **L4** `LED_RGB_EN=0` | — |
| ClearSweep / MuteHold | L1/L2 量级 | 过渡 |

禁 Nest 大环面色块扫场、绿闪 Nightly。

## 探针表（增量）
| 测法 | 判据 |
|---|---|
| 硬路径 ≤50ms | 判决→HORN/LED；Alarm 先 `LED_RGB_EN=1` 再出帧，仍 ≤50ms |
| L1–L4 合规 | 环扫窗≤8、亮≤25%、连续≤60s；Idle EN=0 |
| 峰值墙 | 持续≤600mA / 脉冲≤1200mA（工装电流） |
| 烟 Hush | 启动→声降→默认 9 min 恢复；危急打断立即恢复 |
| CO 禁 Hush | 按键拒绝；声光不停 |
| 级联跟报 | 断云仍跟报；本机 Hush 不压他机；`relay_enable=0` 不跟 |
| 其余 | 同 v0.1.4a（断网声光、OTA 拒升、COVER、轨识、CO、三色可区分） |

## Pin 草案（网表 v0.1.2）
| 逻辑名 | SoC Pin | 备注 |
|---|---|---|
| LED_STATUS | P0.01 | 直驱 |
| LED_RGB_DATA | P2.06 | 单线默认 |
| LED_RGB_CLK | P2.07 | 单线 DNP |
| LED_RGB_EN | P2.08 | Idle=0 |
| 其余 | 同 v0.1.3 | 绝对坐标仍 3–5 日冻 |

## 硬路径与时序
- 判决 → 声光 **≤50 ms**；Alarm：`LED_RGB_EN=1` → 缓存帧/全局亮度，忌阻塞长串
- Matter 异步；边沿队列 8–16
- L7：射频高峰非 P0 **亮度减半**（窗不减半）

## 互检
| 角色 | 结论 |
|---|---|
| 硬件 | 网表 v0.1.2 + 寿命/SED v0.1 已交 |
| 软件 | **ICD v0.1.7 正式** · 对齐市场范围锁 v0.1.1 |
| 测试 | 可挂 L 限额复现项 |
| 财务 | 知会电流风险；$32 不动 |



## SED 超限策略（G3 测前勾认 · 软件锁定）

**总则**：可钳参数 → **降级（clamp）** 并回读实际值；硬禁态 → **拒绝（reject）** 不进入；P0 报警永不因限额关声光，只降段/降亮。

| TC | 策略 | 行为 |
|---|---|---|
| TC-L1 窗>8 | **降级** | clamp `window≤8`；进入 PathGuide |
| TC-L2 亮>25% | **降级** | clamp `brightness≤25%` |
| TC-L3 >60s | **强制停** → 冷却期 **拒绝** | 停扫 + `LED_RGB_EN=0`；冷却≥5 min 内再请求连续环扫 → `reject` |
| TC-L4 Idle | **强制** | 入 Idle 必须 `LED_RGB_EN=0` |
| TC-L5 报警超段/亮 | **降级** | clamp ≤16 段、红/橙≤70%；HORN+光保留 |
| TC-L6 64 满白 | **拒绝** | 不进入；产测外电+工装标志除外 |
| TC-L7 射频高峰 | **降级** | 非 P0：**亮度→L2 的一半（≤12.5%）**；窗仍≤8；P0 仍守 L5 |
| TC-L8 | — | 与 voice 无关，策略同上 |
| TC-PK 破峰值墙 | **降级** | 限流/降亮；声光不消失 |

**L7 精确定义**：一半 = **亮度减半**（非窗减半）。

### 测试钩子与回读（EVT；量产可关）
- 调用：`ux_light_set(mode, window?, brightness?, segments?, flags?)` → 返回 `ux_light_result_t`
- 回读：`ux_light_get_status()` / 调试寄存器同构

| 字段 | 类型 | 含义 |
|---|---|---|
| `mode` | enum | 当前/请求模式 |
| `applied_window` | u8 | 钳后窗宽 |
| `applied_brightness` | u8 | 钳后亮度 % |
| `applied_segments` | u8 | 钳后同时亮段数 |
| `led_rgb_en` | bool | EN 脚逻辑 |
| `limit_action` | enum | `none` / `clamp` / `reject` / `stop` |
| `reject_reason` | enum | `none` / `window` / `brightness` / `cooldown` / `full_white` / `peak` / `other` |
| `rf_half` | bool | L7 亮度减半档生效 |
| `sweep_elapsed_s` | u16 | 本段环扫已持续 |
| `cooldown_remain_s` | u16 | 冷却剩余 |

对齐文档：`超级烟感_SED超限额拒绝降级_可复现步骤_v0.1.md` §4。



## Hush / 消音（正式 · 对齐市场范围锁 v0.1.1）

| 项 | 值 |
|---|---|
| 状态名 | `MuteWindow`（仅挂 **烟** P0） |
| 触发 | 报警态 `BTN_TEST_MUTE` 短按 |
| 时长 | 常量可配 **8–10 min**，默认 **`HUSH_SMOKE_S = 540`（9 min）**；超时自动恢复声光 |
| **CO** | **拒绝 Hush**（`hush_reject=co_forbidden`）；声光不停 |
| 危急打断 | 烟再升级 / 新危急探测 / CO 出现 → **立即结束** MuteWindow，恢复完整声光 |
| 与语音 | `voice_enable` 无关；声光不可永久关 |
| 光效 | MuteHold；守 SED |
| 级联时 | 本机 Hush **不压制**他机已级联报警声光 |

回读：`hush_active` / `hush_remaining_s`（别名 `hush_remain_s`）/ `hush_reject`（别名 `hush_reject_reason`）。  
**确认窗 20 s ≠ MuteWindow**。

## 无线多站级联（正式 · 同品牌离线跟报）

```
L0 本地判决 → ≤50ms → 本机 HORN+LED（硬路径）
L1 Thread / 自研安全报文 → 已配对同品牌机同类型本地声光跟报（**离线仍可**，不依赖云/手机）
L2 Matter 状态/事件通知（**不替代** L0/L1）
```

| 项 | 值 |
|---|---|
| 形态 | 一台烟或 CO **P0** → 同品牌已配对机 **同类型**本地声光跟报 |
| 承载 | Thread + 自研安全报文 |
| Matter | 仅状态/通知 |
| 用户开关 | NVS `relay_enable`（级联报警）默认开，可关 |
| 本机 Hush | **不压**他机 |
| 网表 | **无**有线 `INTERCONNECT` 脚 |
| 禁区 | 消防总线替代声称；Nest Heads-Up /「烟雾在×房间」 |

接口：`alarm_relay_tx/rx`；测试钩子注入远端报警。L1 延时**不计入** ≤50 ms。功耗守 **L9** + 峰值墙 600/1200 mA。

## 风险与工期
限额下家用换电目标 ≥2 年可谈（名义表）；EVT 实测后寿命表 v0.2。旋转驱动按 L 推，约 +2–3 日；级联协议+Hush 状态机另 +2–4 日。风险：Thread 级联功耗与认证文案越界。不下单。
