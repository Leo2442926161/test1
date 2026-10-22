# 超级烟感 · SED 超限额拒绝/降级 · 可复现步骤 v0.1.1

**依据**：ICD **v0.1.6**；`超级烟感_CR123A_寿命与SED限额_v0.1.md`（L1–L8）  
**门禁**：G3 软件联调必测（功耗/光效合规）；**§4 已勾 · 可挂 G3**（2026-10-22）  
**日期**：2026-10-22 Asia/Shanghai · 测试  
**前提**：白壳 DUT；固件对齐 ICD v0.1.6；钩子 `ux_light_set` / `ux_light_get_status`；电流钳（峰值墙必测）

---

## 0. 通用判据符号

| 符号 | 含义 |
|---|---|
| **拒绝** | `limit_action=reject`；不进入违规态；回读 `reject_reason` |
| **降级** | `limit_action=clamp`；进入允许态；回读 `applied_*` 为钳后值 |
| **强制停** | L3：`limit_action=stop` → EN=0；冷却期内再请求环扫 → reject |
| **P0 例外** | Smoke/CO 仍须声光；只降段/降亮；守 L5 + 峰值墙 |

每条用例记录：固件版本、注入参数、`ux_light_get_status` 字段、EN 脚、电流、通过/失败。

---

## 1. 准备

1. DUT 上电；`voice_enable` 开/关各跑一遍关键项（L8）。
2. 探针：`LED_RGB_EN`(P2.08)、`LED_RGB_DATA`(P2.06)；可选电池回路电流钳。
3. 调用：`ux_light_set(mode, window?, brightness?, segments?, flags?)` → `ux_light_result_t`；回读 `ux_light_get_status()`。
4. 基线：Idle → **`led_rgb_en=0`**（L4）。

### 回读字段（ICD v0.1.6）

| 字段 | 用途 |
|---|---|
| `applied_window` / `applied_brightness` / `applied_segments` | 钳后实际值 |
| `led_rgb_en` | EN 逻辑 |
| `limit_action` | `none` / `clamp` / `reject` / `stop` |
| `reject_reason` | `window` / `brightness` / `cooldown` / `full_white` / `peak` / … |
| `rf_half` | L7 亮度减半档 |
| `sweep_elapsed_s` / `cooldown_remain_s` | L3 计时 |

---

## 2. 用例表（策略已锁定 · ICD v0.1.6）

### TC-L1 同时亮段超窗 → **降级**
- 步骤：`ux_light_set(PathGuide/RingSweep, window=16)`
- 期望：`limit_action=clamp`；`applied_window≤8`；窗外灭

### TC-L2 亮度超限 → **降级**
- 步骤：非报警环扫 `brightness=50%`
- 期望：`clamp`；`applied_brightness≤25%`；周期 1.0–1.5 s

### TC-L3 连续环扫超时 → **停 + 冷却拒绝**
- 步骤：RingSweep 保持 >60 s；冷却期内再请求连续环扫
- 期望：≤60 s 后 `stop` + `led_rgb_en=0`；冷却≥5 min 内再请求 → `reject` / `reject_reason=cooldown`

### TC-L4 Idle 灯串关电 → **强制**
- 步骤：PathGuide → Idle；或 Idle 后强推 RGB 帧
- 期望：Idle 稳定 `led_rgb_en=0`；仅 `LED_STATUS` 呼吸

### TC-L5 报警超段/亮 → **降级保声光**
- 步骤：SmokeAlarm + `segments=64` 或红 100%
- 期望：`clamp` ≤16 段、红/橙≤70%；HORN+光在；持续电流≤600 mA

### TC-L6 全环满白 → **拒绝**
- 步骤：非工装外电请求 64 段满幅白
- 期望：`reject` / `reject_reason=full_white`；不进入该态

### TC-L7 射频高峰 → **降级**
- 步骤：OTA/Thread 高峰 + PathGuide 正常亮度
- 期望：非 P0：`rf_half=true`；**亮度≤12.5%**；**窗仍≤8**；P0 仍守 L5

### TC-L8 关语音
- 步骤：`voice_enable=0`，重跑 TC-L1 + SmokeAlarm
- 期望：无语音；声光限额与开语音时相同

### TC-PK 峰值墙 → **限流降亮**
- 步骤：报警脉动 + 蜂鸣；电流采样
- 期望：持续 >100 ms ≤600 mA；<20 ms ≤1200 mA；超限降亮但声光不消失

---

## 3. G3 通过/有条件/退回

| 结论 | 条件 |
|---|---|
| **通过** | TC-L1～L6、L8、PK 全过；L7 可触发且过 |
| **有条件通过** | 仅 L7 触发手段弱，有书面替代；其余全过 |
| **退回** | Idle EN≠0；可进 64 满白；报警因限额无声光；破 600 mA 无降级 |

**挂 G3 状态（测前）**：§4 已勾 · **用例表可进 G3 联调清单** · 实测结论待 EVT。

---

## 4. 测前勾认（已勾 · ICD v0.1.6）

- [x] 超限策略：L1/L2 **clamp**；L3 **stop + 冷却 reject**；L4 Idle **EN=0**；L5 **降级保声光**；L6 满白 **reject**；L7 **降级**；PK **限流降亮**
- [x] 钩子：`ux_light_set` / `ux_light_get_status`；回读字段见 ICD v0.1.6（`applied_*` / `limit_action` / `reject_reason` / `rf_half` 等）
- [x] L7「一半」= **亮度≤12.5%**，窗仍 **≤8**（非窗减半）

**不下单。不替代 EVT 实测寿命表 v0.2。**
