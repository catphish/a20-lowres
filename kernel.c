#include "uart.h"
#include "display.h"
#include <stdint.h>

// Main program loop
void kernel_main()
{
  // Populate the pagetable
  int n;
  for(n=0;n<4096;n++)
  {
    if(n==0)
    {
      // SRAM.  Write back.
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (0<<12) | (3<<10) | (3<<2) | 2;
    } else if (n>=0x400 && n<0xc00) {
      // DRAM. Write through.
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (0<<12) | (3<<10) | (2<<2) | 2;
    } else {
      // Other stuff. Non-shared device.
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (2<<12) | (3<<10) | (0<<2) | 2;
    }
  }
  // Disable MMU
  asm("ldr r8, =0x0;"
      "mcr p15, 0, r8, c1, c0, 0;"
      : : : "r8");

  // Set up the pagetable
  asm("ldr r8, =0x4000; mcr p15, 0, r8, c2, c0, 0" : : : "r8");
  asm("ldr r8, =0x0;    mcr p15, 0, r8, c2, c0, 2" : : : "r8");
  asm("ldr r8, =0x3;    mcr p15, 0, r8, c3, c0, 0" : : : "r8");

  // Enable MMU
  asm(
    "MCR p15, 0, r8, c8, C3, 0;"
    "MCR p15, 0, r8, c8, C5, 0;"
    "MCR p15, 0, r8, c8, C6, 0;"
    "MCR p15, 0, r8, c8, C7, 0;"

    "mrc p15, 0, r8, c1, c0, 0;"
    "orr r8, r8, #0x0005;"
    "orr r8, r8, #0x1000;"
    "mcr p15, 0, r8, c1, c0, 0;"
    : : : "r8");


  uart_init();
  uart_print("Booting A20 Woo...\r\n");

  display_init();
  while ( 1 );
}
