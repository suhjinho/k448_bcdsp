//==================================================================================//
// ������ ���ö��(NTHS) ���͸� ������ġ(BACH) �� ����������ġ(APS) ��������		//
//==================================================================================//
// Main Program																		//
//==================================================================================//
// ProgramVersion                                                by PACTECH R&D LSH //
//----------------------------------------------------------------------------------//
// REV    | DATE          | ����                                                    //
//------------------------------------------------------------------------------------------------------//
// REV10  | 2012��1��19�� | �������� ��Ű����(2012��01��19��)-APS 4��, BACH 2�� ����� ��������			//
// REV11  | 2012��1��19�� | BACH HWF���� ����(EPLD/SEN����/����͸����α׷� Ȯ����				  		//
// REV12  | 2012��1��31�� | BACH �����������ѱ�� Ʃ���Ѱ� ����(TC/MC)-1/30        				  		//
//                        | ��ü ���α׷� ������(�ڵ�������� ������)           				  		//
// REV13  | 2012��2��01�� |  APS ���������Ʈ���� �ø�, DCLINK������Ʈ���� �ø�, APS/BACH�Է���������Ʈ��������(250V)	//
// REV14  | 2012��2��18�� |  APS/BACH�Է���������Ʈ��������(230V)	//
//======================================================================================================//

#define	GLOBAL__VARIABLE_FUNCTION_H
#include "Global_Variable_Function.h"

extern Uint16 msec_tmr;	

extern Uint16  xint1_cnt;
extern Uint16  xint1_cnt1;
extern Uint16  xint1_cnt2;
extern Uint16  xint1_cnt3;
// These are defined by the linker (see FLASH.cmd)
// �� ������ ���� �Լ��� �ʱ�ȭ ��ƾ���� "ramfuncs" �̶�� ���ǵ� �ڵ� ������
// ���� Flash���� RAM�������� �����ϱ� ���� MemCopy �Լ����� ����.
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

	// ProgramVersion = 10;//20120119_������_��Ű����
	// ���� ǥ�� 100 => 1.00
//	ProgramVersion = 110;//
//	ProgramVersion = 120;// REV12  | 2012��1��31�� | BACH �����������ѱ�� Ʃ���Ѱ� ����(TC/MC)-1/30//
//	ProgramVersion = 130;// REV13  | 2012��2��01�� | APS ���������Ʈ���� �ø�, DCLINK������Ʈ���� �ø�, APS/BACH�Է���������Ʈ��������(250V)
//	ProgramVersion = 140;// REV13  | 2012��2��18�� | APS/BACH�Է���������Ʈ��������(230V)
	ProgramVersion = 10;
//  ProgramVersion_PAC = ���� ���� Ȯ���� ���� ���� �߰�, FAULT ��������� ���� �ּ� ó�� 20210603 ���� DRK

	ProgramVersion_PAC = 2;	// 220317 �ܺμ��� �� ������� ���Ƽ� DEADTEAM 650->400, bcDutyLimit 0.9 -> 0.95, BCOCUC FAULT�� BCHSOT FAULT �߻� �ð� ��Ȯ�� ����.
							// 220506 ��������� CHK,LK ���۵��Ͽ� AC �Է����� �̻��, BC ������� REFERENCE 99.5V -> 98.0V, SPI��żӵ� ����,  
	// �ֺ���ġ �ʱ�ȭ
	System_Define();			// �ý��� ����
	PeripheralInit();			// DSP �ʱ�ȭ
	PwmAllDisable();			// PWM ��� �𽺿��̺�
	SensorBoard_Scale_Init();	// SEN CARD ADC �Ƴ��α� �����ϸ� �� ����
	FaultLevelSetting();		// ��Ʈ ���� ����
	TimerInterruptEnable(); 	// Ÿ�̸� ���ͷ�Ʈ �ο��̺�
	
	VariableInit();				// ��� ���� 0���� �ʱ�ȭ
	FaultClear();				// �������� �ϵ���� ��Ʈ, DSP IDU��Ʈ Ŭ����
	
	delay_ms(500);				// �����̺� ���� �ϵ���� ����ȭ �ð�
	
	DioSenCard_OutputInit();	// IO���� ���, LED SENSOR���� LED ��� ����	
	DIO_InputChk();
	DIO_InputChk();
	DIO_InputChk();	
	DIO_InputChk();
	DIO_InputChk();
	delay_ms(1000);				// �����̺� ���� �ϵ���� ����ȭ �ð�	
	msec_1000m = msec_500m=msec_200m = msec_10m = msec_1m = msec_tmr;	// ���� �ֱ� task�� ����� ���ؼ� ���� �ʱ�ȭ		

	EPwmInterruptEnable();		// ACDC PWM 5 ���ͷ�Ʈ
	FaultClear();				// �������� �ϵ���� ��Ʈ, DSP IDU��Ʈ Ŭ����
	ReloadCpuTimer1();  
	delay_ms(500);				// �����̺� ���� �ϵ���� ����ȭ �ð�

	MidiumFault_3min3N_Check_TEST_INIT();	// 3��3ȸ �߰��� ó���κ�
	while(MODEAck == BOARDTEST){
		
		if( 5000 <= msec_dif(msec_tmr, msec_1000m)){		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;			
			//WHM();	//���·� ��� -> �Һ������� �����ؼ� 1KW�϶� 1�ʰ� TDCS�� 'H'��������
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

//			DpramWrite();//20120208 ȸ�翡�� 10msec -> 1msec�� ������

		}
	}
	//=======main loop===========//		
	while(TRUE){

		//== 1.0 sec tesk ====
		if( 5000 <= msec_dif(msec_tmr, msec_1000m)){		// 1000mse = 200usec*5000
			msec_1000m = msec_tmr;			
			WHM();	//���·� ��� -> �Һ������� �����ؼ� 1KW�϶� 1�ʰ� TDCS�� 'H'��������
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
			Bach_Sequence(); // 20190415 ������ ���α׷� �ڵ� �߰�
			//	DpramWrite();
		}

		//== 1msec tesk ====		
		if( 5 <= msec_dif(msec_tmr, msec_1m)){				// 1ms = 200usec*5; 
			msec_1m = msec_tmr;

			//20190318 DAC 1ms ������ �߰�
		//	if(DAC_CONTROL){							//20210818 SUHJINHO DA ��� ��÷� ���� ����
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				DAC_OUT();
				//SpiaRegs.SPIFFTX.bit.TXFIFO=0;
		//	}										     //20210818 SUHJINHO DA ��� ��÷� ���� ����	
		//	else{							
		//		DAC_STOP();								//20210818 SUHJINHO DA ��� ��÷� ���� ����
		//	}
			//20190318 DAC 1ms ������ �߰�

			DpramWrite();

		}
		if( 10 <= msec_dif(msec_tmr, msec_1m)){				// 2ms = 200usec*5; 
			//DpramTestToFromTCMS();
			//msec_1m = msec_tmr;
			//DpramWrite();//20120208 ȸ�翡�� 10msec -> 1msec�� ������
		}
	}   
}
//================================================================//
//==�ý��� ���ݰ��� ����
//#device_Program = NTHS_APS_PROGRAM;
//#define NTHS_APS_PROGRAM		0
//#define NTHS_BACH_PROGRAM		1
//#define ETC_XXX_PROGRAM		5
//================================================================//
// !!!!���������� �ݵ��� Ȯ���Ұ�!!!!!!
//================================================================//
void System_Define()
{
	RatedACInputVoltage = 380;
	RatedDCInputVoltage = 900;
	RatedBCOutputVoltage = 100;
	

}



/////////////   K448 �ϻ꼱 BC   ////////////////////

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

	msec_500m = msec_200m = msec_10m = msec_1m = msec_tmr = 0;	// ���� �ֱ� task�� ����� ���ؼ� ���� �ʱ�ȭ
	//---DigitalInput.c/h ���� �ʱ�ȭ, �����ϰ� �Էº����� �ʱ�ȭ�� �ʿ����----//	
	DioInPutBuf = 0;
 	SenInPutBuf = 0;

 	DioOutBuf1 = 0;
 	DioLedBuf01_16 = 0;
 	DioLedBuf17_26 = 0;
 	SenLedBuf = 0;
	//=====sequence ���ú��� �ʱ�ȭ============//
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

	//=====APS/BACH FAULT ���� ����============//
	HighFaultMJF = 0;
 	FaultAck = 0;
 	FaultSeqBuf = 0;	
    FaultEtcBuf = 0;				//
	FaultRestartTimer = 0;
	FaultRestartTimerSet = 500;		// fault�� ��⵿ �ð� 10msec*500 = 5sec
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

	OverLoadAck = 0;	// �����ϸ� 1, ������ 1���̻� ���ӽ� ��Ʈó��
	OverLoad_Counter = 0;
	THRFAck_Counter = 0; // ��Ʈ��ũ 90���̻� 1��
	BCLKa_cnt = 0;
	BCCHKa_cnt = 0;
	PSF100Ack_cnt = 0;
	BCOCUC_FL_cnt = 0;

	Test_bcDutyRatio = 0;
	
	PSF_Counter = 0;	// PSF ������ ī���� �ʱ�ȭ(10msec*20 = 200msec)
	//=====APS Acknowledge ���� �����ʱ�ȭ============//
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
// code ����													//
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
