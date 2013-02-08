#include "scan.h"
using namespace std;

#define SCAN_TIME 10
#define SCAN_DEPTH 226

DigitalOut lock_indicator(LED2);
Serial pc(USBTX, USBRX);//tx, rx => for debugging purposes
DigitalOut phi (p16);
DigitalOut phi_bar (p17);
DigitalOut load_chain (p15);
DigitalOut load_chip (p20);
DigitalOut data_in (p19);
DigitalIn data_out(p18);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// constructor
PLL::PLL()
{
    A = new unsigned int [SCAN_DEPTH];
    for (int i=0; i<SCAN_DEPTH; i++) {
        A[i] = 0;
    }
}

/*******************************
By default, this function setup the PLL as internal feedback
(i.e. no phase synchronization guarantee between internal clock
to refclk)
The equations are:
DCO frequency = (REFCLK/Predivider) x Feedback Divider x Feedback Divider Pre-Scaler
PLLOUTA frequency = DCO frequency / (Forward Divider Pre-Scaler x Forward Divider A)
PLLOUTB frequency = DCO frequency / (Forward Divider Pre-Scaler x Forward Divider B)
Bit vectors to set:
RO
internal_clk_source
ext_div_by
ext_clk_source
DCO_BYPASS
PLL_FEEDBACK_MODE
PLL_TUNE
PLL_A_OR_B

Values to set:
        void plloutAon();// set PLLout A on and B off
        void plloutBon();// set PLLout A off and B on
        void setPrediv(int val);//Setting the Prediv bits A[143:138] Range 1-32
        void setFbDivPreScaler(int val);//Setting the Feedback Divider Pre-scalar bits A[143:138] Range: 1-4
        void setFbDiv(int val);//Setting the Feedback Divider bits A[133:126] Range: 1-32
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

*******************************/

int PLL::setPLL(int frequency)
{

    //input should be multiple of 5MHz, range should be from 100 - 200

    if (frequency < 50) {
        //infeasible frequency
        //default 20MHz
        setINTMUX(0);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency <63) {
        frequency = frequency * 2;
        //DCO=20/4*(freq*2)*3, PLLOUT=DCO/12 for freq=[50-62]
        //DCO range: 3G to 3.75G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(4);
        setFbDivPreScaler(3);
        setFbDiv(frequency*2);
        setFwDivPreScalerA(1);
        setFwDivA(12);
        setFwDivPreScalerB(1);
        setFwDivB(12);
        if (frequency<107) {  //3G - 3.2G
            setLFTUNE(0);
            setFFTUNE(0);
        } else { //3.2G - 3.75G
            setLFTUNE(1);
            setFFTUNE(1);
        }
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency <81) {
        frequency = frequency * 2;
        //DCO=20*freq, PLLOUT=DCO/8 for freq=[63-80]
        //DCO range: 2.5G - 3.2G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(1);
        setFbDivPreScaler(1);
        setFbDiv(frequency);
        setFwDivPreScalerA(1);
        setFwDivA(8);
        setFwDivPreScalerB(1);
        setFwDivB(8);
        setLFTUNE(0);
        setFFTUNE(0);
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency <=100) {
        frequency = frequency * 2;
        //DCO=20*freq, PLLOUT=DCO/8 for freq=[81-100]
        //DCO range: 3.2G - 4G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(1);
        setFbDivPreScaler(1);
        setFbDiv(frequency);
        setFwDivPreScalerA(1);
        setFwDivA(8);
        setFwDivPreScalerB(1);
        setFwDivB(8);
        setLFTUNE(1);
        setFFTUNE(1);
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency < 125) {
        //DCO=20/4*(freq*2)*3, PLLOUT=DCO/6 for freq=[100-124]
        //DCO range: 3G to 3.75G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(4);
        setFbDivPreScaler(3);
        setFbDiv(frequency*2);
        setFwDivPreScalerA(1);
        setFwDivA(6);
        setFwDivPreScalerB(1);
        setFwDivB(6);
        if (frequency<107) {  //3G - 3.2G
            setLFTUNE(0);
            setFFTUNE(0);
        } else { //3.2G - 3.75G
            setLFTUNE(1);
            setFFTUNE(1);
        }
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency < 161) {
        //DCO=20*freq, PLLOUT=DCO/4 for freq=[125-160]
        //DCO range: 2.5G - 3.2G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(1);
        setFbDivPreScaler(1);
        setFbDiv(frequency);
        setFwDivPreScalerA(1);
        setFwDivA(4);
        setFwDivPreScalerB(1);
        setFwDivB(4);
        setLFTUNE(0);
        setFFTUNE(0);
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else if (frequency <= 200) {
        //DCO=20*freq, PLLOUT=DCO/4 for freq=[160-200]
        //DCO range: 3.2G - 4G
        setFBK_MODE(1);//set internal mode
        plloutAon();
        setPrediv(1);
        setFbDivPreScaler(1);
        setFbDiv(frequency);
        setFwDivPreScalerA(1);
        setFwDivA(4);
        setFwDivPreScalerB(1);
        setFwDivB(4);
        setLFTUNE(1);
        setFFTUNE(1);
        setINTFBK();
        setROandOther();
        setINTMUX(2);//PLLOUTA
        setEXTMUX(1);//HCLK_DIVIDER
        setEXTDIV(10);
        resetPLL(1);
        initScan();
        scanIn();
        resetPLL(0);
        scanIn();
    } else {
        //infeasible frequency
    }
    wait(0.1);
    return check_lock();
}

/********************************************
PLL test function
********************************************/

int PLL::testPLL()
{
    return 0;
}

// lock indicator
int PLL::check_lock()
{
    scanOut();
    lock_indicator = A[219];
    return lock_indicator;
}

// feedback mode setting
void PLL::setFBK_MODE(int MODE)
{
    FB_MODE = MODE;
}


/**************************************************
***************************************************
Private Functions:
***************************************************
**************************************************/


/**************************************************
Scan chain operation
**************************************************/
// scan A into chip
int PLL::scanIn()
{
    for (int i=0; i<SCAN_DEPTH; i++) {
        data_in = A[i];
        scanAction();
    }
    loadChip();
}

// scan outto A
int PLL::scanOut()
{
    loadChain();
    for (int i=0; i<SCAN_DEPTH; i++) {
        A[i] = 0;
        A[i] = data_out;
        scanAction();
    }

}

// initialize the scan chain
void PLL::initScan()
{
    phi = 0;
    phi_bar = 0;
    load_chain = 0;
    load_chip = 0;
    data_in = 0;
    data_out.mode(PullNone);
    lock_indicator = 0;
}

// load scan chain values to chip
void PLL::loadChip()
{
    wait_us(SCAN_TIME);
    load_chip = 0;
    wait_us(SCAN_TIME);
    load_chip = 1;
    wait_us(SCAN_TIME);
    load_chip = 0;
    wait_us(SCAN_TIME);
}

// load chip values to scan chain
void PLL::loadChain()
{
    wait_us(SCAN_TIME);
    load_chain = 0;
    wait_us(SCAN_TIME);
    load_chain = 1;
    wait_us(SCAN_TIME);
    phi = 0;
    phi_bar = 0;
    wait_us(SCAN_TIME);
    phi = 1;
    wait_us(SCAN_TIME);
    phi = 0;
    wait_us(SCAN_TIME);
    phi_bar = 1;
    wait_us(SCAN_TIME);
    phi_bar = 0;
    wait_us(SCAN_TIME);
    load_chain = 0;
    wait_us(SCAN_TIME);
}

// scan chain step in
void PLL::scanAction ()
{
    wait_us(SCAN_TIME);
    phi = 0;
    phi_bar = 0;
    wait_us(SCAN_TIME);
    phi = 1;
    wait_us(SCAN_TIME);
    phi = 0;
    wait_us(SCAN_TIME);
    phi_bar = 1;
    wait_us(SCAN_TIME);
    phi_bar = 0;
    wait_us(SCAN_TIME);
}


/**************************************************
Scan vector setting functions
**************************************************/
// set Vector for PLL testing
void PLL::setVector()
{
    for (int i=0; i<SCAN_DEPTH; i++) {
        A[i] = 0;
    }
    A[1] = 1;
    A[2] = 1;
    A[4] = 1;
    A[9] = 1;
    A[10] = 1;
    A[13] = 1;
    A[14] = 1;
    A[34] = 1;
    A[42] = 1;
    A[63] = 1;
    A[73] = 1;
    A[79] = 1;
    A[81] = 1;
    A[126] = 1;
    A[128] = 1;
    A[129] = 1;
    A[130] = 1;
    A[131] = 1;
    A[132] = 1;
    A[145] = 1;
    A[146] = 1;
    A[147] = 1;
    A[152] = 1;
    A[153] = 1;
    A[154] = 1;
    A[168] = 1;
    A[169] = 1;
}

/**************************************************
Setting PLL_STOPCLKA bit A[162]
Setting PLL_STOPCLKB bit A[163]
**************************************************/
void PLL::plloutAon()
{
    A[162] = 0;
    A[163] = 1;
}

/**************************************************
Setting PLL_STOPCLKA bit A[162]
Setting PLL_STOPCLKB bit A[163]
**************************************************/
void PLL::plloutBon()
{
    A[162] = 1;
    A[163] = 0;
}


/**************************************************
Setting the Prediv bits A[143:138]
Range: 1-32
**************************************************/
void PLL::setPrediv(int val)
{
    //scan_load[143:138] = PLL_PREDIV;
    A[143]=0; //PREDIV5 unused, connect to ground
    switch(val) {
        case 1:  {
            A[142]=0;
            A[141]=0;
            A[140]=0;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 2:  {
            A[142]=1;
            A[141]=1;
            A[140]=1;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 3:  {
            A[142]=1;
            A[141]=1;
            A[140]=1;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 4:  {
            A[142]=1;
            A[141]=1;
            A[140]=1;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 5:  {
            A[142]=1;
            A[141]=1;
            A[140]=0;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 6:  {
            A[142]=1;
            A[141]=0;
            A[140]=0;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 7:  {
            A[142]=0;
            A[141]=0;
            A[140]=1;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 8:  {
            A[142]=0;
            A[141]=1;
            A[140]=1;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 9:  {
            A[142]=1;
            A[141]=1;
            A[140]=0;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 10: {
            A[142]=1;
            A[141]=0;
            A[140]=1;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 11: {
            A[142]=0;
            A[141]=1;
            A[140]=0;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 12: {
            A[142]=1;
            A[141]=0;
            A[140]=0;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 13: {
            A[142]=0;
            A[141]=0;
            A[140]=1;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 14: {
            A[142]=0;
            A[141]=1;
            A[140]=0;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 15: {
            A[142]=1;
            A[141]=0;
            A[140]=0;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 16: {
            A[142]=0;
            A[141]=0;
            A[140]=0;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 17: {
            A[142]=0;
            A[141]=0;
            A[140]=0;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 18: {
            A[142]=0;
            A[141]=0;
            A[140]=1;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 19: {
            A[142]=0;
            A[141]=1;
            A[140]=0;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 20: {
            A[142]=1;
            A[141]=0;
            A[140]=1;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 21: {
            A[142]=0;
            A[141]=1;
            A[140]=0;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 22: {
            A[142]=1;
            A[141]=0;
            A[140]=1;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 23: {
            A[142]=0;
            A[141]=1;
            A[140]=1;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 24: {
            A[142]=1;
            A[141]=1;
            A[140]=1;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 25: {
            A[142]=1;
            A[141]=1;
            A[140]=0;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 26: {
            A[142]=1;
            A[141]=0;
            A[140]=1;
            A[139]=1;
            A[138]=0;
            break;
        }
        case 27: {
            A[142]=0;
            A[141]=1;
            A[140]=1;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 28: {
            A[142]=1;
            A[141]=1;
            A[140]=0;
            A[139]=0;
            A[138]=0;
            break;
        }
        case 29: {
            A[142]=1;
            A[141]=0;
            A[140]=0;
            A[139]=0;
            A[138]=1;
            break;
        }
        case 30: {
            A[142]=0;
            A[141]=0;
            A[140]=0;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 31: {
            A[142]=0;
            A[141]=0;
            A[140]=1;
            A[139]=1;
            A[138]=1;
            break;
        }
        case 32: {
            A[142]=0;
            A[141]=1;
            A[140]=1;
            A[139]=1;
            A[138]=1;
            break;
        }
        default: {
            A[142]=0;
            A[141]=0;
            A[140]=0;
            A[139]=0;
            A[138]=0;
            break;
        }
    }
}

/**************************************************
Setting the Feedback Divider Pre-scalar bits A[136:134]
Range: 1-4
**************************************************/
void PLL::setFbDivPreScaler(int val)
{
    //scan_load[136:126] = PLL_MULTINT;
    //PLL_MULTINT[10] unused, connect to ground
    A[136] = 0;
    //Feedback Divider Pre-Scaler = MULTINT[9:8]
    switch(val) {
        case 1: {
            A[135] = 0;
            A[134] = 0;
            break;
        }
        case 2: {
            A[135] = 1;
            A[134] = 0;
            break;
        }
        case 3: {
            A[135] = 0;
            A[134] = 1;
            break;
        }
        case 4: {
            A[135] = 1;
            A[134] = 1;
            break;
        }
        default: {
            A[135] = 0;
            A[134] = 0;
            break;
        }
    }
}

/**************************************************
Setting the Feedback Divider bits A[133:126]
Range: 1-255
**************************************************/
void PLL::setFbDiv(int val)
{
    //scan_load[133:126] = PLL_MULTINT;
    //Feedback Divider = MULTINT[7:0]
    switch(val) {
        case 1: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 34: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 67: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 2: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 35: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 68: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 3: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 36: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 69: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 4: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 37: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 70: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 5: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 38: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 71: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 6: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 39: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 72: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 7: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 40: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 73: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 8: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 41: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 74: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 9: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 42: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 75: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 10: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 43: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 76: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 11: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 44: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 77: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 12: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 45: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 78: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 13: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 46: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 79: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 14: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 47: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 80: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 15: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 48: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 81: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 16: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 49: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 82: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 17: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 50: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 83: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 18: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 51: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 84: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 19: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 52: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 85: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 20: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 53: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 86: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 21: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 54: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 87: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 22: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 55: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 88: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 23: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 56: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 89: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 24: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 57: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 90: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 25: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 58: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 91: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 26: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 59: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 92: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 27: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 60: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 93: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 28: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 61: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 94: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 29: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 62: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 95: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 30: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 63: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 96: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 31: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 64: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 97: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 32: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 65: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 98: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 33: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 66: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 99: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 100: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 136: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 172: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 101: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 137: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 173: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 102: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 138: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 174: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 103: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 139: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 175: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 104: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 140: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 176: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 105: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 141: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 177: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 106: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 142: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 178: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 107: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 143: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 179: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 108: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 144: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 180: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 109: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 145: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 181: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 110: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 146: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 182: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 111: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 147: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 183: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 112: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 148: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 184: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 113: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 149: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 185: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 114: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 150: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 186: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 115: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 151: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 187: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 116: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 152: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 188: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 117: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 153: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 189: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 118: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 154: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 190: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 119: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 155: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 191: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 120: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 156: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 192: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 121: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 157: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 193: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 122: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 158: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 194: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 123: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 159: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 195: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 124: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 160: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 196: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 125: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 161: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 197: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 126: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 162: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 198: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 127: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 163: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 199: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 128: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 164: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 200: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 129: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 165: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 201: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 130: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 166: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 202: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 131: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 167: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 203: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 132: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 168: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 204: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 133: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 169: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 205: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 134: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 170: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 206: {
            A[133]=1;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 135: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 171: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 207: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 208: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 224: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 240: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 209: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 225: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 241: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 210: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 226: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 242: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 211: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 227: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 243: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 212: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 228: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 244: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 213: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 229: {
            A[133]=0;
            A[132]=1;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 245: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 214: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 230: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 246: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 215: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 231: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 247: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 216: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 232: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 248: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 217: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 233: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 249: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 218: {
            A[133]=1;
            A[132]=1;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 234: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 250: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 219: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 235: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 251: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 220: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 236: {
            A[133]=1;
            A[132]=0;
            A[131]=1;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 252: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 221: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 237: {
            A[133]=0;
            A[132]=1;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 253: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 222: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=0;
            A[127]=0;
            A[126]=1;
            break;
        }
        case 238: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=1;
            A[127]=1;
            A[126]=0;
            break;
        }
        case 254: {
            A[133]=1;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 223: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=1;
            A[129]=0;
            A[128]=0;
            A[127]=1;
            A[126]=1;
            break;
        }
        case 239: {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=1;
            A[128]=1;
            A[127]=0;
            A[126]=0;
            break;
        }
        case 255: {
            A[133]=0;
            A[132]=0;
            A[131]=1;
            A[130]=1;
            A[129]=1;
            A[128]=1;
            A[127]=1;
            A[126]=1;
            break;
        }
        default : {
            A[133]=0;
            A[132]=0;
            A[131]=0;
            A[130]=0;
            A[129]=0;
            A[128]=0;
            A[127]=0;
            A[126]=0;
            break;
        }
    }
}

/**************************************************
Setting the Forward Divider Pre-scalar bits for A A[150:149]
Range: 1,2,4
**************************************************/
void PLL::setFwDivPreScalerA(int val)
{
    //scan_load[150:144] = PLL_RANGEA; => 29
    //RANGEA[6:5]
    switch(val) {
        case 1: {
            A[150] = 0;
            A[149] = 0;
            break;
        }
        case 2: {
            A[150] = 1;
            A[149] = 0;
            break;
        }
        //case 3 not supported
        case 4: {
            A[150] = 1;
            A[149] = 1;
            break;
        }
    }
}

/**************************************************
Setting the Forward Divider Pre-scalar bits A[148:144]
Range: 1-32
**************************************************/
void PLL::setFwDivA(int val)
{
    switch(val) {
        case 32: {
            A[148]=0;
            A[147]=1;
            A[146]=1;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 31: {
            A[148]=0;
            A[147]=1;
            A[146]=1;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 30: {
            A[148]=0;
            A[147]=0;
            A[146]=1;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 29: {
            A[148]=0;
            A[147]=0;
            A[146]=1;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 28: {
            A[148]=0;
            A[147]=0;
            A[146]=0;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 27: {
            A[148]=0;
            A[147]=0;
            A[146]=0;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 26: {
            A[148]=1;
            A[147]=0;
            A[146]=0;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 25: {
            A[148]=1;
            A[147]=0;
            A[146]=0;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 24: {
            A[148]=0;
            A[147]=1;
            A[146]=0;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 23: {
            A[148]=0;
            A[147]=1;
            A[146]=0;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 22: {
            A[148]=0;
            A[147]=0;
            A[146]=1;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 21: {
            A[148]=0;
            A[147]=0;
            A[146]=1;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 20: {
            A[148]=1;
            A[147]=0;
            A[146]=0;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 19: {
            A[148]=1;
            A[147]=0;
            A[146]=0;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 18: {
            A[148]=1;
            A[147]=1;
            A[146]=0;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 17: {
            A[148]=1;
            A[147]=1;
            A[146]=0;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 16: {
            A[148]=0;
            A[147]=1;
            A[146]=1;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 15: {
            A[148]=0;
            A[147]=1;
            A[146]=1;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 14: {
            A[148]=1;
            A[147]=0;
            A[146]=1;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 13: {
            A[148]=1;
            A[147]=0;
            A[146]=1;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 12: {
            A[148]=0;
            A[147]=1;
            A[146]=0;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 11: {
            A[148]=0;
            A[147]=1;
            A[146]=0;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 10: {
            A[148]=1;
            A[147]=0;
            A[146]=1;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 9: {
            A[148]=1;
            A[147]=0;
            A[146]=1;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 8: {
            A[148]=1;
            A[147]=1;
            A[146]=0;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 7: {
            A[148]=1;
            A[147]=1;
            A[146]=0;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 6: {
            A[148]=1;
            A[147]=1;
            A[146]=1;
            A[145]=0;
            A[144]=0;
            break;
        }
        case 5: {
            A[148]=1;
            A[147]=1;
            A[146]=1;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 4: {
            A[148]=1;
            A[147]=1;
            A[146]=1;
            A[145]=1;
            A[144]=0;
            break;
        }
        case 3: {
            A[148]=1;
            A[147]=1;
            A[146]=1;
            A[145]=1;
            A[144]=1;
            break;
        }
        case 2: {
            A[148]=0;
            A[147]=0;
            A[146]=0;
            A[145]=0;
            A[144]=1;
            break;
        }
        case 1: {
            A[148]=0;
            A[147]=0;
            A[146]=0;
            A[145]=0;
            A[144]=0;
            break;
        }
        default: {
            A[148]=0;
            A[147]=0;
            A[146]=0;
            A[145]=0;
            A[144]=0;
            break;
        }
    }
}

/**************************************************
Setting the Forward Divider Pre-scalar bits for B A[157:156]
Range: 1,2,4
**************************************************/
void PLL::setFwDivPreScalerB(int val)
{
    //scan_load[150:144] = PLL_RANGEA; => 29
    //RANGEA[6:5]
    switch(val) {
        case 1: {
            A[157] = 0;
            A[156] = 0;
            break;
        }
        case 2: {
            A[157] = 1;
            A[156] = 0;
            break;
        }
        //case 3 not supported
        case 4: {
            A[157] = 1;
            A[156] = 1;
            break;
        }
    }
}

/**************************************************
Setting the Forward Divider Pre-scalar bits for B A[155:151]
Range: 1-32
**************************************************/
void PLL::setFwDivB(int val)
{
    //scan_load[157:151] = PLL_RANGEB; => 24
    //RANGEB[4:0]
    int init = 151;
    switch(val) {
        case 32: {
            A[155]=0;
            A[154]=1;
            A[153]=1;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 31: {
            A[155]=0;
            A[154]=1;
            A[153]=1;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 30: {
            A[155]=0;
            A[154]=0;
            A[153]=1;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 29: {
            A[155]=0;
            A[154]=0;
            A[153]=1;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 28: {
            A[155]=0;
            A[154]=0;
            A[153]=0;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 27: {
            A[155]=0;
            A[154]=0;
            A[153]=0;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 26: {
            A[155]=1;
            A[154]=0;
            A[153]=0;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 25: {
            A[155]=1;
            A[154]=0;
            A[153]=0;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 24: {
            A[155]=0;
            A[154]=1;
            A[153]=0;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 23: {
            A[155]=0;
            A[154]=1;
            A[153]=0;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 22: {
            A[155]=0;
            A[154]=0;
            A[153]=1;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 21: {
            A[155]=0;
            A[154]=0;
            A[153]=1;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 20: {
            A[155]=1;
            A[154]=0;
            A[153]=0;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 19: {
            A[155]=1;
            A[154]=0;
            A[153]=0;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 18: {
            A[155]=1;
            A[154]=1;
            A[153]=0;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 17: {
            A[155]=1;
            A[154]=1;
            A[153]=0;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 16: {
            A[155]=0;
            A[154]=1;
            A[153]=1;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 15: {
            A[155]=0;
            A[154]=1;
            A[153]=1;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 14: {
            A[155]=1;
            A[154]=0;
            A[153]=1;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 13: {
            A[155]=1;
            A[154]=0;
            A[153]=1;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 12: {
            A[155]=0;
            A[154]=1;
            A[153]=0;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 11: {
            A[155]=0;
            A[154]=1;
            A[153]=0;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 10: {
            A[155]=1;
            A[154]=0;
            A[153]=1;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 9: {
            A[155]=1;
            A[154]=0;
            A[153]=1;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 8: {
            A[155]=1;
            A[154]=1;
            A[153]=0;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 7: {
            A[155]=1;
            A[154]=1;
            A[153]=0;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 6: {
            A[155]=1;
            A[154]=1;
            A[153]=1;
            A[152]=0;
            A[151]=0;
            break;
        }
        case 5: {
            A[155]=1;
            A[154]=1;
            A[153]=1;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 4: {
            A[155]=1;
            A[154]=1;
            A[153]=1;
            A[152]=1;
            A[151]=0;
            break;
        }
        case 3: {
            A[155]=1;
            A[154]=1;
            A[153]=1;
            A[152]=1;
            A[151]=1;
            break;
        }
        case 2: {
            A[155]=0;
            A[154]=0;
            A[153]=0;
            A[152]=0;
            A[151]=1;
            break;
        }
        case 1: {
            A[155]=0;
            A[154]=0;
            A[153]=0;
            A[152]=0;
            A[151]=0;
            break;
        }
        default: {
            A[155]=0;
            A[154]=0;
            A[153]=0;
            A[152]=0;
            A[151]=0;
            break;
        }
    }
}

/**************************************************
Setting LFTUNE bits A[83:43]
Range: 0(2.5-3.2) - 1(3.2-5.0)
int FB_MODE;//feedback mode, 0 for external, 1 for internal
**************************************************/
void PLL::setLFTUNE(int interval)
{
    if (FB_MODE) { //internal
        A[79]=1;//LFTUNE[36]=1
        A[73]=1;//LFTUNE[30]=1
        A[63]=1;//LFTUNE[20]=1
        switch (interval) {
            case 0: {
                A[82]=0;
                A[81]=1;
                break;
            }
            case 1: {
                A[82]=1;
                A[81]=1;
                break;
            }
            default: {
                A[82]=1;
                A[81]=1;
                break;
            }
        }
    } else { //external
        A[83]=1;//LFTUNE[40]=1
        A[79]=1;//LFTUNE[36]=1
        A[63]=1;//LFTUNE[20]=1
        switch (interval) {
            case 0: {
                A[82]=0;
                A[81]=1;
                break;
            }
            case 1: {
                A[82]=1;
                A[81]=1;
                break;
            }
            default: {
                A[82]=1;
                A[81]=1;
                break;
            }
        }
    }
}

/**************************************************
Setting FFTUNE bits A[41:26]
int FB_MODE;//feedback mode, 0 for external, 1 for internal
it seems that Matt's code set A[34] to 1 instead of 35, not sure why
**************************************************/
void PLL::setFFTUNE(int interval)
{
    if (FB_MODE) { //internal
        //A[35]=1;//FFTUNE[9]=1
        //debug
        switch (interval) {
            case 0: {
                A[35]=0;    //slow DCO
                A[34]=1;
                break;
            }
            case 1: {
                A[35]=1;    //fast DCO
                A[34]=0;
                break;
            }
            default: {
                A[35]=0;
                A[34]=1;
                break;
            }
        }
    } else { //external
        A[37]=1;//FFTUNE[11]=1
        switch (interval) {
            case 0: {
                A[35]=0;    //slow DCO
                A[34]=1;
                break;
            }
            case 1: {
                A[35]=1;    //fast DCO
                A[34]=0;
                break;
            }
            default: {
                A[35]=0;
                A[34]=1;
                break;
            }
        }
    }
}

/**************************************************
Setting INTFBK bit A[42]
int FB_MODE;//feedback mode, 0 for external, 1 for internal
**************************************************/
void PLL::setINTFBK()
{
    A[42] = FB_MODE;
}

/**************************************************
Setting the RO bits for A[7:0]
**************************************************/
void PLL::setROandOther()
{
    A[1] = 1; // reset ring_osc
    A[168] = 1;
    A[169] = 1;
}

/**************************************************
Setting internal clk source bits A[9:8]
Range: 0: HCLK_EXT 1: ring_osc 2: PLLOUT_A 3: PLLOUT_B
**************************************************/
void PLL::setINTMUX(int clk)
{
    switch(clk) {
        case 3: {
            A[9]=1;
            A[8]=1;
            break;
        }
        case 2: {
            A[9]=1;
            A[8]=0;
            break;
        }
        case 1: {
            A[9]=0;
            A[8]=1;
            break;
        }
        case 0: {
            A[9]=0;
            A[8]=0;
            break;
        }
        default: {
            A[9]=0;
            A[8]=0;
            break;
        }
    }
}

/**************************************************
Setting external clk source bits A[15:14]
Range: 0: HCLK_EXT 1: HCLK_divider 2: PLLOUT_A 3: PLLOUT_B
**************************************************/
void PLL::setEXTMUX(int clk)
{
    switch(clk) {
        case 3: {
            A[15]=1;
            A[14]=1;
            break;
        }
        case 2: {
            A[15]=1;
            A[14]=0;
            break;
        }
        case 1: {
            A[15]=0;
            A[14]=1;
            break;
        }
        case 0: {
            A[15]=0;
            A[14]=0;
            break;
        }
        default: {
            A[15]=0;
            A[14]=0;
            break;
        }
    }
}

/**************************************************
Setting clk divider bits for B A[13:10]
Range: 0-15, divided by 2^val
**************************************************/
void PLL::setEXTDIV(int val)
{
    switch(val) {
        case 15: {
            A[13]=1;
            A[12]=1;
            A[11]=1;
            A[10]=1;
            break;
        }
        case 14: {
            A[13]=1;
            A[12]=1;
            A[11]=1;
            A[10]=0;
            break;
        }
        case 13: {
            A[13]=1;
            A[12]=1;
            A[11]=0;
            A[10]=1;
            break;
        }
        case 12: {
            A[13]=1;
            A[12]=1;
            A[11]=0;
            A[10]=0;
            break;
        }
        case 11: {
            A[13]=1;
            A[12]=0;
            A[11]=1;
            A[10]=1;
            break;
        }
        case 10: {
            A[13]=1;
            A[12]=0;
            A[11]=1;
            A[10]=0;
            break;
        }
        case 9: {
            A[13]=1;
            A[12]=0;
            A[11]=0;
            A[10]=1;
            break;
        }
        case 8: {
            A[13]=1;
            A[12]=0;
            A[11]=0;
            A[10]=0;
            break;
        }
        case 7: {
            A[13]=0;
            A[12]=1;
            A[11]=1;
            A[10]=1;
            break;
        }
        case 6: {
            A[13]=0;
            A[12]=1;
            A[11]=1;
            A[10]=0;
            break;
        }
        case 5: {
            A[13]=0;
            A[12]=1;
            A[11]=0;
            A[10]=1;
            break;
        }
        case 4: {
            A[13]=0;
            A[12]=1;
            A[11]=0;
            A[10]=0;
            break;
        }
        case 3: {
            A[13]=0;
            A[12]=0;
            A[11]=1;
            A[10]=1;
            break;
        }
        case 2: {
            A[13]=0;
            A[12]=0;
            A[11]=1;
            A[10]=0;
            break;
        }
        case 1: {
            A[13]=0;
            A[12]=0;
            A[11]=0;
            A[10]=1;
            break;
        }
        case 0: {
            A[13]=0;
            A[12]=0;
            A[11]=0;
            A[10]=0;
            break;
        }
        default: {
            A[13]=0;
            A[12]=0;
            A[11]=0;
            A[10]=0;
            break;
        }
    }
}

/**************************************************
Setting clk divider bits for B A[158]
Range: 0:unreset 1:reset
**************************************************/
void PLL::resetPLL(int val)
{
    if (val) {
        A[158]=1;
    } else {
        A[158]=0;
    }
}