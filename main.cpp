
#include "mbed.h"
#include "string"
#include "dac.h"
#include "board_test.h"
#include "scan.h"
#include "power.h"
#include "pinout.h"
#include "pll.h"
#include "lcd.h"
#include "jtag.h"
#include "mmap.h"
#include "clock.h"
#include "EasyBMP.h"
#include "main.h"
#include "march.h"


//port from dpcs
Serial _USB_CONSOLE(USBTX, USBRX);
JTAG* _JTAG;
FILE* _FP; //PORT THIS FUNCTIONALITY FROM DPCS


DigitalOut myled(LED4);

//#define TEST_DMEM                             // uncomment this to test Data Memory, currently post-processing and march test functions do not support data memory

#ifdef WRITE_RESULTS
//LocalFileSystem local("local");               // Create the local filesystem under the name "local"
#endif




int doDroopTest(double nominalVoltage, double droopVoltage) {
    fprintf(_FP, "Droop Voltage %f,,Word Number,Fault Code\n", droopVoltage);
      
    _USB_CONSOLE.printf("-----------------------------------------------------------------\r\n");
    _USB_CONSOLE.printf("------------- BEGINNING MARCH TESTS @ %0.02f V (droop) ----------\r\n", droopVoltage);
    _USB_CONSOLE.printf("-----------------------------------------------------------------\r\n");
    doMarchSS_droopVoltage(nominalVoltage, droopVoltage);
    _USB_CONSOLE.printf("----------------------------------------------------------------\r\n");
    _USB_CONSOLE.printf("---------- TEST SEQUENCE COMPLETE @ %0.02f V (droop) -----------\r\n", droopVoltage);
    _USB_CONSOLE.printf("----------------------------------------------------------------\r\n");
    
    fprintf(_FP, "\n");
    
    return 0;
}

int voltageDroopMarchTestRoutine(double nominalVoltage, double droopVoltageMin, double droopVoltageMax, double voltageStep) {   
    int iter = 0;
    int test_return = 0;
    
    for (double voltage = droopVoltageMax; voltage >= droopVoltageMin; voltage -= voltageStep) {
        //Init file to write to
        _USB_CONSOLE.printf("** Opening data file...\r\n");
        
        if (iter == 0)
            _FP = fopen("/local/drpmt0.csv", "w");
        else if (iter == 1)
            _FP = fopen("/local/drpmt1.csv", "w");
        else if (iter == 2)
            _FP = fopen("/local/drpmt2.csv", "w");
        else if (iter == 3)
            _FP = fopen("/local/drpmt3.csv", "w");
        else if (iter == 4)
            _FP = fopen("/local/drpmt4.csv", "w");
        else if (iter == 5)
            _FP = fopen("/local/drpmt5.csv", "w");
        else if (iter == 6)
            _FP = fopen("/local/drpmt6.csv", "w");
        else if (iter == 7)
            _FP = fopen("/local/drpmt7.csv", "w");
        else if (iter == 8)
            _FP = fopen("/local/drpmt8.csv", "w");
        else if (iter == 9)
            _FP = fopen("/local/drpmt9.csv", "w");
        else if (iter == 10)
            _FP = fopen("/local/drpmt10.csv", "w");
        else if (iter == 11)
            _FP = fopen("/local/drpmt11.csv", "w");
        else if (iter == 12)
            _FP = fopen("/local/drpmt12.csv", "w");
        else if (iter == 13)
            _FP = fopen("/local/drpmt13.csv", "w");
        else if (iter == 14)
            _FP = fopen("/local/drpmt14.csv", "w");
        else if (iter == 15)
            _FP = fopen("/local/drpmt15.csv", "w");
        else if (iter == 16)
            _FP = fopen("/local/drpmt16.csv", "w");
        else if (iter == 17)
            _FP = fopen("/local/drpmt17.csv", "w");
        else if (iter == 18)
            _FP = fopen("/local/drpmt18.csv", "w");
        else if (iter == 19)
            _FP = fopen("/local/drpmt19.csv", "w");
        else if (iter == 20)
            _FP = fopen("/local/drpmt20.csv", "w");
        else
            _FP = fopen("/local/drpmtUNK.csv", "w");
            
        if (_FP == NULL) {
            _USB_CONSOLE.printf("** !!! ERROR: Couldn't open data file, iter %d\r\n", iter);
            return 1;
        }
        
        test_return = doDroopTest(nominalVoltage, voltage);
  
        //Close file for data
        if (_FP)
            fclose(_FP);
            
        if (test_return == 1) {
            _USB_CONSOLE.printf("!!! ERROR: Test failed to complete at SRAM droop voltage of %0.02f V, terminating.\r\n", voltage);
            return 1;
        }
        
        iter++;
    }
   
    return 0;
}

int standardMarchTests(double minVoltage, double maxVoltage, double voltageStep, unsigned int lowAddr, unsigned int highAddr, int bankNum) {
    int iter = 0;
    int test_return = 0;
    
    for (double voltage = maxVoltage; voltage >= minVoltage; voltage -= voltageStep) {
        //Init file to write to
        _USB_CONSOLE.printf("** Opening data file...\r\n");
        char to_c_str[10];
        sprintf(to_c_str, "%d", iter);
        string iterStr(to_c_str);
        string fname("/local/data" + iterStr + ".csv");
        _FP = fopen(fname.c_str(), "w");
//        if (iter == 0)
//            _FP = fopen("/local/data0.csv", "w");
//        else if (iter == 1)
//            _FP = fopen("/local/data1.csv", "w");
//        else if (iter == 2)
//            _FP = fopen("/local/data2.csv", "w");
//        else if (iter == 3)
//            _FP = fopen("/local/data3.csv", "w");
//        else if (iter == 4)
//            _FP = fopen("/local/data4.csv", "w");
//        else if (iter == 5)
//            _FP = fopen("/local/data5.csv", "w");
//        else if (iter == 6)
//            _FP = fopen("/local/data6.csv", "w");
//        else if (iter == 7)
//            _FP = fopen("/local/data7.csv", "w");
//        else if (iter == 8)
//            _FP = fopen("/local/data8.csv", "w");
//        else if (iter == 9)
//            _FP = fopen("/local/data9.csv", "w");
//        else if (iter == 10)
//            _FP = fopen("/local/data10.csv", "w");
//        else if (iter == 11)
//            _FP = fopen("/local/data11.csv", "w");
//        else if (iter == 12)
//            _FP = fopen("/local/data12.csv", "w");
//        else if (iter == 13)
//            _FP = fopen("/local/data13.csv", "w");
//        else if (iter == 14)
//            _FP = fopen("/local/data14.csv", "w");
//        else if (iter == 15)
//            _FP = fopen("/local/data15.csv", "w");
//        else if (iter == 16)
//            _FP = fopen("/local/data16.csv", "w");
//        else if (iter == 17)
//            _FP = fopen("/local/data17.csv", "w");
//        else if (iter == 18)
//            _FP = fopen("/local/data18.csv", "w");
//        else if (iter == 19)
//            _FP = fopen("/local/data19.csv", "w");
//        else if (iter == 20)
//            _FP = fopen("/local/data20.csv", "w");
//        else
//            _FP = fopen("/local/dataUNK.csv", "w");
            
        if (_FP == NULL) {
            _USB_CONSOLE.printf("** !!! ERROR: Couldn't open data file, iter %d\r\n", iter);
            return 1;
        }
        
        _USB_CONSOLE.printf("---------> Setting SRAM voltage: %0.03f V <--------\r\n", voltage);
        adjustSRAMVoltage(voltage);
        _USB_CONSOLE.printf("---------> Low addr:\t0x%08X <--------\r\n", lowAddr);
        _USB_CONSOLE.printf("---------> High addr:\t0x%08X <--------\r\n", highAddr);
        _USB_CONSOLE.printf("---------> Bank num:\t%d <--------\r\n", bankNum);
    
        
        fprintf(_FP, "Voltage %f,,Effective Byte Address,Fault Code\n", voltage);
           
        _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
        _USB_CONSOLE.printf("---------- BEGINNING MARCH TESTS @ %0.03f V ---------\r\n", voltage);
        _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
        test_return = doMarchSS(lowAddr, highAddr, bankNum);
        _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
        _USB_CONSOLE.printf("---------- COMPLETED MARCH TESTS @ %0.03f V ---------\r\n", voltage);
        _USB_CONSOLE.printf("-----------------------------------------------------\r\n");
        
        fprintf(_FP, "\n");
        
        
        //Close file for data
        if (_FP)
            fclose(_FP);
            
        if (test_return == 1) {
            _USB_CONSOLE.printf("!!! ERROR: Test failed to complete at SRAM voltage of %0.02f V, terminating.\r\n", voltage);
            return 1;
        }
        
        iter++;
    }

    return 0;
}

int main()
{
    DigitalOut FINISH_FLAG (LED3);
    _USB_CONSOLE.baud(9600);
    
    //begin variables port from dpcs
    //unsigned int value;
    //unsigned int address;
    
    int retval = 0;
    
    double vmin = 1;
    double vmax = 1;
    double vstep = 0;
    unsigned int lowAddr = SRAMBANK0_BASE_ADDR;
    unsigned int highAddr = SRAMBANK0_END_ADDR;
    int bankNum = 0;
    //end variables port from dpcs

    // power up the board
    _USB_CONSOLE.printf("** Resetting power...\r\n");
    power_down();
    _USB_CONSOLE.printf("** Powering up test chip...\r\n");
    float core_volt = 1;
    _USB_CONSOLE.printf("** Setting global voltages: %f V\r\n", core_volt);
    power_up(core_volt); // Power Up Chip
    pc.printf("Powered up!\r\n");

    _USB_CONSOLE.printf("** Resetting test chip...\r\n");
    // initializing the resets
    PORESETn = 0;
    CORERESETn = 0;
    wait_us(100);
    PORESETn = 1;
    CORERESETn = 1;

    //maybe set PLL to 20 MHz here?

    _JTAG = new JTAG;
    // read and verify chip ID
    pc.printf("** Verify Chip ID...\r\n");
    int idcode = _JTAG->readID();
    
    if(idcode != 0x4ba00477) {
        pc.printf("ERROR: IDCode %X\r\n", idcode);
        wait(2);
        power_down();
        return -1;
    }
    pc.printf("IDCode %X\r\n", idcode);

    // power up the JTAG port
    _JTAG->reset();
    _JTAG->leaveState();
    _JTAG->PowerupDAP();

    // setup necessary internal clock source selection
    _JTAG->writeMemory(intclk_source, 2);
    _JTAG->writeMemory(extclk_source, 1);
    _JTAG->writeMemory(ext_div_by, 10);

    float voltage=1;
    power_core(voltage); //adjust core voltage to 1V
    pc.printf("V: %f\r\n", voltage);

    // Halt Core and set Imem to begin at 0
    if (_JTAG->haltCore() != 0) {
        pc.printf("ERROR: Could not halt core \r\n");
        wait(2);
        power_down();
        return -1;
    }
    if (_JTAG->zeroImemOffset() != 0) {
        pc.printf("ERROR: Could not set Instruction Memory to 0x00 offset \r\n");
        wait(2);
        power_down();
        return -1;
    }
    
    
    _USB_CONSOLE.printf("** Init success!\r\n");
    
    
    // Begin march testing
    ////////////////////////////////    
    _USB_CONSOLE.printf("****************************************************************\r\n");
    _USB_CONSOLE.printf("****************** Starting SRAM_fault_map *********************\r\n");
    _USB_CONSOLE.printf("******************** Author: Mark Gottscho *********************\r\n");
    _USB_CONSOLE.printf("*************** Based on code by Liangzhen Lai *****************\r\n");
    _USB_CONSOLE.printf("************ Ported to Orange Ferrari by Albert Liu ************\r\n");
    _USB_CONSOLE.printf("********** UCLA NanoCAD Lab, www.nanocad.ee.ucla.edu ***********\r\n");
    _USB_CONSOLE.printf("****************************************************************\r\n\r\n");
    
    // IMem March Test Parameters
    vmin = 0.449;
    vmax = 0.700;
    vstep = 0.025;
    
#ifdef TEST_DMEM
    lowAddr = DMEM_MIN;
    highAddr = DMEM_MAX - 0x04;
    bankNum = 1;
#else //TEST_IMEM
    lowAddr = IMEM_MIN;
    highAddr = IMEM_MAX - 0x04;
    bankNum = 0;
#endif
    
    retval = standardMarchTests(vmin, vmax, vstep, lowAddr, highAddr, bankNum);
    
    // If another round of March Tests is inserted for DMem, then dpcs-sram-fault-test-post-process will need to be updated to read the data files as well
 
    if (retval != 0)
        _USB_CONSOLE.printf("** Testing failed!\r\n");
        
    _USB_CONSOLE.printf("** Powering down test chip...\r\n");
    power_down();
    
    _USB_CONSOLE.printf("** Done!\r\n");
    
    while(1) {
        FINISH_FLAG != FINISH_FLAG;
        wait(1);
    }
    
    delete _JTAG;
    return 0;
}