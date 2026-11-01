/*
 * On-device session in front of fw_node / fw_status.
 * Not an EVT flash image. fw_sim.c stays on the host.
 *
 * The JSON object is fw_status_json() unchanged, so the line still says
 * image=host-sim and ncs_board_build=false. That string lives in the hashed
 * host source. This session does not rewrite it into a flash identity.
 */
#ifndef HOOK_SESSION_H
#define HOOK_SESSION_H

#include "fw.h"

#include <stdbool.h>
#include <stddef.h>

/* Future Zephyr settings key. CONFIG_SETTINGS is off; nothing is stored. */
#define HOOK_RELAY_SETTINGS_KEY "chaoji/relay_enable"

/*
 * Planned SEGGER RTT channel for the hook frame. Channel 0 stays logs.
 * The channel is not opened. hook_rtt_write() returns -1.
 */
#define HOOK_RTT_CHANNEL_PLAN 1

/*
 * UART_DBG baud is not chosen. HARDWARE_NEEDED.md leaves it blank.
 * 0 means "unlocked", not a real baud. hook_uart_write() returns -1.
 */
#define HOOK_UART_BAUD_PLAN 0

typedef enum hook_sink {
	HOOK_SINK_NONE = 0,
	HOOK_SINK_CONSOLE = 1, /* DK printk only. Not the production test port. */
	HOOK_SINK_RTT = 2,
	HOOK_SINK_UART = 3
} hook_sink_t;

void hook_session_boot(void);
fw_node_t *hook_session_node(void);

/* One fw_status_json object, no newline. Peer is NULL (one node, no radio). */
int hook_session_json(char *buf, size_t buflen);

/*
 * Production framing once a sink accepts bytes: the JSON object plus one
 * trailing '\n' and no other newline. out_len counts the newline, not the NUL.
 */
int hook_session_frame(char *buf, size_t buflen, size_t *out_len);

/* Active view until RTT is linked. */
hook_sink_t hook_session_active_sink(void);
const char *hook_session_sink_name(hook_sink_t sink);

/* "not_linked" until CONFIG_USE_SEGGER_RTT exists and a west image links it. */
const char *hook_session_rtt_status(void);
/* "pin_tbd" until UART_DBG leaves PIN_TBD in HARDWARE_NEEDED.md. */
const char *hook_session_uart_status(void);

/* "none" until a real west build of an EVT board is the flash identity. */
const char *hook_session_image_claim(void);

/* RAM mirror only. Boot always runs fw_node_init(), so the default is true. */
const char *hook_session_relay_store_name(void);
bool hook_session_relay_persisted(void);
int hook_session_set_relay_enable(bool enable);

/*
 * Sinks. Return 0 only if the bytes were accepted by that port.
 * Both return -1 in this tree. They do not write the console.
 */
int hook_rtt_write(const char *bytes, size_t len);
int hook_uart_write(const char *bytes, size_t len);

#endif
