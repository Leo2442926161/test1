/*
 * Host link check for the nRF hook session. This is not west and not a hex.
 */
#include "hook_session.h"

#include <stdio.h>
#include <string.h>

static int g_fail;

static void expect(int cond, const char *msg)
{
	if (!cond) {
		fprintf(stderr, "fail: %s\n", msg);
		g_fail = 1;
	}
}

static void expect_has(const char *json, const char *needle)
{
	if (strstr(json, needle) == NULL) {
		fprintf(stderr, "fail: missing %s\n", needle);
		g_fail = 1;
	}
}

static int count_char(const char *s, char c)
{
	int n = 0;

	for (; *s != '\0'; s++) {
		if (*s == c) {
			n++;
		}
	}
	return n;
}

int main(void)
{
	char json[2048];
	char frame[2048];
	size_t framed = 0;
	fw_node_t *node;

	hook_session_boot();
	node = hook_session_node();
	expect(node != NULL, "node");
	expect(fw_relay_enable(node), "default relay_enable");
	expect(!fw_hush_active(node), "default hush off");
	expect(hook_session_json(json, sizeof(json)) == 0, "json");
	expect(strchr(json, '\n') == NULL, "json has no newline");
	expect_has(json, "\"hush_active\": false");
	expect_has(json, "\"hush_remaining_s\":");
	expect_has(json, "\"hush_remain_s\":");
	expect_has(json, "\"hush_reject\":");
	expect_has(json, "\"alarm_relay_tx\": 0");
	expect_has(json, "\"alarm_relay_rx\": 0");
	expect_has(json, "\"relay_enable\": true");
	expect_has(json, "\"image\": \"host-sim\"");
	expect_has(json, "\"ncs_board_build\": false");
	expect_has(json, "\"peer\": null");

	expect(hook_session_active_sink() == HOOK_SINK_CONSOLE, "sink");
	expect(strcmp(hook_session_sink_name(HOOK_SINK_CONSOLE), "console") == 0, "sink name");
	expect(strcmp(hook_session_rtt_status(), "not_linked") == 0, "rtt");
	expect(strcmp(hook_session_uart_status(), "pin_tbd") == 0, "uart");
	expect(strcmp(hook_session_image_claim(), "none") == 0, "claim");
	expect(strcmp(hook_session_relay_store_name(), "ram") == 0, "store");
	expect(!hook_session_relay_persisted(), "not persisted");
	expect(strcmp(HOOK_RELAY_SETTINGS_KEY, "chaoji/relay_enable") == 0, "settings key");
	expect(HOOK_RTT_CHANNEL_PLAN == 1, "rtt plan");
	expect(HOOK_UART_BAUD_PLAN == 0, "baud unlocked");
	expect(hook_rtt_write(json, strlen(json)) == -1, "rtt refuses");
	expect(hook_uart_write(json, strlen(json)) == -1, "uart refuses");

	expect(hook_session_frame(frame, sizeof(frame), &framed) == 0, "frame");
	expect(framed == strlen(json) + 1, "frame length");
	expect(frame[framed - 1] == '\n', "frame newline");
	expect(count_char(frame, '\n') == 1, "one newline");
	expect(hook_rtt_write(frame, framed) == -1, "rtt refuses frame");
	expect(hook_uart_write(frame, framed) == -1, "uart refuses frame");

	expect(fw_set_alarm(node, FW_ALARM_SMOKE) == 0, "smoke");
	expect(fw_mute(node) == 0, "mute");
	expect(hook_session_json(json, sizeof(json)) == 0, "json after mute");
	expect_has(json, "\"hush_active\": true");
	expect_has(json, "\"horn_on\": false");
	expect_has(json, "\"hush_reject\": \"none\"");

	fw_clear_alarm(node);
	expect(fw_set_alarm(node, FW_ALARM_CO) == 0, "co");
	expect(fw_mute(node) == -1, "co mute rejected");
	expect(hook_session_json(json, sizeof(json)) == 0, "json after co");
	expect_has(json, "\"hush_reject\": \"co_forbidden\"");
	expect_has(json, "\"hush_active\": false");

	expect(hook_session_set_relay_enable(false) == 0, "relay off");
	expect(!hook_session_relay_persisted(), "still ram");
	expect(hook_session_json(json, sizeof(json)) == 0, "json relay off");
	expect_has(json, "\"relay_enable\": false");

	hook_session_boot();
	expect(fw_relay_enable(hook_session_node()), "boot restores default");
	expect(hook_session_json(json, sizeof(json)) == 0, "json after reboot");
	expect_has(json, "\"relay_enable\": true");
	expect_has(json, "\"hush_active\": false");

	if (g_fail) {
		fprintf(stderr, "hook session hostcheck failed\n");
		return 1;
	}
	printf("hook session hostcheck pass\n");
	return 0;
}
