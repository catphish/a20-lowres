#include "uart.h"
#include "display.h"
#include <stdint.h>

#define DRAM_BASE  (uint32_t)0x40000000
#define DRAM_SIZE  (uint32_t)0x40000000

#define LAYER0_DATA_ADDR (DRAM_BASE + DRAM_SIZE - 0x1000000)
volatile uint32_t * layer0_data = (volatile uint32_t *)LAYER0_DATA_ADDR;

void display_init() {
  // Enable FE0 clocks
  struct sunxi_ccm_reg * const ccm =(struct sunxi_ccm_reg *)CCM_BASE;
  ccm->ahb_gate1 |= (1<<14);     // AHB Clock -> FE0
  ccm->dram_clk_gate |= (1<<25); // FE0 -> DRAM
  ccm->fe0_clk_cfg = 0xC2000002; // FE0 Clock

  // Configure FE0
  struct sunxi_de_fe_reg * const de_fe =(struct sunxi_de_fe_reg *)DEFE0_BASE;
  de_fe->enable = 1; // Enable
  // The code below sets the scaling taps and coefficients so that only
  // the tap at 0x0 is active, disabling antialiasing.
  for (int i = 0; i < 32; i++) {
    // Configure the scaling coefficients to 0x40,0,0,0(,0,0,0,0)
    de_fe->ch0_horzcoef0[i] = 0x00000040;
    de_fe->ch1_horzcoef0[i] = 0x00000040;
    de_fe->ch0_horzcoef1[i] = 0x00000000;
    de_fe->ch1_horzcoef1[i] = 0x00000000;
    de_fe->ch0_vertcoef[i]  = 0x00000040;
    de_fe->ch1_vertcoef[i]  = 0x00000040;
  }
  // Set tap offsets to 0,1,1,1(,1,1,1,1)
  de_fe->ch0_horztapoffset0 = 0x01010100;
  de_fe->ch1_horztapoffset0 = 0x01010100;
  de_fe->ch0_horztapoffset1 = 0x01010101;
  de_fe->ch1_horztapoffset1 = 0x01010101;
  de_fe->ch0_verttapoffset  = 0x01010100;
  de_fe->ch1_verttapoffset  = 0x01010100;

  // Contigure the remainder of FE0
  de_fe->frame_ctrl |= 2;
  de_fe->algorithm_sel = (1<<8);
  de_fe->ch0_addr = LAYER0_DATA_ADDR;
  de_fe->ch0_stride = 480*4;
  de_fe->input_fmt = 0x151;
  de_fe->output_fmt = 0x2;
  de_fe->bypass = 2;
  de_fe->ch0_insize = ((1080/4-1)<<16) | (1920/4-1);
  de_fe->ch1_insize = ((1080/4-1)<<16) | (1920/4-1);
  de_fe->ch0_outsize = (1079<<16) | 1919;
  de_fe->ch1_outsize = (1079<<16) | 1919;
  de_fe->ch0_horzfact = (0<<16)|0x4000;
  de_fe->ch1_horzfact = (0<<16)|0x4000;
  de_fe->ch0_vertfact = (0<<16)|0x4000;
  de_fe->ch1_vertfact = (0<<16)|0x4000;
  de_fe->line_int_ctrl = 65;
  de_fe->int_enable = (1<<9);
  // Start rendering frames
  de_fe->frame_ctrl |= 1;
  de_fe->frame_ctrl |= (1<<16);

  // Configure BE0
  // This is not complete and relies on several steps performed by u-boot
  struct sunxi_de_be_reg * const de_be =(struct sunxi_de_be_reg *)DEBE0_BASE;
  de_be->mode = (1<<8) | 3;
  de_be->layer0_size = (1079<<16) | 1919; // 1920x1080
  de_be->layer0_attr0_ctrl = 2;
  de_be->layer0_attr1_ctrl = 9<<8;

  // Configure HDMI for 1920x1080
  // This is not complete and relies on several steps performed by u-boot
  struct sunxi_hdmi_reg * const hdmi =(struct sunxi_hdmi_reg *)HDMI_BASE;
  hdmi->ctrl = (1<<31); // Enable HDMI
  hdmi->video_ctrl = (1<<31)|(1<<30); // Enable video output in HDMI mode
  hdmi->video_size = (1079<<16) | 1919; // Thie and below settings are for 1080p
  hdmi->video_bp = ((36+5-1)<<16) | (148+44-1); // Back porch + Sync pulse (??)
  hdmi->video_fp = ((4-1)<<16) | (88-1); //Front porch
  hdmi->video_spw = ((5-1)<<16) | (44-1); // Sync pulse

}