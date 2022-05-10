
#ifndef __EXTERNAL_INPUT_OUTPUT_H
#define __EXTERNAL_INPUT_OUTPUT_H


//====NTHS APS/BACH DIO, LED BIT DEFINE=====//

//------------------------------------------------------//
// EXTERNAL INPUT  FROM 차량 및 장비내부				//
//------------------------------------------------------//
	//--APS/BACH --DIN_CS1(),DINEN1(U2 )
#define TESTMODE	0
#define NORMALMODE	1
#define BOARDTEST	2
	
	
var Uint16 	DioInPutBuf;
var Uint16 	DioInPutBuf1;
var Uint16 	DioInPutBuf1_1st;
var Uint16 	DioInPutBuf1_2nd;
var Uint16 	DioInPutBuf1_3rd;

var Uint16 TEST_DioOutBuf1;
var Uint16 TEST_DioLedBuf01_16;
var Uint16 TEST_DioLedBuf17_26;



	//--DIN_CS1()
#define DIN1_BCCHKa		0	//BCCHKa
#define DIN1_BCLKa		1	//BCLKa
#define DIN1_BCTEMP		2	//BCTEMP
#define DIN1_BCTEST		3	//BCTEST

#define DIN1_BCRUN		4	//BCRUN
#define DIN1_BCRESET	5	//BCRESET
#define DIN1_BCMJFa		6	//BCMJFa
#define DIN1_SPARE7		7	

	//DIN_CS2()
#define DIN2_SPARE0		8
#define DIN2_SPARE1		9	
#define DIN2_SPARE2		10	
#define DIN2_SPARE3		11	

#define DIN2_SPARE4		12	
#define DIN2_SPARE5		14	
#define DIN2_SPARE6		15	
#define DIN2_SPARE7		16	
#define DIN2_SPARE8		17	

	//--DIN_CS2()
var Uint16 	DioInPutBuf2;
var Uint16 	DioInPutBuf2_1st;
var Uint16 	DioInPutBuf2_2nd;
var Uint16 	DioInPutBuf2Ack;

	//--DIN_CS1() ACK
var Uint16 	DINAck1_BCCHKa;
var Uint16 	DINAck1_BCLKa;
var Uint16 	DINAck1_BCTEMP;
var Uint16 	DINAck1_BCTEST;
var Uint16 	DINAck1_BCRUN;
var Uint16 	DINAck1_BCRESET;
var Uint16 	DINAck1_BCRESET_OLD;
var Uint16 	DINAck1_BCMJFa;
var Uint16 	DINAck1_SPARE7;

	//--DIN_CS2() ACK
var Uint16 	DINAck2_SPARE0;
var Uint16 	DINAck2_SPARE1;
var Uint16 	DINAck2_SPARE2;
var Uint16 	DINAck2_SPARE3;
var Uint16 	DINAck2_SPARE4;
var Uint16 	DINAck2_SPARE5;
var Uint16 	DINAck2_SPARE6;
var Uint16 	DINAck2_SPARE7;
//------------------------------------------------------//
// EXTERNAL OUTPUT  TO 차량 및 장비내부					//
//------------------------------------------------------//
//--APS/BACH --DioOutBuf1,DOUT_CS1(),DOUTEN1(U33 )
var Uint16 	DioOutBuf1;
#define DOUT_BCMJF	0
#define DOUT_BCOK		1
#define DOUT_BCCHK	2
#define DOUT_BCLK		3
#define	DOUT_SPQRE4	4
#define	DOUT_SPQRE5	5
#define	DOUT_SPQRE6	6
#define	DOUT_SPQRE7	7

//------------------------------------------------------//
// DIO CARD LED											//
//------------------------------------------------------//
	//--APS/BACH --DIO_LED_CS1(),EPLD(U24), (U26 ) - LED 1-8
	//--APS/BACH --DIO_LED_CS2(),EPLD(U24), (U25 ) - LED 9-12
//20110613 전면판 LED수정,20120115 전면판정의 APS/BACH묷음
var Uint16 	DioLedBuf01_16;	// 주로 상태표시 // DioLedBuf1+DioLedBuf2
#define DLED_RESET	0	
#define DLED_RUN	1	
#define DLED_BCCHKx	2	
#define DLED_BCCHKa	3
#define DLED_BCLKx	4	
#define DLED_BCLKa	5	
#define DLED_TEST	6	
#define DLED_BCOK	7	
#define DLED_GBCVY	8
#define DLED_GBCUX	9
#define DLED_BCMJF	10
#define DLED_BCHSOT	11
#define DLED_GBCVYF	12
#define DLED_GBCUXF	13
#define DLED_BCIFCUV	14
#define DLED_BCIFCOV	15
//#define APS_DLED_PPRF		7	//PPRF, 상충돌 폴트검지..
//===================================
var unsigned int	DioLedBuf17_26;	
#define DLED_BCOVUV		0	
#define DLED_BCOVOV		1	
#define DLED_BCOCOC		2	
#define DLED_BCIFCOC	3	
#define DLED_BCCPSF		4	
#define DLED_BCOCUC		5	
#define DLED_BCLKF		6	
#define DLED_BCCHKF		7	
//------------------------------------------------------//
// SEN CARD LED											//
//------------------------------------------------------//
	//--APS/BACH --SEN1_LED_CS1(), (U22 )
	//--APS/BACH --SEN1_LED_CS2(), (U23 )
var Uint16 	SenLedBuf;
#define SLED_DCLINK_OCF		0	//DCLINK 과전류 폴트
#define SLED_ACI_OCF		1	//AC입력 과전류 폴트
#define SLED_SPR_BATOCF		2	//SPARE(APS), BAT충전과전류(BACH)
#define SLED_OCOCF			3	//인버터 출력 UVW상 과전류 폴트/출력과전류
#define SLED_OVLD			4	//오버로드
#define SLED_PSF			5	//제어전원 폴트(L)
#define SLED_ACIV_LV		6	//입력저전압
#define SLED_ACIV_OV		7	//입력과전압
#define SLED_DCV_LV			8	//DCLINK저전압
#define SLED_DCV_OV			9	//DCLINK과전압
#define SLED_OV_LV			10	//출력저전압
#define SLED_OV_OV			11	//출력과전압
#define SLED_LD1			13	//SPARE
#define SLED_LD2			12	//SPARE
#define SLED_LD3			15	//SPARE
#define SLED_LD4			14	//SPARE

	//--APS/BACH --SEN1_VOL_CHK_CS(), (EPLD,U13 )
var Uint16 	SenInPutBuf;
var Uint16 	SenInPutBuf_1st;
var Uint16 	SenInPutBuf_2nd;
var Uint16 	PSFAck;

var	Uint16	SW_VFDSW;
var	Uint16	SW_VFDSW_1st;
var	Uint16	SW_VFDSW_2nd;
var	Uint16	SW_VFDSW_3rd;

var Uint16	SW_HEXSW;
var Uint16	SW_HEXSW_1st;
var Uint16	SW_HEXSW_2nd;
var Uint16	SW_HEXSW_3rd;


#define PS24V_Chk				0	//제어전원 /24V_OK, ACTIVE(L)
#define PS5V_Chk				1	//제어전원 /5V_OK, ACTIVE(L)
#define PS15V_Chk				2	//제어전원 /15V_OK, ACTIVE(L)
#define PS100V_Chk				3	//제어전원 /100V_OK, ACTIVE(L)

var char	DioVfdBuf1;
var char	DioVfdBuf2;
var char	DioVfdBuf3;
var char	DioVfdBuf4;
var Uint16	VFD_mode;
var Uint16	SW_VFDSW_cnt;
var Uint16	DAC_CONTROL;



//APS_DIO_Process
void DioSenCard_OutputInit();//nths꺼..aps/bach공용
void DIO_InputChk();
void DIO_OutPutProcess();
void VFD_CONTROL();
void VFD_data();

#endif
