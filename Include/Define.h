//###############################################################################################################
// ���ϸ�   : DSK2833x_Define.h
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.02.21
// �ۼ���   : R.N.K
// ����     : TMX(S)320F28335 ������ ŰƮ�� RealSYS DSP28335 DSK(Developer Start Kit) �������� ����ϱ� ����
// 			  ������ ���, ��ũ�� �Լ� �����̸� DSK28335 KIT ������ �������� ���̴� ��� ������ Include��
//
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v8.0(CCS_v3.3_SR8_77.exe ����) - 2008.02.21
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30323.exe ����) - 2007.12.19
// 3) C2000 Code Generation Tools v5.0 Release(C2000CodeGenerationTools5.0.0.exe ����) - 2008.02.21
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.10 ��� - 2008.02.15
//
// [ Revision History ]
//
//###############################################################################################################

#ifndef DSK2833x_DEFINE_H
#define DSK2833x_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif
//============================================
// Sequence Operation
//============================================
#define ON	1
#define OFF	0
#define YES 1
#define NO	0

//#define Ts				(333.333e-6)
#define Tsv				(2e-3)
#define PI				(3.1415922654)
#define	PI2				(6.2831845308)
#define PI_2	(PI * 2)
#define SQRT2		1.414213562
#define SQRT3		1.732050808


#define BOOL			char
#define BYTE			unsigned char
#define WORD			unsigned int
#define LONG			unsigned long
#define LLONG			unsigned long long

#define PBYTE(X)		*(volatile BYTE *)(X)
#define PWORD(X)		*(volatile WORD *)(X)
#define PLONG(X)		*(volatile LONG *)(X)
#define PLLONG(X)		*(volatile LLONG *)(X)

#define NOP				asm("	NOP")

#define NULL        	(void *)0

#define TRUE			1
#define FALSE			0

#define HIGH			1
#define LOW				0

#define	HI_BYTE(val)	((val >> 8) & 0xff)
#define	LO_BYTE(val)	(val & 0xff)


// Bit ����� �Ϲ������� ���̴� �κ��� ��ũ�� �Լ��� ������  
#define BIT_MASK(bit)				(1 << (bit))
#define GetBit(val, bit)			(((val) & BIT_MASK(bit)) >> (bit))
#define SetBit(val, bit)			(val |= BIT_MASK(bit))
#define ClearBit(val, bit)			(val &= ~BIT_MASK(bit))
#define ToggleBit(val, bit)			(val ^= BIT_MASK(bit))
#define bit_is_set(val, bit)		(val & BIT_MASK(bit))
#define bit_is_clear(val, bit)		(~val & BIT_MASK(bit))


// lsh �߰�(20100331)
#define	BitTest(val, bit)		(BOOL)((val) & BIT_MASK(bit))
#define	btst(val, bit)		(BOOL)((val) & BIT_MASK(bit))
#define sbit(val, bit)			(val |= BIT_MASK(bit))
#define cbit(val, bit)			(val &= ~BIT_MASK(bit))


#define SenLedOn(val,bit)		(val |= BIT_MASK(bit))
#define SenLedOff(val,bit)		(val &= ~BIT_MASK(bit))

#define Sen2LedOn(val,bit)		(val |= BIT_MASK(bit))
#define Sen2LedOff(val,bit)		(val &= ~BIT_MASK(bit))

/* Bit ��ġ ����(�ַ� ������ ������ ��ũ�� �Լ����� ����ϱ� ���� ������) */
#define BIT0_POS    	0
#define BIT1_POS    	1
#define BIT2_POS    	2
#define BIT3_POS    	3
#define BIT4_POS    	4
#define BIT5_POS    	5
#define BIT6_POS    	6
#define BIT7_POS    	7
#define BIT8_POS    	8
#define BIT9_POS    	9
#define BIT10_POS   	10
#define BIT11_POS   	11
#define BIT12_POS   	12
#define BIT13_POS   	13
#define BIT14_POS   	14
#define BIT15_POS   	15

/* Bit Mask Data ���� */
#define BIT0_MASK    	0x0001
#define BIT1_MASK    	0x0002
#define BIT2_MASK    	0x0004
#define BIT3_MASK    	0x0008
#define BIT4_MASK    	0x0010
#define BIT5_MASK    	0x0020
#define BIT6_MASK    	0x0040
#define BIT7_MASK    	0x0080
#define BIT8_MASK    	0x0100
#define BIT9_MASK    	0x0200
#define BIT10_MASK   	0x0400
#define BIT11_MASK   	0x0800
#define BIT12_MASK   	0x1000
#define BIT13_MASK   	0x2000
#define BIT14_MASK   	0x4000
#define BIT15_MASK   	0x8000



//===========================================================================//
// �ð����� �������				                            			//
//===========================================================================//
// �����ֱ�(200usec)���� �ð����, �ð� = CtrTimer * 200usec                 //
//===========================================================================//
// overload ���ӽð� : 60��, CtrTime_60s                                     //
//===========================================================================//
#define CtrTimer_05s    	2500
#define CtrTimer_1s    		5000
#define CtrTimer_2s    		10000
#define CtrTimer_10s    	50000
#define CtrTimer_20s    	100000
#define CtrTimer_30s    	150000
#define CtrTimer_60s    	300000
//===========================================================================//
// ������(10msec)���� �ð����, �ð� = SEQTimer * 200usec                 //
//===========================================================================//
// �߰��� �Ǵܽð� : 3���̳� 3ȸ���� : 3�� = 18000 * 10msec                  //
//===========================================================================//
#define SEQTimer_3min	18000



// DSK28335 KIT�� �������� �������� ���̴� ��� ����-------------------
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "math.h"

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSK2833x_DEFINE_H definition
