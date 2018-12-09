#define DEFE0_BASE (uint32_t)0x01E00000
#define DEBE0_BASE (uint32_t)0x01E60000
#define CCM_BASE   (uint32_t)0x01C20000
#define HDMI_BASE  (uint32_t)0x01C16000

#define DRAM_BASE  (uint32_t)0x40000000
#define DRAM_SIZE  (uint32_t)0x40000000

#define BUFFER_0_DATA_ADDR (DRAM_BASE + 0x000000)
#define BUFFER_1_DATA_ADDR (DRAM_BASE + 0x100000)

void display_init();

struct sunxi_de_fe_reg {
  uint32_t enable;     /* 0x000 */
  uint32_t frame_ctrl;     /* 0x004 */
  uint32_t bypass;     /* 0x008 */
  uint32_t algorithm_sel;    /* 0x00c */
  uint32_t line_int_ctrl;    /* 0x010 */
  uint8_t  res0[0x0c];      /* 0x014 */
  uint32_t ch0_addr;     /* 0x020 */
  uint32_t ch1_addr;     /* 0x024 */
  uint32_t ch2_addr;     /* 0x028 */
  uint32_t field_sequence;   /* 0x02c */
  uint32_t ch0_offset;     /* 0x030 */
  uint32_t ch1_offset;     /* 0x034 */
  uint32_t ch2_offset;     /* 0x038 */
  uint8_t  res1[0x04];      /* 0x03c */
  uint32_t ch0_stride;     /* 0x040 */
  uint32_t ch1_stride;     /* 0x044 */
  uint32_t ch2_stride;     /* 0x048 */
  uint32_t input_fmt;      /* 0x04c */
  uint32_t ch3_addr;     /* 0x050 */
  uint32_t ch4_addr;     /* 0x054 */
  uint32_t ch5_addr;     /* 0x058 */
  uint32_t output_fmt;     /* 0x05c */
  uint32_t int_enable;     /* 0x060 */
  uint32_t int_status;     /* 0x064 */
  uint32_t status;     /* 0x068 */
  uint8_t  res2[0x04];      /* 0x06c */
  uint32_t csc_coef00;     /* 0x070 */
  uint32_t csc_coef01;     /* 0x074 */
  uint32_t csc_coef02;     /* 0x078 */
  uint32_t csc_coef03;     /* 0x07c */
  uint32_t csc_coef10;     /* 0x080 */
  uint32_t csc_coef11;     /* 0x084 */
  uint32_t csc_coef12;     /* 0x088 */
  uint32_t csc_coef13;     /* 0x08c */
  uint32_t csc_coef20;     /* 0x090 */
  uint32_t csc_coef21;     /* 0x094 */
  uint32_t csc_coef22;     /* 0x098 */
  uint32_t csc_coef23;     /* 0x09c */
  uint32_t deinterlace_ctrl;   /* 0x0a0 */
  uint32_t deinterlace_diag;   /* 0x0a4 */
  uint32_t deinterlace_tempdiff; /* 0x0a8 */
  uint32_t deinterlace_sawtooth; /* 0x0ac */
  uint32_t deinterlace_spatcomp; /* 0x0b0 */
  uint32_t deinterlace_burstlen; /* 0x0b4 */
  uint32_t deinterlace_preluma;  /* 0x0b8 */
  uint32_t deinterlace_tile_addr;  /* 0x0bc */
  uint32_t deinterlace_tile_stride;  /* 0x0c0 */
  uint8_t  res3[0x0c];      /* 0x0c4 */
  uint32_t wb_stride_enable;   /* 0x0d0 */
  uint32_t ch3_stride;     /* 0x0d4 */
  uint32_t ch4_stride;     /* 0x0d8 */
  uint32_t ch5_stride;     /* 0x0dc */
  uint32_t fe_3d_ctrl;     /* 0x0e0 */
  uint32_t fe_3d_ch0_addr;   /* 0x0e4 */
  uint32_t fe_3d_ch1_addr;   /* 0x0e8 */
  uint32_t fe_3d_ch2_addr;   /* 0x0ec */
  uint32_t fe_3d_ch0_offset;   /* 0x0f0 */
  uint32_t fe_3d_ch1_offset;   /* 0x0f4 */
  uint32_t fe_3d_ch2_offset;   /* 0x0f8 */
  uint8_t  res4[0x04];      /* 0x0fc */
  uint32_t ch0_insize;     /* 0x100 */
  uint32_t ch0_outsize;    /* 0x104 */
  uint32_t ch0_horzfact;   /* 0x108 */
  uint32_t ch0_vertfact;   /* 0x10c */
  uint32_t ch0_horzphase;    /* 0x110 */
  uint32_t ch0_vertphase0;   /* 0x114 */
  uint32_t ch0_vertphase1;   /* 0x118 */
  uint8_t  res5[0x04];      /* 0x11c */
  uint32_t ch0_horztapoffset0;   /* 0x120 */
  uint32_t ch0_horztapoffset1;   /* 0x124 */
  uint32_t ch0_verttapoffset;    /* 0x128 */
  uint8_t  res6[0xd4];      /* 0x12c */
  uint32_t ch1_insize;     /* 0x200 */
  uint32_t ch1_outsize;    /* 0x204 */
  uint32_t ch1_horzfact;   /* 0x208 */
  uint32_t ch1_vertfact;   /* 0x20c */
  uint32_t ch1_horzphase;    /* 0x210 */
  uint32_t ch1_vertphase0;   /* 0x214 */
  uint32_t ch1_vertphase1;   /* 0x218 */
  uint8_t  res7[0x04];      /* 0x21c */
  uint32_t ch1_horztapoffset0;   /* 0x220 */
  uint32_t ch1_horztapoffset1;   /* 0x224 */
  uint32_t ch1_verttapoffset;    /* 0x228 */
  uint8_t  res8[0x1d4];     /* 0x22c */
  uint32_t ch0_horzcoef0[32];    /* 0x400 */
  uint32_t ch0_horzcoef1[32];    /* 0x480 */
  uint32_t ch0_vertcoef[32];   /* 0x500 */
  uint8_t  res9[0x80];      /* 0x580 */
  uint32_t ch1_horzcoef0[32];    /* 0x600 */
  uint32_t ch1_horzcoef1[32];    /* 0x680 */
  uint32_t ch1_vertcoef[32];   /* 0x700 */
  uint8_t  res10[0x280];    /* 0x780 */
  uint32_t vpp_enable;     /* 0xa00 */
  uint32_t vpp_dcti;     /* 0xa04 */
  uint32_t vpp_lp1;      /* 0xa08 */
  uint32_t vpp_lp2;      /* 0xa0c */
  uint32_t vpp_wle;      /* 0xa10 */
  uint32_t vpp_ble;      /* 0xa14 */
};

struct sunxi_de_be_reg {
  uint8_t  res0[0x800];     /* 0x000 */
  uint32_t mode;     /* 0x800 */
  uint32_t backcolor;      /* 0x804 */
  uint32_t disp_size;      /* 0x808 */
  uint8_t  res1[0x4];     /* 0x80c */
  uint32_t layer0_size;    /* 0x810 */
  uint32_t layer1_size;    /* 0x814 */
  uint32_t layer2_size;    /* 0x818 */
  uint32_t layer3_size;    /* 0x81c */
  uint32_t layer0_pos;     /* 0x820 */
  uint32_t layer1_pos;     /* 0x824 */
  uint32_t layer2_pos;     /* 0x828 */
  uint32_t layer3_pos;     /* 0x82c */
  uint8_t  res2[0x10];      /* 0x830 */
  uint32_t layer0_stride;    /* 0x840 */
  uint32_t layer1_stride;    /* 0x844 */
  uint32_t layer2_stride;    /* 0x848 */
  uint32_t layer3_stride;    /* 0x84c */
  uint32_t layer0_addr_low32b;   /* 0x850 */
  uint32_t layer1_addr_low32b;   /* 0x854 */
  uint32_t layer2_addr_low32b;   /* 0x858 */
  uint32_t layer3_addr_low32b;   /* 0x85c */
  uint32_t layer0_addr_high4b;   /* 0x860 */
  uint32_t layer1_addr_high4b;   /* 0x864 */
  uint32_t layer2_addr_high4b;   /* 0x868 */
  uint32_t layer3_addr_high4b;   /* 0x86c */
  uint32_t reg_ctrl;     /* 0x870 */
  uint8_t  res3[0xc];     /* 0x874 */
  uint32_t color_key_max;    /* 0x880 */
  uint32_t color_key_min;    /* 0x884 */
  uint32_t color_key_config;   /* 0x888 */
  uint8_t  res4[0x4];     /* 0x88c */
  uint32_t layer0_attr0_ctrl;    /* 0x890 */
  uint32_t layer1_attr0_ctrl;    /* 0x894 */
  uint32_t layer2_attr0_ctrl;    /* 0x898 */
  uint32_t layer3_attr0_ctrl;    /* 0x89c */
  uint32_t layer0_attr1_ctrl;    /* 0x8a0 */
  uint32_t layer1_attr1_ctrl;    /* 0x8a4 */
  uint32_t layer2_attr1_ctrl;    /* 0x8a8 */
  uint32_t layer3_attr1_ctrl;    /* 0x8ac */
  uint8_t  res5[0x110];     /* 0x8b0 */
  uint32_t output_color_ctrl;    /* 0x9c0 */
  uint8_t  res6[0xc];     /* 0x9c4 */
  uint32_t output_color_coef[12];  /* 0x9d0 */
};

struct sunxi_ccm_reg {
  uint32_t pll1_cfg;   /* 0x00 pll1 control */
  uint32_t pll1_tun;   /* 0x04 pll1 tuning */
  uint32_t pll2_cfg;   /* 0x08 pll2 control */
  uint32_t pll2_tun;   /* 0x0c pll2 tuning */
  uint32_t pll3_cfg;   /* 0x10 pll3 control */
  uint8_t  res0[0x4];
  uint32_t pll4_cfg;   /* 0x18 pll4 control */
  uint8_t  res1[0x4];
  uint32_t pll5_cfg;   /* 0x20 pll5 control */
  uint32_t pll5_tun;   /* 0x24 pll5 tuning */
  uint32_t pll6_cfg;   /* 0x28 pll6 control */
  uint32_t pll6_tun;   /* 0x2c pll6 tuning */
  uint32_t pll7_cfg;   /* 0x30 pll7 control */
  uint32_t pll1_tun2;    /* 0x34 pll5 tuning2 */
  uint8_t  res2[0x4];
  uint32_t pll5_tun2;    /* 0x3c pll5 tuning2 */
  uint8_t  res3[0xc];
  uint32_t pll_lock_dbg; /* 0x4c pll lock time debug */
  uint32_t osc24m_cfg;   /* 0x50 osc24m control */
  uint32_t cpu_ahb_apb0_cfg; /* 0x54 cpu,ahb and apb0 divide ratio */
  uint32_t apb1_clk_div_cfg; /* 0x58 apb1 clock dividor */
  uint32_t axi_gate;   /* 0x5c axi module clock gating */
  uint32_t ahb_gate0;    /* 0x60 ahb module clock gating 0 */
  uint32_t ahb_gate1;    /* 0x64 ahb module clock gating 1 */
  uint32_t apb0_gate;    /* 0x68 apb0 module clock gating */
  uint32_t apb1_gate;    /* 0x6c apb1 module clock gating */
  uint8_t  res4[0x10];
  uint32_t nand0_clk_cfg;  /* 0x80 nand sub clock control */
  uint32_t ms_sclk_cfg;  /* 0x84 memory stick sub clock control */
  uint32_t sd0_clk_cfg;  /* 0x88 sd0 clock control */
  uint32_t sd1_clk_cfg;  /* 0x8c sd1 clock control */
  uint32_t sd2_clk_cfg;  /* 0x90 sd2 clock control */
  uint32_t sd3_clk_cfg;  /* 0x94 sd3 clock control */
  uint32_t ts_clk_cfg;   /* 0x98 transport stream clock control */
  uint32_t ss_clk_cfg;   /* 0x9c */
  uint32_t spi0_clk_cfg; /* 0xa0 */
  uint32_t spi1_clk_cfg; /* 0xa4 */
  uint32_t spi2_clk_cfg; /* 0xa8 */
  uint32_t pata_clk_cfg; /* 0xac */
  uint32_t ir0_clk_cfg;  /* 0xb0 */
  uint32_t ir1_clk_cfg;  /* 0xb4 */
  uint32_t iis_clk_cfg;  /* 0xb8 */
  uint32_t ac97_clk_cfg; /* 0xbc */
  uint32_t spdif_clk_cfg;  /* 0xc0 */
  uint32_t keypad_clk_cfg; /* 0xc4 */
  uint32_t sata_clk_cfg; /* 0xc8 */
  uint32_t usb_clk_cfg;  /* 0xcc */
  uint32_t gps_clk_cfg;  /* 0xd0 */
  uint32_t spi3_clk_cfg; /* 0xd4 */
  uint8_t  res5[0x28];
  uint32_t dram_clk_gate;  /* 0x100 */
  uint32_t be0_clk_cfg;  /* 0x104 */
  uint32_t be1_clk_cfg;  /* 0x108 */
  uint32_t fe0_clk_cfg;  /* 0x10c */
  uint32_t fe1_clk_cfg;  /* 0x110 */
  uint32_t mp_clk_cfg;   /* 0x114 */
  uint32_t lcd0_ch0_clk_cfg; /* 0x118 */
  uint32_t lcd1_ch0_clk_cfg; /* 0x11c */
  uint32_t csi_isp_clk_cfg;  /* 0x120 */
  uint8_t  res6[0x4];
  uint32_t tvd_clk_reg;  /* 0x128 */
  uint32_t lcd0_ch1_clk_cfg; /* 0x12c */
  uint32_t lcd1_ch1_clk_cfg; /* 0x130 */
  uint32_t csi0_clk_cfg; /* 0x134 */
  uint32_t csi1_clk_cfg; /* 0x138 */
  uint32_t ve_clk_cfg;   /* 0x13c */
  uint32_t audio_codec_clk_cfg;  /* 0x140 */
  uint32_t avs_clk_cfg;  /* 0x144 */
  uint32_t ace_clk_cfg;  /* 0x148 */
  uint32_t lvds_clk_cfg; /* 0x14c */
  uint32_t hdmi_clk_cfg; /* 0x150 */
  uint32_t mali_clk_cfg; /* 0x154 */
  uint8_t  res7[0x4];
  uint32_t mbus_clk_cfg; /* 0x15c */
  uint8_t  res8[0x4];
  uint32_t gmac_clk_cfg; /* 0x164 */
};

struct sunxi_hdmi_reg {
  uint32_t version_id;     /* 0x000 */
  uint32_t ctrl;     /* 0x004 */
  uint32_t irq;      /* 0x008 */
  uint32_t hpd;      /* 0x00c */
  uint32_t video_ctrl;     /* 0x010 */
  uint32_t video_size;     /* 0x014 */
  uint32_t video_bp;     /* 0x018 */
  uint32_t video_fp;     /* 0x01c */
  uint32_t video_spw;      /* 0x020 */
  uint32_t video_polarity;   /* 0x024 */
  uint8_t  res0[0x58];      /* 0x028 */
  uint8_t  avi_info_frame[0x14];  /* 0x080 */
  uint8_t  res1[0x4c];      /* 0x094 */
  uint32_t qcp_packet0;    /* 0x0e0 */
  uint32_t qcp_packet1;    /* 0x0e4 */
  uint8_t  res2[0x118];     /* 0x0e8 */
  uint32_t pad_ctrl0;      /* 0x200 */
  uint32_t pad_ctrl1;      /* 0x204 */
  uint32_t pll_ctrl;     /* 0x208 */
  uint32_t pll_dbg0;     /* 0x20c */
  uint32_t pll_dbg1;     /* 0x210 */
  uint32_t hpd_cec;      /* 0x214 */
  uint8_t  res3[0x28];      /* 0x218 */
  uint8_t  vendor_info_frame[0x14]; /* 0x240 */
  uint8_t  res4[0x9c];      /* 0x254 */
  uint32_t pkt_ctrl0;      /* 0x2f0 */
  uint32_t pkt_ctrl1;      /* 0x2f4 */
  uint8_t  res5[0x8];     /* 0x2f8 */
  uint32_t unknown;      /* 0x300 */
  uint8_t  res6[0xc];     /* 0x304 */
  uint32_t audio_sample_count;   /* 0x310 */
  uint8_t  res7[0xec];      /* 0x314 */
  uint32_t audio_tx_fifo;    /* 0x400 */
  uint8_t  res8[0xfc];      /* 0x404 */
  uint32_t ddc_ctrl;     /* 0x500 */
  uint32_t ddc_exreg;      /* 0x504 */
  uint32_t ddc_cmnd;     /* 0x508 */
  uint32_t ddc_addr;     /* 0x50c */
  uint32_t ddc_int_mask;   /* 0x510 */
  uint32_t ddc_int_status;   /* 0x514 */
  uint32_t ddc_fifo_ctrl;    /* 0x518 */
  uint32_t ddc_fifo_status;    /* 0x51c */
  uint32_t ddc_clock;      /* 0x520 */
  uint32_t ddc_timeout;    /* 0x524 */
  uint8_t  res9[0x18];      /* 0x528 */
  uint32_t ddc_dbg;      /* 0x540 */
  uint8_t  res10[0x3c];     /* 0x544 */
  uint32_t ddc_fifo_data;    /* 0x580 */
};
