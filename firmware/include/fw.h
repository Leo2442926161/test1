/*
 * Host-portable behavioral model for ICD v0.1.7 hush, offline cascade,
 * and SED L1–L9. This is not an nRF54L15 flash image.
 */
#ifndef FW_H
#define FW_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FW_VERSION "fw-hush-relay-0.1.0-evt"
#define FW_ICD_BASELINE "v0.1.7"

/* Default smoke MuteWindow. Configurable only inside 8–10 min. */
#define HUSH_SMOKE_S 540
#define HUSH_SMOKE_S_MIN 480
#define HUSH_SMOKE_S_MAX 600

#define FW_PEAK_CONTINUOUS_MA 600
#define FW_PEAK_PULSE_MA 1200
#define FW_L3_SWEEP_MAX_S 60
#define FW_L3_COOLDOWN_S 300

typedef enum fw_alarm {
    FW_ALARM_NONE = 0,
    FW_ALARM_SMOKE = 1,
    FW_ALARM_CO = 2
} fw_alarm_t;

typedef enum fw_hush_reject {
    FW_HUSH_REJECT_NONE = 0,
    FW_HUSH_REJECT_CO_FORBIDDEN = 1,
    FW_HUSH_REJECT_NO_ALARM = 2
} fw_hush_reject_t;

typedef enum fw_role {
    FW_ROLE_IDLE = 0,
    FW_ROLE_ORIGIN = 1,
    FW_ROLE_FOLLOW = 2
} fw_role_t;

typedef enum fw_ux_mode {
    FW_UX_IDLE = 0,
    FW_UX_RING_SWEEP = 1,
    FW_UX_PATH_GUIDE = 2,
    FW_UX_PULSE_SMOKE = 3,
    FW_UX_PULSE_CO = 4,
    FW_UX_PULSE_FAULT = 5,
    FW_UX_MUTE_HOLD = 6,
    FW_UX_FULL_WHITE = 7
} fw_ux_mode_t;

typedef enum fw_limit_action {
    FW_LIMIT_NONE = 0,
    FW_LIMIT_CLAMP = 1,
    FW_LIMIT_REJECT = 2,
    FW_LIMIT_STOP = 3
} fw_limit_action_t;

typedef enum fw_reject_reason {
    FW_REJECT_NONE = 0,
    FW_REJECT_WINDOW = 1,
    FW_REJECT_BRIGHTNESS = 2,
    FW_REJECT_COOLDOWN = 3,
    FW_REJECT_FULL_WHITE = 4,
    FW_REJECT_PEAK = 5,
    FW_REJECT_OTHER = 6
} fw_reject_reason_t;

#define FW_UX_FLAG_RF_PEAK (1u << 0)
#define FW_UX_FLAG_FACTORY (1u << 1)

typedef struct fw_ux {
    fw_ux_mode_t mode;
    uint8_t applied_window;
    uint8_t applied_brightness;
    uint8_t applied_segments;
    bool led_rgb_en;
    bool led_status_on;
    uint16_t led_status_avg_ua;
    fw_limit_action_t limit_action;
    fw_reject_reason_t reject_reason;
    bool rf_half;
    bool external_power;
    uint16_t sweep_elapsed_s;
    uint16_t cooldown_remain_s;
    uint16_t sweep_period_ms;
    uint16_t est_continuous_ma;
} fw_ux_t;

typedef struct fw_node {
    uint32_t group_id;
    bool relay_enable; /* NVS, default ON */
    bool cloud_up;
    bool matter_session;
    uint32_t matter_notify_attempts;
    uint32_t matter_notify_sent;
    fw_alarm_t alarm;
    bool condition_present;
    bool hush_active;
    uint16_t hush_remaining_s;
    uint16_t hush_cfg_s;
    fw_hush_reject_t hush_reject;
    bool horn_on;
    bool full_alarm_light;
    bool sensing_active;
    bool voice_enable;
    uint32_t alarm_relay_tx;
    uint32_t alarm_relay_rx;
    uint32_t tx_seq;
    uint32_t cascade_fail_count;
    bool tx_fail;
    fw_role_t role;
    fw_alarm_t last_tx_type;
    fw_alarm_t last_rx_type;
    bool silenced_by_remote_hush;
    fw_ux_t ux;
} fw_node_t;

const char *fw_version(void);
const char *fw_icd_baseline(void);
const char *fw_matter_role(void);
bool fw_matter_equals_cascade(void);
bool fw_wired_interconnect_supported(void);

void fw_node_init(fw_node_t *n);

int fw_set_alarm(fw_node_t *n, fw_alarm_t type);
void fw_clear_alarm(fw_node_t *n);
int fw_mute(fw_node_t *n);
int fw_critical(fw_node_t *n);
void fw_tick(fw_node_t *n, uint32_t dt_s);

/* Returns 0 or -1 if seconds is outside 8–10 min. Does not change HUSH_SMOKE_S. */
int fw_set_hush_smoke_s(fw_node_t *n, int seconds);

void fw_nvs_set_relay_enable(fw_node_t *n, bool enable);
void fw_set_cloud_up(fw_node_t *n, bool up);
void fw_set_matter_session(fw_node_t *n, bool up);
void fw_set_group(fw_node_t *n, uint32_t group_id);
void fw_set_tx_fail(fw_node_t *n, bool fail);
void fw_set_voice_enable(fw_node_t *n, bool enable);

/* Always rejects. There is no user path that permanently disables the horn. */
int fw_user_disable_horn(fw_node_t *n);

/*
 * Deliver one remote cascade frame into dst.
 * Returns 1 if dst followed, 0 if dropped. Never copies hush state.
 * group_id 0 or a group mismatch or relay_enable=0 drops the frame.
 */
int fw_alarm_relay_inject_rx(fw_node_t *dst, fw_alarm_t type, uint32_t src_group);

bool fw_hush_active(const fw_node_t *n);
uint16_t fw_hush_remaining_s(const fw_node_t *n);
fw_hush_reject_t fw_hush_reject(const fw_node_t *n);
uint16_t fw_hush_cfg_s(const fw_node_t *n);
fw_alarm_t fw_alarm(const fw_node_t *n);
bool fw_horn_on(const fw_node_t *n);
bool fw_full_alarm_light(const fw_node_t *n);
bool fw_sensing_active(const fw_node_t *n);
bool fw_relay_enable(const fw_node_t *n);
uint32_t fw_alarm_relay_tx(const fw_node_t *n);
uint32_t fw_alarm_relay_rx(const fw_node_t *n);
uint32_t fw_tx_seq(const fw_node_t *n);
uint32_t fw_cascade_fail_count(const fw_node_t *n);
uint32_t fw_group(const fw_node_t *n);
fw_alarm_t fw_last_tx_type(const fw_node_t *n);
fw_alarm_t fw_last_rx_type(const fw_node_t *n);
bool fw_silenced_by_remote_hush(const fw_node_t *n);
bool fw_cloud_up(const fw_node_t *n);
bool fw_matter_session(const fw_node_t *n);
uint32_t fw_matter_notify_attempts(const fw_node_t *n);
uint32_t fw_matter_notify_sent(const fw_node_t *n);
fw_role_t fw_role(const fw_node_t *n);
const fw_ux_t *fw_ux(const fw_node_t *n);

/* Negative window/brightness/segments select the mode default. */
int fw_ux_set(fw_node_t *n, fw_ux_mode_t mode, int window, int brightness,
              int segments, uint32_t flags);
void fw_ux_tick(fw_node_t *n, uint32_t dt_s);
/* Idle (LED_RGB_EN=0) refuses a data frame. Returns 0 if a frame may go out. */
int fw_ux_push_frame(const fw_node_t *n);

const char *fw_alarm_str(fw_alarm_t alarm);
const char *fw_hush_reject_str(fw_hush_reject_t reason);
const char *fw_ux_mode_str(fw_ux_mode_t mode);
const char *fw_limit_str(fw_limit_action_t action);
const char *fw_reject_reason_str(fw_reject_reason_t reason);
const char *fw_role_str(fw_role_t role);

/* JSON status dump. peer may be NULL. Returns 0 or -1 if buf is too small. */
int fw_status_json(const fw_node_t *dut, const fw_node_t *peer, char *buf, size_t buflen);

#endif
