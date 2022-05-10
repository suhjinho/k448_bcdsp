#include "Global_Variable_Function.h"

extern Uint16 msec_tmr;	
// Initialize SPI for DAC7714 DAC device
unsigned int testdac16=0;

float batcur_dac=0.0;	    		
float batcur_sum=0.0;
unsigned int batcur_idx=0;    		


void Init_Spi_Dac(void){
	// Initialize GPIO for SPI-A according to MCP4822 DAC
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;  // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;  // Configure GPIO56 as SPICLKA
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;  // Configure GPIO57 as SPISTEA
	
	/* Configure /LDAC */
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;  // Configure GPIO58 as GPIO(/LDAC)
	GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;	  // Output Mode
	GpioDataRegs.GPBSET.bit.GPIO58 = 1; 
	EDIS;
	
	// Disable the FIFO operation
	SpiaRegs.SPIFFTX.all = 0xA000;
	SpiaRegs.SPIFFRX.all = 0x2000;
	SpiaRegs.SPIFFCT.all = 0x0;

	SpiaRegs.SPICCR.all =0x000F;	// Reset on, rising edge, 16-bit char bits  
	SpiaRegs.SPICTL.all =0x000E;    // Enable master mode, one half-cycle delay phase,
                                    // Enable TALK, and SPI int disabled.

	// SPI clock speed가 20MHz(DAC소자가 지원하는 최대속도)가 넘지 않도록 설정함
	// For SPIBRR = 3 to 127	LSPCLK/(SPIBRR+1)  LSPCLK = 75M/2=37.5MHz
	// For SPIBRR = 0, 1 to 2	LSPCLK/4
	// Here, SPIBRR = 16;
	SpiaRegs.SPIBRR =0x000a;			// Maximum SPI speed(LSPCLK/16 = 150MHz/32 = 4.6875MHz)									
    SpiaRegs.SPICCR.all =0x008F;	// Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;	// Set so breakpoints don't disturb xmission	 
}

// DAC 채널1/2을 출력하는 함수
void DAC_Out(WORD ch, WORD dat){
WORD d;
	
	switch(ch)
	{
		case 0	:	d = (0x0000 | (dat & 0xfff));	// DAC_A
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
				    break;
		case 1	:	d = (0x4000 | (dat & 0xfff));	// DAC_B
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		case 2	:	d = (0x8000 | (dat & 0xfff));	// DAC_C
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		case 3	:	d = (0xc000 | (dat & 0xfff));	// DAC_D
					SpiaRegs.SPIDAT = d;	
					while(SpiaRegs.SPISTS.bit.INT_FLAG == 0);
				    d = SpiaRegs.SPIRXBUF;			// clear SPI INT Flag
					break;
		default : 	break;

	}

	//while(!SpiaRegs.SPISTS.bit.INT_FLAG);
	
	// Set /LDAC signal timing according to MCP4822 DAC device datasheet
	DELAY_US(0.04L);
	GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;
	DELAY_US(0.1L);
	GpioDataRegs.GPBSET.bit.GPIO58 = 1;	
}

#define TESTCARDMODE_TRAIN_0			0
#define TESTCARDMODE_SELF_TEST_1		1

#define TESTCARDMODE_APS_SEQ_10			10
#define TESTCARDMODE_APS_SEN_11			11
#define TESTCARDMODE_APS_TUNNING_12		12
#define TESTCARDMODE_APS_TEST_13		13
#define TESTCARDMODE_APS_TEST_14		14
#define TESTCARDMODE_APS_TEST_15		15

#define TESTCARDMODE_BACH_SEQ_20		20
#define TESTCARDMODE_BACH_SEN_21		21
#define TESTCARDMODE_BACH_TUNNING_22	22
#define TESTCARDMODE_BACH_TEST_23		23
#define TESTCARDMODE_BACH_TEST_24		24
#define TESTCARDMODE_BACH_TEST_25		25

Uint16 TesTCard_mode = TESTCARDMODE_TRAIN_0;

//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// dac chip DAC8413FPC(PLCC-28)
// dac address
//=========================================
// TEST 보드 어드레스
//=========================================
//#define TEST_DA1_CS()		PWORD(0x20B000)
//================APS=================================================//
// 시제차량 계측모니터링 //
//입력전압(실효치)	전압출력	±10 V	100V/1.5V	380V일때 5.7V출력
//입력전류실효치)	전압출력	±10 V	100A/1.5V	
//출력전압(실효치)	전압출력	±10 V	100V/1.5V	440V일때 6.6V출력
//출력전류(실효치)	전압출력	±10 V	100A/2V	
//장비내부온도		전압출력	±10 V	1도/0.1V	0도일때 0V,-35도일때 -3.5V,+20도일때 2.0V
//=================================================================//
float   	Dac1_data;
float   	Dac2_data;
float   	Dac3_data;
float   	Dac4_data;
float   	Dac5_data;
float   	Dac6_data;
float   	Dac1_data1;
float   	Dac2_data2;
float   	Dac3_data3;
float   	Dac4_data4;
float   	Dac5_data5;
float   	Dac6_data6;

float   	ttst_DAC1;
float   	ttst_DAC2;
float   	ttst_DAC3;
float   	ttst_DAC4;
float   	ttst_DAC5;
float   	ttst_DAC6;
float   	ttst_DAC7;
float   	ttst_DAC8;
float   	ttst_DAC9;
float   	ttst_DAC10;
float   	ttst_DAC11;
float   	ttst_DAC12;
float   	ttst_DAC13;
float   	ttst_DAC14;
float   	ttst_DAC15;
float   	ttst_DAC16;

void Tcard_DAC_Out(WORD ch, WORD dat)
{
    PWORD(0x20B000+ch-1) = dat & 0xfff;		//Heartbeat(H)
}	

// 제어게인 

//===DAC13은 출력안됨..
void Tcard_Sequence_Out()
{
	
    
}	

void InitTestVariable()
{
	// variable.h에 선언되었슴
	tst_DAC1 = 0;
	tst_DAC2 = 0;
	tst_DAC3 = 0;
	tst_DAC4 = 0;
	tst_DAC5 = 0;
	tst_DAC6 = 0;
	tst_DAC7 = 0;
	tst_DAC8 = 0;
	tst_DAC9 = 0;
	tst_DAC10 = 0;
	tst_DAC11 = 0;
	tst_DAC12 = 0;
	tst_DAC13 = 0;
	tst_DAC14 = 0;
	tst_DAC15 = 0;
	tst_DAC16 = 0;
	tst_DAC17 = 0;
	
	tst_DAC1offset = 0;
	tst_DAC2offset = 0;
	tst_DAC3offset = 0;
	tst_DAC4offset = 0;
	tst_DAC5offset = 0;
	tst_DAC6offset = 0;
	tst_DAC7offset = 0;
	tst_DAC8offset = 0;
	tst_DAC9offset = 0;
	tst_DAC10offset = 0;
	tst_DAC11offset = 0;
	tst_DAC12offset = 0;
	tst_DAC13offset = 0;
	tst_DAC14offset = 0;
	tst_DAC15offset = 0;
	tst_DAC16offset = 0;
	
}

void Init_GPIO_Dac(void)
{
		EALLOW;
		PieVectTable.SPIRXINTA = &spiRxFifoIsr;
 		PieVectTable.SPITXINTA = &spiTxFifoIsr;
	/* Enable internal pull-up for the selected pins */
		GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA
		GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   
		GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
		GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

	/* Set qualification for selected pins to asynch only */
		GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
		GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3;
		GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
		GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

	/* Configure SPI-A pins using GPIO regs*/
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;  // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;  // Configure GPIO56 as SPICLKA
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;
	IER |= M_INT6;
	EDIS;
}

void spi_fifo_init()
{
// Initialize SPI FIFO registers
   SpiaRegs.SPICCR.bit.SPISWRESET=0; // Reset SPI

//   SpiaRegs.SPICCR.all=0x001F;       //16-bit character, Loopback mode on
   SpiaRegs.SPICCR.all=0x0007;       //8-bit character, Loopback mode off
   SpiaRegs.SPICTL.all=0x0017;       //Interrupt enabled, Master/Slave XMIT enabled
   SpiaRegs.SPISTS.all=0x0000;
//20190318 SPI 속도 증가      SpiaRegs.SPIBRR=0x0032;           // Baud rate
   SpiaRegs.SPIBRR=0x000a;     //20190323 Baud rate 수정       // Baud rate
//   SpiaRegs.SPIFFTX.all=0xC028;      // Enable FIFO's, set TX FIFO level to 8
//   SpiaRegs.SPIFFRX.all=0x0028;      // Set RX FIFO level to 8

   SpiaRegs.SPIFFTX.all=0xC02C;      // Enable FIFO's, set TX FIFO level to txff_point
   SpiaRegs.SPIFFRX.all=0x0000;      // Set RX FIFO level to txff_point

   SpiaRegs.SPIFFCT.all=0x00;
   SpiaRegs.SPIPRI.all=0x0010;

   SpiaRegs.SPICCR.bit.SPISWRESET=1;  // Enable SPI

   SpiaRegs.SPIFFTX.bit.TXFIFO= 0;
   SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;
}

void DAC_CMDCH_init(void)
{
	DAC_CMD_SEQ = 0;
	DAC_CH_SEQ = 0;
	DAC_CONTROL = 0;
	aaaaa = 0;
	bbbbb = 0;
	ccccc = 0;
	ddddd = 0;
	
}
void DAC_OUT()
{
	SpiaRegs.SPIFFTX.bit.TXFIFO=1;
}
void DAC_STOP()
{
	DAC_CMD_SEQ = 0;
	SpiaRegs.SPIFFTX.bit.TXFIFO=0;
}