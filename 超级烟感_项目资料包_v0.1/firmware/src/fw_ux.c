#include "fw.h"

/*
 * Current model uses the life-table design assumptions so SED clamps are
 * executable. It is not a bench measurement.
 *   full-white segment 45 mA, horn 120 mA, cascade TX budget 40 mA.
 * L7 integer brightness ceiling is 12 (spec ≤12.5%, u8 percent cannot store .5).
 */

static bool is_p0_pulse(fw_ux_mode_t mode)
{
    return mode == FW_UX_PULSE_SMOKE || mode == FW_UX_PULSE_CO || mode == FW_UX_PULSE_FAULT;
}

static uint16_t estimate_ma(uint8_t segments, uint8_t brightness, bool horn, bool tx)
{
    uint32_t led = ((uint32_t)segments * 45u * (uint32_t)brightness) / 100u;
    uint32_t ma = led + (horn ? 120u : 0u) + (tx ? 40u : 0u);
    if (ma > 65535u) {
        return 65535u;
    }
    return (uint16_t)ma;
}

static bool relay_tx_budget(const fw_node_t *n)
{
    return n->relay_enable && !n->tx_fail && n->group_id != 0 && n->alarm != FW_ALARM_NONE;
}

static void apply_peak(fw_node_t *n, bool horn, bool factory)
{
    fw_ux_t *u = &n->ux;
    bool tx = relay_tx_budget(n);
    if (factory) {
        u->est_continuous_ma = estimate_ma(u->applied_segments, u->applied_brightness, horn, false);
        u->external_power = true;
        return;
    }
    u->external_power = false;
    u->est_continuous_ma = estimate_ma(u->applied_segments, u->applied_brightness, horn, tx);
    while (u->est_continuous_ma > FW_PEAK_CONTINUOUS_MA) {
        if (u->applied_brightness > 10) {
            u->applied_brightness--;
        } else if (u->applied_segments > 1) {
            u->applied_segments--;
        } else {
            break;
        }
        u->limit_action = FW_LIMIT_CLAMP;
        u->reject_reason = FW_REJECT_PEAK;
        u->est_continuous_ma = estimate_ma(u->applied_segments, u->applied_brightness, horn, tx);
    }
}

int fw_ux_set(fw_node_t *n, fw_ux_mode_t mode, int window, int brightness, int segments,
              uint32_t flags)
{
    fw_ux_t *u = &n->ux;
    bool factory = (flags & FW_UX_FLAG_FACTORY) != 0;
    bool rf = (flags & FW_UX_FLAG_RF_PEAK) != 0;

    if (mode == FW_UX_FULL_WHITE && !factory) {
        u->limit_action = FW_LIMIT_REJECT;
        u->reject_reason = FW_REJECT_FULL_WHITE;
        u->external_power = false;
        return -1;
    }
    if (mode == FW_UX_RING_SWEEP && u->cooldown_remain_s > 0 && !factory) {
        u->limit_action = FW_LIMIT_REJECT;
        u->reject_reason = FW_REJECT_COOLDOWN;
        return -1;
    }

    if (mode == FW_UX_IDLE) {
        uint16_t cool = u->cooldown_remain_s;
        uint16_t swept = u->sweep_elapsed_s;
        u->mode = FW_UX_IDLE;
        u->applied_window = 0;
        u->applied_brightness = 0;
        u->applied_segments = 0;
        u->led_rgb_en = false;
        u->led_status_on = true;
        u->led_status_avg_ua = 500;
        u->limit_action = FW_LIMIT_NONE;
        u->reject_reason = FW_REJECT_NONE;
        u->rf_half = false;
        u->external_power = false;
        u->sweep_period_ms = 0;
        u->est_continuous_ma = 0;
        u->cooldown_remain_s = cool;
        u->sweep_elapsed_s = swept;
        return 0;
    }

    int def_w = 8;
    int def_b = 25;
    int def_s = 8;
    int cap_s = 8;
    int cap_b = 25;
    bool en = true;
    uint16_t period = 1200;

    if (mode == FW_UX_MUTE_HOLD) {
        def_w = 6;
        def_b = 20;
        def_s = 4;
        cap_s = 8;
        cap_b = 25;
    } else if (is_p0_pulse(mode)) {
        def_w = 0;
        def_b = 70;
        def_s = 16;
        cap_s = 16;
        cap_b = 70;
        period = 0;
    } else if (mode == FW_UX_FULL_WHITE) {
        def_w = 64;
        def_b = 100;
        def_s = 64;
        cap_s = 64;
        cap_b = 100;
        period = 0;
    }

    if (window < 0) {
        window = def_w;
    }
    if (brightness < 0) {
        brightness = def_b;
    }
    if (segments < 0) {
        segments = def_s;
    }

    bool clamped = false;
    if (is_p0_pulse(mode)) {
        if (segments > cap_s) {
            segments = cap_s;
            clamped = true;
        }
        if (brightness > cap_b) {
            brightness = cap_b;
            clamped = true;
        }
        window = 0;
    } else if (mode != FW_UX_FULL_WHITE) {
        if (window > 8) {
            window = 8;
            clamped = true;
        }
        if (window < 0) {
            window = 0;
        }
        if (segments > window) {
            segments = window;
            clamped = true;
        }
        if (segments > cap_s) {
            segments = cap_s;
            clamped = true;
        }
        if (brightness > cap_b) {
            brightness = cap_b;
            clamped = true;
        }
    }

    u->rf_half = false;
    if (rf && !is_p0_pulse(mode) && mode != FW_UX_FULL_WHITE) {
        u->rf_half = true;
        if (brightness > 12) {
            brightness = 12;
            clamped = true;
        }
        /* Window stays on the L1 cap. L7 does not halve the window. */
        if (window > 8) {
            window = 8;
            clamped = true;
        }
    }

    if (segments < 0) {
        segments = 0;
    }
    if (brightness < 0) {
        brightness = 0;
    }
    if (segments > 255) {
        segments = 255;
    }
    if (brightness > 255) {
        brightness = 255;
    }
    if (window > 255) {
        window = 255;
    }

    bool entering_sweep = (mode == FW_UX_RING_SWEEP && u->mode != FW_UX_RING_SWEEP);
    u->mode = mode;
    u->applied_window = (uint8_t)window;
    u->applied_brightness = (uint8_t)brightness;
    u->applied_segments = (uint8_t)segments;
    u->led_rgb_en = en;
    u->led_status_on = true;
    u->led_status_avg_ua = 0;
    u->sweep_period_ms = period;
    u->limit_action = clamped ? FW_LIMIT_CLAMP : FW_LIMIT_NONE;
    u->reject_reason = FW_REJECT_NONE;
    if (entering_sweep) {
        u->sweep_elapsed_s = 0;
    }
    apply_peak(n, n->horn_on, factory && mode == FW_UX_FULL_WHITE);
    return 0;
}

void fw_ux_tick(fw_node_t *n, uint32_t dt_s)
{
    fw_ux_t *u = &n->ux;
    if (dt_s == 0) {
        return;
    }
    if (u->mode == FW_UX_RING_SWEEP) {
        uint32_t elapsed = (uint32_t)u->sweep_elapsed_s + dt_s;
        if (elapsed > FW_L3_SWEEP_MAX_S) {
            u->mode = FW_UX_IDLE;
            u->applied_window = 0;
            u->applied_brightness = 0;
            u->applied_segments = 0;
            u->led_rgb_en = false;
            u->led_status_on = true;
            u->led_status_avg_ua = 500;
            u->rf_half = false;
            u->external_power = false;
            u->sweep_period_ms = 0;
            u->est_continuous_ma = 0;
            u->sweep_elapsed_s = FW_L3_SWEEP_MAX_S;
            u->cooldown_remain_s = FW_L3_COOLDOWN_S;
            u->limit_action = FW_LIMIT_STOP;
            u->reject_reason = FW_REJECT_NONE;
            return;
        }
        u->sweep_elapsed_s = (uint16_t)elapsed;
        return;
    }
    if (u->cooldown_remain_s > 0) {
        if (dt_s >= u->cooldown_remain_s) {
            u->cooldown_remain_s = 0;
        } else {
            u->cooldown_remain_s = (uint16_t)(u->cooldown_remain_s - dt_s);
        }
    }
}

int fw_ux_push_frame(const fw_node_t *n)
{
    if (!n->ux.led_rgb_en) {
        return -1;
    }
    return 0;
}
