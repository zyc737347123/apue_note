#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <shell.h>

static struct termios termsettings;
static volatile int shell_isrunning = 0;

struct commands_desc commands_list = 
{
	.count = MAX_COMMANDS,
	.head  = 0,
	.tail  = 0,
};

static int init_console(struct termios *defsetting)
{
	struct termios setting;

	tcgetattr(0, &setting);

	memcpy(defsetting, &setting, sizeof(struct termios));

	setting.c_lflag &= ~ICANON;
	setting.c_lflag &= ~ECHO;
	setting.c_lflag &= ~ISIG;

	setting.c_cc[VMIN] = 1;
	setting.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &setting);

	return 0;
}

static void restore_console(struct termios *defsetting)
{
	tcsetattr(0, TCSANOW, defsetting);
}

int __cmd_shell_quit(int argc, char **argv)
{
	pr_debug("executing command : %s@%d\n", argv[0], argc);
	shell_isrunning = 0;
	return 0;
}

int execute_cmdline(const char *cmdline, int argc, char **argv)
{
    struct commands_desc *p = &commands_list;
	int cmdret = -EINVAL;
	int i;

	for (i = p->head; i <= p->tail; i++) {
		if (strcmp(cmdline, p->entry[i].cmdstr) == 0) {
			cmdret = p->entry[i].function(argc, argv);
			break;
		}
	}

	if (i > p->tail) {
		fprintf(stderr, "Unknown cmdline \"%s\"\n", cmdline);
	}

	return cmdret;
}

void command_register(const char *name, void (*function), const char *hints)
{
    struct commands_desc *p = &commands_list;

    if((p->tail + 1) % p->count == p->head){
        fprintf(stderr, "Register command failed, exceeds MAX commands entry!\n");
        return;
    }
    strncpy(p->entry[p->tail].cmdstr, name, CMDNAME_LEN);
    p->entry[p->tail].function = function;

    p->tail++;
    p->tail %= p->count;

    return;
}

void shell_run(void)
{
	int len;
	int argc;
	char *argv[CMDLINE_MAX_ARGS];
	char cmdline[CMDLINE_MAX_LEN];
	char *substr, *saveptr;
	char delims[] = " \t";

	if (shell_isrunning) {
		fprintf(stderr, "Shell is running, do not run again!\n");
		return;
	}

	init_console(&termsettings);

	/* register default commands */
	command_register("quit", __cmd_shell_quit, NULL);

	/* set flag to indicate shell is runing */
	shell_isrunning = 1;
	fprintf(stderr, "shell is running\n");

	do {
		len = readline(cmdline);
		if (len <= 0)
			continue;

		pr_debug("readline bytes %d\n", len);

		for (argc = 0, substr = cmdline; ; argc++, substr = NULL) {
			argv[argc] = strtok_r(substr, delims, &saveptr);
			if (argv[argc] == NULL)
				break;

			pr_debug("cmdline argc %d --> %s\n", argc, argv[argc]);
		}

		pr_debug("cmdline command : %s\n", argv[0]);
		execute_cmdline(argv[0], argc, argv);

	} while(shell_isrunning);

	restore_console(&termsettings);
}
