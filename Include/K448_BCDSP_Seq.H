#ifndef __NTHS_SEQ_H
#define __NTHS_SEQ_H

//============================================================================
// Function Prototype Declaration
//============================================================================	
//=============Nths_Seq.c=========================================//
//===SEQ TIMER SETTING====
#define SEQ0_1SEC	10
#define SEQ0_5SEC	50
#define SEQ1_0SEC	100
#define SEQ1_5SEC	150
#define SEQ2_0SEC	200
#define SEQ2_5SEC	250
#define SEQ3_0SEC	300
#define SEQ3_5SEC	350
#define SEQ4_0SEC	400
#define SEQ4_5SEC	450
#define SEQ5_0SEC	500
#define SEQ10_0SEC	1000
#define SEQ20_0SEC	2000
#define SEQ30_0SEC	3000


void SequenceTimer();


void BCMJF_ON();
void BCMJF_OFF();
void BCOK_ON();
void BCOK_OFF();
void CHK_ON();
void CHK_OFF();
void LK_ON();
void LK_OFF();
void ALL_PWM_OFF();
void BachPWM_ON();
void BachPWM_OFF();
void Bach_Sequence();
void SEQ_StatusCheck();
void SEQ_StartChk();
void SEQ_Step_Process();
void Reset_Sequence();

#endif


