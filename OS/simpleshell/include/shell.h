#ifndef SHELL_H
#define SHELL_H

#ifdef DEBUG
# define pr_debug(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
#else
# define pr_debug(format, ...)
#endif

#define MAX_COMMANDS        64      /* max support command numbers */
#define CMDLINE_MAX_ARGS    16      /* max args of each commands */
#define CMDLINE_MAX_LEN     256     /* max input line length */
#define CMDNAME_LEN         32      /* command name string length */
#define MAX_HISTENTRY       16

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
	}entry[MAX_HISTENTRY];
	int count;
	int head;
	int tail;
	int index;
};

int readline(char *buffer);
void shell_run(void);

#endif
