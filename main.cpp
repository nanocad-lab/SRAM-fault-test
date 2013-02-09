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
#include "march.h"

using namespace std;

Serial _USB_CONSOLE(USBTX, USBRX);
JTAG* _JTAG;
FILE* _FP;


int main()
{    
    DigitalOut RESET (p21);
    DigitalOut FINISH_FLAG (LED3);
    
    double voltage = 0;
    double duty_cycle = 0.5;
    PLL clk;
    
    unsigned int value;
    unsigned int address;

    _USB_CONSOLE.printf("*** Starting SRAM_fault_map ***\r\n");
    _USB_CONSOLE.printf("*** Author: Mark Gottscho\r\n");
    _USB_CONSOLE.printf("*** Based on code by Liangzhen Lai\r\n");
    _USB_CONSOLE.printf("*** UCLA NanoCAD Lab, www.nanocad.ee.ucla.edu\r\n");
    
    voltage = 1;
    _JTAG =  new JTAG;

    // first power up chip with 1V
    _USB_CONSOLE.printf("Resetting power...\r\n");
    powerReset();
    _USB_CONSOLE.printf("Powering up test chip...\r\n");
    _USB_CONSOLE.printf("Setting SRAM array voltage: %f V\r\n", voltage);
    powerUp(voltage);
    
    _USB_CONSOLE.printf("Resetting test chip...\r\n");
    RESET = 0;
    wait(1);
    RESET = 1;

    // set the clock frequency to 20 MHz
    _USB_CONSOLE.printf("Setting test chip clock frequency to 20 MHz\r\n");
    clk.setPLL(20);

    // Init ____JTAG and halt processor
    _USB_CONSOLE.printf("Initializing JTAG...\r\n");
    _JTAG->DAP_enable();
    address = 0xE000EDF0;
    value = 0xA05F0003;
    _JTAG->writeMemory(address, value);
    value = _JTAG->readMemory(address);

    _USB_CONSOLE.printf("Halting processor...\r\n");   
    if (value&0x00000003 != 0x00000003) { //Check to make sure CPU halted
        _USB_CONSOLE.printf("ERROR: Processor FAILED TO HALT!\r\n");
        return 1;
    }
        
    //Set SRAM bank 0 offset address
    _USB_CONSOLE.printf("Setting SRAM bank 0 offset address to 0x%X\r\n", 0x0);
    address = 0x44000008;
    value = 1; //This sets the offset address to 0x000000000
    _JTAG->writeMemory(address, value);
    
    //Init file to write to
    _USB_CONSOLE.printf("Opening results.csv file...\r\n");
    _FP = fopen("/local/results.csv", "w");
    if (_FP == NULL) {
        _USB_CONSOLE.printf("ERROR: Couldn't open /local/results.csv\r\n");
        return 1;
    }
       
    _USB_CONSOLE.printf("Init success!\r\n");
    
    /* MY CODE HERE */
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    _USB_CONSOLE.printf("---------- BEGINNING MARCH TESTS @ %0.02f V ----------\r\n", voltage);
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    doMarchSS();
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    _USB_CONSOLE.printf("---------- TEST SEQUENCE COMPLETE @ %0.02f V ---------\r\n", voltage);
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    
    
    
    _USB_CONSOLE.printf("Powering down test chip...\r\n");
    powerDown();
    
    //Close file for results
    if (_FP)
        fclose(_FP);
    _USB_CONSOLE.printf("Done!\r\n");
    while(1) {
        FINISH_FLAG = !FINISH_FLAG;
        wait(1);
    }
    return 0;
}
