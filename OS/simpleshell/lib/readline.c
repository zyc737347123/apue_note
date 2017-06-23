#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <shell.h>

static char defprompt[] = "[debug@oaid] $";
static char backspace[] = "\b \b";
static char enterline[] = "\r\n";

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

#define enterline() outstr(enterline)
#define backspace() outstr(backspace)
#define beeponly()      outbyte('\a')

int readline(char *buffer)
{
	char *pb = buffer;
	char *pc = pb;

	int cnt = 0;
	int plen = 0;
	int col = 0;
	char c;

	plen = strlen(defprompt);
	outstr(defprompt);

	col = plen;

	for (;;) {
		c = inbyte();
		switch (c) {
			case '\r':
			case '\n':
				*pc = '\0';
				enterline();
				return (pc - pb);
			case 0x03:    /* ^C - break  */     
				*pb = '\0'; /* discard input */
				return 0;
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
								outbyte('U');
								break;
							case 0x42:
								outbyte('D');
								break;
							case 0x43:
								outbyte('R');
								break;
							case 0x44:
								outbyte('L');
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
}
