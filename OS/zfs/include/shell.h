#ifndef SHELL_H
#define SHELL_H

#ifdef DEBUG
# define pr_debug(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
#else
# define pr_debug(format, ...)
#endif

#define MAX_COMMANDS        64      /* max support command numbers */
#define MAX_HISTCMDS        4
#define CMDLINE_MAX_ARGS    16      /* max args of each commands */
#define CMDLINE_MAX_LEN     256     /* max input line length */
#define CMDNAME_LEN         32      /* command name string length */

struct commands_desc
{
	struct {
		char cmdstr[CMDNAME_LEN];
		char *hints;
		int (*function)(int argc, char **args);
	}entry[MAX_COMMANDS];
	int count;
	int head;
	int tail;
};

struct histcommand_desc
{
	struct {
		char cmdline[CMDLINE_MAX_LEN];
	}entry[MAX_HISTCMDS];
	int count;
	int head;
	int tail;
	int pos;
};

void shell_init(void);
void shell_run(void);
void shell_cmd_init(void);
void command_register(const char *name,
		int (*function)(int, char**), const char *hints);

#endif
