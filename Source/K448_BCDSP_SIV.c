//==================================================================================//
// ������ ���ö��(NTHS) APS 3���ι��� �����                                 		//
// �ι��ʹ� ���� �ڱ�λ��� SIV�����κ� �״�� ���                             //
// (������ �Լ���, ������ ���̻��)                                               // 
//===================================================================================//
// DATE : 2010. 08. 10            						  by PACTECH R&D LSH		//
//==================================================================================//

#include "Global_Variable_Function.h"


//==================================================================================//
// APS 3�� PLL																		//
// �Լ��� : SivPllRefTrans()														//
// calling : Timer Int
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//
void SivPllRefTrans()
{
	float Temp = 0.;
	//---------------------------------------------------------------
	// ���� clarke ��ȯ==>3���� 2������ ��ȯ
	//---------------------------------------------------------------
	siv_vclarke.As = ApsOutVolU; //lsh. sivUphaseVol;
	siv_vclarke.Bs = ApsOutVolV; //lsh. sivVphaseVol;
	siv_vclarke.calc(&siv_vclarke);
	
	// ���� ����͸�
	sivVolMag = sqrt(siv_vclarke.Alpha * siv_vclarke.Alpha + siv_vclarke.Beta * siv_vclarke.Beta);
	sivInvVolMag = (1./sivVolMag);	            //������
	sivVolRms = 0.707 * sivVolMag;				// RMS ���� = Peak/sqrt(2)
	sivVolLLRms = sivVolRms * 1.732;            // �������� = ������*sqrt(3) 
	
	
	//---------------------------------------------------------------
	// ���� clarke ��ȯ, ���� �ִ밪, ��ȿ�� ���
	//---------------------------------------------------------------
	siv_iclarke.As = ApsOutCurU;// lsh. sivUphaseCurr;
	siv_iclarke.Bs = ApsOutCurV;// lsh. sivVphaseCurr;
	siv_iclarke.calc(&siv_iclarke);
	
	// ���� ����͸�
	sivCurrMag = sqrt(siv_iclarke.Alpha*siv_iclarke.Alpha + siv_iclarke.Beta*siv_iclarke.Beta);
	sivCurrRms = 0.707 * sivCurrMag;				// RMS ���� = Peak/sqrt(2)
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
	    				
	// ���� ���� ���ӵ� ������ �ǵ�������
	Temp = siv_pll_pi.Out + sivRatedAngleVel;				
		
	sivThetaPre	+= Temp*Ts;
	if(sivThetaPre > PI)	sivThetaPre -= PI2;
	if(sivThetaPre < -PI)	sivThetaPre += PI2;

	sivTheta = sivThetaPre;//+GridThetaComp;
	
	if(sivTheta > PI)	sivTheta -= PI2;
	if(sivTheta < -PI)	sivTheta += PI2;
			
	//--------------------------------------
	// Grid voltage park transformation ==> 2�� ����->ȸ��
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
// �Լ��� : sivController()														//
// calling : Timer Int
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//
float sivTestVolRef = 0;
float sivTestVolLimit;
void sivController()
{
	float TempOutVolfb;
	//---------------------------------------
	// SIV ���� �����
	//---------------------------------------
	siv_vpi.Kp = sivVolKp;
	siv_vpi.Ki = sivVolKi;
    siv_vpi.Kdamp = 1.;
	siv_vpi.OutMax = sivVolLimit;
    siv_vpi.OutMin = -sivVolLimit;
    siv_vpi.SampTime = Ts;
    // PI �����
	siv_vpi.Ref = sivVolRef;
	siv_vpi.Fdb = siv_vpark.QsLPF;//20120202 ���� feedback���а� ���(dq) 
	
	
	//20120202 CM������ ������а��� 460V�Ѱ� ���������� ����ϱ� ������
	//DQ�� �����Ѱ� �����ϰ� RMS�� ����Ѱ��� FEEDBACK���а����� �����
	//---->������ ��������(11��30�п� ���� �ڵ����ܵ�) ������ ���� TC APS�� ���뿹��
	//TempOutVolfb = (fabs(ApsOutVolULLRms)+fabs(ApsOutVolVLLRms)+fabs(ApsOutVolWLLRms))/3.0;		//rms�� ����� ���а�, 3�����
//	siv_vpi.Fdb = (TempOutVolfb/SQRT3)*SQRT2;
	
	
sivTestVolLimit = siv_vpi.Fdb;	//  �̼��� �׽�Ʈ �߰�,,	
	//siv_vpi.Fdb = siv3PhaseVolPeak;
	siv_vpi.calc(&siv_vpi);
	// ����� ��� ���ѱ�
//sivTestVolLimit = siv_vpi.Out;	// �̼��� �׽�Ʈ �߰�,,
	if (siv_vpi.Out > sivVolLimit)			siv_vpi.Out = sivVolLimit;
	else if(siv_vpi.Out < -sivVolLimit)		siv_vpi.Out = -sivVolLimit;
	
/*---20110528, dcdcInvPwmModulation(YES)�� ������� �׻� ���� ���ļ� �߻��� �ϱ����� sivController() �ٱ����� ��
	// SIV ���ļ� �߻�
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
    // ȸ���� ���������� ������ǥ���
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
	
	RmsCalculationVoltage(60,&ApsOutCurUSum,&ApsOutCurUTime,ApsOutCurU, &ApsOutCurURms);//UOC,U���������(������)
	RmsCalculationVoltage(60,&ApsOutCurVSum,&ApsOutCurVTime,ApsOutCurV, &ApsOutCurVRms);//VOC,V���������(������)	
	RmsCalculationVoltage(60,&ApsOutCurWSum,&ApsOutCurWTime,ApsOutCurW, &ApsOutCurWRms);//WOC,W���������(������)	

}
//=================================================================================//
// �Լ��� : SivSoftStart()                                                         //
// calling : Timerint1                                                             //
// APS SIV Softstart                                                               //
// �����ۼ� : 2010. 08. 16                    by PACTECH R&D LSH                   //
// �����ۼ� : 2012. 02. 2~3                    by PACTECH R&D LSH                  //
// 400v���� softstrat�ϰ� sivkȮ���� �� 2.5���Ŀ� 430v�� ref������ softstart�� �ø�//
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
			sivVolRefSet = (sivVolLtoLSet/SQRT3)*SQRT2;//	sivVolLtoLSet = 430;//�⵿�Ϸ��� ���пø�
			SIV_SoftStart_flag = YES;
		}
	}	
}

//==================================================================================//
// �Լ��� : InitApsSivContolVar()	    											//
// calling : ??????                                                                 //
// ����� ���� ���� �ʱ�ȭ   (init or   FaultAck  )                                 //
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
	// SIV ��� ���� �����
	// ��������� ���� �� ��� ����Ʈ
	//------------------------------------
	sivVolKp = 0.5;
	sivVolKi = 5.;

	//sivVolLimit = 445.;//20110901���� ������� ���Ƽ� 450->445�� ����
	//sivVolLimit = 350.;//20120203���� ������� ���Ƽ� 445->400�� ����
	//sivVolLimit = 345.;//20120203 ������� ���Ƽ� 445->400�� ����
	sivVolLimit = 355.;//20120203���� ������� ���Ƽ� 445->400�� ����(tc,m2,m3,m4 ����)
	sivVolLimit = 365.;//�ϼ�������� ������� ���Ƽ� ����(tc,m2�� ����)
					
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

	//sivVolLtoLInit = 434;//340;//20110901����, ��������� ���Ƽ�..438->434
	sivVolLtoLInit = 400;//20120202..434-->400
	sivVolLtoLSet = 438;//�⵿�Ϸ��� ���пø�,(tc,m2,m3,m4 ����)
	sivVolLtoLSet = 442;//�ϼ�������� ������� ���Ƽ� ����(tc,m2�� ����)

	//------------------------------------
	// ������������ ���� ������ �ִ밪 ���
	// SIV ���� ���� �� ���
	// (Vll/sqrt(3))*sqrt(2)
	//------------------------------------	
	sivVolRefSet = (sivVolLtoLInit/SQRT3)*SQRT2;

	SIV_SoftStart_flag = 0;	
	
}





