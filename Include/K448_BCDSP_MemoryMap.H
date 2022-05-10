#ifndef NTHS_MEMORYMAP_H
#define NTHS_MEMORYMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 
/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000)

#define CS0_START_ADDR		0x004000
#define CS0_END_ADDR		0x004FFF
#define CS0_SIZE			0x001000

#define CS6_START_ADDR		0x100000
#define CS6_END_ADDR		0x1FFFFF
#define CS6_SIZE			0x100000

#define CS7_START_ADDR		0x200000
#define CS7_END_ADDR		0x2FFFFF
#define CS7_SIZE			0x100000

//========================================
// 메인 DSP 어드레스
//========================================


#define	TzInterruptClear()		EALLOW;\
								EPwm1Regs.TZCLR.bit.OST = 1;\
	 							EPwm1Regs.TZCLR.bit.INT = 1;\
	 							EDIS
	 							
#define	TzTripClearInverter()	EALLOW;\
								EPwm1Regs.TZCLR.bit.OST = 1;\
								EPwm2Regs.TZCLR.bit.OST = 1;\
								EPwm3Regs.TZCLR.bit.OST = 1;\
	 							EDIS

#define	TzTripSetInverter() 	EALLOW;\
								EPwm1Regs.TZFRC.bit.OST = 1;\
								EPwm2Regs.TZFRC.bit.OST = 1;\
								EPwm3Regs.TZFRC.bit.OST = 1;\
								EDIS
								
#define	TzTripClearConverter()	EALLOW;\
								EPwm4Regs.TZCLR.bit.OST = 1;\
								EPwm5Regs.TZCLR.bit.OST = 1;\
								EPwm6Regs.TZCLR.bit.OST = 1;\
	 							EDIS
	 							
#define	TzTripSetConverter() 	EALLOW;\
								EPwm4Regs.TZFRC.bit.OST = 1;\
								EPwm5Regs.TZFRC.bit.OST = 1;\
								EPwm6Regs.TZFRC.bit.OST = 1;\
								EDIS	
								
//================================================================
// PWM 관련 메크로
//================================================================
// PWM 인에이블
#define Pwm12LegEnable()		sbit(MainEpldData, BIT0_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm34LegEnable()		sbit(MainEpldData, BIT1_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm56LegEnable()		sbit(MainEpldData, BIT2_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm78LegEnable()		sbit(MainEpldData, BIT3_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm9LegEnable()			sbit(MainEpldData, BIT4_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
// PWM 디스에이블								
#define Pwm12LegDisable()		cbit(MainEpldData, BIT0_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm34LegDisable()		cbit(MainEpldData, BIT1_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm56LegDisable()		cbit(MainEpldData, BIT2_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm78LegDisable()		cbit(MainEpldData, BIT3_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff
#define Pwm9LegDisable()		cbit(MainEpldData, BIT4_POS);\
								WR_DSP_PWM_EN_CS() = MainEpldData & 0xff

#define PwmAllEnable()			Pwm12LegEnable();\
								Pwm34LegEnable();\
								Pwm56LegEnable();\
								Pwm78LegEnable();\
								Pwm9LegEnable()
								
#define PwmAllDisable()			Pwm12LegDisable();\
								Pwm34LegDisable();\
								Pwm56LegDisable();\
								Pwm78LegDisable();\
								Pwm9LegDisable()

//**********************************************************************//
// 차세대 재설정														//
//**********************************************************************//
//**********************************************************************//
// 메인 DSP 어드레스													//
//**********************************************************************//
// 입력
#define	CS_WR_VFD1()	PWORD(0x20E000)
#define	CS_WR_VFD2()	PWORD(0x20E001)
#define	CS_WR_VFD3()	PWORD(0x20E002)
#define	CS_WR_VFD4()	PWORD(0x20E003)

#define RD_DSP_FAULT_CS()		PWORD(0x20E008)
#define RD_DSP_UINXX_CS()	   	PWORD(0x20E00A)

// 출력
#define WR_DSP_PWM_EN_CS()	PWORD(0x20E004)
#define WR_DSP_FAULT_CLR_CS()	PWORD(0x20E005)
#define WR_DSP_SW_FAULT_CS()	PWORD(0x20E006) //Fault GeN
#define WR_DSP_XOUT_CS()	PWORD(0x20E007)//MONBD


#define FAULTCLR()				WR_DSP_FAULT_CLR_CS() = 0x00
#define FAULTGENSW()			WR_DSP_SW_FAULT_CS() = 0x00
//=======================================
// MON 보드 어드레스
//=======================================
// drpam<---2010.03.18 DPRAM프로그램 추가
#define DPRAM_BASEADDR              0x20A080

//=======================================
// DIO 보드 어드레스
//=======================================
// 입력
#define DIN_CS1()	PWORD(0x20D000)
#define DIN_CS2()	PWORD(0x20D001)
//#define DIN_CHK_CS1()		PWORD(0xD0A002)
//#define DIN_CHK_CS2()		PWORD(0xD0A003)
// 출력
#define DOUT_CS1()	PWORD(0x20D004)
#define DOUT_CS2()	PWORD(0x20D005)
#define DIO_LED_CS1()	PWORD(0x20D007)
#define DIO_LED_CS2()	PWORD(0x20D008)
#define DIO_LED_CS3()	PWORD(0x20D006)
#define DIO_LED_CS4()	PWORD(0x20D009)

// CPU 상태 LED
#define DSP_LED1_ON()			(GpioDataRegs.GPACLEAR.bit.GPIO12 = 1)	
#define DSP_LED1_OFF()			(GpioDataRegs.GPASET.bit.GPIO12 = 1)
#define DSP_LED1_TOGGLE()		(GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1)

//=========================================
// TEST 보드 어드레스
//=========================================
#define TEST_DA1_CS()		PWORD(0x20B000)
#define TEST_DA2_CS()		PWORD(0x20B001)
#define TEST_DA3_CS()		PWORD(0x20B002)
#define TEST_DA4_CS()		PWORD(0x20B003)
#define TEST_DA5_CS()		PWORD(0x20B004)
#define TEST_DA6_CS()		PWORD(0x20B005)
#define TEST_DA7_CS()		PWORD(0x20B006)
#define TEST_DA8_CS()		PWORD(0x20B007)
#define TEST_DA9_CS()		PWORD(0x20B008)
#define TEST_DA10_CS()		PWORD(0x20B009)
#define TEST_DA11_CS()		PWORD(0x20B00A)
#define TEST_DA12_CS()		PWORD(0x20B00B)
#define TEST_DA13_CS()		PWORD(0x20B00C)
#define TEST_DA14_CS()		PWORD(0x20B00D)
#define TEST_DA15_CS()		PWORD(0x20B00E)
#define TEST_DA16_CS()		PWORD(0x20B00F)


//====================================================
// APS/BACH 공용
// AC/DC CONVERTER EPWM 
//====================================================
// Enable
#define AcDcConPwmEnable()			Pwm56LegEnable()
// Disable
#define AcDcConPwmDisable() 		Pwm56LegDisable()
//====================================================
// APS/BACH DC/DC, SIV Inverter PWM 
//====================================================	
// Enable							
#define BachPwmEnable()    	Pwm12LegEnable();\
								Pwm34LegEnable()
// Disable								
#define BachPwmDisable()    	Pwm12LegDisable();\
								Pwm34LegDisable()
														


// DSP28335 DSK에서 외부 버스 영역 중 Zone0과  Zone7를 16비트 모드로 초기화하는 함수
// RealSYS DSP28335 DSK에서는 SCI-A와 CAN-A를 사용하므로 A17 ~ A19를 External Address 핀으로 사용하지 않음.
// 따라서 최대 쓸수 있는 영역 사이즈는 0x100000 -> 0x20000 임. 이점 유의하시길 바랍니다.
void Init_Ext_Bus(void);	// DSP ADDRESS/DATA BUS
 /* 데이터 변환 관련 함수(LCD 및 통신 프로토콜 처리에서 사용함 */
extern char asc2hex(char asc);  // ASCII 데이터를 HEX 데이터로 변환(debug_2833x.c 파일에 구현)
extern char hex2asc(char hex);  // HEX 데이터를 ASCII 데이터로 변환(debug_2833x.c 파일에 구현)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSK2833x_GLOBALFUNC_H definition
