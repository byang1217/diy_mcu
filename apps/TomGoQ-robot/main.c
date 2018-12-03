#include "common.h"

#define SHELL_UART_ID 1
#define SHELL_UART_SPEED 9600

static int shell_test(int argc, char **argv)
{
	int i;

	pr_log("argc = %d\n", argc);
	for (i = 0; i < argc; i++)
		pr_log("argv[%d]: \"%s\"\n", i, argv[i]);
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
		.cmd_str = "test_1",
		.help_str =	"test 001\n"
				"	it is test 001\n"
				"	by Yang, Bin\n",
		.fn = shell_test,
	},
};

struct shell default_shell = {
	.shell_putc = shell_putc,
	.shell_getc = shell_getc,
	.cmds = app_cmds,
	.cmd_num = sizeof(app_cmds)/sizeof(app_cmds[0]),
};

void onboard_led(int on)
{
	static int init;

	if (!init) {
		bsp_gpio_direction(PB_12, 0);
		init = 1;
	}
	bsp_gpio_set(PB_12, !on);
}

void main(void)
{
	bsp_init();
	bsp_uart_init(SHELL_UART_ID, SHELL_UART_SPEED);
	for (;;)
		shell_kick(&default_shell);
}
