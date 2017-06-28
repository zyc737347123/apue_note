#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <shell.h>

static struct termios termsettings;
char defprompt[100] = "[zyc@zfs] $ ";
static char bsstring[] = "\b \b";
static char endline[] = "\r\n";

extern volatile int shell_isrunning;
extern struct histcommand_desc hist_cmdlist;
extern struct commands_desc commands_list;

#define backspace()     outstr(bsstring)
#define beeponly()      outbyte('\a')
#define end_and_return(v) {                               \
	hist_cmdlist.pos = hist_cmdlist.tail;                 \
	outstr(endline);                                      \
	return (v); }

#define sizeof_validhistcmd(p)                            \
({ int size;                                              \
    if ((p)->tail >= (p->head))                           \
		size = ((p)->tail - (p)->head);                   \
	else                                                  \
		size = ((p->count) - ((p)->head - (p)->tail) + 1);\
	size;                                                 \
 })

static int init_console(struct termios *defsetting)
{
	struct termios setting;

	tcgetattr(0, &setting);

	memcpy(defsetting, &setting, sizeof(struct termios));

	setting.c_lflag &= ~ICANON;
	setting.c_lflag &= ~ECHO;
	//setting.c_lflag &= ~ISIG;

	setting.c_cc[VMIN] = 1;
	setting.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &setting);

	return 0;
}

static void restore_console(struct termios *defsetting)
{
	tcsetattr(0, TCSANOW, defsetting);
}

int cmdline_execute(const char *cmdline, int argc, char **argv)
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

void cmdline_histadd(const char *cmdline)
{
    struct histcommand_desc *p = &hist_cmdlist;

    strncpy(p->entry[p->tail].cmdline, cmdline, CMDLINE_MAX_LEN);

	p->pos = p->tail;

	p->tail++;
	p->tail %= p->count;

	if (p->tail == p->head) {
		p->head++;
		p->head %= p->count;
	}
}

char inbyte(void)
{
	char c;
	int n;

	for (;;) {
		n = read(STDIN_FILENO, &c, 1);
		if (n <= 0){
			continue;
		}
		return c;
	}
}

static void outbyte(char c)
{
	write(STDOUT_FILENO, &c, sizeof(char));
}

static void outstr(char *str)
{
	char *p;
	int len, rc;

	p = str;
	len = strlen(str);

	for (; len > 0; len -= rc, p += rc) {
		rc = write(STDOUT_FILENO, p, len);
	}
}

char *upscroll_hist(char *buffer, int *cnt, int *col)
{
	int len;
    struct histcommand_desc *p = &hist_cmdlist;

	if (sizeof_validhistcmd(p) == 0) {
		beeponly();
		return buffer;
	}

	for (; (*cnt) > 0; (*cnt)--, (*col)--)
		backspace();

	len = strlen(p->entry[p->pos].cmdline);
    strncpy(buffer, p->entry[p->pos].cmdline, CMDLINE_MAX_LEN);

	if (p->pos == 0)
		p->pos = MAX_HISTCMDS - 1;
	p->pos--;

	(*cnt) += len;
	(*col) += len;
	return (buffer + len);
}

char *downscroll_hist(char *buffer, int *cnt, int *col)
{
	int len;
    struct histcommand_desc *p = &hist_cmdlist;

	if (p->pos == p->tail) {
		beeponly();
		return buffer;
	}

	for (; (*cnt) > 0; (*cnt)--, (*col)--)
		backspace();

	len = strlen(p->entry[p->pos].cmdline);
    strncpy(buffer, p->entry[p->pos].cmdline, CMDLINE_MAX_LEN);

	p->pos++;
	p->pos %= p->count;

	(*cnt) += len;
	(*col) += len;
	return (buffer + len);
}

int readline(char *buffer)
{
	char *pb = buffer;  /* pointer of base */
	char *pc = pb;

	int cnt = 0;
	int plen = 0;
	int col = 0;
	int loop;
	char c;

	plen = strlen(defprompt);
	outstr(defprompt);

	col = plen;
	loop = 1;

	for (; loop == 1;) {
		c = inbyte();
		switch (c) {
			case '\r':
			case '\n':
				*pc = '\0';
				end_and_return((pc-pb));
			case 0x03:    /* ^C - break  */     
				*pb = '\0'; /* discard input */
				end_and_return(0);
			case '\0':
				break;
			case 0x08:    /* ^H  - backspace */
			case 0x7F:    /* DEL - backspace */
				if (cnt > 0) {
					*pc-- = '\0';
					cnt--;
					col--;
					backspace();
				} else {
					beeponly();
				}
				break;
			case 0x1B:
				c = inbyte();
				switch(c) {
					case 0x5B:
						c = inbyte();
						switch(c) {
							case 0x41:
								pc = upscroll_hist(pb, &cnt, &col);
								if (pc != pb)
									outstr(pb);
								break;
							case 0x42:
								pc = downscroll_hist(pb, &cnt, &col);
								if (pc != pb)
									outstr(pb);
								break;
							case 0x43:
								beeponly();
								break;
							case 0x44:
								beeponly();
								break;
							default :
								break;
						}
						break;
					default :
						break;
				}
				break;
			default:
				if (isprint(c)) {
					/* buffer is full */
					if (cnt >= CMDLINE_MAX_LEN - 2) {
						beeponly();
					} else {
						*pc++ = c;
						cnt++;
						col++;
						outbyte(c);
					}
				} else {
					beeponly();
				}
				break;
		}
	}
	return cnt;
}

void shell_init(void)
{
	/* register default commands */
	shell_cmd_init();
}

void shell_run(void)
{
	int len, argc;
	char *argv[CMDLINE_MAX_ARGS];
	char *substr, *saveptr;
	char cmdline[CMDLINE_MAX_LEN];
	char delims[] = " \t";

	if (shell_isrunning) {
		fprintf(stderr, "Shell is running, do not run again!\n");
		return;
	}

	init_console(&termsettings);

	/* set flag to indicate shell is runing */
	shell_isrunning = 1;
	fprintf(stderr, "shell is running\n");

	do {
		len = readline(cmdline);
		if (len <= 0)
			continue;

		pr_debug("readline [%d] : %s\n", len, cmdline);
		cmdline_histadd(cmdline);

		for (argc = 0, substr = cmdline; ; argc++, substr = NULL) {
			argv[argc] = strtok_r(substr, delims, &saveptr);
			if (argv[argc] == NULL)
				break;

			pr_debug("cmdline argc %d --> %s\n", argc, argv[argc]);
		}

		pr_debug("cmdline command : %s\n", argv[0]);
		cmdline_execute(argv[0], argc, argv);

	} while(shell_isrunning);

	restore_console(&termsettings);
}
