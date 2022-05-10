//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// DIO card Program																	//
// 자기부상열차 보조전원장치용 프로그램 수정 사용함									//
// SensorCardModel : MdBc SENSOR BOARD-A1											//
// DIOCardModel : DIO BOARD BOARD-A1												//
// LED  : Active high	(H->LED ON, L-> LED OFF										//
// DOUT : Active high	(카드내장 릴레이 동작)										//
// DIN  : Active high																//
//==================================================================================//


#include "Global_Variable_Function.h"
float	DINAck1_BCRUN_init = 0.;
float	DINAck1_BCRUN_LPF = 0.;
float	DINAck1_BCRUN_LPF1 = 0.;


float	DINAck1_BCRESET_init = 0.;
float	DINAck1_BCRESET_LPF = 0.;
float	DINAck1_BCRESET_LPF1 = 0.;
//==================================================================================//
// 함수명 : DioSenCard_OutputInit()													//
// 기  능 : DIO/SEN CARD 출력/led 초기화											//
// DATE  : 2010. 05. 13																//
//==================================================================================//


void DioSenCard_OutputInit()
{

	DioOutBuf1 = 0x00;
	VFD_mode = 0;
	DioLedBuf01_16 = 0x00;
	DioLedBuf17_26 = 0x00;	
	TEST_DioOutBuf1 = 0x0001;
	TEST_DioLedBuf01_16 = 0x0001;
	TEST_DioLedBuf17_26 = 0x0001;
	SenLedBuf = 0x00;

	// 전면 현시 LED 클리어
	DSP_LED1_OFF();
			
	DOUT_CS1() = DioOutBuf1;// ALL DOUT OFF
	
	DIO_LED_CS1() = ((DioLedBuf01_16 >> 0) & 0xFF);// ALL LED OFF
	DIO_LED_CS2() = ((DioLedBuf01_16 >> 8) & 0xFF);// ALL LED OFF
	DIO_LED_CS3() = ((DioLedBuf17_26 >> 0) & 0xFF);// ALL LED OFF
	DIO_LED_CS4() = ((DioLedBuf17_26 >> 8) & 0xFF);// ALL LED OFF

}

//==================================================================================//
// 함수명 : DIO_InputChk()		            										//
// 기  능 : DIN체크하고(채터링 3회) 관련 ACK신호 생성								//
// Calling : main()루틴에서 일정시간마다 실행함(약 10msec)							//
// DATE(수정)  : 2010. 08. 13														//
//==================================================================================//
void DIO_InputChk()
{
	unsigned int buf, buf1, buf2, buf3, buf4;
	
	//==================================
	// VFD SW, 채터링 방지(3회)
	//==================================
	
	buf1  = GpioDataRegs.GPADAT.bit.GPIO20;
	buf2 = GpioDataRegs.GPADAT.bit.GPIO21;
	buf = (buf2 << 1) | buf1;
	buf = (~buf) & (0x03);

	if((buf == SW_VFDSW_1st)&&(buf == SW_VFDSW_2nd)&&(buf == SW_VFDSW_3rd)){
		SW_VFDSW = buf;
	}
	SW_VFDSW_3rd = SW_VFDSW_2nd;
	SW_VFDSW_2nd = SW_VFDSW_1st;
	SW_VFDSW_1st = buf;
	
	//==================================================
	// HEX SW, 채터링 방지(3회)
	//==================================================
	buf  = GpioDataRegs.GPBDAT.bit.GPIO60;
	buf1 = GpioDataRegs.GPBDAT.bit.GPIO61;
	buf2 = GpioDataRegs.GPBDAT.bit.GPIO62;
	buf3 = GpioDataRegs.GPBDAT.bit.GPIO63;
	
	buf4 = (buf3<<3)|(buf2<<2)|(buf1<<1)|buf;
	buf4 = (buf4 ^ 0x0f) & 0x0f;
	SW_HEXSW_1st = buf4;
	if((buf4 == SW_HEXSW_1st)&&(buf4 == SW_HEXSW_2nd)&&(buf4 == SW_HEXSW_3rd)){
		SW_HEXSW = buf4;
	}
	SW_HEXSW_3rd = SW_HEXSW_2nd;
	SW_HEXSW_2nd = SW_HEXSW_1st;
	SW_HEXSW_1st = buf;
	if(SW_HEXSW == 10){
		MODEAck = BOARDTEST;
	}
	//==================================================
	// DIO 보드 입력, 채터링 방지(3회)
	//==================================================
	buf1 = DIN_CS1()& 0x01FF;
	//buf2 = DIN_CS2()& 0xFF;// 0x1F 에서는 OPFa, MJF를 입력 받지 않음

	//buf = ((buf2<<8)& 0xFF00)| buf1;
	buf = (~buf1) & 0x01FF;
	DioInPutBuf2_1st = buf;
	if((buf == DioInPutBuf1_1st)&&(buf == DioInPutBuf1_2nd)&&(buf == DioInPutBuf1_3rd)){
		DioInPutBuf = buf;
	}
	DioInPutBuf1_3rd = DioInPutBuf1_2nd;
	DioInPutBuf1_2nd = DioInPutBuf1_1st;
	DioInPutBuf1_1st = buf;
		
		DioInPutBuf = buf;
	



		//----------DIN_CS1()--------------------------------//
	if(btst(DioInPutBuf,DIN1_BCCHKa))  DINAck1_BCCHKa = YES;	
	else                               DINAck1_BCCHKa = NO;
	if(btst(DioInPutBuf,DIN1_BCLKa))  DINAck1_BCLKa = YES;	
	else                               DINAck1_BCLKa = NO;
	if(btst(DioInPutBuf,DIN1_BCTEMP))  DINAck1_BCTEMP = YES;	
	else                               DINAck1_BCTEMP = NO;
	if(btst(DioInPutBuf,DIN1_BCTEST)) DINAck1_BCTEST = NO;	
	else                               DINAck1_BCTEST = YES;
//------------- AC 외부전원 입력 시 RUN신호 깜박임 문제로 LPF 추가 20210308 dcy 	
//------------- LPF1(float unit_time,float pole,float input,float *output)
	if(btst(DioInPutBuf,DIN1_BCRUN)) DINAck1_BCRUN_init = 100.;	
	else                              DINAck1_BCRUN_init = 0.;

	LPF1(0.001, 100, DINAck1_BCRUN_init, &DINAck1_BCRUN_LPF);
	
	
	if(DINAck1_BCRUN_LPF> 30.)
	{
		DINAck1_BCRUN = 1;
	}
	else
	{
		DINAck1_BCRUN = 0;
	}

if(btst(DioInPutBuf,DIN1_BCRESET)) DINAck1_BCRESET_init = 100.;	
	else                              DINAck1_BCRESET_init = 0.;

	LPF1(0.001, 100, DINAck1_BCRESET_init, &DINAck1_BCRESET_LPF);
	
	
	if(DINAck1_BCRESET_LPF> 30.)
	{
		DINAck1_BCRESET = 1;
	}
	else
	{
		DINAck1_BCRESET = 0;
	}		

/*
	if(btst(DioInPutBuf,DIN1_BCRUN)) DINAck1_BCRUN = YES;	
	else                              DINAck1_BCRUN = NO;
	if(btst(DioInPutBuf,DIN1_BCRESET)) DINAck1_BCRESET = YES;	
	else                              DINAck1_BCRESET = NO;
*/


	

//------------------------ AC 외부전원 입력 시 RUN신호 깜박임 문제로 LPF 추가 20210308 dcy 	
	if(btst(DioInPutBuf,DIN1_BCMJFa)) DINAck1_BCMJFa = YES;	
	else                              DINAck1_BCMJFa = NO;
	if(btst(DioInPutBuf,DIN1_SPARE7)) DINAck1_SPARE7 = YES;	
	else                              DINAck1_SPARE7 = NO;
		
	//----------DIN_CS2()-----------------------------------//
	if(btst(DioInPutBuf,DIN2_SPARE0)) DINAck2_SPARE0 = YES;	
	else                              DINAck2_SPARE0 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE1)) DINAck2_SPARE1 = YES;	
	else                              DINAck2_SPARE1 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE2)) DINAck2_SPARE2 = YES;	
	else                              DINAck2_SPARE2 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE3)) DINAck2_SPARE3 = YES;	
	else                              DINAck2_SPARE3 = NO;
			
	if(btst(DioInPutBuf,DIN2_SPARE4)) DINAck2_SPARE4 = YES;	
	else                              DINAck2_SPARE4 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE5)) DINAck2_SPARE5 = YES;
	else                              DINAck2_SPARE5 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE6)) DINAck2_SPARE6 = YES;	
	else                              DINAck2_SPARE6 = NO;
	if(btst(DioInPutBuf,DIN2_SPARE7)) DINAck2_SPARE7 = YES;	
	else                              DINAck2_SPARE7 = NO;
	
	//-------------------------------------------------
	if(SW_HEXSW != 10)
	{
		if(DINAck1_BCTEST == YES)
		{
			MODEAck = TESTMODE;
		}
		else
		{
			MODEAck = NORMALMODE;
		}
	}
	else 
	{
		MODEAck = BOARDTEST;
	}
}


//==========================================================================//
// 함수명 : DIO_OutPutProcess()												//
// 기  능 : DIO CARD, SEN CARD Digital output(릴레이) 및 LED ON/OFF			//
// Calling : main()루틴에서 일정시간마다 실행함(약 10msec)					//
// DATE  : 2010. 08. 7														//
//==========================================================================//

void DIO_OutPutProcess()
{
	/*
	#define		SWF_BCIFCOV	0
	#define		SWF_BCIFCUV	1
	#define   SWF_BCOVOV	2
	#define   SWF_BCOVUV	3
	
	#define   SWF_BCIFCOC	4	
	#define   SWF_BCOCOC	5	
	#define   SWF_BCOCUC	6	
	*/
	if(DINAck1_BCRESET == YES)     sbit(DioLedBuf01_16, DLED_RESET);		//#define APS_DLED_APSOK	0	//장비동작전 점검 정상동작중
	else      cbit(DioLedBuf01_16, DLED_RESET);
	if(DINAck1_BCRUN == YES)     sbit(DioLedBuf01_16, DLED_RUN);	//#define APS_DLED_MODE		1	//NOMAL(LED ON)/TEST(LED OFF) MODE
	else      cbit(DioLedBuf01_16, DLED_RUN);		//MODEAck = NOMAL(L),TEST(H) MODE
	if(btst(DioOutBuf1,DOUT_BCCHK))     sbit(DioLedBuf01_16, DLED_BCCHKx);				//#define APS_DLED_RUN		2	//RUN 지령오면 ON
	else      cbit(DioLedBuf01_16, DLED_BCCHKx);
	if(DINAck1_BCCHKa == YES)     sbit(DioLedBuf01_16, DLED_BCCHKa);	//#define APS_DLED_ACIVOK	3	//AC가선전압(380V임,입력전압)
	else      cbit(DioLedBuf01_16, DLED_BCCHKa);
	if(btst(DioOutBuf1,DOUT_BCLK))     sbit(DioLedBuf01_16, DLED_BCLKx);				//#define APS_DLED_LKA		4	// Line Contactor led on	
	else      cbit(DioLedBuf01_16, DLED_BCLKx);
	if(DINAck1_BCLKa == YES)     sbit(DioLedBuf01_16, DLED_BCLKa);				
	else      cbit(DioLedBuf01_16, DLED_BCLKa);
	if(MODEAck == TESTMODE)     sbit(DioLedBuf01_16, DLED_TEST);	
	else      cbit(DioLedBuf01_16, DLED_TEST);
	if(btst(DioOutBuf1,DOUT_BCOK))			sbit(DioLedBuf01_16, DLED_BCOK);	
	else      cbit(DioLedBuf01_16, DLED_BCOK);
	
	if(BachPwmModulation == YES)     sbit(DioLedBuf01_16, DLED_GBCVY);	
	else      cbit(DioLedBuf01_16, DLED_GBCVY);
	if(BachPwmModulation == YES)			sbit(DioLedBuf01_16, DLED_GBCUX);	
	else      cbit(DioLedBuf01_16, DLED_GBCUX);
	if(btst(DioOutBuf1,DOUT_BCMJF))     sbit(DioLedBuf01_16, DLED_BCMJF);							
	else      cbit(DioLedBuf01_16, DLED_BCMJF);
	if(btst(FLFaultEtcBuf, ETCERR_HSOT))    	sbit(DioLedBuf01_16, DLED_BCHSOT);					
	else      cbit(DioLedBuf01_16, DLED_BCHSOT);
	if(btst(FLFaultGDFBuf,GDF2))    	sbit(DioLedBuf01_16, DLED_GBCVYF);					
	else      cbit(DioLedBuf01_16, DLED_GBCVYF);
	if(btst(FLFaultGDFBuf,GDF1))    	sbit(DioLedBuf01_16, DLED_GBCUXF);					
	else      cbit(DioLedBuf01_16, DLED_GBCUXF);
	if(btst(FLFaultSWBuf,SWF_BCIFCUV) || btst(FLFaultSWBuf,SWF_AFCUV))    	sbit(DioLedBuf01_16, DLED_BCIFCUV);					
	else      cbit(DioLedBuf01_16, DLED_BCIFCUV);
	if(btst(FLFaultSWBuf,SWF_BCIFCOV) || (btst(FLFaultSWBuf,SWF_BCACIOV)))    	sbit(DioLedBuf01_16, DLED_BCIFCOV);					
	else      cbit(DioLedBuf01_16, DLED_BCIFCOV);

				
	if(btst(FLFaultSWBuf,SWF_BCOVUV))     sbit(DioLedBuf17_26, DLED_BCOVUV);		//#define APS_DLED_APSOK	0	//장비동작전 점검 정상동작중
	else      cbit(DioLedBuf17_26, DLED_BCOVUV);
	if(btst(FLFaultSWBuf,SWF_BCOVOV))     sbit(DioLedBuf17_26, DLED_BCOVOV);	//#define APS_DLED_MODE		1	//NOMAL(LED ON)/TEST(LED OFF) MODE
	else      cbit(DioLedBuf17_26, DLED_BCOVOV);		//MODEAck = NOMAL(L),TEST(H) MODE
	if(btst(FLFaultSWBuf,SWF_BCOCOC))     sbit(DioLedBuf17_26, DLED_BCOCOC);				//#define APS_DLED_RUN		2	//RUN 지령오면 ON
	else      cbit(DioLedBuf17_26, DLED_BCOCOC);
	if(btst(FLFaultSWBuf,SWF_BCIFCOC))     sbit(DioLedBuf17_26, DLED_BCIFCOC);	//#define APS_DLED_ACIVOK	3	//AC가선전압(380V임,입력전압)
	else      cbit(DioLedBuf17_26, DLED_BCIFCOC);
	if(btst(FLFaultEtcBuf, ETCERR_CPSF))     sbit(DioLedBuf17_26, DLED_BCCPSF);				//#define APS_DLED_LKA		4	// Line Contactor led on	
	else      cbit(DioLedBuf17_26, DLED_BCCPSF);
	if(btst(FLFaultSWBuf,SWF_BCOCUC))     sbit(DioLedBuf17_26, DLED_BCOCUC);				
	else      cbit(DioLedBuf17_26, DLED_BCOCUC);
	if(btst(FLFaultSeqBuf, SEQERR_LKa))     sbit(DioLedBuf17_26, DLED_BCLKF);	
	else      cbit(DioLedBuf17_26, DLED_BCLKF);
	if(btst(FLFaultSeqBuf, SEQERR_CHKa))			sbit(DioLedBuf17_26, DLED_BCCHKF);	
	else      cbit(DioLedBuf17_26, DLED_BCCHKF);
	

	if(MODEAck == BOARDTEST)
	{
		//======================================
		// DIO보드 출력
		//======================================	
		DOUT_CS1() = (TEST_DioOutBuf1 & 0xFFFF);
		//======================================
		// DIO보드 LED 표시
		//======================================
		DIO_LED_CS1() = (TEST_DioLedBuf01_16 & 0xFFFF);// DIO1 BD
		DIO_LED_CS2() = (TEST_DioLedBuf17_26 & 0xFFFF);// DIO1 BD
	}
	else
	{
		//======================================
		// DIO보드 출력
		//======================================	
		DOUT_CS1() = (DioOutBuf1 & 0xFFFF);//
		//======================================
		// DIO보드 LED 표시
		//======================================
		DIO_LED_CS1() = (DioLedBuf01_16 & 0xFFFF);// DIO1 BD
		DIO_LED_CS2() = (DioLedBuf17_26 & 0xFFFF);// DIO1 BD
	}
	//=====================
	// VFD 표시
	//=====================
	VFD_data(); //VFD 출력
}

void VFD_data(void)
{
	CS_WR_VFD1() = DioVfdBuf4 & 0xFF;
	CS_WR_VFD2() = DioVfdBuf3 & 0xFF;
	CS_WR_VFD3() = DioVfdBuf2 & 0xFF;
	CS_WR_VFD4() = DioVfdBuf1 & 0xFF;	
}

void VFD_CONTROL(void)
{
	Uint16 Temp100 = 0;
	Uint16 Temp10 = 0;
	Uint16 Temp1 = 0;
	if( (SW_VFDSW == 0x03))
	{// TEST 모드에서만 모드변경 가능
		SW_VFDSW_cnt++; 
		if(SW_VFDSW_cnt>3)
		{ // 3초 동안 눌러저야 모드 변경 가능
			SW_VFDSW_cnt = 0;
			VFD_mode++;
			// 20190323 모드 추가 변수 초기화
			SW_VFDSW = 0x02;
			DAC_CONTROL = 0;
			DAC_CMD_SEQ = 0;
			DAC_CH_SEQ = 1;
			// 20190323 모드 추가 변수 초기화
			if(MODEAck == TESTMODE)
			{//TEST 모드에서는 VFD모드 전부 실행가능
				if(VFD_mode >3)
				{
					VFD_mode = 0;
				}  // 20190323 모드 추가 조건 수정 'VFD_mode > 2' => 'VFD_mode > 3'
			}
			else
			{// NORMAL 모드에서는  VFD모드 전부 실행가능
				if(VFD_mode >3)
				{
					VFD_mode = 0;
				}
			}
		}
	}
	else
	{
		SW_VFDSW_cnt = 0;
	}
	/*
	if(MODEAck == NORMALMODE){// NORMAL모드에서는 DAC 동작 못시키게 강제로 막아놓음
		DAC_CONTROL = 0;
		if(VFD_mode >1){VFD_mode = 0;}
	}
	*/
	switch(VFD_mode){
		case 0 :
			if(MODEAck == BOARDTEST){
				DioVfdBuf1 = 'B';
				DioVfdBuf2 = 'D';
				DioVfdBuf3 = 'T';
				DioVfdBuf4 = 'S';
			}
			else if(MODEAck == NORMALMODE){
				DioVfdBuf1 = 'K';
				DioVfdBuf2 = '4';
				DioVfdBuf3 = '4';
				DioVfdBuf4 = '8';
			}
			else{
				DioVfdBuf1 = 'T';
				DioVfdBuf2 = 'E';
				DioVfdBuf3 = 'S';
				DioVfdBuf4 = 'T';
			}
			
			break;
		case 1 :
			Temp100 = ProgramVersion/100;
			Temp10  = (ProgramVersion - Temp100*100)/10;
			Temp1   = (ProgramVersion - Temp100*100 - Temp10*10);
			
			DioVfdBuf1 = 'V';
			DioVfdBuf2 = Temp10 + 48;
			DioVfdBuf3 = Temp1 + 48;
			DioVfdBuf4 = 48;

			break;
		case 2 :
			if(SW_VFDSW == 0x01){
				DAC_CONTROL = 1;
				DAC_CMD_SEQ = 0;
				DAC_CH_SEQ = 1;
			}
			if(SW_VFDSW == 0x02){
				DAC_CONTROL = 0;
				DAC_CMD_SEQ = 0;
				DAC_CH_SEQ = 1;
			}
			if(DAC_CONTROL){
				DioVfdBuf1 = 'D';
				DioVfdBuf2 = '1'; // 20190323 모드 추가로 인한 표시 변경 'A' => '1'
				DioVfdBuf3 = 'O';
				DioVfdBuf4 = 'N';
			}
			else{
				DioVfdBuf1 = 'D';
				DioVfdBuf2 = '1'; // 20190323 모드 추가로 인한 표시 변경 'A' => '1'
				DioVfdBuf3 = 'O';
				DioVfdBuf4 = 'F';
			}
			break;
		// 20190323 모드 추가
				case 3 :
			if(SW_VFDSW == 0x01){
				DAC_CONTROL = 1;
				DAC_CMD_SEQ = 0;
				DAC_CH_SEQ = 1;
			}
			if(SW_VFDSW == 0x02){
				DAC_CONTROL = 0;
				DAC_CMD_SEQ = 0;
				DAC_CH_SEQ = 1;
			}
			if(DAC_CONTROL){
				DioVfdBuf1 = 'D';
				DioVfdBuf2 = '2';
				DioVfdBuf3 = 'O';
				DioVfdBuf4 = 'N';
			}
			else{
				DioVfdBuf1 = 'D';
				DioVfdBuf2 = '2';
				DioVfdBuf3 = 'O';
				DioVfdBuf4 = 'F';
			}
			break;
		// 20190323 모드 추가

		default :
			break;
	}
}




