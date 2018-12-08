#include "uart.h"
#include "display.h"
#include <stdint.h>

#define GICD_BASE 0x01C81000
struct gicd_reg {
  uint32_t ctlr;           /* 0x000 */
  uint32_t typer;          /* 0x004 */
  uint32_t iidr;           /* 0x008 */
  uint32_t rfu0[29];       /* 0x00C - 0x07C */
  uint32_t igroupr[32];    /* 0x080 - 0x0FC */
  uint32_t isenabler[32];
  uint32_t icenabler[32];
  uint32_t ispender[32];
  uint32_t icpender[32];
  uint32_t isactiver[32];
  uint32_t icactiver[32];
  uint8_t  ipriorityr[1024];
  uint8_t  itargetsr[1024];
  uint32_t icfgr[64];
  uint32_t rfu1[64];
  uint32_t nsacr[64];
  uint32_t sgir;
  uint32_t rfu2[3];
  uint32_t cpendsgir[4];
  uint32_t spendsgir[4];
  uint32_t rfu3[52];
};

#define GICC_BASE 0x01C82000
struct gicc_reg {
  uint32_t ctlr;         /* 0x000 */
  uint32_t pmr;          /* 0x004 */
  uint32_t bpr;          /* 0x008 */
  uint32_t iar;          /* 0x00C */
  uint32_t eoir;
  uint32_t rpr;
  uint32_t hppir;
  uint32_t abpr;
  uint32_t aiar;
  uint32_t aeoir;
  uint32_t ahppir;
  uint32_t rfu0[41];
  uint32_t apr[4];
  uint32_t nsapr[4];
  uint32_t rfu1[3];
  uint32_t iidr;
  uint32_t rfu2[960];
  uint32_t dir;
};

int m=0;
extern volatile uint32_t * layer0_data;
void __attribute__((interrupt("FIQ"))) interrupt(void)
{
  struct gicc_reg* gicc = (struct gicc_reg*) GICC_BASE;
  uint32_t iar = gicc->iar;

  //uart_print("Interrupt: ");
  //uart_print_uint32(iar);
  //uart_print("\r\n");

  uint32_t * sprite = (uint32_t*) 0x60000000;
  // Output a simple test pattern
  for(int n=0;n<480*270;n++) {
    if(n&2) layer0_data[n] = 0xff444444;
    else layer0_data[n] = 0xff000000;
  }
  // Copy a 16x16 sprite from 0x60000000 and display it at 0,m
  for(int x=0;x<16;x++) {
    for(int y=0;y<16;y++) {
      if(sprite[y*16+x] & 0xff000000)
        layer0_data[y*480+x+m] = sprite[y*16+x] | 0xff000000;
    }
  }
  m++;
  if(m==120) m=0;
  struct sunxi_de_fe_reg * const de_fe =(struct sunxi_de_fe_reg *)DEFE0_BASE;
  de_fe->int_status = 0xffffffff;
  gicc->eoir = iar;
}

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
    "ldr r8, =0x0;"
    "MCR p15, 0, r8, c8, C3, 0;"
    "MCR p15, 0, r8, c8, C5, 0;"
    "MCR p15, 0, r8, c8, C6, 0;"
    "MCR p15, 0, r8, c8, C7, 0;"
    "mcr p15, 0, r8, c12, c0, 0;"

    "ldr r8, =0x1005;"
    "mcr p15, 0, r8, c1, c0, 0;"
    : : : "r8");


  uart_init();
  uart_print("Booting A20 Woo...\r\n");

  asm("cpsid if;"); // Disable interrupts

  struct gicd_reg* gicd = (struct gicd_reg*) GICD_BASE;
  gicd->ctlr = 1;
  gicd->isenabler[79/32] = 1<<(79%32);
  gicd->itargetsr[79] = 1;
  gicd->ipriorityr[79] = 1;
  struct gicc_reg* gicc = (struct gicc_reg*) GICC_BASE;
  gicc->ctlr = 1;
  gicc->pmr = 10;
  asm("cpsie if;"); // Enable interrupts

  uart_print("Interrupts configured...\r\n");

  display_init();
  while ( 1 );
}
