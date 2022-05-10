#include "Global_Variable_Function.h"


void 	InitEPwm5_AcDcConLD();
void 	InitEPwm6_AcDcConLG();
	
void 	InitEPwm1_BachDcDcConLDHs();
void 	InitEPwm2_BachDcDcConLGHs();	

void 	InitEPwm1_ApsInvU();
void 	InitEPwm2_ApsInvV();	
void 	InitEPwm3_ApsInvW();
	
void InitTzGpio(void);

//==================================================================================//
// 함수명 : InitBachEpwm()			    										//
// 기  능 : BACH EPWM  초기화 											//
// DATE   : 2010. 07. 19 -> 2010.08.10															//
//==================================================================================//
void InitBachEpwm(void){
	// Stop all the TB clocks
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;
	// Trip Zone
	//InitTzGpio();
		
	InitEPwm1_BachDcDcConLDHs();
	InitEPwm2_BachDcDcConLGHs();	
	


	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;// Start all the timers synced
	EDIS;

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;

	PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;// 트립존 인터럽트는 EPWM1만 사용, 나머지 ePWM은 출력만 HIZ로
	
	PieVectTable.EPWM1_INT = &epwm1_timer_isr;
	//	PieVectTable.EPWM2_INT = &epwm2_timer_isr;
	//	PieVectTable.EPWM3_INT = &epwm3_timer_isr;
	//PieVectTable.EPWM4_INT = &epwm4_timer_isr;
	PieVectTable.EPWM5_INT = &epwm5_timer_isr;	
 
    EDIS;
	
	// Enable CPU INT3 which is connected to EPWM1-3 INT:
   	IER |= M_INT2;

	// Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
   	//PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
   	//PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
	
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	//PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	
	// Enable CPU INT3 which is connected to EPWM1/2/3/4 INT
	IER |= M_INT3;
	
}
//==================================================================================//
// 함수명 : InitApsEpwm()			    										//
// 기  능 : APS EPWM  초기화 											//
// DATE   : 2010. 07. 19 -> 2010.08.10														//
//==================================================================================//
void InitApsEpwm(void){
	// Stop all the TB clocks
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;
	// Trip Zone
	//InitTzGpio();
	
	InitEPwm5_AcDcConLD();
	InitEPwm6_AcDcConLG();
	
	InitEPwm1_ApsInvU();
	InitEPwm2_ApsInvV();	
	InitEPwm3_ApsInvW();
	
	
	// Start all the timers synced
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	// 트립존 인터럽트는 EPWM1만 사용, 나머지 ePWM은 출력만 HIZ로
	PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;
	
	PieVectTable.EPWM1_INT = &epwm1_timer_isr;
//	PieVectTable.EPWM2_INT = &epwm2_timer_isr;
//	PieVectTable.EPWM3_INT = &epwm3_timer_isr;
	//PieVectTable.EPWM4_INT = &epwm4_timer_isr;
	PieVectTable.EPWM5_INT = &epwm5_timer_isr;

    EDIS;
	
	// Enable CPU INT3 which is connected to EPWM1-3 INT:
   	IER |= M_INT2;

	// Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
   	//PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
   	//PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
	
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	//PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	
	// Enable CPU INT3 which is connected to EPWM1/2/3/4 INT
	IER |= M_INT3;
	
}


//==================================================================================//
// 함수명 : InitEPwm1_BachDcDcConLD()	    										//
// 기  능 : BACH DC/DC Converter Leadin Leg EPWM(1) , hardswitching 초기화			//
// DATE   : 2010. 10. 30															//
//==================================================================================//
void InitEPwm1_BachDcDcConLDHs()
{
	EALLOW;
	// Initialize GPIO for ePWM1 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
  	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)   
    
	/* Configure GPIO regs for ePWM1 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
  	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
	EDIS;
/**/
 	EALLOW;
   	EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
   	EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
   	//Enable TZ interrupt
    //EPwm2Regs.TZEINT.bit.OST = 1;
   	EDIS;
/**/		
	// Setup TBCLK
	EPwm1Regs.TBPRD = EPWM_DCDC_TBPRD_HALF;        		// Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up/down
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    		// Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		// sync down-stream module
	// Setup shadow register load on ZERO
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	
	// Set actions
	EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;			// set actions for EPWM1A
	EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;		
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	
	EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm1Regs.DBCTL.bit.IN_MODE   = 0x00;  // EPWMxA is signal to RED and FED
  EPwm1Regs.DBCTL.bit.POLSEL    = 0x02;  // EPWMxA is noninverted bur EPWMxB is inverted
  EPwm1Regs.DBCTL.bit.OUT_MODE  = 0x03;  // EPWMxA and EPWMxB are fully enabled     
  
  //Deadband's rising and falling edges configuration
  EPwm1Regs.DBRED               = DEADTIME_DCDC;    //Set rising  edge delay 
  EPwm1Regs.DBFED               = DEADTIME_DCDC;   //Set falling edge delay 
   
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

	EPwm1Regs.CMPA.half.CMPA = 0;    // Set compare A value init
//	EPwm1Regs.CMPB = 0;              // Set Compare B value init
	EPwm1Regs.CMPB = EPWM_DCDC_TBPRD_HALF;              // Set Compare B value init

	EPwm1Regs.PCCTL.bit.CHPEN = 0;  // Disable PWM-chopping function
	 
	// Interrupt where we will change the Compare Values
	
	
	EPwm1Regs.ETSEL.bit.SOCAEN    = 1;     // Enable SOCA
  EPwm1Regs.ETSEL.bit.SOCASEL   = 1;     // Enable CNT_zero event for SOCA
  EPwm1Regs.ETPS.bit.SOCAPRD    = 2;     // Generate SOCA on the 1st event
	EPwm1Regs.ETCLR.bit.SOCA      = 1;     // Clear SOCA flag
	
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_2ND;           // Generate INT on 1st event
	EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
	
}

//==================================================================================//
// 함수명 : InitEPwm1_BachDcDcConLD()	    										//
// 기  능 : BACH DC/DC Converter Leadin Leg EPWM(1)  초기화 						//
// DATE   : 2010. 11. 30															//
//==================================================================================//
void InitEPwm2_BachDcDcConLGHs()
{
	//*****************************************
	// EPWM2A Configuration
	//*****************************************
	EALLOW;
	// Initialize GPIO for ePWM2 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;	  // Enable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;	  // Enable pull-up on GPIO3 (EPWM2B)

	/* Configure GPIO regs for ePWM2 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;	  // Configure GPIO2 as EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
	EDIS;
	
	// Setup TBCLK
	EPwm2Regs.TBPRD = EPWM_DCDC_TBPRD_HALF;        		// Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 0;       		// Phase is 0
	
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up/down
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;    		// Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// sync flow-through
	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	
	// Set actions
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// set actions for EPWM1A
	EPwm2Regs.AQCTLA.bit.CBD = AQ_SET;		

	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	
	//Deadband configuration
	EPwm2Regs.DBCTL.bit.IN_MODE   = 0x00;  // EPWMxA is signal to RED and FED
	EPwm2Regs.DBCTL.bit.POLSEL    = 0x02;  // EPWMxA is noninverted bur EPWMxB is inverted
	EPwm2Regs.DBCTL.bit.OUT_MODE  = 0x03;  // EPWMxA and EPWMxB are fully enabled     
   
   //Deadband's rising and falling edges configuration
    
	EPwm2Regs.DBRED = DEADTIME_DCDC;     //Set rising  edge delay 
	EPwm2Regs.DBFED = DEADTIME_DCDC;    //Set falling edge delay
	
	EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV2;
	// Set Compare values	

	// Disable PWM-chopping function
	EPwm2Regs.PCCTL.bit.CHPEN = 0;
	 

}

//==================================================================================//
// 함수명 : PwmUpDate_AcDc()	    					    					//
// 기  능 : AC/DC Converter PWM값 업데이트              						//
// DATE   : 2010. 08. 16															//
//==================================================================================//
void PwmUpDate_AcDc(Uint16 PwmCount, Uint16 DeadTime)
{
	
	EPwm5Regs.CMPA.half.CMPA = PwmCount;
	EPwm5Regs.ETCLR.bit.INT = 1;

	EPwm6Regs.CMPA.half.CMPA = PwmCount;
	EPwm6Regs.ETCLR.bit.INT = 1;	
}


void EPwmInterruptEnable()
{
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	//  PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
}
void EPwmInterruptDisable()
{
	// Enable EPWM INTn in the PIE: Group 3 interrupt 1/2/3/4
	PieCtrlRegs.PIEIER3.bit.INTx1 = 0;
	//PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
	//PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
	//  PieCtrlRegs.PIEIER3.bit.INTx5 = 0;
}
//==================================================================================//
// APS SIV U-PHASE EPWM INIT													    //
//==================================================================================//
// 함수명 : InitEPwm1_ApsInvU()			    										//
// 기  능 : APS Invertr u상 epwm(1) 초기화 											//
// DATE   : 2010. 07. 19															//
//==================================================================================//
void InitEPwm1_ApsInvU(void){
	EALLOW;
	// Initialize GPIO for ePWM1 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)   
    
	/* Configure GPIO regs for ePWM1 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
	EDIS;
	//----------------------------------------------------------------------
	// Trip zone interrupt는 ePWM1A에서 사용, 나머지 ePWM에서는 TZI사용 안함
	//----------------------------------------------------------------------
	// Enable TZ5 and TZ2 as one shot trip sources
   	EALLOW;
   	//EPwm1Regs.TZSEL.bit.OSHT5 = 1;
   	//EPwm1Regs.TZSEL.bit.OSHT2 = 1;
   
   	// What do we want the TZ1 and TZ2 to do?
   	//EPwm1Regs.TZCTL.bit.TZA = TZ_HIZ;
   	//EPwm1Regs.TZCTL.bit.TZB = TZ_HIZ;
   	EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
	EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    
   	// Enable TZ interrupt
   	EPwm1Regs.TZEINT.bit.OST = 1;
   	EDIS;
	
	
	// Setup TBCLK
	
	EPwm1Regs.TBPRD = EPWM_INV_TBPRD;        		// Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Count up/down
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    		// Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		// sync down-stream module
	// Setup shadow register load on ZERO
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	// Set actions
	//EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;      	// Set PWM1A on Zero
	// Action when the counter equals the active CMPA register and the counter is incrementing.
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// set actions for EPWM1A
	//EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;			// set actions for EPWM1A
	// Action when the counter equals the active CMPA register and the counter is decrementing.
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;		
	//EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;		
	
	// Set Dead-time
	EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	//EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm1Regs.DBRED = DEADTIME_INV;
	EPwm1Regs.DBFED = DEADTIME_INV;
	
	EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
	// High Speed Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//  /1, /2, /4, /6, /8, /10, /12, /14  
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	// Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//  /1, /2, /4, /8, /16, /32, /64, /128
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;

	// TBCLK = 150e+6/(2*4)= 18.75e+6	

	// Set Compare values
	EPwm1Regs.CMPA.half.CMPA = 0;    // Set compare A value
	EPwm1Regs.CMPB = 0;              // Set Compare B value

	
	// Disable PWM-chopping function
	EPwm1Regs.PCCTL.bit.CHPEN = 0;
	 
	// Interrupt where we will change the Compare Values
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_2ND;           // Generate INT on 2ND event
	EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
}
//==================================================================================//
// APS SIV V-PHASE EPWM INIT													    //
//==================================================================================//
// 함수명 : InitEPwm2_ApsInvV()			    										//
// 기  능 : APS Invertr V상 epwm(2) 초기화										//
// DATE   : 2010. 07. 19																//
//==================================================================================//
void InitEPwm2_ApsInvV(void){
	EALLOW;
	// Initialize GPIO for ePWM2 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;	  // Enable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;	  // Enable pull-up on GPIO3 (EPWM2B)

	/* Configure GPIO regs for ePWM2 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;	  // Configure GPIO2 as EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
	EDIS;
	
	//----------------------------------------------------------------------
	// Trip zone interrupt는 ePWM1A에서 사용, 나머지 ePWM에서는 TZI사용 안함
	//----------------------------------------------------------------------
	// Enable TZ5 and TZ2 as one shot trip sources
   	EALLOW;
   	//EPwm2Regs.TZSEL.bit.OSHT5 = 1;
   	//EPwm1Regs.TZSEL.bit.OSHT2 = 1;
   
   	// What do we want the TZ1 and TZ2 to do?
   	//EPwm2Regs.TZCTL.bit.TZA = TZ_HIZ;
   	//EPwm2Regs.TZCTL.bit.TZB = TZ_HIZ;
   	EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
   	EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
   	// Enable TZ interrupt
   //	EPwm2Regs.TZEINT.bit.OST = 1;
   	EDIS;
	
	// Setup TBCLK
	EPwm2Regs.TBPRD = EPWM_INV_TBPRD;        		// Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Count up/down
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;    		// Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// sync flow-through
	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	// Set actions
	//EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;      	// Set PWM1A on Zero
	// Action when the counter equals the active CMPA register and the counter is incrementing.
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// set actions for EPWM1A
	// Action when the counter equals the active CMPA register and the counter is decrementing.
	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;		
	
	// Set Dead-time
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	//EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm2Regs.DBRED = DEADTIME_INV;
	EPwm2Regs.DBFED = DEADTIME_INV;
	
	EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
	// High Speed Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	// 000, 001, 010, 011, 100, 101, 110, 111 : /1, /2, /4, /6, /8, /10, /12, /14  
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	// Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	// 000, 001, 010, 011, 100, 101, 110, 111 : /1, /2, /4, /8, /16, /32, /64, /128
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV2;
	
	// Disable PWM-chopping function
	EPwm2Regs.PCCTL.bit.CHPEN = 0;
	 
	// Interrupt where we will change the Compare Values
	//EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	//EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	//EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
}
//==================================================================================//
// APS SIV W-PHASE EPWM INIT													    //
//==================================================================================//
// 함수명 : InitEPwm3_ApsInvW()			    										//
// 기  능 : APS Invertr W상 epwm(3) 초기화			    							//
// DATE   : 2010. 07. 19															//
//==================================================================================//
void InitEPwm3_ApsInvW(void){
	EALLOW;
	// Initialize GPIO for ePWM3 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)   
    
	/* Configure GPIO regs for ePWM3 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
	EDIS;
	
	//----------------------------------------------------------------------
	// Trip zone interrupt는 ePWM1A에서 사용, 나머지 ePWM에서는 TZI사용 안함
	//----------------------------------------------------------------------
	// Enable TZ5 and TZ2 as one shot trip sources
   	EALLOW;
   	//EPwm3Regs.TZSEL.bit.OSHT5 = 1;
   	//EPwm1Regs.TZSEL.bit.OSHT2 = 1;
   
   	// What do we want the TZ1 and TZ2 to do?
   	//EPwm3Regs.TZCTL.bit.TZA = TZ_HIZ;
   	//EPwm3Regs.TZCTL.bit.TZB = TZ_HIZ;
   	EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
   	EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
   
   	// Enable TZ interrupt
   //	EPwm3Regs.TZEINT.bit.OST = 1;
   	EDIS;
	
	// Setup TBCLK
	EPwm3Regs.TBPRD = EPWM_INV_TBPRD;        		// Set timer period
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Count up/down
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;    		// Slave module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
	// Setup shadow register load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	// Set actions
	//EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;      	// Set PWM1A on Zero
	// Action when the counter equals the active CMPA register and the counter is incrementing.
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// set actions for EPWM1A
	// Action when the counter equals the active CMPA register and the counter is decrementing.
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;		
	
	// Set Dead-time
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
//	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm3Regs.DBRED = DEADTIME_INV;
	EPwm3Regs.DBFED = DEADTIME_INV;
	
	EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
	// High Speed Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	// 000, 001, 010, 011, 100, 101, 110, 111 : /1, /2, /4, /6, /8, /10, /12, /14  
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	// Time-base Clock Prescale Bits
	// These bits determine part of the time-base clock prescale value.
	// TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	// 000, 001, 010, 011, 100, 101, 110, 111 : /1, /2, /4, /8, /16, /32, /64, /128
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV2;
	
	// Disable PWM-chopping function
	EPwm3Regs.PCCTL.bit.CHPEN = 0;
	 
	// Interrupt where we will change the Compare Values
	//EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	//EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	//EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
}


//==================================================================================//
// BACH/APS AC/DC CONVERTER EPWM INIT											    //
//==================================================================================//
// 함수명 : InitEPwm5_AcDcConLD()			    									//
// 기  능 : BACH/APS AC/DC CONVERTER Leaging Leg(LD) EPWM5 초기화					//
// DATE   : 2010. 12. 09															//
//==================================================================================//
void InitEPwm5_AcDcConLD(void){
	EALLOW;
	// Initialize GPIO for ePWM5 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;    // Enable pull-up on GPIO9 (EPWM5B)   
    
	/* Configure GPIO regs for ePWM4 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
	EDIS;

   	EALLOW;

   	EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
   	EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
  
   	EDIS;
	
	// Setup TBCLK
	EPwm5Regs.TBPRD = EPWM_ACDC_TBPRD_HALF;        		// Set timer period
	EPwm5Regs.TBPHS.half.TBPHS = 0x0000;       		// Phase is 0
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Count up/down
	EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;    		// Master module
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		// sync down-stream module
	// Setup shadow register load on ZERO
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	// Set actions
	EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// set actions for EPWM1A
	EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;		

	EPwm5Regs.AQCTLB.bit.CAD = AQ_CLEAR;			// set actions for EPWM1A
	EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;	

	// Set Dead-time
	EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	//EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;


// POLSEL bits
#define	DB_ACTV_HI		0x0
#define	DB_ACTV_LOC		0x1
#define	DB_ACTV_HIC		0x2
#define	DB_ACTV_LO		0x3


	EPwm5Regs.DBRED = DEADTIME_ACDC;
	EPwm5Regs.DBFED = DEADTIME_ACDC;
	
	EPwm5Regs.TBCTR = 0x0000;                  // Clear counter

	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV2;

	// Set Compare values
	EPwm5Regs.CMPA.half.CMPA = 0;    // Set compare A value
	EPwm5Regs.CMPB = 0;              // Set Compare B value
	
	// Disable PWM-chopping function
	EPwm5Regs.PCCTL.bit.CHPEN = 0;
	 
	// Interrupt where we will change the Compare Values

//	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event// +일경우 파형좋음, -일경우 나쁨
//	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event// +일경우 파형안좋음, -일경우 나쁨
	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;     // Select INT on Zero event// +일경우 파형좋음, -일경우 나쁨
	
	
	// ETSEL (Event Trigger Select)
//=============================
/*
#define	ET_CTR_ZERO		0x1
#define	ET_CTR_PRD		0x2
#define	ET_CTRU_CMPA	0x4
#define	ET_CTRD_CMPA	0x5
#define	ET_CTRU_CMPB	0x6
#define	ET_CTRD_CMPB	0x7
*/	
	
	
/*
	EPwm5Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	
	EPwm5Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
*/
}
//==================================================================================//
// BACH/APS AC/DC CONVERTER EPWM INIT											    //
//==================================================================================//
// 함수명 : InitEPwm6_AcDcConLG()			    									//
// 기  능 : BACH/APS AC/DC CONVERTER Laggine Leg(LD) EPWM6 초기화					//
// DATE   : 2010. 12. 09															//
//==================================================================================//
void InitEPwm6_AcDcConLG(void){
	EALLOW;
	// Initialize GPIO for ePWM6 output pin
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up on GPIO11 (EPWM6B)   
    
	/* Configure GPIO regs for ePWM4 output */
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
	EDIS;

	// Setup TBCLK
	EPwm6Regs.TBPRD = EPWM_ACDC_TBPRD_HALF;        		// Set timer period

	//EPwm6Regs.TBPHS.half.TBPHS = 0x0000; 			// Bipolar, Phase is 0
	EPwm6Regs.TBPHS.half.TBPHS = EPWM_ACDC_TBPRD_HALF;  // Unipolar	-->180도 위상 지연되는것..
	
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// Count up/down
	EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;    		// Slave module
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// sync flow-through
	// Setup shadow register load on ZERO
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	// Set actions
	EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;			// set actions for EPWM1A
	EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;		
	
	// Set Dead-time
	EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;

	EPwm6Regs.DBRED = DEADTIME_ACDC;
	EPwm6Regs.DBFED = DEADTIME_ACDC;
	
	EPwm6Regs.TBCTR = 0x0000;                  // Clear counter
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT/2
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV2;
	// Set Compare values
	EPwm6Regs.CMPA.half.CMPA = 0;    
	EPwm6Regs.CMPB = 0;              
	
	// Disable PWM-chopping function
	EPwm6Regs.PCCTL.bit.CHPEN = 0;
}

//================================
//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitTzGpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
//  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
//  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
//   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
//   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
// GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up on GPIO28 (TZ5)

//   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6) 
// GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up on GPIO29 (TZ6)  
   
/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
// GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (TZ5)

//   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6) 
// GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (TZ6)  

   
/* Configure TZ pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be TZ functional pins.
// Comment out other unwanted lines.   
//   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
//   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
//   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
//   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4

   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Configure GPIO16 as TZ5
// GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // Configure GPIO28 as TZ5

//  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Configure GPIO17 as TZ6               
// GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // Configure GPIO29 as TZ6  

   EDIS;
}



