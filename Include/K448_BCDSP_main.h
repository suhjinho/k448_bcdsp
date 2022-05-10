//==================================================================================//
// ������ ���ö��(NTHS) ���͸� ������ġ(BACH) �� ����������ġ(APS) ��������		//
//==================================================================================//
// APS/BACH main header file														//
// DATE : 2010. 05. 14            �����ۼ�				  by PACTECH R&D LSH		//
// DATE : 2010. 07. 20            ����					  by PACTECH R&D LSH		//
//==================================================================================//


//==================================================================================//
#ifndef __NTHS_MAIN__   
#define __NTHS_MAIN__   

//***************************************************************//


//***************************************************************//
//20190322 ���� ���� �߰�
var Uint16 WR_DSP_XOUT_CS_buf;

#ifdef GLOBAL__VARIABLE_FUNCTION_H

var PIDAMP 	bcOutCur_pi = PIDAMP_DEFAULTS;	// bach dc/dc ������ Ǯ�긴�� ������ ��������� pi
var PIDAMP 	bcOutVol_pi = PIDAMP_DEFAULTS;	// bach dc/dc ������ Ǯ�긴�� ������ ��������� pi
var PIDAMP 	AcdcOutCur_pi = PIDAMP_DEFAULTS;	// ac/dc ������ ��������� pi
var PIDAMP 	AcdcOutVol_pi = PIDAMP_DEFAULTS;	// ac/dc ������ ��������� pi
#else 

var PIDAMP 	bcOutCur_pi;
var PIDAMP 	bcOutVol_pi;
var PIDAMP 	AcdcOutCur_pi;
var PIDAMP 	AcdcOutVol_pi;
#endif

//===============================================================//

//============================================
// ��������
//============================================
var Uint16 	MainEpldData;
var Uint16 	Sen1EpldData;
//var Uint16 	SenLedData;

var Uint16	BachPwmModulation;
var Uint16 DEADTIME1_RED;
var Uint16 DEADTIME1_FED;
var Uint16 DEADTIME2_RED;
var Uint16 DEADTIME2_FED;

var Uint16	MODEAck;
var Uint16	TEST_board;
//==================================================================================//
// DATE : 2010. 05. 14            						  by PACTECH R&D LSH		//
//==================================================================================//
//==================================================================================//
// DATE : 2010. 07. 20            						  by PACTECH R&D LSH		//
//==================================================================================//
//==�ý��� ���ݰ���===

var Uint16 ProgramVersion;// = NTHS_APS_PROGRAM;
var Uint16 ProgramVersion_PAC;// = NTHS_APS_PROGRAM;


var float	ACInputVolU;
var float	ACInputVolU_LPF;
var float	ACInputVolU_prev;
var float	ACInputVolV;
var float	ACInputVolV_LPF;
var float	ACInputVolV_prev;
var float	ACInputVolW;
var float	ACInputVolW_LPF;
var float	ACInputVolW_prev;

var float	DCInputVol;
var float	DCInputVol_LPF;
var float	DCInputVol_prev;
var float	BachOutVol;
var float	BachOutVol_LPF;
var float	BachOutVol_prev;
var float	InputCur;
var float	InputCur_LPF;
var float	InputCur_prev;
var float	BachOutCur;
var float	BachOutCur_LPF;
var float	BachOutCur_prev;
var float	BachOutCur2_LPF;
var float	BachOutCur2_prev;
var float	BachOutCur3_LPF;
var float	BachOutCur3_prev;


var float ControlVol;
var float ControlVol_LPF;
var float ControlVol_prev;

// 20200321 LKF Ȯ�ο� �ڵ� �߰�
var float TEST_ACInputVolU_LPF;
var float TEST_ACInputVolV_LPF;
var float TEST_ACInputVolW_LPF;
var float TEST_AcInputVolUVWLLRms;
var float TEST_DCInputVol_LPF;
var float TEST_BachOutVol_LPF;
var float TEST_InputCur_LPF;
var float TEST_BachOutCur_LPF;
// 20200321 LKF Ȯ�ο� �ڵ� �߰�

var float RatedACInputVoltage;
var float RatedDCInputVoltage;
var float RatedBCOutputVoltage;

//-------------------------BACH---------------------------------------------
	//--- �Է�����(UVW) ���� 440Vacrms
var float AcInputVolU;	
var float AcInputVolURms;
var float AcInputVolV;
var float AcInputVolVRms;
var float AcInputVolW;	
var float AcInputVolWRms;
var float AcInputVolUVF;
var float	AcInputVolOVF;
var float	AcInputVolMAX;
var float	AcInputMIN;

var float	AcInputVolUTime;
var float	AcInputVolUSum;

var float	AcInputVolVTime;
var float	AcInputVolVSum;

var float	AcInputVolWTime;
var float	AcInputVolWSum;

var float	AcInputVolUVWRms;
var float	AcInputVolUVWPeak;
var float	AcInputVolUVWLLRms; 	// line-line voltage
var float	AcInputVolULLRms; 	// line-line voltage
var float	AcInputVolVLLRms; 	// line-line voltage
var float	AcInputVolWLLRms; 	// line-line voltage


//---------------------------------------------
// ������ ���ú���
// Nths_Seq.c
//---------------------------------------------
var Uint16	SeqTimer;
var Uint16	SeqTimerSet;
var Uint16	SeqTimer_flag;
var Uint16	FaultRestartTimer;
var Uint16	FaultRestartTimerSet;
var Uint16	FaultRestartTimer_flag;

var Uint16	FanCheckCounter;

var Uint16	ACDC_SoftStart_flag;

var Uint16	ACDC_SoftStart_firstflag;
var Uint16	DCDC_SoftStart_flag;
var Uint16	SIV_SoftStart_flag;

var float	acdcss_dv;
var float 	Test_bcDutyRatio;



var Uint16	acdcDutyCountepwm5;
var Uint16	acdcDutyCountepwm6;


#define FLAGSET		1
#define FLAGCLR		0

var Uint16	RunOKNG; // Init = APS_RUN_NG(0)
#define RUN_OK	1
#define RUN_NG	0

var Uint16	Sys_Status;	// Init = SYS_INIT_STATE(0)
#define SYS_INIT_STATE			0
#define SYS_CHK_STATE			1
#define SYS_Restart_STATE		2
#define SYS_MajorFault_STATE	3

var Uint16	SEQ_Step;// Init = SEQ_DO_NOTHING_STATE(0)
#define SEQ_DO_NOTHING_STATE	0
#define SEQ_START	1
#define SEQ_CHK_ON	2
#define SEQ_CHKa_CHK	3

#define SEQ_LK_ON	4
#define SEQ_LKa_CHK	5

#define SEQ_CHK_OFF	6
#define SEQ_BACH_CON_ON	7
#define SEQ_OUTV_CHK	8
#define SEQ_Normal_State	9
#define SEQ_ALLOFF	10
#define SEQ_RESET	11

// ���۸��
var Uint16	StartMode;
var Uint16  StartMode_old;
var Uint16	StartMode_buf;
var Uint16  Mode_Change_cnt;
#define	SEQ_ACMODE	0
#define	SEQ_DCMODE	1
#define SEQ_NOMODE  2

var Uint16	InputACVolAck;		// �Է����а���
var Uint16  InputACVolAck_cnt;
var Uint16  InputACVolAck_buf;
var Uint16  InputACVolAck_old;
var Uint16	InputDCVolAck;		// �Է����а���
var Uint16  InputDCVolAck_cnt;
var Uint16  InputDCVolAck_buf;
var Uint16  InputDCVolAck_old;
var Uint16	InputDCVolAckYES_cnt;
var Uint16	InputDCVolAckNO_cnt;
var Uint16	CatenaryVolAck;		// �������а���
var Uint16	ReadyAck;
var Uint16	MCBRUNAck;
var Uint16	TEST_DEADTIME_DCDC;


//---------------------------------------------
// ��Ʈ ���� ����
//---------------------------------------------
var Uint16 LowFaultAck;		// 1. ����� 
var Uint16 MidFaultAck;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����)
var Uint16 MidFaultAckold;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����)
var Uint16 HighFaultAck;	// 3. �߰��� 
//==MidFaultAck BIT ����====
#define MidFaultAck0	0 //GDF
#define MidFaultAck1	1 
#define MidFaultAck2	2 
#define MidFaultAck3	3 

#define MidFaultAck4	4 
#define MidFaultAck5	5	
#define MidFaultAck6	6 
#define MidFaultAck7	7 

#define MidFaultAck8	8 
#define MidFaultAck9	9
#define MidFaultAck10	10
#define MidFaultAck11	11

var Uint16 MidFaultMJF;		// 2. ���߰���(3���̳� ���ϰ��� 3ȸ�� �߰��� ó���Ǵ� ����) 
var Uint16 HighFaultMJF;	// 3. �߰��� 
//====MidFaultAck���� ��Ʈ����
// GDF���� ����
var Uint16 MidFCounter0;	//GDF1,2,3,4,5 �߻�Ƚ�� 
var Uint16 MidFTimer01;		//GDFs ù��° ����ð�  
var Uint16 MidFTimer02;		//GDFs �ι�° ����ð�  

var Uint16 MidFCounter1;	//�Է°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer11;		//�Է°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer12;		//�Է°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter2;	//�Է°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer21;		//�Է°����� ��Ʈ ù��° ����ð�  
var Uint16 MidFTimer22;		//�Է°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter3;	//��°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer31;		//��°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer32;		//��°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter4;	//��°����� ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer41;		//��°����� ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer42;		//��°����� ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter5;	//������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer51;		//������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer52;		//������ ��Ʈ �ι�° ����ð�    

var Uint16 MidFCounter6;	//FCV(DCLINK VOLTAGE) ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer61;		//FCV(DCLINK VOLTAGE) ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer62;		//FCV(DCLINK VOLTAGE) ������ ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter7;	//FCI(DCLINK CURRENT) ������ �Ʈ �߻�Ƚ��
var Uint16 MidFTimer71;		//FCI(DCLINK CURRENT) ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer72;		//FCI(DCLINK CURRENT) ������ ��Ʈ �ι�° ����ð� 

var Uint16 MidFCounter8;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer81;		//�������� ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer82;		//�������� ������ ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter9;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer91;		//�������� ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer92;		//�������� ������ ��Ʈ �ι�° ����ð�  

var Uint16 MidFCounter10;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer101;		//�������� ������ ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer102;		//�������� ������ ��Ʈ �ι�° ����ð� 

var Uint16 MidFCounter11;	//�������� ������ ��Ʈ �߻�Ƚ��
var Uint16 MidFTimer111;		//�������� ����? ��Ʈ ù��° ����ð� 
var Uint16 MidFTimer112;		//�������� ������ ��Ʈ �ι�° ����ð� 
//=======================================================//


var Uint16	FaultCounter;
var Uint16	FaultCheckCounter;
var Uint16	FaultCounter_old;
var Uint16	MajorFault;

var float BCACICOV_FL;
var float BCIFCOV_FL;
var float BCIFCUV_FL;
var float BCACUV_FL;
var float BCOVOV_FL;
var float BCOVUV_FL;
var float BCIFCOC_FL;
var float BCOCOC_FL;
var float BCOCUC_FL;
var float PSF_FL;

var Uint16 SWF_BCIFCUV_cnt;
var Uint16 SWF_BCOVUV_cnt;
var Uint16 SWF_BCOVOV_cnt;
var Uint16 SWF_BCIFCOV_cnt;
var Uint16 SWF_BCIFCOV2_cnt;
var Uint16 SWF_BCACICOV_cnt;
//---------------------------------------------
// GDU ��Ʈ ���� bit ����
//---------------------------------------------
//--APS/BACH --
var Uint16	FaultGDFBuf;
#define GDF1		0	// APS(siv-U), BACH(dcdc-Leading Leg)
#define GDF2		1	// APS(siv-V), BACH(dcdc-Lagging Leg)
#define GDF3		2	// APS(siv-W), BACH(none)
#define GDF4		3	// APS,BACH (acdc - Leading Leg)
#define GDF5		4	// APS,BACH (acdc - Lagging Leg)
#define GDF6		5	// SPARE
#define GDF7		6	// SPARE
#define GDF8		7	// SPARE

//---------------------------------------------
// HW ��Ʈ ���� bit ����
//---------------------------------------------
var Uint16	FaultHWBuf;
//--APS --SENCARD EPLD

#define APS_HWF_DCLINKOCF	0	//DCCT1F = DCIC,DCLINK ������,  ��ȸ��(DCCT1)
#define APS_HWF_BIT1		1	//DCCT2F = NC
#define APS_HWF_BIT2		2	//DCCT3F = NC
#define APS_HWF_DCLINKOVF	3	//DCCT4F = DCLINK OVERVOLTAGE H/W ��Ʈ�߰���

#define APS_HWF_ACICOCF		4	//ACCT1F = ACIC, AC LINE �Է�����, ��ȸ��(ACCT1)
#define APS_HWF_UOCF		5	//ACCT1F = U�� ������,  ��ȸ��(ACCT3-1)
#define APS_HWF_VOCF		6	//ACCT2F = V�� ������,  ��ȸ��(ACCT3-2)
#define APS_HWF_WOCF		7	//ACCT3F = W�� ������,  ��ȸ��(ACCT3-3)


//--BACH --
//#define BACH_HWF_ACICOCF	1	//ACCT1F = ACIC, AC LINE �Է�����, ��ȸ��(ACCT)
//#define BACH_HWF_DCLINKOCF	2	//ACCT2F = DCIC,DCLINK ������,  ��ȸ��(DCCT1)
//#define BACH_HWF_DCLINKOVF	3	//DCCT4F = DCLINK OVERVOLTAGE H/W ��Ʈ�߰���
//#define BACH_HWF_OCOCF		4	//ACCT3F = ��� ������,  ��ȸ��(DCCT2)
//#define BACH_HWF_BATOCF		5	//ACCT4F = BAT����������,  ��ȸ��(DCCT3)
//--BACH -20120119-�߸��Ȱ� �ٷ�����
#define BACH_HWF_DCLINKOVF	3	//DCCT4F = DCLINK OVERVOLTAGE H/W ��Ʈ�߰���

#define BACH_HWF_ACICOCF	4	//ACCT1F = ACIC, AC LINE �Է�����, ��ȸ��(ACCT)
#define BACH_HWF_DCLINKOCF	5	//ACCT2F = DCIC,DCLINK ������,  ��ȸ��(DCCT1)
#define BACH_HWF_OCOCF		6	//ACCT3F = ��� ������,  ��ȸ��(DCCT2)
#define BACH_HWF_BATOCF		7	//ACCT4F = BAT����������,  ��ȸ��(DCCT3)

//---------------------------------------------
// S/W ��Ʈ ���� ���� �� bit ����
//---------------------------------------------
var Uint16 	FaultSWBuf;    //--low byte(SEN1_LED_CS1(), (U22 )), high byte(SEN1_LED_CS2(), (U23 ))
//--APS --
#define		SWF_BCIFCOV	0
#define		SWF_BCIFCUV	1
#define   SWF_BCOVOV	2
#define   SWF_BCOVUV	3

#define   SWF_BCIFCOC	4	
#define   SWF_BCOCOC	5	
#define   SWF_BCOCUC	6	
#define   SWF_BCACIOV	7	

#define 	SWF_AFCUV	8	
#define 	SWF_SPARE9	9	
#define 	SWF_SPARE10	10	
#define 	SWF_SPARE11	11	

#define 	SWF_SPARE12	12	
#define 	SWF_SPARE13	13	
#define   SWF_SPARE14	14	
#define   SWF_SPARE15	15	

var Uint16	FLFaultGDFBuf;
var Uint16	FLFaultHWBuf;
var Uint16	FLFaultSWBuf;
var Uint16	FLFaultSeqBuf;
var Uint16	FLFaultEtcBuf;

var Uint16	FaultSeqBuf;// Init = 0
#define SEQERR_CHKa		0
#define SEQERR_LKa		1
#define SEQERR_SPARE2	2
#define SEQERR_SPARE3	3

#define SEQERR_SPARE4	4
#define SEQERR_SPARE5	5
#define SEQERR_SPARE6	6
#define SEQERR_SPARE7	7

var Uint16	FaultEtcBuf;// Init = 0
#define ETCERR_MJF		0
#define ETCERR_HSOT	1
#define ETCERR_CPSF	2
#define ETCERR_SPARE3	3

#define ETCERR_SPARE4	4
#define ETCERR_SPARE5	5
#define ETCERR_SPARE6	6
#define ETCERR_SPARE7	7

#define ETCERR_SPARE8	8
#define ETCERR_SPARE9	8
#define ETCERR_SPARE10	9
#define ETCERR_SPARE11	10

#define ETCERR_SPARE12	11
#define ETCERR_SPARE13	13
#define ETCERR_SPARE14	14
#define ETCERR_SPARE115	15


var Uint16 	FaultAck;
var Uint16 PSF100Ack;

var Uint16 	OverLoadAck;	// �����ϸ� 1
var Uint16 	OverLoadFAck;	// ������(OverLoadAck==1)���°� 1���̻� ���ӽ� 1
var Uint32 	OverLoad_Counter;
var Uint32 	THRFAck_Counter;	// ��Ʈ��ũ 95���̻� 1�� ī����
var Uint16	BCLKa_cnt;
var Uint16	BCCHKa_cnt;
var Uint16  PSF100Ack_cnt;
var Uint32	BCOCUC_FL_cnt;

var Uint32 	PSF_Counter;	// PSF �߻��ϸ� ��ٷ� ��Ʈ�߻��Ƚ�Ű��,,, �����ð� ���� ==> �������������п��� ������ ���۽� ��Ʈ���°� ����
                            // ������ �� 100msec�ε�...������ 150msec, �ð��� ��Ʈ�������������̽ð����� ���ж������� ������ ���۾��ϸ� ��.

//============================================================================
// Function Prototype Declaration
//==========================================================================	
// Nths_main.c
void System_Define();
void VarialbeUpdate();
void VariableInit();
// GlobalFunc.c
void TimerInterruptEnable();
void PeripheralInit();
void Init_Gpio(void);
void Init_External_Interrupt(void);
Uint16	msec_dif(Uint16 msec1, Uint16 msec2);

// debug_2833x.c
void sci_debug_init();// Initialize SCI-A for data monitoring 


#endif // __NTHS_MAIN__  




