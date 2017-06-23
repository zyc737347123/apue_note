int readline_into_buffer(const char *const prompt, char *buffer, int timeout)
{
	char *p = buffer;
	char *p_buf = p;
	int n = 0;
	int plen = 0;
	int col;
	char c;

	/* print prompt */  
	if (prompt) {
		plen = strlen (prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {

		c = getc();
		switch (c) {
			case '\r':
			case '\n':
				*p = '\0';
				puts ("\r\n");
				return (p - p_buf);

			case '\0':
				continue;

			case 0x03:    /* ^C - break  */     
				p_buf[0] = '\0'; /* discard input */
				return (-1);

			case 0x15:    /* ^U - erase line */ 
				while (col > plen) {
					puts (erase_seq);
					--col;
				}
				p = p_buf;
				n = 0;
				continue;

			case 0x17:    /* ^W - erase word */ 
				p=delete_char(p_buf, p, &col, &n, plen);
				while ((n > 0) && (*p != ' ')) {
					p=delete_char(p_buf, p, &col, &n, plen);
				}
				continue;

			case 0x08:    /* ^H  - backspace */
			case 0x7F:    /* DEL - backspace */
				p=delete_char(p_buf, p, &col, &n, plen);
				continue;

			default:
				/*
				 * Must be a normal character then
				 */
				if (n < CONFIG_SYS_CBSIZE-2) {
					if (c == '\t') { /* expand TABs  */
						puts (tab_seq+(col&07));
						col += 8 - (col&07);
					} else {
						++col;  /* echo input  */
						putc (c);
					}
					*p++ = c;
					++n;
				} else {   /* Buffer full  */
					putc ('\a');
				}
		}
	}
}
