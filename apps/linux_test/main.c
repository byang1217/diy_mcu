#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#include "common.h"

#include "shell.h"

static struct termios orig_attr;

static int shell_test(int argc, char **argv)
{
	int i;

	pr_log("argc = %d\n", argc);
	for (i = 0; i < argc; i++)
		pr_log("argv[%d]: \"%s\"\n", i, argv[i]);

	if (strcmp(argv[0], "q") == 0) {
		tcsetattr(0, TCSANOW, &orig_attr);
		exit(0);
	}
	return 0;
}

static const struct shell_cmd test_cmds[] = {
	{
		.cmd_str = "q",
		.help_str = "exit shell",
		.fn = shell_test,
	},
	{
		.cmd_str = "test_1",
		.help_str = "test 001",
		.fn = shell_test,
	},
	{
		.cmd_str = "test_2",
		.help_str = "test 002",
		.fn = shell_test,
	},
	{
		.cmd_str = "hello",
		.help_str = "hello test",
		.fn = shell_test,
	},
};

struct shell default_shell = {
	.shell_putc = putchar,
	.shell_getc = getchar,
	.cmds = test_cmds,
	.cmd_num = sizeof(test_cmds)/sizeof(test_cmds[0]),
};

int main(int argc, char **argv)
{
	struct termios tty_attr;

	tcgetattr(0, &orig_attr);

	/* Set raw mode. */
	tcgetattr(0,&tty_attr);
	tty_attr.c_iflag |= IGNPAR;
	tty_attr.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXANY | IXOFF);
	tty_attr.c_lflag &= ~(ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL);
	tty_attr.c_oflag &= ~OPOST;
	tty_attr.c_cc[VTIME] = 0;
	tty_attr.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&tty_attr);

	for (;;)
		shell_kick(&default_shell);
	return 0;
}
