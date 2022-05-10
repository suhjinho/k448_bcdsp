#ifndef GLOBAL__VARIABLE_FUNCTION
#define	GLOBAL__VARIABLE_FUNCTION


// 변수 선언을 한번만 하기 위해서 메크로 사용
#ifdef GLOBAL__VARIABLE_FUNCTION_H
#define	var
#else
#define	var	extern
#endif


#include "C28x_FPU_FastRTS.h"
#include "Define.h"
#include "K448_BCDSP_MemoryMap.h"
#include "DSP2833x_Device.h"     // Headerfile Include File

#include "K448_BCDSP_Dio.h"
#include "K448_BCDSP_ECap.h"
#include "K448_BCDSP_EPWM.h"
#include "K448_BCDSP_ADC.h"   
#include "K448_BCDSP_Dac.h"   
#include "K448_BCDSP_Controller.h"
#include "K448_BCDSP_DcDcConverter.h"
#include "K448_BCDSP_SIV.h"
#include "K448_BCDSP_main.h"
#include "K448_BCDSP_Fault.h"
#include "K448_BCDSP_Seq.h"
#include "K448_BCDSP_monitoring.h"
#include "K448_BCDSP_AcdcConverter.h"
#include "K448_BCDSP_Isr.h"
void InitADC();

#endif



