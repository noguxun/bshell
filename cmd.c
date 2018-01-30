#define NULL	0

void put_chs(const char *s);
int put_ch(char c);
int strcmp(const char * cs,const char * ct);

struct cmd_tbl_s {
	char		*name;		/* Command Name			*/
	int		(*cmd)(int argc, char * const argv[]);
	char		*help;
};

typedef struct cmd_tbl_s	cmd_tbl_t;
extern const cmd_tbl_t cmd_table[];


static int cmd_hello(int argc, char * const argv[])
{
	put_chs("Hello, World\n");

	return 0;
}

static int cmd_echo(int argc, char * const argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		put_chs(argv[i]);
		put_ch('\n');
	}

	return 0;
}

static int cmd_help(int argc, char * const argv[])
{
	int i;

	for (i = 0; cmd_table[i].cmd; i++ ) {
		put_chs(cmd_table[i].name);
		put_chs("      ");
		put_chs(cmd_table[i].help);
		put_chs("\n");
	}
}

const cmd_tbl_t  cmd_table[] = {
	{"hello",	cmd_hello,	"Just print hello"},
	{"echo",	cmd_echo,	"Echo parameters"},
	{"help",	cmd_help,	"Print command help info"},
	{NULL,	NULL, NULL}
};

int cmd_process(int argc, char * const argv[])
{
	int i;

	if (argc < 1) {
		return -1;
	}

	for (i = 0; cmd_table[i].cmd; i++ ) {
		if(!strcmp(cmd_table[i].name, argv[0])) {
			return cmd_table[i].cmd(argc, argv);
		}
	}

	put_chs("Error: command not found\n");
	return -1;
}
