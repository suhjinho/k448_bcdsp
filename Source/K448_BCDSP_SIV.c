//==================================================================================//
// 차세대 고속철도(NTHS) APS 3상인버터 제어기                                 		//
// 인버터는 기존 자기부상열차 SIV제어기부분 그대로 사용                             //
// (가급적 함수명, 변수명도 같이사용)                                               // 
//===================================================================================//
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//

#include "Global_Variable_Function.h"


//==================================================================================//
// APS 3상 PLL																		//
// 함수명 : SivPllRefTrans()														//
// calling : Timer Int
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//
void SivPllRefTrans()
{
	float Temp = 0.;
	//---------------------------------------------------------------
	// 전압 clarke 변환==>3상을 2상으로 변환
	//---------------------------------------------------------------
	siv_vclarke.As = ApsOutVolU; //lsh. sivUphaseVol;
	siv_vclarke.Bs = ApsOutVolV; //lsh. sivVphaseVol;
	siv_vclarke.calc(&siv_vclarke);
	
	// 전압 모니터링
	sivVolMag = sqrt(siv_vclarke.Alpha * siv_vclarke.Alpha + siv_vclarke.Beta * siv_vclarke.Beta);
	sivInvVolMag = (1./sivVolMag);	            //사용안함
	sivVolRms = 0.707 * sivVolMag;				// RMS 전압 = Peak/sqrt(2)
	sivVolLLRms = sivVolRms * 1.732;            // 선간전압 = 상전압*sqrt(3) 
	
	
	//---------------------------------------------------------------
	// 전류 clarke 변환, 전류 최대값, 실효값 계산
	//---------------------------------------------------------------
	siv_iclarke.As = ApsOutCurU;// lsh. sivUphaseCurr;
	siv_iclarke.Bs = ApsOutCurV;// lsh. sivVphaseCurr;
	siv_iclarke.calc(&siv_iclarke);
	
	// 전류 모니터링
	sivCurrMag = sqrt(siv_iclarke.Alpha*siv_iclarke.Alpha + siv_iclarke.Beta*siv_iclarke.Beta);
	sivCurrRms = 0.707 * sivCurrMag;				// RMS 전류 = Peak/sqrt(2)
	// ---------------------------------------------------------------
	// Grid voltage PLL
	// ---------------------------------------------------------------  
    siv_pll_pi.Kp = sivPllKp;
	siv_pll_pi.Ki = sivPllKi;
    siv_pll_pi.Kdamp = 1.;
	siv_pll_pi.OutMax = sivRatedAngleVel;	// w = 2*pi*f(60)
    siv_pll_pi.OutMin = -sivRatedAngleVel;
    siv_pll_pi.SampTime = Ts;
    // PI controller for the PLL
    siv_pll_pi.Ref = 0;
	siv_pll_pi.Fdb = siv_vpark.DsLPF;
	//gv_pll_pi.Fdb = gv_park.Ds;
	siv_pll_pi.calc(&siv_pll_pi);
	    				
	// 정격 전원 각속도 값으로 피드포워드
	Temp = siv_pll_pi.Out + sivRatedAngleVel;				
		
	sivThetaPre	+= Temp*Ts;
	if(sivThetaPre > PI)	sivThetaPre -= PI2;
	if(sivThetaPre < -PI)	sivThetaPre += PI2;

	sivTheta = sivThetaPre;//+GridThetaComp;
	
	if(sivTheta > PI)	sivTheta -= PI2;
	if(sivTheta < -PI)	sivTheta += PI2;
			
	//--------------------------------------
	// Grid voltage park transformation ==> 2상 고정->회전
	//--------------------------------------
	siv_vpark.Alpha = siv_vclarke.Alpha;
	siv_vpark.Beta	= siv_vclarke.Beta;
	siv_vpark.Angle = sivTheta;
	siv_vpark.calc(&siv_vpark);
	
	// Low Pass Filter
	LPF1(Ts, sivRatedAngleVel*0.5, siv_vpark.Ds, &siv_vpark.DsLPF);
	LPF1(Ts, sivRatedAngleVel*0.5, siv_vpark.Qs, &siv_vpark.QsLPF);
	
	//--------------------------------------
	// Grid current park transformation
	//--------------------------------------
	siv_ipark.Alpha = siv_iclarke.Alpha;
	siv_ipark.Beta	= siv_iclarke.Beta;
	
	siv_ipark.Angle = sivTheta;
	siv_ipark.calc(&siv_ipark);
}

//==================================================================================//
// APS SIV Controller															//
// 함수명 : sivController()														//
// calling : Timer Int
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//
float sivTestVolRef = 0;
float sivTestVolLimit;
void sivController()
{
	float TempOutVolfb;
	//---------------------------------------
	// SIV 전압 제어기
	//---------------------------------------
	siv_vpi.Kp = sivVolKp;
	siv_vpi.Ki = sivVolKi;
    siv_vpi.Kdamp = 1.;
	siv_vpi.OutMax = sivVolLimit;
    siv_vpi.OutMin = -sivVolLimit;
    siv_vpi.SampTime = Ts;
    // PI 제어기
	siv_vpi.Ref = sivVolRef;
	siv_vpi.Fdb = siv_vpark.QsLPF;//20120202 원래 feedback전압값 계산(dq) 
	
	
	//20120202 CM구동시 출력전압값이 460V넘게 전반적으로 상승하기 때문에
	//DQ로 검출한것 사용안하고 RMS로 계산한것을 FEEDBACK전압값으로 사용함
	//---->가선이 내려가서(11시30분에 가선 자동차단됨) 적용은 내일 TC APS에 적용예정
	//TempOutVolfb = (fabs(ApsOutVolULLRms)+fabs(ApsOutVolVLLRms)+fabs(ApsOutVolWLLRms))/3.0;		//rms로 계산한 전압값, 3상평균
//	siv_vpi.Fdb = (TempOutVolfb/SQRT3)*SQRT2;
	
	
sivTestVolLimit = siv_vpi.Fdb;	//  이선희 테스트 추가,,	
	//siv_vpi.Fdb = siv3PhaseVolPeak;
	siv_vpi.calc(&siv_vpi);
	// 제어기 출력 제한기
//sivTestVolLimit = siv_vpi.Out;	// 이선희 테스트 추가,,
	if (siv_vpi.Out > sivVolLimit)			siv_vpi.Out = sivVolLimit;
	else if(siv_vpi.Out < -sivVolLimit)		siv_vpi.Out = -sivVolLimit;
	
/*---20110528, dcdcInvPwmModulation(YES)과 관계없이 항상 기준 주파수 발생을 하기위해 sivController() 바깥으로 뺌
	// SIV 주파수 발생
	sivThetaGen += sivRatedAngleVel*Ts;		
	if (sivThetaGen > PI)		sivThetaGen -= PI2;
	if (sivThetaGen < -PI)		sivThetaGen += PI2;
----------------*/
	//----------------------------------
	// Inverse Park
	//----------------------------------
	siv_ref_ipark.Ds = 0;
	siv_ref_ipark.Qs = siv_vpi.Out;	
	sivTestVolRef = siv_vpi.Out;
		
	//----------------------------------
	// Inverse Park
	//----------------------------------
	siv_ref_ipark.Ds = siv_ref_ipark.Ds;
    siv_ref_ipark.Qs = siv_ref_ipark.Qs;	
    siv_ref_ipark.Angle = sivThetaGen;
    siv_ref_ipark.calc(&siv_ref_ipark);
    
    sivVolRefMag = sqrt(siv_ref_ipark.Alpha*siv_ref_ipark.Alpha + siv_ref_ipark.Beta*siv_ref_ipark.Beta);
    //----------------------------------
    // 회전자 지령전압을 동기좌표계로
    //----------------------------------
    siv_ref_park.Alpha 	= siv_ref_ipark.Alpha;
	siv_ref_park.Beta	= siv_ref_ipark.Beta;
	siv_ref_park.Angle 	= sivThetaGen;
	siv_ref_park.calc(&siv_ref_park);
    //======================================================
	// Space Vector Modulation
	//======================================================
	siv_svpwm.Vdc 		= DClinkVol; //mdOutPutVol;	// lsh. dclink voltage --acdc outvoltage
  	siv_svpwm.Ualpha 	= siv_ref_park.Alpha; 
  	siv_svpwm.Ubeta 	= siv_ref_park.Beta;
  	siv_svpwm.calc(&siv_svpwm);
	//======================================================
	// Loading the duty count value to the EPWM
	//======================================================
	sivDutyCountU = (Uint16)(EPWM_INV_TBPRD*siv_svpwm.DutyU);
	sivDutyCountV = (Uint16)(EPWM_INV_TBPRD*siv_svpwm.DutyV);
	sivDutyCountW = (Uint16)(EPWM_INV_TBPRD*siv_svpwm.DutyW);
	
	sivDutyCountU = (sivDutyCountU > (EPWM_INV_TBPRD - DEADTIME_INV))? (EPWM_INV_TBPRD - DEADTIME_INV) : (sivDutyCountU < DEADTIME_INV)? DEADTIME_INV : sivDutyCountU;
	sivDutyCountV = (sivDutyCountV > (EPWM_INV_TBPRD - DEADTIME_INV))? (EPWM_INV_TBPRD - DEADTIME_INV) : (sivDutyCountV < DEADTIME_INV)? DEADTIME_INV : sivDutyCountV;
	sivDutyCountW = (sivDutyCountW > (EPWM_INV_TBPRD - DEADTIME_INV))? (EPWM_INV_TBPRD - DEADTIME_INV) : (sivDutyCountW < DEADTIME_INV)? DEADTIME_INV : sivDutyCountW;

	EPwm1Regs.CMPA.half.CMPA = sivDutyCountU;
	EPwm2Regs.CMPA.half.CMPA = sivDutyCountV;
	EPwm3Regs.CMPA.half.CMPA = sivDutyCountW;
	
	
}

void SivOutputVolRms()
{
	ApsOutVolUTime += Ts;
	RmsCalculationVoltage(60,&ApsOutVolUSum,&ApsOutVolUTime,ApsOutVolU, &ApsOutVolURms);
	
	ApsOutVolVTime += Ts;
	RmsCalculationVoltage(60,&ApsOutVolVSum,&ApsOutVolVTime,ApsOutVolV, &ApsOutVolVRms);
	
	ApsOutVolWTime += Ts;
	RmsCalculationVoltage(60,&ApsOutVolWSum,&ApsOutVolWTime,ApsOutVolW, &ApsOutVolWRms);
	
	ApsOutVolUVWRms = (ApsOutVolURms + ApsOutVolVRms + ApsOutVolWRms)/3;
	
	ApsOutVolUVWPeak = ApsOutVolUVWRms*1.414;
	ApsOutVolUVWLLRms = ApsOutVolUVWRms * 1.732;

	ApsOutVolULLRms = ApsOutVolURms * 1.732;
	ApsOutVolVLLRms = ApsOutVolVRms * 1.732;	
	ApsOutVolWLLRms = ApsOutVolWRms * 1.732;	
	
	ApsOutCurUTime += Ts;
	ApsOutCurVTime += Ts;
	ApsOutCurWTime += Ts;
	
	RmsCalculationVoltage(60,&ApsOutCurUSum,&ApsOutCurUTime,ApsOutCurU, &ApsOutCurURms);//UOC,U상출력전류(필터후)
	RmsCalculationVoltage(60,&ApsOutCurVSum,&ApsOutCurVTime,ApsOutCurV, &ApsOutCurVRms);//VOC,V상출력전류(필터후)	
	RmsCalculationVoltage(60,&ApsOutCurWSum,&ApsOutCurWTime,ApsOutCurW, &ApsOutCurWRms);//WOC,W상출력전류(필터후)	

}
//=================================================================================//
// 함수명 : SivSoftStart()                                                         //
// calling : Timerint1                                                             //
// APS SIV Softstart                                                               //
// 최초작성 : 2010. 08. 16                    by PACTECH R&D LSH                   //
// 수정작성 : 2012. 02. 2~3                    by PACTECH R&D LSH                  //
// 400v까지 softstrat하고 sivk확인후 약 2.5초후에 430v로 ref전압을 softstart로 올림//
//=================================================================================//
void SivSoftStart()
{
//	float ref;
//	float ssCnt; // SoftStart count = ss_msec*1000/dt(usec)
//	sivVolRefSet = (sivVolLtoL/SQRT3)*SQRT2;
	
	if(SIV_SoftStart_flag == YES){
		if(sivVolRef < sivVolRefSet){
			if(sivVolRef > sivVolRefSet){
				sivVolRef = sivVolRefSet;
				SIV_SoftStart_flag = NO;
			}
			else if(sivVolRef < sivVolRefSet){			
				sivVolRef = sivVolRef + 0.01176; // APS : (380Vrms-0)/5sec = dv = (v2-v1)/ssCnt; // 380-0/50000msec
			}
			else{
				sivVolRef = sivVolRefSet;
				SIV_SoftStart_flag = NO;
			}
		}
		else{
			sivVolRef = sivVolRefSet;
			SIV_SoftStart_flag = NO;
		}
	}
	else{
		sivVolRef = sivVolRefSet;		
	}
	
	if(SEQ_Step==SEQ_Normal_State){
		if(SIV_SoftStart_flag == NO){
			sivVolRefSet = (sivVolLtoLSet/SQRT3)*SQRT2;//	sivVolLtoLSet = 430;//기동완료후 전압올림
			SIV_SoftStart_flag = YES;
		}
	}	
}

//==================================================================================//
// 함수명 : InitApsSivContolVar()	    											//
// calling : ??????                                                                 //
// 제어기 관련 변수 초기화   (init or   FaultAck  )                                 //
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//
void InitApsSivContolVar()
{
	sivInvVolMag = 0;
	sivVolRms = 0;
	sivVolLLRms = 0;
	sivCurrMag = 0;
	sivCurrRms = 0;
	siv_park = 0;
	sivRatedAngleVel = 0;
	sivThetaPre = 0;
	sivTheta = 0;
	
	sivVolRef = 0;
	sivVolRefSet = 0;

	sivThetaGen = 0;
	sivVolRefMag = 0;
	sivVolLtoL = 0;
	sivVolMag = 0;

	sivDutyCountU = 0;
	sivDutyCountV = 0;
	sivDutyCountW = 0;
	
	ApsOutCurUTime = 0.;
	ApsOutCurVTime = 0.;
	ApsOutCurWTime = 0.;

	ApsOutCurUSum = 0.;
	ApsOutCurVSum = 0.;
	ApsOutCurWSum = 0.;

	ApsOutCurURms = 0.;
	ApsOutCurVRms = 0.;
	ApsOutCurWRms = 0.;

	ApsOutVolUTime = 0.;
	ApsOutVolUSum = 0.;
	ApsOutVolURms = 0.;

	ApsOutVolVTime = 0.;
	ApsOutVolVSum = 0.;
	ApsOutVolVRms = 0.;

	ApsOutVolWTime = 0.;
	ApsOutVolWSum = 0.;
	ApsOutVolWRms = 0.;
	ApsOutVolUVWRms = 0.;
	ApsOutVolUVWPeak = 0.;
	ApsOutVolUVWLLRms = 0.; 	// line-line voltage
	
	ApsOutCurU = 0.;
	ApsOutCurV = 0.;
	ApsOutCurW = 0.;
	

	sivPllKp = 20.;
	sivPllKi = 2000.;
	siv_pll_pi.Kp = sivPllKp;
	siv_pll_pi.Ki = sivPllKi;
    siv_pll_pi.Kdamp = 1.;
	siv_pll_pi.OutMax = sivRatedAngleVel;
    siv_pll_pi.OutMin = -sivRatedAngleVel;
    siv_pll_pi.SampTime = Ts;
    
	siv_pll_pi.Ui = 0;
	siv_pll_pi.Out = 0;
	siv_pll_pi.FF = 0;    
	//------------------------------------
	// SIV 출력 전압 제어기
	// 전압제어기 게인 및 출력 리미트
	//------------------------------------
	sivVolKp = 0.5;
	sivVolKi = 5.;

	//sivVolLimit = 445.;//20110901새벽 출력전압 높아서 450->445로 변경
	//sivVolLimit = 350.;//20120203새벽 출력전압 높아서 445->400로 변경
	//sivVolLimit = 345.;//20120203 출력전압 높아서 445->400로 변경
	sivVolLimit = 355.;//20120203새벽 출력전압 높아서 445->400로 변경(tc,m2,m3,m4 적용)
	sivVolLimit = 365.;//완성차시험시 출력전압 낮아서 높임(tc,m2만 적용)
					
	siv_vpi.Kp = sivVolKp;
	siv_vpi.Ki = sivVolKi;
    siv_vpi.Kdamp = 1.;
	siv_vpi.OutMax = sivVolLimit;
    siv_vpi.OutMin = -sivVolLimit;
    siv_vpi.SampTime = Ts;

	siv_vpi.Ui = 0;
	siv_vpi.Out = 0;
	siv_vpi.FF = 0;
	
	siv_svpwm.DutyU = 0;
	siv_svpwm.DutyV = 0;
	siv_svpwm.DutyW = 0;
	
	EPwm1Regs.CMPA.half.CMPA = 0;
	EPwm2Regs.CMPA.half.CMPA = 0;
	EPwm3Regs.CMPA.half.CMPA = 0;
	
	sivRatedFreq = 60;
	sivRatedAngleVel = PI2*60;

	//sivVolLtoLInit = 434;//340;//20110901새벽, 출력전압이 높아서..438->434
	sivVolLtoLInit = 400;//20120202..434-->400
	sivVolLtoLSet = 438;//기동완료후 전압올림,(tc,m2,m3,m4 적용)
	sivVolLtoLSet = 442;//완성차시험시 출력전압 낮아서 높임(tc,m2만 적용)

	//------------------------------------
	// 선간전압으로 부터 상전압 최대값 계산
	// SIV 전압 기준 값 계산
	// (Vll/sqrt(3))*sqrt(2)
	//------------------------------------	
	sivVolRefSet = (sivVolLtoLInit/SQRT3)*SQRT2;

	SIV_SoftStart_flag = 0;	
	
}





