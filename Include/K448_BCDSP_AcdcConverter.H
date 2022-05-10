#ifndef __ACDCPLL_H
#define __ACDCPLL_H


#define LPF1_Ts	200e-6//수정
#define APF_Ts	200e-6	//수정
#define PLL_Ts	200e-6	//수정
#define	Iac_Ts	200e-6	//수정
#define Vdc_Ts	200e-6	//수정


#define FREQ_50hz	50
#define FREQ_60hz	60
#define FREQ_2hz	2
//float Theta_delay_pll = 400500.;

#define PHASE_DELAY		666000		// 4.44msec 
// timer0_cnt로 계산 -> 각도변경이 보상한 곳에서 일어남

var unsigned int PD_Counter;
var unsigned int PLLAck;
//	Theta_pll

//========ac/dc controller 관련변수선언=======//
//void BachDcDcController();
//void InitBachDcDcControlVar();
//=================================================//

var float	acdcInCurKp;
var float	acdcInCurKi;
var float	acdcInCurKt;
var float	acdcInCurLimit;
var float	acdcInCurRef;	    // 전류지령 
var float	acdcInCurLPF;	    // 검출전류 LPF/rms?
var float	acdcInCurPI;	    // pi제어기 출력
var float	acdcInCurI;	    // pi제어기 출력
var float	acdcInCur_Err;     // pi제어기 에러

var float	acdcOutCurKp;
var float	acdcOutCurKi;
var float	acdcOutCurLimit;

var float	acdcOutCurRef;	// 전류지령 
var float	acdcOutCurLPF;	// 검출전류 LPF
var float	acdcOutCurPI;	    // pi제어기 출력

var float	acdcOutVolKp;
var float	acdcOutVolKi;
var float	acdcDutyLimit;
var float	acdcOutVolLimit;
var float	acdcOutVolKdamp;
var float	acdcOutVolSampTime;

var Uint16	acdcIacControCounter;	// acdc전류제어기 카운터(10번 전류제어하면 전압제어기 1번)


var float	acdcOutVolRef;	// 전압지령 (dclink전압)

var float	acdcOutVolRefSet;
var float	acdcOutVolLPF;	// 검출전압 LPF(dclink전압)
var float	acdcOutVolPI;	    // pi제어기 출력
var float	acdcOutVolI;		// pi제어기 적분치
var float	acdcOutVol_Err;     // pi제어기 에러

var float	acdcVacRef;

var float	acdcDutyRatio;
var Uint16	acdcDutyCount;
var float	acdcDutyCountValue;
//var float	DcLinkVolRef;
	//--- 라인 주파수-----< 어느정도 평균값 취하고, 외부 인터럽트 루틴(xint1, zerocrossing int)에서 읽음
var float LineFreqCounter;	// = ReadCpuTimer1Period() - ReadCpuTimer1Counter();
var float LineFreq;// = SYS_CLK/LineFreqCounter;
var float LineFreqAvg;
var float LineFreqSum;
var float LineFreqAvgOld;
var float LineFreqAvgNew;


var float Vac_Value_Compensation;
var float E_Compensation; 
var float Vac_Value_LPF1;
var float PhaseAngleToRadian;
var float Phase_Angle;
var float APF_Cutoff_Freq;
var float APF_Coeff_K1;
var float APF_Coeff_K2;
var float Vac_APF_DS_Old;
var float Vac_APF_QS_Old;
var float Vac_DS;
var float Vac_QS;
var float Vac_Hat_W;
var float Vac_Hat_Theta;
var float Vac_Hat_E;
var float Vac_CosTheta;
var float Vac_SinTheta;
var float Vac_DE;
var float Vac_QE;
var float Vac_Theta_Error;
var float Vac_Kp_PLL;
var float Vac_Ki_PLL;
var float Vac_ICon_PLL;
var float Vac_PICon_PLL;
var float Vac_Delta_W;
var float W_Reference;
var float Vac_Compensation_Theta;
var float Vac_Hat_Theta_Comp;
var float Vac_Comp;
var float Unit_Sine;
var float Unit_Sine_preComp;
var float Unit_Sine_preCompValue;
var float Iac_Ref;
var float Iac_Peak;
var float Vac_Ref;
var float Vac_Peak;
var float Vac_Hat_E_LPF1;
var float Vac_Hat_E_Compensation;
var float Vac_Hat_E_Comp;
var float Vac_Peak_Error;
var float Vac_Kp_Peak;
var float Vac_Ki_Peak;
var float Vac_ICon_Peak;
var float Vac_PICon_Peak;


var float Iac_DE;
var float Iac_QE;
var float Iac_APF_DS_Old;
var float Iac_APF_QS_Old;
var float Iac_DS;
var float Iac_QS;
var float Vac_DE_PLL;
var float Vac_QE_PLL;




void SingleINVControl();
void Vac_APF(float Vac_APF_Input, float *Vac_APF_DS, float *Vac_APF_QS);
void Vac_PLL(float Vac_DS_Input, float Vac_QS_Input);
void VacIac_APF(float Vac_APF_Input, float *Vac_APF_DS, float *Vac_APF_QS, float Iac_APF_Input, float *Iac_APF_DS, float *Iac_APF_QS);
void VacIac_PLL(float Vac_DS_Input, float Vac_QS_Input, float Iac_DS_Input, float Iac_QS_Input);

void InitAcDcControlVar();
void InitAcDcPllVariable();
void AcDcConController();
void SingleInputVoltagePll_Check();
void AcDcInVolPLL();
void AcdcConSoftStart();
#endif

