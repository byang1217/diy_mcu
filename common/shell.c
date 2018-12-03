#ifdef __SHELL_TEST
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#define pr_err printf
#define pr_dbg printf
#define pr_log printf
#define assert_param assert
#else
#include "common.h"
#endif

#include "shell.h"

void shell_print(struct shell *sh, const char *fmt, ...)
{
	va_list args;
	int i;
	char tmp[SHELL_MAX_STR_SIZE];

	va_start(args, fmt);
	vsnprintf(tmp, sizeof(tmp), fmt, args);
	va_end(args);

	for (i = 0; tmp[i] != 0 && i < sizeof(tmp); i++)
		sh->shell_putc(tmp[i]);
}

static int string_to_argv(char *str, char **argv)
{
	int argc = 0;

	if (str == NULL)
		return argc;
	while (*str == ' ' || *str == '\t')
		str++;
	while (*str != 0) {
		argv[argc] = str;
		argc++;
		if (argc >= SHELL_MAX_PARAMS) {
			pr_err("argc > SHELL_MAX_PARAMS\n");
			return -1;
		}
		while ((*str!= ' ') && (*str != '\t') && (*str != ',') && (*str != 0))
			str ++;

		if (*str != 0) {
			*str = 0;
			str++;
			while (*str == ' ' || *str == '\t')
				str++;
		}
	}

	return argc;
}

void shell_kick(struct shell *sh)
{
	const struct shell_cmd *cmd = NULL;
	char *line;
	char *argv[SHELL_MAX_PARAMS];
	int argc;
	int i;

	for (;;) {
		int c;
		c = sh->shell_getc();
		if (c < 0)
			return;

		/* Backspace or delete */
		if (c == '\b' || c == 127) {
			sh->line[--sh->line_end] = 0;
			if (!sh->echo_disable) {
				/* echo */
				sh->shell_putc('\b');
				/* send space + \b to delete character */
				sh->shell_putc(' ');
				sh->shell_putc('\b');
			}
			continue;
		}

		if (c == '\n')
			continue;

		if (c == '\r') {
			if (!sh->echo_disable) {
				/* echo */
				sh->shell_putc('\r');
				sh->shell_putc('\n');
			}
			break;
		}

		if (sh->line_end >= sizeof(sh->line))
			continue;

		sh->line[sh->line_end++] = c;
		sh->shell_putc(c);
	}

	//sh->line will be destroyed
	argc = string_to_argv(sh->line, argv);
	if (argc > 0) {
		if (strcmp(argv[0], "help") == 0) {
			for (i = 0; i < sh->cmd_num; i ++) {
				shell_print(sh, "%s\n", sh->cmds[i].cmd_str);
				shell_print(sh, "\t%s\n", sh->cmds[i].help_str);
			}
			goto exit;
		}
		for (i = 0; i < sh->cmd_num; i ++) {
			if (strcmp(argv[0], sh->cmds[i].cmd_str) == 0) {
				cmd = &sh->cmds[i];
				break;
			}
		}
		if (cmd == NULL) {
			shell_print(sh, "Error: Invalid command\n");
			goto exit;
		}

		if (cmd->fn(argc, argv) != 0) {
			shell_print(sh, "Error: command failed\n");
			goto exit;
		}
	}

	if (argc < 0) {
		shell_print(sh, "Error: Invalid command\n");
	}

exit:
	memset(sh->line, 0, sizeof(sh->line));
	sh->line_end = 0;
	shell_print(sh, SHELL_PROMPT_STR);
}

#ifdef __SHELL_TEST
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
#endif
