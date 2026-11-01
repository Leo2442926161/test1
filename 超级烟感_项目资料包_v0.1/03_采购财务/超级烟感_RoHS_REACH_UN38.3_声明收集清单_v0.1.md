# 超级烟感 · RoHS / REACH / UN38.3 声明收集清单 v0.1

| 字段 | 内容 |
|------|------|
| 原稿日期 | 2026-10-22（Asia/Shanghai） |
| 入库修订 | 2026-11-01（Asia/Shanghai） |
| 关联 | GitHub [#12](https://github.com/Leo2442926161/test1/issues/12) · CO 询价 [#8](https://github.com/Leo2442926161/test1/issues/8) |
| 性质 | **收集清单。发询 / 索取仅 Leo。本清单入库不对外发送、不下单。** |
| 证书 | **本 PR 没有已收到的声明或报告。** 状态只有「待收」或「不适用」。 |

状态只用三字：**待收** / **已收** / **不适用**。没有文件就不得写成「已收」。

---

## 归档路径

根目录：`超级烟感_项目资料包_v0.1/03_采购财务/declarations/`

约定（收到扫描件或 PDF 后再按此命名放入；现在只有 README 占位）：

| 类别 | 目录 | 文件名 |
|------|------|--------|
| RoHS | `declarations/rohs/` | `<料号>_RoHS_<YYYYMMDD>.pdf` |
| REACH | `declarations/reach/` | `<料号>_REACH_<YYYYMMDD>.pdf` |
| 电芯 UN38.3 | `declarations/un38.3/` | `<电芯型号>_UN38.3_<YYYYMMDD>.pdf` |
| CO 组件 UL/EN 报告 | `declarations/ul-en/` | `<料号>_UL-EN_<YYYYMMDD>.pdf` |

说明见 [`declarations/README.md`](declarations/README.md)。各子目录 README 写明「待归档」。不要把网页摘录、财务占位或本清单本身当成证书。

---

## 一、电池路径（已定方向，报告未到）

| 项 | 内容 |
|----|------|
| 电源 | 可换 **CR123A × 2**（锂金属） |
| 路径 | **含电池出货 / 随附电芯** → 需要 **UN38.3** |
| 品牌 / 型号 | **未定**。不预设 Panasonic / Duracell / Energizer 中的独家料号 |
| UN38.3 文件 | **无**。状态 = 待收。路径预留 `declarations/un38.3/` |
| 若仅空壳出货 | 仍锁定电芯型号与 UN38.3，供配件渠道；在报告到达前不改成不适用 |

---

## 二、关键料声明矩阵

| # | 物料 | 料号/规格 | RoHS | REACH | UN38.3 | 归档路径（文件未到） | 索取对象 | 状态 |
|---|------|-----------|------|-------|--------|----------------------|----------|------|
| 1 | CO 传感器 | TGS5141-P00（主） | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/`；UL/EN 另放 `declarations/ul-en/` | Figaro USA / 上海 / 欧分销（随 CO RFQ 草稿，**尚未发出**） | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 2 | CO 备选 | NAP-508 | 要 | 要 | — | 同上，文件名用 NAP-508 | Nemoto（备选同询，**尚未发出**） | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 3 | 光学 | ADPD188BI，或 TI 分立（具体 PN **未在本清单锁定，不编造**） | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/` | ADI / TI 或代理；主轨定后再索取 | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 4 | 主控模组 | Raytac AN54LQ-15 / nRF54L15 | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/` | Raytac / Nordic 代理 | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 5 | **电芯** | CR123A，品牌待 Leo 定 | 要 | 要 | **要** | RoHS/REACH 同上；UN38.3 → `declarations/un38.3/` | 电芯 OEM / 品牌代理（单独 RFQ，**尚未发出**） | RoHS **待收** · REACH **待收** · UN38.3 **待收** |
| 6 | 路径光 | 64× 可寻址 RGB FPC+LED | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/` | FPC/LED OEM（核价时同要） | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 7 | 注塑导光环 | PC/光学料 | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/` | 结构件厂（开模前） | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |
| 8 | 白壳结构件 | PC/ABS | 要 | 要 | — | `declarations/rohs/`、`declarations/reach/` | 注塑厂（开模前） | RoHS **待收** · REACH **待收** · UN38.3 **不适用** |

关键料至少覆盖：CO 传感器、电芯 CR123A、主控模组、光学。上表 1–5 即这四类（CO 含主备）。6–8 一并列出，避免开模前漏索取。全部 **没有「已收」**。

---

## 三、CR123A 询价要项（模板要点，未发出）

对外索取（非 PO），须 Leo 确认后才发：

1. 型号、化学体系、容量、尺寸、工作温度  
2. **UN38.3** 试验报告（完整件，或可索取流程）  
3. **RoHS / REACH** 声明  
4. 若有：IEC 60086 / 运输包装说明、MSDS  
5. 量档价：10 / 100 / 1k / 10k；MOQ、交期（**本清单不填任何电芯价格**）  
6. 是否适合烟感长寿待机（自放电、脉冲能力简述）

候选方向（待 Leo 定主询，不锁定独家）：Panasonic / Duracell / Energizer / 国产一线且能提供 UN38.3 的 CR123A。以报告为准。

---

## 四、验收对照（Issue #12）

- [ ] 关键料声明收集齐全 — **未完成**。矩阵已列，文件未到，状态均为待收或不适用。  
- [ ] 电池路径明确并归档 UN38.3 — 路径已写明（含电池 / 随附 CR123A×2）。**报告未归档**。  
- [ ] 回执入库后 @测试 + 财务互检 — 未开始。

本页入库只完成「清单 + 空目录约定」，不勾验收。

---

## 五、修订

| 版本 | 日期 | 说明 |
|------|------|------|
| v0.1 | 2026-10-22 | 初稿：电池路径、关键料矩阵、电芯 RFQ 要点 |
| v0.1 入库 | 2026-11-01 | 补归档路径；状态列改为待收/已收/不适用；建立 `declarations/` 占位。无证书文件。未外发 |

*— 采购 · RoHS/REACH/UN38.3 声明收集清单 v0.1 · 入库 2026-11-01 —*
