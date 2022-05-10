//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// Main Program																		//
//==================================================================================//
// ProgramVersion                                                by PACTECH R&D LSH //
//----------------------------------------------------------------------------------//
// REV    | DATE          | 내용                                                    //
//------------------------------------------------------------------------------------------------------//
// REV10  | 2012년1월19일 | 시제차량 터키데모(2012년01년19일)-APS 4대, BACH 2대 살려서 부하투입			//
// REV11  | 2012년1월19일 | BACH HWF순서 정정(EPLD/SEN도면/모니터링프로그램 확인후				  		//
// REV12  | 2012년1월31일 | BACH 충전전류제한기능 튜닝한것 적용(TC/MC)-1/30        				  		//
//                        | 전체 프로그램 정리함(코드수정없이 정리함)           				  		//
// REV13  | 2012년2월01일 |  APS 출력전류폴트레벨 올림, DCLINK전류폴트레벨 올림, APS/BACH입력저전압폴트레벨낮춤(250V)	//
// REV14  | 2012년2월18일 |  APS/BACH입력저전압폴트레벨낮춤(230V)	//
//======================================================================================================//

#define	GLOBAL__VARIABLE_FUNCTION_H
#include "Global_Variable_Function.h"

extern Uint16 msec_tmr;	

extern Uint16  xint1_cnt;
extern Uint16  xint1_cnt1;
extern Uint16  xint1_cnt2;
extern Uint16  xint1_cnt3;
// These are defined by the linker (see FLASH.cmd)
// 이 변수는 메인 함수의 초기화 루틴에서 "ramfuncs" 이라고 정의된 코드 섹션을
// 내부 Flash에서 RAM영역으로 복사하기 위한 MemCopy 함수에서 사용됨.
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

Uint16 msec_1000m = 0,msec_500m = 0,msec_200m = 0, msec_10m = 0, msec_100m = 0;
Uint32 msec_1m = 0;

void BoardTest(unsigned char boardname);// test card lsh

Uint16 TEST_Pin = 0x00;
Uint16 TEST_DPRAM0 = 0x00;
Uint16 TEST_DPRAM1 = 0x00;
Uint16 TEST_DPRAM2 = 0x00;
Uint16 TEST_DPRAM3 = 0x00;

Uint16 TEST_DPRAM_RD0 = 0x00;
Uint16 TEST_DPRAM_RD1 = 0x00;
Uint16 TEST_DPRAM_RD2 = 0x00;
Uint16 TEST_DPRAM_RD3 = 0x00;

Uint16 TEST_DPRAM_RD4 = 0x00;
Uint16 TEST_DPRAM_RD5 = 0x00;
Uint16 TEST_DPRAM_RD6 = 0x00;
Uint16 TEST_DPRAM_RD7 = 0x00;

Uint16 TEST_DPRAM_RD8 = 0x00;
Uint16 TEST_DPRAM_RD9 = 0x00;
Uint16 TEST_DPRAM_RD10 = 0x00;
Uint16 TEST_DPRAM_RD11 = 0x00;

Uint16 TEST_DPRAM_RD12 = 0x00;
Uint16 TEST_DPRAM_RD13 = 0x00;
Uint16 TEST_DPRAM_RD14 = 0x00;
Uint16 TEST_DPRAM_RD15 = 0x00;

Uint16 TEST_DPRAM_RD16 = 0x00;
Uint16 TEST_DPRAM_RD17 = 0x00;
Uint16 TEST_DPRAM_RD18 = 0x00;
Uint16 TEST_DPRAM_RD19 = 0x00;

Uint16 TEST_DPRAM_RD20 = 0x00;
Uint16 TEST_DPRAM_RD21 = 0x00;
Uint16 TEST_DPRAM_RD22 = 0x00;
Uint16 TEST_DPRAM_RD23 = 0x00;

Uint16 TEST_DPRAM_RD24 = 0x00;
Uint16 TEST_DPRAM_RD25 = 0x00;
Uint16 TEST_DPRAM_RD26 = 0x00;
Uint16 TEST_DPRAM_RD27 = 0x00;

Uint16 TEST_DPRAM_RD28 = 0x00;
Uint16 TEST_DPRAM_RD29 = 0x00;
Uint16 TEST_DPRAM_RD30 = 0x00;
Uint16 TEST_DPRAM_RD31 = 0x00;

Uint16 TEST_DPRAM_RD32 = 0x00;
Uint16 TEST_DPRAM_RD33 = 0x00;
Uint16 TEST_DPRAM_RD34 = 0x00;


Uint16 sec_cnt =0;
Uint16 msec_cnt = 0;



//==================================================================================//
// Main()		    																
//==================================================================================//
void main(void){

	// ProgramVersion = 10;//20120119_시제차_터키데모
	// 버전 표기 100 => 1.00
//	ProgramVersion = 110;//
//	ProgramVersion = 120;// REV12  | 2012년1월31일 | BACH 충전전류제한기능 튜닝한것 적용(TC/MC)-1/30//
//	ProgramVersion = 130;// REV13  | 2012년2월01일 | APS 출력전류폴트레벨 올림, DCLINK전류폴트레벨 올림, APS/BACH입력저전압폴트레벨낮춤(250V)
//	ProgramVersion = 140;// REV13  | 2012년2월18일 | APS/BACH입력저전압폴트레벨낮춤(230V)
	ProgramVersion = 10;
//  ProgramVersion_PAC = 내부 버전 확인을 위한 변수 추가, FAULT 출력저전류 구문 주석 처리 20210603 수정 DRK

	ProgramVersion_PAC = 2;	// 220317 외부수전 시 출력전압 낮아서 DEADTEAM 650->400, bcDutyLimit 0.9 -> 0.95, BCOCUC FAULT와 BCHSOT FAULT 발생 시간 정확히 맞춤.
							// 220506 연장급전시 CHK,LK 미작동하여 AC 입력전압 미사용, BC 출력전압 REFERENCE 99.5V -> 98.0V, SPI통신속도 감소,  
	// 주변장치 초기화
	System_Define();			// 시스템 정격
	PeripheralInit();			// DSP 초기화
	PwmAllDisable();			// PWM 출력 디스에이블
	SensorBoard_Scale_Init();	// SEN CARD ADC 아날로그 스케일링 값 설정
	FaultLevelSetting();		// 폴트 레벨 설정
	TimerInterruptEnable(); 	// 타이머 인터럽트 인에이블
	
	VariableInit();				// 모든 변수 0으로 초기화
	FaultClear();				// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	
	delay_ms(500);				// 슬레이브 보드 하드웨어 안정화 시간
	
	DioSenCard_OutputInit();	// IO보드 출력, LED SENSOR보드 LED 모두 끄기	
	DIO_InputChk();
	DIO_InputChk();
	DIO_InputChk();	
	DIO_InputChk();
	DIO_InputChk();
	delay_ms(1000);				// 슬레이브 보드 하드웨어 안정화 시간	
	msec_1000m = msec_500m=msec_200m = msec_10m = msec_1m = msec_tmr;	// 일정 주기 task를 만들기 위해서 변수 초기화		

	EPwmInterruptEnable();		// ACDC PWM 5 인터럽트
	FaultClear();				// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	ReloadCpuTimer1();  
	delay_ms(500);				// 슬레이브 보드 하드웨어 안정화 시간

	MidiumFault_3min3N_Check_TEST_INIT();	// 3분3회 중고장 처리부분
	while(MODEAck == BOARDTEST){
		
		if( 5000 <= msec_dif(msec_tmr, msec_1000m)){		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;			
			//WHM();	//전력량 계산 -> 소비전력을 누적해서 1KW일때 1초간 TDCS에 'H'상태유지
			VFD_CONTROL(); //ok
			FaultClear(); //ok
			sec_cnt++;

			if(TEST_DioLedBuf01_16 == 0x8000){
				TEST_DioLedBuf01_16 = 0x0001;
			}
			else{
				TEST_DioLedBuf01_16 = (TEST_DioLedBuf01_16 << 1);
			}
			if(TEST_DioLedBuf17_26 == 0x0080){
				TEST_DioLedBuf17_26 = 0x0001;
			}
			else{
				TEST_DioLedBuf17_26 = (TEST_DioLedBuf17_26 << 1);
			}
			if(TEST_DioOutBuf1 == 0x80){
				TEST_DioOutBuf1 = 0x01;
			}
			else{
				TEST_DioOutBuf1 = (TEST_DioOutBuf1 << 1);
			}			
		}
		
		//== 0.5sec tesk ====
		if( 2500 <= msec_dif(msec_tmr, msec_500m)){			// 500mse = 200usec*2500
			DSP_LED1_TOGGLE();
			msec_cnt++;
			msec_500m = msec_tmr;
		}
		//== 10msec tesk ====
		if( 50 <= msec_dif(msec_tmr, msec_10m)){			// 10ms = 200usec*50; 
			msec_10m = msec_tmr;
			//Bach_Sequence();
		//	DpramWrite();
			//DIO_LED_CS1() = (TEST_DioLedBuf01_16 & 0xFFFF); //ok
			//DIO_LED_CS2() = (TEST_DioLedBuf17_26 & 0xFFFF); //ok
			//DOUT_CS1() = TEST_DioOutBuf1; //ok
			//BachPwmModulation = YES; //ok
			Bach_Sequence(); //ok
		}
		//== 1msec tesk ====		
		if( 5 <= msec_dif(msec_tmr, msec_1m)){				// 1ms = 200usec*5; 
			msec_1m = msec_tmr;

//			DpramWrite();//20120208 회사에서 10msec -> 1msec로 변경함

		}
	}
	//=======main loop===========//		
	while(TRUE){

		//== 1.0 sec tesk ====
		if( 5000 <= msec_dif(msec_tmr, msec_1000m)){		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;			
			WHM();	//전력량 계산 -> 소비전력을 누적해서 1KW일때 1초간 TDCS에 'H'상태유지
			VFD_CONTROL();
		}
		
		//== 0.5sec tesk ====
		if( 2500 <= msec_dif(msec_tmr, msec_500m)){			// 500mse = 200usec*2500
			DSP_LED1_TOGGLE(); //Heart beat
			msec_500m = msec_tmr;
		}

		//== 100msec tesk ====
		if( 500 <= msec_dif(msec_tmr, msec_100m)){			// 10ms = 200usec*50; 
			msec_100m = msec_tmr;		
			
		}
		//== 10msec tesk ====
		if( 50 <= msec_dif(msec_tmr, msec_10m)){			// 10ms = 200usec*50; 
			msec_10m = msec_tmr;
			Bach_Sequence(); // 20190415 스퀀스 프로그램 코드 추가
			//	DpramWrite();
		}

		//== 1msec tesk ====		
		if( 5 <= msec_dif(msec_tmr, msec_1m)){				// 1ms = 200usec*5; 
			msec_1m = msec_tmr;

			//20190318 DAC 1ms 루프에 추가
		//	if(DAC_CONTROL){							//20210818 SUHJINHO DA 출력 상시로 변경 위해
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				//SpiaRegs.SPIFFTX.bit.TXFIFO=0;
		//	}										     //20210818 SUHJINHO DA 출력 상시로 변경 위해	
		//	else{							
		//		DAC_STOP();								//20210818 SUHJINHO DA 출력 상시로 변경 위해
		//	}
			//20190318 DAC 1ms 루프에 추가

			DpramWrite();

		}
		if( 10 <= msec_dif(msec_tmr, msec_1m)){				// 2ms = 200usec*5; 
			//DpramTestToFromTCMS();
			//msec_1m = msec_tmr;
			//DpramWrite();//20120208 회사에서 10msec -> 1msec로 변경함
		}
	}   
}
//================================================================//
//==시스템 정격관련 정의
//#device_Program = NTHS_APS_PROGRAM;
//#define NTHS_APS_PROGRAM		0
//#define NTHS_BACH_PROGRAM		1
//#define ETC_XXX_PROGRAM		5
//================================================================//
// !!!!컴파일전에 반듯이 확인할것!!!!!!
//================================================================//
void System_Define()
{
	RatedACInputVoltage = 380;
	RatedDCInputVoltage = 900;
	RatedBCOutputVoltage = 100;
	

}



/////////////   K448 일산선 BC   ////////////////////

void VariableInit()
{
	WR_DSP_XOUT_CS_buf = 0;  // 20190322 DpramWrite() 
	cbit(WR_DSP_XOUT_CS_buf,0);	// 20190322 DpramWrite()
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf; // 20190322 DpramWrite()
	delay_us(1);				
	cbit(WR_DSP_XOUT_CS_buf,1); // 20190322 DpramWrite()
	WR_DSP_XOUT_CS() = WR_DSP_XOUT_CS_buf; // 20190322 DpramWrite()

	MainEpldData = 0;	// epwm enalbe/disable
	Sen1EpldData = 0;	// 

	msec_500m = msec_200m = msec_10m = msec_1m = msec_tmr = 0;	// 일정 주기 task를 만들기 위해서 변수 초기화
	//---DigitalInput.c/h 변수 초기화, 엄밀하게 입력변수는 초기화할 필요없슴----//	
	DioInPutBuf = 0;
 	SenInPutBuf = 0;

 	DioOutBuf1 = 0;
 	DioLedBuf01_16 = 0;
 	DioLedBuf17_26 = 0;
 	SenLedBuf = 0;
	//=====sequence 관련변수 초기화============//
	Sys_Status = 0;	// Init = SYS_INIT_STATE(0)
	SEQ_Step = 0;	// Init = SEQ_DO_NOTHING_STATE(0)
	SeqTimer = 0;
	SeqTimerSet = 0;
	SeqTimer_flag = 0;
	StartMode = SEQ_NOMODE;
	InputACVolAck = 0;
	InputACVolAck_cnt = 0;
	InputACVolAck_buf = 0;
	InputACVolAck_old = 2;
	InputDCVolAck = 0;
	InputDCVolAck_cnt = 0;
	InputDCVolAck_buf = 0;
	InputDCVolAck_old = 2;	
	StartMode_old = 1;
	StartMode_buf = 1;
	Mode_Change_cnt = 0;

	//=====APS/BACH FAULT 관련 변수============//
	HighFaultMJF = 0;
 	FaultAck = 0;
 	FaultSeqBuf = 0;	
    FaultEtcBuf = 0;				//
	FaultRestartTimer = 0;
	FaultRestartTimerSet = 500;		// fault후 재기동 시간 10msec*500 = 5sec
	FaultRestartTimer_flag = FLAGSET;
	
	FaultCheckCounter = 0;
	FaultGDFBuf = 0;
	FaultHWBuf = 0;
	FaultSWBuf = 0;
	FaultCounter = 0;
	MajorFault = 0;
	FanCheckCounter = 0;

	FaultCounter = 0;
	FaultCounter_old = 0;

	OverLoadAck = 0;	// 과부하명 1, 과부하 1분이상 지속시 폴트처리
	OverLoad_Counter = 0;
	THRFAck_Counter = 0; // 히트싱크 90도이상 1분
	BCLKa_cnt = 0;
	BCCHKa_cnt = 0;
	PSF100Ack_cnt = 0;
	BCOCUC_FL_cnt = 0;

	Test_bcDutyRatio = 0;
	
	PSF_Counter = 0;	// PSF 딜레이 카운터 초기화(10msec*20 = 200msec)
	//=====APS Acknowledge 관련 변수초기화============//
	//-DIN_CS1()
 	

	TEST_DEADTIME_DCDC = 0;
  	DEADTIME1_RED = 600;    //Set rising  edge delay 2020.12.16 BC test 598 fix noise low 
  	DEADTIME1_FED = 600;   //
	DEADTIME2_RED = 600;
	DEADTIME2_FED = 600;
 	
	//----------DIN_CS2()
	BachPwmModulation = 0;
	InitBachDcDcControlVar();
	InitSensorVar();
	InitTestVariable();
	
	PI_AntiWindupVar = 0.;
}    

//======lsh test function=======================================//
// code 검증													//
//==============================================================//

//=========lsh test==========================//

//=========lsh test==========================//
void BoardTest(unsigned char boardname)
{
	if(boardname == 0){
		DIO_LED_CS1()= 0xff;
		DIO_LED_CS2()= 0xff;
		DIO_LED_CS3()= 0xff;
		DIO_LED_CS4()= 0xff;
			
		DOUT_CS1() = 0xff;
		DOUT_CS2() = 0xff;		
	}
	else{
		DIO_LED_CS1()= 0x00;
		DIO_LED_CS2()= 0x00;
		DIO_LED_CS3()= 0x00;
		DIO_LED_CS4()= 0x00;
		DOUT_CS1() = 0x00;
		DOUT_CS2() = 0x00;					
	}
}
