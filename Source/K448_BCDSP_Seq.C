//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// BACH Main Program																//
// 나중에 화일이름 NTHS_Bach로 변경													//
// DATE : 2010. 05. 14            						  by PACTECH R&D LSH		//
//==================================================================================//
// 차량과 인터페이스 항목===========================================================//
//⊙RESET          ☞  주로 과전압등 신호 RESET, 중고장인 경우 고장이 우선          //
//⊙MCB(VCB) ON :  ☞  APS/BACH START신호임                                         //
//⊙가선전압검출 : ☞  AC100V 입력받기로 함                                         //

#include "Global_Variable_Function.h"

//==================================================================================//
// 함수명 : SequenceTimer()															//
// 기  능 : seq_timer, fault timer등 시퀸스 관련 타이머 세팅						//
// calling : main(), 10msec															//
// 함수명 : Bach_Sequence()														    //
// 기  능 : 프로그램 시퀸스 부분 처리												//
// calling : main(), 10msec에서 동작                                                //
// DATE  : 2010. 08. 9																//
//==================================================================================//

Uint16 faultcnt1 = 0;
Uint16 faultcnt2 = 0;
Uint16 faultcnt3 = 0;

Uint16 MCBRUN_USER = 0;
Uint16 StartMode_cnt = 0;

void Bach_Sequence()
{
	DIO_InputChk();			//Digital 입력신호 처리		
	SequenceTimer();		//Sequence동작 타이머
	if(MODEAck != BOARDTEST){
		SEQ_StartChk();
		Reset_Sequence();
		SEQ_StatusCheck();	
		SEQ_Step_Process();
	}
	DIO_OutPutProcess();
	
}

void Reset_Sequence()
{
	if((DINAck1_BCRESET_OLD == 1)&&(DINAck1_BCRESET == 0) && (MajorFault == YES)){
		MidFCounter0 = 0;
		MidFCounter1 = 0;
		MidFCounter2 = 0;
		MidFCounter3 = 0;
		MidFCounter4 = 0;
		MidFCounter5 = 0;
		MidFCounter6 = 0;
		MidFCounter7 = 0;
		MidFCounter8 = 0;
		MidFCounter9 = 0;
		MidFCounter10 = 0;
		MidFCounter11 = 0;
		MidFaultMJF = 0;		// 2. 경중고장(3분이내 동일고장 3회시 중고장 처리되는 고장) 
		HighFaultMJF = 0;	// 
		MidFaultAck = 0;
		HighFaultAck = 0;
		MajorFault = 0;
		FaultClear();
		RunOKNG = RUN_NG;
		Sys_Status = SYS_Restart_STATE;
	}
	DINAck1_BCRESET_OLD = DINAck1_BCRESET;
}

//==================================================================================//
//==================================================================================//
// 함수명 : SEQ_StartChk()													    //
// 기  능 : 기동조건 점검(가선전압, 입력전압,차량RUN지령, PLL, FAULT등)				//
// calling : Bach_Sequence()                                   //
// 결과 : APSRunOKNG : YES(기동조건만족), NO(기동안됨)                              //
// DATE  : 2010. 08. 9																//
//==================================================================================//
void SEQ_StartChk()
{
	if(MODEAck == TESTMODE){
		StartMode = SEQ_ACMODE;

		
		if(AcInputVolUVWRms > (RatedACInputVoltage * 0.1))	InputACVolAck = YES;//(380v*0.1 = 38v, 테스트모드에서 38V이상시 동작안함)
		else	InputACVolAck = NO;
	
		if(DCInputVol_LPF > (RatedDCInputVoltage * 0.1))	InputDCVolAck = YES;//(380v*0.1 = 38v, 테스트모드에서 38V이상시 동작안함)
		else	InputDCVolAck = NO;


			//-----------------------20201121 test---------------------------------------------------------
/*		
		if(AcInputVolUVWRms > (500))	InputACVolAck = YES;//(380v*0.1 = 38v, 테스트모드에서 38V이상시 동작안함)
		else	InputACVolAck = NO;
	
		if(DCInputVol_LPF > (900))	InputDCVolAck = YES;//(380v*0.1 = 38v, 테스트모드에서 38V이상시 동작안함)
		else	InputDCVolAck = NO;
*/		
		//-----------------------20201121 test--------------------------------------------------------------
		if( (InputACVolAck == NO) && (InputDCVolAck == NO) && (FaultAck == NO) ){
			ReadyAck = YES;
		}
		else{
			ReadyAck = NO;
		}
		
// 20190314 jjh 런 신호 없는 경우 테스트 모드 진입 조건 추가		if((ReadyAck == YES)&&(DINAck1_BCRUN == YES)){
		if((ReadyAck == YES)&&(DINAck1_BCRUN == NO)){
			RunOKNG = RUN_OK;	// 테스트모드에서 입력전압이 없으면 APSRunOKNG = APS_RUN_OK;
		}
		else{
			RunOKNG = RUN_NG;	// 테스트모드에서 입력전압이 있으면 APSRunOKNG = APS_RUN_NG;
		}
	}
	else{	// NORMAL MODE

//-----------------------------------------------------------------------------------------------------------------------------------------
	if((AcInputVolUVWLLRms > 303.)&&(InputACVolAck == NO)){ // 20210128 dcy AC 저전압 폴트가 350이므로 330에서 350으로 변경 
		InputACVolAck_buf = YES;
		if (InputACVolAck_buf != InputACVolAck_old){
			InputACVolAck_cnt++;
			if (InputACVolAck_cnt >= 600){ // 20210414 DC 모드 입력전압 인가상태에서 AC모드 기동 안하게 시간 늦춤 300--> 600
				InputACVolAck = YES;
				InputACVolAck_cnt = 0;
				InputACVolAck_old = InputACVolAck;
			}
		}
		else{
			InputACVolAck_cnt = 0;
			InputACVolAck = NO;
		}
	}	
/*
	else if(((AcInputVolUVWLLRms > 280.)&&(AcInputVolUVWLLRms <= 350.))&&(InputACVolAck == YES))
	{
		InputACVolAck_buf = NO;
		if (InputACVolAck_buf != InputACVolAck_old){
			InputACVolAck_cnt++;
			if (InputACVolAck_cnt >= 300){ //20210210 dcy 300
				InputACVolAck = NO;
				InputACVolAck_cnt = 0;
				InputACVolAck_old = InputACVolAck;
			}
		}
		else{
			InputACVolAck_cnt = 0;
			InputACVolAck = YES;
		}
	}
*/
	//------------------------20210217 dcy 입력저전압 폴트 때문에 
	else if((AcInputVolUVWLLRms <= 242.)&&(InputACVolAck == YES))
	{
		InputACVolAck_buf = NO;
		if (InputACVolAck_buf != InputACVolAck_old){
			InputACVolAck_cnt++;
			if (InputACVolAck_cnt >= 200){ //20210210 dcy 300
				InputACVolAck = NO;
				InputACVolAck_cnt = 0;
				InputACVolAck_old = InputACVolAck;
			}
		}
		else{
			InputACVolAck_cnt = 0;
			InputACVolAck = YES;
		}
	}
	else{
		InputACVolAck_cnt = 0;
		InputACVolAck_old = SEQ_NOMODE;
	}
//-----------------------------------------------------------------------------------------------------------------------------------------
	if((DCInputVol_LPF > 800.)&&(InputDCVolAck == NO)){
		InputDCVolAck_buf = YES;
		if (InputDCVolAck_buf != InputDCVolAck_old){
			InputDCVolAck_cnt++;
			if (InputDCVolAck_cnt >= 300){
				InputDCVolAck = YES;
				InputDCVolAck_cnt = 0;
				InputDCVolAck_old = InputDCVolAck;
			}
		}
		else{
			InputDCVolAck_cnt = 0;
			InputDCVolAck = NO;
		}
	}	
	else if((DCInputVol_LPF <= 800.)&&(InputDCVolAck == YES)){
		InputDCVolAck_buf = NO;
		if (InputDCVolAck_buf != InputDCVolAck_old){
			InputDCVolAck_cnt++;
			if (InputDCVolAck_cnt >= 200){ //20210219 dcy 300
				InputDCVolAck = NO;
				InputDCVolAck_cnt = 0;
				InputDCVolAck_old = InputDCVolAck;
			}
		}
		else{
			InputDCVolAck_cnt = 0;
			InputDCVolAck = YES;
		}
	}
	else{
		InputDCVolAck_cnt = 0;
		InputDCVolAck_old = SEQ_NOMODE;
	}

//-----------------------------------------------------------------------------------------------------------------------------------------
		if(((InputACVolAck == YES) || (InputDCVolAck == YES)) && (FaultAck == NO)){
			ReadyAck = YES;
		}
		else{
			ReadyAck = NO;
		}
//-----------------------------------------------------------------------------------------------------------------------------------------
	//	if((InputACVolAck == YES)&&(InputDCVolAck == NO)) //20210129 dcy 인버터 폴트 시 AC모드로 바뀌면서 저전압 폴트나는 것을 방지하기 위해 수정 
		if(((InputACVolAck == YES)&&(InputDCVolAck == NO))&&(SEQ_Step < SEQ_START))
		{
			StartMode = SEQ_ACMODE; // 0
		}

		// 20200620 else if((InputACVolAck == YES)&&(InputDCVolAck == YES))
		//else if((InputACVolAck == YES)&&(InputDCVolAck == YES) || (InputACVolAck == NO)&&(InputDCVolAck == YES))//20210129 dcy 인버터 폴트 시 AC모드로 바뀌면서 저전압 폴트나는 것을 방지하기 위해 수정 
		else if(((InputACVolAck == YES)&&(InputDCVolAck == YES) || (InputACVolAck == NO)&&(InputDCVolAck == YES)))
		{
			StartMode = SEQ_DCMODE; // 1
		}
// 20200425 코드 수정 SEQ_NOMODE 추가
		if(StartMode == SEQ_NOMODE){ // 2 
			ReadyAck = NO;
		}
		else{;}		
// 20200425 코드 수정 SEQ_NOMODE 추가
// 20201215 dcy StartMode_cnt 추가 형식시험 기동 정지 시험 중 무부하시 재기동 할 때 dc 전압 출력 발생 방지 위해  20210120 지움 
		if(((StartMode == SEQ_ACMODE)&&(AcInputVolUVWLLRms <= 242.))||(DINAck1_BCRUN == NO)){ //20210219 dcy  350->280
			StartMode_cnt++;
			if(StartMode_cnt > 200) //20210210 dcy 출력전압 저전압 폴트 방지를 위해 
			{
				ReadyAck = NO;
				InputACVolAck_old = SEQ_NOMODE;
				InputDCVolAck_old = SEQ_NOMODE;	
				StartMode = SEQ_NOMODE;	//20210129 dcy 인버터 폴트 시 AC모드로 바뀌면서 저전압 폴트나는 것을 방지하기 위해 수정 
				SEQ_Step = SEQ_DO_NOTHING_STATE;
				StartMode_cnt = 0;
			}	
		}
	//	else{StartMode_cnt = 0;}
		else if(((StartMode == SEQ_DCMODE)&&(DCInputVol_LPF <= 800.))||(DINAck1_BCRUN == NO)){
			
			StartMode_cnt++;
			if(StartMode_cnt > 200)
			{
				ReadyAck = NO;
				InputACVolAck_old = SEQ_NOMODE;
				InputDCVolAck_old = SEQ_NOMODE;	
				StartMode = SEQ_NOMODE; //20210129 dcy 인버터 폴트 시 AC모드로 바뀌면서 저전압 폴트나는 것을 방지하기 위해 수정 
				SEQ_Step = SEQ_DO_NOTHING_STATE;
				StartMode_cnt = 0;
			}	
		}
		else{StartMode_cnt = 0;}

//-----------------------------------------------------------------------------------------------------------------------------------------

		if((DINAck1_BCRUN == YES)&&(ReadyAck == YES)){
			RunOKNG = RUN_OK;	// 노말모드에서 상위RUN지령이 있고,입력전압이 있고, PLL이 인식되고, 폴트가 없으면 APSRunOKNG = APS_RUN_OK;
	//		SeqTimerSet = 300;
		}
		else{
			RunOKNG = RUN_NG;	// 노말모드에서 상위RUN지령이 없거나, 입력전압이 없거나, PLL이 인식안되거나, 폴트가 있으면 APSRunOKNG = APS_RUN_NG;	
			StartMode = SEQ_NOMODE;
		}
	}
}
//==================================================================================//
void SEQ_StatusCheck()
{	
	
	switch(Sys_Status){
		case SYS_INIT_STATE:	//0		//Seq 초기조건			
 			SEQ_Step = SEQ_DO_NOTHING_STATE;
			Sys_Status = SYS_CHK_STATE;
		break;
		case SYS_CHK_STATE:		
			if(RunOKNG == RUN_OK){	// 초기 기동시 정상기동조건이면
				if(SEQ_Step == SEQ_DO_NOTHING_STATE){ 
					SEQ_Step = SEQ_START;			//정상기동조건에서 아무것도 하지 않고 있으면 처음 시작 Sequence Start
				}
			}
			else{// 정상기동조건이 아닐때...
				ALL_PWM_OFF();
				SEQ_Step = SEQ_ALLOFF;	// 정상조건 아니면..

				if(FaultAck){
					if(MidFaultAck){// 중간레벨 고장시(3분3회고장적용고장) 5초후 재기동
						MidiumFault_CountCheck_TEST();
						FaultCounter++;		//폴트 카운터 증가시키고
						FaultRestartTimer_flag = FLAGCLR;
						FaultRestartTimerSet = 500;// 5sec후 재기동 5sec = 10msec*500;	
						Sys_Status = SYS_Restart_STATE;
					}
					else{		// 경고장일때 토탈 폴트タ疽拷? 하고...자동 재기동
						FaultCounter++;		//폴트 카운터 증시키고 -->모니터링 폴트 카운터 -->재기동 횟수??
						FaultRestartTimer_flag = FLAGCLR;
						FaultRestartTimerSet = 500;// 5sec후 재기동 5sec = 10msec*500;	
						Sys_Status = SYS_Restart_STATE;
					}
				}
				if((MidFaultMJF > 0)||(HighFaultMJF==1)){	// 3분3회중고장 또는 중고장시 메이저폴트발생
					MajorFault = YES;
		 			SEQ_Step = SEQ_ALLOFF;			//시퀸스 초기화
					Sys_Status = SYS_MajorFault_STATE;	//Sys_Status를 majorfault로 ..
				}
			}
		break;	
		case SYS_Restart_STATE:		//2
			if (FaultRestartTimer_flag == FLAGSET){						// 재기동 시간 5sec check
				FaultClear();	// 재기동전 폴트 클리어시킴...
				Sys_Status = SYS_CHK_STATE;
				SEQ_Step = SEQ_DO_NOTHING_STATE; 	
				//VariableInit();	// 변수 초기화
			}
			if((MidFaultMJF > 0)||(HighFaultMJF==1)){	// 3분3회중고장 또는 중고장시 메이저폴트발생
				MajorFault = YES;
	 			SEQ_Step = SEQ_ALLOFF;			//시퀸스 초기화
				Sys_Status = SYS_MajorFault_STATE;	//Sys_Status를 majorfault로 ..
			}
		break;								
		case SYS_MajorFault_STATE:		//3		//중고장일경우 폴트 클리어 안함
			
			BachPwmDisable();
			BachPwmModulation = NO;
		
			// all output clear
			CHK_OFF();
			LK_OFF();
			MajorFault = YES;
			MidFaultAck = 0;
			//FaultAck = NO;			
		break;	
		default:
			Sys_Status = SYS_INIT_STATE;
		break;
	}
	
	MidiumFault_3min3N_Check();	//3분이내 3회 동일고장시 중고장 검사

}
//==================================================================================//
// 함수명 : SEQ_Step_Process()													    //
// 기  능 : APS/BACH SEQUENCE STEP                                                  //
// calling : Bach_Sequence()                                                        //
// DATE  : 2010. 12. 16   , 수정 : 2010.12.22                                       // SEQ_Step = SEQ_DO_NOTHING_STATE;
//==================================================================================//
void SEQ_Step_Process()
{
	switch(SEQ_Step){
		case SEQ_DO_NOTHING_STATE:		//0
			BCOK_OFF();
			BCOCUC_FL_cnt = 0;
			;;
		break;							
		case SEQ_START:					//1
			BCOCUC_FL_cnt = 0;
//			FaultClear();	// in Nths_Fault.c
//			ALL_PWM_OFF();
			bc_vol_cnt = 6;
			SeqTimer_flag = FLAGCLR;//Clear;
			//SeqTimerSet = 50;			//0.5Sec = 10msec*50	
			SeqTimerSet = 100;			//// 201108300.5Sec = 10msec*50	
			if(StartMode == SEQ_ACMODE){
//				SEQ_Step = SEQ_CHK_ON;            //20220502 연장급전 시 chk lk 미작동하여 배터리 충전기 미작동 
 				SEQ_Step = SEQ_START;
			}
			//else { //20210129 dcy StartMode = SEQ_NOMODE 일때에 SEQ_Step = SEQ_BACH_CON_ON;으로 넘어가는 것을 방지하기 위해 추가 
			//	SEQ_Step = SEQ_BACH_CON_ON;
			//}
			else if(StartMode == SEQ_DCMODE)
			{
				SEQ_Step = SEQ_BACH_CON_ON;
			}
			else	SEQ_Step = SEQ_START;
		break;
		case SEQ_CHK_ON:				//2
			if(SeqTimer_flag){
				CHK_ON();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 300;			// CHK동작시간 & 자연충전시간 세팅 0.5Sec
				//SeqTimerSet = 250;			// 20110830,CHK동작시간 & 자연충전시간 세팅 0.5Sec
				
				SEQ_Step = SEQ_CHKa_CHK;
			}
		break;
		case SEQ_CHKa_CHK:			//3
			if(SeqTimer_flag){ 											
				if(((DINAck1_BCCHKa == YES) && (DCInputVol_LPF > 216.0))||((DINAck1_BCCHKa == YES) && (MODEAck == TESTMODE))){	
					SEQ_Step = SEQ_LK_ON;
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 50;	//다음 1sec시간 세팅	// 20110830,100->50
				}
				else{
					SEQ_Step = SEQ_ALLOFF;
					sbit(FaultSeqBuf,SEQERR_CHKa);	// CHK or DCLINK Voltage low level ERROR
					sbit(MidFaultAck,MidFaultAck10);
				}				
			}
		break;
		case SEQ_LK_ON:					//4
			if(SeqTimer_flag){	
				LK_ON();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 100;			//LK동작시간 1sec시간 세팅
				SEQ_Step = SEQ_LKa_CHK;
			}
		break;
		case SEQ_LKa_CHK:			//5
			if(SeqTimer_flag){ 	// Normal mode에서는 chk 접점, dclink자연충전전압 확인													
				if(((DINAck1_BCLKa == YES) && (DCInputVol_LPF > 346.0)) || ((DINAck1_BCLKa == YES) && (MODEAck == TESTMODE))){
					SEQ_Step = SEQ_CHK_OFF;
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 100;	//다음 1sec시간 세팅	
				}
				else{
					SEQ_Step = SEQ_ALLOFF;
					sbit(FaultSeqBuf,SEQERR_LKa);	// LK or DCLINK Voltage low level ERROR
					sbit(MidFaultAck,MidFaultAck11);
				}				
			}
		break;
		case SEQ_CHK_OFF:				//6
			if(SeqTimer_flag){
				CHK_OFF();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 100;				//LK off동작시간 1sec시간 세팅
				SEQ_Step = SEQ_BACH_CON_ON;
			}
		break;			
		case SEQ_BACH_CON_ON:		//7
			if(SeqTimer_flag){
				DCDC_SoftStart_flag = YES;		//제어기,Ramp, pwm_ON
				bcOutVolRef = BachOutVol_LPF;
				BachPWM_ON();
				SeqTimer_flag = FLAGCLR;
				SeqTimerSet = 200;				//2.0Sec // softstart시간보다 길게...
				SEQ_Step = SEQ_OUTV_CHK;
			}
		break;				
		case SEQ_OUTV_CHK:			//8
			if(SeqTimer_flag){						//정격의 98%이상
			//20200425 95 -> 80													
				if(((MODEAck == NORMALMODE)&&(BachOutVol_LPF > 80.)) || (MODEAck == TESTMODE)){
					SeqTimer_flag = FLAGCLR;
					SeqTimerSet = 200;						//다음 Seq시간 세팅
					SEQ_Step = SEQ_Normal_State;
				}
			}
		break;
		case SEQ_Normal_State:			//9
			BCOK_ON();

		break;
		case SEQ_ALLOFF:				//10
			BCOCUC_FL_cnt = 0;
			BCOK_OFF();
			InitBachDcDcControlVar();	
			ALL_PWM_OFF();			
			CHK_OFF();
			LK_OFF();
			SeqTimer_flag = FLAGCLR;
			SeqTimerSet = 50;	//다음 Seq시간 세팅	
			SEQ_Step =  SEQ_DO_NOTHING_STATE;
		break;
		case SEQ_RESET:					//15
			SEQ_Step =  SEQ_ALLOFF;
			BCOK_OFF();
		break;		
		default:
			SEQ_Step = SEQ_ALLOFF;
			BCOK_OFF();
		break;
	}
	
}

//==================================================================================//
// 함수명 : SequenceTimer()														    //
// DATE  : 2010. 11. 4																//
//==================================================================================//
void SequenceTimer()
{
	if(SeqTimer_flag == FLAGCLR){
		SeqTimer++;
		if(SeqTimer >= SeqTimerSet)	{
			SeqTimer_flag = FLAGSET;
			SeqTimer = 0;
		}
	}
	if(FaultRestartTimer_flag == FLAGCLR){
		FaultRestartTimer++;
		if(FaultRestartTimer >= FaultRestartTimerSet){
			FaultRestartTimer_flag = FLAGSET;
			FaultRestartTimer = 0;
		}
	}

}
//==================================================================================//
// 함수명 : DIO 처리 함수들()													    //
// DATE  : 2010. 08. 6																//
//==================================================================================//
void BCMJF_ON()
{
	sbit(DioOutBuf1,DOUT_BCMJF);		// BOXOK -- APS/BACH 정상출력동작일때...차량으로 정상상태라고 출력
}
void BCMJF_OFF()
{
	cbit(DioOutBuf1,DOUT_BCMJF);		// BOXOK -- APS/BACH 정상출력동작일때...차량으로 정상상태라고 출력
}

void BCOK_ON()
{
	sbit(DioOutBuf1,DOUT_BCOK);		// BOXOK -- APS/BACH 정상출력동작일때...차량으로 정상상태라고 출력
}
void BCOK_OFF()
{
	cbit(DioOutBuf1,DOUT_BCOK);		// BOXOK -- APS/BACH 정상출력동작일때...차량으로 정상상태라고 출력
}

void CHK_ON()
{
	sbit(DioOutBuf1,DOUT_BCCHK);	// CHK ON();
}
void CHK_OFF()
{
	cbit(DioOutBuf1,DOUT_BCCHK);	// CHK ON();
}

void LK_ON()
{
	sbit(DioOutBuf1,DOUT_BCLK);		// LK ON();
}
void LK_OFF()
{
	cbit(DioOutBuf1,DOUT_BCLK);		// LK OFF();
}

void BachPWM_ON(){
	BachPwmModulation = YES;
}
void BachPWM_OFF(){
	BachPwmModulation = NO;
	BachPwmDisable();
}

void ALL_PWM_OFF()
{
	BachPwmModulation = NO;
	BachPwmDisable();
}
