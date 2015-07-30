#define CONFIG_SYS_CBSIZE		256
#define CONFIG_SYS_MAXARGS		16

#define isblank(c)	(c == ' ' || c == '\t')

#define NULL		0


int cmd_process(int argc, char * const argv[]);


static char console_buffer[CONFIG_SYS_CBSIZE + 1]; /* console I/O buffer */
static const char erase_seq[] = "\b \b";	/* erase sequence */
static const char   tab_seq[] = "        ";	/* used to expand TABs */


void put_chs(const char *s) {
	while(*s != '\0') { /* Loop until end of string */
		put_ch(*s);
		s++; /* Next char */
	}
}

static char *delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0)
		return p;

	if (*(--p) == '\t') {		/* will retype the whole line */
		while (*colp > plen) {
			put_chs(erase_seq);
			(*colp)--;
		}
		for (s = buffer; s < p; ++s) {
			if (*s == '\t') {
				put_chs(tab_seq + ((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				put_ch(*s);
			}
		}
	} else {
		put_chs(erase_seq);
		(*colp)--;
	}
	(*np)--;

	return p;
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

int cli_readline(void)
{
	char *prompt = "Bell>";
	char *p = console_buffer;
	int plen = 0;
	char c;
	char *p_buf = p;
	int n = 0;
	int col;

	console_buffer[0] = '\0';

	plen = strlen(prompt);
	put_chs(prompt);
	col = plen;

	for (;;) {
		c = get_ch();
		switch(c) {
		case '\r':
		case '\n':
			*p = '\0';
			put_chs("\r\n");
			return p - p_buf;

		case '\0':
			continue;

		case 0x17:			/* ^W - erase word	*/
			p = delete_char(p_buf, p, &col, &n, plen);
			while ((n > 0) && (*p != ' '))
				p = delete_char(p_buf, p, &col, &n, plen);
			continue;

		case 0x08:			/* ^H  - backspace	*/
		case 0x7F:			/* DEL - backspace	*/
			p = delete_char(p_buf, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CONFIG_SYS_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs */
					put_chs(tab_seq + (col & 07));
					col += 8 - (col & 07);
				} else {
					char buf[2];

					/*
					 * Echo input using put_chs() to force an
					 * LCD flush if we are using an LCD
					 */
					++col;
					buf[0] = c;
					buf[1] = '\0';
					put_chs(buf);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full */
				put_ch('\a');
			}
		}
	}
}

int cli_simple_parse_line(char *line, char *argv[])
{
	int nargs = 0;

	while (nargs < CONFIG_SYS_MAXARGS) {
		/* skip any white space */
		while (isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && !isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	put_chs("** Too many args **\n");

	return nargs;
}



int run_command(char *cmd)
{
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int rc = 0;
	char *str = cmd;

	if (!cmd || !*cmd)
		return -1;	/* empty command */

	if (strlen(cmd) >= CONFIG_SYS_CBSIZE) {
		put_chs("## Command too long!\n");
		return -1;
	}

	while (*str) {
		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep == '\'') &&
			    (*(sep - 1) != '\\'))
				inquotes = !inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    (sep != str) &&	/* past string start	*/
			    (*(sep - 1) != '\\'))	/* and NOT escaped */
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		} else {
			str = sep;	/* no more commands for next pass */
		}
		/* find macros in this token and replace them */

		/* Extract arguments */
		argc = cli_simple_parse_line(token, argv);
		if (argc == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		if (cmd_process(argc, argv))
			rc = -1;
	}

	return rc;
}

void cli_loop(void)
{
	int len;

	for(;;) {
		len = cli_readline();
		if (len > 0)
			run_command(console_buffer);
		else if (len == 0)
			continue;
		else
			put_chs("Unexpected Input\n");

	}
}

void c_entry() {
	put_chs("\nWelcome to Bare Shell, a tiny shell for bare metal\n\n");
	cli_loop();
}


