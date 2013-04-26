#include "power_up.h"

using namespace std;

#define ADDR 0x98 //change to 94 or 98 if doesn't work. MARK: Use 0x94
#define POWER_UP_TIME 0.3

I2C i2c(p9, p10);       //sda, scl
DigitalOut ADDR0(p6);   //ADDR0 in DAC
Serial power_pc(USBTX, USBRX);//tx, rx => for debugging purposes
DigitalOut power_indicator (LED1);
DigitalOut power_error_indicator (LED4);
DigitalOut LDAC_BAR(p7);
DigitalOut CLR_BAR(p5);

void power(const int A, char CA, char MSDB, char LSDB)
{
    int state;
    char data[3];
    data[0]=CA;
    data[1]=MSDB;
    data[2]=LSDB;
    debugPower();
    power_error_indicator = 1;
    //pc.printf ("%X\n", A);
    while (i2c.write(A,data,3,false)) {
    }
    power_error_indicator = 0;
}

void powerUp(double voltage)
{
    if (voltage > 1 || voltage <0.6) {
        voltage = 0.9;
    }
    unsigned int volt = (voltage/3.3*65535);
    char MSDB, LSDB;
    LSDB = volt % 0x100;
    MSDB = volt / 0x100;
    voltage += 0.05;
    if(voltage >1) {
        voltage = 1;
    };
    unsigned int boosted_volt = (voltage/3.3*65535);
    char MSDB2, LSDB2;
    LSDB2 = volt % 0x100;
    MSDB2 = volt / 0x100;
    //power_pc.printf("%x%x\n", MSDB, LSDB);
    power(ADDR,0x31,0x8B,0xA0) ;  //update channel B 1.8V    DVDD2
    wait(POWER_UP_TIME);
    power(ADDR,0x37,0x4D,0x90) ;  //update channel H 1V      AVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x35,0x26,0xC0) ;  //update channel F 0.5V    AVDD2
    wait(POWER_UP_TIME);
    power(ADDR,0x34,MSDB2,LSDB2) ;  //update channel E 1V      WRAPPERVD
    wait(POWER_UP_TIME);
    power(ADDR,0x32,MSDB,LSDB) ;  //update channel C 1V      COREVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x30,MSDB2,LSDB2) ;  //update channel A 1V      SRAMVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x36,MSDB,LSDB) ;  //update channel G 1V      SENSEVDD
    wait(POWER_UP_TIME);
    power_indicator = 1;
    power(ADDR,0x33,0xFF,0xF0) ;  //update channel D 3.3V    DVDD
    wait(POWER_UP_TIME);
}

void powerAdjust(double voltage) {
    if(voltage >1) {
        voltage = 1;
    }
    if (voltage > 1 || voltage <0.6) {
        voltage = 1;
    }
    unsigned int volt = (voltage/3.3*65535);
    char MSDB, LSDB;
    LSDB = volt % 0x100;
    MSDB = volt / 0x100;
    voltage += 0.05;
    if(voltage >1) {
        voltage = 1;
    };
    unsigned int boosted_volt = (voltage/3.3*65535);
    char MSDB2, LSDB2;
    LSDB2 = volt % 0x100;
    MSDB2 = volt / 0x100;
    power(ADDR,0x34,MSDB2,LSDB2) ;  //update channel E 1V      WRAPPERVD
    wait(POWER_UP_TIME);
    power(ADDR,0x32,MSDB2,LSDB2) ;  //update channel C 1V      COREVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x30,MSDB,LSDB) ;  //update channel A 1V      SRAMVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x36,MSDB2,LSDB2) ;  //update channel G 1V      SENSEVDD
    wait(POWER_UP_TIME);
}


void adjustSRAMVoltage(double voltage) {
    unsigned int volt = (voltage/3.3*65535);
    char MSDB, LSDB;
    LSDB = volt % 0x100;
    MSDB = volt / 0x100;
    
    //Adjust peripheral voltage
    voltage += 0.10;
    if (voltage > 1.00) //Clamp peripheral voltage at max VDD
        voltage = 1.00;
        
    unsigned int boosted_volt = (voltage/3.3*65535);
    char MSDB2, LSDB2;
    LSDB2 = volt % 0x100;
    MSDB2 = volt / 0x100;
    power(ADDR,0x34,MSDB2,LSDB2) ;  //update channel E 1V      WRAPPERVD
    wait(POWER_UP_TIME);
    power(ADDR,0x32,MSDB2,LSDB2) ;  //update channel C 1V      COREVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x30,MSDB,LSDB) ;  //update channel A 1V      SRAMVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x36,MSDB2,LSDB2) ;  //update channel G 1V      SENSEVDD
    wait(POWER_UP_TIME);
}

void powerDown()
{
    power(ADDR,0x33,0x00,0x00) ;  //update channel D 3.3V    DVDD
    wait(POWER_UP_TIME);
    power_indicator = 0;
    power(ADDR,0x37,0x00,0x00) ;  //update channel H 1V      AVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x35,0x00,0x00) ;  //update channel F 0.5V    AVDD2
    wait(POWER_UP_TIME);
    power(ADDR,0x34,0x00,0x00) ;  //update channel E 1V      WRAPPERVD
    wait(POWER_UP_TIME);
    power(ADDR,0x32,0x00,0x00) ;  //update channel C 1V      COREVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x30,0x00,0x00) ;  //update channel A 1V      SRAMVDD
    wait(POWER_UP_TIME);
    power(ADDR,0x36,0x00,0x00) ;  //update channel G 1V      SENSEVDD
    wait(POWER_UP_TIME);
//    power(ADDR,0x31,0x00,0x00) ;  //update channel B 1.8V    DVDD2
//    wait(POWER_UP_TIME);
}

void powerReset()
{
    power_indicator = 0;
    CLR_BAR = 0;
    ADDR0 = 0;
    LDAC_BAR = 0;
    CLR_BAR = 1;
}

void debugPower()
{
    char data[3];
    data[0]=0x31;
    data[1]=0x8B;
    data[2]=0xA0;
    int A = 0;
    for (A = 0; A<=0xFF; A++) {
        if (i2c.write(A,data,3,false)==0) {
            //pc.printf ("%X\n", A);
        }
    }
}