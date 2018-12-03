#include "common.h"

#define SCL_PIN	PB_6
#define SDA_PIN	PB_7

static struct soft_i2c s_i2c = {
	.pin_mode_od = 1,
	.scl_pin = PB_6,
	.sda_pin = PB_7,
	.pin_set_func = bsp_gpio_set,
	.pin_get_func = bsp_gpio_get,
};

int shell_soft_i2c_scan(int argc, char **argv)
{
	int addr;

	bsp_gpio_mode(s_i2c.scl_pin, GPIO_Mode_Out_OD);
	bsp_gpio_mode(s_i2c.sda_pin, GPIO_Mode_Out_OD);
	pr_log("start i2c scan ...\n");
	for (addr = 0x3; addr < 0x7f; addr ++) {
		if (soft_i2c_xfer(&s_i2c, addr, NULL, 0, NULL, 0) == 0)
			pr_log("Found dev, slave addr = 0x%x\n", addr);
	}
	pr_log("i2c scan end\n");
	return 0;
}
