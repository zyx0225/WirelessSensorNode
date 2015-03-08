#include "serial.h"

#include <avr/io.h>
#include <stdio.h>


//
// Support for use of UART as stdin/stdout/stderr
//
static int write(char ch, FILE __attribute__((unused)) *unused)
{
    serial_put(ch);
    return ch;
}

static int read(FILE __attribute__((unused)) *unused)
{
    return serial_get();
}

/* Set up stdio to use uart */

static FILE uart0 = FDEV_SETUP_STREAM(write, read, _FDEV_SETUP_RW);

// setup the UART at the specified speed (baud rate)
// note: read the 128RFA1 datasheet - depending on the clock speed, only some
//       baud rates work accurately

void serial_init(unsigned long baud)
{
   baud = baud/10;
   baud = ((F_CPU/16)/baud)+5;
   baud = (baud/10)-1;
   UCSR0B = 0x00;        // disable UART0
   UCSR0A = 0x0;         // clear status; initialise normal mode
   UCSR0C = 0x06;        // asynch, no parity, 1 stop, 8 bits
   UBRR0H = (baud >> 8) & 0x0F;   // baud high
   UBRR0L = baud & 0xFF; // baud low
   UCSR0B = 0x18;        // enable rx and tx
   // setup for stdio
   stdout = &uart0;
   stdin = &uart0;
   stderr = &uart0;
}

// send a character to the UART

void serial_put(unsigned char c)
{
   while ( !(UCSR0A & (1<<UDRE0)) );
   UDR0 = c;
}

// send a string to the UART

void serial_puts(char *text)
{
   while (*text!=0)
      serial_put(*text++);
}

// check if input data is ready

bool serial_ready()
{
   return ((UCSR0A & (1<<RXC0))!=0);
}

// ready a character

char serial_get()
{
   while ( !serial_ready() );
   return UDR0;
}
