#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <shell.h>

volatile int shell_isrunning = 0;

struct commands_desc commands_list = 
{
	.count = MAX_COMMANDS,
	.head  = 0,
	.tail  = 0,
};

struct histcommand_desc hist_cmdlist =
{
	.count = MAX_HISTCMDS,
	.head = 0,
	.tail = 0,
	.pos = 0,
};

void command_register(const char *name,
		int (*function)(int, char**), const char *hints)
{
    struct commands_desc *p = &commands_list;

    if((p->tail + 1) % p->count == p->head){
        fprintf(stderr, "Register command failed, exceeds MAX commands entry!\n");
        return;
    }
    strncpy(p->entry[p->tail].cmdstr, name, CMDNAME_LEN);
    p->entry[p->tail].function = function;
    p->entry[p->tail].hints = hints;

    p->tail++;
    p->tail %= p->count;

    return;
}

int __cmd_shell_quit(int argc, char **argv)
{
	pr_debug("executing command : %s@%d\n", argv[0], argc);
	shell_isrunning = 0;
	return 0;
}

int __cmd_shell_help(int argc, char **argv)
{
	int i;
	struct commands_desc *p = &commands_list;
	
	i = p->head;
	while(i != p->tail) {
		printf("%s", p->entry[i].cmdstr);
		if(p->entry[i].hints) {
			printf(" - %s",p->entry[i].hints);
		}
		printf("\n");
		i++;
		i %= p->count;
	}
	return 0;
}

int do_face_id(int argc, char * argv[])
{
     printf("face id\n");
     return 0;
}

int do_face_reg(int argc, char **argv)
{
     printf("face reg\n");
     return 0;
}

int do_db_ops(int argc, char ** argv)
{
     printf("face ops\n");
     return 0;
}

void shell_cmd_init(void)
{
    command_register("quit", __cmd_shell_quit, "quit shell");
    command_register("help", __cmd_shell_help, NULL);
    command_register("db", do_db_ops, "[-l] [-r id]\t\tlist or remove database entry");
    command_register("reg", do_face_reg, "-i id -n name\t\tregister a face into database");
    command_register("id", do_face_id, "-g  -s        \t\tstart or stop face recoginition");
}
