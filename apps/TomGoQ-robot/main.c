#include "common.h"
#include "version.h"

#define SHELL_UART_ID 1
#define SHELL_UART_SPEED 9600

int shell_robot_test(int argc, char **argv);
int shell_soft_i2c_scan(int argc, char **argv);
int shell_mpu6050_test(int argc, char **argv);

static int shell_test(int argc, char **argv)
{
	int i;

	pr_log("argc = %d\n", argc);
	for (i = 0; i < argc; i++)
		pr_log("argv[%d]: \"%s\"\n", i, argv[i]);
	return 0;
}

static void onboard_led(int on)
{
	static int init;

	if (!init) {
		bsp_gpio_direction(PB_12, 0);
		init = 1;
	}
	bsp_gpio_set(PB_12, !on);
}
static int shell_onboard_led_toggle(int argc, char **argv)
{
	static char on;
	on = !on;
	onboard_led(on);
	pr_log("led: %s\n", on ? "on" : "off");
	return 0;
}

static int shell_putc(int c)
{
	bsp_uart_putc(SHELL_UART_ID, (char)c);
	return c;
}

static int shell_getc(void)
{
	return bsp_uart_getc(SHELL_UART_ID);
}

static const struct shell_cmd app_cmds[] = {
	{
		.cmd_str = "led",
		.help_str = "led toggle",
		.fn = shell_onboard_led_toggle,
	},
	{
		.cmd_str = "robot",
		.help_str = "robot LH LL RH RL LA RA",
		.fn = shell_robot_test,
	},
	{
		.cmd_str = "soft_i2c_scan",
		.help_str = "soft i2c scan",
		.fn = shell_soft_i2c_scan,
	},
	{
		.cmd_str = "mp6050_test",
		.help_str = "mp6050 test",
		.fn = shell_mpu6050_test,
	},
	{
		.cmd_str = "test",
		.help_str = "test",
		.fn = shell_test,
	},
};

struct shell default_shell = {
	.shell_putc = shell_putc,
	.shell_getc = shell_getc,
	.cmds = app_cmds,
	.cmd_num = sizeof(app_cmds)/sizeof(app_cmds[0]),
};

void main(void)
{
	bsp_init();
	bsp_uart_init(SHELL_UART_ID, SHELL_UART_SPEED);
	pr_log("\n%s\n", VERSION_STRING);
	for (;;) {
		shell_kick(&default_shell);
	}
}
