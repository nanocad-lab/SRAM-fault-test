/*
This is the JTAG driver file for mbed master
Refer to buspriate + openOCD
*/

class JTAG {
    public:
        JTAG();
        void setJTAGspeed(int speed);    // set the JTAG transaction speed
        unsigned int readID(void);   // read the device JTAG ID
        void DAP_enable(void);
        void loadProgram(void);     // load program.hex into the chip
        //basic JTAG state machine related functions
        void setState(unsigned char state);
        void leaveState(void);
        void reset(void);
        //higher level operations
        void setIR(unsigned char A);
        void writeDPACC(unsigned char A, unsigned char B, unsigned char C, unsigned char D, unsigned char E);
        void writeAPACC(unsigned char A, unsigned char B, unsigned char C, unsigned char D, unsigned char E);
        void readDPACC(unsigned char& A, unsigned char& B, unsigned char& C, unsigned char& D, unsigned char& E);
        void readAPACC(unsigned char& A, unsigned char& B, unsigned char& C, unsigned char& D, unsigned char& E);
        unsigned int readMemory(unsigned int address);
        void writeMemory(unsigned int address, unsigned int value);
        //high level byte operation
        void writeByte(char c);
        char readByte(void);
        //bit level functions
        void writeBit(char c);
        unsigned char readBit(void);
        unsigned char readDataState(void);
        //bus level control
        void DataLow(void);
        void DataHigh(void);
        void clockLow(void);
        void clockHigh(void);
        void clockTicks(unsigned char c);
        void TMSHigh(void);
        void TMSLow(void);
        bool JTAG_test(void);
        char state; // n=null, r=reset, d=data, i=instruction
        int delay; // wait time for each signal switching, in us
};