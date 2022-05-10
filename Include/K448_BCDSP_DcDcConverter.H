//==================================================================================//
// 차세대 고속철도(NTHS) 배터리 충전장치(BACH) DC/DC Converter Controller    		//
//==================================================================================//
// BACH Main header file															//
// 나중에 화일이름 NTHS_Bach.h로 변경												//
// DATE : 2010. 05. 11            						  by PACTECH R&D LSH		//
//==================================================================================//

#ifndef __NTHS_BACH_DCDCCONTROLLER__   
#define __NTHS_BACH_DCDCCONTROLLER__   

//==================================================================================//
// DATE : 2010. 08. 11            						  by PACTECH R&D LSH		//
//==================================================================================//

//========bach dc/dc controller 관련변수선언=======//
void BachDcDcController();
void InitBachDcDcControlVar();
void BcConSoftStart();
//=================================================//
var float	bcOutCurKp;
var float	bcOutCurKi;
var float bcOutCurLimit;
var	Uint16 bc_vol_cnt;

var float	bcOutCurRef;	// 전류지령 
var float	bcOutCurLPF;	// 검출전류 LPF
var float	bcOutCurPI;	    // pi제어기 출력
var float	bcOutCur_pi_I;
var float	bcOutCur_pi_Err;
var float	bcOutVolKp;
var float	bcOutVolKi;
var float	bcDutyLimit;

var float	bcOutVolRef;	// 전류지령 
var float	bcOutVolRefSet;	// 전류지령 
var float	bcOutVolLPF;	// 검출전류 LPF
var float	bcOutVolPI;	    // pi제어기 출력
var float	bcOutVol_pi_I;
var float	bcOutVol_pi_Err;
var float	bcDutyRatio;
var Uint16	bcDutyCount;

//----------------------------------//	

#endif //  




