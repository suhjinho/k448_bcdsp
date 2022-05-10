//#ifndef GLOBAL__VARIABLE_FUNCTION
//#define	GLOBAL__VARIABLE_FUNCTION
#ifndef __NTHS_ISR_H
#define __NTHS_ISR_H
//============================================================================
// Interrupt declaration
//============================================================================
// ������ ���������� ���ͷ�Ʈ ���� ��ƾ�� "ramfuncs" �̶�� ���ǿ� �Ҵ��ϰ�
// "ramfuncs" ������ ���� ���� ó���� ��� ���� RAM���� �����ϵ��� ��

#pragma CODE_SECTION(epwm1_timer_isr, "ramfuncs");
interrupt void epwm1_timer_isr(void);

#pragma CODE_SECTION(epwm5_timer_isr, "ramfuncs");
interrupt void epwm5_timer_isr(void);

// CPU Timer0/1/2�� Interrupt Service Function ���
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
interrupt void cpu_timer0_isr(void);

#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
interrupt void cpu_timer1_isr(void);

// ADC Interrupt Service Function ����
#pragma CODE_SECTION(adc_isr, "ramfuncs");
interrupt void adc_isr(void);

#pragma CODE_SECTION(xint1_isr, "ramfuncs");
interrupt void xint1_isr(void);
//----- �̼���, XINT2 �߰�
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



