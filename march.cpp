#include "JTAG.h"
#include "march.h"
#include "mbed.h"
#include "scan.h"

using namespace std;

extern Serial _USB_CONSOLE;
extern JTAG* _JTAG;
extern FILE* _FP;

int marchSS_M0(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
        
    //March SS element M0
    _USB_CONSOLE.printf("Element M0...\r\n");
    value = 0x00000000;
    for (address = base_addr; address <= end_addr; address += stride) {
        //w0
        _JTAG->writeMemory(address, value);
        iter++;
    }
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int marchSS_M1(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
    
    //March SS element M1
    _USB_CONSOLE.printf("Element M1...\r\n");
    for (address = base_addr; address <= end_addr; address += stride) {
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0)
            _USB_CONSOLE.printf("ERROR at March SS Element M1.1, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0)
            _USB_CONSOLE.printf("ERROR at March SS Element M1.2, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
           
        //w0
        value = 0x00000000;
        _JTAG->writeMemory(address, value);
        
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0)
            _USB_CONSOLE.printf("ERROR at March SS Element M1.3, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        //w1
        value = 0xFFFFFFFF;
        _JTAG->writeMemory(address, value);
        
        iter++;
    }
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int marchSS_M2(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
    
    //March SS element M2
    _USB_CONSOLE.printf("Element M2...\r\n");
    for (address = base_addr; address <= end_addr; address += stride) {
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M2.1, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M2.2, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w1
        value = 0xFFFFFFFF;
        _JTAG->writeMemory(address, value);
        
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M2.3, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w0
        value = 0x00000000;
        _JTAG->writeMemory(address, value);
        
        iter++;
    }
    
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int marchSS_M3(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
    
    //March SS element M3
    _USB_CONSOLE.printf("Element M3...\r\n");
    for (address = base_addr; address >= base_addr && address <= end_addr; address -= stride) {
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0x00000000)
            _USB_CONSOLE.printf("ERROR at March SS Element M3.1, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0x00000000)
            _USB_CONSOLE.printf("ERROR at March SS Element M3.2, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //w0
        value = 0x00000000;
        _JTAG->writeMemory(address, value);
        
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0x00000000)
            _USB_CONSOLE.printf("ERROR at March SS Element M3.3, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
            
        //w1
        value = 0xFFFFFFFF;
        _JTAG->writeMemory(address, value);
        
        iter++;
    }
    
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int marchSS_M4(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
    
    //March SS element M4
    _USB_CONSOLE.printf("Element M4...\r\n");
    for (address = base_addr; address >= base_addr && address <= end_addr; address -= stride) {
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M4.1, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M4.2, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w1
        value = 0xFFFFFFFF;
        _JTAG->writeMemory(address, value);
        
        //r1
        value = _JTAG->readMemory(address);
        if (value != 0xFFFFFFFF)
            _USB_CONSOLE.printf("ERROR at March SS Element M4.3, address = %X, exp value = %X, actual value = %X\r\n", address, 0xFFFFFFFF, value);
            
        //w0
        value = 0x00000000;
        _JTAG->writeMemory(address, value);
        
        iter++;
    }
    
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int marchSS_M5(unsigned int base_addr, unsigned int end_addr, unsigned int stride) {
    int iter = 0;
    unsigned int value;
    unsigned int address;
    
    //March SS element M5
    _USB_CONSOLE.printf("Element M5...\r\n");
    value = 0x00000000;
    for (address = base_addr; address <= end_addr; address += stride) {
        //r0
        value = _JTAG->readMemory(address);
        if (value != 0x00000000)
            _USB_CONSOLE.printf("ERROR at March SS Element M5.1, address = %X, exp value = %X, actual value = %X\r\n", address, 0, value);
        
        iter++;
    }
    
    _USB_CONSOLE.printf("Finished %d iterations with stride of %d bytes, total space tested: %d\r\n", iter, stride, (end_addr - base_addr)+stride);
    
    return 0;
}

int doMarchSS_SRAMBank0() {  
    marchSS_M0(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    marchSS_M1(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    marchSS_M2(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    marchSS_M3(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    marchSS_M4(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    marchSS_M5(SRAMBANK0_BASE_ADDR, SRAMBANK0_END_ADDR, ADDR_INCR);      
    
    return  0;
}

int doMarchSS_SRAMBank1() {
    marchSS_M0(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    marchSS_M1(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    marchSS_M2(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    marchSS_M3(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    marchSS_M4(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    marchSS_M5(SRAMBANK1_BASE_ADDR, SRAMBANK1_END_ADDR, ADDR_INCR);      
    
    return  0;
}

//Based on Hamdioui et. al March SS 2002
int doMarchSS() {
    int retval = 0;
    _USB_CONSOLE.printf("...March SS on SRAM Bank 0...\r\n");
    retval += doMarchSS_SRAMBank0();
    _USB_CONSOLE.printf("...March SS on SRAM Bank 1...\r\n");
    retval += doMarchSS_SRAMBank1(); 
    
    return retval;
}