#ifndef __SHELL_H__
#define __SHELL_H__

#define SHELL_MAX_STR_SIZE	120
#define SHELL_MAX_PARAMS	16
#define SHELL_PROMPT_STR	"# "

typedef int (*shell_cmd_fn_t)(int argc, char **argv);

struct shell_cmd {
	char *cmd_str;
	char *help_str;
	shell_cmd_fn_t fn;
};

struct shell {
	char line[SHELL_MAX_STR_SIZE];
	int line_end;
	char tab_seq;
	char echo_disable;
	char esc_seq;
	int (*shell_putc)(int c);
	int (*shell_getc)(void);
	int cmd_num;
	const struct shell_cmd *cmds;
};

extern struct shell default_shell;
void shell_kick(struct shell *sh);
void shell_print(struct shell *sh, const char *fmt, ...);

#endif /* __SHELL_H__ */
