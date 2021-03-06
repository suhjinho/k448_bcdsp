//#ifndef GLOBAL__VARIABLE_FUNCTION
//#define	GLOBAL__VARIABLE_FUNCTION
#ifndef __NTHS_ISR_H
#define __NTHS_ISR_H
//============================================================================
// Interrupt declaration
//============================================================================
// 제공된 예제에서는 인터럽트 서비스 루틴을 "ramfuncs" 이라는 섹션에 할당하고
// "ramfuncs" 섹션은 빠른 응답 처리를 ㎸? 내부 RAM에서 동작하도록 함

#pragma CODE_SECTION(epwm1_timer_isr, "ramfuncs");
interrupt void epwm1_timer_isr(void);

#pragma CODE_SECTION(epwm5_timer_isr, "ramfuncs");
interrupt void epwm5_timer_isr(void);

// CPU Timer0/1/2의 Interrupt Service Function 굶?
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
interrupt void cpu_timer0_isr(void);

#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
interrupt void cpu_timer1_isr(void);

// ADC Interrupt Service Function 선언
#pragma CODE_SECTION(adc_isr, "ramfuncs");
interrupt void adc_isr(void);

#pragma CODE_SECTION(xint1_isr, "ramfuncs");
interrupt void xint1_isr(void);
//----- 이선희, XINT2 추가
#pragma CODE_SECTION(xint2_isr, "ramfuncs");
interrupt void xint2_isr(void);

#pragma CODE_SECTION(epwm1_tzint_isr, "ramfuncs");
interrupt void epwm1_tzint_isr(void);

#pragma CODE_SECTION(spiTxFifoIsr, "ramfuncs");
__interrupt void spiTxFifoIsr(void);
#pragma CODE_SECTION(spiRxFifoIsr, "ramfuncs");
__interrupt void spiRxFifoIsr(void);

#pragma CODE_SECTION(DpramWrite, "ramfuncs");


#endif



