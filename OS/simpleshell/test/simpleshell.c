#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <shell.h>

struct termios def_setting;

char (*inbyte)(void);
void (*outbyte)(char c);

char noncanon_inbyte(struct console * cosl)
{
	char thischar;
	int bnr;

	if(cosl->inbuf.phead == cosl->inbuf.ptail){
		bnr = read(STDIN_FILENO, cosl->inbuf.fifo, cosl->inbuf.size);
		if(bnr <= 0){
			thischar = '\0';
			cosl->inbuf.phead = 0;
			cosl->inbuf.ptail = 0;
			goto out;
		}

		/*
		 * If read byte number is MAX_CMDSTR_LEN, drop all the bytes
		 */
		cosl->inbuf.phead = 0;
		cosl->inbuf.ptail = bnr %  cosl->inbuf.size;
	}

	thischar = cosl->inbuf.fifo[cosl->inbuf.phead++];
out :
	return thischar;
}

void noncanon_outbyte(struct console * cosl, char c)
{
	write(STDOUT_FILENO, &c, sizeof(char));
}

int init_console(struct console * cosl)
{
	struct termios term_setting;

	tcgetattr(0, &term_setting);

	memcpy(&def_setting, &term_setting, sizeof(struct termios));

	term_setting.c_lflag &= ~ICANON;
	term_setting.c_lflag &= ~ECHO;
	term_setting.c_lflag &= ~ISIG;

	term_setting.c_cc[VMIN] = 1;
	term_setting.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &term_setting);

	cosl->inbyte = noncanon_inbyte;
	cosl->outbyte = noncanon_outbyte;
	cosl->inbuf.size = INFIFO_SIZE;
	cosl->inbuf.phead = 0;
	cosl->inbuf.ptail = 0;

	return 0;
}

int restore_console(struct console * cosl)
{
	tcsetattr(0, TCSANOW, &def_setting);
	return 0;
}


int main(int argc, char ** argv)
{
	struct console raw_console;
	char ch;

#if 0
	ch = '\r';
	fprintf(stderr,"[%02X] ", ch);
	ch = '\n';
	fprintf(stderr,"[%02X] ", ch);
#endif

	init_console(&raw_console);

	do{
		fprintf(stderr, "Please enter Key ...");
		ch = raw_console.inbyte(&raw_console);
		if(ch == 0)
			continue;
		switch(ch){
			case 'q':
				goto out;
				break;
			case 'p' :
				fprintf(stderr, "TEST\n");
				break;
#if 0
			case 0x1B:
				ch = raw_console.inbyte(&raw_console);
				switch(ch){
					case 0x5B:
						ch = raw_console.inbyte(&raw_console);
						switch(ch){
							case 0x41:
								printf("UP!\n");
								break;
							case 0x42:
								printf("DOWN!\n");
								break;
							case 0x43:
								printf("RIGHT!\n");
								break;
							case 0x44:
								printf("LEFT!\n");
								break;
						}
						break;
				}
				break;

#endif
			default:
				raw_console.outbyte(&raw_console, ch);
				fprintf(stderr, " - [%02x]\n", ch);
		}
	}while(1);

out:
	restore_console(&raw_console);
	return 0;
}
