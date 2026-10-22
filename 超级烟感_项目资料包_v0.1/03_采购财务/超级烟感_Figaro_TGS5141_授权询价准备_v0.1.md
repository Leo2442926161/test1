# 超级烟感 · Figaro/Nemoto 授权询价准备 v0.1

| 字段 | 内容 |
|------|------|
| 日期 | 2026-10-22（Asia/Shanghai） |
| 主询料号 | **Figaro TGS5141-P00** |
| 备选 | Nemoto NAP-508 |
| 对照 | TGS5342-G03（非主路径） |
| 性质 | **仅询价草稿；发询须 Leo 确认；严禁下单付款** |

---

## 一、推荐联系对象（排序）

| 优先级 | 对象 | 地区 | 渠道 | 备注 |
|--------|------|------|------|------|
| 1 | Figaro USA Inc. | 美加墨 | [询价表单](https://figarosensor.com/contact/form.html?type=omitsumori) · +1-847-832-1701 | 官网 RFQ；下拉含 TGS5141 系列相关型号，未列型号可勾 No + Question 自由文本 |
| 2 | 费加罗传感科技（上海） | 中国 | figaro@figaro-china.com · 021-6253-1501 · [报价表](http://www.figaro-china.com/contact/) | **下拉已含 TGS5141-P00**；仅网站公布在售型号可报价 |
| 3 | UNITRONIC GmbH | 德/欧 | info@unitronic.de · +49 2131 752918-0 | Figaro 官方欧洲分销列表 |
| 4 | Omni Sensors & Transmitters | 英 | sales@sensorsandtransmitters.com · 01926 614263 | Figaro 英国官方分销 |
| 备 | Nemoto | 欧/美/中 | eusensor@nemoto.co.jp / nasensor@nemoto.co.jp / sales@sh-nemoto.com | NAP-508 备选；Digi-Key/Mouser 未见稳定 listing |

**公开零星价（非 OEM，不可当量产价）：** SOS Electronic TGS5141-P00 ≈ **EUR 15.10/1**、库存 20（2026-10-22）→ https://www.soselectronic.com/en-us/products/figaro/tgs-5141-p00-232438

---

## 二、附：nRF54L15 / 模组公开价线索（同日）

| 料号 | 渠道 | 单价 | 库存/交期 | 置信度 |
|------|------|------|-----------|--------|
| Raytac **AN54LQ-15** | Digi-Key Marketplace | **USD 4.10 / 1** | 9；约 14 天自 Raytac 发 | 高（页面实价） |
| Nordic **NRF54L15-CAAA-R** | Mouser（聚合摘录） | 1 ≈ **USD 4.15**；10k ≈ **USD 2.31** | 有少量现货报道；DK UK 标 LT **20 Weeks** | 中–高 |
| 对照 MGM240 / MG24 | Silabs 官网 1k 宣传 | 模组约 **USD 3.9–5.3 /1k** | 见已有 MG24 Digi-Key 实价表 | 中 |
| 对照 ESP32-C6-WROOM-1-N4 | Digi-Key | USD 5.10/1；TR650 ≈ 2.85 | 现货；LT 8 Weeks | 高（非主路径） |

硬件粗估模组 $4–7：AN54LQ-15 样机公开价落在区间下沿；**1k 量产价仍须代理/Raytac 年框**。

---

## 三、询价函全文（中英，待 Leo 确认后发出）

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

Application: combo Smoke+CO; target certs UL217+UL2034 / EN14604+EN50291; replaceable CR123A×2.

This is RFQ research only — **no PO, no payment at this time**.

Thank you,  
Leo Xie / Procurement

---

## 四、注意事项

1. 换 CO 料号 = 整机 UL/EN **重认证**；量产料号必须与认证报告一致。  
2. TGS5141 厂商宣称 UL recognized / meets UL2034 & EN50291；EN50291 厂商建议气校（条码免校路径以 Figaro 技术答复为准）。  
3. Digi-Key/Mouser 对 TGS5141 / NAP-508 **无稳定公开货**；SOS €15 级仅为样机参考，远高于硬件 OEM 粗估 $5–12。  
4. Ashlea 上的 TGS5342 零星货仅样机，非量产通道。  
5. **发询前：** Heiman 审稿 → **Leo 确认** → 再提交 Figaro USA 表单 / 发邮件。本文件不授权对外发送。

---

## 五、修订

| 版本 | 日期 | 说明 |
|------|------|------|
| v0.1 | 2026-10-22 | 主询改 TGS5141-P00；附 nRF54L15/Raytac 公开价线索 |
