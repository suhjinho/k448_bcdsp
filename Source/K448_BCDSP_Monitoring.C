#include "Global_Variable_Function.h"

//======================================================//
// Dpram Read											//
// Dpram Read from moncard/tcms, 2010.04.13				//
//======================================================//
#define TCMSTESTMODE	1
#define TCMSNORMALMODE	0
Uint16  TCMSMode	= TCMSNORMALMODE;

Uint16  TCMSModeTestCounter = 0;

Uint16  DpramTCMSHeartBeat;
Uint16  DpramYear = 0;
Uint16  DpramMonth = 0;
Uint16  DpramDate = 0;
Uint16  DpramHour = 0;
Uint16  DpramMinute = 0;
Uint16  DpramSec = 0;
Uint16  DpramTCMStestRead = 0;

Uint16 TEST_BachPwmModulation = 0;
Uint16 TEST_InputACVolAck = 0;
Uint16 TEST_ReadyAck = 0;
Uint16 TEST_DINAck1_BCRUN = 0;
Uint16 TEST_FLFaultSWBuf = 0;
Uint16 TEST_FLFaultGDFBuf = 0;
Uint16 TEST_MajorFault = 0;
Uint16 TEST_FLFaultEtcBuf = 0;
Uint16 TEST_FLFaultSeqBuf = 0;
Uint16 TEST_DINAck1_BCLKa = 0;
Uint16 TEST_DINAck1_BCCHKa = 0;

// DpramTCMStestRead(DpramAddr = 0x88) BIT define
#define DpramTimeValid		0
#define DpramBIT1			1
#define DpramTSTART			2
#define DpramTESK			3
#define DpramBIT4			4
#define DpramTVF			5
//================== SIV1, From Moncard, from 차량
//BIT0	TimeValid1	0/1, 1=Date & Time Validation
//BIT1	*	
//BIT2	TSTART1	Test Start Request(시험시작요청)
//BIT3	TESK1	Test ESK 1(연장급전 시험요청, SIV1 off)
//BIT4	*	
//BIT5	TVF1	Test V/F Request(기본시험 v/f 실행요청)
//================== SIV2, From Moncard, from 차량
//TimeValid2	0/1, 1=Date & Time Validation
//*	
//TSTART2	Test Start Request(시험시작요청)
//TESK1	Test ESK 1(연장급전 시험요청, SIV2 off)
//*	
//TVF2	Test V/F Request(기본시험 v/f 실행요청)


//==================================================================//
// Dpram Write														//
// Dpram Write TO moncard/tcms, 2010.04.13							//
// Dpram Write TO moncard/tcms, 2010.06.30 lsh 대전에서 pc모니터링 수정							//
//==================================================================//
Uint16  DPRAM6_MON = 0;

Uint16  Tst_DpramDig0 = 0;
Uint16  Tst_DpramDig1 = 0;
Uint16  Tst_DpramDig2 = 0;
Uint16  Tst_DpramDig3 = 0;
Uint16  Tst_DpramDig4 = 0;
Uint16  Tst_DpramDig5 = 0;
Uint16  Tst_DpramDig6 = 0;
Uint16  Tst_DpramDig7 = 0;
Uint16	a12345 = 0;



float  Tst_DpramAna0 = 0.;
float  Tst_DpramAna1 = 0.;
float  Tst_DpramAna2 = 0.;
float  Tst_DpramAna3 = 0.;
float  Tst_DpramAna4 = 0.;
float  Tst_DpramAna5 = 0.;
float  Tst_DpramAna6 = 0.;
float  Tst_DpramAna7 = 0.;
float  Tst_DpramAna8 = 0.;
float  Tst_DpramAna9 = 0.;
float  Tst_DpramAna10 = 0.;
float  Tst_DpramAna11 = 0.;
float  Tst_DpramAna12 = 0.;
float  Tst_DpramAna13 = 0.;
float  Tst_DpramAna14 = 0.;
float  Tst_DpramAna15 = 0.;
float  Tst_DpramAna16 = 0.;
float  Tst_DpramAna17 = 0.;
float  Tst_DpramAna18 = 0.;
float  Tst_DpramAna19 = 0.;
float  Tst_DpramAna20 = 0.;
float  Tst_DpramAna21 = 0.;
float  Tst_DpramAna22 = 0.;
float  Tst_DpramAna23 = 0.;



//===Dpram Write===============
Uint16  DpramHeatBeat_APSBACH = 0;
Uint16  DpramDig0 = 0;
Uint16  DpramDig1 = 0;
Uint16  DpramDig2 = 0;
Uint16  DpramDig3 = 0;
Uint16  DpramDig4 = 0;
Uint16  DpramDig5 = 0;
Uint16  DpramDig6 = 0;
Uint16  DpramDig7 = 0;
Uint16  DpramDig8 = 0;
Uint16  DpramDig9 = 0;
Uint16  DpramDig10 = 0;
Uint16  DpramDig11 = 0;
Uint16  DpramDig12 = 0;
Uint16  DpramDig13 = 0;
Uint16  DpramDig14 = 0;
Uint16  DpramDig15 = 0;
Uint16  DpramDig16 = 0;

Uint16  TEST_DpramDig0 = 0;
Uint16  TEST_DpramDig1 = 0;
Uint16  TEST_DpramDig2 = 0;
Uint16  TEST_DpramDig3 = 0;
Uint16  TEST_DpramDig4 = 0;
Uint16  TEST_DpramDig5 = 0;
Uint16  TEST_DpramDig6 = 0;
Uint16  TEST_DpramDig7 = 0;
Uint16  TEST_DpramDig8 = 0;
Uint16  TEST_DpramDig9 = 0;
Uint16  TEST_DpramDig10 = 0;
Uint16  TEST_DpramDig11 = 0;
Uint16  TEST_DpramDig12 = 0;
Uint16  TEST_DpramDig13 = 0;
Uint16  TEST_DpramDig14 = 0;
Uint16  TEST_DpramDig15 = 0;
Uint16  TEST_DpramDig16 = 0;
Uint16  TEST_DpramDig17 = 0;
Uint16  TEST_DpramDig18 = 0;
Uint16  TEST_DpramDig19 = 0;
Uint16  TEST_DpramDig20 = 0;
Uint16  TEST_DpramDig21 = 0;
Uint16  TEST_DpramDig22 = 0;
Uint16  TEST_DpramDig23 = 0;
Uint16  TEST_DpramDig24 = 0;
Uint16  TEST_DpramDig25 = 0;
Uint16  TEST_DpramDig26 = 0;
Uint16  TEST_DpramDig27 = 0;
Uint16  TEST_DpramDig28 = 0;
Uint16  TEST_DpramDig29 = 0;
Uint16  TEST_DpramDig30 = 0;
Uint16  TEST_DpramDig31 = 0;
Uint16  TEST_DpramDig32 = 0;
Uint16  TEST_DpramDig33 = 0;
Uint16  TEST_DpramDig34 = 0;
Uint16  TEST_DpramDig35 = 0;

Uint16	TEST_DpramDig20_old = 0;
Uint16	TEST_DpramDig23_old = 0;
//===Dpram Write===============

Uint16  DpramAna0 = 0;
Uint16  DpramAna1 = 0;
Uint16  DpramAna2 = 0;
Uint16  DpramAna3 = 0;
Uint16  DpramAna4 = 0;
Uint16  DpramAna5 = 0;
Uint16  DpramAna6 = 0;
Uint16  DpramAna7 = 0;
Uint16  DpramAna8 = 0;
Uint16  DpramAna9 = 0;
Uint16  DpramAna10 = 0;
Uint16  DpramAna11 = 0;
Uint16  DpramAna12 = 0;
Uint16  DpramAna13 = 0;
Uint16  DpramAna14 = 0;
Uint16  DpramAna15 = 0;
Uint16  DpramAna16 = 0;
Uint16  DpramAna17 = 0;
Uint16  DpramAna18 = 0;
Uint16  DpramAna19 = 0;
Uint16  DpramAna20 = 0;
Uint16  DpramAna21 = 0;
Uint16  DpramAna22 = 0;
Uint16  DpramAna23 = 0;


Uint16  DpramDig0rd;
Uint16  DpramDig1rd;
Uint16  DpramDig2rd;
Uint16  DpramDig3rd;
Uint16  DpramDig4rd;
Uint16  DpramDig5rd;
Uint16  DpramDig6rd;
Uint16  DpramDig7rd;
Uint16  DpramDig8rd;
Uint16  DpramDig9rd;
Uint16  DpramDig10rd;
Uint16  DpramDig11rd;
Uint16  DpramDig12rd;
Uint16  DpramDig13rd;
Uint16  DpramDig14rd;
Uint16  DpramDig15rd;

Uint16  DpramAna0rd;
Uint16  DpramAna1rd;
Uint16  DpramAna2rd;
Uint16  DpramAna3rd;
Uint16  DpramAna4rd;
Uint16  DpramAna5rd;
Uint16  DpramAna6rd;
Uint16  DpramAna7rd;
Uint16  DpramAna8rd;
Uint16  DpramAna9rd;
Uint16  DpramAna10rd;
Uint16  DpramAna11rd;
Uint16  DpramAna12rd;
Uint16  DpramAna13rd;
Uint16  DpramAna14rd;
Uint16  DpramAna15rd;
Uint16  DpramAna16rd;
Uint16  DpramAna17rd;
Uint16  DpramAna18rd;
Uint16  DpramAna19rd;
Uint16  DpramAna20rd;
Uint16  DpramAna21rd;
Uint16  DpramAna22rd;
Uint16  DpramAna23rd;



//======================================================//
//	DSP->MON		DpramDig0		2010. 04.13							//
//======================================================//
//====== dig0 ==========================================
//TSTR	시험 시작 응답(Test Start Response)
//TFROK	Test Frequency Result OK(기본시험 F 응답)
//TFRNG	Test Frequency Result NG(기본시험 F 응답)
//TVROK	Test Voltage Result OK(기본시험 V 응답)
//TVRNG	Test Voltage Result NG(기본시험 V 응답)
//TVFING	Test V/F on-going(기본시험 진행중)
//TVFEND	Test V/F End(기본시험 종료)
#define DpramTSTR	0
#define DpramTFROK	1
#define DpramTFRNG	2
#define DpramTVROK	3
#define DpramTVRNG	4
#define DpramTVFING	5
#define DpramTVFEND	6

//BIT0 : TEST ING
//BIT1 : TEST END
//BIT2 : SPARE
//BIT3 : TEST ERROR
//BIT3 : TEST
//BIT4 : NORMAL
//BIT5 : TEST OK
//BIT6 : TEST FAIL
//BIT7 : TESTING



//======================================================
unsigned int WHM1KWHAck;

unsigned int ApsPprAck;
//======================================================
extern Uint16 msec_tmr;	

//DpramHeatBeat_HFC = 35;

//==================================================================================//
// 함수명 : DpramWriteDataRemake()     			                                    //
// 기  능 : TCMS 통신데이타 DPRAM WRITE                                             //
// DATE  : 2011. 02. 28 									                         //
//==================================================================================//
void DpramWriteDataRemake()
{
//축전지 충전기는 정상시는 정상상태 신호 및 차단상태 신호는 High로 출력하고, 차단상태일 경우는 정상상태 신호 및 차단상태 신호는 Low로 출력한다.
//	int i;

//	float tmp;
	//====> APSDOWN  APS DOWN지령(HVAC 차단신호)
	//---- DIG1 status --------------//
	if(0)	sbit(DpramDig1, 0);
	else	cbit(DpramDig1, 0);	
	if(btst(DioInPutBuf,DIN1_BCRESET))	sbit(DpramDig1, 1);
	else 	cbit(DpramDig1, 1); 
	if(btst(DioOutBuf1,DOUT_BCOK))	sbit(DpramDig1, 2);	
	else	cbit(DpramDig1, 2);
	if(BachPwmModulation == YES)	sbit(DpramDig1, 3);	
	else	cbit(DpramDig1, 3);
	//if(InputACVolAck == YES)	sbit(DpramDig1, 5);	
	if((InputACVolAck == YES) || (InputDCVolAck == YES))	sbit(DpramDig1, 4);
	else	cbit(DpramDig1, 4);
	if(ReadyAck == YES)	sbit(DpramDig1, 5);	
	else	cbit(DpramDig1, 5);
	if(MODEAck == TESTMODE)	sbit(DpramDig1, 6);	
	else	cbit(DpramDig1, 6);
	if(DINAck1_BCRUN == YES)	sbit(DpramDig1, 7);	
	else	cbit(DpramDig1, 7);
	/*
	SWF_BCIFCOV	0
#define		SWF_BCIFCUV	1
#define   SWF_BCOVOV	2
#define   SWF_BCOVUV	3

#define   SWF_BCIFCOC	4	
#define   SWF_BCOCOC	5	
#define   SWF_BCOCUC	6	
	*/
//---- DIG2 status --------------//
	if(0)	sbit(DpramDig2, 0);	
	else	cbit(DpramDig2, 0);	
	if(0)	sbit(DpramDig2, 1);	
	else	cbit(DpramDig2, 1);
	if(0)	sbit(DpramDig2, 2);	
	else	cbit(DpramDig2, 2);
	if(0)	sbit(DpramDig2, 3);	
	else	cbit(DpramDig2, 3);
	if(DINAck1_BCLKa == YES)	sbit(DpramDig2, 4);
	else	cbit(DpramDig2, 4);
	if(btst(DioOutBuf1,DOUT_BCLK))	sbit(DpramDig2, 5);
	else	cbit(DpramDig2, 5);	
	if(DINAck1_BCCHKa == YES)	sbit(DpramDig2, 6);	
	else	cbit(DpramDig2, 6);
	if(btst(DioOutBuf1,DOUT_BCCHK))	sbit(DpramDig2, 7);	
	else	cbit(DpramDig2, 7);
	
//---- DIG3 status --------------//
	if(btst(FLFaultSWBuf,SWF_BCOVUV))	sbit(DpramDig3, 0);	//
	else	cbit(DpramDig3, 0);	
	if(btst(FLFaultSWBuf,SWF_BCOVOV))	sbit(DpramDig3, 1);	//
	else	cbit(DpramDig3, 1);
	if(btst(FLFaultSWBuf,SWF_BCIFCUV))	sbit(DpramDig3, 2);	
	else	cbit(DpramDig3, 2);
	if(btst(FLFaultSWBuf,SWF_BCIFCOV))	sbit(DpramDig3, 3);	
	else	cbit(DpramDig3, 3);
	if(btst(FaultSWBuf,SWF_AFCUV))	sbit(DpramDig3, 4); //BCINUV
	else	cbit(DpramDig3, 4);	
	if(btst(FaultSWBuf,SWF_BCACIOV))	sbit(DpramDig3, 5); //BCINOV
	else	cbit(DpramDig3, 5);	
	if(btst(FLFaultGDFBuf,GDF2))	sbit(DpramDig3, 6);	
	else	cbit(DpramDig3, 6);
	if(btst(FLFaultGDFBuf,GDF1))	sbit(DpramDig3, 7);	
	else	cbit(DpramDig3, 7);
	
	//---- DIG4 status --------------//
	if(0)	sbit(DpramDig4, 0);	//
	else	cbit(DpramDig4, 0);	
	if(btst(FLFaultEtcBuf, ETCERR_CPSF))	sbit(DpramDig4, 1);	//
	else	cbit(DpramDig4, 1);
	if(btst(FLFaultSeqBuf, SEQERR_LKa))	sbit(DpramDig4, 2);	
	else	cbit(DpramDig4, 2);
	if(btst(FLFaultSeqBuf, SEQERR_CHKa))	sbit(DpramDig4, 3);	
	else	cbit(DpramDig4, 3);
	if(btst(FLFaultEtcBuf, ETCERR_HSOT))	sbit(DpramDig4, 4);
	else	cbit(DpramDig4, 4);	
	if(btst(FLFaultSWBuf,SWF_BCOCUC))	sbit(DpramDig4, 5);
	else	cbit(DpramDig4, 5);	
	if(btst(FLFaultSWBuf,SWF_BCOCOC))	sbit(DpramDig4, 6);	
	else	cbit(DpramDig4, 6);
	if(btst(FLFaultSWBuf,SWF_BCIFCOC))	sbit(DpramDig4, 7);	
	else	cbit(DpramDig4, 7);
/*
	//---- DIG2 status --------------//
	if(0)	sbit(DpramDig2, 0);	
	else	cbit(DpramDig2, 0);	
	if(btst(FLFaultEtcBuf, ETCERR_CPSF))	sbit(DpramDig2, 1);	
	else	cbit(DpramDig2, 1);
	if(btst(FLFaultSeqBuf, SEQERR_LKa))	sbit(DpramDig2, 2);	
	else	cbit(DpramDig2, 2);
	if(btst(FLFaultSeqBuf, SEQERR_CHKa))	sbit(DpramDig2, 3);	
	else	cbit(DpramDig2, 3);
	if(btst(FLFaultEtcBuf, ETCERR_HSOT))	sbit(DpramDig2, 4);
	else	cbit(DpramDig2, 4);
	if(btst(FLFaultSWBuf,SWF_BCIFCOV))	sbit(DpramDig2, 5);
	else	cbit(DpramDig2, 5);	
	if(btst(FLFaultGDFBuf,GDF2))	sbit(DpramDig2, 6);	
	else	cbit(DpramDig2, 6);
	if(btst(FLFaultGDFBuf,GDF1))	sbit(DpramDig2, 7);	
	else	cbit(DpramDig2, 7);

	if(btst(FLFaultSWBuf,SWF_AFCUV))	sbit(DpramDig3, 0);	//SWF_AFCUV
	else	cbit(DpramDig3, 0);	
	if(btst(FLFaultSWBuf,SWF_BCACIOV))	sbit(DpramDig3, 1);	//SWF_BCACIOV
	else	cbit(DpramDig3, 1);
	if((MajorFault == YES) ||(btst(FLFaultEtcBuf, ETCERR_MJF)) )	sbit(DpramDig3, 2);	
	else	cbit(DpramDig3, 2);
	if(btst(FLFaultSeqBuf, SEQERR_LKa))	sbit(DpramDig3, 3);	
	else	cbit(DpramDig3, 3);
	if(btst(FLFaultSeqBuf, SEQERR_CHKa))	sbit(DpramDig3, 4);
	else	cbit(DpramDig3, 4);	
	if(btst(FLFaultEtcBuf, ETCERR_CPSF))	sbit(DpramDig3, 5);
	else	cbit(DpramDig3, 5);	
	if(btst(FLFaultEtcBuf, ETCERR_HSOT))	sbit(DpramDig3, 6);	
	else	cbit(DpramDig3, 6);
	if(btst(FLFaultSWBuf,SWF_BCOCUC))	sbit(DpramDig3, 7);	
	else	cbit(DpramDig3, 7);

*/


	if(0)	sbit(DpramDig5, 0);	//ACCT1F = ACIC, AC LINE 입력전류, 주회로(ACCT)
	else	cbit(DpramDig5, 0);	
	if(0)	sbit(DpramDig5, 1);	//ACCT2F = DCIC,DCLINK 과전류,  주회로(DCCT2)
	else	cbit(DpramDig5, 1);	
	if(0)	sbit(DpramDig5, 2);	//DCCT4F = DCLINK OVERVOLTAGE H/W 폴트추가함
	else	cbit(DpramDig5, 2);	
	if(0)	sbit(DpramDig5, 3);	//ACCT3F = 출력 과전류,  주회로(DCCT2)
	else	cbit(DpramDig5, 3);	
	if(0)	sbit(DpramDig5, 4);	//ACCT4F = BAT충전과전류,  주회로(DCCT3)
	else	cbit(DpramDig5, 4);
	if(0)	sbit(DpramDig5, 5);//적산전력표시(APS만, BACH는 스페어)
	else	cbit(DpramDig5, 5);			//25 
	if(0)	sbit(DpramDig5, 6);	//스텍과온상태상태일때...
	else	cbit(DpramDig5, 6);
	if((MajorFault == YES) ||(btst(FLFaultEtcBuf, ETCERR_MJF)))	sbit(DpramDig5, 7);	//OVERLOAD상태일때...
	else	cbit(DpramDig5, 7);

	DpramDig6 = 0x00;
	DpramDig7 = 0x00;
	DpramDig8 = 0x00;
	DpramDig9 = 0x00;
		//---- DIG10 status --------------//
	if(1)		sbit(DpramDig10, 0);	//CPUOK DSP 부팅했으면 1, 무조건 1
	else	cbit(DpramDig10, 0);	
	if(btst(FLFaultEtcBuf, ETCERR_CPSF))		cbit(DpramDig10, 1);	//제어전원 고장 없으면 1
	else	sbit(DpramDig10, 1);	
	if(SWF_ADC == 0)		sbit(DpramDig10, 2);	//기동전 후 ADC전압 전류 이상없으면 1
	else	cbit(DpramDig10, 2);	
	if(btst(FLFaultSeqBuf, SEQERR_CHKa)||btst(FLFaultSeqBuf, SEQERR_LKa))		cbit(DpramDig10, 3);	// 기동전 후  I/O 이상없으면 1
	else	sbit(DpramDig10, 3);	
	if((BachPwmModulation == YES)&&(SEQ_Step > SEQ_BACH_CON_ON))	sbit(DpramDig10, 4);	// 쵸퍼 컨버터 PWM 후 쵸퍼 출력 이상ㅇ없으면 1
	else	cbit(DpramDig10, 4);
	if(0)		sbit(DpramDig10, 5);	//
	else	cbit(DpramDig10, 5);	
	if(0)		sbit(DpramDig10, 6);	// 
	else	cbit(DpramDig10, 6);
	if(btst(DioOutBuf1,DOUT_BCOK))		sbit(DpramDig10, 7);	// 배터리 장치 전체 정상 동작 BCOK 동작하고 폴트 없으면 1
	else	cbit(DpramDig10, 7);


//---SIV ANALOG Trace data ----------------//
	DpramWriteAnalogData();

}		


//==================================================================================//
// 함수명 : DpramWrite()                                                            //
// 기  능 : TCMS 통신데이타 셋업                                                    //
/// DATE  : 2010. 07. 01 (4월달에 대전에 프로그램한것 수정해서)                     //
//  monitor card irq인식시간 : 555usec												//
//  monitor card model : 중국심천 모니터 카드										//
//  epld source        : ??															//
//==================================================================================//
	
void DpramWrite()
{
	DpramHeatBeat_APSBACH++;
	DpramWriteDataRemake();

    PWORD(DPRAM_BASEADDR+0) = DpramHeatBeat_APSBACH & 0xff;		
    PWORD(DPRAM_BASEADDR+1) = (DpramHeatBeat_APSBACH>>8) & 0xFF;			
    PWORD(DPRAM_BASEADDR+2) = DpramAna1 & 0xFF;         
    PWORD(DPRAM_BASEADDR+3) = DpramAna0 & 0xFF;			
    PWORD(DPRAM_BASEADDR+4) = DpramAna3 & 0xFF;         
    PWORD(DPRAM_BASEADDR+5) = DpramAna2 & 0xFF;         
	PWORD(DPRAM_BASEADDR+6) = DpramAna5 & 0xFF; 								// Spare
    PWORD(DPRAM_BASEADDR+7) = DpramAna4 & 0xFF;
    PWORD(DPRAM_BASEADDR+8) = DpramAna7 & 0xFF;
	PWORD(DPRAM_BASEADDR+9) = DpramAna6 & 0xFF;

//	PWORD(DPRAM_BASEADDR+10) = 0x00 & 0xFF; 							// Spare
//	PWORD(DPRAM_BASEADDR+11) = 0x00 & 0xFF; 							// Spare 
//	PWORD(DPRAM_BASEADDR+12) = 0x00 & 0xFF;								// Spare
//	PWORD(DPRAM_BASEADDR+13) = 0x00 & 0xFF;								// Spare
	PWORD(DPRAM_BASEADDR+14) = DpramDig10 & 0xFF;								// 로템 테스트용 
//	PWORD(DPRAM_BASEADDR+15) = 0x00 & 0xFF; 							// Spare
//	PWORD(DPRAM_BASEADDR+16) = 0x00 & 0xFF; 							// Spare
//	PWORD(DPRAM_BASEADDR+17) = 0x00 & 0xFF; 							// Spare
//	PWORD(DPRAM_BASEADDR+18) = 0x00 & 0xFF; 							// Spare
//	PWORD(DPRAM_BASEADDR+19) = 0x00 & 0xFF; 							// Spare
    PWORD(DPRAM_BASEADDR+20) =DpramDig2 & 0xFF;
    PWORD(DPRAM_BASEADDR+21) =DpramDig1 & 0xFF;          
    PWORD(DPRAM_BASEADDR+22) =DpramDig4 & 0xFF;
    PWORD(DPRAM_BASEADDR+23) =DpramDig3 & 0xFF;
//	PWORD(DPRAM_BASEADDR+24) = 0x00 & 0xFF; 							// Spare
    PWORD(DPRAM_BASEADDR+25) = DpramDig5 & 0xFF;

    PWORD(DPRAM_BASEADDR+31) = ProgramVersion & 0xFF;         

	sbit(WR_DSP_XOUT_CS_buf,1);						
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;
 	delay_us(3);
	cbit(WR_DSP_XOUT_CS_buf,1);						
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;

	if(FaultCounter != FaultCounter_old)
	{
	
 		delay_us(50);						
		sbit(WR_DSP_XOUT_CS_buf,0);						
		WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;
 		delay_us(3);//3
		cbit(WR_DSP_XOUT_CS_buf,0);						
		WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;
		// 20190323 비트 연산후 딜레이 추가
	}
	

    FaultCounter_old = FaultCounter;


/* //20190322 상태데이터 신호를 인터럽트 구문으로 이동
	if(FaultCounter != FaultCounter_old){
		MonCardIrqGen(0);	// 0 : int0 fault write	
	}
//	 if(FaultCounter == FaultCounter_old){
 		//MonCardIrqGen(1);	// 1 : int1 Normal write 
 		MonCardIrqGen(1);	// 1 : int1 Normal write 
//	}
//	else{;}

    FaultCounter_old = FaultCounter;
*/
    DpramRead();    
	//20190322 상태데이터 신호를 인터럽트 구문으로 이동
    
}
//==============================================//
// IRQ(0) : 상태데이타 Write
// IRQ(1) : 폴트데이타 Write
//=============================================//
void MonCardIrqGen(unsigned int irqN)
{
	// 20190322 데이터 쓰기위한 코드 추가 수정
//    cbit(WR_DSP_XOUT_CS(),irqN);   // INT0, in Monitor Card, Fault data wirte
//    sbit(WR_DSP_XOUT_CS(),irqN);
//	cbit(WR_DSP_XOUT_CS(),irqN);
	// 20190322 데이터 쓰기위한 코드 추가 수정/

	// 20190322 데이터 쓰기위한 코드 추가 수정
//  cbit(WR_DSP_XOUT_CS_buf,irqN);						
//	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;

	sbit(WR_DSP_XOUT_CS_buf,irqN);					
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;
	delay_us(1);							// 20190322 비트 연산후 딜레이 추가
	cbit(WR_DSP_XOUT_CS_buf,irqN);						
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf;
	// 20190322 데이터 쓰기위한 코드 수정
	
}

void DpramTestToFromTCMS()
{

	DpramTCMStestRead = PWORD(DPRAM_BASEADDR+0x88)& 0xff; 
	
	if(btst(DpramTCMStestRead,DpramTSTART)){
		TCMSMode = TCMSTESTMODE;
	}
	if(TCMSMode == TCMSTESTMODE){
		sbit(DpramDig0, DpramTSTR);			//TSTR	시험 시작 응답(Test Start Response)
		sbit(DpramDig0, DpramTVFING);			//TVFING	Test V/F on-going(기본시험 진행중)
		TCMSModeTestCounter++;				// 10msec*500 = 5,0000msec = 5sec
		
		if(TCMSModeTestCounter>2000){//400==1sec about
			cbit(DpramDig0, DpramTVFING);			//TVFING	Test V/F on-going(기본시험 진행)-->종료
			sbit(DpramDig0, DpramTVFEND);		//TVFEND	Test V/F End(기본시험 종료)
			sbit(DpramDig0, DpramTFROK);				//TFROK	Test Frequency Result OK(기본시험 F 응답)	
			sbit(DpramDig0, DpramTVROK);				//TVROK	Test Voltage Result OK(기본시험 V 응답)		
		}
		if(TCMSModeTestCounter>6000){	// 테스트종료, 초기화//2400
			cbit(DpramDig0, DpramTSTR);			//TSTR	시험 시작 응답(Test Start Response)-->종료
			cbit(DpramDig0, DpramTVFEND);		//TVFEND	Test V/F End(기본시험 종료)
			cbit(DpramDig0, DpramTFROK);				//TFROK	Test Frequency Result OK(기본시험 F 응답)	
			cbit(DpramDig0, DpramTVROK);				//TVROK	Test Voltage Result OK(기본시험 V 응답)		
			TCMSModeTestCounter = 0;
			TCMSMode = TCMSNORMALMODE;					// TCMSMODE = NORMAL MODE
		}
	
	}

}

void DpramRead()
{
	Uint16  tmp;
//-------------------------------------------	
	tmp = ((PWORD(DPRAM_BASEADDR+0x80) & 0xff)<<8);       
	DpramTCMSHeartBeat = (PWORD(DPRAM_BASEADDR+0x81) & 0xff) | tmp; 

	DpramYear = PWORD(DPRAM_BASEADDR+0x82)& 0xff;       
	DpramMonth = PWORD(DPRAM_BASEADDR+0x83)& 0xff;   
	DpramDate = PWORD(DPRAM_BASEADDR+0x84)& 0xff;       
	DpramHour = PWORD(DPRAM_BASEADDR+0x85)& 0xff;   
	DpramMinute = PWORD(DPRAM_BASEADDR+0x86)& 0xff;       
	DpramSec = PWORD(DPRAM_BASEADDR+0x87)& 0xff;   
	
	DpramTCMStestRead = PWORD(DPRAM_BASEADDR+0x88)& 0xff;   
//-----------------------
	DpramDig0rd = PWORD(DPRAM_BASEADDR+3)& 0xff;// 
	DpramDig1rd = PWORD(DPRAM_BASEADDR+4)& 0xff;// 
	DpramDig2rd = PWORD(DPRAM_BASEADDR+5)& 0xff;// 
	DpramDig3rd = PWORD(DPRAM_BASEADDR+6)& 0xff;// 
	DpramDig4rd = PWORD(DPRAM_BASEADDR+7)& 0xff;// 
	DpramDig5rd = PWORD(DPRAM_BASEADDR+8)& 0xff;// 
	DpramDig6rd = PWORD(DPRAM_BASEADDR+9)& 0xff;// 
	DpramDig7rd = PWORD(DPRAM_BASEADDR+10)& 0xff;// 
	DpramDig8rd = PWORD(DPRAM_BASEADDR+11)& 0xff;// 
	DpramDig9rd = PWORD(DPRAM_BASEADDR+12)& 0xff;// 
	DpramDig10rd = PWORD(DPRAM_BASEADDR+13)& 0xff;// 
	DpramDig11rd = PWORD(DPRAM_BASEADDR+14)& 0xff;// 
	DpramDig12rd = PWORD(DPRAM_BASEADDR+15)& 0xff;// 
	
	DpramAna0rd = PWORD(DPRAM_BASEADDR+0x10)& 0xff;// 
	DpramAna1rd = PWORD(DPRAM_BASEADDR+0x11)& 0xff;// 
	DpramAna2rd = PWORD(DPRAM_BASEADDR+0x12)& 0xff;// 
	DpramAna3rd = PWORD(DPRAM_BASEADDR+0x13)& 0xff;// 
	DpramAna4rd = PWORD(DPRAM_BASEADDR+0x14)& 0xff;// 
	DpramAna5rd = PWORD(DPRAM_BASEADDR+0x15)& 0xff;// 
	DpramAna6rd = PWORD(DPRAM_BASEADDR+0x16)& 0xff;// 
	DpramAna7rd = PWORD(DPRAM_BASEADDR+0x17)& 0xff;// 
	DpramAna8rd = PWORD(DPRAM_BASEADDR+0x18)& 0xff;// 
	DpramAna9rd = PWORD(DPRAM_BASEADDR+0x19)& 0xff;// 
	DpramAna10rd = PWORD(DPRAM_BASEADDR+0x1a)& 0xff;// 
	DpramAna11rd = PWORD(DPRAM_BASEADDR+0x1b)& 0xff;// 
	DpramAna12rd = PWORD(DPRAM_BASEADDR+0x1c)& 0xff;// 
	DpramAna13rd = PWORD(DPRAM_BASEADDR+0x1d)& 0xff;// 
	DpramAna14rd = PWORD(DPRAM_BASEADDR+0x1e)& 0xff;// 
	DpramAna15rd = PWORD(DPRAM_BASEADDR+0x1f)& 0xff;// 

}

//=======================================================//
//  소비전력 1KWH일때마다 TCMS로 HIGH를 1초간 지속시킴  //
//  1초마다 메인루프에서 실행함							//
//=======================================================//
//======================================================//
//	Dpram Data make	2010. 06.29							//
//======================================================//
void WHM()
{
	float Watt, Watt_sec;
	
	Watt = fabs(DCInputVol_LPF) * fabs(InputCur_LPF) / 1000.;
	Watt_sec = Watt/3600.0;
	WHM1KW += Watt_sec;

}
//==================================================================================//
// 함수명 : DpramWriteAnalogData()                                         //
// 기  능 : 상태 모니터링                                                          //
// DATE  : 2010. 10. 11 (센서카드 데이타 모니터링)                                //
// 센서카드에서 읽은것 직접 표시                                                    //
//==================================================================================//
void DpramWriteAnalogData()
{ 
	if(MODEAck != BOARDTEST){
		
		DpramAna0 = (Uint16)(AcInputVolUVWLLRms/4.);
		DpramAna1 = (Uint16)(DCInputVol_LPF/8.);	
		DpramAna2 = (Uint16)(InputCur_LPF/2.);	
		DpramAna3 = (Uint16)(BachOutVol_LPF);	
		DpramAna4 = (Uint16)(BachOutCur_LPF/2.);
		// 20200401
		DpramAna5 = (Uint16)(ControlVol_LPF);	
		DpramAna6 = ((Uint16)(WHM1KW) >> 8) & 0xff;					// WHM_Low, Number of Trace Data(TD) in TCMS, This value was sent TCMS from the MONitoring board(MON)
		DpramAna7 = (Uint16)(WHM1KW) & 0xff;
		// 20200401

//		DpramAna6 = (Uint16)(0);	
//		DpramAna7 = (Uint16)(0);	
		DpramAna8 = (Uint16)(0); 
		DpramAna9 = (Uint16)(0); 
		
		DpramAna11 = (Uint16)(0);			
		DpramAna12 = (Uint16)(0);
		DpramAna13 = (Uint16)(0);
		DpramAna14 = (Uint16)(0);	
		DpramAna15 = (Uint16)(0);
		DpramAna16 = (Uint16)(0);
		DpramAna17 = (Uint16)(0);
		
		DpramAna13 = (Uint16)(0);	
		DpramAna14 = (Uint16)(0);	
	
		DpramAna18 = (Uint16)(0);	
		DpramAna19 = (Uint16)(0);	
		DpramAna20 = (Uint16)(0);	
		DpramAna21 = (Uint16)(0);	
		DpramAna22 = (Uint16)(0);	
		DpramAna23 = (Uint16)(0);	
	}
	else if(MODEAck == BOARDTEST){
		DpramAna0 = (Uint16)(4/4);	
		DpramAna1 = (Uint16)(8/8);	
		DpramAna2 = (Uint16)(12/2);	
		DpramAna3 = (Uint16)(16/1);	
		DpramAna4 = (Uint16)(20/2);	
		DpramAna5 = (Uint16)(24/1);	
		DpramAna6 = (Uint16)(0);	
		DpramAna7 = (Uint16)(0);	
		DpramAna8 = (Uint16)(0); 
		DpramAna9 = (Uint16)(0); 
		
		DpramAna11 = (Uint16)(0);			
		DpramAna12 = (Uint16)(0);
		DpramAna13 = (Uint16)(0);
		DpramAna14 = (Uint16)(0);	
		DpramAna15 = (Uint16)(0);
		DpramAna16 = (Uint16)(0);
		DpramAna17 = (Uint16)(0);
		
		DpramAna13 = (Uint16)(0);	
		DpramAna14 = (Uint16)(0);	
	
		DpramAna18 = (Uint16)(0);	
		DpramAna19 = (Uint16)(0);	
		DpramAna20 = (Uint16)(0);	
		DpramAna21 = (Uint16)(0);	
		DpramAna22 = (Uint16)(0);	
		DpramAna23 = (Uint16)(0);
	}
}

