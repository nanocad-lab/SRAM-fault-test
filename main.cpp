/* Author: Mark Gottscho
 * UCLA NanoCAD Lab
 * Based on code by Liangzhen Lai
 * For the mbed microcontroller, NSF Variability Expedition Red Mini Cooper board
 * SRAM Fault Map program
 */ 

#include "mbed.h"
#include "power_up.h"
#include "scan.h"
#include "JTAG.h"

using namespace std;

Serial s(USBTX, USBRX);

DigitalOut RESET (p21);
DigitalOut finish_flag (LED3);

double voltage;
double duty_cycle;
JTAG* jtag;

#define SRAMBANK0_BASE_ADDR 0x00000000
#define SRAMBANK1_BASE_ADDR 0x60000000
#define SRAMBANK0_END_ADDR 0x00001FFC
#define SRAMBANK1_END_ADDR 0x60001FFC
#define ADDR_INCR 0x00000004

int doMarchSS_SRAMBank0() {
    unsigned int address;
    unsigned int value;
    int iter = 0;
        
    //March SS element M0
    s.printf("Element M0...\r\n");
    value = 0x00000000;
    for (address = SRAMBANK0_BASE_ADDR; address <= SRAMBANK0_END_ADDR; address += ADDR_INCR) {
        //w0
        jtag->writeMemory(address, value);
        iter++;
    }
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    //March SS element M1
    s.printf("Element M1...\r\n");
    for (address = SRAMBANK0_BASE_ADDR; address <= SRAMBANK0_END_ADDR; address += ADDR_INCR) {
        //r0
        value = jtag->readMemory(address);
        if (value != 0)
            s.printf("ERROR at March SS Bank 0, Element M1.1, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        //r0
        value = jtag->readMemory(address);
        if (value != 0)
            s.printf("ERROR at March SS Bank 0, Element M1.2, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
           
        //w0
        value = 0x00000000;
        jtag->writeMemory(address, value);
        
        //r0
        value = jtag->readMemory(address);
        if (value != 0)
            s.printf("ERROR at March SS Bank 0, Element M1.3, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        //w1
        value = 0xFFFFFFFF;
        jtag->writeMemory(address, value);
        
        iter++;
    }
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    //March SS element M2
    s.printf("Element M2...\r\n");
    for (address = SRAMBANK0_BASE_ADDR; address <= SRAMBANK0_END_ADDR; address += ADDR_INCR) {
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M2.1, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M2.2, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w1
        value = 0xFFFFFFFF;
        jtag->writeMemory(address, value);
        
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M2.3, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w0
        value = 0x00000000;
        jtag->writeMemory(address, value);
        
        iter++;
    }
    
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    //March SS element M3
    s.printf("Element M3...\r\n");
    for (address = SRAMBANK0_END_ADDR; address >= SRAMBANK0_BASE_ADDR && address <= SRAMBANK0_END_ADDR; address -= ADDR_INCR) {
        //r0
        value = jtag->readMemory(address);
        if (value != 0x00000000)
            s.printf("ERROR at March SS Bank 0, Element M3.1, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //r0
        value = jtag->readMemory(address);
        if (value != 0x00000000)
            s.printf("ERROR at March SS Bank 0, Element M3.2, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //w0
        value = 0x00000000;
        jtag->writeMemory(address, value);
        
        //r0
        value = jtag->readMemory(address);
        if (value != 0x00000000)
            s.printf("ERROR at March SS Bank 0, Element M3.3, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //w1
        value = 0xFFFFFFFF;
        jtag->writeMemory(address, value);
        
        iter++;
    }
    
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    //March SS element M4
    s.printf("Element M4...\r\n");
    for (address = SRAMBANK0_END_ADDR; address >= SRAMBANK0_BASE_ADDR && address <= SRAMBANK0_END_ADDR; address -= ADDR_INCR) {
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M4.1, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M4.2, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w1
        value = 0xFFFFFFFF;
        jtag->writeMemory(address, value);
        
        //r1
        value = jtag->readMemory(address);
        if (value != 0xFFFFFFFF)
            s.printf("ERROR at March SS Bank 0, Element M4.3, addr = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w0
        value = 0x00000000;
        jtag->writeMemory(address, value);
        
        iter++;
    }
    
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    //March SS element M5
    s.printf("Element M5...\r\n");
    value = 0x00000000;
    for (address = SRAMBANK0_BASE_ADDR; address <= SRAMBANK0_END_ADDR; address += ADDR_INCR) {
        //r0
        value = jtag->readMemory(address);
        if (value != 0x00000000)
            s.printf("ERROR at March SS Bank 0, Element M5.1, addr = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        iter++;
    }
    
    s.printf("Iter: %d\r\n", iter);
    iter = 0;
    
    return  0;
}

int doMarchSS_SRAMBank1() {
    return  0;
}

//Based on Hamdioui et. al March SS 2002
int doMarchSS() {
    int retval = 0;
    s.printf("...March SS on SRAM Bank 0...\r\n");
    retval += doMarchSS_SRAMBank0();
    s.printf("...March SS on SRAM Bank 1...\r\n");
    retval += doMarchSS_SRAMBank1(); 
    
    return retval;
}

int main()
{
    unsigned int address;
    unsigned int value;
    FILE *fp;
    
    s.printf("*** SRAM_fault_map ***\r\n");
    voltage = 1;
    PLL clk;
    jtag =  new JTAG;

    // first power up chip with 1V
    s.printf("Resetting power...\r\n");
    powerReset();
    s.printf("Powering up...\r\n");
    s.printf("Starting with %f V\r\n", voltage);
    powerUp(voltage);
    
    s.printf("Resetting test chip...\r\n");
    RESET = 0;
    wait(1);
    RESET = 1;

    // set the clock frequency to 20 MHz
    s.printf("Setting clock frequency to 20 MHz\r\n");
    clk.setPLL(20);

    // Init JTAG and halt processor
    s.printf("Initializing JTAG...\r\n");
    jtag->DAP_enable();
    address = 0xE000EDF0;
    value = 0xA05F0003;
    jtag->writeMemory(address, value);
    value = jtag->readMemory(address);

    s.printf("Halting processor...\r\n");   
    if (value&0x00000003 != 0x00000003) { //Check to make sure CPU halted
        s.printf("ERROR: Processor FAILED TO HALT!\r\n");
        return 1;
    }
        
    //Set SRAM bank 0 offset address
    s.printf("Setting SRAM bank 0 offset address to 0x%X\r\n", 0x0);
    address = 0x44000008;
    value = 1; //This sets the offset address to 0x000000000
    jtag->writeMemory(address, value);
    
    //Init file to write to
    s.printf("Opening results.csv file...\r\n");
    fp = fopen("/local/results.csv", "w");
    if (fp == NULL) {
        s.printf("ERROR: Couldn't open /local/results.csv\r\n");
        return 1;
    }
       
    /* MY CODE HERE */
    s.printf("---------- INIT SUCCESS: BEGINNING MARCH TESTS ----------\r\n");
    
    s.printf("Starting test sequence March-SS...\r\n");
    doMarchSS();
    s.printf("Test sequence complete!\r\n");
    
    
    s.printf("Powering down...\r\n");
    powerDown();
    
    //Close file for results
    if (fp)
        fclose(fp);
    s.printf("Done!\r\n");
    while(1) {
        finish_flag = !finish_flag;
        wait(1);
    }
    return 0;
}
