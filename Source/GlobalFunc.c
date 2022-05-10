#include "Global_Variable_Function.h"
// Prototype Statements for functions found within this file.
interrupt void wakeint_isr(void);

// Global variables for this example
Uint32 WakeCount;
interrupt void wakeint_isr(void)
{
	WakeCount++;
	
	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
void PeripheralInit()
{

	InitSysCtrl();// Step 1. Initialize System Control: PLL, WatchDog, enable Peripheral Clocks

	Init_Gpio();// Step 2. Initalize GPIO:
	
	DINT;// Step 3. Clear all interrupts and initialize PIE vector table: Disable CPU interrupts

	InitPieCtrl();// Initialize the PIE control registers to their default State.
	              // The default State is all PIE interrupts disabled and flags are cleared.
	IER = 0x0000;// Disable CPU interrupts and clear all CPU interrupt flags:
	IFR = 0x0000;

	InitPieVectTable();// Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
	
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart); // FLASH Code Copy and �ʱ�ȭ ��ƾ 
		// Copy time critical code and Flash setup code to RAM
		// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart symbols are created by the linker. Refer to the FLASH.cmd file.                                     

	InitFlash();// Call Flash Initialization to setup flash waitStates This function must reside in RAM
	//-----------------------------------------------------------------------------
	// Step 4. Initialize the Device Peripheral.
	Init_Ext_Bus();		// Initalize External Interface according to RealSYS DSP28335 DSK
	
	InitCpuTimers();

	sci_debug_init();// Initialize SCI-A for data monitoring 

	//Init_Spi_Dac();	// Initailze SPI-A for 2ch 12bit DAC(MCP4822)
	Init_GPIO_Dac(); //Initailize DAC GPIO
	spi_fifo_init(); //Initailize DAC ���
	DAC_CMDCH_init(); //Initailize DAC ����

	
	// ePWM���� ����	
	InitBachEpwm(); // 20100707 ������ APS EPWM �¾�	
	

	// Initialize ADC operation mode
	InitADC();
	Init_External_Interrupt();	// Initialize external interrupt

	//CANInit();	// Initialize CAN Port
	  
  /* 	EALLOW;  // This is needed to write to EALLOW protected registers
   	//PieVectTable.TINT0     = &cpu_timer0_isr;
  	// PieVectTable.XINT13    = &cpu_timer1_isr;
   	PieVectTable.SCIRXINTB = &scib_RxFifo_isr; // SCI-B
   	PieVectTable.SCITXINTB = &scib_TxFifo_isr; // SCI-B
   	PieVectTable.SCIRXINTC = &scic_RxFifo_isr; // SCI-C
   	PieVectTable.SCITXINTC = &scic_TxFifo_isr; // SCI-C
   	PieVectTable.ECAN0INTB = &eCAN0INTB_ISR;
   	PieVectTable.ECAN1INTB = &eCAN1INTB_ISR;	
   	EDIS;    // This is needed to disable write to EALLOW protected registers
	*/
		PieCtrlRegs.PIEIER6.bit.INTx1=1;     // Enable PIE Group 6, SPI_TX FIFO
   	PieCtrlRegs.PIEIER6.bit.INTx2=1;     // Enable PIE Group 6, SPI_RX FIFO	
	// Enable PIE Group 8, Interrupt 5 for SCIC RX
   	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
// 	Enable PIE Group 8, Interrupt 6 for SCIC TX
   	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
// 	Enable PIE Group 9, Interrupt 3 for SCIB RX
   	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
// 	Enable PIE Group 9, Interrupt 4 for SCIB TX
   	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
// 	Enable PIE Group 9, Interrupt 7 for eCANB 0
   	PieCtrlRegs.PIEIER9.bit.INTx7 = 1;
// 	Enable PIE Group 9, Interrupt 8 for eCANB 1
   	PieCtrlRegs.PIEIER9.bit.INTx8 = 1;

// 	Enable the PIE block
   	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
// 	Enable CPU INT8&INT9 for SCIC & SCIB: Enable Comm Interrupts
//   	EN_COM();
	// Enable WAKEINT in the PIE: Group 1 interrupt 8
	// Enable INT1 which is connected to WAKEINT:
   //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   //PieCtrlRegs.PIEIER1.bit.INTx8 = 1;   // Enable PIE Gropu 1 INT8
   //IER |= M_INT1;                       // Enable CPU int1
	
	// bit2 : 0 -> \WDRST output signal is enabled (��ġ���� ����)
	// bit2 : 1 -> \WDINT output signal is enabled(��ġ���� ���ͷ�Ʈ �� ���)
	EALLOW;
   	SysCtrlRegs.SCSR = 0x00;
   	EDIS;
	
   	// Reset the watchdog counter
   	ServiceDog();
    //��ġ���� ī���� Ŭ�� ����
    // ��ġ���� ī���� Ŭ��(�ð�)�� Ŭ���� �ϴ� �ֱ� ���� ���� �Ѵ�.
    // �� �ý����� 400us�̹Ƿ� �̰ͺ��� ũ�� Ŭ�� ���� �����ߴ�.  
	// OSCLCK = 30MHz
	// 000=OSCCLK/512/1
	// 001=OSCCLK/512/1
	// 010=OSCCLK/512/2
	// 011=OSCCLK/512/4
	// 100=OSCCLK/512/8 
	// 101=OSCCLK/512/16 273us
	// 110=OSCCLK/512/32 546us(��)
	// 111=OSCCLK/512/64 1.092ms(��)
    
   	EALLOW;
   	SysCtrlRegs.WDCR = 0x0028+0x06;   
   	EDIS;

	// Enables the PIE module and CPU interrupts and global Interrupt INTM
	// refer to DSP2833x_PieCtrl.c file
	EnableInterrupts();
	
	// Enable Global realtime interrupt DBGM
	ERTM; 
	
	// Connect the watchdog to the WAKEINT interrupt of the PIE
	// Write to the whole SCSR register to avoid clearing WDOVERRIDE bit

    
}

void Init_Gpio(void){
	EALLOW;
	//---------------------------------------
	// GPBMUX1 = 32~47,GPAMUX1 = 00~15 
	// GPBMUX2 = 48~63,GPAMUX2 = 16~31
	//---------------------------------------
	// Main Dsp ���� ����͸�
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	
	// Dip Switch �Է�
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO16 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;
	
	// Hex Switch �Է�
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO62 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO63 = 0;
	
	//VFD Switch �Է�
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;
	EDIS;
}


// DSP28335 DSK���� �ܺ� ���� ���� �� Zone0��  Zone7�� 16��Ʈ ���� �ʱ�ȭ�ϴ� �Լ�
// RealSYS DSP28335 DSK������ SCI-A�� CAN-A�� ����ϹǷ� A17 ~ A19�� External Address ������ ������� ����.
// ���� �ִ� ���� �ִ� ���� ������� 0x100000 -> 0x20000 ��. ���� �����Ͻ�� ٶ��ϴ�.
void Init_Ext_Bus(void){
	// Make sure the XINTF clock is enabled
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

	// Configure the GPIO for XINTF with a 16-bit data bus
	// RealSYS DSP28335 DSK�� ���� �ʿ��� GPIO���� �Ҵ���
	// RealSYS DSP28335 DSK������ SCI-A�� CAN-A�� ����ϹǷ�
	// A17 ~ A19�� External Address ������ ������� ������
	// ���� Ready ��ȣ�� ������� ����.
	EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;  // XD15
	GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // XD14
	GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;  // XD13
	GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;  // XD12
	GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;  // XD11
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // XD10
	GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;  // XD19
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // XD8
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;  // XD7
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // XD6
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;  // XD5
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;  // XD4
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;  // XD3
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;  // XD2
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;  // XD1
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;  // XD0

	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3;  // XA0/XWE1n
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3;  // XA1
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;  // XA2
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;  // XA3
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3;  // XA4
	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3;  // XA5
	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3;  // XA6
	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3;  // XA7

	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;  // XA8
	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;  // XA9
	GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;  // XA10
	GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;  // XA11
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;  // XA12
	GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;  // XA13
	GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;  // XA14
	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;  // XA15
//	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 3;  // XA16
//	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 3;  // XA17
//	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 3;  // XA18
//	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // XA19

//	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 3;  // XREADY
	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 3;  // XRNW
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 3;  // XWE0

	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 3;  // XZCS0
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3;  // XZCS7
//	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // XZCS6
	
	// All Zones---------------------------------
	// Timing for all zones based on XTIMCLK = SYSCLKOUT
	XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
	// XCLKOUT = XTIMCLK               
	XintfRegs.XINTCNF2.bit.CLKMODE = 1;
	// XCLKOUT is enabled
	XintfRegs.XINTCNF2.bit.CLKOFF = 0;
	// Buffer up to 3 writes
	XintfRegs.XINTCNF2.bit.WRBUFF = 3;
	
	
	
	
	// Zone 0(Access Time�� ���� �ܺ� ��ġ�� ����) ----------------
	// When using ready, ACTIVE must be 1 or greater
	// Lead must always be 1 or greater
	// Zone 0 write timing
	XintfRegs.XTIMING0.bit.XWRLEAD = 3;
	XintfRegs.XTIMING0.bit.XWRACTIVE = 15;
	XintfRegs.XTIMING0.bit.XWRTRAIL = 2;
	// Zone 0 read timing
	XintfRegs.XTIMING0.bit.XRDLEAD = 3;
	XintfRegs.XTIMING0.bit.XRDACTIVE = 15;
	XintfRegs.XTIMING0.bit.XRDTRAIL = 2;

	// don't double Zone 0 read/write lead/active/trail timing
	XintfRegs.XTIMING0.bit.X2TIMING = 0;

	// Zone 0 will not sample XREADY signal
	XintfRegs.XTIMING0.bit.USEREADY = 0;
	XintfRegs.XTIMING0.bit.READYMODE = 0;

	// Size must be either:
	// 0,1 = x32 or
	// 1,1 = x16 other values are reserved
	XintfRegs.XTIMING0.bit.XSIZE = 3;

	// Zone 7(Wiznet W3150A+�� ����) ------------------------------
	// �׽�Ʈ ��� �Ʒ��� Read/Write Ÿ�� ���� �۰� ������ ��쿡��
	// W3150A+�� �������� �����ϴ�.
	// When using ready, ACTIVE must be 1 or greater
	// Lead must always be 1 or greater
	// Zone 7 write timing
	//XintfRegs.XTIMING7.bit.XWRLEAD = 3;    
	//XintfRegs.XTIMING7.bit.XWRACTIVE = 7; 
	//XintfRegs.XTIMING7.bit.XWRTRAIL = 2;   
	//// Zone 7 read timing                  
	//XintfRegs.XTIMING7.bit.XRDLEAD = 3;    
	//XintfRegs.XTIMING7.bit.XRDACTIVE = 7; 
	//XintfRegs.XTIMING7.bit.XRDTRAIL = 2;   

	XintfRegs.XTIMING7.bit.XWRLEAD = 3;
	XintfRegs.XTIMING7.bit.XWRACTIVE = 15;
	XintfRegs.XTIMING7.bit.XWRTRAIL = 2;
	// Zone 7 read timing
	XintfRegs.XTIMING7.bit.XRDLEAD = 3;
	XintfRegs.XTIMING7.bit.XRDACTIVE = 15;
	XintfRegs.XTIMING7.bit.XRDTRAIL = 2;

	// don't double Zone 7 read/write lead/active/trail timing
	XintfRegs.XTIMING7.bit.X2TIMING = 1;

	// Zone 7 will not sample XREADY signal
	XintfRegs.XTIMING7.bit.USEREADY = 0;
	XintfRegs.XTIMING7.bit.READYMODE = 0;

	// 1,1 = x16 data bus
	// 0,1 = x32 data bus
	// other values are reserved
	XintfRegs.XTIMING7.bit.XSIZE = 3;

	// Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 7 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 3;
    XintfRegs.XBANK.bit.BCYC = 3;
	EDIS;

	//Force a pipeline flush to ensure that the write to
	//the last register configured occurs before returning.
	asm(" RPT #7 || NOP");
}

/*
//====�����ҽ�===
void Init_External_Interrupt(void)
{
	// Set GPIO Interrupt Select
	EALLOW; 
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 18; // GPIO12
	PieVectTable.XINT1 = &xint1_isr;
	EDIS;

	// Set XINT Control Register
	XIntruptRegs.XINT1CR.bit.POLARITY = 1;
	
	// Enable External Interrupt
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;
	

	IER |= M_INT1;	// Enable CPU int12 which is connected to XINT1

	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;	// Enable XINT1 in the PIE: Group 1 interrupt 4
	
}
*/
//====����===2010.05.24, �̼���=====
void Init_External_Interrupt(void)
{
	// step1
	EALLOW;		
	PieVectTable.XINT1 = &xint1_isr;	// Mapping
	EDIS;
	// step2	
	// Enable Xint1 in the PIE: Group 1 interrupt 4 & 5
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;		// Enable XINT1 in the PIE: Group 1 interrupt 4
	// step3
	// �ٽü��� Init_Gpio(void)���� �����ؾ���, Ȥ�� ��������� ����Ͽ�  �ٽ� ���� 
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;	// GPIO�� ����
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 0;		// input
	EDIS;	
	
	// step4
	// Set GPIO Interrupt Select
	EALLOW; 
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 18;	// netname(GPIO00), DSP PIN(GPIO18, PIN62)
	EDIS;												// SEN(/AC_SYNC->IRQ2) -> DSP(IRQ2->GPIO00(GPIO18))

	// step5
	// Set XINT Control Register
	XIntruptRegs.XINT1CR.bit.POLARITY = 0; // Rising edge(1), Falling edge(0) interrupt
	
	// step6
	// Enable External Interrupt
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;	// Enable XINT1

	// step7	
	IER |= M_INT1;	// Enable CPU int12 which is connected to XINT1
	
}



void TimerInterruptEnable()
{
	
	// This function can be found in DSP2833x_CpuTimers.c
	// For this example, only initialize the Cpu Timers
	InitCpuTimers();
	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

/********************************************************************************/
/* ���� : ���� �ð����� ���� ��� �Լ�											*/
/* ���� :																		*/
/********************************************************************************/
Uint16	msec_dif(Uint16 msec1, Uint16 msec2)
{
	if (msec1 >= msec2)
	{
		return (msec1 - msec2);
	}
	else
	{
		//return (msec1 + (4294967295 - msec2));
		return (msec1 + (65535 - msec2));
	}
}

