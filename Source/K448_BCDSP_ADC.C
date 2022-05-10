//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) 및 보조전원장치(APS) 공용적용		//
//==================================================================================//
// sensor card adc Program															//
// ADC IC : AD7864AS-1, ;+-5V Range	, ;=(5V*2)/4096, 2's complement					//
// 자기부상열차 보조전원장치용 프로그램 수정 사용함									//
// SensorCardModel : MdBc SENSOR BOARD-A1											//
// DATE : 2010. 05. 13            						  by PACTECH R&D LSH		//
//==================================================================================//

#include "Global_Variable_Function.h"
#include "TempTable.h"
//==================================================================================//
// 함수명 : SensorBoard_Scale_Init()													//
// 기  능 : 외부 센서입력 BASE값 계산												//
// BASE정의 :	ADC 입력 5V일때 입력되는 시스템 파라메타값(입력전압등)				//
// BASE계산 =	5V*센서비/(저항*회로게인) - DC ,									//
//              5V*센서비/(저항*회로게인*sqrt(2))- AC,								//
//				open loop sensor시 저항=1로 계산, closed loop sensor시 사용저항값	//
// BASE활용 : 	프로그램 인식값 = BASE/2048											//
// 계산화일 : NTHS_센서계산.XLS														//
// DATE  : 2010. 05. 13																//
//==================================================================================//
float dclinkv_Temp;
int sen_err = 0;
int SEN1_CS1_cnt = 0;
int SEN1_CS2_cnt = 0;
int SEN2_CS1_cnt = 0;
int SEN2_CS2_cnt = 0;
int SWF_ADC0 = 0;
int SWF_ADC1 = 0;
int SWF_ADC2 = 0;
int SWF_ADC3 = 0;

int16	ADCRD0_old = 0;
int16	ADCRD1_old = 0;
int16	ADCRD2_old = 0;
int16	ADCRD3_old = 0;

int16	ADCRD4_old = 0;
int16	ADCRD5_old = 0;
int16	ADCRD6_old = 0;
int16	ADCRD7_old = 0;

int16	ADCRD8_old = 0;
int16	ADCRD9_old = 0;
int16	ADCRD10_old = 0;
int16	ADCRD11_old = 0;

int16	ADCRD12_old = 0;
int16	ADCRD13_old = 0;
int16	ADCRD14_old = 0;
int16	ADCRD15_old = 0;

int Step_Search(unsigned int SensorType, int adcvalue);


void SensorBoard_Scale_Init()
{
	// ADC 정의(12BIT)
	InvMaxQ12 = 4.8828125e-4;//(1/2048)
 	MaxQ12 	  = 2048;//-2047~2048
	
 	ACInputVolBase = 0.32127629*205/255;
	DCInputVolBase = 0.3728693;  //현차
	//DCInputVolBase = 0.908712; //목업
	BachOutVolBase = 0.0294; //20201213 dcy
//	BachOutVolBase = 0.0296; //20201223 dcy 
	InputCurBase   = 0.0454356;
	BachOutCurBase = 0.1177159; // 20201122 K128 전류 스케일 
//	BachOutCurBase = 0.15; // 20201122 K448 전류 스케일 
//	ControlVolBase = 0.0488281;
	ControlVolBase = 0.05; // 20201124 K448 70V에서 전원전압 이상으로 SWF, 게인값 올려 해결  20201213 dcy SIV와 입력값 차이 나서 수정 
  	
}
//==================================================================================//
// 함수명 : SensorBoard_AnalogRead()												//
// 기  능 : 외부 센서입력 읽어서 각 변수에 할당										//
// ADC IC : AD7864AS-1(12BIT) ;+-5V Range;=(5V*2)/4096, 2's complement,MSB(부호BIT)	//
// BASE계산 =	5V*센서비/(저항*회로게인) - DC ,									//
//				open loop sensor시 저항=1로 계산, closed loop sensor시 사용저항값	//
//	DatQ12 = SEN1_AD_CS1()&0xFFF;  	<- ADC 읽음, 12BIT								//
//	DatQ12 =( DatQ12<<4)>>4;		<--부호처리MSB(부호BIT)							//
//	Temp = ((float)(DatQ12))*InvMaxQ12; <-- 스케일링, ADCread/2048					//
//	ApsDcLinkVol = Temp*ApsDcLinkVolBase; <-- Real값으로 환산, 읽은값 * 스케일		//
//----------------------------------------------------------------------------------//
//  계산화일 : NTHS_센서계산.XLS													//
//  DATE  : 2010. 05. 13															//
//==================================================================================//

void InitADC()

{
//Clearify the ADC speed.

	DELAY_US(10000L);	
	AdcRegs.ADCTRL3.all = 0x00E0;  			// Power up bandgap/reference/ADC circuits
	DELAY_US(10000L);			    	// Delay before powering up rest of ADC   
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;	   	// Power up rest of ADC
	DELAY_US(10000L);	

//*****PRESCALER TO BE DETERMINED*************
/*
   EALLOW;
   SysCtrlRegs.HISPCP.bit.HSPCLK = 0x3; //HSPCLK  = SYSCLOCKOUT/6  = 40ns
   EDIS;


   AdcRegs.ADCTRL1.bit.CPS = 1; // 40ns * 2 = 80ns
   AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;   //80ns * 2 = 160ns (ADC sampling rate) =>6.25 MHz 
   AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;   //aquisition window time (ACQ_PS+1)*160ns
*/    

//************MODIFIED**************

	// Set internal reference selection
	// F28335의 내부 ADC Reference를 사용하지 않고 외부 ADC Reference를 쓰는 경우에는
	// 아래의 값을 사용하는 Referecne Voltage에 따라 설정하십시요. 
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x0; // 내부 ADC Referecne 사용할 때
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0x01; // 외부 ADC Referecne(2.048 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x02; // 외부 ADC Referecne(1.500 V) 사용할 때
	//AdcRegs.ADCREFSEL.bit.REF_SEL = 0x03; // 외부 ADC Referecne(1.024 V) 사용할 때



	AdcRegs.ADCTRL3.bit.ADCCLKPS 	= 16;   // Set up ADCTRL3 register 0000 1000 -> HSPCLK/(ADCCTRL[7]+1)(by kim)
	AdcRegs.ADCTRL1.bit.CPS 		= 1; 	// 40ns * 2 = 80ns
	AdcRegs.ADCTRL1.bit.ACQ_PS 		= 0x1;  // aquisition window time (ACQ_PS+1)*160ns

	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x0; 	// 샘플링모드(0-->시퀸스(순차적) 샘플링, 1--> 동시샘플링) 
//	AdcRegs.ADCTRL1.bit.SEQ_CASC  = 0x0;	// 시퀸스선택
//	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x1; 	// 샘플링모드(0-->시퀸스(순차적) 샘플링, 1--> 동시샘플링) 
	AdcRegs.ADCTRL1.bit.SEQ_CASC  = 0x1;	// 시퀸스선택
	                                        //(0-->Dual-sequencer mode. SEQ1 and SEQ2 operate as two 8-state sequencers.)
	                                        //(1-->Casecaded mode, SEQ1 and SEQ2 operate as a single 16-state sequencers.)

	//ADCTRL1
	AdcRegs.ADCTRL1.bit.CONT_RUN  = 0;		// 변환모드설정(Start-stop mode), 0-->SOC신호로 변환, 1--> 연속변환모드	
	AdcRegs.ADCTRL1.bit.SEQ_OVRD  = 0;	    // Override disabled	

	//ADCTRL2 (SEQ1 initializaton)
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 	= 1;	//Interrupt from SEQ1 enabled
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 	= 0;    //INT_SEQ1 is set at the end of every SEQ1 sequence
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1	= 1;	//Allows SEQ1 to be started by ePWM1 SOCA trigger.
//	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ1	= 1;	//Allows SEQ1 to be started by ePWM1 SOCA trigger.

	//ADCTRL2 (SEQ2 initialization)
//	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 	= 1;	//Interrupt from SEQ2 enabled
//	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 	= 0;    //INT_SEQ2 is set at the end of every SEQ2 sequence
//	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2	= 1;	//Allows SEQ2 to be started by ePWM2 SOCB trigger.

	//Reset SEQ1 and SEQ2
	AdcRegs.ADCTRL2.bit.RST_SEQ1 		= 1;	//Immidiately reset SEQ1
	//AdcRegs.ADCTRL2.bit.RST_SEQ2 		= 1;	//Immidiately reset SEQ2

	//Reset interrupt flags
	AdcRegs.ADCST.bit.INT_SEQ1_CLR 		= 1;       // Clear INT SEQ1 bit
	//AdcRegs.ADCST.bit.INT_SEQ2_CLR 		= 1;       // Clear INT SEQ2 bit

	//AdcRegs.ADCMAXCONV.all = 0x22; 			    // 3 double conv's each sequencer (6 total)
	//AdcRegs.ADCMAXCONV.all = 0x77; 			    // 3 double conv's each sequencer (6 total)
	//AdcRegs.ADCMAXCONV.all = 0x07; 			    // 3 double conv's each sequencer (6 total)
	AdcRegs.ADCMAXCONV.all = 0x0F; 			    // 3 double conv's each sequencer (6 total)

	//Sequence for SOC1
	//AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;  	// Setup conv from ADCINA0
	//AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x2;  	// Setup conv from ADCINA1
	//AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x7;  	// Setup conv from ADCINA2
	//Sequence for SOC1

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;  	// Setup conv from ADCINA0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;  	// Setup conv from ADCINA1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;  	// Setup conv from ADCINA2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;  	// Setup conv from ADCINA3

	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4;  	// Setup conv from ADCINA4
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5;  	// Setup conv from ADCINA5
	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6;  	// Setup conv from ADCINA6
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7;  	// Setup conv from ADCINA7

	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8;  	// Setup conv from ADCINA8
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9;  	// Setup conv from ADCINA9
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xA;  	// Setup conv from ADCINA10
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0xB;  	// Setup conv from ADCINA11
	
	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0xC;  	// Setup conv from ADCINB4	<-- ADC6(ACDC(WV),DCDC(NONE))
	AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0xD;  	// Setup conv from ADCINB6  <-- ADC7(ACDC(DCV),DCDC(12V OUTV))
	AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0xE;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
	AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xF;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
	
 /*
 	AdcRegs.ADCCHSELSEQ1.all = 0x3210;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ2.all = 0x7654;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ3.all = 0xBA98;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
 	AdcRegs.ADCCHSELSEQ4.all = 0xFEDC;  	// Setup conv from ADCINB7  <-- ADC_MP(1.5V REF)
*/ 
	return;
}
void SensorBoard_AnalogRead()
{   
	ADCRD0 = (AdcRegs.ADCRESULT0>>4) - 2048 - adc0_offset; 
	ADCRD1 = (AdcRegs.ADCRESULT1>>4) - 2048 - adc1_offset; 
	ADCRD2 = (AdcRegs.ADCRESULT2>>4) - 2048 - adc2_offset; 
	ADCRD3 = (AdcRegs.ADCRESULT3>>4) - adc3_offset; 
	
	ADCRD4 = (AdcRegs.ADCRESULT4>>4) - adc4_offset; 
	ADCRD5 = (AdcRegs.ADCRESULT5>>4) - adc5_offset; 
	ADCRD6 = (AdcRegs.ADCRESULT6>>4) - adc6_offset; 
	ADCRD7 = (AdcRegs.ADCRESULT7>>4) - adc7_offset; 
	
	ADCRD8 = (AdcRegs.ADCRESULT8>>4) - adc8_offset; 
	ADCRD9 = (AdcRegs.ADCRESULT9>>4) - adc9_offset; 
	ADCRD10 = (AdcRegs.ADCRESULT10>>4) - adc10_offset; 
	ADCRD11 = (AdcRegs.ADCRESULT11>>4) - adc11_offset; 

	ADCRD12 = (AdcRegs.ADCRESULT12>>4) - adc12_offset; 
	ADCRD13 = (AdcRegs.ADCRESULT13>>4) - adc13_offset; 
	ADCRD14 = (AdcRegs.ADCRESULT14>>4) - adc14_offset; 
	ADCRD15 = (AdcRegs.ADCRESULT15>>4) - adc15_offset; 
	
	ACInputVolU = ADCRD0 * ACInputVolBase;
	ACInputVolV = ADCRD1 * ACInputVolBase;
	ACInputVolW = ADCRD2 * ACInputVolBase;

	DCInputVol = ADCRD3 * DCInputVolBase;  //현차
	//DCInputVol = ADCRD4 * DCInputVolBase;//목업
	InputCur = ADCRD4 * InputCurBase;
	BachOutVol = ADCRD5 * BachOutVolBase;
	BachOutCur = ADCRD6 * BachOutCurBase;
	ControlVol = ADCRD7 * ControlVolBase;

	switch(sen_err)
	{
		case 0:
		if((ADCRD0_old == ADCRD0)||(ADCRD1_old == ADCRD1)||(ADCRD2_old == ADCRD2))
		{ 
			SEN1_CS1_cnt++;
			if(SEN1_CS1_cnt > 300)
			{
				SEN1_CS1_cnt = 301;
				SWF_ADC0 = 1;
			}
		}
		else
		{
			SEN1_CS1_cnt = 0;
			SWF_ADC0 = 0;
		}
		sen_err = 1;
		break;

		case 1:
		if((ADCRD3_old == ADCRD3)||(ADCRD4_old == ADCRD4)||(ADCRD5_old == ADCRD5)||(ADCRD6_old == ADCRD6))
		{ 
			SEN1_CS2_cnt++;
			if(SEN1_CS2_cnt > 300)
			{
				SEN1_CS2_cnt = 301;
				SWF_ADC1 = 1;
			}
		}
		else
		{
			SEN1_CS2_cnt = 0;
			SWF_ADC1 = 0;
		}
		sen_err = 2;
		break;

		case 2:
		if((ADCRD7_old == ADCRD7)||(ADCRD8_old == ADCRD8)||(ADCRD9_old == ADCRD9)||(ADCRD10_old == ADCRD10)||(ADCRD11_old == ADCRD11))
		{ 
			SEN2_CS1_cnt++;
			if(SEN2_CS1_cnt > 300)
			{
				SEN2_CS1_cnt = 301;
				SWF_ADC2 = 1;
			}
		}
		else
		{
			SEN2_CS1_cnt = 0;
			SWF_ADC2 = 0;
		}
		sen_err = 3;
		break;
		case 3:
		if((ADCRD12_old == ADCRD12)||(ADCRD13_old == ADCRD13)||(ADCRD14_old == ADCRD14)||(ADCRD15_old == ADCRD15))
		{ 
			SEN2_CS2_cnt++;
			if(SEN2_CS2_cnt > 300)
			{
				SEN2_CS2_cnt = 301;
				SWF_ADC3 = 1;
			}
		}
		else
		{
			SEN2_CS2_cnt = 0;
			SWF_ADC3 = 0;
		}
		sen_err = 4;
		break;
		case 4:

		if((SWF_ADC0 ==1)||(SWF_ADC1 ==1)||(SWF_ADC2 ==1)||(SWF_ADC2 ==1))
		{
			SWF_ADC = 1;
		}
		else SWF_ADC = 0;

		//sen1		
		ADCRD0_old = ADCRD0;
		ADCRD1_old = ADCRD1;
		ADCRD2_old = ADCRD2;

		ADCRD3_old = ADCRD3;
		ADCRD4_old = ADCRD4;
		ADCRD5_old = ADCRD5;
		ADCRD6_old = ADCRD6;
	//sen2
		ADCRD7_old = ADCRD7;
		ADCRD8_old = ADCRD8;
		ADCRD9_old = ADCRD9;
		ADCRD10_old = ADCRD10;
		ADCRD11_old = ADCRD11;

		ADCRD12_old = ADCRD12;
		ADCRD13_old = ADCRD13;
		ADCRD14_old = ADCRD14;
		ADCRD15_old = ADCRD15;
		
		sen_err = 0;
		break;
	}

	
	SensorBoard_AnalogRmsLPF();
	RMS_caluculation();

} 
//==================================================================================// 
// 함수명 : SensorBoard_AnalogRmsLPF()							// 
// 기  능 : AD값 RMS OR Low pass filter함										// 
//==================================================================================// 
//  DATE  : 2010. 10. 18															// 
//==================================================================================// 
void SensorBoard_AnalogRmsLPF() {   volatile float Temp = 0.;
	
	LowpassFT_ADC (ACInputVolU, &ACInputVolU_LPF, &ACInputVolU_prev, 100., Ts);
	LowpassFT_ADC (ACInputVolV, &ACInputVolV_LPF, &ACInputVolV_prev, 100., Ts);
	LowpassFT_ADC (ACInputVolW, &ACInputVolW_LPF, &ACInputVolW_prev, 100., Ts);
	LowpassFT_ADC (DCInputVol, &DCInputVol_LPF, &DCInputVol_prev, 100., Ts);
	LowpassFT_ADC (BachOutVol, &BachOutVol_LPF, &BachOutVol_prev, 100., Ts);
	LowpassFT_ADC (InputCur, &InputCur_LPF, &InputCur_prev, 100., Ts);
	LowpassFT_ADC (BachOutCur, &BachOutCur_LPF, &BachOutCur_prev, 100., Ts);
	LowpassFT_ADC (BachOutCur, &BachOutCur2_LPF, &BachOutCur2_prev, 0.8, Ts);
	LowpassFT_ADC (ControlVol, &ControlVol_LPF, &ControlVol_prev, 100., Ts);
}

void RMS_caluculation(){
	
	AcInputVolUTime += Ts;
	RmsCalculationVoltage(60,&AcInputVolUSum,&AcInputVolUTime,ACInputVolU, &AcInputVolURms);
	
	AcInputVolVTime += Ts;
	RmsCalculationVoltage(60,&AcInputVolVSum,&AcInputVolVTime,ACInputVolV, &AcInputVolVRms);
	
	AcInputVolWTime += Ts;
	RmsCalculationVoltage(60,&AcInputVolWSum,&AcInputVolWTime,ACInputVolW, &AcInputVolWRms);
	
	AcInputVolUVWRms = (AcInputVolURms + AcInputVolVRms + AcInputVolWRms) /3.;
	AcInputVolUVWLLRms = AcInputVolUVWRms * 1.732;
}

void CurrentOffsetCalc()
{

	volatile float Temp=0;

}

void InitSensorVar()
{
  DcCtOffset1 = 0;
	DcCtOffset2 = 0;
	DcCtOffset3 = 0;
	DcCtOffset4 = 0;
	DcCtOffset5 = 0;
	DcCtOffset6 = 0;
	DcCtOffset7 = 0;
	DcCtOffset8 = 0;
	
	CurrOffsetCount = 0;
	CurrOffsetCalcCount = 0;
	CurrOffsetComplete = 0;

	DcCtOffsetSum1 = 0;
	DcCtOffsetSum2 = 0;	
	DcCtOffsetSum3 = 0;	
	DcCtOffsetSum4 = 0;	
	DcCtOffsetSum5 = 0;
	DcCtOffsetSum6 = 0;	
	DcCtOffsetSum7 = 0;	
	DcCtOffsetSum8 = 0;	
	
	adc0_offset = -8;
	adc1_offset = 7;
	adc2_offset = 15;
	adc3_offset = 0;
	adc4_offset = 10;
	adc5_offset = 10;
	adc6_offset = 15;
	adc7_offset = 7;
	adc8_offset = 0;
	adc9_offset = 0;
	adc10_offset = 0;
	adc11_offset = 0;
	adc12_offset = 0;
	adc13_offset = 0;
	adc14_offset = 0;
	adc15_offset = 0;
	
	ACInputVolU = 0;
	ACInputVolU_LPF = 0;
	ACInputVolU_prev = 0;
	ACInputVolV = 0;
	ACInputVolV_LPF = 0;
	ACInputVolV_prev = 0;
	ACInputVolW = 0;
	ACInputVolW_LPF = 0;
	ACInputVolW_prev = 0;
	
	DCInputVol = 0;
	DCInputVol_LPF = 0;
	DCInputVol_prev = 0;
	BachOutVol = 0;
	BachOutVol_LPF = 0;
	BachOutVol_prev = 0;
	InputCur = 0;
	InputCur_LPF = 0;
	InputCur_prev = 0;
	BachOutCur = 0;
	BachOutCur_LPF = 0;
	BachOutCur_prev = 0;
	BachOutCur2_LPF = 0;
	BachOutCur2_prev = 0;
	ControlVol = 0;
	ControlVol_LPF = 0;
	ControlVol_prev = 0;
	
	AcInputVolU = 0;	
	AcInputVolURms = 0;
	AcInputVolV = 0;
	AcInputVolVRms = 0;
	AcInputVolW = 0;	
	AcInputVolWRms = 0;
	AcInputVolUVF = 0;
	AcInputVolOVF = 0;
	AcInputVolMAX = 0;
	AcInputMIN = 0;

	AcInputVolUTime = 0;
	AcInputVolUSum = 0;

	AcInputVolVTime = 0;
	AcInputVolVSum = 0;

	AcInputVolWTime = 0;
	AcInputVolWSum = 0;

	AcInputVolUVWRms = 0;
	AcInputVolUVWPeak = 0;
	AcInputVolUVWLLRms = 0; 
	AcInputVolULLRms = 0; 	 
	AcInputVolVLLRms = 0; 	 
	AcInputVolWLLRms = 0;

	TEST_ACInputVolU_LPF = 0.;
	TEST_ACInputVolV_LPF = 0.;
	TEST_ACInputVolW_LPF = 0.;
	TEST_AcInputVolUVWLLRms = 0.;
	TEST_DCInputVol_LPF = 0.;
	TEST_BachOutVol_LPF = 0.;
	TEST_InputCur_LPF = 0.;
	TEST_BachOutCur_LPF = 0.;	

}

//========================================================================//
// 
//===========================================================================//
int Step_Search(unsigned int SensorType, int adcvalue)
{
	int idx = 0;
	int calok = 1;
	
	if(SensorType == 0){//ntc10k pull-dn 10k(15v), 센서는 위에..
 	   while(calok){
 		    if(adcvalue <= TempTable_ntc10k[idx]){
				calok = 0;
			}	   
 	 	   idx++;
		}
		if(abs(adcvalue - TempTable_ntc10k[idx-2]) < abs(adcvalue - TempTable_ntc10k[idx-1]))   	return idx-2;		
    	else   	return idx-1;		
	}
	else if(SensorType == 1){//인피니언 IGBT내장, 센서는 위에..
 	   while(calok){
 		    if(adcvalue <= TempTable_FF1000R17IE4_ntc5k[idx]){
				calok = 0;
			}	   
 		    idx++;
		}
 	   if(abs(adcvalue - TempTable_FF1000R17IE4_ntc5k[idx-2]) < abs(adcvalue - TempTable_FF1000R17IE4_ntc5k[idx-1]))   	return idx-2;		
 	   else   	return idx-1;		
	}
	else if(SensorType == 2){//ntc10k pull-up 10k(15v), 센서는 밑에..
 	   while(calok){
 		    if(adcvalue >= TempTable_ntc10k_nths[idx]){
				calok = 0;
			}	   
 	 	   idx++;
		}
		if(abs(adcvalue - TempTable_ntc10k_nths[idx-2]) > abs(adcvalue - TempTable_ntc10k_nths[idx-1]))   	return idx-2;	//여기코드 나중에 재검증해야함	
    	else   	return idx-1;		
	}
	else if(SensorType == 3){//pt1000 pull-up 10k(15v), 센서는 밑에..
 	   while(calok){
 		    if(adcvalue <= TempTable_PT1000_bat[idx]){
				calok = 0;
			}	   
 	 	   idx++;
		}
		if(abs(adcvalue - TempTable_PT1000_bat[idx-2]) < abs(adcvalue - TempTable_PT1000_bat[idx-1]))   	return idx-2;	//여기코드 나중에 재검증해야함	
    	else   	return idx-1;		
	}
	else {
 	   return 0;	
	}		
		
}

