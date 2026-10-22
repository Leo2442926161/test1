# 超级烟感 · 文档抽检 · TC-CLAIM-BOUND-01 / TC-MATTER-BOUND-02 · v0.1

**日期**：2026-10-22 Asia/Shanghai  
**方式**：无样机桌面抽检（仅文案；≠ 实机 G3）  
**依据**：`超级烟感_Hush与级联_可复现测项_v0.1.2.md` §6–§7  
**红线（本抽检强制核对）**：禁 Nest 房间剧本 · 禁 Matter=消防互联 · 禁可静音 CO · 无线≠有线多站  

**抽检范围（已读）**  
| 类型 | 路径 |
|---|---|
| 范围锁 | `01_市场产品/互联与Hush_产品范围锁定_v0.1.md`（文内 v0.1.1） |
| PRD | `01_市场产品/PRD-v0.1_大纲.md` |
| Listing | `超级烟感-Amazon-US-EU-Listing草稿.md`（及资料包 `01_市场产品/` 同步稿） |
| 竞品对齐 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` |
| 认证主稿 | `超级烟感_UL_EN认证流_标准与资料清单_v0.1.md`（§4 #4/#9） |
| 测项定义 | `超级烟感_Hush与级联_可复现测项_v0.1.2.md` |

**未齐（影响闭环）**：说明书正式稿、包装文案、增长上架终稿 — 见认证主稿 §3.4 / §3.9。

---

## 0. 汇总

| 用例 | 结论 | Pass | Fail | 待补 |
|---|---|---:|---:|---:|
| **TC-MATTER-BOUND-02** | **待补**（现有稿无违规；说明书/包装未齐） | 5 | 0 | 2 |
| **TC-CLAIM-BOUND-01** | **待补**（现有稿无「有线多站等同」声称；Listing 级联分列未写全） | 4 | 0 | 3 |
| **关联禁区**（Nest 房间 / 可静音 CO） | **待补**（禁区已写死；Listing 尚未写「烟 Hush / 不写可静音 CO」正向句） | 4 | 0 | 2 |
| **合计（子项）** | — | **13** | **0** | **7** |

> **无 Fail。** G3 文案门禁：**说明书 + Listing 级联分列补齐前，不得将本两 TC 标为全过。**

---

## 1. TC-MATTER-BOUND-02 · 禁止「Matter/App = 消防有线互联」声称

**测项要点**（摘自测项 v0.1.2）：抽检 PRD/Listing/说明书/认证包；无「替代有线多站消防互连 / AC interconnect wire」；允许同品牌无线 + Matter 通知**分列**。

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| M1 | `01_市场产品/互联与Hush_产品范围锁定_v0.1.md` §2.1–2.2 | **Pass** | 「Matter \| **仅状态/通知**，**不替代**本地级联」；「Matter = 消防级互联 \| **禁止声称**」 |
| M2 | `01_市场产品/PRD-v0.1_大纲.md` · 互联与 Hush | **Pass** | 「Matter=状态通知不替代；**无有线互连主轨**」 |
| M3 | `超级烟感-Amazon-US-EU-Listing草稿.md` §1.2 #3 | **Pass** | 「Matter-compatible status and notifications … **Not** a fire-system interconnect claim.」 |
| M4 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` §A/§B | **Pass** | 「**禁止**声称=消防互联」；「Matter 仅作通知补充…**不替代** Opt1」 |
| M5 | `超级烟感_UL_EN认证流_标准与资料清单_v0.1.md` §4 #9 | **Pass** | 「Matter **仅状态/通知**；**不做**有线多站互连消防级声称」 |
| M6 | 说明书 / 用户手册 | **待补** | 认证主稿 §3.4 / §3.9：「说明书/包装/DoP 展示 — G3 前出草稿」→ **尚无正文可抽** |
| M7 | 包装 / 铭牌声称 | **待补** | 同上；无可抽正文 |

**本 TC 判定**：**待补**（M1–M5 全 Pass、0 Fail；M6–M7 未齐 → 不可签 G3 全过）。

---

## 2. TC-CLAIM-BOUND-01 · 无线跟报 ≠ UL/EN 有线多站互连声称

**测项要点**：可写同品牌无线 / whole-home alerts（无线）；**禁止**将无线等同/替代 UL217·EN **有线** multiple-station interconnect 符合性，或暗示已获有线多站 Listing。

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| C1 | `01_市场产品/互联与Hush_产品范围锁定_v0.1.md` §2.1–2.2 | **Pass** | Listing 指引：「interconnected / whole-home alerts（**同品牌**）；不写消防总线替代」；「有线多站互连主轨 \| **不做**」 |
| C2 | `01_市场产品/PRD-v0.1_大纲.md` | **Pass** | 「同品牌**无线本地**级联跟报」；「**无有线互连主轨**」 |
| C3 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` §B | **Pass** | 「不可写成有线消防总线替代」；「Opt3 有线互联脚 … 一期明确**不做**」 |
| C4 | `超级烟感_UL_EN认证流_标准与资料清单_v0.1.md` §4 #9 + 产品头 | **Pass** | 「与 UL/EN 对『multiple station / interconnect』**有线**要求区分写清」；「**电池型，非 AC 硬线主轨**」 |
| C5 | `超级烟感-Amazon-US-EU-Listing草稿.md` Title / Backend / Bullets | **待补** | ① Bullets **未写**「同品牌无线 whole-home / interconnected」正向句（范围锁 §2.1 Listing 行要求）。② Title 仍 `[Hardwired/Battery TBD]`、Backend 含 `hardwired`——易被读成 AC 硬线/互联，须改为 **Battery** 主轨并加「wireless same-brand alerts；**not** wired AC interconnect」分列。**未见**「已满足有线多站 Listing」等同语 → **非 Fail**。 |
| C6 | 说明书 | **待补** | 未齐（同 M6） |
| C7 | 包装 / 增长终稿 | **待补** | 未齐 |

**本 TC 判定**：**待补**（C1–C4 Pass、0 Fail；C5–C7 待增长/市场按范围锁补无线分列 + 去 Hardwired 歧义）。

---

## 3. 关联禁区抽检（用户指定 · 非独立 TC 号）

### 3.1 禁 Nest 房间剧本 / Heads-Up

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| N1 | 范围锁 §2.2 | **Pass** | 「Nest Heads-Up /『烟雾在×房间』剧本 \| **禁止照搬**」 |
| N2 | PRD · IP 红线 / 互联节 | **Pass** | 「禁复刻 Heads-Up/『烟雾在×房间』类句式」；「禁 Nest Heads-Up/房间名剧本」 |
| N3 | Listing 纪律 / 语音行 | **Pass** | 「不上架；不写 Nest / … / Heads-Up」；禁区列「Nest 房间句式」 |
| N4 | 竞品对齐 §A | **Pass** | 「禁照搬 Nest『烟雾在×房间』剧本」；Heads-Up「**禁止照搬**」 |

### 3.2 禁可静音 CO

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| H1 | 范围锁 §1 | **Pass** | 「**CO** \| **禁止用户静音**」；Listing：「temporary silence for smoke / hush；**不写可静音 CO**」 |
| H2 | PRD · 互联与 Hush | **Pass** | 「**CO 禁止静音**」 |
| H3 | 竞品对齐 §C | **Pass** | 「**CO 禁止 Hush**」；「**不写**可静音 CO」 |
| H4 | Listing §1.2 #4 / A+ #6 | **Pass**（无违规） | 仅「voice muteable; siren/lights not」/「Mute (Siren Stays On)」——**未**写 CO hush。 |
| H5 | Listing 能力层 | **待补** | 范围锁要求正向写「temporary silence for **smoke** / hush」；当前 Listing **未出现** smoke hush 句 → 补一句并明示 CO 不可静音。 |
| H6 | 认证主稿 §4 #4 | **待补** | 写「消音/Hush：若提供…」时长约束，**未点名「仅烟 / CO 禁」**；建议与范围锁对齐补一句（否则送检说明易歧义）。 |

---

## 4. 动作（测试 → 下游）

| 角色 | 动作 |
|---|---|
| **市场产品 / 增长** | Listing：① 去掉/改写 Hardwired TBD → Battery 主轨；② 加同品牌**无线** whole-home 句 + **not wired AC interconnect**；③ 加 smoke hush + **CO cannot be silenced**；④ 保持 Matter ≠ interconnect。关 Issue 能力层与 #7 文案子项。 |
| **市场产品** | G3 前出说明书/包装草稿（认证 §3.4），复跑本抽检。 |
| **认证主稿维护** | §4 #4 回写「Hush 仅烟、CO 禁止静音」。 |
| **测试** | 本文件挂 G3 文案门禁；**0 Fail**；两 TC 标 **待补** 直至 C5/H5 + 说明书闭环。实机 TC-MATTER-BOUND-01（断 Matter 仍级联）**不在本次桌面抽检范围**。 |

---

## 5. 变更

- v0.1：首轮无样机桌面抽检；Pass 13 / Fail 0 / 待补 7；两 TC 均为待补。
