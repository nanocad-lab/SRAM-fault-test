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

int doTest(double voltage) {
    _USB_CONSOLE.printf("---------> Setting SRAM voltage: %0.02f V <--------\r\n", voltage);
    adjustSRAMVoltage(voltage);
    
    fprintf(_FP, "Voltage %f,Address,Failure,", voltage);
       
    /* MY CODE HERE */
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    _USB_CONSOLE.printf("---------- BEGINNING MARCH TESTS @ %0.02f V ----------\r\n", voltage);
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    doMarchSS();
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    _USB_CONSOLE.printf("---------- TEST SEQUENCE COMPLETE @ %0.02f V ---------\r\n", voltage);
    _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
    
    fprintf(_FP, "\n");
    
    return 0;
}


int main()
{    
    DigitalOut FINISH_FLAG (LED3);
    DigitalOut RESET (p21); 
    
    double duty_cycle = 0.5;
    double voltage = 1;
    PLL clk;
    
    unsigned int value;
    unsigned int address;
    int iter = 0;
    int test_return = 0;
    
          
    // power up chip with voltage
    _USB_CONSOLE.printf("** Resetting power...\r\n");
    powerReset();
    _USB_CONSOLE.printf("** Powering up test chip...\r\n");
    _USB_CONSOLE.printf("** Setting global voltages: %f V\r\n", voltage);
    powerUp(voltage);
    
    _USB_CONSOLE.printf("** Resetting test chip...\r\n");
    RESET = 0;
    wait(1);
    RESET = 1;

    // set the clock frequency to 20 MHz
    _USB_CONSOLE.printf("** Setting test chip clock frequency to 20 MHz\r\n");
    clk.setPLL(20);

    // Init _JTAG and halt processor
    _USB_CONSOLE.printf("** Initializing JTAG...\r\n");
    _JTAG = new JTAG;
    _JTAG->DAP_enable();
    address = 0xE000EDF0;
    value = 0xA05F0003;
    _JTAG->writeMemory(address, value);
    value = _JTAG->readMemory(address);

    _USB_CONSOLE.printf("** Halting processor...\r\n");   
    if (value&0x00000003 != 0x00000003) { //Check to make sure CPU halted
        _USB_CONSOLE.printf("** !!! ERROR: Processor FAILED TO HALT!\r\n");
        return 1;
    }
        
    //Set SRAM bank 0 offset address
    _USB_CONSOLE.printf("** Setting SRAM bank 0 offset address to 0x%X\r\n", 0x0);
    address = 0x44000008;
    value = 1; //This sets the offset address to 0x000000000
    _JTAG->writeMemory(address, value);
         
    _USB_CONSOLE.printf("** Init success!\r\n");
    
    
    ////////////////////////////////
    _USB_CONSOLE.printf("***************************************************************\r\n");
    _USB_CONSOLE.printf("****************** Starting SRAM_fault_map ********************\r\n");
    _USB_CONSOLE.printf("******************** Author: Mark Gottscho*********************\r\n");
    _USB_CONSOLE.printf("*************** Based on code by Liangzhen Lai*****************\r\n");
    _USB_CONSOLE.printf("************* UCLA NanoCAD Lab, www.nanocad.ee.ucla.edu********\r\n");
    _USB_CONSOLE.printf("***************************************************************\r\n\r\n");
    
    for (double voltage = 1.00; voltage >= 0.20; voltage -= 0.10) {
        //Init file to write to
        _USB_CONSOLE.printf("** Opening results file...\r\n");
        if (iter == 0)
            _FP = fopen("/local/results0.csv", "w");
        else if (iter == 1)
            _FP = fopen("/local/results1.csv", "w");
        else if (iter == 2)
            _FP = fopen("/local/results2.csv", "w");
        else if (iter == 3)
            _FP = fopen("/local/results3.csv", "w");
        else if (iter == 4)
            _FP = fopen("/local/results4.csv", "w");
        else if (iter == 5)
            _FP = fopen("/local/results5.csv", "w");
        else if (iter == 6)
            _FP = fopen("/local/results6.csv", "w");
        else if (iter == 7)
            _FP = fopen("/local/results7.csv", "w");
        else if (iter == 8)
            _FP = fopen("/local/results8.csv", "w");
        else if (iter == 9)
            _FP = fopen("/local/results9.csv", "w");
        else if (iter == 10)
            _FP = fopen("/local/results10.csv", "w");
        else
            _FP = fopen("/local/results_UNK.csv", "w");
            
        if (_FP == NULL) {
            _USB_CONSOLE.printf("** !!! ERROR: Couldn't open results file, iter %d\r\n", iter);
            return 1;
        }
        
        test_return = doTest(voltage);
        //Close file for results
        if (_FP)
            fclose(_FP);
            
        if (test_return == 1) {
            _USB_CONSOLE.printf("!!! ERROR: Test failed to complete at SRAM voltage of %0.02f V, terminating.\r\n");
            return 1;
        }
        
        iter++;
    }
        
    _USB_CONSOLE.printf("** Powering down test chip...\r\n");
    powerDown();
    
    _USB_CONSOLE.printf("** Done!\r\n");
    while(1) {
        FINISH_FLAG = !FINISH_FLAG;
        wait(1);
    }
    return 0;
}
