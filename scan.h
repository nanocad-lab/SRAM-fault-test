#ifndef SCAN_H
#define SCAN_H

#include "mbed.h"

class PLL{
    public:
        PLL();
        void setFBK_MODE(int MODE);//feedback mode, 0 for external, 1 for internal
        int setPLL(int frequency);// main PLL setting function
        int testPLL();// use to test PLL working
        int check_lock();// check if PLL is locked

        unsigned int* A;//scan chain bit vector
        int FB_MODE;//feedback mode, 0 for external, 1 for internal
        void initScan();// initialize the scan chain
        int scanIn();// write A to scan chain
        int scanOut();// read scan chain value to A
        void loadChip();// load the scan in value to chip
        void loadChain();// load the scan chain value from chip
        void scanAction();// step in the value by 1 bit
        void setVector();// set bit-vector A for test
        void plloutAon();// set PLLout A on and B off
        void plloutBon();// set PLLout A off and B on
        void setPrediv(int val);//Setting the Prediv bits A[143:138] Range 1-32
        void setFbDivPreScaler(int val);//Setting the Feedback Divider Pre-scalar bits A[136:134] Range: 1-4
        void setFbDiv(int val);//Setting the Feedback Divider bits A[133:126] Range: 1-255
        void setFwDivPreScalerA(int val);//Setting the Forward Divider Pre-scalar bits for A A[150:149] Range: 1,2,4
        void setFwDivA(int val);//Setting the Forward Divider Pre-scalar bits A[148:144] Range: 1-32
        void setFwDivPreScalerB(int val);//Setting the Forward Divider Pre-scalar bits for B A[150:149] Range: 1,2,4
        void setFwDivB(int val);//Setting the Forward Divider Pre-scalar bits for B A[148:144] Range: 1-32
        void setLFTUNE(int interval);//Setting LFTUNE A[83:43] Range: 0(2.5-3.2) - 1(3.2-5.0)
        void setFFTUNE(int interval);//Setting FFTUNE A[41:26]
        void setINTFBK();//Setting feedback mode A[42]
        void setROandOther();//Setting(disabling) ring_osc A[7:0]
        void setINTMUX(int clk);//Setting internal clk source bits A[9:8] Range: 0: HCLK_EXT 1: ring_osc 2: PLLOUT_A 3: PLLOUT_B
        void setEXTMUX(int clk);//Setting external clk source bits A[15:14] Range: 0: HCLK_EXT 1: HCLK_divider 2: PLLOUT_A 3: PLLOUT_B
        void setEXTDIV(int val);//Setting clk divider bits for B A[13:10] Range: 0-15, divided by 2^val
        void resetPLL(int val);//Setting PLL_RESET A[158], range: 0:unreset 1:reset
};

#endif