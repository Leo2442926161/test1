#include "fw.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static int g_pass = 0;
static int g_fail = 0;
static int g_skip = 0;
static char g_why[320];

static void pass(const char *id)
{
    printf("PASS %s\n", id);
    g_pass++;
}

static void fail(const char *id, const char *msg)
{
    fprintf(stderr, "FAIL %s: %s\n", id, msg);
    g_fail++;
}

static void skip(const char *id, const char *msg)
{
    printf("SKIP %s: %s\n", id, msg);
    g_skip++;
}

static int policy_has_line(const char *want)
{
    FILE *fp = fopen("policy/claims.txt", "r");
    char line[256];
    if (fp == NULL) {
        return 0;
    }
    while (fgets(line, sizeof line, fp) != NULL) {
        size_t n = strlen(line);
        while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r')) {
            line[--n] = '\0';
        }
        if (strcmp(line, want) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

static void pair(fw_node_t *a, fw_node_t *b)
{
    fw_node_init(a);
    fw_node_init(b);
    fw_set_group(a, 1);
    fw_set_group(b, 1);
    fw_set_cloud_up(a, false);
    fw_set_matter_session(a, false);
}

static void tc_hush_01(void)
{
    fw_node_t n;
    fw_node_init(&n);
    if (fw_set_alarm(&n, FW_ALARM_SMOKE) != 0 || !fw_horn_on(&n) || !fw_sensing_active(&n)) {
        fail("TC-HUSH-01", "smoke alarm did not start horn/sensing");
        return;
    }
    if (fw_ux(&n)->mode != FW_UX_PULSE_SMOKE) {
        fail("TC-HUSH-01", "expected pulse_smoke before hush");
        return;
    }
    if (fw_mute(&n) != 0 || !fw_hush_active(&n) || fw_hush_remaining_s(&n) != HUSH_SMOKE_S) {
        snprintf(g_why, sizeof g_why, "mute window remaining=%u active=%d",
                 (unsigned)fw_hush_remaining_s(&n), fw_hush_active(&n));
        fail("TC-HUSH-01", g_why);
        return;
    }
    if (fw_horn_on(&n) || !fw_sensing_active(&n) || fw_ux(&n)->mode != FW_UX_MUTE_HOLD) {
        fail("TC-HUSH-01", "hush did not silence horn into MuteHold while sensing stayed up");
        return;
    }
    if (fw_hush_reject(&n) != FW_HUSH_REJECT_NONE || fw_alarm(&n) != FW_ALARM_SMOKE) {
        fail("TC-HUSH-01", "alarm type or reject reason drifted");
        return;
    }
    pass("TC-HUSH-01");
}

static void tc_hush_02(void)
{
    fw_node_t n;
    fw_node_init(&n);
    fw_set_cloud_up(&n, false);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_mute(&n);
    fw_tick(&n, 510); /* 8.5 min */
    if (!fw_hush_active(&n) || fw_hush_remaining_s(&n) != 30) {
        snprintf(g_why, sizeof g_why, "at 8.5 min active=%d remaining=%u", fw_hush_active(&n),
                 (unsigned)fw_hush_remaining_s(&n));
        fail("TC-HUSH-02", g_why);
        return;
    }
    fw_tick(&n, 30);
    if (fw_hush_active(&n) || !fw_horn_on(&n) || fw_ux(&n)->mode != FW_UX_PULSE_SMOKE) {
        fail("TC-HUSH-02", "window did not restore smoke horn at 540 s");
        return;
    }
    if (fw_cloud_up(&n)) {
        fail("TC-HUSH-02", "restore depended on cloud");
        return;
    }
    pass("TC-HUSH-02");
}

static void tc_hush_03(void)
{
    fw_node_t n;
    fw_node_init(&n);
    fw_set_cloud_up(&n, false);
    fw_set_matter_session(&n, false);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_mute(&n);
    if (fw_critical(&n) != 0 || fw_hush_active(&n) || !fw_horn_on(&n) || !fw_full_alarm_light(&n)) {
        fail("TC-HUSH-03", "critical did not end hush locally");
        return;
    }

    fw_node_init(&n);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_mute(&n);
    fw_set_alarm(&n, FW_ALARM_CO);
    if (fw_hush_active(&n) || fw_alarm(&n) != FW_ALARM_CO || !fw_horn_on(&n)) {
        fail("TC-HUSH-03", "CO appearance did not end the smoke hush");
        return;
    }
    pass("TC-HUSH-03");
}

static void tc_hush_04(void)
{
    fw_node_t n;
    fw_node_init(&n);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    if (fw_user_disable_horn(&n) == 0 || !fw_horn_on(&n)) {
        fail("TC-HUSH-04", "permanent horn disable succeeded");
        return;
    }
    fw_set_voice_enable(&n, false);
    if (!fw_horn_on(&n)) {
        fail("TC-HUSH-04", "voice off killed the horn");
        return;
    }
    fw_mute(&n);
    fw_tick(&n, HUSH_SMOKE_S);
    if (!fw_horn_on(&n) || fw_hush_active(&n) || !fw_sensing_active(&n)) {
        fail("TC-HUSH-04", "hush did not stay a temporary window");
        return;
    }
    pass("TC-HUSH-04");
}

static void tc_hush_co(void)
{
    fw_node_t n;
    fw_node_init(&n);
    fw_set_alarm(&n, FW_ALARM_CO);
    if (!fw_horn_on(&n) || fw_ux(&n)->mode != FW_UX_PULSE_CO) {
        fail("TC-HUSH-CO-01", "CO alarm pattern did not start");
        return;
    }
    if (fw_mute(&n) == 0 || fw_hush_active(&n) || fw_hush_reject(&n) != FW_HUSH_REJECT_CO_FORBIDDEN) {
        fail("TC-HUSH-CO-01", "CO mute was not rejected as co_forbidden");
        return;
    }
    if (!fw_horn_on(&n) || !fw_full_alarm_light(&n) || fw_ux(&n)->mode != FW_UX_PULSE_CO) {
        fail("TC-HUSH-CO-01", "CO horn/light dropped after rejected hush");
        return;
    }
    pass("TC-HUSH-CO-01");
}

static void tc_hush_multi(void)
{
    fw_node_t a, b;
    uint32_t tx;
    pair(&a, &b);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_alarm_relay_inject_rx(&b, fw_last_tx_type(&a), fw_group(&a)) != 1 || !fw_horn_on(&b)) {
        fail("TC-HUSH-MULTI-01", "peer did not follow before hush");
        return;
    }
    tx = fw_alarm_relay_tx(&a);
    if (fw_mute(&a) != 0 || fw_horn_on(&a) || !fw_horn_on(&b) || fw_silenced_by_remote_hush(&b)) {
        fail("TC-HUSH-MULTI-01", "origin hush changed the peer");
        return;
    }
    if (fw_alarm_relay_tx(&a) != tx || fw_alarm(&b) != FW_ALARM_SMOKE) {
        fail("TC-HUSH-MULTI-01", "hush emitted a cascade frame or retargeted the peer");
        return;
    }
    pass("TC-HUSH-MULTI-01");
}

static void tc_cascade_01(void)
{
    fw_node_t a, b;
    pair(&a, &b);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_alarm_relay_tx(&a) < 1 || fw_last_tx_type(&a) != FW_ALARM_SMOKE) {
        fail("TC-CASCADE-01", "origin did not emit a smoke relay frame");
        return;
    }
    if (fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, 1) != 1 || fw_alarm(&b) != FW_ALARM_SMOKE ||
        !fw_horn_on(&b) || fw_ux(&b)->mode != FW_UX_PULSE_SMOKE || fw_role(&b) != FW_ROLE_FOLLOW) {
        fail("TC-CASCADE-01", "peer did not annunciate smoke locally");
        return;
    }
    if (fw_alarm_relay_rx(&b) < 1) {
        fail("TC-CASCADE-01", "alarm_relay_rx did not increment");
        return;
    }
    pass("TC-CASCADE-01");
}

static void tc_cascade_02(void)
{
    fw_node_t a, b;
    pair(&a, &b);
    fw_set_alarm(&a, FW_ALARM_CO);
    fw_alarm_relay_inject_rx(&b, fw_last_tx_type(&a), fw_group(&a));
    if (fw_alarm(&b) != FW_ALARM_CO || fw_ux(&b)->mode != FW_UX_PULSE_CO || !fw_horn_on(&b)) {
        fail("TC-CASCADE-02", "peer did not follow CO");
        return;
    }
    if (fw_mute(&b) == 0 || fw_hush_reject(&b) != FW_HUSH_REJECT_CO_FORBIDDEN || !fw_horn_on(&b)) {
        fail("TC-CASCADE-02", "follow CO opened a hush window");
        return;
    }
    pass("TC-CASCADE-02");
}

static void tc_cascade_03(void)
{
    fw_node_t a, b;
    pair(&a, &b);
    fw_set_cloud_up(&a, false);
    fw_set_matter_session(&a, false);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_cloud_up(&a) || fw_matter_session(&a) || fw_alarm_relay_tx(&a) < 1) {
        fail("TC-CASCADE-03", "offline origin did not transmit");
        return;
    }
    if (fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, fw_group(&a)) != 1 || !fw_horn_on(&b)) {
        fail("TC-CASCADE-03", "offline peer did not follow");
        return;
    }
    pass("TC-CASCADE-03");
}

static void tc_cascade_04(void)
{
    fw_node_t a, b;
    pair(&a, &b);
    fw_nvs_set_relay_enable(&a, false);
    if (fw_relay_enable(&a) || fw_set_alarm(&a, FW_ALARM_SMOKE) != 0 || !fw_horn_on(&a) ||
        fw_alarm_relay_tx(&a) != 0) {
        fail("TC-CASCADE-04", "relay_enable=0 still transmitted or dropped local horn");
        return;
    }
    fw_clear_alarm(&a);
    fw_nvs_set_relay_enable(&a, true);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_alarm_relay_tx(&a) < 1 || fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, 1) != 1 ||
        !fw_horn_on(&b)) {
        fail("TC-CASCADE-04", "re-enable did not restore follow");
        return;
    }
    fw_node_init(&b);
    fw_set_group(&b, 1);
    fw_nvs_set_relay_enable(&b, false);
    if (fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, 1) != 0 || fw_alarm(&b) != FW_ALARM_NONE) {
        fail("TC-CASCADE-04", "peer relay_enable=0 still followed");
        return;
    }
    pass("TC-CASCADE-04");
}

static void tc_cascade_05(void)
{
    fw_node_t a, c;
    fw_node_init(&a);
    fw_node_init(&c);
    fw_set_group(&a, 1);
    fw_set_group(&c, 2);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_alarm_relay_inject_rx(&c, FW_ALARM_SMOKE, 1) != 0 || fw_alarm(&c) != FW_ALARM_NONE ||
        fw_horn_on(&c) || fw_alarm_relay_rx(&c) != 0) {
        fail("TC-CASCADE-05", "foreign group followed");
        return;
    }
    fw_set_group(&c, 0);
    if (fw_alarm_relay_inject_rx(&c, FW_ALARM_SMOKE, 1) != 0 || fw_alarm(&c) != FW_ALARM_NONE) {
        fail("TC-CASCADE-05", "unpaired node followed");
        return;
    }
    pass("TC-CASCADE-05");
}

static void tc_cascade_fail(void)
{
    fw_node_t a;
    fw_node_init(&a);
    fw_set_group(&a, 1);
    fw_set_tx_fail(&a, true);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (!fw_horn_on(&a) || !fw_full_alarm_light(&a) || fw_ux(&a)->led_rgb_en == false ||
        fw_ux(&a)->applied_segments < 1 || fw_alarm_relay_tx(&a) != 0 ||
        fw_cascade_fail_count(&a) < 1) {
        snprintf(g_why, sizeof g_why, "horn=%d tx=%u fail=%u segs=%u", fw_horn_on(&a),
                 (unsigned)fw_alarm_relay_tx(&a), (unsigned)fw_cascade_fail_count(&a),
                 (unsigned)fw_ux(&a)->applied_segments);
        fail("TC-CASCADE-FAIL-01", g_why);
        return;
    }
    pass("TC-CASCADE-FAIL-01");
}

static void tc_matter_01(void)
{
    fw_node_t a, b;
    pair(&a, &b);
    fw_set_matter_session(&a, false);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_matter_notify_attempts(&a) < 1 || fw_matter_notify_sent(&a) != 0) {
        fail("TC-MATTER-BOUND-01", "notify accounting did not treat Matter as optional");
        return;
    }
    if (fw_alarm_relay_tx(&a) < 1 || fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, 1) != 1) {
        fail("TC-MATTER-BOUND-01", "cascade required a Matter session");
        return;
    }
    if (fw_matter_equals_cascade() || strcmp(fw_matter_role(), "notify_only") != 0) {
        fail("TC-MATTER-BOUND-01", "Matter role is not notify-only");
        return;
    }
    pass("TC-MATTER-BOUND-01");
}

static void tc_matter_02(void)
{
    if (fw_wired_interconnect_supported() || fw_matter_equals_cascade()) {
        fail("TC-MATTER-BOUND-02", "firmware claims wired or Matter interconnect");
        return;
    }
    if (!policy_has_line("matter_role=notify_only") ||
        !policy_has_line("matter_equals_cascade=false") ||
        !policy_has_line("wired_interconnect=false") ||
        !policy_has_line("ac_interconnect_wire_claim=false")) {
        fail("TC-MATTER-BOUND-02", "package policy lock missing (listing pack is not this PR)");
        return;
    }
    pass("TC-MATTER-BOUND-02 (package policy lock; listing pack is manual)");
}

static void tc_claim(void)
{
    if (!policy_has_line("ul_en_wired_multiple_station_claim=false") ||
        !policy_has_line("wired_interconnect=false")) {
        fail("TC-CLAIM-BOUND-01", "package still claims wired multiple-station compliance");
        return;
    }
    if (fw_wired_interconnect_supported()) {
        fail("TC-CLAIM-BOUND-01", "wired interconnect flag is set");
        return;
    }
    pass("TC-CLAIM-BOUND-01 (package policy lock; listing pack is manual)");
}

static void tc_l1(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    if (fw_ux_set(&n, FW_UX_PATH_GUIDE, 16, 25, 16, 0) != 0) {
        fail("TC-L1", "path guide rejected");
        return;
    }
    u = fw_ux(&n);
    if (u->limit_action != FW_LIMIT_CLAMP || u->applied_window > 8 || u->applied_segments > 8 ||
        u->applied_segments > u->applied_window) {
        snprintf(g_why, sizeof g_why, "window=%u segs=%u action=%s", u->applied_window,
                 u->applied_segments, fw_limit_str(u->limit_action));
        fail("TC-L1", g_why);
        return;
    }
    pass("TC-L1");
}

static void tc_l2(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    if (fw_ux_set(&n, FW_UX_RING_SWEEP, 8, 50, 8, 0) != 0) {
        fail("TC-L2", "ring sweep rejected");
        return;
    }
    u = fw_ux(&n);
    if (u->limit_action != FW_LIMIT_CLAMP || u->applied_brightness > 25 || u->sweep_period_ms < 1000 ||
        u->sweep_period_ms > 1500) {
        snprintf(g_why, sizeof g_why, "brightness=%u period=%u", u->applied_brightness,
                 u->sweep_period_ms);
        fail("TC-L2", g_why);
        return;
    }
    pass("TC-L2");
}

static void tc_l3(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    fw_ux_set(&n, FW_UX_RING_SWEEP, 8, 25, 8, 0);
    fw_ux_tick(&n, 60);
    u = fw_ux(&n);
    if (u->mode != FW_UX_RING_SWEEP || u->sweep_elapsed_s != 60 || !u->led_rgb_en) {
        fail("TC-L3", "sweep stopped before 60 s");
        return;
    }
    fw_ux_tick(&n, 1);
    u = fw_ux(&n);
    if (u->mode != FW_UX_IDLE || u->led_rgb_en || u->limit_action != FW_LIMIT_STOP ||
        u->cooldown_remain_s < 300) {
        fail("TC-L3", "sweep did not stop and drop LED_RGB_EN after 60 s");
        return;
    }
    if (fw_ux_set(&n, FW_UX_RING_SWEEP, 8, 25, 8, 0) != -1 ||
        fw_ux(&n)->limit_action != FW_LIMIT_REJECT ||
        fw_ux(&n)->reject_reason != FW_REJECT_COOLDOWN || fw_ux(&n)->led_rgb_en) {
        fail("TC-L3", "cooldown re-entry was not rejected");
        return;
    }
    fw_ux_tick(&n, 300);
    if (fw_ux_set(&n, FW_UX_RING_SWEEP, 8, 25, 8, 0) != 0 || fw_ux(&n)->mode != FW_UX_RING_SWEEP) {
        fail("TC-L3", "sweep stayed rejected after cooldown");
        return;
    }
    pass("TC-L3");
}

static void tc_l4(void)
{
    fw_node_t n;
    fw_node_init(&n);
    fw_ux_set(&n, FW_UX_PATH_GUIDE, 8, 25, 8, 0);
    if (!fw_ux(&n)->led_rgb_en) {
        fail("TC-L4", "path guide left EN off");
        return;
    }
    if (fw_ux_set(&n, FW_UX_IDLE, 8, 100, 64, 0) != 0 || fw_ux(&n)->led_rgb_en ||
        !fw_ux(&n)->led_status_on || fw_ux(&n)->led_status_avg_ua > 500) {
        fail("TC-L4", "idle did not force LED_RGB_EN=0");
        return;
    }
    if (fw_ux_push_frame(&n) == 0) {
        fail("TC-L4", "idle accepted an RGB frame");
        return;
    }
    pass("TC-L4");
}

static void tc_l5(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    if (fw_ux_set(&n, FW_UX_PULSE_SMOKE, 0, 100, 64, 0) != 0) {
        fail("TC-L5", "smoke pulse rejected");
        return;
    }
    u = fw_ux(&n);
    if (u->limit_action != FW_LIMIT_CLAMP || u->applied_segments > 16 || u->applied_segments < 1 ||
        u->applied_brightness > 70 || !u->led_rgb_en || !fw_horn_on(&n)) {
        snprintf(g_why, sizeof g_why, "segs=%u bri=%u horn=%d en=%d", u->applied_segments,
                 u->applied_brightness, fw_horn_on(&n), u->led_rgb_en);
        fail("TC-L5", g_why);
        return;
    }
    pass("TC-L5");
}

static void tc_l6(void)
{
    fw_node_t n;
    fw_node_init(&n);
    if (fw_ux_set(&n, FW_UX_FULL_WHITE, 64, 100, 64, 0) != -1 ||
        fw_ux(&n)->limit_action != FW_LIMIT_REJECT ||
        fw_ux(&n)->reject_reason != FW_REJECT_FULL_WHITE || fw_ux(&n)->mode != FW_UX_IDLE ||
        fw_ux(&n)->led_rgb_en) {
        fail("TC-L6", "full-white on battery was accepted");
        return;
    }
    if (fw_ux_set(&n, FW_UX_FULL_WHITE, 64, 100, 64, FW_UX_FLAG_FACTORY) != 0 ||
        fw_ux(&n)->mode != FW_UX_FULL_WHITE || fw_ux(&n)->applied_segments != 64 ||
        !fw_ux(&n)->external_power) {
        fail("TC-L6", "factory external-power full-white was rejected");
        return;
    }
    pass("TC-L6");
}

static void tc_l7(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    if (fw_ux_set(&n, FW_UX_PATH_GUIDE, 16, 25, 16, FW_UX_FLAG_RF_PEAK) != 0) {
        fail("TC-L7", "rf path guide rejected");
        return;
    }
    u = fw_ux(&n);
    if (!u->rf_half || u->applied_brightness > 12 || u->applied_window > 8 || u->applied_window < 4) {
        snprintf(g_why, sizeof g_why, "rf=%d bri=%u win=%u", u->rf_half, u->applied_brightness,
                 u->applied_window);
        fail("TC-L7", g_why);
        return;
    }
    fw_node_init(&n);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_ux_set(&n, FW_UX_PULSE_SMOKE, 0, 100, 64, FW_UX_FLAG_RF_PEAK);
    u = fw_ux(&n);
    if (u->rf_half || u->applied_brightness <= 12 || u->applied_brightness > 70 ||
        u->applied_segments > 16 || !fw_horn_on(&n)) {
        snprintf(g_why, sizeof g_why, "p0 rf=%d bri=%u segs=%u", u->rf_half, u->applied_brightness,
                 u->applied_segments);
        fail("TC-L7", g_why);
        return;
    }
    pass("TC-L7");
}

static void tc_l8(void)
{
    fw_node_t on, off;
    fw_node_init(&on);
    fw_node_init(&off);
    fw_set_voice_enable(&on, true);
    fw_set_voice_enable(&off, false);
    fw_ux_set(&on, FW_UX_PATH_GUIDE, 16, 50, 16, 0);
    fw_ux_set(&off, FW_UX_PATH_GUIDE, 16, 50, 16, 0);
    if (fw_ux(&on)->applied_window != fw_ux(&off)->applied_window ||
        fw_ux(&on)->applied_brightness != fw_ux(&off)->applied_brightness ||
        fw_ux(&on)->applied_segments != fw_ux(&off)->applied_segments) {
        fail("TC-L8", "voice changed the light clamp");
        return;
    }
    fw_set_alarm(&on, FW_ALARM_SMOKE);
    fw_set_alarm(&off, FW_ALARM_SMOKE);
    if (!fw_horn_on(&off) || fw_ux(&on)->applied_segments != fw_ux(&off)->applied_segments ||
        fw_ux(&on)->applied_brightness != fw_ux(&off)->applied_brightness) {
        fail("TC-L8", "voice changed smoke annunciation limits");
        return;
    }
    pass("TC-L8");
}

static void tc_pk(void)
{
    fw_node_t n;
    const fw_ux_t *u;
    fw_node_init(&n);
    fw_set_group(&n, 1);
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_ux_set(&n, FW_UX_PULSE_SMOKE, 0, 100, 64, 0);
    u = fw_ux(&n);
    if (u->est_continuous_ma > FW_PEAK_CONTINUOUS_MA || u->est_continuous_ma == 0 || !fw_horn_on(&n) ||
        !u->led_rgb_en || u->applied_segments < 1 || u->limit_action != FW_LIMIT_CLAMP) {
        snprintf(g_why, sizeof g_why, "ma=%u horn=%d segs=%u", u->est_continuous_ma, fw_horn_on(&n),
                 u->applied_segments);
        fail("TC-PK", g_why);
        return;
    }
    /* Short-pulse wall is a model ceiling. This host sim does not sample a shunt. */
    if (FW_PEAK_PULSE_MA < FW_PEAK_CONTINUOUS_MA) {
        fail("TC-PK", "pulse ceiling below continuous ceiling");
        return;
    }
    pass("TC-PK (model clamp, not a bench milliamp reading)");
}

static void tc_l9(void)
{
    fw_node_t a, b;
    const fw_ux_t *u;
    pair(&a, &b);
    fw_nvs_set_relay_enable(&a, false);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (fw_alarm_relay_tx(&a) != 0 || !fw_horn_on(&a)) {
        fail("TC-L9", "relay_enable=0 did not drop cascade TX");
        return;
    }
    fw_node_init(&a);
    fw_set_group(&a, 1);
    fw_set_tx_fail(&a, true);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    if (!fw_horn_on(&a) || !fw_ux(&a)->led_rgb_en || fw_alarm_relay_tx(&a) != 0) {
        fail("TC-L9", "cascade failure blocked local annunciation");
        return;
    }
    fw_node_init(&a);
    fw_set_group(&a, 1);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    u = fw_ux(&a);
    if (u->est_continuous_ma > FW_PEAK_CONTINUOUS_MA || !fw_horn_on(&a) || u->applied_segments < 1) {
        snprintf(g_why, sizeof g_why, "tx budget ma=%u", u->est_continuous_ma);
        fail("TC-L9", g_why);
        return;
    }
    pass("TC-L9");
}

static void tc_hush_cfg(void)
{
    fw_node_t n;
    fw_node_init(&n);
    if (HUSH_SMOKE_S != 540 || fw_hush_cfg_s(&n) != 540) {
        fail("TC-HUSH-CFG", "default HUSH_SMOKE_S is not 540");
        return;
    }
    if (fw_set_hush_smoke_s(&n, 479) == 0 || fw_set_hush_smoke_s(&n, 601) == 0 ||
        fw_hush_cfg_s(&n) != 540) {
        fail("TC-HUSH-CFG", "accepted a window outside 8–10 min");
        return;
    }
    if (fw_set_hush_smoke_s(&n, 480) != 0) {
        fail("TC-HUSH-CFG", "rejected 8 min");
        return;
    }
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_mute(&n);
    if (fw_hush_remaining_s(&n) != 480) {
        fail("TC-HUSH-CFG", "8 min config did not apply");
        return;
    }
    fw_clear_alarm(&n);
    if (fw_set_hush_smoke_s(&n, 600) != 0) {
        fail("TC-HUSH-CFG", "rejected 10 min");
        return;
    }
    fw_set_alarm(&n, FW_ALARM_SMOKE);
    fw_mute(&n);
    if (fw_hush_remaining_s(&n) != 600 || HUSH_SMOKE_S != 540) {
        fail("TC-HUSH-CFG", "10 min config changed the 540 constant");
        return;
    }
    pass("TC-HUSH-CFG");
}

static void tc_hooks_json(void)
{
    fw_node_t a, b;
    char buf[8192];
    pair(&a, &b);
    fw_set_alarm(&a, FW_ALARM_SMOKE);
    fw_alarm_relay_inject_rx(&b, FW_ALARM_SMOKE, 1);
    fw_mute(&a);
    if (fw_status_json(&a, &b, buf, sizeof buf) != 0) {
        fail("TC-HOOKS-JSON", "status dump failed");
        return;
    }
    if (strstr(buf, "\"hush_active\": true") == NULL ||
        strstr(buf, "\"hush_remaining_s\": 540") == NULL ||
        strstr(buf, "\"hush_remain_s\": 540") == NULL ||
        strstr(buf, "\"hush_reject\": \"none\"") == NULL ||
        strstr(buf, "\"hush_reject_reason\": \"none\"") == NULL ||
        strstr(buf, "\"alarm_relay_tx\":") == NULL || strstr(buf, "\"alarm_relay_rx\":") == NULL ||
        strstr(buf, "\"relay_enable\": true") == NULL || strstr(buf, "\"HUSH_SMOKE_S\": 540") == NULL ||
        strstr(buf, "\"version\": \"fw-hush-relay-0.1.0-evt\"") == NULL ||
        strstr(buf, "\"icd_baseline\": \"v0.1.7\"") == NULL) {
        fail("TC-HOOKS-JSON", buf);
        return;
    }
    if (strstr(buf, "\"horn_on\": false") == NULL || strstr(buf, "\"horn_on\": true") == NULL) {
        fail("TC-HOOKS-JSON", "dut silenced and peer horn were not both visible");
        return;
    }
    pass("TC-HOOKS-JSON");
}

static void tc_default_relay_on(void)
{
    fw_node_t n;
    fw_node_init(&n);
    if (!fw_relay_enable(&n)) {
        fail("TC-NVS-RELAY-DEFAULT", "relay_enable default is not ON");
        return;
    }
    pass("TC-NVS-RELAY-DEFAULT");
}

int main(void)
{
    time_t t0 = time(NULL);
    tc_hush_01();
    tc_hush_02();
    tc_hush_03();
    tc_hush_04();
    tc_hush_co();
    tc_hush_multi();
    tc_cascade_01();
    tc_cascade_02();
    tc_cascade_03();
    tc_cascade_04();
    tc_cascade_05();
    tc_cascade_fail();
    tc_matter_01();
    tc_matter_02();
    tc_claim();
    tc_l1();
    tc_l2();
    tc_l3();
    tc_l4();
    tc_l5();
    tc_l6();
    tc_l7();
    tc_l8();
    tc_pk();
    tc_l9();
    tc_hush_cfg();
    tc_hooks_json();
    tc_default_relay_on();
    skip("TC-CASCADE-PWR-01", "EVT hardware Vbat/current; not run and no result claimed");
    printf("RESULT pass=%d fail=%d skip=%d elapsed_s=%ld\n", g_pass, g_fail, g_skip,
           (long)(time(NULL) - t0));
    if ((time(NULL) - t0) > 5) {
        fprintf(stderr, "FAIL suite used wall clock; hush must be virtual via fw_tick\n");
        return 1;
    }
    return g_fail == 0 ? 0 : 1;
}
