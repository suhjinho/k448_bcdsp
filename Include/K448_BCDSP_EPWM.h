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
// ����Ī ���ļ� 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// ī��Ʈ ������ 75E+6/5k = 15000��
// ī��Ʈ ������ 75E+6/10k = 7500��
//***********************************
//#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)

//***********************************
// Phase Shifted Time Base Period
// Switching Freq 7.5kHz
// Count Up 
// ����Ī ���ļ� 5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// ī��Ʈ ������ 75E+6/3k = 25000
// ī��Ʈ ������ 75E+6/5k = 15000
// ī��Ʈ ������ 75E+6/6k = 12500
// ī��Ʈ ������ 75E+6/7 = 10714
// ī��Ʈ ������ 75E+6/7.5 = 10000
// ī��Ʈ ������ 75E+6/10k = 7500
// ī��Ʈ ������ 75E+6/12k = 6250
//***********************************
#define PSFB_TIMER_TBPRD 		15000
//#define PSFB_TIMER_TBPRD 		37500
//#define PSFB_TIMER_TBPRD 		14999
#define PSFB_TIMER_TBPRD_HALF 	(PSFB_TIMER_TBPRD/2)


// TBCLK�� Ÿ���� 0.01333us, 0.01333us*375 = 5us;     
#define PSFB_DEADTIME_GAP	  375	// programmable delay to rising/falling edges
//#define PSFB_DEADTIME_GAP	  100	// programmable delay to rising/falling edges
// TBCLK�� Ÿ���� 0.01333us, 0.01333us*600 = 8us;     
//#define PSFB_DEADTIME_GAP	  600	// programmable delay to rising/falling edges

//==============================================


// ePWM1/2/3���� ����ϴ� Timer Period
// ����Ī ���ļ�2.5kHz, TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150E+6/(1*2)=75E+6;
// ī��Ʈ ������ 75E+6/5k = 15000��
// ī���͸� ��/�ٿ� ���� ����ϱ� ������ TBPRD�� ī��Ʈ ������ ������ �ε��Ѵ�.
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
//#define EPWM_DCDC_TBPRD_HALF  5357 //7kHz//--2021. 11. 6 k448 �ϻ꼱 ���ð������� ���ļ� ���� 7.5khz-->7khz
#define EPWM_DCDC_TBPRD_HALF  5769 //7kHz//--2021. 11. 6 k448 �ϻ꼱 ���ð������� ���ļ� ���� 7.5khz-->7khz-->6.5khz
//#define EPWM_DCDC_TBPRD_HALF  5769 //6.5kHz
//#define EPWM_DCDC_TBPRD  37500  
//20200322 change 650 -> 200
//#define DEADTIME_DCDC	  650	// programmable delay to rising/falling edges)(8usec)

//20200425	650 -> 100
//#define DEADTIME_DCDC	  650	// programmable delay to rising/falling edges)(8usec) 220226 �ܺμ��� �� ������� �������� 650->400 
#define DEADTIME_DCDC	  400	// programmable delay to rising/falling edges)(5usec)


//====SIV Switching frequence EPWM_INV_TBPRD = 37500 => 1khz
//====SIV Switching frequence PWM_INV_TBPRD = 15000 => 2.5khz
//====SIV Switching frequence PWM_INV_TBPRD = 7500 => 5khz
//#define EPWM_INV_TBPRD	7500
//#define EPWM_INV_TBPRD	37500

//#define EPWM_INV_TBPRD	25000
#define EPWM_INV_TBPRD	18750
//---20120203 â������ ���������Ƽ� ����Ī���ļ� 1KHZ(37500)->1.5KHZ(25000)���� ����
#define DEADTIME_INV	  600	// programmable delay to rising/falling edges

// 3kHz 
//#define EPWM_TIMER_TBPRD  12500  
#define EPWM_TIMER_TBPRD_MG  600  
// 3us�� ����Ÿ���� ��� ���ؼ��� 
// TBCLK * ����Ÿ�� : 75e+6 * 3e-6 = 225��
// TBCLK�� Ÿ���� 0.01333us, 0.01333us*225 = 3us;     
//#define DEADTIME_GAP	  225	// programmable delay to rising/falling edges

// 5us�� ����Ÿ���� ��� ���ؼ��� 
// TBCLK * ����Ÿ�� : 75e+6 * 5e-6 = 375��
// TBCLK�� Ÿ���� 0.01333us, 0.01333us*375 = 5us;     
//#define DEADTIME_GAP	  375	// programmable delay to rising/falling edges
// TBCLK�� Ÿ���� 0.01333us, 0.01333us*600 = 8us;     
#define DEADTIME_GAP	  600	// programmable delay to rising/falling edges
// TBCLK�� Ÿ���� 0.01333us, 0.01333us*450 = 6us;     
//#define DEADTIME_GAP	  450	// programmable delay to rising/falling edges

void EPwmInterruptEnable();
void EPwmInterruptDisable();


void InitApsEpwm();
void InitBachEpwm();
void PwmUpDate_AcDc(Uint16 PwmCount, Uint16 DeadTime);

void PwmUpDate_mdPhaseShiftUpper(Uint16 PhaseShift, Uint16 DeadTime);
#endif


