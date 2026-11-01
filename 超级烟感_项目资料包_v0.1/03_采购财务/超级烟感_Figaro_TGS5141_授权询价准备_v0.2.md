# 超级烟感 · Figaro/Nemoto 授权询价准备 v0.2

| 字段 | 内容 |
|------|------|
| 原稿日期 | 2026-10-22（Asia/Shanghai） |
| 入库修订 | 2026-11-01（Asia/Shanghai） |
| 主询料号 | **Figaro TGS5141-P00** |
| 备选 | Nemoto NAP-508 |
| 对照 | TGS5342-G03（非主路径；只问供货状态） |
| 性质 | **仅询价草稿。外发询价仅 Leo。本文件入库不发邮件、不下单、不付款。** |
| 关联 | GitHub [#8](https://github.com/Leo2442926161/test1/issues/8) · [#12](https://github.com/Leo2442926161/test1/issues/12) |
| 回填 | [`超级烟感_CO_OEM缺口回填表_v0.1.md`](超级烟感_CO_OEM缺口回填表_v0.1.md) |
| 声明 | [`超级烟感_RoHS_REACH_UN38.3_声明收集清单_v0.1.md`](超级烟感_RoHS_REACH_UN38.3_声明收集清单_v0.1.md) |

> **无伪造实价。** 下文没有 Figaro / Nemoto 的 OEM 成交价，也没有「已回价」。1k / 5k 真单价只留在缺口回填表，状态为「待 Leo 询价回执」。

---

## 内部红线（勿写入对外询价函）

本 PR **不修改** `超级烟感_成本毛利模型_v0.2.2.md`（财务独占）。下表只引用警戒口径，供回价后对照。

| 项 | 值 | 来源 | 状态 |
|----|-----|------|------|
| **CO 1k 财务警戒** | **约 ≤$7.9@1k** | **财务 v0.2.2，路径光中位** | 现行警戒。**不是 OEM 实价，不是已回价** |
| CO 1k 旧口径 | ≤约 $6.5（路径光低档时原稿曾写可约 ≤$7.5） | 财务 v0.2.1；询价准备 v0.2 原稿 | **作废** |
| 到仓警戒 | ≤ $32 | 财务 v0.2.2（本文件不复核、不改模型） | 背景；勿写入对外函 |
| 路径光 64×RGB | 中位 +$2.50（区间 $1.5–3.5，待采购核价） | 财务 v0.2.2 | 背景；勿写入对外函 |
| OEM 1k / 5k 单价 | （空） | — | **待 Leo 询价回执** |

对外函量档仍是 **10 / 100 / 1,000 / 10,000**。缺口表另留 **5k** 列，收到回执后再填，禁止用公开价或财务占位内插。

---

## 一、推荐联系对象（排序）

| 优先级 | 对象 | 地区 | 渠道 | 备注 |
|--------|------|------|------|------|
| 1 | Figaro USA Inc. | 美加墨 | [询价表单](https://figarosensor.com/contact/form.html?type=omitsumori) · +1-847-832-1701 | 官网 RFQ。**本文件不授权提交表单** |
| 2 | 费加罗传感科技（上海） | 中国 | figaro@figaro-china.com · 021-6253-1501 · [报价表](http://www.figaro-china.com/contact/) | 下拉含 TGS5141-P00。**本文件不授权发信** |
| 3 | UNITRONIC GmbH | 德/欧 | info@unitronic.de · +49 2131 752918-0 | Figaro 欧洲分销 |
| 4 | Omni Sensors & Transmitters | 英 | sales@sensorsandtransmitters.com · 01926 614263 | Figaro 英国分销 |
| 备 | Nemoto | 欧/美/中 | eusensor@nemoto.co.jp / nasensor@nemoto.co.jp / sales@sh-nemoto.com | NAP-508 备选 |

**公开零星价（非 OEM 实价）：** SOS Electronic TGS5141-P00 **€15.10/1 @2026-10-22**。这是样机渠道价，不能写入 1k/5k OEM 单价格。详见缺口回填表与公开渠道对照页。

---

## 二、询价函全文（中英，待 Leo 确认后才可发出）

以下是草稿。**入库 ≠ 已发送。** 只有 Leo 可以外发。本 PR 不发邮件、不提交厂商表单、不下单。

### 中文稿

**主题：** 询价（非 PO）— TGS5141-P00 / 备选 NAP-508 — 智能烟感 Smoke+CO 项目

您好，

我们正在开发面向美欧零售的智能烟感（Smoke+CO，Matter/Thread），请提供以下**仅询价、非采购订单**信息：

1. **主询料号：** Figaro TGS5141-P00  
2. **备选（若贵司代理）：** Nemoto NAP-508  
3. **对照（供货状态即可）：** Figaro TGS5342-G03 是否仍供货  

请分别给出数量档：**样机 10 / 小批 100 / 试产 1,000 / 意向量产 10,000** 的：
- 单价（标明币种、是否含税、有效期）
- 交期 / Lead time、MOQ、SPQ、包装
- 可提供的 **UL 2034 / EN 50291**（及如有 EN54-31）相关组件认可/报告复印件或索取流程、是否需 NDA
- **RoHS / REACH** 符合性声明（或可索取流程与周期）

应用简述：住宅烟感+CO 二合一；目标认证 US UL217+UL2034、EU EN14604+EN50291；电源可换 CR123A×2。

再次确认：本邮件仅为询价调研，**不构成 PO，暂不下单付款**。

谢谢！  
Leo Xie / 采购

---

### English

**Subject:** RFQ only (not a PO) — Figaro TGS5141-P00 / alt. Nemoto NAP-508 — Smart Smoke+CO alarm

Hello,

We are developing a residential Smoke+CO detector (Matter/Thread) for US/EU retail. Please provide a **quotation only (not a purchase order)** for:

1. **Primary:** Figaro TGS5141-P00  
2. **Alternate (if you distribute):** Nemoto NAP-508  
3. **Status check:** Figaro TGS5342-G03 — still available?

Please quote tiers **10 / 100 / 1,000 / 10,000 pcs**:
- Unit price (currency, tax, validity)
- Lead time, MOQ, SPQ, packaging
- Availability of **UL 2034 / EN 50291** (and EN54-31 if any) component recognition / test reports, and NDA requirements
- **RoHS / REACH** declarations (or how/when we can obtain them)

Application: combo Smoke+CO; target certs UL217+UL2034 / EN14604+EN50291; replaceable CR123A×2.

This is RFQ research only — **no PO, no payment at this time**.

Thank you,  
Leo Xie / Procurement

---

## 三、注意事项

1. 换 CO 料号 = 整机 UL/EN **重认证**；量产料号必须与认证报告一致。  
2. Digi-Key/Mouser 对 TGS5141 / NAP-508 **无稳定公开货**；SOS €15.10/1 @2026-10-22 仅为样机参考，**非 OEM 实价**。  
3. **对外函勿写内部财务警戒**（现行约 ≤$7.9@1k，以及已作废的 ≤$6.5）。回价后由采购对照缺口表，财务再复算。本文件不代填回价。  
4. **发询前：** Heiman 审稿（原稿已过）→ **Leo 确认** → 再由 Leo 提交 Figaro USA 表单或发邮件。本文件不授权对外发送。  
5. 厂商营销页上的 “UL recognized / meets UL2034 & EN50291” 只是索取线索，**不是**已收到的报告。UL/EN 列保持空白，直到回执入库。

---

## 四、修订

| 版本 | 日期 | 说明 |
|------|------|------|
| v0.1 | 2026-10-22 | 主询 TGS5141-P00（同目录保留原稿） |
| v0.2 | 2026-10-22 | 原稿对齐当时财务 v0.2.1（CO ≤约 $6.5）；询价函加 RoHS/REACH；关联 #8/#12 |
| v0.2 入库 | 2026-11-01 | 采购准备入库。财务警戒改为 **约 ≤$7.9@1k（财务 v0.2.2，路径光中位）**；旧 ≤$6.5 **作废**。未改成本毛利模型正文。未外发、未下单。真单价仍空白 |

*— 采购 · Figaro/Nemoto 授权询价准备 v0.2 · 入库 2026-11-01 · 无伪造实价 —*
