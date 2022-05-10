#ifndef __ADC_H
#define __ADC_H

var int ADCRD0;
var int ADCRD1;
var int ADCRD2;
var int ADCRD3;
var int ADCRD4;
var int ADCRD5;
var int ADCRD6;
var int ADCRD7;
var int ADCRD8;
var int ADCRD9;
var int ADCRD10;
var int ADCRD11;
var int ADCRD12;
var int ADCRD13;
var int ADCRD14;
var int ADCRD15;
var int SWF_ADC;

var int adc0_offset;
var int adc1_offset;
var int adc2_offset;
var int adc3_offset;
var int adc4_offset;
var int adc5_offset;
var int adc6_offset;
var int adc7_offset;
var int adc8_offset;
var int adc9_offset;
var int adc10_offset;
var int adc11_offset;
var int adc12_offset;
var int adc13_offset;
var int adc14_offset;
var int adc15_offset;

var float InvMaxQ12;//(1/2048)
var float MaxQ12;//-2047~2048

var float	ACInputVolBase;
var float	DCInputVolBase;
var float	BachOutVolBase;
var float	InputCurBase;
var float	BachOutCurBase;
var float ControlVolBase;


		
var float   DcCtOffset1,
			DcCtOffset2,
			DcCtOffset3,
			DcCtOffset4,
			DcCtOffset5,
			DcCtOffset6,
			DcCtOffset7,
			DcCtOffset8;
			

//------------------------------------
// SIV 관련 변수
//------------------------------------			
var float	sivUphaseVol,
			sivVphaseVol,
			sivWphaseVol;

		

var Uint32	CurrOffsetCount,
			CurrOffsetCalcCount;
var Uint16	CurrOffsetComplete,
			CurrOffsetCalcOrder;



var float 	DcCtOffsetSum1;
var float 	DcCtOffsetSum2;
var float 	DcCtOffsetSum3;
var float 	DcCtOffsetSum4;
var float 	DcCtOffsetSum5;
var float 	DcCtOffsetSum6;
var float 	DcCtOffsetSum7;
var float 	DcCtOffsetSum8;


//===========================================================
// 함수 선언
//===========================================================

//void AdcInitialization();
void CurrentOffsetCalc();
void SensorBoard_Scale_Init();	// 차세대용, APS, BACH공용
void SensorBoard_AnalogRead();	// 차세대용, APS, BACH공용
void SensorBoard_AnalogRmsLPF();
void InitSensorVar();
void RMS_caluculation();

#endif
