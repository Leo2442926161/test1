#include "hook_session.h"

#include <string.h>

static fw_node_t g_node;
static bool g_booted;

void hook_session_boot(void)
{
	fw_node_init(&g_node);
	g_booted = true;
}

fw_node_t *hook_session_node(void)
{
	if (!g_booted) {
		hook_session_boot();
	}
	return &g_node;
}

int hook_session_json(char *buf, size_t buflen)
{
	return fw_status_json(hook_session_node(), NULL, buf, buflen);
}

int hook_session_frame(char *buf, size_t buflen, size_t *out_len)
{
	size_t n;

	if (buf == NULL || out_len == NULL || buflen < 3) {
		return -1;
	}
	if (hook_session_json(buf, buflen - 1) != 0) {
		return -1;
	}
	n = strlen(buf);
	if (n == 0 || n + 2 > buflen) {
		return -1;
	}
	if (memchr(buf, '\n', n) != NULL || memchr(buf, '\r', n) != NULL) {
		return -1;
	}
	buf[n] = '\n';
	buf[n + 1] = '\0';
	*out_len = n + 1;
	return 0;
}

hook_sink_t hook_session_active_sink(void)
{
	return HOOK_SINK_CONSOLE;
}

const char *hook_session_sink_name(hook_sink_t sink)
{
	switch (sink) {
	case HOOK_SINK_CONSOLE:
		return "console";
	case HOOK_SINK_RTT:
		return "rtt";
	case HOOK_SINK_UART:
		return "uart";
	case HOOK_SINK_NONE:
	default:
		return "none";
	}
}

const char *hook_session_rtt_status(void)
{
	return "not_linked";
}

const char *hook_session_uart_status(void)
{
	return "pin_tbd";
}

const char *hook_session_image_claim(void)
{
	return "none";
}

const char *hook_session_relay_store_name(void)
{
	return "ram";
}

bool hook_session_relay_persisted(void)
{
	return false;
}

int hook_session_set_relay_enable(bool enable)
{
	fw_nvs_set_relay_enable(hook_session_node(), enable);
	return 0;
}

int hook_rtt_write(const char *bytes, size_t len)
{
	(void)bytes;
	(void)len;
	return -1;
}

int hook_uart_write(const char *bytes, size_t len)
{
	(void)bytes;
	(void)len;
	return -1;
}
