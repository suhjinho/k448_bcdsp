//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) dc/dc 컨버터 제어기           		//
//==================================================================================//
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//
#include "Global_Variable_Function.h"

//==================================================================================//
// Rated DCLINK VOLTAGE : 700VDC
// Rated Output VOLTAGE : 100VDC
// 스위칭 방식 : Hard switching 
// 스위칭 주파수 : 5khz
//==================================================================================//
// 트랜스포머 사양                         설계:팩테크, 제작 : (주)휴렉스           //
//==================================================================================//
// 용량 : 45kva *2ea 1차,2차 병렬구성                                               //
// 턴수비 = N1/N2 = V1/V2 = 597/115 = 5.1913 : 1   =0.1926(V2/V1)                   //
// 1차전압(597v), 2차전압(115v-0-115v)-2차측 센터탭방식                             //
//==================================================================================//
// 2차측 리액터 사양                       설계:팩테크, 제작 : (주)휴렉스           //
// 용량 : 400A, 50mH *2ea , 트랜스포머 정류후                                       //
//==================================================================================//
//=======파워회로/PWM==============================================================
// FB(Full-Bridge) dc/dc컨버터 hardswitching
// PWM1A(Q1), PWM1B(Q2), PWM2A(Q4), PWM2B(Q3)
//=================================================================================
// 회로구성 및 설계자료 : 설계자료 및 도면참고                                      //
//==================================================================================//
//unsigned int vol_count = 0;
//==================================================================================//
// BACH DC/DC Controller															//
// 함수명 : BachDcDcController()	    											//
// calling : Timer Int
// switching 주파수 : 트랜스 사양(5kHZ)으로 
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//
void BachDcDcController()
{
	float bcOutVol_KI_damp = 1.0;

	bc_vol_cnt++;
	bcOutVol_pi_Err = bcOutVolRef - BachOutVol_LPF;
	
	bcOutVol_KI_damp = 1.0;
//20190620 voltage controller change	PI_Damp_Controller((bcDutyLimit*0.9),-1., Ts, bcOutVol_KI_damp, bcOutVolKp, bcOutVolKi, bcOutVolRef, BachOutVol_LPF, &bcOutVol_pi_I, &bcOutVolPI);	
//	PI_Damp_Controller((bcDutyLimit*0.9),-(bcDutyLimit*0.9), Ts, bcOutVol_KI_damp, bcOutVolKp, bcOutVolKi, bcOutVolRef, BachOutVol_LPF, &bcOutVol_pi_I, &bcOutVolPI);
//	if(bcOutVolPI > bcDutyLimit*0.9)			bcOutVolPI = bcDutyLimit*0.9;
// 220226 외부수전 시 출력전압 낮음으로 변경  bcDutyLimit*0.9 --> 0.95 
	PI_Damp_Controller((bcDutyLimit*0.95),-(bcDutyLimit*0.95), Ts, bcOutVol_KI_damp, bcOutVolKp, bcOutVolKi, bcOutVolRef, BachOutVol_LPF, &bcOutVol_pi_I, &bcOutVolPI);
	if(bcOutVolPI > bcDutyLimit*0.95)			bcOutVolPI = bcDutyLimit*0.95;

	if(bcOutVolPI < 0.)			bcOutVolPI = 0.;//-bcDutyLimit;
	
	if(BachOutVol_LPF > 110.){
		if((bcOutVolPI > 0) &&(BachOutCur_LPF > 10.))
		{
			bcOutVol_pi_I = bcOutVol_pi_I *0.95;
			bcOutVolPI = bcOutVolPI *0.95;
		}
		else if((bcOutVolPI > 0) &&(BachOutCur_LPF <= 10.))
		{
			bcOutVol_pi_I = bcOutVol_pi_I *0.1;
			bcOutVolPI = bcOutVolPI *0.1;
		}
	}
	if(BachOutCur_LPF > 350.)
	{
		if(bcOutVolPI > 0)
		{
			bcOutVol_pi_I = bcOutVol_pi_I *0.99;
			bcOutVolPI = bcOutVolPI *0.99;
		}
	}
	
  	EPwm1Regs.DBRED = DEADTIME1_RED;    //Set rising  edge delay 
  	EPwm1Regs.DBFED = DEADTIME1_FED;
	EPwm2Regs.DBRED = DEADTIME2_RED;
	EPwm2Regs.DBFED = DEADTIME2_FED;


	bcDutyRatio = bcOutVolPI/bcDutyLimit;
	
	if(TEST_DEADTIME_DCDC == 1)
	{
	bcDutyRatio = Test_bcDutyRatio;
	}		
	
	bcDutyCount = (Uint16)(bcDutyRatio*EPWM_DCDC_TBPRD_HALF);

	if(bcDutyCount > (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC))
	{	// 데드타임보다 크지 않게..
		bcDutyCount = (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC);
	}

	EPwm2Regs.CMPA.half.CMPA = bcDutyCount;	
	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
	
}



void BachDcDcController1()
{
	/*
	if(vol_count > 20){
		vol_count = 0;
		bcOutVol_pi_Err = bcOutVolRef - bcOutVolLPF;
		PI_Damp_Controller(1000., Ts*20, 1.0, bcOutVolKp, bcOutVolKi, bcOutVolRef, bcOutVolLPF, &bcOutVol_pi_I, &bcOutVolPI);	

	    bcOutVolPI = bcOutVolPI*0.1;
		if(bcOutVolPI > 100.)			bcOutVolPI = 100.;
		if(bcOutVolPI < 0)			bcOutVolPI = 0.;//-bcDutyLimit;
		
	}

	BachBatCur_pi_Err = bcOutVolPI - BachBatCur;//20120118 전류제어기 추가(기존 )
	PI_Damp_Controller(90., Ts, 1.0, bcOutCurKp, bcOutCurKi,	bcOutVolPI, BachBatCur, &BachBatCur_pi_I, &BachBatCurPI);	

	if(BachBatCurPI > 90)				    BachBatCurPI = 90;
	if(BachBatCurPI < 0)		BachBatCurPI = 0;
		
	bcDutyRatio = BachBatCurPI/90.;

	if(bcDutyRatio > bcDutyLimit)     bcDutyRatio = bcDutyLimit;
	if(bcDutyRatio < 0.)              bcDutyRatio = 0.;


	bcDutyCount = (Uint16)(bcDutyRatio*EPWM_DCDC_TBPRD_HALF);

	if(bcDutyCount > (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC)){	// 데드타임보다 크지 않게..
		bcDutyCount = (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC);
	}

	EPwm1Regs.CMPA.half.CMPA = bcDutyCount;
	EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
	EPwm1Regs.ETCLR.bit.INT = 1;

	EPwm2Regs.CMPA.half.CMPA = bcDutyCount;
	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
	EPwm2Regs.ETCLR.bit.INT = 1;	
	*/
}

void BachDcDcController22()
{
	/*
	vol_count++;

	if(vol_count > 20){
		vol_count = 0;
		bcOutVol_pi_Err = bcOutVolRef - bcOutVolLPF;
		PI_Damp_Controller(1000., Ts*20, 1.0, bcOutVolKp, bcOutVolKi, bcOutVolRef, bcOutVolLPF, &bcOutVol_pi_I, &bcOutVolPI);	
	    bcOutVolPI = bcOutVolPI*0.1;
		if(bcOutVolPI > 100.)			bcOutVolPI = 100.;
		if(bcOutVolPI < 0)			bcOutVolPI = 0.;//-bcDutyLimit;
		
	}

	BachBatCur_pi_Err = bcOutVolPI - BachBatCur;//20120118 전류제어기 추가(기존 )
	PI_Damp_Controller(90., Ts, 1.0, bcOutCurKp, bcOutCurKi,	BachBatCurRef, BachBatCur, &BachBatCur_pi_I, &BachBatCurPI);	

	if(BachBatCurPI > 90)				    BachBatCurPI = 90;
	if(BachBatCurPI < 0)		BachBatCurPI = 0;
		
	bcDutyRatio = BachBatCurPI/90.;
		    

	if(bcDutyRatio > bcDutyLimit)     bcDutyRatio = bcDutyLimit;
	if(bcDutyRatio < 0.)              bcDutyRatio = 0.;

	//=======PWM 값 계산========================================================
	//= FB(Full-Bridge) dc/dc컨버터 hardswitching
	//= PWM1A(Q1), PWM1B(Q2), PWM2A(Q4), PWM2B(Q3)
	//==========================================================================

	bcDutyCount = (Uint16)(bcDutyRatio*EPWM_DCDC_TBPRD_HALF);

	if(bcDutyCount > (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC)){	// 데드타임보다 크지 않게..
		bcDutyCount = (EPWM_DCDC_TBPRD_HALF - DEADTIME_DCDC);
	}

	EPwm1Regs.CMPA.half.CMPA = bcDutyCount;
	EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
	EPwm1Regs.ETCLR.bit.INT = 1;

	EPwm2Regs.CMPA.half.CMPA = bcDutyCount;
	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF - bcDutyCount;
	EPwm2Regs.ETCLR.bit.INT = 1;	
	*/
}




//==================================================================================//
// 함수명 : BcConSoftStart()	    											//
// calling : Timerint1                                                                //
// BACH DC/DC Softstart                                 //
// DATE : 2010. 08. 16            						  by PACTECH R&D LSH		//
//==================================================================================//
void BcConSoftStart1()
{
//	float ref;
//	float ssCnt; // SoftStart count = ss_msec*1000/dt(usec)
	//DCDC_SoftStart_flag = NO;  // test
	
	if(DCDC_SoftStart_flag == YES){		//제어기,Ramp, pwm_ON
		if(bcOutVolRef < bcOutVolRefSet){		//acdcOutPutVolRefSet = RatedDclinkVoltage;
			if(bcOutVolRef > bcOutVolRefSet){
				bcOutVolRef = bcOutVolRefSet;
			}
			else if(bcOutVolRef < bcOutVolRefSet){			
				//bcOutVolRef = bcOutVolRef + 0.01176; // APS : (750Vdc-456Vdc)/5sec = dv = (v2-v1)/ssCnt; // 750-456/50000msec
				//bcOutVolRef = bcOutVolRef + 0.00176; // APS : (750Vdc-456Vdc)/5sec = dv = (v2-v1)/ssCnt; // 750-456/50000msec
				bcOutVolRef = bcOutVolRef + 0.00076; // APS : (750Vdc-456Vdc)/5sec = dv = (v2-v1)/ssCnt; // 750-456/50000msec
			                                                   // bach : (700Vdc-456Vdc)/5sec = dv = (v2-v1)/ssCnt; // 750-456/50000msec
			//	bcOutVolRef = bcOutVolRef + 0.0040; //BACH 임의로...우선
			//	bcOutVolRef = bcOutVolRef + 0.0040; //BACH 임의로...우선

			}
			else{
				bcOutVolRef = bcOutVolRefSet;
			}
		}
		else{
			DCDC_SoftStart_flag = NO;	//DCDC CON Soft_start==>SET	
		}
	}
	else{
		bcOutVolRef = bcOutVolRefSet;		
	}
}


void BcConSoftStart()
{
	float bcOutVol_dINCDEC;
	/*
	if((bcOutVolRefSet - bcOutVolLPF) > 1.0){
		 bcOutVol_dINCDEC = 0.00376;
		 DCDC_SoftStart_flag = YES;
	}
	else if((bcOutVolRefSet - bcOutVolLPF) < 1.0){
		 bcOutVol_dINCDEC = 0.00376;
		 DCDC_SoftStart_flag = YES;
	}
	else {
		bcOutVol_dINCDEC = 0.0;
		DCDC_SoftStart_flag = NO;
	}


	if(DCDC_SoftStart_flag == YES){		//제어기,Ramp, pwm_ON
		if(bcOutVolRef < bcOutVolRefSet){
			bcOutVolRef = bcOutVolRef + bcOutVol_dINCDEC; // 
			if(bcOutVolRef >= bcOutVolRefSet){
				bcOutVolRef = bcOutVolRefSet;
				DCDC_SoftStart_flag = NO;
			}
		}
		else if(bcOutVolRef >= bcOutVolRefSet){
			bcOutVolRef = bcOutVolRef - bcOutVol_dINCDEC; // 
			if(bcOutVolRef <= bcOutVolRefSet){
				bcOutVolRef = bcOutVolRefSet;
				DCDC_SoftStart_flag = NO;
			}
		}
		else{
			bcOutVolRef = bcOutVolRefSet;
			DCDC_SoftStart_flag = YES;
		}
	}
	else{
		bcOutVolRef = bcOutVolRefSet;		
	}
	*/
	bcOutVol_dINCDEC =  0.004;
	if(DCDC_SoftStart_flag == YES){
		if(bcOutVolRef <= bcOutVolRefSet){
			bcOutVolRef = bcOutVolRef + bcOutVol_dINCDEC;
		}
		else{
			bcOutVolRef = bcOutVolRefSet;
			DCDC_SoftStart_flag = NO;
		}
	}
}
//==================================================================================//
// 함수명 : InitBachDcDcControlVar()	    										//
// calling : ??????                                                                 //
// 제어기 관련 변수 초기화   (init or   FaultAck  )                                 //
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//
void InitBachDcDcControlVar()
{
	bc_vol_cnt = 6; 
	bcOutVolKp = 2.;	
	bcOutVolKi = 200.;	
//20190622 	bcOutVolRef = -5.;	
	bcOutVolRef = 0.;
	
	bcOutVolPI = 0.;	    
	bcOutVol_pi_I = 0.;
	bcOutVol_pi_Err = 0.;
	
	bcOutCurKp = 3.;
	bcOutCurKi = 50.;
	bcOutCurRef = 0.;	    
	
	bcOutCurPI = 0.;	    
	bcOutCur_pi_I = 0.;
	bcOutCur_pi_Err = 0.;
	
	bcDutyRatio = 0.;
	bcDutyCount = 0;
	
	
	bcOutCurLimit = 310.;
	bcDutyLimit = 1000;	
	
	bcOutVolRefSet = 98.0;	              //211106 K448 일산선 BC HS 온도 90도 초과로 100->99.5 SUHJINHO
										  //220506 K448 일산선 BC DC OUTPUT 리플 측정시 MIN값 100V 초과하여 하향 
/*------------------------*/	
	EPwm1Regs.CMPA.half.CMPA = 0;    // Set compare A value init
	EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF;              // Set Compare B value init
	EPwm2Regs.CMPA.half.CMPA = 0;	
	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF;	
	
	//EPwm2Regs.CMPA.half.CMPA = 0;
//	EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF;
	//EPwm2Regs.ETCLR.bit.INT = 1;		
	
	DCDC_SoftStart_flag = 0;
}
//=================================================================//


