# 超级烟感 · 文档抽检 · TC-CLAIM-BOUND-01 / TC-MATTER-BOUND-02 · v0.1.1

**日期**：2026-10-22 Asia/Shanghai  
**方式**：无样机桌面抽检（仅文案；≠ 实机 G3）  
**依据**：`超级烟感_Hush与级联_可复现测项_v0.1.2.md` §6–§7  
**红线（本抽检强制核对）**：禁 Nest 房间剧本 · 禁 Matter=消防互联 · 禁可静音 CO · 无线≠有线多站  

**抽检范围（已读）**  
| 类型 | 路径 |
|---|---|
| 范围锁 | `01_市场产品/互联与Hush_产品范围锁定_v0.1.md`（文内 v0.1.1） |
| PRD | `01_市场产品/PRD-v0.1_正式版.md` / 大纲 |
| Listing | `01_市场产品/超级烟感-Amazon-US-EU-Listing草稿.md`（**v0.2.2**） |
| 竞品对齐 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` |
| 认证主稿 | `01_市场产品/超级烟感_UL_EN认证流_标准与资料清单_v0.1.md`（§4 #4/#9） |
| 回写说明 | `01_市场产品/CLAIM_Matter_抽检回写_v0.1.md` |
| 测项定义 | `超级烟感_Hush与级联_可复现测项_v0.1.2.md` |

**未齐（影响闭环）**：完整用户手册 / 包装 / 铭牌正文 — 见认证主稿 §3.4 / §3.9；M6/C6 等仍待补。

---

## 0. 汇总

| 用例 | 结论 | Pass | Fail | 待补 |
|---|---|---:|---:|---:|
| **TC-MATTER-BOUND-02** | **待补**（现有稿无违规；说明书/包装未齐） | 5 | 0 | 2 |
| **TC-CLAIM-BOUND-01** | **待补**（C5 已过；说明书/包装未齐） | 5 | 0 | 2 |
| **关联禁区**（Nest 房间 / 可静音 CO） | **Pass**（H5/H6 已回写） | 6 | 0 | 0 |
| **合计（子项）** | — | **16** | **0** | **4** |

> **无 Fail。** G3 文案门禁：**完整说明书 + 包装正文齐前，不得将本两 TC 标为全过**（M6/M7/C6/C7）。

---

## 1. TC-MATTER-BOUND-02 · 禁止「Matter/App = 消防有线互联」声称

**测项要点**（摘自测项 v0.1.2）：抽检 PRD/Listing/说明书/认证包；无「替代有线多站消防互连 / AC interconnect wire」；允许同品牌无线 + Matter 通知**分列**。

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| M1 | `01_市场产品/互联与Hush_产品范围锁定_v0.1.md` §2.1–2.2 | **Pass** | 「Matter \| **仅状态/通知**，**不替代**本地级联」；「Matter = 消防级互联 \| **禁止声称**」 |
| M2 | `01_市场产品/PRD-v0.1_正式版.md` · 互联与 Hush | **Pass** | Matter=状态通知不替代；无有线互连主轨 |
| M3 | Listing v0.2.2 §1.2 #3 | **Pass** | 「Matter-compatible status and notifications … **Not** a fire-system interconnect claim」；「**Matter ≠ wireless cascade**」 |
| M4 | `超级烟感_功能补齐_级联与静音_竞品对齐_v0.1.md` §A/§B | **Pass** | 「**禁止**声称=消防互联」；「Matter 仅作通知补充…**不替代** Opt1」 |
| M5 | 认证主稿 §4 #9 | **Pass** | 「Matter **仅状态/通知**；**不做**有线多站互连消防级声称」 |
| M6 | 说明书 / 用户手册 | **待补** | 尚无完整正文可抽（N-06 仅安装距离草案） |
| M7 | 包装 / 铭牌声称 | **待补** | 无可抽正文 |

**本 TC 判定**：**待补**（M1–M5 全 Pass、0 Fail；M6–M7 未齐 → 不可签 G3 全过）。

---

## 2. TC-CLAIM-BOUND-01 · 无线跟报 ≠ UL/EN 有线多站互连声称

**测项要点**：可写同品牌无线 / whole-home alerts（无线）；**禁止**将无线等同/替代 UL217·EN **有线** multiple-station interconnect 符合性。

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| C1 | 范围锁 §2.1–2.2 | **Pass** | interconnected / whole-home alerts（同品牌）；有线多站主轨不做 |
| C2 | PRD 正式版 | **Pass** | 同品牌无线本地级联；无有线互连主轨 |
| C3 | 竞品对齐 §B | **Pass** | 不可写成有线消防总线替代；一期不做有线互联 |
| C4 | 认证主稿 §4 #9 + 产品头 | **Pass** | 与有线 multiple station 区分；电池型非 AC 硬线主轨 |
| C5 | Listing v0.2.2 Title / Backend / Bullets | **Pass** | Title：**Battery-Powered**；Bullet#4 same-brand **wireless** whole-home + **not** wired AC interconnect；Backend 去 hardwired |
| C6 | 说明书 | **待补** | 完整手册未齐 |
| C7 | 包装 / 增长终稿 | **待补** | 未齐；Listing 不上架 |

**本 TC 判定**：**待补**（C1–C5 Pass、0 Fail；C6–C7 待完整说明书/包装）。

---

## 3. 关联禁区抽检（用户指定 · 非独立 TC 号）

### 3.1 禁 Nest 房间剧本 / Heads-Up

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| N1 | 范围锁 §2.2 | **Pass** | Nest Heads-Up /「烟雾在×房间」禁止照搬 |
| N2 | PRD 正式版 IP 红线 | **Pass** | 禁 Heads-Up/房间名句式；Listing 不点名 Nest |
| N3 | Listing v0.2.2 纪律 / Bullet#5 | **Pass** | 「No Nest-style “smoke in [room]” scripts」；不点名 Nest |
| N4 | 竞品对齐 §A | **Pass** | 禁照搬 Nest 房间剧本 |
| N5 | N-06 §7 CLAIM 红线 | **Pass** | 禁 Heads-Up /「烟雾在×房间」；不点名 Nest |

### 3.2 禁可静音 CO

| # | 文件 | 结果 | 摘句（短） |
|---|---|---|---|
| H1 | 范围锁 §1 | **Pass** | CO 禁止用户静音 |
| H2 | PRD 正式版 | **Pass** | CO 禁止静音 |
| H3 | 竞品对齐 §C | **Pass** | CO 禁止 Hush |
| H4 | Listing 语音/声光 | **Pass** | 声光不可关；未写可静音 CO |
| H5 | Listing 能力层 Bullet#5 | **Pass** | 「Temporary hush for smoke only；**CO cannot be silenced**」 |
| H6 | 认证主稿 §4 #4 | **Pass** | 「消音/Hush：**仅烟雾**…；**CO 禁止用户静音**」 |

**关联禁区判定**：**Pass**（0 Fail / 0 待补）。

---

## 4. 动作（测试 → 下游）

| 角色 | 动作 |
|---|---|
| **市场产品** | C5/H5/H6 已过；继续 G3 前出完整说明书/包装草稿，复跑 M6/C6。 |
| **增长** | Listing 终稿 freeze；不上架直至认证与说明书闭环。 |
| **测试** | 本文件挂 G3 文案门禁；**0 Fail**；两 TC 仍 **待补** 直至说明书/包装闭环。实机 TC-MATTER-BOUND-01 **不在本次范围**。 |

---

## 5. 变更

- v0.1：首轮无样机桌面抽检；Pass 13 / Fail 0 / 待补 7。
- v0.1.1（2026-10-22）：复抽市场产品回写；**C5/H5/H6 → Pass**；合计 Pass **16** / Fail **0** / 待补 **4**（仅说明书/包装）。
