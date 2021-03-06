#ifndef LYNXNETEMERGENCY_ADF_H
#define LYNXNETEMERGENCY_ADF_H

// ADF API ===================================================================
void adf_hw_setup(void);
void adf_turn_off(void);
void adf_reset_config(void);
void adf_write_config(void);
void adf_transmit_string(char* data, u32 baud);
u08 adf_locked(void);
u08 adf_reg_ready(void);

void adf_set_frequency_error_correction(u16 error);
void adf_set_r_divider(u08 r);
void adf_set_vco_adjust(u08 adjust);
void adf_set_frequency(void);
void adf_set_n(u08 n);
void adf_set_m(u16 m);
void adf_set_pa_level(u08 level);
void adf_set_pll_enable(u08 enable);
void adf_set_pa_enable(u08 enable);
void adf_set_clkout_enable(u08 enable);
void adf_set_charge_pump_current(u08 current);
void adf_set_vco_disable(u08 disable);
void adf_set_muxout(u08 muxout);
void adf_set_vco_bias(u08 bias);
void adf_set_pa_bias(u08 bias);

void adf_setup(void);
void adf_power_on(void);
void adf_power_off(void);
u08 adf_lock(void);

u08 adf_get_n(void);
u16 adf_get_m(void);
u08 adf_get_vco_adjust(void);
u08 adf_get_vco_bias(void);

// The ADF7012 pin connections ===============================================
#define ADF_CLK         4
#define ADF_CLK_PORT    2
#define ADF_DATA        5
#define ADF_DATA_PORT   2
#define ADF_CE          0
#define ADF_CE_PORT     1
#define ADF_LE          1
#define ADF_LE_PORT     1
#define ADF_MUXOUT      1
#define ADF_MUXOUT_PORT 2
#define ADF_TXDATA      3
#define ADF_TXDATA_PORT 1
/*#define ADF_CLKOUT      GPIO7
#define ADF_CLKOUT_PORT GPIOA
#define ADF_TXCLK       GPIO13
#define ADF_TXCLK_PORT  GPIOB*/

// Timing constants ==========================================================
#define ADF_50_BAUD     (u32)(479441)
#define ADF_300_BAUD    (u32)(79915)
#define ADF_1200_BAUD   (u32)(19979)

// General Purpose ===========================================================
#define ADF_OFF 0
#define ADF_ON  1

// Register Constants ========================================================
// Register 1 ----------------------------------------------------------------
#define ADF_PRESCALER_4_5 0
#define ADF_PRESCALER_8_9 1

// Register 2 ----------------------------------------------------------------
#define ADF_MODULATION_FSK  0
#define ADF_MODULATION_GFSK 1
#define ADF_MODULATION_ASK  2
#define ADF_MODULATION_OOK  3

// Register 3 ----------------------------------------------------------------
#define ADF_CP_CURRENT_0_3 0
#define ADF_CP_CURRENT_0_9 1
#define ADF_CP_CURRENT_1_5 2
#define ADF_CP_CURRENT_2_1 3
#define ADF_MUXOUT_LOGIC_LOW 0
#define ADF_MUXOUT_LOGIC_HIGH 1
#define ADF_MUXOUT_REG_READY 3
#define ADF_MUXOUT_DIGITAL_LOCK 4
#define ADF_MUXOUT_ANALOGUE_LOCK 5
#define ADF_MUXOUT_R_DIVIDER_2 6
#define ADF_MUXOUT_N_DIVIDER_2 7
#define ADF_MUXOUT_RF_R_DIVIDER 8
#define ADF_MUXOUT_DATA_RATE 9
#define ADF_MUXOUT_BATT_2_35 10
#define ADF_MUXOUT_BATT_2_75 11
#define ADF_MUXOUT_BATT_3 12
#define ADF_MUXOUT_BATT_3_25 13
#define ADF_MUXOUT_TEST_MODE 14
#define ADF_MUXOUT_SD_TEST_MODE 15
#define ADF_LD_PRECISION_3_CYCLES 0
#define ADF_LD_PRECISION_5_CYCLES 1

#endif /*LYNXNETEMERGENCY_ADF_H*/
