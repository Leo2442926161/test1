# 超级烟感 · G3 钩子验收单 · fw-hush-relay-0.1.0-evt v0.1

| 项 | 值 |
|---|---|
| **结论** | **有条件通过** |
| 角色 | 测试 / DQE |
| 验收时刻 | 2026-10-22 14:35 Asia/Shanghai (CST, UTC+8) |
| 制品版本 | `fw-hush-relay-0.1.0-evt` |
| PR | https://github.com/Leo2442926161/test1/pull/14 |
| 分支 | `cursor/fw-hush-relay-evt-a653` |
| Head SHA | `ba6f34ea36ffe125258aa11be010e8d240042bfd` |
| 工作树 | `/workspace/fw-hush-relay-g3/`（浅克隆 depth=1） |
| 对照测项 | `/workspace/超级烟感_Hush与级联_可复现测项_v0.1.2.md`（仅钩子可复现部分） |
| ICD 基线 | v0.1.7（制品 MANIFEST / JSON `icd_baseline`） |
| 原始日志 | `/workspace/g3-logs/` |

## 0. 结论与条件

**有条件通过**（宿主 sim G3 钩子门禁）。本单**不**等同实机 G3 全过，亦**不**等同 UL/EN 实验室过线。

| 条件 ID | 条件 | 状态 |
|---|---|---|
| C1 | 范围锁定为 **宿主 sim**（`image=host-sim`，`ncs_board_build=false`）；**不是** nRF 烧录镜像 | 已声明并实测 |
| C2 | `TC-CASCADE-PWR-01`（EVT N 台同响 Vbat/电流）**SKIP**，无硬件实测声称 | 已 SKIP |
| C3 | Matter/Claim 文档抽检（Listing/说明书）仍走资料包人工抽检；本包仅 **policy lock** 单测 | 已标明 |
| C4 | 本机构建 `build/fw_sim` SHA 与 PR 自述 Linux 二进制 SHA 可不一致（重编）；以 **源码包 SHA256** 为准 | 已核 |

**Fail 列表**：**无**（钩子场景 0 Fail）。

---

## 1. SHA256 校验

| 文件 | 期望 | 实测 | 结果 |
|---|---|---|---|
| `firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz` | `8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d` | `8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d` | **Pass** |

```text
# 复现
cd /workspace/fw-hush-relay-g3
sha256sum firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz
```

本机重编后 `firmware/build/fw_sim` SHA256 = `372ba0ffbb1f02b07fd4c176564e26a17d90dec8551694951364a48d15d542fa`（≠ PR 自述 `aee80bce…`；属宿主重编差异，不作为退回依据）。

---

## 2. `make -C firmware test`

| 计数 | 值 |
|---|---|
| pass | **28** |
| fail | **0** |
| skip | **1**（`TC-CASCADE-PWR-01`） |
| exit | 0 |
| 日志 | `/workspace/g3-logs/01-make-test.log` |

摘句：

```text
PASS TC-HUSH-01 … PASS TC-HOOKS-JSON
PASS TC-NVS-RELAY-DEFAULT
SKIP TC-CASCADE-PWR-01: EVT hardware Vbat/current; not run and no result claimed
RESULT pass=28 fail=0 skip=1 elapsed_s=0
PASS CLI hooks
```

```text
# 复现
cd /workspace/fw-hush-relay-g3
make -C firmware test
```

---

## 3. CLI 指定序列（PR 钩子）

```sh
cd /workspace/fw-hush-relay-g3/firmware && make
./build/fw_sim --state /tmp/dut.state reset
./build/fw_sim --state /tmp/dut.state smoke
./build/fw_sim --state /tmp/dut.state mute
./build/fw_sim --state /tmp/dut.state status
```

日志：`02-cli-reset.json` … `05-cli-status-hush.json`。

| 步骤 | 关键字段摘句 | 结果 |
|---|---|---|
| reset | `"relay_enable": true`, `"HUSH_SMOKE_S": 540`, `"matter_equals_cascade": false` | Pass |
| smoke | `"alarm_type": "smoke"`, `"horn_on": true`, `"cascade_role": "origin"` | Pass |
| mute | `"hush_active": true`, `"hush_remaining_s": 540`, `"hush_remain_s": 540`, `"horn_on": false`, `"sensing_active": true`, `"ux.mode": "mute_hold"` | Pass |
| status | 同上稳定可读 | Pass |

---

## 4. 强制场景（ICD / 范围锁）

### 4.1 烟 Hush（含 tick 递减）— Pass

- mute 后：`hush_active=true`，`hush_remaining_s=540`（=`HUSH_SMOKE_S`）
- `tick 60` 后：`hush_remaining_s=480` / `hush_remain_s=480`（仍 `hush_active=true`）
- 日志：`05-cli-status-hush.json`，`10-tick-decr.json`

```sh
./build/fw_sim --state /tmp/dut-tick.state reset
./build/fw_sim --state /tmp/dut-tick.state smoke
./build/fw_sim --state /tmp/dut-tick.state mute
./build/fw_sim --state /tmp/dut-tick.state tick 60
./build/fw_sim --state /tmp/dut-tick.state status
```

### 4.2 CO 禁静音 — Pass（CLI + 单测双覆盖）

CLI 有 `co` 命令（**非**「仅单测覆盖 / CLI 缺口」）。

- `co` → `alarm_type=co`, `horn_on=true`
- `mute` → `hush_active=false`, `hush_reject=co_forbidden`, `hush_reject_reason=co_forbidden`, 声光不降（`horn_on=true`, `alarm_light_on=true`）
- 日志：`11-co-mute.json`；单测亦有 `PASS TC-HUSH-CO-01`

```sh
./build/fw_sim --state /tmp/dut-co.state reset
./build/fw_sim --state /tmp/dut-co.state co
./build/fw_sim --state /tmp/dut-co.state mute
./build/fw_sim --state /tmp/dut-co.state status
```

### 4.3 级联钩子 + `relay_enable` 默认 ON — Pass

- reset：`relay_enable=true`（DUT 与 peer）
- `pair` → `smoke` → `hop`：`alarm_relay_tx=1`（type smoke），peer `alarm_relay_rx=1`，peer `alarm_type=smoke`，`horn_on=true`，roles `origin`/`follow`
- 日志：`12-cascade.json`，`15-relay-default.json`

```sh
./build/fw_sim --state /tmp/dut-cascade.state reset
./build/fw_sim --state /tmp/dut-cascade.state pair
./build/fw_sim --state /tmp/dut-cascade.state smoke
./build/fw_sim --state /tmp/dut-cascade.state hop
./build/fw_sim --state /tmp/dut-cascade.state status
```

### 4.4 Matter ≠ 级联 — Pass

- 策略标志：`matter_role=notify_only`，`matter_equals_cascade=false`，`wired_interconnect=false`
- `cloud 0` + `matter 0` 后仍 `hop` 跟报成功（peer 烟声光）
- 日志：`13-matter-bound.json`

```sh
./build/fw_sim --state /tmp/dut-matter.json.state reset
./build/fw_sim --state /tmp/dut-matter.json.state pair
./build/fw_sim --state /tmp/dut-matter.json.state cloud 0
./build/fw_sim --state /tmp/dut-matter.json.state matter 0
./build/fw_sim --state /tmp/dut-matter.json.state smoke
./build/fw_sim --state /tmp/dut-matter.json.state hop
./build/fw_sim --state /tmp/dut-matter.json.state status
```

### 4.5 SED L 字段 — Pass（模型钳位；电流实测 SKIP）

smoke 后 UX 摘句：`applied_brightness=66`, `applied_segments=16`, `limit_action=clamp`, `reject_reason=peak`, `est_continuous_ma=595`, `rf_half=false`。  
电流 / Vbat 实测：**SKIP**（可接受，对齐 C2）。日志：`14-sed.json`。

---

## 5. 测项对照（钩子可复现 ⊂ v0.1.2）

| TC ID | 宿主 sim 钩子 | 本单 |
|---|---|---|
| TC-HUSH-01 | 单测 + CLI mute | Pass |
| TC-HUSH-02 | 单测虚拟 tick（非实睡 9 min） | Pass |
| TC-HUSH-03 | 单测 critical | Pass |
| TC-HUSH-04 | 单测 | Pass |
| TC-HUSH-CO-01 | 单测 + CLI `co`/`mute` | Pass |
| TC-HUSH-MULTI-01 | 单测 peer | Pass |
| TC-CASCADE-01..05 | 单测；CLI hop 抽检 01 | Pass |
| TC-CASCADE-FAIL-01 | 单测 | Pass |
| TC-CASCADE-PWR-01 | 硬件 | **SKIP / N/A** |
| TC-MATTER-BOUND-01 | 单测 + CLI 断云/Matter | Pass |
| TC-MATTER-BOUND-02 / TC-CLAIM-BOUND-01 | 包内 policy lock；Listing 人工另轨 | Pass（包内）/ Listing N/A 本单 |

样机声压 / 多机 RF / 实机烧录：**N/A**（超出宿主 sim 范围）。

---

## 6. 范围声明（必读）

1. **宿主 sim ≠ 实机 G3 全过**；无 nRF Connect SDK 板级构建、无 flash 镜像验收。
2. **TC-CASCADE-PWR-01** 及一切分流计 / Vbat / 声压探针项：**未测、不声称 Pass**。
3. 本结论仅覆盖 PR#14 声明的 ICD v0.1.7 钩子可复现行为。
4. 关闭 Issue #3 / 升实机 G3 前：需实机镜像轨 + EVT 硬件项另开验收单。

---

## 7. 完整复现命令（原样）

```sh
rm -rf /workspace/fw-hush-relay-g3
git clone --branch cursor/fw-hush-relay-evt-a653 --single-branch --depth 1 \
  https://github.com/Leo2442926161/test1.git /workspace/fw-hush-relay-g3
cd /workspace/fw-hush-relay-g3
git rev-parse HEAD   # expect ba6f34ea36ffe125258aa11be010e8d240042bfd
sha256sum firmware/dist/fw-hush-relay-0.1.0-evt-src.tar.gz
# expect 8e0aad9b237b4dfccef37da4ab51ee65ccd5d471ff58a020404cd0c17470ea3d

make -C firmware test

cd firmware
./build/fw_sim --state /tmp/dut.state reset
./build/fw_sim --state /tmp/dut.state smoke
./build/fw_sim --state /tmp/dut.state mute
./build/fw_sim --state /tmp/dut.state status
```

---

## 8. 签字栏

| 角色 | 结论 | 备注 |
|---|---|---|
| 测试/DQE | **有条件通过** | 条件见 §0；Fail=0 |
| 软件 | （待阅） | PR#14 交付方 |
| 结构/硬件 | N/A 本单 | PWR/实机另轨 |

— 报告结束 —
