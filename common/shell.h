#ifndef __SHELL_H__
#define __SHELL_H__

#define SHELL_MAX_STR_SIZE	80
#define SHELL_MAX_HISTORY_NUM	4
#define SHELL_MAX_PARAMS	8
#define SHELL_PROMPT_STR	"# "

typedef int (*shell_cmd_fn_t)(int argc, char **argv);

struct shell_cmd {
	char *cmd_str;
	char *help_str;
	shell_cmd_fn_t fn;
};

struct shell {
	char line[SHELL_MAX_STR_SIZE];
	char his_lines[SHELL_MAX_HISTORY_NUM][SHELL_MAX_STR_SIZE];
	uint8_t line_end;
	uint8_t his_end;
	uint8_t init_done;
	uint8_t his_seq;
	uint8_t tab_seq;
	uint8_t esc_seq;
	int (*shell_putc)(int c);
	int (*shell_getc)(void);
	uint8_t cmd_num;
	const struct shell_cmd *cmds;
};

extern struct shell default_shell;
void shell_kick(struct shell *sh);
void shell_print(struct shell *sh, const char *fmt, ...);

#endif /* __SHELL_H__ */
