#include "uart.h"
#include "display.h"
#include <stdint.h>
#include <string.h>

struct layer {
  struct tilemap * tilemap;
  uint32_t x_offset;
  uint32_t y_offset;
};

struct tilemap {
  uint32_t x_size;
  uint32_t y_size;
  uint16_t* pattern;
};

uint16_t pattern_a[30*17];
struct tilemap tilemap_a;
struct layer layer_a;

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

volatile uint32_t * back_buffer  = (volatile uint32_t *)BUFFER_1_DATA_ADDR;
uint32_t frame_counter=0;

void __attribute__((interrupt("FIQ"))) interrupt(void)
{
  struct gicc_reg* gicc = (struct gicc_reg*) GICC_BASE;
  uint32_t iar = gicc->iar;

  // Animate the layer for fun
  layer_a.x_offset++;
  if(layer_a.x_offset > 15) layer_a.x_offset = 0;

  uint32_t * sprite = (uint32_t*) 0x60000000;

  // Output a simple test pattern as a background for now
  for(int n=0;n<480*270;n++) {
    if(n&2) back_buffer[n] = 0xff444444;
    else back_buffer[n] = 0xff000000;
  }

  // As a test / demo, just render one layer
  struct layer active_layer = layer_a;

  // Loop ofer the tile (sprite) map for this layer, incrementing the x, y
  for(int tilemap_offset_x=0;tilemap_offset_x<active_layer.tilemap->x_size;tilemap_offset_x++) {
    for(int tilemap_offset_y=0;tilemap_offset_y<active_layer.tilemap->y_size;tilemap_offset_y++) {
      // Check whether a tile is defined in the map at this offset (0 means no tile)
      int tilemap_offset = tilemap_offset_y * active_layer.tilemap->x_size + tilemap_offset_x;
      uint16_t tile_id;
      if(tile_id = active_layer.tilemap->pattern[tilemap_offset]) {
        // Fetch tile pixel data
        uint32_t* tile_data = sprite + (tile_id - 1) * 256;
        // Loop over pixels
        for(int x=0;x<16;x++) {
          for(int y=0;y<16;y++) {
            // Check pixel isn't transparent
            if(tile_data[y*16+x] & 0xff000000){
              int destination_x = x + active_layer.x_offset + tilemap_offset_x * 16;
              int destination_y = y + active_layer.y_offset + tilemap_offset_y * 16;
              back_buffer[destination_y*480 + destination_x] = tile_data[y*16+x];
            }
          }
        }
      }
    }
  }

  struct sunxi_de_fe_reg * const de_fe =(struct sunxi_de_fe_reg *)DEFE0_BASE;

  // Swap the front and back buffers ready for next frame
  uint32_t * tmp = (uint32_t*)de_fe->ch0_addr;
  de_fe->ch0_addr = (uint32_t)back_buffer; // Make back buffer live
  de_fe->frame_ctrl |= 1; // Latch FE config registers
  back_buffer = tmp; // The previously rendered frame becomes the back buffer

  de_fe->int_status = 0xffffffff;
  gicc->eoir = iar;

  frame_counter++;
  if(frame_counter == 60) {
    frame_counter = 0;
    uart_print("Rendered 60 frames.\r\n");
  }
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
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (1<<12) | (3<<10) | (3<<2) | 2;
    } else if(n>=0x400 && n<=0x401) {
      // First 2MB of DRAM, we will use this for the framebuffer, no cache
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (0<<12) | (3<<10) | (0<<2) | 2;
    } else if(n>=0x402 && n<0xc00) {
      // Remaining DRAM. Write back.
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (1<<12) | (3<<10) | (3<<2) | 2;
    } else {
      // Other stuff. Strictly ordered for safety.
      *(volatile uint32_t *)(0x4000 + n*4) = (n<<20) | (0<<12) | (3<<10) | (0<<2) | 2;
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

  for(int n=0;n<17*29;n++) {
    pattern_a[n] = 1;
  }
  tilemap_a.x_size = 29;
  tilemap_a.y_size = 17;
  tilemap_a.pattern = pattern_a;
  
  layer_a.x_offset = 8;
  layer_a.y_offset = 0;
  layer_a.tilemap = &tilemap_a;

  display_init();

  // while(1) {
  //   for(int mb=0; mb<10;mb++){
  //     for(int n=0;n<1048576;n++) { // 4MB
  //       uint32_t tmp = front_buffer[n];
  //     }
  //   }
  //   uart_print("Copied 40MB.\r\n");
  // }
}
