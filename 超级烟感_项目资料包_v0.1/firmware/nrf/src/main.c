/*
 * NCS skeleton for a future nRF54L15 image.
 * This translation unit does not drive lamps, measure Vbat, or emit hook JSON.
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#if IS_ENABLED(CONFIG_CHAOJI_SMOKE_EVT_IMAGE)
#error "EVT flash image refused. Confirm HARDWARE_NEEDED.md before enabling CONFIG_CHAOJI_SMOKE_EVT_IMAGE."
#endif

int main(void)
{
	printk("chaoji-smoke NCS skeleton\n");
	printk("not an EVT image; host image=host-sim; ncs_board_build=false\n");
	printk("hook JSON is not on this console\n");
	return 0;
}
