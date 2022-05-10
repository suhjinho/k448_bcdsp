#ifndef __ePWM_H
#define __ePWM_H
//#define Ts				(125.000e-6) //8kHz
//#define Ts				(142.857e-6) //7kHz
//#define Ts				(153.846e-6) //6.5kHz
//#define Ts				(125.000e-6)
//#define Ts				(166.666e-6)
#define Ts				(133.333e-6) //7.5kHz
//#define Ts				(142.857e-6)
//***********************************
// Phase Shifted Time Base Period
// Switching Freq 5kHz
// Count Up 
// 스위칭 주파수 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/5k = 15000개
// 카운트 갯수는 75E+6/10k = 7500개
//***********************************
//#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)

//***********************************
// Phase Shifted Time Base Period
// Switching Freq 7.5kHz
// Count Up 
// 스위칭 주파수 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/3k = 25000
// 카운트 갯수는 75E+6/5k = 15000
// 카운트 갯수는 75E+6/6k = 12500
// 카운트 갯수는 75E+6/7 = 10714
// 카운트 갯수는 75E+6/7.5 = 10000
// 카운트 갯수는 75E+6/10k = 7500
// 카운트 갯수는 75E+6/12k = 6250
//***********************************
#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD 		37500
//#define PSFB_TIMER_TBPRD 		14999
#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)


// TBCLK의 타임은 0.01333us, 0.01333us*375 = 5us;     
#define PSFB_DEADTIME_GAP	  375	// programmable delay to rising/falling edges
//#define PSFB_DEADTIME_GAP	  100	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*600 = 8us;     
//#define PSFB_DEADTIME_GAP	  600	// programmable delay to rising/falling edges

//==============================================


// ePWM1/2/3에서 사용하는 Timer Period
// 스위칭 주파수2.5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// 카운트 갯수는 75E+6/5k = 15000개
// 카운터를 업/다운 으로 사용하기 때문에 TBPRD에 카운트 갯수의 절반을 로드한다.
//====75000 => 1khz
//#define EPWM_TIMER_TBPRD  35000  
#define EPWM_TIMER_TBPRD  37500  
//====BACH/APS AC/DC Switching frequence75000 => 1khz
//#define EPWM_ACDC_TBPRD  37500
//#define DEADTIME_ACDC	  600	// programmable delay to rising/falling edges

#define EPWM_ACDC_TBPRD  15000
#define EPWM_ACDC_TBPRD_HALF  7500
//#define EPWM_ACDC_TBPRD_HALF  7499
//#define EPWM_ACDC_TBPRD  37500

#define DEADTIME_ACDC	  600	// programmable delay to rising/falling edges
//====BACH DC/DC Switching frequence 15000 => 5khz
#define EPWM_DCDC_TBPRD  15000
//#define EPWM_DCDC_TBPRD_HALF  3750 //10kHz
//#define EPWM_DCDC_TBPRD_HALF  4200 //8.93kHz
//#define EPWM_DCDC_TBPRD_HALF  4687 //8kHz
//#define EPWM_DCDC_TBPRD_HALF  5000 //7.5kHz
//#define EPWM_DCDC_TBPRD_HALF  5357 //7kHz//--2021. 11. 6 k448 일산선 스택과온으로 주파수 줄임 7.5khz-->7khz
#define EPWM_DCDC_TBPRD_HALF  5769 //7kHz//--2021. 11. 6 k448 일산선 스택과온으로 주파수 줄임 7.5khz-->7khz-->6.5khz
//#define EPWM_DCDC_TBPRD_HALF  5769 //6.5kHz
//#define EPWM_DCDC_TBPRD  37500  
//20200322 change 650 -> 200
//#define DEADTIME_DCDC	  650	// programmable delay to rising/falling edges)(8usec)

//20200425	650 -> 100
//#define DEADTIME_DCDC	  650	// programmable delay to rising/falling edges)(8usec) 220226 외부수전 시 출력전압 낮음으로 650->400 
#define DEADTIME_DCDC	  400	// programmable delay to rising/falling edges)(5usec)


//====SIV Switching frequence EPWM_INV_TBPRD = 37500 => 1khz
//====SIV Switching frequence PWM_INV_TBPRD = 15000 => 2.5khz
//====SIV Switching frequence PWM_INV_TBPRD = 7500 => 5khz
//#define EPWM_INV_TBPRD	7500
//#define EPWM_INV_TBPRD	37500

//#define EPWM_INV_TBPRD	25000
#define EPWM_INV_TBPRD	18750
//---20120203 창원에서 왜율안좋아서 스위칭주파수 1KHZ(37500)->1.5KHZ(25000)으로 변경
#define DEADTIME_INV	  600	// programmable delay to rising/falling edges

// 3kHz 
//#define EPWM_TIMER_TBPRD  12500  
#define EPWM_TIMER_TBPRD_MG  600  
// 3us의 데드타임을 얻기 위해서는 
// TBCLK * 데드타임 : 75e+6 * 3e-6 = 225개
// TBCLK의 타임은 0.01333us, 0.01333us*225 = 3us;     
//#define DEADTIME_GAP	  225	// programmable delay to rising/falling edges

// 5us의 데드타임을 얻기 위해서는 
// TBCLK * 데드타임 : 75e+6 * 5e-6 = 375개
// TBCLK의 타임은 0.01333us, 0.01333us*375 = 5us;     
//#define DEADTIME_GAP	  375	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*600 = 8us;     
#define DEADTIME_GAP	  600	// programmable delay to rising/falling edges
// TBCLK의 타임은 0.01333us, 0.01333us*450 = 6us;     
//#define DEADTIME_GAP	  450	// programmable delay to rising/falling edges

void EPwmInterruptEnable();
void EPwmInterruptDisable();


void InitApsEpwm();
void InitBachEpwm();
void PwmUpDate_AcDc(Uint16 PwmCount, Uint16 DeadTime);

void PwmUpDate_mdPhaseShiftUpper(Uint16 PhaseShift, Uint16 DeadTime);
#endif


