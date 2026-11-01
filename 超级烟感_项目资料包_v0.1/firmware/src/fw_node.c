#include "fw.h"

#include <string.h>

const char *fw_version(void)
{
    return FW_VERSION;
}

const char *fw_icd_baseline(void)
{
    return FW_ICD_BASELINE;
}

const char *fw_matter_role(void)
{
    return "notify_only";
}

bool fw_matter_equals_cascade(void)
{
    return false;
}

bool fw_wired_interconnect_supported(void)
{
    return false;
}

void fw_node_init(fw_node_t *n)
{
    memset(n, 0, sizeof(*n));
    n->relay_enable = true; /* NVS default ON */
    n->hush_cfg_s = HUSH_SMOKE_S;
    n->sensing_active = true;
    n->voice_enable = true;
    n->ux.mode = FW_UX_IDLE;
    n->ux.led_status_on = true;
    n->ux.led_status_avg_ua = 500;
    n->ux.led_rgb_en = false;
}

static void note_matter(fw_node_t *n)
{
    /* Notify-only. Session state never gates L0 horn or L1 cascade TX. */
    n->matter_notify_attempts++;
    if (n->matter_session) {
        n->matter_notify_sent++;
    }
}

static void relay_on_local_alarm(fw_node_t *n)
{
    if (n->alarm == FW_ALARM_NONE) {
        return;
    }
    if (!n->relay_enable || n->group_id == 0) {
        return;
    }
    if (n->tx_fail) {
        /* Air failure is recorded. L0 horn/light were already committed. */
        n->cascade_fail_count++;
        return;
    }
    n->alarm_relay_tx++;
    n->tx_seq++;
    n->last_tx_type = n->alarm;
}

static void apply_alarm_ux(fw_node_t *n)
{
    fw_ux_mode_t mode = FW_UX_PULSE_SMOKE;
    if (n->alarm == FW_ALARM_CO) {
        mode = FW_UX_PULSE_CO;
    }
    fw_ux_set(n, mode, -1, -1, -1, 0);
}

int fw_set_alarm(fw_node_t *n, fw_alarm_t type)
{
    if (type != FW_ALARM_SMOKE && type != FW_ALARM_CO) {
        return -1;
    }
    /* CO appearance ends a smoke MuteWindow immediately. */
    if (type == FW_ALARM_CO && n->hush_active) {
        n->hush_active = false;
        n->hush_remaining_s = 0;
    }
    bool keep_hush = n->hush_active && type == FW_ALARM_SMOKE;
    n->alarm = type;
    n->condition_present = true;
    n->role = FW_ROLE_ORIGIN;
    n->sensing_active = true;
    n->silenced_by_remote_hush = false;
    if (keep_hush) {
        n->horn_on = false;
        n->full_alarm_light = false;
        fw_ux_set(n, FW_UX_MUTE_HOLD, -1, -1, -1, 0);
    } else {
        n->horn_on = true;
        n->full_alarm_light = true;
        apply_alarm_ux(n);
    }
    note_matter(n);
    relay_on_local_alarm(n);
    /* TX budget can change the peak clamp. Re-apply once tx state is known. */
    if (keep_hush) {
        fw_ux_set(n, FW_UX_MUTE_HOLD, n->ux.applied_window, n->ux.applied_brightness,
                  n->ux.applied_segments, 0);
    } else {
        apply_alarm_ux(n);
    }
    return 0;
}

void fw_clear_alarm(fw_node_t *n)
{
    n->alarm = FW_ALARM_NONE;
    n->condition_present = false;
    n->hush_active = false;
    n->hush_remaining_s = 0;
    n->horn_on = false;
    n->full_alarm_light = false;
    n->role = FW_ROLE_IDLE;
    n->last_tx_type = FW_ALARM_NONE;
    fw_ux_set(n, FW_UX_IDLE, -1, -1, -1, 0);
}

int fw_mute(fw_node_t *n)
{
    if (n->alarm == FW_ALARM_CO) {
        n->hush_reject = FW_HUSH_REJECT_CO_FORBIDDEN;
        /* Horn and alarm light stay where they are. */
        return -1;
    }
    if (n->alarm != FW_ALARM_SMOKE) {
        n->hush_reject = FW_HUSH_REJECT_NO_ALARM;
        return -1;
    }
    n->hush_reject = FW_HUSH_REJECT_NONE;
    n->hush_active = true;
    n->hush_remaining_s = n->hush_cfg_s;
    n->horn_on = false;
    n->full_alarm_light = false;
    n->sensing_active = true;
    /* Local hush must not emit a suppress frame toward peers. */
    fw_ux_set(n, FW_UX_MUTE_HOLD, -1, -1, -1, 0);
    return 0;
}

int fw_critical(fw_node_t *n)
{
    if (n->alarm == FW_ALARM_NONE) {
        return -1;
    }
    n->hush_active = false;
    n->hush_remaining_s = 0;
    n->horn_on = true;
    n->full_alarm_light = true;
    n->sensing_active = true;
    apply_alarm_ux(n);
    return 0;
}

void fw_tick(fw_node_t *n, uint32_t dt_s)
{
    fw_ux_tick(n, dt_s);
    if (!n->hush_active || dt_s == 0) {
        return;
    }
    if (dt_s >= n->hush_remaining_s) {
        n->hush_remaining_s = 0;
        n->hush_active = false;
        if (n->condition_present && n->alarm == FW_ALARM_SMOKE) {
            n->horn_on = true;
            n->full_alarm_light = true;
            apply_alarm_ux(n);
        }
    } else {
        n->hush_remaining_s = (uint16_t)(n->hush_remaining_s - dt_s);
    }
}

int fw_set_hush_smoke_s(fw_node_t *n, int seconds)
{
    if (seconds < HUSH_SMOKE_S_MIN || seconds > HUSH_SMOKE_S_MAX) {
        return -1;
    }
    n->hush_cfg_s = (uint16_t)seconds;
    return 0;
}

void fw_nvs_set_relay_enable(fw_node_t *n, bool enable)
{
    n->relay_enable = enable;
}

void fw_set_cloud_up(fw_node_t *n, bool up)
{
    n->cloud_up = up;
}

void fw_set_matter_session(fw_node_t *n, bool up)
{
    n->matter_session = up;
}

void fw_set_group(fw_node_t *n, uint32_t group_id)
{
    n->group_id = group_id;
}

void fw_set_tx_fail(fw_node_t *n, bool fail)
{
    n->tx_fail = fail;
}

void fw_set_voice_enable(fw_node_t *n, bool enable)
{
    n->voice_enable = enable;
}

int fw_user_disable_horn(fw_node_t *n)
{
    (void)n;
    return -1;
}

int fw_alarm_relay_inject_rx(fw_node_t *dst, fw_alarm_t type, uint32_t src_group)
{
    if (type != FW_ALARM_SMOKE && type != FW_ALARM_CO) {
        return 0;
    }
    if (!dst->relay_enable) {
        return 0;
    }
    if (dst->group_id == 0 || src_group == 0 || dst->group_id != src_group) {
        return 0;
    }
    if (type == FW_ALARM_CO && dst->hush_active) {
        dst->hush_active = false;
        dst->hush_remaining_s = 0;
    }
    dst->alarm_relay_rx++;
    dst->last_rx_type = type;
    dst->alarm = type;
    dst->condition_present = true;
    dst->role = FW_ROLE_FOLLOW;
    dst->sensing_active = true;
    dst->silenced_by_remote_hush = false;
    if (!dst->hush_active) {
        dst->horn_on = true;
        dst->full_alarm_light = true;
        if (type == FW_ALARM_CO) {
            fw_ux_set(dst, FW_UX_PULSE_CO, -1, -1, -1, 0);
        } else {
            fw_ux_set(dst, FW_UX_PULSE_SMOKE, -1, -1, -1, 0);
        }
    }
    return 1;
}

bool fw_hush_active(const fw_node_t *n) { return n->hush_active; }
uint16_t fw_hush_remaining_s(const fw_node_t *n) { return n->hush_remaining_s; }
fw_hush_reject_t fw_hush_reject(const fw_node_t *n) { return n->hush_reject; }
uint16_t fw_hush_cfg_s(const fw_node_t *n) { return n->hush_cfg_s; }
fw_alarm_t fw_alarm(const fw_node_t *n) { return n->alarm; }
bool fw_horn_on(const fw_node_t *n) { return n->horn_on; }
bool fw_full_alarm_light(const fw_node_t *n) { return n->full_alarm_light; }
bool fw_sensing_active(const fw_node_t *n) { return n->sensing_active; }
bool fw_relay_enable(const fw_node_t *n) { return n->relay_enable; }
uint32_t fw_alarm_relay_tx(const fw_node_t *n) { return n->alarm_relay_tx; }
uint32_t fw_alarm_relay_rx(const fw_node_t *n) { return n->alarm_relay_rx; }
uint32_t fw_tx_seq(const fw_node_t *n) { return n->tx_seq; }
uint32_t fw_cascade_fail_count(const fw_node_t *n) { return n->cascade_fail_count; }
uint32_t fw_group(const fw_node_t *n) { return n->group_id; }
fw_alarm_t fw_last_tx_type(const fw_node_t *n) { return n->last_tx_type; }
fw_alarm_t fw_last_rx_type(const fw_node_t *n) { return n->last_rx_type; }
bool fw_silenced_by_remote_hush(const fw_node_t *n) { return n->silenced_by_remote_hush; }
bool fw_cloud_up(const fw_node_t *n) { return n->cloud_up; }
bool fw_matter_session(const fw_node_t *n) { return n->matter_session; }
uint32_t fw_matter_notify_attempts(const fw_node_t *n) { return n->matter_notify_attempts; }
uint32_t fw_matter_notify_sent(const fw_node_t *n) { return n->matter_notify_sent; }
fw_role_t fw_role(const fw_node_t *n) { return n->role; }
const fw_ux_t *fw_ux(const fw_node_t *n) { return &n->ux; }
