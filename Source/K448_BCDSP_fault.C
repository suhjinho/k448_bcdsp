//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// BACH Main Program																//
// 나중에 화일이름 NTHS_Bach로 변경													//
// DATE : 2010. 05. 14            						  by PACTECH R&D LSH		//
//==================================================================================//
#include "Global_Variable_Function.h"


Uint16	tst_fault = 0;
float	Tst_floatvar1 = 0;
float	Tst_floatvar2 = 300.;
Uint16	ACIV_LV_delalyCnt = 0;
Uint16  SWF_AFCUV_cnt = 0;
//==================================================================================//
// 함수명 : FaultLevelSetting()							    						//
// 기  능 : H/W, S/W FAULT LEVEL SETTING											//
//----------------------------------------------------------------------------------//
// 전류 : HW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
// 전압 : SW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
//----------------------------------------------------------------------------------//
// 계산화일 : NTHS_센서계산.XLS														//
// DATE  : 2010. 08. 13																//
//==================================================================================//
void FaultLevelSetting()
{
	BCACICOV_FL = 600;
	BCIFCOV_FL = 1300.;
	BCIFCUV_FL = 450.;
	BCOVOV_FL = 115.;
	BCOVUV_FL = 75.;
	BCIFCOC_FL = 150.;
	BCOCOC_FL = 400.0;
	BCOCUC_FL = 10.0;
	PSF_FL = 70.0;
	BCACUV_FL = 270.;          //242V ~ 302V
}

//==================================================================================//
// 함수명 : SW_FaultProcess()		     										//
// 기  능 : S/W FAULT 1개라도 발생하면 dsp card epld에 sw fault를 써줌				//
//----------------------------------------------------------------------------------//
// 전류 : HW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
// 전압 : SW로 FAULT, SW(MIN/MAX)로 LIMIT제어										//
//----------------------------------------------------------------------------------//
// DATE  : 2010. 08. 13																//
//==================================================================================//
void SW_FaultProcess_int()
{	
	float Temp = 0;

	// 20200321 LKF 확인용 코드 추가
/*	ACInputVolU_LPF = TEST_ACInputVolU_LPF;
	ACInputVolV_LPF = TEST_ACInputVolV_LPF;
	ACInputVolW_LPF = TEST_ACInputVolW_LPF;

	AcInputVolUVWLLRms = TEST_AcInputVolUVWLLRms;

	DCInputVol_LPF = TEST_DCInputVol_LPF;
	BachOutVol_LPF = TEST_BachOutVol_LPF;
	InputCur_LPF = TEST_InputCur_LPF;
	BachOutCur_LPF = TEST_BachOutCur_LPF;	
*/
	Temp = fabs(DCInputVol_LPF);
	if (Temp > (BCIFCOV_FL+200)){
		SWF_BCIFCOV_cnt++;
		if(SWF_BCIFCOV_cnt > 2000){
			SWF_BCIFCOV_cnt = 2001;
			sbit(MidFaultAck, MidFaultAck1);	
			sbit(FaultSWBuf,SWF_BCIFCOV);
		}
	}
	else{
		SWF_BCIFCOV_cnt = 0;
	}
	if (Temp > BCIFCOV_FL){
		SWF_BCIFCOV2_cnt++;
		if(SWF_BCIFCOV2_cnt > 6000){
			SWF_BCIFCOV2_cnt = 6001;
			sbit(MidFaultAck, MidFaultAck1);	
			sbit(FaultSWBuf,SWF_BCIFCOV);
		}
	}
	else{
		SWF_BCIFCOV2_cnt = 0;
	}


	Temp = fabs(AcInputVolUVWLLRms);
	if (Temp > BCACICOV_FL){
		SWF_BCACICOV_cnt++;
		if(SWF_BCACICOV_cnt > 500){
			SWF_BCACICOV_cnt = 501;
			sbit(MidFaultAck, MidFaultAck1);	
			sbit(FaultSWBuf,SWF_BCACIOV);
		}
	}
	else{
		SWF_BCACICOV_cnt = 0;
	}
	
	Temp = fabs(BachOutVol_LPF);
	if (Temp > BCOVOV_FL){
		SWF_BCOVOV_cnt++;
		if(SWF_BCOVOV_cnt > 500){
			SWF_BCOVOV_cnt = 501;
			sbit(MidFaultAck, MidFaultAck2);	
			sbit(FaultSWBuf,SWF_BCOVOV);
		}
	}
	else{
		SWF_BCOVOV_cnt = 0;
	}
	
	Temp = fabs(InputCur_LPF);
	if (Temp > BCIFCOC_FL){
		sbit(MidFaultAck, MidFaultAck3);	
		sbit(FaultSWBuf,SWF_BCIFCOC);
	}
	
	Temp = fabs(BachOutCur_LPF);
	if (Temp > BCOCOC_FL){
		sbit(MidFaultAck, MidFaultAck4);	
		sbit(FaultSWBuf,SWF_BCOCOC);
	}
	
//	Temp = fabs(ControlVol_LPF);		// AC 출력 과전류
	Temp = ControlVol_LPF;		// 20210409 BC 제어전원 -값 나오는 거 수정 

	if(PSF100Ack == NO){	
		if ((Temp < (PSF_FL-5)))
		{
			PSF100Ack = YES;
		}
		else
		{
			PSF100Ack = NO;
		}
	}
	else
	{
		if ((Temp < (PSF_FL-2)))
		{
			PSF100Ack = YES;
		}
		else
		{
			PSF100Ack = NO;
		}
	}
	
//	if((MODEAck == NORMALMODE) && (SEQ_Step == SEQ_Normal_State)){ //DC링크 저전압 450V 
	if((MODEAck == NORMALMODE) && (SEQ_Step == SEQ_Normal_State))
	{ 
		Temp = fabs(DCInputVol_LPF); //DC링크 저전압 450V 
//		if (Temp < BCIFCUV_FL){
		if ((Temp < BCIFCUV_FL))// && (StartMode == SEQ_ACMODE)) 
		{
			SWF_BCIFCUV_cnt++;
	 	//if(SWF_BCIFCUV_cnt > 9376)             // 21.0713 suhjinho bclinkvoltage 사구간  under voltage 걸려서 fault 발생하여 검지 시간늘림 
		  if(SWF_BCIFCUV_cnt > 28000)			  // 9376->28000 1.2초 -> 3.7초 5	
			{ 									  // 210127  : 5000
			//	SWF_BCIFCUV_cnt = 9377;
				SWF_BCIFCUV_cnt = 28001;
				sbit(MidFaultAck, MidFaultAck6); //20201220 dcy 원래는 Ack6	
				sbit(FaultSWBuf,SWF_BCIFCUV);
			}
		}
		else
		{
			SWF_BCIFCUV_cnt = 0;
		}

		Temp = fabs(AcInputVolUVWLLRms); //AC 입력저전압  242~302V
		if (((Temp>242)&&( Temp < 302)) && (StartMode == SEQ_ACMODE))
		{
			SWF_AFCUV_cnt++;
			if(SWF_AFCUV_cnt > 9376)
			{
				SWF_AFCUV_cnt = 9377;
				sbit(MidFaultAck, MidFaultAck6);	
				sbit(FaultSWBuf,SWF_AFCUV);
			}
		}
		else
		{
			SWF_AFCUV_cnt = 0;
		}
	
		Temp = fabs(BachOutVol_LPF); //BC 출력 저전압 
		if((Temp < BCOVUV_FL))
		{
			SWF_BCOVUV_cnt++;
			if(SWF_BCOVUV_cnt > 9375)
			{
				SWF_BCOVUV_cnt = 9376;  // 210127 dcy : 9376
				sbit(MidFaultAck, MidFaultAck7);	//20210210 dcy 7
				sbit(FaultSWBuf,SWF_BCOVUV);	
			}
		}
		else
		{
			SWF_BCOVUV_cnt = 0;
		}
		
//----20210603 경인선 2편성 창원에서 3대중 1개 출력저전류 고장발생----------------------
//----출력저전류 폴트검지 기능 리마크함     ---------------------------------------
//-----20210603 이전 기존코드---------------------------------------------------------------
  		Temp = fabs(BachOutCur_LPF); //BC출력 저전류 
/*		if ((Temp < BCOCUC_FL))
		{
			BCOCUC_FL_cnt++; // 3750 1초 ,60초 225000, 7.5hz     5분 1125000 -> 6.5hz 975434 
			if(BCOCUC_FL_cnt > 975434)
			{
				BCOCUC_FL_cnt = 975435;
				HighFaultMJF = YES;	
				sbit(FaultSWBuf,SWF_BCOCUC);
			}
		}
		else
		{
			BCOCUC_FL_cnt = 0; 
		}
*/
//----------------------------------------------------------------------*/
	}

	if(FaultSWBuf > 0){
		FAULTGENSW(); // 또는 R_DSP_SW_FAULT_CS() = 0x00;
	}
}



//==================================================================================//
// 함수명 : GDU_FaultProcess()														//
// 기  능 : GDU FAULT LED 켜주고, 처리방법은....									//
//==================================================================================//
void GDU_FaultProcess_int()
{
	//---DSP FAULT (DSP GDU FAULT, SENCARD H/W FAULT, SW FAULT)
	FaultGDFBuf = RD_DSP_FAULT_CS() & 0x03; //PWORD(0x20D000)

	if(FaultGDFBuf != 0){
		sbit(MidFaultAck, MidFaultAck0);		// 중간레벨(3분3회) - GDF
	}
}


void ETC_FaultProcess_int()
{	
	if(MajorFault == YES){
		BCMJF_ON();
	}
	else{
		BCMJF_OFF();
	}
	if(DINAck1_BCTEMP == NO){
		THRFAck_Counter++;
		if(THRFAck_Counter > 195653){ 			//	7.5hz 225000 ->  6.5hz 195653
			THRFAck_Counter = 195654;
			sbit(FaultEtcBuf, ETCERR_HSOT);
			sbit(MidFaultAck, MidFaultAck8);
		}
	}
	else{
		THRFAck_Counter = 0;
	}
	
	if(PSF100Ack == YES){
		PSF100Ack_cnt++;
		if(PSF100Ack_cnt > 18750){
			PSF100Ack_cnt = 18751;
			sbit(FaultEtcBuf, ETCERR_CPSF);
			sbit(MidFaultAck, MidFaultAck9);
		}
	}
	else{
		PSF100Ack_cnt =0;
	}

	
	if(((SEQ_Step < SEQ_CHK_ON) && (DINAck1_BCCHKa == YES) &&(StartMode == SEQ_ACMODE)) || ((SEQ_Step < SEQ_CHK_ON) && (DINAck1_BCCHKa == YES) &&(MODEAck == TESTMODE))){
		BCCHKa_cnt++;
		if(BCCHKa_cnt > 20000){
			sbit(FaultSeqBuf, SEQERR_CHKa);
			sbit(MidFaultAck, MidFaultAck10);
		}
	}
	else{
		BCCHKa_cnt = 0;
	}
	
	if(((SEQ_Step > SEQ_BACH_CON_ON) && (DINAck1_BCCHKa == YES) && (StartMode == SEQ_ACMODE)) || ((SEQ_Step > SEQ_BACH_CON_ON) && (DINAck1_BCCHKa == YES) &&(MODEAck == TESTMODE))){
		BCCHKa_cnt++;
		if(BCCHKa_cnt > 20000){
			sbit(FaultSeqBuf, SEQERR_CHKa);
			sbit(MidFaultAck, MidFaultAck10);
			
		}
	}
	else{
		BCCHKa_cnt = 0;
	}
	
	if((SEQ_Step < SEQ_LK_ON) && (DINAck1_BCLKa == YES)){
		BCLKa_cnt++;
		if(BCLKa_cnt > 20000){
			sbit(FaultSeqBuf, SEQERR_LKa);
			sbit(MidFaultAck, MidFaultAck11);
		}
	}
	else{
		BCLKa_cnt = 0;
	}
	


// 20200322 5000 Comment 이유는 알 수 없지만 주석처리 되어 있었음 
// 20201123 dcy 기동 후 LKa 접점 신호 확인하기 위해 주석처리 된 구문 다시 살림

	if(((SEQ_Step > SEQ_BACH_CON_ON) && (DINAck1_BCLKa == NO)&&(StartMode == SEQ_ACMODE)) || ((SEQ_Step > SEQ_BACH_CON_ON) && (DINAck1_BCLKa == NO) && (MODEAck == TESTMODE))){
		BCLKa_cnt++;


		if(BCLKa_cnt > 20000){
			sbit(FaultSeqBuf, SEQERR_LKa);
			sbit(MidFaultAck, MidFaultAck11);
		}
	}
	else{
		BCLKa_cnt = 0;
	}
	
}


//==================================================================================//
// 함수명 : HW_FaultProcess()														//
// 기  능 : H/W FAULT LED 켜주고, 처리방법은....									//
//----------------------------------------------------------------------------------//
// DATE  : 2010. 08. 13																//
//==================================================================================//

void HW_FaultProcess_int()
{
	//---HW FAULT(APS,BACH는 EPLD 소스가 틀림)--

//--------------------20201121 EPLD HWF 수정 -----------------------------	
/*FaultHWBuf = ~RD_DSP_FAULT_CS() & 0xFC;//
*/
//--------------------20201121---------------------------------------------
//-------------------20201121 EPLD 수정으로 인한 HWF 수정 -----------------
	FaultHWBuf = RD_DSP_FAULT_CS() & 0xFC;//
	//FaultHWBuf = (FaultHWBuf>>2) & 0x3F;
	FaultHWBuf = (FaultHWBuf>>2) & 0x38;
	//FaultHWBuf = (FaultHWBuf) & 0x38;
//-------------------20201121 EPLD 수정으로 인한 HWF 수정 -----------------	
	/*
	if(btst(FaultHWBuf,BACH_HWF_ACICOCF)){	//ACCT1F = ACIC, AC LINE 입력전류, 주회로(ACCT)
		sbit(MidFaultAck, MidINOCF);	// 중간레벨(3분3회) - 입력과전류
		sbit(SenLedBuf, SLED_ACI_OCF);
	}
	*/
				
	
}

//==================================================================================//
// 함수명 : FaultProcess() 인터럽트루틴에서 바로 차단 ==>테스트용				        //
// 기  능 : H/W, S/W , GDU, FAULT LEVEL SETTING									    //
//----------------------------------------------------------------------------------//
// CALLING : Nths_Seq.c                        										//
// DATE  : 2010. 12. 02	, 수정(20101207) : etc폴트 추가							//
//==================================================================================//
void FaultProcess_int()
{	

	SW_FaultProcess_int();	// 센서 SW FAULT 처리 //20190409 MON보드 펌웨어 수정을 위해 폴트 삭제 처리
	GDU_FaultProcess_int();			// GDU FAULT 처리
	HW_FaultProcess_int();		// HW FAULT 처리 20201121 주석처리되어 있는것 활성 20201220 dcy

	ETC_FaultProcess_int();		//방열판온도,휴즈 폴트처리
	
	if((FaultGDFBuf != 0)||(FaultHWBuf != 0) ||(FaultSWBuf != 0) ||(FaultSeqBuf != 0) ||(FaultEtcBuf != 0)){
		if(MajorFault == YES){
			FaultAck = YES;
			FLFaultGDFBuf |= FaultGDFBuf;
		//	FLFaultHWBuf |= FaultHWBuf;
			FLFaultSWBuf |= FaultSWBuf;
			FLFaultSeqBuf |= FaultSeqBuf;
			FLFaultEtcBuf |= FaultEtcBuf;
		}
		else{
			FaultAck = YES;
			FLFaultGDFBuf = FaultGDFBuf;
		//	FLFaultHWBuf = FaultHWBuf;
			FLFaultSWBuf = FaultSWBuf;
			FLFaultSeqBuf = FaultSeqBuf;
			FLFaultEtcBuf = FaultEtcBuf;
		}
		
		FAULTGENSW(); //소프트웨어로 하드웨어 폴트 발생하여 PWM차단
	}
//====추가--중고장인 경우 인터럽트에서 바로 차단====

	if(FaultAck == YES){
		BachPwmModulation = NO;
		CHK_OFF();
		LK_OFF();
	}
}

//==================================================================================//
// 함수명 : FaultClear()													//
// 기  능 : H/W, S/W, GDU FAULT Clear											//
//----------------------------------------------------------------------------------//
// DATE  : 2010. 08. 09																//
//==================================================================================//
void FaultClear()
{	
	FAULTCLR();
	THRFAck_Counter = 0;
	SWF_BCIFCUV_cnt = 0;
	SWF_BCOVUV_cnt = 0;
	SWF_BCOVOV_cnt = 0;
	SWF_BCIFCOV_cnt = 0;
	SWF_BCIFCOV2_cnt = 0;
	SWF_BCACICOV_cnt = 0;
	PSF100Ack_cnt = 0;
	FaultAck = NO;	
	MidFaultAck = 0;
	LowFaultAck = 0;
	
	FaultGDFBuf = 0x00;	
	FaultHWBuf = 0x00;
	FaultSWBuf = 0x00;
	FaultSeqBuf = 0x00;
	FaultEtcBuf = 0x00;
	
	FLFaultGDFBuf = 0x00;
	FLFaultHWBuf = 0x00;
	FLFaultSWBuf = 0x00;
	FLFaultSeqBuf = 0x00;
	FLFaultEtcBuf = 0x00;
	
	//VariableInit();	// 변수 초기화

}
//==================================================================================//
// 함수명 : MidiumFaultProcess_int()													//
// 기  능 : 3분3회 동일고장시 중고장 처리하는 중간레벨 폴트										//
//----------------------------------------------------------------------------------//
// DATE  : 2011. 03. 09																//
//=====================_=============================================================//

//==========================================================================//
// 폴트처리
// 폴트를 레벨로 분류해서
// 
//==============================================================================//
// 1. 경고장 --> 조건해제시 재기동함, 3분이내 3회 중고장 처리 안함
//    LowLevelFault(LF)
//    1.1 입력저전압
//    1.2 출력저전압
//    1.3 DCLINK저전압
//    1.4 과부하
//==============================================================================//
// 2. 경중고장 --> 3분이내 3회 동일고장시 중고장 처리
//    MidiumLevleFault(MF)
//    2.1 GDF
//    2.2 입력과전압, 입력과전류, 출력과전압, 출력과전류
//    2.3 과부하 <- 120% 1분이상 지속시
//    2.4 제어전원 저전압(PSF)
//==============================================================================//
// 3. 중고장 --> 재기동 안함
//    HighLevelFault(HF,Major fault)
//    MTF : 주변압기 과열검지(149도)
//    OPF : 주변압기 오일펌프 2대모두 고장
//    GRF : 장비내부 지락폴트
//    FUSEF : 입력휴즈 단선
//    HSKTHRF : 히트싱크 2개중 하나라도 폴트발생시 과온폴트(95돛鵑?1분이상 지속시 과온판단)
//    FANF : APS(TC,M2,M3 CAR)/BACH - HVAC차단상태 신호, APS(MC) - 박스내 팬 폴트
//           APS(TC,M2,M3 CAR) 재기동시 HVAC가 차단상태이므로....APS재기동 기간에는 FANF신호를 FAULT처리하면 안됨
//           APS(TC,M2,M3 CAR) - HVAC차단상태신호, 자체 정상출력후 점검해야함
//           APS(MC CAR) - 내장 송풍팬 폴트신호, 자체 정상출력후 점검해야함
//           BACH - APS(TC,M2,M3 CAR)의 출력전압으로 HVAC가 동작하므로 APS(TC,M2,M3)가 기동전에는 폴트처리하지 말고(판단은??)
//                                                                                     재기동시에도 폴트처리하지 말아야 함(판단??)
//                 --> 혹시 차량에서 고려해서 신호 보내주는지 확인하고, 차량에서 고려해서 신호보낼시 즉시 폴트처리하면됨.
//=========================================================================================================================//     
// 4. 수동리셋시  --> 1회 재기동(GRF, FUSEF는 제외 <- 1회재기동 안함)
//    ResetRestart
//    4.1 GRF
//    4.2 FUSEF
//==============================================================================//
/*
LFaultAck;		// 1. 경고장 
MFaultAck;		// 2. 경중고장
HFaultAck;		// 3. 중고장 

// GDF관련 변수
MFCounter1;		//GDF1,2,3,4,5 발생횟수 
MFTimer11;		//GDFs 첫번째 경과시간  
MFTimer12;		//GDFs 두번째 경과시간  

MFCounter2;		//입력과전압 폴트 발생횟수
MFTimer21;		//입력과전압 폴트 첫번? 경과시간 
MFTimer22;		//입력과전압 폴트 두번째 경과시간  

MFCounter3;		//입력과전류 폴트 발생횟수
MFTimer31;		//입력과전류 폴트 경과시간  
MFTimer32;		//입력과전류 폴트 경과시? 

MFCounter4;		//출력과전압 폴트 발생횟수
MFTimer41;		//출력과전압 폴트 경과시간  
MFTimer42;		//출력과전압 폴트 경과시간  

MFCounter5;		//출력과전류 폴트 발생횟수
MFTimer51;		//출력과전류 폴트 경과시간  
MFTimer52;		//출력과전류 폴트 경과시간  

MFCounter6;		//과부하 폴트 발생횟수
MFTimer61;		//과부하 폴트 경과시간  
MFTimer62;		//과부하 폴트 경과시간  
*/



//==============================================================================//
// 2. 중고장 --> 3분이내 3회 동일고장시 중고장 처리
//    MidiumLevleFault(MF)
//    2.1 GDF
//    2.2 입력과압, 입력과전류, 출력과전압, 출력과전류
//    2.3 과부하 <- 120% 1분이상 지속시
//    2.4 기타 폴트
//==============================================================================//
// FCounter : fault 횟수
// FTtimer1 : 뭐枰? 폴트후 경과시간
// FTtimer2 : 두번째 폴트후 경과시간
// Fmjf : 중고장발생(1), 중고장 아님(0)
//==============================================================================//
void MidiumFault_3min3N_Check_sub(Uint16 *FCounter, Uint16 *FTimer1, Uint16 *FTimer2, Uint16 *Fmjf)
{
	unsigned int fcnt, ftmr1, ftmr2, fmjff;
	//unsigned int MIN3Temp = 6000;	
	unsigned int MIN3Temp = 18000;	
	
	fcnt = *FCounter;
	ftmr1 = *FTimer1;
	ftmr2 = *FTimer2;
	fmjff = *Fmjf;
	
	if(fcnt == 1){//폴트가 1회면
		ftmr1++;
		if(ftmr1 >= MIN3Temp){	// 폴트가 1번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 처리 안하고, 폴트누적 클리어
			ftmr1 = 0;	//폴트타이머 클리어
			fcnt = 0;	// 폴트카운트 하나 감소	
			//fmjff = 0;	// 중고장 없슴					
		}
	}
	if(fcnt == 2){//폴트가 2회면
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// 폴트가 2번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 아니고, 폴트누적 1감소
			ftmr1 = ftmr2;	//
			fcnt = 1;	// 폴트카운트 하나 감소		
			ftmr2 = 0;	//폴트타이머2 클리어
			//fmjff = 0;	// 중고장 없슴					
		}
	}		
	if(fcnt == 3){//폴트가 3회면 중고장 검사
		ftmr1++;
		ftmr2++;		
		if(ftmr1 >= MIN3Temp){	// 폴트가 2번걸렸는데,,,첫번째 폴트발생후 3분이 지났으면 중고장 아니고, 폴트누적 1감소
			ftmr1 = ftmr2;	//
			fcnt = 2;	// 폴트카운트 하나 감소		
			ftmr2 = 0;	//폴트타이머2 클리어
			//fmjff = 0;	// 중고장 없슴					
		}
		else{		//폴트 3회고 첫번째 폴트발생후  3분미만이면 중고장 처리
			ftmr1 = 0;	//폴트타이머 클리어	
			ftmr2 = 0;	//폴트타이머 클리어		
			fmjff = 1;	// 중고장 발생					
		}
	}	
	
	*FCounter = fcnt;
	*FTimer1 = ftmr1;
	*FTimer2 = ftmr2;
	*Fmjf = fmjff;
			
}

void HighLevelFault_Check_TEST()
{
	;;
}
void LowLevelFault_Check_TEST()
{
	;;
}
void ResetSW_Check_TEST()
{

	;;
}
void MidiumFault_3min3N_Check()
{
	MidiumFault_3min3N_Check_sub(&MidFCounter0, &MidFTimer01, &MidFTimer02, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter1, &MidFTimer11, &MidFTimer12, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter2, &MidFTimer21, &MidFTimer22, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter3, &MidFTimer31, &MidFTimer32, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter4, &MidFTimer41, &MidFTimer42, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter5, &MidFTimer51, &MidFTimer52, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter6, &MidFTimer61, &MidFTimer62, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter7, &MidFTimer71, &MidFTimer72, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter8, &MidFTimer81, &MidFTimer82, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter9, &MidFTimer91, &MidFTimer92, &MidFaultMJF);	
	MidiumFault_3min3N_Check_sub(&MidFCounter10, &MidFTimer101, &MidFTimer102, &MidFaultMJF);
	MidiumFault_3min3N_Check_sub(&MidFCounter11, &MidFTimer111, &MidFTimer112, &MidFaultMJF);
}
void MidiumFault_CountCheck_TEST()
{
		if(btst(MidFaultAck,MidFaultAck0)){	// BIT 0 		
			MidFCounter0++;
		}
		if(btst(MidFaultAck,MidFaultAck1)){	// BIT 1 
			MidFCounter1++;
		}
		if(btst(MidFaultAck,MidFaultAck2)){	// BIT 2 
			MidFCounter2++;
		}
		if(btst(MidFaultAck,MidFaultAck3)){	// BIT 3 
			MidFCounter3++;
		}	
		if(btst(MidFaultAck,MidFaultAck4)){	// BIT 4 
			MidFCounter4++;
		}
		if(btst(MidFaultAck,MidFaultAck5)){	// BIT 5
			MidFCounter5++;
		}
		if(btst(MidFaultAck,MidFaultAck6)){	// BIT 6
			MidFCounter6++;
		}
		if(btst(MidFaultAck,MidFaultAck7)){	// BIT 7
			MidFCounter7++;
		}
		if(btst(MidFaultAck,MidFaultAck8)){	// BIT 7
			MidFCounter8++;
		}
		if(btst(MidFaultAck,MidFaultAck9)){	// BIT 9
			MidFCounter9++;
		}
		if(btst(MidFaultAck,MidFaultAck10)){	// BIT 9
			MidFCounter10++;
		}
		if(btst(MidFaultAck,MidFaultAck11)){	// BIT 9
			MidFCounter11++;
		}
}

void MidiumFault_3min3N_Check_TEST_INIT()
{
	//==3분3회 동일고장 폴트처리 테스트변수===
	MidFaultAck = 0;
	LowFaultAck = 0;
	HighFaultMJF = 0;
		
	MidFaultMJF = 0;

	MidFCounter0 = 0;	//GDF1,2,3,4,5 발생횟수 
	MidFTimer01 = 0;		//GDFs 첫번째 경과시간  
	MidFTimer02 = 0;		//GDFs 두번째 경과시간  

	MidFCounter1 = 0;	//입력과전압 폴트 발생횟수
	MidFTimer11 = 0;		//입력과전압 폴트 첫번째 경과시간 
	MidFTimer12 = 0;		//입력과전압 폴트 두번째 경과시간  

	MidFCounter2 = 0;	//입력과전류 폴트 발생횟수
	MidFTimer21 = 0;		//입력과전류 폴트 첫번째 경과시간  
	MidFTimer22 = 0;		//입력과전류 폴트 두번째 경과시간  

	MidFCounter3 = 0;	//출력과전압 폴트 발생횟수
	MidFTimer31 = 0;		//출력과전압 폴트 첫번째 경과시간 
	MidFTimer32 = 0;		//출력과전압 폴트 두번째 경과시간  

	MidFCounter4 = 0;	//출력과전류 폴트 발생횟수
	MidFTimer41 = 0;		//출력과전류 폴트 첫번째 경과시간 
	MidFTimer42 = 0;		//출력과전류 폴트 두번째 경과시간  

	MidFCounter5 = 0;	//과부하 폴트 발생횟수
	MidFTimer51 = 0;		//과부하 폴트 두번째 첫번째 경과시간 
	MidFTimer52 = 0;		//과부하 폴트 두번째 경과시간    

	MidFCounter6 = 0;	//FCV(DCLINK VOLTAGE) 과전압 폴트 발생횟수
	MidFTimer61 = 0;		//FCV(DCLINK VOLTAGE) 과전압 폴트 첫번째 경과시간 
	MidFTimer62 = 0;		//FCV(DCLINK VOLTAGE) 과전압 폴트 두번째 경과시간  

	MidFCounter7 = 0;	//FCI(DCLINK CURRENT) 과전류 폴트 발생횟수
	MidFTimer71 = 0;		//FCI(DCLINK CURRENT) 과전류 폴트 첫번째 경과시간 
	MidFTimer72 = 0;		//FCI(DCLINK CURRENT) 과전류 폴트 두번째 경과시간 
	
	MidFCounter8 = 0;	//제어전원 저전압 폴트 발생횟수
	MidFTimer81 = 0;		//제어전원 저전압 폴트 첫번째 경과시간 
	MidFTimer82 = 0;		//제어전원 저전압 폴트 두번째 경과시간  

	MidFCounter9 = 0;	//팬폴트 임시로 중간고장으로 처리 폴트 발생횟수
	MidFTimer91 = 0;		//팬폴트 첫번째 경과시간 
	MidFTimer92 = 0;		//팬폴트 두번째 경과시간  

	MidFCounter10 = 0;	//팬폴트 임시로 중간고장으로 처리 폴트 발생횟수
	MidFTimer101 = 0;		//팬폴트 첫번째 경과시간 
	MidFTimer102 = 0;		//팬폴트 두번째 경과시간  
	
	MidFCounter11 = 0;	//팬폴트 임시로 중간고장으로 처리 폴트 발생횟수
	MidFTimer111 = 0;		//팬폴트 첫번째 경과시간 
	MidFTimer112 = 0;		//팬폴트 두번째 경과시간  
	
}
