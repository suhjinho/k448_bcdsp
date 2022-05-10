#ifndef __NTHS_SIV__
#define	__NTHS_SIV__

#ifdef GLOBAL__VARIABLE_FUNCTION_H

// PI 제어기
var PIDAMP 	siv_vpi = PIDAMP_DEFAULTS;
var PIDAMP 	siv_pll_pi = PIDAMP_DEFAULTS;

// 축 변환
var PARK 	siv_vpark = PARK_DEFAULTS;
var CLARKE 	siv_vclarke = CLARKE_DEFAULTS;
var PARK 	siv_ipark = PARK_DEFAULTS;
var CLARKE 	siv_iclarke = CLARKE_DEFAULTS;

var IPARK 	siv_ref_ipark = IPARK_DEFAULTS;
var PARK 	siv_ref_park = PARK_DEFAULTS;
// 공간전압벡터
var SVGENDQ siv_svpwm = SVGENDQ_DEFAULTS;


#else 
// PI 제어기
var PIDAMP 	siv_vpi;
var PIDAMP 	siv_pll_pi;

// 축 변환
var PARK 	siv_vpark;
var CLARKE 	siv_vclarke;
var PARK 	siv_ipark;
var CLARKE 	siv_iclarke;

var IPARK 	siv_ref_ipark;
var PARK 	siv_ref_park;
// 공간전압벡터
var SVGENDQ siv_svpwm;

#endif

var float	sivInvVolMag,
			sivVolRms,
			sivVolLLRms,
			sivCurrMag,
			sivCurrRms,
			siv_park,
			sivRatedAngleVel,
			sivThetaPre,
			sivTheta;
			
var float	sivPllKp,
			sivPllKi,
			sivVolRef,
			sivVolRefSet,
			sivVolLimit,
			sivThetaGen,
			sivVolRefMag,
			sivVolLtoL,
			sivVolLtoLInit,
			sivVolLtoLSet,
			
			sivVolKp,
			sivVolKi,
			sivVolMag,
			sivRatedFreq;

var Uint16	sivDutyCountU,
			sivDutyCountV,
			sivDutyCountW;

var	float	sivUVWphaseVolMax,
			sivUVWphaseCurrMax;


void SivPllRefTrans();
void sivController();
void InitApsSivContolVar();
void SivOutputVolRms();

void SivSoftStart();
#endif

