# 超级烟感 · Hush 与级联 · 可复现测项 v0.1.2

**日期**：2026-10-22 Asia/Shanghai  
**状态**：G3 挂载（内部过线≠已过 UL/EN；实验室限值不编造）  
**对齐**：ICD **`超级烟感_软件硬件_ICD_v0.1.7.md`**；范围锁 v0.1.1；网表骨架 v0.1.2 §5；SED **L9**  
**钩子（ICD）**：`hush_active` / `hush_remaining_s`（别名 `hush_remain_s`）/ `hush_reject`（别名 `hush_reject_reason`）；`alarm_relay_tx` / `alarm_relay_rx`；NVS `relay_enable`；常量 `HUSH_SMOKE_S=540`

| 产品锁定（摘） | 测项约束 |
|---|---|
| Hush **仅烟**；默认 **9 min**；超时自动恢复；危急打断 | TC-HUSH-* 默认测 9 min |
| **CO 禁止 Hush** | TC-HUSH-CO-* 必须拒绝/无入口 |
| 同品牌无线级联：P0 烟/CO → 已组网他机**同类型**本地声光跟报；**断云仍级联** | TC-CASCADE-* |
| 本机 Hush **不压制**他机报警 | TC-HUSH-MULTI-* |
| Matter = 状态/通知；**≠**消防有线互联 | TC-MATTER-BOUND-* |
| EVT **N 台同响** 压降/RF 跌落（硬件风险） | TC-CASCADE-PWR-* |
| 级联帧失败 **不挡**本机本地声光（SED L9） | TC-CASCADE-FAIL-* |
| 无线跟报 ≠ UL/EN 有线多站互连声称 | TC-CLAIM-BOUND-* |

**前置**：ICD v0.1.7 钩子 `hush_*` / `alarm_relay_tx|rx` / `ux_light_*`；声压探针；组网工装；≥2 台同固件；断云：断互联网或 Border；`relay_enable` 可写 NVS。  
**声压**：相对判据（Hush 后本机蜂鸣显著下降或静默；恢复后恢复报警音型）。绝对 dB 以实验室限值为准，本表不编造。

---

## 0. 用例索引（G3）

| ID | 标题 | 期望摘要 | 门禁 |
|---|---|---|---|
| TC-HUSH-01 | 烟报警进入 Hush | 本机声压降/静默；光效进 MuteHold | G3 |
| TC-HUSH-02 | 默认 9 min 自动恢复 | 启动后 ≈9 min 恢复烟报警音型 | G3 |
| TC-HUSH-03 | 危急/再探测打断 Hush | 窗内升级或再确认 → 立即恢复声光 | G3 |
| TC-HUSH-04 | 声光不可永久关 | Hush≠关机；传感器/判决仍工作 | G3 |
| TC-HUSH-CO-01 | CO 禁止 Hush | 无入口或拒绝；声光不降 | G3 |
| TC-HUSH-MULTI-01 | 本机 Hush 不压他机 | 他机继续同类型跟报 | G3 |
| TC-CASCADE-01 | 烟 P0 → 他机烟跟报 | 已组网他机本地烟声光 | G3 |
| TC-CASCADE-02 | CO P0 → 他机 CO 跟报 | 同类型；不误发烟音型 | G3 |
| TC-CASCADE-03 | 断云仍级联 | 无互联网仍跟报 | G3 |
| TC-CASCADE-04 | 级联开关关 | 他机不跟报 | G3 |
| TC-CASCADE-05 | 未配对不跟报 | 异组/未配对机安静 | G3 |
| TC-CASCADE-FAIL-01 | 级联帧失败不挡本机 | 丢包/无 ACK 本机声光仍满 | G3 |
| TC-CASCADE-PWR-01 | EVT N 台同响压降/RF | 记录 Vbat/丢包；不破峰值墙 | G3-EVT |
| TC-MATTER-BOUND-01 | Matter 仅通知 | 断 Matter 仍本地级联 | G3 |
| TC-MATTER-BOUND-02 | 禁止消防有线声称验证 | 文档/Listing 无有线互连替代语 | G3 |
| TC-CLAIM-BOUND-01 | 无线跟报≠有线多站声称 | Listing/说明禁 UL/EN interconnect 等同语 | G3 |

**用例数**：**16**（Hush 6 + Cascade 5 + FAIL/PWR 2 + Matter-bound 2 + Claim 1）

---

## 1. TC-HUSH-*（烟临时静音）

### TC-HUSH-01 · 烟报警进入 Hush
**步骤**
1. 单机注入/工装触发 **烟 P0**（T3 音型 + 报警红脉）。
2. 确认蜂鸣与光效已起。
3. 按 `BTN_TEST_MUTE`（或规格定义的 Hush 手势）一次。
4. 回读固件：`hush_active=true`、`hush_remaining_s≈540`（=`HUSH_SMOKE_S`）、`alarm_type=smoke`。

**判据（通过）**
- 本机蜂鸣在 ≤2 s 内显著下降或静默；光效允许 MuteHold（仍可有非满幅指示）。
- 语音若开可播短提示；`voice_enable=0` 时不影响声光路径。
- **不得**关闭传感器采样与判决任务。

**退回**
- 无 Hush 响应；或 Hush 后采样/判决停；或误把 CO 当烟处理。

---

### TC-HUSH-02 · 默认 9 min 自动恢复
**步骤**
1. 同 TC-HUSH-01 进入烟 Hush；记录 `t0`。
2. 保持烟条件仍在（或工装维持「烟未清除」标志，避免自然 Clear 干扰）。
3. 在 `t0+8.5 min` 抽检：仍 `hush_active=true`。
4. 在 `t0+9.0 min ±15 s` 观察自动退出。

**判据（通过）**
- 默认窗 **9 min**（540 s）；允许工装时钟 ±15 s。
- 超时后自动恢复烟报警音型/光效（若条件仍在）。
- 不得依赖 App/云计时。

**退回**
- 窗显著偏离 8–10 min；永不恢复；或依赖云倒计时。

---

### TC-HUSH-03 · 危急/再探测打断 Hush
**步骤**
1. 进入烟 Hush（窗内）。
2. 工装注入「危急升级」或「再确认火警」（规格：浓度/趋势升级标志）。
3. 观察 ≤2 s 内是否退出 Hush 并恢复满声光。

**判据（通过）**
- Hush 被打断；`hush_active=false`；P0 声光恢复。
- 打断路径不经 Thread/Matter。

**退回**
- 危急下仍保持静音超过 2 s。

---

### TC-HUSH-04 · Hush ≠ 关机 / 声光不可永久关
**步骤**
1. 烟 Hush 期间尝试：关语音、关路径光、App「关闭报警」类入口（若有）。
2. 确认无「永久关闭声光报警」的成功路径。
3. Hush 窗结束或打断后声光可恢复。

**判据（通过）**
- 语音可关 ≠ 声光可关（对齐产品红线）。
- 无用户路径将 `HORN_DRV` / 报警光永久禁用。
- Hush 仅为临时窗。

**退回**
- 存在永久关声光的用户路径。

---

## 2. TC-HUSH-CO-*（CO 禁止）

### TC-HUSH-CO-01 · CO 禁止 Hush
**步骤**
1. 触发 **CO P0**（T4 音型）。
2. 按 `BTN_TEST_MUTE` / 调 Hush API / App 静音（凡产品暴露的入口）。
3. 回读：应拒绝或无 Hush 状态机切入；回读 `hush_reject=co_forbidden`（或别名 `hush_reject_reason`）。

**判据（通过）**
- CO 下 **无** `hush_active=true`；或明确 `hush_reject=co_forbidden`。
- 本机 CO 声光**不降**。
- Listing/说明书不写「可静音 CO」（文档抽检，见 TC-MATTER-BOUND-02 同类）。

**退回**
- CO 可进入与烟相同的静音窗。

---

## 3. TC-HUSH-MULTI-*（多机不压制）

### TC-HUSH-MULTI-01 · 本机 Hush 不压制他机
**前置**：两机 A/B 已同品牌无线组网；`relay_enable=1`。
**步骤**
1. A 触发烟 P0 → 确认 B **烟类型**本地跟报。
2. 仅在 **A** 上 Hush。
3. 观察 B 声光。

**判据（通过）**
- A：进入 Hush（声压降）。
- B：**继续**同类型报警声光（不被 A 的 Hush 压制）。
- 若规格要求「仅起始机可 Hush」，B 上按键不得错误静音全网（若 B 为跟报机：按键行为以 ICD 为准，但不得导致 A 危急路径失效）。

**退回**
- A Hush 后 B 同步静音。

---

## 4. TC-CASCADE-*（同品牌无线级联）

### TC-CASCADE-01 · 烟 P0 → 他机烟跟报
**步骤**
1. A/B 组网；`relay_enable=1`；云可连（本项不依赖断云）。
2. 仅 A 注入烟 P0。
3. 可选：工装经 `alarm_relay_tx` 注入远端烟报警验跟报路径。
4. 在 ≤3 s（L1 延时不计入 ICD ≤50 ms 本地硬路径）内观察 B；对端应有 `alarm_relay_rx`。

**判据（通过）**
- B 起 **烟** 本地声光（同类型）；硬路径在 B 本地执行（蜂鸣不经云转发播放）。
- A 为起始机；B 为跟报机（回读角色标志若有）。

**退回**
- B 无声光；或误报 CO 音型；或仅 App 通知无本机声。

---

### TC-CASCADE-02 · CO P0 → 他机 CO 跟报
**步骤**
1. 同组网；仅 A 注入 CO P0。
2. 观察 B。

**判据（通过）**
- B 起 **CO** 本地声光；音型与烟可区分。
- B 不得因跟报而开放 CO Hush。

**退回**
- 类型错乱；或 B 无本地声。

---

### TC-CASCADE-03 · 断云仍级联
**步骤**
1. A/B 组网后：**断开互联网 / 关闭手机 / Border Router 失联**（保留本地 Thread/自研射频如规格）。
2. A 触发烟 P0。
3. 观察 B。

**判据（通过）**
- B 仍本地烟跟报（与有云时一致）。
- App 通知可缺失；**级联不得依赖云**。

**退回**
- 断云后级联失效。

---

### TC-CASCADE-04 · 用户关闭「级联报警」
**步骤**
1. 写 NVS `relay_enable=0`（或设置关「级联报警」）；A 触发烟 P0。
2. 观察 B。

**判据（通过）**
- B 不跟报；A 本机报警正常。
- 再置 `relay_enable=1` 后恢复 TC-CASCADE-01。

**退回**
- 开关无效。

---

### TC-CASCADE-05 · 未配对 / 异组不跟报
**步骤**
1. C 未与 A 配对（或异家庭/异密钥）。
2. A 烟 P0；观察 C。

**判据（通过）**
- C 保持 Idle/非报警。
- 不得串网误报。

**退回**
- 异组误跟报。

---

## 5. TC-CASCADE-FAIL-* / TC-CASCADE-PWR-*（硬件·SED L9）

对齐：网表骨架 **v0.1.2 §5**；寿命表 SED **L9**（无线级联突发；级联失败不阻断本地声光；TX 不破峰值墙）。

### TC-CASCADE-FAIL-01 · 级联帧失败不挡本机本地声光（G3 · P0）
**步骤**
1. 单机 A 触发烟或 CO P0，确认本机声光已满幅。
2. 工装制造级联失败：屏蔽 RF / 对端关机 / 注入持续 NACK/丢包（按 ICD 重试上限跑满）。
3. 观察 A 在失败与重试全程的本地声光；回读不得出现「等 ACK 才鸣」。

**判据（通过）**
- A 本地声光**不中断、不降级为等网**；硬路径不经级联 ACK。
- 级联可记失败/重试耗尽；**不得**因级联失败关闭 `HORN_DRV`/报警光。
- 与 SED L9「级联失败不阻断本地声光」一致。

**退回**
- 丢包或无对端时本机声光停、卡死、或明显变弱超过瞬态抖动。

---

### TC-CASCADE-PWR-01 · EVT N 台同响：压降与 RF 跌落（G3-EVT · P0）
**前置**：EVT ≥N 台同固件已组网（建议 N=起始机+跟报机，**N≥3**；N 写入报告）。峰值墙：网表 §5 **600 mA 持续 / 1200 mA 脉冲**。
**步骤**
1. 全员进入同响：A 触发烟 P0 → 已组网机同类型本地跟报，或工装同步触发多机报警态。
2. 测点：各机电池端或板级 `Vbat`（示波器/记录仪）；同时记级联帧成功率/RSSI/重试（软件计数即可）。
3. 持续 ≥30 s 同响窗；抽检是否出现欠压复位、射频失联、跟报中断。

**判据（通过）**
- 报告给出：**N、最低 Vbat、最大压降、同响窗丢包/重试率**（实测定性，不编造限值）。
- 同响期间本机与跟报机声光保持；无因压降导致的系统性 RF 失联（允许记「风险观察」若压降大但仍可鸣）。
- 软件级联 TX 占空受控；电流尖峰叙述对照峰值墙（若有分流采样）。

**退回**
- 同响导致本机或跟报机声光失效、看门狗复位、或级联全面垮掉且无复现数据。
- 未测 N 台同响却宣称硬件级联 EVT 通过。

**注**：绝对电流/灵敏度限值以实验室/硬件冻结为准；本项强制**出数**，禁止口头「应该没问题」。

---

## 6. TC-MATTER-BOUND-*（Matter 边界）


### TC-MATTER-BOUND-01 · Matter 仅通知，不替代本地级联
**步骤**
1. A/B 本地级联可用。
2. 断开 Matter 控制器会话 / 禁用 Matter fabric（保留本地无线级联承载）。
3. A 烟 P0 → 看 B；同时确认无 Matter 订阅时 App 可能无推送。

**判据（通过）**
- B 本地跟报仍成功。
- 日志/状态允许 Matter 事件缺失；**不得**把 Matter 连通当作级联前置。

**退回**
- 无 Matter 则他机不响。

---

### TC-MATTER-BOUND-02 · 禁止「Matter/App = 消防有线互联」声称
**步骤**
1. 抽检 PRD/Listing/说明书/认证资料包文案。
2. 确认无「替代有线多站消防互连 / AC interconnect wire」等违规声称。
3. Listing 允许：interconnected / whole-home alerts（**同品牌无线**）+ Matter 通知分列。

**判据（通过）**
- 文案与 `互联与Hush_产品范围锁定_v0.1` §2.2 一致。
- 有线互连主轨标注不做/可延。

**退回**
- 出现消防级有线互连或 Matter=互连的对外声称。

---


## 7. TC-CLAIM-BOUND-*（文案边界 · 与 Matter 分列）

### TC-CLAIM-BOUND-01 · 无线跟报 ≠ UL/EN 有线多站互连声称（G3 · P0）
**步骤**
1. 抽检 PRD、Listing、说明书、包装、认证资料包、增长话术。
2. 对照网表 §5「无线级联 ≠ UL/EN 有线 multiple-station interconnect 声称」与产品范围锁 §2.2。

**判据（通过）**
- 可写：同品牌无线互联 / whole-home alerts（无线）。
- **禁止**：将无线跟报等同或替代 UL217/EN 等 **有线 multiple-station interconnect** 符合性声称；禁止暗示已获有线多站互连 Listing。
- 与 TC-MATTER-BOUND-02 互补：本项盯「无线≠有线互连」；彼项盯「Matter≠消防互联」。

**退回**
- 出现有线互连符合性等同语或「已满足有线多站」类对外表述。

---
## 8. G3 汇总通过 / 退回

### 8.1 通过（内部）
- TC-HUSH-01/02/03/04、TC-HUSH-CO-01、TC-HUSH-MULTI-01 **全过**。
- TC-CASCADE-01/02/03/04/05、**TC-CASCADE-FAIL-01** **全过**。
- TC-MATTER-BOUND-01/02、**TC-CLAIM-BOUND-01** **全过**。
- **EVT**：TC-CASCADE-PWR-01 出具 N/Vbat/丢包报告后，方可签硬件级联风险关闭。
- 证据：工装日志、双机录屏/声压相对曲线、组网密钥版本、固件哈希；EVT 示波/计数导出。

### 8.2 退回（任一）
- CO 可 Hush；或烟 Hush 默认窗严重偏离 9 min；或危急不打断。
- 本机 Hush 压制他机。
- 断云后级联失败；或级联依赖 Matter/云。
- 异组串报；或类型错乱（烟↔CO）。
- **级联帧失败导致本机声光中断**（违 SED L9）。
- EVT N 台同响未测或同响致失声/复位且无数据。
- 对外文案把 Matter/无线跟报写成 UL/EN 有线多站消防互连替代。

### 8.3 明确不在本表
- UL/EN 绝对声压限值、实验室互联协议符合性证书号。
- Nest Heads-Up /「烟雾在×房间」剧本（产品禁止照搬）。
- 有线多站互连（一期不做）。

---

## 9. 下游回写
| 角色 | 动作 |
|---|---|
| 软件 | ICD **v0.1.7 已定**，测项只消费钩子；无需再回写 |
| 硬件 | 网表 §5 / SED L9；EVT 执行 TC-CASCADE-PWR-01；无有线互联脚（一期） |
| 市场产品 / 增长 | Listing：无线跟报与有线互连分列；禁 Nest 房间名剧本 |
| 测试 | 本表挂 G3；FAIL-01 为 P0；PWR-01 为 G3-EVT 出门禁 |

**变更**：
- v0.1：对齐产品锁定 v0.1.1（Hush 仅烟 / 9 min / CO 禁 / 断云级联 / 不压他机）。
- v0.1.1：补 TC-CASCADE-FAIL-01、TC-CASCADE-PWR-01（N 台同响）、TC-CLAIM-BOUND-01；对齐网表 v0.1.2 §5 与 SED L9。
- **v0.1.2**：标题/引用对齐 **ICD v0.1.7**（`HUSH_SMOKE_S=540`、`hush_*`、`alarm_relay_tx/rx`、`relay_enable`）；软件无需再为测项回写 ICD；G3 任一 P0 Fail→退回。L9/N 台同响已在本册 TC-CASCADE-PWR-01，不另挂 SED 册。
