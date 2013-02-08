#include "JTAG.h"
#include "mbed.h"

using namespace std;

LocalFileSystem local("local");

Serial pc_jtag(USBTX, USBRX);//tx, rx => for debugging purposes
DigitalOut TCK(p26);
DigitalOut TMS(p25);
DigitalOut TDI(p24);
DigitalIn TDO(p23);
DigitalOut JTAG_RESET (p21);

int expected_result[256] = {
    0x2B8FC0B
    ,    0xF8A2005D
    ,    0xF825F95F
    ,    0x1B1FAA7
    ,    0x24BFFBA
    ,    0xFD0E02DD
    ,    0xFA880752
    ,    0xFCC00793
    ,    0x478FD3C
    ,    0xF9FFFF8D
    ,    0x48B04E1
    ,    0x43202C8
    ,    0xF8AB010B
    ,    0xF93AF535
    ,    0xFE19FA08
    ,    0x7709A0
    ,    0xFBD1FC74
    ,    0x2A20109
    ,    0x76C0722
    ,    0xFFFCFCD2
    ,    0xFE97FF8F
    ,    0x29EFA92
    ,    0x7CB03B6
    ,    0x56017A
    ,    0x2F0FEFF
    ,    0xF695FD10
    ,    0x29405F8
    ,    0xFCD402DD
    ,    0xFEF40220
    ,    0xFC70FBA0
    ,    0xFCBFFA4D
    ,    0xF60323
    ,    0xFC4D00B2
    ,    0xFE73FB95
    ,    0xF9F0FD15
    ,    0x2800057
    ,    0x2390133
    ,    0xFF90FA93
    ,    0xFF75FEEE
    ,    0xFE24022E
    ,    0x59602D5
    ,    0xFE1CFE5A
    ,    0xFC4A052B
    ,    0x575F8D5
    ,    0xA6FA7B
    ,    0xFE87FEE1
    ,    0x2160075
    ,    0x2D0FBD9
    ,    0xFDB5FACF
    ,    0xFD2FFF68
    ,    0x2CC00AC
    ,    0x725FCD0
    ,    0x17202BF
    ,    0x7F6FF0B
    ,    0xFC75FD2D
    ,    0xA39FC4F
    ,    0xD3FA86
    ,    0xFEB0FD08
    ,    0x8BDF829
    ,    0x41D0233
    ,    0x5FB033D
    ,    0xEB0259
    ,    0x2C1FEA4
    ,    0xF8D507D8
    ,    0xFFBCFBBD
    ,    0xFBE9FF9F
    ,    0xFC8D0583
    ,    0x610056F
    ,    0x44FFD1E
    ,    0xFE080213
    ,    0xFDBE02CD
    ,    0x7460455
    ,    0x7F06CA
    ,    0xFECEFDB8
    ,    0xF6E50261
    ,    0xFED5FB0B
    ,    0x4D900E6
    ,    0xFB91FED5
    ,    0xFB7DFC9D
    ,    0xFA630051
    ,    0xF755039F
    ,    0x5A3FE19
    ,    0xFDA5FB37
    ,    0xFBC0FD89
    ,    0xFDC702F5
    ,    0xF93D0164
    ,    0xFD650806
    ,    0xFC70FF32
    ,    0xFCFDF722
    ,    0xFA6C04AF
    ,    0xFD5A017D
    ,    0x44FFB39
    ,    0x2D5FE04
    ,    0xFDF8FEF0
    ,    0xFA2BFF07
    ,    0x160F87E
    ,    0xF7C604B0
    ,    0x24101C0
    ,    0xFC6DF9BB
    ,    0x1D30328
    ,    0x580038F
    ,    0x552FD58
    ,    0xFB93FFC4
    ,    0xFCCD0460
    ,    0x62EFE73
    ,    0x22709A5
    ,    0xFE6AF97A
    ,    0x4C900E1
    ,    0xF6C204F6
    ,    0x21F037F
    ,    0xFFFDFE17
    ,    0xFC70FF92
    ,    0xFD8800E6
    ,    0xFF93FE5E
    ,    0x45E015D
    ,    0x2460564
    ,    0xFCE4FEEF
    ,    0xFFE1FE69
    ,    0x5FD012B
    ,    0x2C80105
    ,    0xFD160071
    ,    0xFC220060
    ,    0xF6ABFA44
    ,    0x1BCFE0F
    ,    0x172FA07
    ,    0xFC4DFB2E
    ,    0x1BF0832
    ,    0x1240277
    ,    0xBAAFFE5
    ,    0xAEFDE6
    ,    0x4410424
    ,    0xF80AFB2F
    ,    0xFD7EFC9F
    ,    0xCEF9FC
    ,    0x34D00EE
    ,    0xD3009F
    ,    0xF756050A
    ,    0xF9BAFEFC
    ,    0xFC240233
    ,    0xFEDA02A5
    ,    0xFF36001A
    ,    0xFFB302FB
    ,    0x5CCFE00
    ,    0xFDF6FE08
    ,    0x21C02E0
    ,    0x516FF02
    ,    0x27F843
    ,    0x45CFE47
    ,    0x195FC3B
    ,    0xFC330442
    ,    0x3F903BD
    ,    0xFA3801CC
    ,    0x19904A3
    ,    0xF75E00B8
    ,    0xFBC30044
    ,    0x38FF43
    ,    0xFE4605A4
    ,    0x32BFD41
    ,    0xFBEBFA81
    ,    0xA8D0848
    ,    0xFE94F9E2
    ,    0x70C0402
    ,    0xFF400475
    ,    0xF721FF3F
    ,    0x632051C
    ,    0x49E0420
    ,    0x796FDF2
    ,    0x3D1F92A
    ,    0x1D3FB6C
    ,    0x53BF9C5
    ,    0x2D90B51
    ,    0x39F0331
    ,    0xD59F9BA
    ,    0x31DFCCC
    ,    0xFB44FC3F
    ,    0x8804D2
    ,    0x136FFFC
    ,    0xFAD20224
    ,    0xF8ADFA57
    ,    0xFD72FA15
    ,    0x3AA099A
    ,    0x39DF85C
    ,    0xFA650025
    ,    0x87FE4C
    ,    0xFC86005A
    ,    0x1700AE1
    ,    0x2230312
    ,    0x4DF0783
    ,    0xFBE80513
    ,    0xFC8F081C
    ,    0x2C6086D
    ,    0x1FE64
    ,    0xF0FB92
    ,    0x11508C5
    ,    0x68FA5D
    ,    0x33047D
    ,    0xBD5FEB1
    ,    0x2D5FDDC
    ,    0x13CFEFC
    ,    0xF3EFA1D
    ,    0xFF880319
    ,    0xFE58FEBD
    ,    0x90265
    ,    0xFBBD0518
    ,    0xFD6FFAC6
    ,    0x28D0215
    ,    0x52F060B
    ,    0x183F76F
    ,    0xFAC3FCB9
    ,    0x5C1FDAD
    ,    0xFF720477
    ,    0x751F5D3
    ,    0x3CC09C7
    ,    0x561045C
    ,    0xFF48FEC1
    ,    0x146F496
    ,    0x2ACFD84
    ,    0xFED403CE
    ,    0xFB91FF04
    ,    0xFC1702A4
    ,    0xFE0A019F
    ,    0xFE560322
    ,    0xFCA400E5
    ,    0xFCF6FC95
    ,    0x656FC73
    ,    0xFDBD015B
    ,    0x402058C
    ,    0xFE63F7AF
    ,    0xF7D30635
    ,    0xBC029A
    ,    0xFDB1017F
    ,    0x336FA01
    ,    0x5F9FDB4
    ,    0x6A701D0
    ,    0xFD92FFA8
    ,    0x30E0752
    ,    0x4C302E4
    ,    0xFE9DFF0E
    ,    0x10DFE7D
    ,    0xFB1E099F
    ,    0xFEBDFE61
    ,    0xFD870408
    ,    0x239FEA7
    ,    0x37004CE
    ,    0xFD69FF8D
    ,    0xFD8402A8
    ,    0x28A05B0
    ,    0xF90C0032
    ,    0xFB21037A
    ,    0xFB3CFF8A
    ,    0xF4FDFC3C
    ,    0xFA5502C5
    ,    0x6A702EA
    ,    0x27C0484
    ,    0x500002D
    ,    0x937052F
};

JTAG::JTAG()
{
    TDO.mode(PullUp);
    delay = 10;
    TMS = 0;
    TCK = 0;
    TDI = 0;
    return;
}

unsigned int JTAG::readID(void)
{
    unsigned int id = 0;
    setState('r');
    leaveState();
    setState('d');
    unsigned char a,b,c,d;
    a = readByte();
    b = readByte();
    c = readByte();
    d = readByte();
    leaveState();
    id = id | d;
    id = id << 8;
    id = id | c;
    id = id << 8;
    id = id | b;
    id = id << 8;
    id = id | a;
    pc_jtag.printf("JTAG ID: 0x%X\n",id);
    return id;
}

bool JTAG::JTAG_test(void)
{

    //for FFT test
    unsigned int address = 0x44000008;
    unsigned int value = 0x00000001;
    JTAG_RESET = 0;
    wait_us(100);
    JTAG_RESET = 1;
    wait_us(100);

    //readID();
    DAP_enable();

    loadProgram();

    address = 0x60000100;
    value = 0x0000FFFF;
    writeMemory(address, value);
    address = 0x60000104;
    value = 0x0000FFFF;
    writeMemory(address, value);
    address = 0x60000108;
    value = 0x0000FFFF;
    writeMemory(address, value);

    address = 0x44000008;
    value = 0x00000001;
    writeMemory(address, value);


    /*
    wait(0.5);
    bool if_match = true;
    for(int i=0; i<256; i++) {
        address = 0x60000100;
        address += i*0x4;
        value = readMemory(address);
        if (expected_result[i]!=value) {
            if_match = false;
        }
    }
    return if_match;
    */
    return true;
}

void JTAG::loadProgram()
{
    unsigned int address;
    unsigned int value;

    address = 0xE000EDF0;
    value = 0xA05F0003;
    writeMemory(address, value);
    value = readMemory(address);


    if (value & 0x00000003 == 0x00000003) {
        //pc_jtag.printf("Halting core...\n");
    } else {
        pc_jtag.printf("cannot halt the core, check DHCSR...\n");
        return;
    }

    address = 0x44000008;
    value = 0x00000000;
    writeMemory(address, value);

    FILE *fp = fopen("/local/program.hex", "r");
    if (fp == NULL) {
        pc_jtag.printf("Error in open /local/program.hex\n");
        return;
    }
    //pc_jtag.printf("open /local/program.hex...OK\n");
    address = 0x10000000;
    value = 0x00000000;
    char buf = 'm';
    bool file_end = false;
    int word_loc=0;
    while (!feof(fp)) {

        buf = fgetc(fp);
        word_loc++;
        switch (buf) {
            case '0' :
                value <<= 4;
                value += 0x0;
                break;
            case '1' :
                value <<= 4;
                value += 0x1;
                break;
            case '2' :
                value <<= 4;
                value += 0x2;
                break;
            case '3' :
                value <<= 4;
                value += 0x3;
                break;
            case '4' :
                value <<= 4;
                value += 0x4;
                break;
            case '5' :
                value <<= 4;
                value += 0x5;
                break;
            case '6' :
                value <<= 4;
                value += 0x6;
                break;
            case '7' :
                value <<= 4;
                value += 0x7;
                break;
            case '8' :
                value <<= 4;
                value += 0x8;
                break;
            case '9' :
                value <<= 4;
                value += 0x9;
                break;
            case 'a' :
                value <<= 4;
                value += 0xA;
                break;
            case 'b' :
                value <<= 4;
                value += 0xB;
                break;
            case 'c' :
                value <<= 4;
                value += 0xC;
                break;
            case 'd' :
                value <<= 4;
                value += 0xD;
                break;
            case 'e' :
                value <<= 4;
                value += 0xE;
                break;
            case 'f' :
                value <<= 4;
                value += 0xF;
                break;
            case 'A' :
                value <<= 4;
                value += 0xA;
                break;
            case 'B' :
                value <<= 4;
                value += 0xB;
                break;
            case 'C' :
                value <<= 4;
                value += 0xC;
                break;
            case 'D' :
                value <<= 4;
                value += 0xD;
                break;
            case 'E' :
                value <<= 4;
                value += 0xE;
                break;
            case 'F' :
                value <<= 4;
                value += 0xF;
                break;
            default  :
                word_loc--;
                break;
        }
        if(word_loc>7) {
            writeMemory(address, value);
            address += 4;
            value = 0;
            word_loc = 0;
        }
    }
    fclose(fp);
}

unsigned int JTAG::readMemory(unsigned int address)
{
    unsigned char a, b, c, d, e;
    d = address & 0xFF;
    address = address >>  8;
    c = address & 0xFF;
    address = address >>  8;
    b = address & 0xFF;
    address = address >>  8;
    a = address & 0xFF;

    // set TAR
    e = 0x40;
    writeAPACC(a,b,c,d,e);

    // read DRW
    e = 0xE0;
    writeAPACC(a,b,c,d,e);
    readAPACC(a,b,c,d,e);

    unsigned int out = 0;
    out = out | a;
    out = out << 8;
    out = out | b;
    out = out << 8;
    out = out | c;
    out = out << 8;
    out = out | d;

    return out;
}

void JTAG::writeMemory(unsigned int address, unsigned int value)
{
    unsigned char a, b, c, d, e;
    d = address & 0xFF;
    address = address >>  8;
    c = address & 0xFF;
    address = address >>  8;
    b = address & 0xFF;
    address = address >>  8;
    a = address & 0xFF;

    // set TAR
    e = 0x40;
    writeAPACC(a,b,c,d,e);

    d = value & 0xFF;
    value = value >>  8;
    c = value & 0xFF;
    value = value >>  8;
    b = value & 0xFF;
    value = value >>  8;
    a = value & 0xFF;

    // check DRW
    e = 0xC0;
    writeAPACC(a,b,c,d,e);

}

void JTAG::DAP_enable(void)
{
    setState('r');
    leaveState();
    // write CTRL to enable DAP
    unsigned char a, b, c, d, e;
    a = 0x50;
    b = 0x00;
    c = 0x00;
    d = 0x00;
    e = 0x40;
    writeDPACC(a,b,c,d,e);
    readDPACC(a,b,c,d,e);

    // set AP select
    a = 0x00;
    b = 0x00;
    c = 0x00;
    d = 0x00;
    e = 0x80;
    writeDPACC(a,b,c,d,e);
    readDPACC(a,b,c,d,e);

    // set AP CSW
    a = 0x23;
    b = 0x00;
    c = 0x00;
    d = 0x42;
    e = 0x00;
    writeAPACC(a,b,c,d,e);
    readAPACC(a,b,c,d,e);

    // set AP CSW
    a = 0x23;
    b = 0x00;
    c = 0x00;
    d = 0x52;
    e = 0x00;
    writeAPACC(a,b,c,d,e);
    readAPACC(a,b,c,d,e);

}

void JTAG::setIR(unsigned char A)
{
    setState('i');
    writeByte(A);
    leaveState();
}

void JTAG::readDPACC(unsigned char& A, unsigned char& B, unsigned char& C, unsigned char& D, unsigned char& E)
{
    unsigned char i;
    E = 0;
    i = 0xA0;
    setIR(i);
    DataHigh();
    setState('d');
    if (TDO) {
        E += 0x04;
    }
    clockTicks(1);
    if (TDO) {
        E += 0x02;
    }
    clockTicks(1);
    if (TDO) {
        E += 0x01;
    }
    clockTicks(1);
    D = readByte();
    C = readByte();
    B = readByte();
    A = readByte();
    leaveState();
}

void JTAG::readAPACC(unsigned char& A, unsigned char& B, unsigned char& C,
                     unsigned char& D, unsigned char& E)
{
    unsigned char i;
    E = 0;
    i = 0xB0;
    setIR(i);
    DataHigh();
    setState('d');
    if (TDO) {
        E += 0x04;
    }
    clockTicks(1);
    if (TDO) {
        E += 0x02;
    }
    clockTicks(1);
    if (TDO) {
        E += 0x01;
    }
    clockTicks(1);
    D = readByte();
    C = readByte();
    B = readByte();
    A = readByte();
    leaveState();
}

void JTAG::writeAPACC(unsigned char A, unsigned char B, unsigned char C,
                      unsigned char D, unsigned char E)
{
    unsigned char i;
    i = 0xB0;
    setIR(i);
    setState('d');
    writeByte(E);
    writeByte(D);
    writeByte(C);
    writeByte(B);
    writeByte(A);
    leaveState();
}

void JTAG::writeDPACC(unsigned char A, unsigned char B, unsigned char C, unsigned char D, unsigned char E)
{
    unsigned char i;
    i = 0xA0;
    setIR(i);
    setState('d');
    writeByte(E);
    writeByte(D);
    writeByte(C);
    writeByte(B);
    writeByte(A);
    leaveState();
}

//moves to specified state from IDLE (reset from anywhere)
void JTAG::setState(unsigned char c)
{
    switch (c) {
        case 'n':
            break;
        case 'r':
            reset();
            break;
        case 'd':
            TMSHigh();
            clockTicks(1);
            TMSLow();
            clockTicks(2);
            state = 'd';
            break;
        case 'i':
            TMSHigh();
            clockTicks(2);
            TMSLow();
            clockTicks(2);
            state = 'i';
            break;
        default:
            break;
    }
}

void JTAG::setJTAGspeed(int speed)
{
    delay = 1000/speed;
    return;
}

//leave from current state to idle state
void JTAG::leaveState(void)
{
    switch (state) {
        case 'n':
            break;
        case 'r':
            TMSLow();
            clockTicks(1);
            state = 'n';
            break;
        case 'i':
            TMSHigh();
            clockTicks(2);
            TMSLow();
            clockTicks(1);
            state = 'n';
            break;
        case 'd':
            TMSHigh();
            clockTicks(2);
            TMSLow();
            clockTicks(1);
            state = 'n';
            break;
        default:
            break;
    }
}

void JTAG::reset(void)
{
    TMSHigh();
    clockTicks(10);
    TMSLow();
    state = 'r';
    return;
}

void JTAG::writeByte(char c)
{
    clockLow();
    int i;
    for (i=0; i<8; i++) {
        clockTicks(1);
        if ( (c & 1)== 0 ) {
            DataLow();
        } else {
            DataHigh();
        }
        c=c>>1;
    }
}

char JTAG::readByte(void)
{
    char c=0;
    clockLow();
    int i;
    for (i=0; i<8; i++) {
        c=c>>1;
        if (TDO) {
            c+=0x80;
        }
        clockTicks(1);

    }
    return c;
}

void JTAG::DataLow(void)
{
    wait_us(delay);
    TDI = 0;
}
void JTAG::DataHigh(void)
{
    wait_us(delay);
    TDI = 1;
}

void JTAG::clockLow(void)
{
    wait_us(delay);
    TCK = 0;
}

void JTAG::clockHigh(void)
{
    wait_us(delay);
    TCK = 1;
}

void JTAG::clockTicks(unsigned char c)
{
    int i;
    clockLow();
    for (i=0; i<c; i++) {
        clockLow();
        clockHigh();
    }
    clockLow();
}

void JTAG::TMSHigh(void)
{
    wait_us(delay);
    TMS = 1;
}

void JTAG::TMSLow(void)
{
    wait_us(delay);
    TMS = 0;
}




















