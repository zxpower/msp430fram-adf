#include "main.h"
#include "adf.h"
#include "led.h"

// Configuration storage structs =============================================
struct {
    struct {
        u16 frequency_error_correction;
        u08 r_divider;
        u08 crystal_doubler;
        u08 crystal_oscillator_disable;
        u08 clock_out_divider;
        u08 vco_adjust;
        u08 output_divider;
    } r0;

    struct {
        u16 fractional_n;
        u08 integer_n;
        u08 prescaler;
    } r1;

    struct {
        u08 mod_control;
        u08 gook;
        u08 power_amplifier_level;
        u16 modulation_deviation;
        u08 gfsk_modulation_control;
        u08 index_counter;
    } r2;

    struct {
        u08 pll_enable;
        u08 pa_enable;
        u08 clkout_enable;
        u08 data_invert;
        u08 charge_pump_current;
        u08 bleed_up;
        u08 bleed_down;
        u08 vco_disable;
        u08 muxout;
        u08 ld_precision;
        u08 vco_bias;
        u08 pa_bias;
        u08 pll_test_mode;
        u08 sd_test_mode;
    } r3;
} adf_config;

// Prototypes for internal functions =========================================
void adf_reset_register_zero(void);
void adf_reset_register_one(void);
void adf_reset_register_two(void);
void adf_reset_register_three(void);
void adf_reset(void);
void adf_write_register_zero(void);
void adf_write_register_one(void);
void adf_write_register_two(void);
void adf_write_register_three(void);
void adf_write_register(u32 reg);
void adf_transmit_byte(u08 byte, u32 baud);
void adf_transmit_bit(u08 bit);
u08 adf_read_muxout(void);

// Setup functions ================================================
void adf_hw_setup(void) {
  pinAsFunction2(ADF_CLK_PORT, ADF_CLK);
  pinAsFunction2(ADF_DATA_PORT, ADF_DATA);

  pinAsData(ADF_CE_PORT, ADF_CE);
  pinAsOutput(ADF_CE_PORT, ADF_CE);
	
  pinAsData(ADF_LE_PORT, ADF_LE);
  pinAsOutput(ADF_LE_PORT, ADF_LE);

  pinAsData(ADF_TXDATA_PORT, ADF_TXDATA);
  pinAsOutput(ADF_TXDATA_PORT, ADF_TXDATA);
  
  pinAsData(ADF_MUXOUT_PORT, ADF_MUXOUT);
  pinAsInput(ADF_MUXOUT_PORT, ADF_MUXOUT);

  /*
  pinAsData(ADF_TXCLK_PORT, ADF_TXCLK);
  pinAsOutput(ADF_TXCLK_PORT, ADF_TXCLK);

  pinAsData(ADF_CLKOUT_PORT, ADF_CLKOUT);
  pinAsInput(ADF_CLKOUT_PORT, ADF_CLKOUT);
  */

  UCA1CTLW0 |= UCSWRST;                     // **Put state machine in reset**
  UCA1CTLW0 |= UCMST+UCSYNC+UCCKPH+UCCKPL+UCMSB;   // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCA1CTLW0 |= UCSSEL_2;                    // SMCLK
  UCA1BR0 = 0x01;                           // /1
  UCA1BR1 = 0;                              //
  UCA1MCTLW = 0;                            // No modulation

  UCA1CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**

}

// Configuration functions ===================================================

// Config resetting functions --------------------------------------------
void adf_reset_config(void) {
    adf_reset_register_zero();
    adf_reset_register_one();
    adf_reset_register_two();
    adf_reset_register_three();

    adf_reset();

    while(!adf_reg_ready());

    adf_write_config();
}

void adf_reset_register_zero(void) {
    adf_config.r0.frequency_error_correction = 0;
    adf_config.r0.r_divider = 1;
    adf_config.r0.crystal_doubler = ADF_OFF;
    adf_config.r0.crystal_oscillator_disable = ADF_OFF;
    adf_config.r0.clock_out_divider = 0;
    adf_config.r0.vco_adjust = 0;
    adf_config.r0.output_divider = 0;
}

void adf_reset_register_one(void) {
    adf_config.r1.fractional_n = 0;
    adf_config.r1.integer_n = 0;
    adf_config.r1.prescaler = ADF_PRESCALER_4_5;
}

void adf_reset_register_two(void) {
    adf_config.r2.mod_control = ADF_MODULATION_FSK;
    adf_config.r2.gook = ADF_OFF;
    adf_config.r2.power_amplifier_level = 0;
    adf_config.r2.modulation_deviation = 3;
    adf_config.r2.gfsk_modulation_control = 0;
    adf_config.r2.index_counter = 0;
}

void adf_reset_register_three(void) {
    adf_config.r3.pll_enable = ADF_OFF;
    adf_config.r3.pa_enable = ADF_OFF;
    adf_config.r3.clkout_enable = ADF_OFF;
    adf_config.r3.data_invert = ADF_OFF;
    adf_config.r3.charge_pump_current = ADF_CP_CURRENT_2_1;
    adf_config.r3.bleed_up = ADF_OFF;
    adf_config.r3.bleed_down = ADF_OFF;
    adf_config.r3.vco_disable = ADF_OFF;
    adf_config.r3.muxout = ADF_MUXOUT_REG_READY;
    adf_config.r3.ld_precision = ADF_LD_PRECISION_5_CYCLES;
    adf_config.r3.vco_bias = 6;
    adf_config.r3.pa_bias = 4;
    adf_config.r3.pll_test_mode = 0;
    adf_config.r3.sd_test_mode = 0;
}

void adf_reset(void) {
  pinClear(ADF_CE_PORT, ADF_CE);
  pinSet(ADF_LE_PORT, ADF_LE);
  pinSet(ADF_TXDATA_PORT, ADF_TXDATA);
  pinSet(ADF_CLK_PORT, ADF_CLK);
  pinSet(ADF_DATA_PORT, ADF_DATA);
  mdelay(1);
  pinSet(ADF_CE_PORT, ADF_CE);
  mdelay(100);
}

void adf_turn_off(void) {
  pinClear(ADF_CE_PORT, ADF_CE);
}

// Configuration writing functions ---------------------------------------
void adf_write_config(void) {
    adf_write_register_zero();
    adf_write_register_one();
    adf_write_register_two();
    adf_write_register_three();
}

void adf_write_register_zero(void) {
    u32 reg =
        (0) |
        ((adf_config.r0.frequency_error_correction  & 0x7FF) <<  2) |
        ((adf_config.r0.r_divider                   & 0xF  ) * 1ul << 13) |
        ((adf_config.r0.crystal_doubler             & 0x1  ) * 1ul << 17) |
        ((adf_config.r0.crystal_oscillator_disable  & 0x1  ) * 1ul << 18) |
        ((adf_config.r0.clock_out_divider           & 0xF  ) * 1ul << 19) |
        ((adf_config.r0.vco_adjust                  & 0x3  ) * 1ul << 23) |
        ((adf_config.r0.output_divider              & 0x3  ) * 1ul << 25);
    adf_write_register(reg);
}

void adf_write_register_one(void) {
    u32 reg =
        (1) |
        ((adf_config.r1.fractional_n                & 0xFFF) <<  2) |
        ((adf_config.r1.integer_n                   & 0xFF ) * 1ul << 14) |
        ((adf_config.r1.prescaler                   & 0x1  ) * 1ul << 22);
    adf_write_register(reg);
}

void adf_write_register_two(void) {
    u32 reg =
        (2) |
        ((adf_config.r2.mod_control                 & 0x3  ) <<  2) |
        ((adf_config.r2.gook                        & 0x1  ) <<  4) |
        ((adf_config.r2.power_amplifier_level       & 0x3F ) <<  5) |
        ((adf_config.r2.modulation_deviation        & 0x1FF) * 1ul << 11) |
        ((adf_config.r2.gfsk_modulation_control     & 0x7  ) * 1ul << 20) |
        ((adf_config.r2.index_counter               & 0x3  ) * 1ul << 23);
    adf_write_register(reg);
}

void adf_write_register_three(void) {
    u32 reg =
        (3) |
        ((adf_config.r3.pll_enable                  & 0x1  ) <<  2) |
        ((adf_config.r3.pa_enable                   & 0x1  ) <<  3) |
        ((adf_config.r3.clkout_enable               & 0x1  ) <<  4) |
        ((adf_config.r3.data_invert                 & 0x1  ) <<  5) |
        ((adf_config.r3.charge_pump_current         & 0x3  ) <<  6) |
        ((adf_config.r3.bleed_up                    & 0x1  ) <<  8) |
        ((adf_config.r3.bleed_down                  & 0x1  ) <<  9) |
        ((adf_config.r3.vco_disable                 & 0x1  ) << 10) |
        ((adf_config.r3.muxout                      & 0xF  ) << 11) |
        ((adf_config.r3.ld_precision                & 0x1  ) * 1ul << 15) |
        ((adf_config.r3.vco_bias                    & 0xF  ) * 1ul << 16) |
        ((adf_config.r3.pa_bias                     & 0x7  ) * 1ul << 20) |
        ((adf_config.r3.pll_test_mode               & 0x1F ) * 1ul << 23) |
        ((adf_config.r3.sd_test_mode                & 0xF  ) * 1ul << 28);
    adf_write_register(reg);
}

void adf_write_register(u32 data) {
    s08 i;

    pinClear(ADF_CLK_PORT, ADF_CLK);
    udelay(20);

    pinClear(ADF_LE_PORT, ADF_LE);
    udelay(100);

    for(i=31; i>=0; i--) {
        if((data & (1<<i))>>i)
            pinSet(ADF_DATA_PORT, ADF_DATA);
        else
            pinClear(ADF_DATA_PORT, ADF_DATA);
        udelay(20);
        pinSet(ADF_CLK_PORT, ADF_CLK);
        udelay(100);
        pinClear(ADF_CLK_PORT, ADF_CLK);
        udelay(100);
    }

    udelay(20);
    pinSet(ADF_LE_PORT, ADF_LE);
}

// Configuration setting functions ---------------------------------------
void adf_set_frequency_error_correction(u16 error) {
    adf_config.r0.frequency_error_correction = error;
}

void adf_set_r_divider(u08 r) {
    adf_config.r0.r_divider = r;
}

void adf_set_vco_adjust(u08 adjust) {
    adf_config.r0.vco_adjust = adjust;
}

void adf_set_frequency(void) {
    u08 n = 41;
    u16 m = 1189;
    adf_set_n(n);
    adf_set_m(m);
}

void adf_set_n(u08 n) {
    adf_config.r1.integer_n = n;
}

void adf_set_m(u16 m) {
    adf_config.r1.fractional_n = m;
}

void adf_set_pa_level(u08 level) {
    adf_config.r2.power_amplifier_level = level;
}

void adf_set_pll_enable(u08 enable) {
    adf_config.r3.pll_enable = enable;
}

void adf_set_pa_enable(u08 enable) {
    adf_config.r3.pa_enable = enable;
}

void adf_set_clkout_enable(u08 enable) {
    adf_config.r3.clkout_enable = enable;
}

void adf_set_charge_pump_current(u08 current) {
    adf_config.r3.charge_pump_current = current;
}

void adf_set_vco_disable(u08 disable) {
    adf_config.r3.vco_disable = disable;
}

void adf_set_muxout(u08 muxout) {
    adf_config.r3.muxout = muxout;
}

void adf_set_vco_bias(u08 bias) {
    adf_config.r3.vco_bias = bias;
}

void adf_set_pa_bias(u08 bias) {
    adf_config.r3.pa_bias = bias;
}

u08 adf_get_n() {
    return adf_config.r1.integer_n;
}

u16 adf_get_m() {
    return adf_config.r1.fractional_n;
}

u08 adf_get_vco_adjust(void) {
    return adf_config.r0.vco_adjust;
}

u08 adf_get_vco_bias(void) {
    return adf_config.r3.vco_bias;
}

void adf_setup(void) {
    adf_reset_config();
    adf_set_r_divider(2);
    adf_set_frequency();
    adf_set_muxout(ADF_MUXOUT_DIGITAL_LOCK);
    adf_set_pll_enable(ADF_ON);
    adf_write_config();
}

u08 adf_lock(void) {
    u08 adj = 0, bias = 5;
    while(!adf_locked()) {
        adf_set_vco_adjust(adj);
        adf_set_vco_bias(bias);
        adf_write_config();
        mdelay(50);
        if(++bias == 14) {
            bias = 1;
            if(++adj == 4) {
                return 0;
            }
        }
    }
    return 1;
}

void adf_power_on(void) {
    adf_set_pa_enable(ADF_ON);
    adf_set_pa_level(60);
    adf_write_config();
}

void adf_power_off(void) {
    adf_set_pa_enable(ADF_OFF);
    adf_write_config();
}

// Data transmission functions ===============================================
void adf_transmit_string(char* data, u32 baud) {
    u32 i;
    for(i=0; data[i]; i++)
        adf_transmit_byte(data[i], baud);
}

void adf_transmit_byte(u08 byte, u32 baud) {
  s08 i;
    
  led_turn_on(LEDSBLOCK1,LED4);
  mdelay(10);
  led_turn_off(LEDSBLOCK1,LED4);
  mdelay(10);
  led_turn_on(LEDSBLOCK1,LED4);
  mdelay(10);
  led_turn_off(LEDSBLOCK1,LED4);

  // Start bit
  adf_transmit_bit(0);
  //udelay(baud);
  mdelay(1);

  // Data byte
  for(i=0; i<8; i++) {
    if(byte & 1<<i)
      adf_transmit_bit(1);
    else
      adf_transmit_bit(0);
    //udelay(baud);
    mdelay(1);
  }

  // Stop bits
  adf_transmit_bit(1);
  //udelay(baud);
  //udelay(baud);
  mdelay(1);
  mdelay(1);
}

void adf_transmit_bit(u08 bit) {
    if(bit) {
        pinSet(ADF_TXDATA_PORT, ADF_TXDATA);
    } else {
        pinClear(ADF_TXDATA_PORT, ADF_TXDATA);
    }
}

// MUXOUT reading functions ==================================================
u08 adf_read_muxout(void) {
    return (u08)(pinRead(ADF_MUXOUT_PORT, ADF_MUXOUT) > 0);
}

u08 adf_locked(void) {
    return adf_read_muxout();
}

u08 adf_reg_ready(void) {
    return adf_read_muxout();
}
