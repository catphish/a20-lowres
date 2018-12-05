#include <stddef.h>
#include <stdint.h>

// The UART registers base address.
#define UART0_BASE 0x01C28000
// Macros to access UART registers.
#define UART0_RBR *(volatile uint32_t *)(UART0_BASE + 0x00)
#define UART0_THR *(volatile uint32_t *)(UART0_BASE + 0x00)
#define UART0_IER *(volatile uint32_t *)(UART0_BASE + 0x04)
#define UART0_FCR *(volatile uint32_t *)(UART0_BASE + 0x08)
#define UART0_LSR *(volatile uint32_t *)(UART0_BASE + 0x14)
#define UART0_USR *(volatile uint32_t *)(UART0_BASE + 0x7C)

// The length of a null terminated string
size_t strlen(const char* str)
{
  size_t ret = 0;
  while ( str[ret] != 0 )
    ret++;
  return ret;
}

// Set up the UART (serial port)
void uart_init()
{
  // Disable UART interrupts.
  UART0_IER = 0x00000000;

  // Configure UART (enable FIFO)
  UART0_FCR = 0x00000001;
}

// UART is ready to receive data to transmit?
unsigned char uart_tx_ready()
{
  return (UART0_USR & 2);
}

// UART has received data?
unsigned char uart_rx_ready()
{
  return (UART0_LSR & 1);
}

// Push one byte to the UART port (blocking until ready to transmit)
void uart_putc(unsigned char byte)
{
  // Wait for UART transmit FIFO to be not full.
  while ( ! uart_tx_ready() );
  UART0_THR = byte;
}

// Write a zero terminated string to the UART
void uart_print(const char* str)
{
  while(*str) {
    uart_putc(*str);
    str++;
  }
}

// Print a char to the UART as ASCII HEX
void uart_print_uint8(unsigned char number)
{
  unsigned char chars[] = "0123456789ABCDEF";
  uart_putc(chars[(number >> 4) & 0xF]);
  uart_putc(chars[(number >> 0) & 0xF]);
}

// Print a uint32 to the UART as ASCII HEX
void uart_print_uint32(uint32_t number)
{
  unsigned char chars[] = "0123456789ABCDEF";
  uart_putc(chars[(number >> 28) & 0xF]);
  uart_putc(chars[(number >> 24) & 0xF]);
  uart_putc(chars[(number >> 20) & 0xF]);
  uart_putc(chars[(number >> 16) & 0xF]);
  uart_putc(chars[(number >> 12) & 0xF]);
  uart_putc(chars[(number >> 8) & 0xF]);
  uart_putc(chars[(number >> 4) & 0xF]);
  uart_putc(chars[(number >> 0) & 0xF]);
}