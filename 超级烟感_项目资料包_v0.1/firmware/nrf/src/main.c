/*
 * DK smoke entry. Links the ICD hook session and prints it on the console.
 * This is not an EVT image: no pins, no RTT channel, no UART_DBG, no hex.
 */
#include "hook_session.h"

#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#if IS_ENABLED(CONFIG_CHAOJI_SMOKE_EVT_IMAGE)
#error "EVT flash image refused. No in-tree hex, UART_DBG is PIN_TBD, RTT write is not linked. See HARDWARE_NEEDED.md."
#endif

#define HOOK_LINE_MAX 2048
#define HOOK_CHUNK 64

static char hook_line[HOOK_LINE_MAX];

static void console_chunk(const char *payload, size_t n)
{
	char chunk[HOOK_CHUNK + 1];
	size_t off;

	for (off = 0; off < n; off += HOOK_CHUNK) {
		size_t take = n - off;

		if (take > HOOK_CHUNK) {
			take = HOOK_CHUNK;
		}
		memcpy(chunk, payload + off, take);
		chunk[take] = '\0';
		printk("HOOK_JSON %s\n", chunk);
	}
}

int main(void)
{
	size_t framed = 0;
	size_t json_len;

	hook_session_boot();
	printk("hook_meta sink=%s rtt=%s uart=%s relay_store=%s image_claim=%s\n",
	       hook_session_sink_name(hook_session_active_sink()),
	       hook_session_rtt_status(), hook_session_uart_status(),
	       hook_session_relay_store_name(), hook_session_image_claim());
	printk("hook_rtt_write=%d hook_uart_write=%d rtt_channel_plan=%d uart_baud_plan=%d\n",
	       hook_rtt_write(hook_line, 0), hook_uart_write(hook_line, 0),
	       HOOK_RTT_CHANNEL_PLAN, HOOK_UART_BAUD_PLAN);

	if (hook_session_json(hook_line, sizeof(hook_line)) != 0) {
		printk("hook_json error\n");
		return 0;
	}
	json_len = strlen(hook_line);
	printk("HOOK_JSON_BEGIN bytes=%u\n", (unsigned)json_len);
	console_chunk(hook_line, json_len);
	printk("HOOK_JSON_END\n");

	if (hook_session_frame(hook_line, sizeof(hook_line), &framed) != 0) {
		printk("hook_frame error\n");
		return 0;
	}
	/*
	 * The frame is the byte contract for a future RTT or UART sink.
	 * Both writers refuse it. Do not treat a 0 return as success.
	 */
	printk("hook_frame_bytes=%u hook_frame_rtt=%d hook_frame_uart=%d\n",
	       (unsigned)framed, hook_rtt_write(hook_line, framed),
	       hook_uart_write(hook_line, framed));
	return 0;
}
