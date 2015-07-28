
#include <stdint.h>



#if 0

#define REG_BASE (0xD000A620)

static volatile uint16_t *rUART_BASE = (uint16_t *)(REG_BASE);
static volatile uint16_t *rUART_RBR = (uint16_t *) (REG_BASE + 0x00);
static volatile uint16_t *rUART_STAT = (uint16_t *)(REG_BASE + 0x0C);
static volatile uint16_t *rUART_TBR = (uint16_t *)(REG_BASE + 0x04);
static volatile uint16_t *rXCPU_ARB =   (uint16_t *)( 0xD000A06E);


void send_char(char c)
{
    int done = 0;
    if ( c == 0xFF ) {
        return;
    }

    while ( done == 0 ) {
        if ((*rUART_STAT) & (1 << 6)) {
            *rUART_TBR = c;
            done = 1;
        }
    }
}

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        send_char(*s);
        s++; /* Next char */
    }
}
#endif


#define UART_PL01x_FR_TXFF              0x20
#define UART_PL01x_FR_RXFE              0x10

volatile uint32_t * const UART0DR = (uint32_t *)(0x101f1000);
volatile uint32_t * const UART0FR = (uint32_t *)(0x101f1000 + 0x18);

/*
void int tstc(void)
{
    return !(*(UART0FR) & UART_PL01x_FR_RXFE);
}
*/

int serial_putc(char c)
{
    while(*UART0FR & UART_PL01x_FR_TXFF);

    *UART0DR = c;

    return 0;
}

int serial_getc(void)
{
    unsigned int data;

    while (*UART0FR & UART_PL01x_FR_RXFE);

    data = *UART0DR;

    return (int)data;
}

void print(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        serial_putc(*s);
        s++; /* Next char */
    }
}



