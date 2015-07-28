
static char console_buffer[1024 + 1];	/* console I/O buffer	*/

void print(const char *s);

int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

int cli_readline(void)
{
	char *prompt = "BShell>";
	char *buffer = console_buffer;
	int plen = 0;

	plen = strlen(prompt);
	if(plen) {
		print("plen is not null");
	}
}

void cli_loop(void)
{
	for(;;) {
		cli_readline();
	}
}

void c_entry() {
	print("\nHello Bare Shell!\r\n");
	cli_loop();
}


