#include "Global_Variable_Function.h"

//20190318 사인파형보기위해 수정 Uint16	DAC_isr_cnt = 0;


Uint16	TEST_DAC0 = 0;
Uint16	TEST_DAC1 = 0;
Uint16	TEST_DAC2 = 0;
Uint16	TEST_DAC3 = 0;

Uint16	TEST_DAC4 = 0;
Uint16	TEST_DAC5 = 0;
Uint16	TEST_DAC6 = 0;
Uint16	TEST_DAC7 = 0;


Uint16  xint1_cnt = 0;
Uint16  xint1_cnt1 = 0;
Uint16  xint1_cnt2 = 0;
Uint16  xint1_cnt3 = 0;

Uint16  xint2_cnt = 0;
Uint16  timer0_cnt = 0;
Uint16  timer1_cnt = 0;

Uint16  EpwmCnt = 0;
Uint16	SystemReset = 0;

float T1READ_TMP1 = 0.;
float T1READ_TMP2 = 0.;
float T1READ_TMP3 = 0.;

float T1READ_TMP4 = 0.;
float T1READ_TMP5 = 0.;
float T1READ_TMP6 = 0.;

float T0READ_TMP1 = 0.;
float T0READ_TMP2 = 0.;
float T0READ_TMP3 = 0.;


Uint16  tst_Epwm1CNT = 0;
Uint16  tst_Epwm2CNT = 0;

Uint16  tst_Epwm3CNT = 0;
Uint16  tst_Epwm4CNT = 0;
Uint16  tst_Epwm5CNT = 0;
Uint16  tst_Epwm6CNT = 0;


float Theta_cnt_pll = 0.;
float Theta_pll = 0.;
float Theta_pll_comp = 0.;
float Theta_delay_pll = 400500.;


Uint16 msec_tmrold = 0;
Uint16 msec_tmr = 0;
Uint16 VdcHys = 0;
Uint16 MainDcHys = 0;

Uint16 n = 0;
float wt_test = 0.;
float sine_test = 0.;
Uint16 k =0;

float T0_PRDuS = 200.;
float MainDcVolSet = 500.;

void Average_Cal(float inval, unsigned int N, float *Output);
void Control_Routine();
//======================================
interrupt void epwm1_timer_isr(void)
{
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag	     210818 SUHJINHO 왜율 개선 위해
	PieCtrlRegs.PIEACK.all|=0x20;  		// Issue PIE ACK
	
	k++;
	if(k == 4)
	{
		k = 1;
//		if(FaultCounter != FaultCounter_old){
//		MonCardIrqGen(0);}	// 0 : int0 fault write	
//		MonCardIrqGen(1);	// 1 : int1 Normal write 
//	    FaultCounter_old = FaultCounter;
	}
	
	EpwmCnt++;


	//DAC_isr_cnt = 1000.*sin(wt_test);
    //20190318 DAC 확인을 위한 코드 수정

	SensorBoard_AnalogRead();	// ok
	Control_Routine();//ok

	EPwm1Regs.ETCLR.bit.INT = 1;
   
   	// Acknowledge this interrupt to receive more interrupts from group 3
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//**********************************************************//
// epwm5 Timer Interrupt									//
// Epwm5 ac/dc 인터럽트 내에서 모든 제어루틴 처리		    //
//**********************************************************//
// 1. ac/dc epwm5 스위칭주파수 5khz							//
// 2. dc/dc(siv) epwm1,2,4 스위칭주파수 5khz				//
// Tperiod = 200usec										//
//**********************************************************//
interrupt void epwm5_timer_isr(void)
{
//DSP_LED1_TOGGLE();

 	EPwm5Regs.ETCLR.bit.INT = 1;   	// Clear INT flag for this timer
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt to receive more interrupts from group 3
}
//**********************************************************//
// Timer0 interrupt Service routine							//
// Tperiod = 200usec										//
//**********************************************************//
interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;
	msec_tmr++;		// 메인 함수에서 일정 주기 task발생을 위한 카운터
	if(!SystemReset)
	{	
   		ServiceDog();// Reset the watchdog counter
	}
	msec_tmrold++;
	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge this interrupt to receive more interrupts from group 1
}
//======================================================//
// 전원주파수 위상검출									//
// 전원주파수카운트값 = Timer1주기 - 현재 Timer1값		// 
// 읽은후 Timer1값을 초기화시킴							//
//======================================================//
//Uint16 FaultDispCount=0;
// 하드웨어 폴트 발생시에 외부 인터럽트 걸림
interrupt void xint1_isr(void){
	//DSP_LED1_OFF();
	xint1_cnt++;

	LineFreqCounter = ReadCpuTimer1Period() - ReadCpuTimer1Counter();
	
	T1READ_TMP1 = ReadCpuTimer1Period();
	T1READ_TMP2 = ReadCpuTimer1Counter();	
	T1READ_TMP3 = T1READ_TMP1-T1READ_TMP2;
	T1READ_TMP4 = 150000/T1READ_TMP3;
	LineFreq    = SYS_CLK/LineFreqCounter;

	ReloadCpuTimer1();									//초기화시킴
	timer1_cnt = 0;
	//LineFreq = msec_tmrold;
	//msec_tmrold = 0;


	// Acknowledge this interrupt to receive more interrupts from group 12	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//=============================
//====500usec 
//=============================
interrupt void cpu_timer1_isr(void)
{
	
	CpuTimer1.InterruptCount++;
	timer1_cnt++;
	ReloadCpuTimer1();
	
	EDIS;
}



//========================================================//
interrupt void xint2_isr(void)
{
	xint2_cnt++;

	// Acknowledge this interrupt to receive more interrupts from group 12	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// 이 아래 인터럽트는 사용하지 않음
interrupt void epwm1_tzint_isr(void)
{
//	tzint_cnt++;
	// Acknowledge this interrupt to receive more interrupts from group 2
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

interrupt void adc_isr(void)
{	
  	//TestPulseSet2();
  	
	//TestPulseClear2();
	//Reinitialize for next ADC sequence
  	//AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	//Re-start ADC conversion
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
  	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
} 


void Average_Cal(float inval, unsigned int N, float *Output)
{
	*Output += (inval-*Output);
	*Output += inval;	
}

//=======================================//
// APS/BACH 제어루틴                     //
// EPWM5 Int에서 수행(200usec)           //
//=======================================//
void Control_Routine()
{	// 20200321 500 => 5000
	if(FaultCheckCounter > 5000)
	{	// 초기에 폴트검지하는것 방지..
		FaultProcess_int();
	}
	else
	{
		FaultCheckCounter++;
		FaultClear();// 센서보드 하드웨어 폴트, DSP IDU폴트 클리어
	}

	if(MODEAck != BOARDTEST)
	{
		if(BachPwmModulation == YES)
		{
			BcConSoftStart();
			BachDcDcController();
			BachPwmEnable();
		}
		else
		{
			InitBachDcDcControlVar();
			EPwm1Regs.CMPA.half.CMPA = 0;    // Set compare A value init
			EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF;              // Set Compare B value init
			EPwm2Regs.CMPA.half.CMPA = 0;	
			EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF;
			BachPwmDisable();
		}
	}
	else if(MODEAck == BOARDTEST)
	{
		EPwm1Regs.CMPA.half.CMPA = 0;    // Set compare A value init
		EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF;              // Set Compare B value init
		EPwm2Regs.CMPA.half.CMPA = 0;	
		EPwm2Regs.CMPB = EPWM_DCDC_TBPRD_HALF;

		BachPwmEnable();
	}
}


__interrupt void spiTxFifoIsr(void)
{
 	Uint16 i,j;
 	Uint16 DAC1,DAC2,DAC3,DAC4;
 	Uint16 TEMP;

  	switch(DAC_CMD_SEQ){
 	case	0:		//output range select unipolar +-10.8V
 	    for(i=0;i<12;i++)
 	    {
 	    	SpiaRegs.SPITXBUF = 0x0c << 8;      // Send data
 	    	SpiaRegs.SPITXBUF = 0x00 << 8;      // Send data
 	    	SpiaRegs.SPITXBUF = 0x05 << 8;      // Send data
 	    }
 	    DAC_CMD_SEQ = 1;
 		break;
 	case	1:		//control
 	    for(i=0;i<12;i++)
 	    {
 	 	    SpiaRegs.SPITXBUF = 0x19 << 8;      // Send data
 	 	    SpiaRegs.SPITXBUF = 0x00 << 8;      // Send data
 	 	    SpiaRegs.SPITXBUF = 0x00 << 8;      // Send data
 	    }
 	    DAC_CMD_SEQ = 2;
 		break;
 	case	2:		//POWER
 	    for(i=0;i<12;i++)
 	    {
 	 	    SpiaRegs.SPITXBUF = 0x10 << 8;      // Send data
 	 	    SpiaRegs.SPITXBUF = 0x07 << 8;      // Send data
 	 	    j=0x9f;
 	 	    SpiaRegs.SPITXBUF = j << 8;      // Send data
 	    }
 	    DAC_CMD_SEQ = 3;
  	    DAC_CH_SEQ = 1;
  		break;
 case	3:		//DAC Register
		if (VFD_mode == 2) //20190323 완성차 시험용 모드 추가
		{
 	 	  switch(DAC_CH_SEQ)
 	 	  {
			case	1:
				// DAC13 of DAJIG
				DAC1 = 0; //20190415 형식시험에 맞추어 수정 (unsigned int)(TEST_DAC12);

				// DAC09 of DAJIG
				DAC2 = (unsigned int)(BachOutVol_LPF / 150. * 2000.);				
				
				// DAC05 of DAJIG
				if(bcOutVolRef < 5)
				{
					DAC3 = (unsigned int)(0);
				}
				else
				{
					DAC3 = (unsigned int)(bcOutVolRef / 150. * 2000.);
				}

				// DAC01 of DAJIG
				DAC4 = (unsigned int)(BachPwmModulation * 2000.);

				DAC_CH_SEQ= 2;
				break;

			case	2:
				// DAC14 of DAJIG
				DAC1 = 0;//20190415 형식시험에 맞추어 수정  (unsigned int)(TEST_DAC13);

				// DAC10 of DAJIG
				if(BachOutVol_LPF < 5)
				{
					DAC2 = (unsigned int)(0);
				}
				else
				{
					DAC2 = (unsigned int)(BachOutVol_LPF / 150 *2000);
				}

				// DAC06 of DAJIG
				if(bcOutVolRef < 10)
				{ //20201210 dcy 5--> 10
					DAC3 = (unsigned int)(0);
				}
				else
				{
					DAC3 = (unsigned int)(bcOutVolRef / 150 *2000);
				}

				// DAC02 of DAJIG
				if(bcOutVolRef < 5)
				{ //20201210 dcy 5--> 10
					DAC4 = 0;
				}
				else
				{
					DAC4 = (unsigned int)(bcOutVolRef / 150 *2000);
				}
				//20190415 형식시험에 맞추어 수정  DAC4 = (unsigned int)(DAC_isr_cnt);
				DAC_CH_SEQ= 3;
				break;

			case	3:
				// DAC15 of DAJIG
				DAC1 = (unsigned int)(0);

				// DAC11 of DAJIG
				DAC2 = (unsigned int)(0);

				// DAC07 of DAJIG
				DAC3 = (unsigned int)(0);

				// DAC03 of DAJIG
				if(BachOutCur_LPF < 10.)
				{ //20201210 dcy 5--> 10
					DAC4 = (unsigned int)(0);
				}
				else
				{
					DAC4 = (unsigned int)(BachOutCur2_LPF/500. * 2000);
				}
				// 20190415 형식시험에 맞추어 수정  DAC4 = (unsigned int)(DAC_isr_cnt);
				DAC_CH_SEQ= 4;
				break;
			case	4:
				// DAC16 of DAJIG
				DAC1 = (unsigned int)(DINAck1_BCRUN*2000);

				// DAC12 of DAJIG
				DAC2 = (unsigned int)(0);

				// DAC08 of DAJIG
				DAC3 = (unsigned int)(0);

				// DAC04 of DAJIG
	 			DAC4 = (unsigned int)(0);
				DAC_CH_SEQ= 5;
				break;
		 	default:
		 		DAC_CH_SEQ= 1;
 	 		}
		}
		else if( VFD_mode == 3|| VFD_mode == 0)
		{
	 	 	switch(DAC_CH_SEQ)
	 	 	{
				case	1:
					DAC1 = (unsigned int)(BachOutVol_LPF / 150. * 2000.);// DAC13 of DAJIG
					DAC2 = (unsigned int)(BachOutVol_LPF / 150. * 2000.);// DAC09 of DAJIG				
					DAC3 = (unsigned int)(BachOutVol_LPF / 150. * 2000.);// DAC05 of DAJIG
	            	DAC4 = (unsigned int)(BachOutVol_LPF / 150. * 2000.);// DAC01 of DAJIG
					DAC_CH_SEQ= 2;
					break;

				case	2:
					DAC1 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC14 of DAJIG	
					DAC2 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC10 of DAJIG	 //5배 상승. 21.10.06 레코드 저장 세팅값으로 최대값 30A
					DAC3 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC06 of DAJIG
					DAC4 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC02 of DAJIG
					DAC_CH_SEQ= 3;
					break;

				case	3:
					DAC1 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC14 of DAJIG	
					DAC2 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC10 of DAJIG	 //5배 상승. 21.10.06 레코드 저장 세팅값으로 최대값 30A
					DAC3 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC06 of DAJIG
					DAC4 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC02 of DAJIG
					DAC_CH_SEQ= 4;
					break;

				case	4:
					DAC1 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC14 of DAJIG	
					DAC2 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC10 of DAJIG	 //5배 상승. 21.10.06 레코드 저장 세팅값으로 최대값 30A
					DAC3 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC06 of DAJIG
					DAC4 = (unsigned int)(BachOutCur2_LPF / 100. * 1000.);// DAC02 of DAJIG
					DAC_CH_SEQ= 5;
					break;
			 	default:
			 		DAC_CH_SEQ= 1;
			}
		}

 	    SpiaRegs.SPITXBUF = (DAC_CH_SEQ - 2) << 8;      // CH
 	    TEMP = (DAC1 & 0x0f) << 4;
 	    DAC1 = DAC1 >> 4;
 	    SpiaRegs.SPITXBUF = DAC1 << 8;      // Send data
 	    SpiaRegs.SPITXBUF = TEMP << 8;      // Send data

 	    SpiaRegs.SPITXBUF = (DAC_CH_SEQ - 2) << 8;      // CH
 	    TEMP = (DAC2 & 0x0f) << 4;
  	    DAC2 = DAC2 >> 4;
 	    SpiaRegs.SPITXBUF = DAC2 << 8;      // Send data
 	    SpiaRegs.SPITXBUF = TEMP << 8;      // Send data

 	    SpiaRegs.SPITXBUF = (DAC_CH_SEQ - 2) << 8;      // CH
 	    TEMP = (DAC3 & 0x0f) << 4;
 	    DAC3 = DAC3 >> 4;
 	    SpiaRegs.SPITXBUF = DAC3 << 8;      // Send data
 	    SpiaRegs.SPITXBUF = TEMP << 8;      // Send data

 	    SpiaRegs.SPITXBUF = (DAC_CH_SEQ - 2) << 8;      // CH
 	    TEMP = (DAC4 & 0x0f) << 4;
 	    DAC4 = DAC4 >> 4;
 	    SpiaRegs.SPITXBUF = DAC4 << 8;      // Send data
 	    SpiaRegs.SPITXBUF = TEMP << 8;      // Send data

 	    DAC_CMD_SEQ = 3;
  		break;

  	default:
 		DAC_CMD_SEQ = 0;
  	}

    while(SpiaRegs.SPIFFTX.bit.TXFFST){}
	 	SpiaRegs.SPIFFTX.bit.TXFIFO=0;

	//20190326 DAJIG 동작참조하기 위해 수정 	if((DAC_CMD_SEQ==3)&&(DAC_CH_SEQ == 5)){
	if(DAC_CH_SEQ == 5)
	{
		DAC_CH_SEQ= 1;
	  //SpiaRegs.SPIFFTX.bit.TXFIFO=0;
	  //20190326 DAJIG 동작참조하기 위해 수정		SpiaRegs.SPIFFTX.bit.TXFIFO=0;
	}

//  SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag		//210818 SUHJINHO왜율 개선 위해
//	PieCtrlRegs.PIEACK.all|=0x20;  		// Issue PIE ACK
}

__interrupt void spiRxFifoIsr(void)
{
    Uint16 i;
    for(i=0;i<12;i++)
    {
//	    rdata[i]=SpiaRegs.SPIRXBUF;		// Read data
	}
	for(i=0;i<12;i++)                    // Check received data
	{
//	    if(rdata[i] != rdata_point+i) error();
	}
//	rdata_point++;
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x20;       // Issue PIE ack
}

