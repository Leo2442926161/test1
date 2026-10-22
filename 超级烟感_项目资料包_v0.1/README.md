# 超级烟感 · 项目资料包 v0.1

**日期**：2026-10-22（Asia/Shanghai）  
**用途**：G2 前一站式索引 + 当前 Markdown 交付物快照。  
**不含**：真实 ID 渲染图、原理图网表、CO OEM 回价。

---

## 1. 产品锁定（已定）

| 项 | 结论 |
|---|---|
| 品类 | 高端智能 Smoke + CO |
| 连接 | Matter / Thread（Matter 仅状态与通知，不做互联卖点） |
| SKU | 美欧并行 + 国内差异清单 |
| 定价带 | $79 / **$89 主推** / $99（ADPD 性能档对照） |
| 光学 | **双轨**：量产默认 TI 分立；ADPD 性能备选（$99） |
| SoC | nRF54L15 / Raytac AN54LQ-15 |
| 电源 | 可换 CR123A×2；开盖不断电 + `COVER_DET` 必选 |
| 体验差异化 | 路径光/语音=应达体验目标（非第四卖点）；禁 Nest 可识别环光/剧本/外观 |

---

## 2. 门禁状态

| 门禁 | 状态 | 说明 |
|---|---|---|
| G1 | 已过 | 正式结论包 + 市场简报已归档 |
| G2 | **阻塞** | 卡 **CO OEM 实价**（TGS5141 1k ≤约 $8–9 警戒）；询价由 Leo 本人发送 |
| G3+ | 未开 | 原理图网表冻结前不进详细设计；开模等 G2 定主轨 |

话术/阈值侧（抗误报冻结 v0.1.1）：确认窗 20s、三档、N-03 n=10、文案禁区——G2 话术侧可视为已回写。

---

## 3. 本包文档清单

### 01_市场产品/
- `G1_超级烟感_正式结论_2026-10-22.md`
- `G2_状态_2026-10-22.md`
- `PRD-v0.1_大纲.md`
- `体验与IP约束_路径光语音_2026-10-22.md`
- `路径光语音_自研体验规格_v0.1.md`
- `抗误报_文案与阈值对齐_2026-10-22.md`
- `国内SKU差异清单.md`
- `超级烟感-Amazon-US-EU-Listing草稿.md`（最新稿；暂不上架）

### 02_工程/
- `超级烟感_软件硬件_ICD_v0.1.3.md`（含 Pin 草案逻辑名 + 探针/IP 规避；绝对脚以网表为准）
- `超级烟感_硬件选型采购互检_v0.1.md`
- `超级烟感_原理图正式版说明_v0.1.md`（Issue #4；冻结网表 v0.2.0-freeze）
- `超级烟感_PCB_EVT布局说明_v0.1.md`（Issue #4；EVT 布局准则）
- `超级烟感_绝对坐标草排_v0.1.md`（Issue #4；板心原点草排）
- `超级烟感_RF_EMC预合规计划_v0.1.md`（Issue #10；预扫报告/预约仍空）
- `超级烟感_EVT灯环峰值墙_电流实测规程_v0.1.md`（Issue #13；空表，不填实测数）

### ID概念/
- `正面昼_细环状态点_v0.1.jpg`（概念示意）
- `场景夜间氛围参考_v0.1.jpg`（概念示意）

### 03_采购财务/
- `超级烟感_Figaro_TGS5141_授权询价准备_v0.1.md`（草稿；**非 PO**；发询由 Leo）
- `超级烟感_交期价格表_v0.1.md`

未收入旧版：ICD v0.1 / v0.1.1 / v0.1.2（以 v0.1.3 为准）。

---

## 4. Page / Canvas 链接表（库内）

| 名称 | 链接 |
|---|---|
| 工作流 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/8d6542f7-e298-4236-a8f1-fd0caa516e40 |
| G1 正式包 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/ff6224ee-af3f-4863-8de9-1f3112b90e88 |
| G1 市场简报 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/f9f466d0-8c39-4c64-958d-7044229091b3 |
| 竞品 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/49f3a308-18c6-4de2-8031-cbabc55b7960 |
| 双 SKU | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/460f6f48-6cf4-4493-a9c8-ba72c9a52975 |
| Listing | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/22635420-c2a7-4aa3-bec5-41d2d0a5c2a6 |
| 固件/Matter | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/15e4ea89-5c01-447a-be43-50558a29a0d0 |
| 选型 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/411cbf1d-efe8-4c49-a0fe-0450b0b8c3d8 |
| 交期价格 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/6b85c239-28a6-486c-9547-6340952755c2 |
| 成本毛利 v0.2 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/70a1a422-51a3-422f-9563-b676d2bf2a0c |
| 结构概念约束 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/6605f63c-cb08-4f9b-a39c-55a8f6a2bdd6 |
| 路径光+语音差异化 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/25c95207-d955-4942-bf2c-763b03ceb16f |
| 测试抗误报冻结 v0.1.1 | https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/73207b43-863b-472c-9b3d-5e02815eaaa0 |
| Git 归档 Cloud Agent | https://cursor.com/agents/bc-051d6ebe-6b92-5b1f-bbf5-c0fe2c430086 |

---

## 5. ID 概念（本包内）

- `ID概念/正面昼_细环状态点_v0.1.jpg` — 细环/状态点方向示意（概念，非量产渲染）
- `ID概念/场景夜间氛围参考_v0.1.jpg` — 夜间氛围参考（概念，非量产渲染）

详细语言仍以 Page「路径光+语音差异化」为准。

## 6. 缺口（本包刻意未含 / 未完成）

1. **无量产级 photoreal ID**：仅有概念 JPG + 差异化短页，非开模定稿外观。
2. **原理图未冻**：Pin 草案已进 ICD；绝对坐标等网表 3–5 工作日后再写回。
3. **CO OEM 未回价**：询价函草稿就绪，发询与付款由 Leo；回价前 G2 成本门禁不开。
4. **不上架**：Listing 维持草稿；不写 Nest 名、不做对等宣称。

---

## 7. 下一步（建议顺序）

1. Leo 发送 Figaro / Nemoto RFQ（按 `03_采购财务` 询价准备）。
2. 硬件冻结原理图网表 → ICD Pin 回填。
3. ID：在差异化约束下出细环/状态点概念板（禁 Nest 大环面）。
4. CO 实价回填财务模型 → 复算到仓 → 再开 G2 详细设计。


## 本轮补录（2026-10-22 12:23）
- 路径光语音规格 **v0.2**（对齐 ID 概念板）
- 原理图网表骨架 v0.1
- ID 概念板 Page：https://cursor.com/canvas/store-271d7084-5738-447b-a891-2dbc3bd702b9/39a37403-153a-4ca8-90c2-355a0b5cff0a
