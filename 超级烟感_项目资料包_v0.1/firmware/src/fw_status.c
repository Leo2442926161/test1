#include "fw.h"

#include <stdarg.h>
#include <stdio.h>

const char *fw_alarm_str(fw_alarm_t alarm)
{
    switch (alarm) {
    case FW_ALARM_SMOKE:
        return "smoke";
    case FW_ALARM_CO:
        return "co";
    case FW_ALARM_NONE:
    default:
        return "none";
    }
}

const char *fw_hush_reject_str(fw_hush_reject_t reason)
{
    switch (reason) {
    case FW_HUSH_REJECT_CO_FORBIDDEN:
        return "co_forbidden";
    case FW_HUSH_REJECT_NO_ALARM:
        return "no_alarm";
    case FW_HUSH_REJECT_NONE:
    default:
        return "none";
    }
}

const char *fw_ux_mode_str(fw_ux_mode_t mode)
{
    switch (mode) {
    case FW_UX_RING_SWEEP:
        return "ring_sweep";
    case FW_UX_PATH_GUIDE:
        return "path_guide";
    case FW_UX_PULSE_SMOKE:
        return "pulse_smoke";
    case FW_UX_PULSE_CO:
        return "pulse_co";
    case FW_UX_PULSE_FAULT:
        return "pulse_fault";
    case FW_UX_MUTE_HOLD:
        return "mute_hold";
    case FW_UX_FULL_WHITE:
        return "full_white";
    case FW_UX_IDLE:
    default:
        return "idle";
    }
}

const char *fw_limit_str(fw_limit_action_t action)
{
    switch (action) {
    case FW_LIMIT_CLAMP:
        return "clamp";
    case FW_LIMIT_REJECT:
        return "reject";
    case FW_LIMIT_STOP:
        return "stop";
    case FW_LIMIT_NONE:
    default:
        return "none";
    }
}

const char *fw_reject_reason_str(fw_reject_reason_t reason)
{
    switch (reason) {
    case FW_REJECT_WINDOW:
        return "window";
    case FW_REJECT_BRIGHTNESS:
        return "brightness";
    case FW_REJECT_COOLDOWN:
        return "cooldown";
    case FW_REJECT_FULL_WHITE:
        return "full_white";
    case FW_REJECT_PEAK:
        return "peak";
    case FW_REJECT_OTHER:
        return "other";
    case FW_REJECT_NONE:
    default:
        return "none";
    }
}

const char *fw_role_str(fw_role_t role)
{
    switch (role) {
    case FW_ROLE_ORIGIN:
        return "origin";
    case FW_ROLE_FOLLOW:
        return "follow";
    case FW_ROLE_IDLE:
    default:
        return "idle";
    }
}

static int append(char **cursor, char *end, const char *fmt, ...)
{
    va_list ap;
    int room;
    int wrote;

    room = (int)(end - *cursor);
    if (room <= 1) {
        return -1;
    }
    va_start(ap, fmt);
    wrote = vsnprintf(*cursor, (size_t)room, fmt, ap);
    va_end(ap);
    if (wrote < 0 || wrote >= room) {
        return -1;
    }
    *cursor += wrote;
    return 0;
}

static const char *json_bool(bool v)
{
    return v ? "true" : "false";
}

static int append_node_hooks(char **cursor, char *end, const fw_node_t *n, bool with_ux)
{
    const fw_ux_t *u = &n->ux;
    if (append(cursor, end,
               "\"hush_active\": %s, \"hush_remaining_s\": %u, \"hush_remain_s\": %u, "
               "\"hush_reject\": \"%s\", \"hush_reject_reason\": \"%s\", "
               "\"alarm_type\": \"%s\", \"horn_on\": %s, \"alarm_light_on\": %s, "
               "\"sensing_active\": %s, \"alarm_relay_tx\": %u, \"alarm_relay_rx\": %u, "
               "\"alarm_relay_tx_type\": \"%s\", \"alarm_relay_rx_type\": \"%s\", "
               "\"relay_enable\": %s, \"cascade_role\": \"%s\", "
               "\"silenced_by_remote_hush\": %s",
               json_bool(n->hush_active), (unsigned)n->hush_remaining_s,
               (unsigned)n->hush_remaining_s, fw_hush_reject_str(n->hush_reject),
               fw_hush_reject_str(n->hush_reject), fw_alarm_str(n->alarm),
               json_bool(n->horn_on), json_bool(n->full_alarm_light),
               json_bool(n->sensing_active), (unsigned)n->alarm_relay_tx,
               (unsigned)n->alarm_relay_rx, fw_alarm_str(n->last_tx_type),
               fw_alarm_str(n->last_rx_type), json_bool(n->relay_enable),
               fw_role_str(n->role), json_bool(n->silenced_by_remote_hush)) != 0) {
        return -1;
    }
    if (!with_ux) {
        return 0;
    }
    return append(cursor, end,
                  ", \"ux\": {\"mode\": \"%s\", \"applied_window\": %u, "
                  "\"applied_brightness\": %u, \"applied_segments\": %u, "
                  "\"led_rgb_en\": %s, \"led_status_on\": %s, \"led_status_avg_ua\": %u, "
                  "\"limit_action\": \"%s\", \"reject_reason\": \"%s\", \"rf_half\": %s, "
                  "\"external_power\": %s, \"sweep_elapsed_s\": %u, "
                  "\"cooldown_remain_s\": %u, \"sweep_period_ms\": %u, "
                  "\"est_continuous_ma\": %u}",
                  fw_ux_mode_str(u->mode), (unsigned)u->applied_window,
                  (unsigned)u->applied_brightness, (unsigned)u->applied_segments,
                  json_bool(u->led_rgb_en), json_bool(u->led_status_on),
                  (unsigned)u->led_status_avg_ua, fw_limit_str(u->limit_action),
                  fw_reject_reason_str(u->reject_reason), json_bool(u->rf_half),
                  json_bool(u->external_power), (unsigned)u->sweep_elapsed_s,
                  (unsigned)u->cooldown_remain_s, (unsigned)u->sweep_period_ms,
                  (unsigned)u->est_continuous_ma);
}

int fw_status_json(const fw_node_t *dut, const fw_node_t *peer, char *buf, size_t buflen)
{
    char *cursor = buf;
    char *end = buf + buflen;

    if (dut == NULL || buf == NULL || buflen < 2) {
        return -1;
    }
    if (append(&cursor, end,
               "{\"version\": \"%s\", \"icd_baseline\": \"%s\", \"image\": \"host-sim\", "
               "\"mcu_target\": \"nRF54L15\", \"ncs_board_build\": false, "
               "\"HUSH_SMOKE_S\": %d, \"hush_cfg_s\": %u, "
               "\"cloud_up\": %s, \"matter_session\": %s, \"matter_role\": \"%s\", "
               "\"matter_equals_cascade\": %s, \"wired_interconnect\": %s, "
               "\"matter_notify_attempts\": %u, \"matter_notify_sent\": %u, "
               "\"cascade_fail_count\": %u, \"voice_enable\": %s, ",
               fw_version(), fw_icd_baseline(), HUSH_SMOKE_S, (unsigned)dut->hush_cfg_s,
               json_bool(dut->cloud_up), json_bool(dut->matter_session), fw_matter_role(),
               json_bool(fw_matter_equals_cascade()),
               json_bool(fw_wired_interconnect_supported()),
               (unsigned)dut->matter_notify_attempts, (unsigned)dut->matter_notify_sent,
               (unsigned)dut->cascade_fail_count, json_bool(dut->voice_enable)) != 0) {
        return -1;
    }
    if (append_node_hooks(&cursor, end, dut, true) != 0) {
        return -1;
    }
    if (peer == NULL) {
        if (append(&cursor, end, ", \"peer\": null") != 0) {
            return -1;
        }
    } else {
        if (append(&cursor, end, ", \"peer\": {") != 0) {
            return -1;
        }
        if (append_node_hooks(&cursor, end, peer, false) != 0) {
            return -1;
        }
        if (append(&cursor, end, "}") != 0) {
            return -1;
        }
    }
    if (append(&cursor, end,
               ", \"peak_continuous_limit_ma\": %d, \"peak_pulse_limit_ma\": %d, "
               "\"l7_brightness_ceiling_pct\": 12}",
               FW_PEAK_CONTINUOUS_MA, FW_PEAK_PULSE_MA) != 0) {
        return -1;
    }
    return 0;
}
