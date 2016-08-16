#ifndef PINOUT_H
#define PINOUT_H

static LocalFileSystem local("local");
static Serial pc(USBTX, USBRX);//tx, rx => for debugging purposes

// TI DAC7578 interface
//static BusOut CLR_BAR(p5, p7);
//static BusOut LDAC_BAR(p6, p8);
static I2C dac_i2c(p9, p10); //sda, scl

static I2C lcd(p9, p10);

// LED Indicators
static DigitalOut power_indicator (LED1);
static DigitalOut power_error_indicator (LED4);
static DigitalOut wait_indicator (LED2);

// To Chip
static DigitalOut PORESETn(p11);
static DigitalOut CORERESETn(p12);

// From Chip
static DigitalIn  HCLK_div_down(p5);
static DigitalIn  RO_clock_out(p6);

// GPIO to/from Chip
//static BusInOut GPIO(p7, p16, p15);
static DigitalInOut GPIO1(p7);
static DigitalInOut GPIO2(p15);
static DigitalInOut GPIO3(p16);

// Analog in from amplifier (for power measurement)
static AnalogIn  meas_sen(p17);
static AnalogIn  meas_mem2(p18);
static AnalogIn  meas_mem1(p19);
static AnalogIn  meas_core(p20);
static DigitalOut gain_ctrl(p8);

// Scan
static DigitalOut scan_data_in(p30);
static DigitalOut scan_phi(p29);
static DigitalOut scan_phi_bar(p13);
static DigitalOut scan_load_chain(p14);
static DigitalOut scan_load_chip(p26);
static DigitalIn  scan_data_out(p25);

// JTAG
static DigitalOut TCK(p24);
static DigitalOut TMS(p23);
static DigitalOut TDI(p22);
static DigitalIn  TDO(p21);

#endif



#define POWER_UP_TIME 0.01

// DAC0, ADDR=1
#define ADVDD           ADDR_1,CHAN_A
#define PADVDD          ADDR_1,CHAN_B
#define DVDD            ADDR_1,CHAN_C
#define ADVDD2          ADDR_1,CHAN_D
#define COREVDD         ADDR_1,CHAN_E
#define DVDD2           ADDR_1,CHAN_F
#define SENSORVDD       ADDR_1,CHAN_G
#define SENSORLOWVDD    ADDR_1,CHAN_H

// DAC1, ADDR=0
#define SENSORSTRESSVDD ADDR_0,CHAN_A
#define CLOCKVDD        ADDR_0,CHAN_B
#define MEM1VDD         ADDR_0,CHAN_C
#define MEM2VDD         ADDR_0,CHAN_D
#define PLLAVDD         ADDR_0,CHAN_E
#define RING_OSC_NBIAS  ADDR_0,CHAN_F

// 20 MHz Crystal
#define PLL_REF 20000