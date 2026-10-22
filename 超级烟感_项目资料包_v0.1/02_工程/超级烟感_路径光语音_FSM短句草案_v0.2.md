# 超级烟感 · 路径光 / 语音 FSM + 短句草案 v0.2

**约束**：Nest 级酷炫体验目标；外形与剧本**自研**；禁大环面 / Nightly Promise 节奏 / Heads-Up「烟雾在×房间」句式。语音可关，**声光不可关**（安规）。非法律意见。

## 光通道（少通道，对齐硬件）
| 通道 | 用途 |
|---|---|
| LED_STATUS（中心点） | 健康/告警主语义 |
| LED_RING（细环，少颗 PWM） | 呼吸/环扫引导，非整圈灯带 |
| LED_PATH（底边线型导光条） | 「事件方向感」流动指引 |

## 状态机（摘要）
`Idle → PathGuide → EventAnnounce → AlarmP0 → MuteWindow → Clear → Idle`  
并行：`LowBat` / `Fault` / `Eos` 不压制 P0。

| 状态 | 中心点 | 细环 | 线型路径光 | 本地声 |
|---|---|---|---|---|
| Idle 夜间 | 极慢暗蓝呼吸 | 暗 | 灭 | 无 |
| PathGuide（入网/找设备） | 白点亮 | 细环单点游走（自研节奏，≠Nest 绿闪） | 底边单向流动 1.2s 循环 | 可选短嘀 |
| SmokeAlarm P0 | 红快闪 | 红细环全亮脉动 | 红向门口方向加速流动 | 火警音型（不可关） |
| COAlarm P0 | 橙/红交替 | 同左脉动 | 橙流动 | CO 音型（不可关） |
| MuteWindow | 红点常亮 | 环灭 | 慢闪提示「临时静音」 | 安规允许窗内静音 |
| Clear | 绿点一闪 | 环收束一圈 | 线型收束到中心 | 短确认音 |
| LowBat | 黄点慢闪 | 灭 | 灭 | 间歇啁啾 |
| Fault/Contam | 黄点双闪 | 灭 | 灭 | 故障音 |

**酷炫但不抄**：流动用「底边线段 chase」+ 中心点强调，不用大环面色块扫场；夜间 Idle 用极低占空比，暗室路径光 ≥4 m 可见底边流动；起亮 ≤0.5 s。

## 语音短句（事件+动作 · 可关）
格式：`[事件]。[立刻动作]。` 英文默认（美欧）；中文另表。禁房间定位互联话术。

| 触发 | 草案 EN | 备注 |
|---|---|---|
| SmokeAlarm | Smoke. Get out now. | 短硬；可选第二段 Leave and call emergency services. |
| COAlarm | Carbon monoxide detected. Move to fresh air now. | |
| Smoke+CO | Smoke and carbon monoxide. Leave now. | |
| MuteWindow | Alarm muted temporarily. Stay alert. | |
| Clear | Alarm cleared. Check the area. | |
| LowBat | Battery low. Replace batteries soon. | |
| Fault | Device fault. Replace or service unit. | |
| PathGuide / Find | Here. Follow the light. | 找设备，非报警 |
| SelfTest OK | Test OK. | |
| SelfTest Fail | Test failed. Check device. | |

中文草案（国内 SKU 可选）：「检测到烟雾。立即撤离并报警。」「检测到一氧化碳。立即到通风处。」「报警已暂时静音。请保持警惕。」「电量低。请尽快换电池。」

## 软件接口
- `ux_light_set(mode)` / `ux_voice_say(event_id)`；`voice_enable` NVS 可关，默认开（区域合规另定）
- 报警态强制 `horn+light`；语音失败不影响声光
- 文案表外置，OTA 可更新；**不**写 Nest 对照进固件

## 待市场 v0.3 / 测试
- 暗室≥3m、1m 可懂正式判据
- 流动速率/颜色是否进应达规格
- 多语言包范围

**工期粗估**：FSM+表驱动光效 3–5 日；语音播报集成视 codec/模块 +2–4 日。风险：酷炫与功耗/SED 冲突——报警外严格限占空比。


## v0.2 变更（市场有条件通过 + 测试判据）
- 路径光：暗室 **≥4 m**、起亮 **≤0.5 s**（废止 ≥3 m）
- Smoke 主句压短；长句改可选第二段
- 对齐 ID v0.2：密分段细环 + wow 路径 + 报警脉动
- 测项：三套告警盲测可区分；voice_enable=0 声光完整；禁 Nest 大环/绿闪/Heads-Up 句式
