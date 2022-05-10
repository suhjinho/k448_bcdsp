#include "Global_Variable_Function.h"

void LPF1(float unit_time,float pole,float input,float *output)
{
	*output+=pole*(input-*output)*unit_time;
}
void LPF2(float unit_time, float wc, float input, float *output)
{
	static float a, b;
	static float input_old=0.;
	
	a = (2.0 - wc*unit_time)/(2.0 + wc*unit_time);
	b = (wc*unit_time)/(2.0 + wc*unit_time);
	
	*output = a*(*output) + b*(input+input_old);
	input_old = input;
}
void HPF2(float unit_time, float wc, float input, float *output)
{
	float a, b;
	static float input_old=0.;
		
	a = (2.0 - wc*unit_time)/(2.0 + wc*unit_time);
	b = 2.0/(2.0 + wc*unit_time);
	
	*output = a*(*output) + b*(input-input_old);
	input_old = input;
	
	
}

void LowpassFT_ADC (float Input, float *output, float *output_prev, float cutoff_freq, float T){
	float LPF_const = 0;
	LPF_const = 2*3.141592*	cutoff_freq * T;
	*output = ((LPF_const)/(LPF_const + 1))*Input + (1/(LPF_const + 1))* *output_prev;
	*output_prev = *output;
}

// PI_Damp_Controller.c에 있던것 옮겨옴
void PI_Damp_calc(PIDAMP *v)
{	
    // Error Calculation
    v->Err = v->Ref - v->Fdb;
    // Integral Calculation		;
	v->Ui += v->Ki*(v->Ref - v->Fdb)*v->SampTime;
	// Anti-Windup
	if (v->Ui > v->OutMax)			v->Ui=v->OutMax;
	else if(v->Ui < v->OutMin)		v->Ui=v->OutMin;
	// IP-PI 제어기
	v->Out=v->Kp*(v->Kdamp * v->Ref - v->Fdb)+v->Ui;
     
}


void PI_Damp_Controller(float limit, float limit_dow, float discrete_time,
						float damp_factor,float Kp,float Ki,
						float reference,float feedback,
						float *integral,float *output)
{
	*integral+=Ki*(reference-feedback)*discrete_time;
	if (*integral>fabs(limit))			*integral=fabs(limit);
	else	if (*integral< limit_dow)	*integral=limit_dow;
	*output=Kp*(damp_factor*reference-feedback)+*integral;
}
void PI_Controller(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output)
{
	*integral+=Ki*(error*discrete_time);
	*output = Kp*error + *integral;
	if (*integral>fabs(limit))			*integral=fabs(limit);
	else	if (*integral<-fabs(limit))	*integral=-fabs(limit);
}


void PI_AntiWindup(float limit,float discrete_time,float Kp,float Ki,float error,float *integral,float *output)
{
	*integral += Ki*(error-((0)*(PI_AntiWindupVar-*output)))*discrete_time;
	/*----*integral += Ki*(error-((1/Kp)*(*output-PI_AntiWindupVar))*discrete_time);	*/

	PI_AntiWindupVar = Kp*error + *integral;
	
	if(PI_AntiWindupVar > fabs(limit))        *output = fabs(limit);
	else if(PI_AntiWindupVar < -fabs(limit))  *output = -fabs(limit);
	else	*output = PI_AntiWindupVar;
}

void PI_AntiWindup1(float limit,float discrete_time,float Kp,float Ki,float Kt, float error,float *integral,float *output)
{
	*integral += Ki*(error-((1/Kt)*(PI_AntiWindupVar-*output))*discrete_time);
	
	PI_AntiWindupVar = Kp*error + *integral;
	
	if(PI_AntiWindupVar > fabs(limit))        *output = fabs(limit);
	else if(PI_AntiWindupVar < -fabs(limit))  *output = -fabs(limit);
	else	*output = PI_AntiWindupVar;
}
void RmsCalculationVoltage(float Freq,float *Sum,float *PeriodTime,float Input, float *Output)
{
	float Tmp = (0);

		
	Tmp = 1/Freq;
	
	//*PeriodTime += SamplingTime;
	if(*PeriodTime > Tmp)
	{
		*Output = sqrt(*Sum * Freq);
		*PeriodTime = 0;
		*Sum = 0;
	}	
	
	*Sum += ((Input*Input)*Ts);
		
	
	
}
//-----integralUsingLPF.C에 있던것 옮겨옴---
void integral_calc(INTEGRAL *v)
{	
  	static float K1,K2,Emf;
  	    
	K1 = (2.0 - v->Wc*v->SampT)/(2.0 + v->Wc*v->SampT);
	K2 = v->SampT/(2.0 + v->Wc*v->SampT);
    
    // Apha axis Emf
    Emf = v->AlphaVol - v->Rs*v->AlphaCurr;
    
    // Apha axis flux estimator using a LPF
	v->Alpha = K1*v->Alpha + K2*(Emf+v->EmfAlphaOld);
	v->EmfAlphaOld = Emf;
	
	// Q axis Emf
    Emf = v->BetaVol - v->Rs*v->BetaCurr;
    // Q axis flux estimator using a LPF
	v->Beta = K1*v->Beta + K2*(Emf+v->EmfBetaOld);
	v->EmfBetaOld = Emf;
	
	// Flux angle calculation
	v->Theta = atan2(v->Beta, v->Alpha);
	// 자속 q축을 기준으로 각도가 0부터 계산
	// 자속 d축을 기준으로 각도를 계산하기 위해서 90도 보상
	//v->Theta = v->Theta - PI/2;
	
	// 보상 값이 PI 오버플로우 처리
	//if(v->Theta > PI)	v->Theta -= PI2;
	//if(v->Theta < -PI)	v->Theta += PI2;
	
   	// Flux Magnitue calculation
   	v->Mag = sqrt(v->Alpha*v->Alpha + v->Beta*v->Beta);
   	
   	// Flux angular velocity calculation
   	v->AlphaDt 	= (v->Alpha - v->AlphaOld)/v->SampT;
   	v->BetaDt 	= (v->Beta 	- v->BetaOld)/v->SampT;
   	v->AlphaOld = v->Alpha; 
   	v->BetaOld  = v->Beta;
   	
   	v->OmegaHat = 	(v->BetaDt*v->Alpha - v->AlphaDt*v->Beta)
   					/( v->Alpha*v->Alpha + v->Beta*v->Beta + 1.0e-15); 
   	LPF1(v->SampT, v->Wc*100, v->OmegaHat, &v->OmegaHatLPF);
}
//-----ipark.c에 있던것 옮겨옴---
void ipark_calc(IPARK *v)
{	
   
   float32 Cosine,Sine;
   
  // TI's fast RTS Library
  sincos(v->Angle, &Sine, &Cosine);
  	
   v->Alpha = v->Ds*Cosine - v->Qs*Sine;
   v->Beta = v->Qs*Cosine + v->Ds*Sine; 
   
// Using look-up sine table 
// (PI/2)/(2*PI) = 0.25
// ((PI/2)/(2*PI))*256 = 0.25*256 = 64
// ((PI/2)-2*PI)/(2*PI) = -0.75
// (((PI/2)-2*PI)/(2*PI))*256 = -0.75*256 = -192

 /*   if (v->Angle+0.25 > 1.0) 
     {
     Sine = sin_tab[(int16)(v->Angle*256)];
     Cosine = sin_tab[(int16)(v->Angle*256) - 192];  
     }
    else
    { 
     Sine = sin_tab[(int16)(v->Angle*256)];
     Cosine = sin_tab[(int16)(v->Angle*256) + 64];
    }
 
     v->Alpha = v->Ds*Cosine - v->Qs*Sine;
     v->Beta = v->Qs*Cosine + v->Ds*Sine;  
*/
}
//-----park.c에 있던것 옮겨옴---
void park_calc(PARK *v)
{	
  
    float32 Cosine,Sine;
    
    // TI's fast RTS Library
  	sincos(v->Angle, &Sine, &Cosine);
    
    v->Ds = v->Alpha*Cosine + v->Beta*Sine;
    v->Qs = v->Beta*Cosine - v->Alpha*Sine;

	
// Using look-up sine table 
// (PI/2)/(2*PI) = 0.25
// ((PI/2)/(2*PI))*256 = 0.25*256 = 64
// ((PI/2)-2*PI)/(2*PI) = -0.75
// (((PI/2)-2*PI)/(2*PI))*256 = -0.75*256 = -192

/*    if (v->Angle+0.25 > 1.0) 
     {
     Sine = sin_tab[(int16)(v->Angle*256)];
     Cosine = sin_tab[(int16)(v->Angle*256) - 192];  
     }
    else
    { 
     Sine = sin_tab[(int16)(v->Angle*256)];
     Cosine = sin_tab[(int16)(v->Angle*256) + 64];
    }

     v->Ds = v->Alpha*Cosine + v->Beta*Sine;
     v->Qs = v->Beta*Cosine - v->Alpha*Sine;
*/
}

//-----clarke.c에 있던것 옮겨옴---
void clarke_calc(CLARKE *v)
{	
   v->Alpha = v->As;

   v->Beta = (v->As + 2*v->Bs)*0.57735026918963;  // 1/sqrt(3) = 0.57735026918963
}

//-----SVGEN_DQ.c에 있던것 옮겨옴---
#define		VDC_MIN			50.0
#define		INV_VDC_MIN		0.02

void svgendq_calc(SVGENDQ *v)
{	
	float Vx,Vy,Dx,Dy,Dz,Dxy;
//	Uint32 Sector = 0;  // Sector is treated as Q0 - independently with global Q
	// Calculate Vx & Vy 
	if(v->Ubeta >= 0)
	{
		if(fabs(0.57735 * v->Ubeta) < fabs(v->Ualpha))
		{
			if(v->Ualpha >= 0)
			{
				Vx = v->Ualpha - 0.57735 * v->Ubeta;
				Vy = 1.154701 * v->Ubeta;			// 2/sqrt(3)=1.154701
				v->Sector = 1;					// sector 1 : from 0 to 60 
			}else{
				Vx = 1.154701 * v->Ubeta;
				Vy = -(v->Ualpha + 0.57735 * v->Ubeta);
				v->Sector = 3;					// sector 3: from 120 to 180 
			}		
		}else{
			Vx = v->Ualpha + 0.57735 * v->Ubeta;
			Vy = -v->Ualpha + 0.57735 * v->Ubeta;
			v->Sector = 2;					// sector 2 : from 60 to 120 
		}
	}else{
		if(fabs(0.57735 * v->Ubeta) < fabs(v->Ualpha))
		{
			if(v->Ualpha >= 0)
			{
				Vx = -1.154701 * v->Ubeta;
				Vy = v->Ualpha + 0.57735 * v->Ubeta;
				v->Sector = 6;					// sector 6 : from 300 to 360 
			}else{
				Vx = 0.57735 * v->Ubeta - v->Ualpha;
				Vy = -1.154701 * v->Ubeta;
				v->Sector = 4;					// sector 4 : from 180 to 240 
			}
		}else{
				Vx = -v->Ualpha - 0.57735 * v->Ubeta;
				Vy = v->Ualpha - 0.57735 * v->Ubeta;
				v->Sector = 5;					// sector 5 : from 240 to 300 
		}
	}
	
	if(v->Vdc > VDC_MIN)
	{
		v->inv_Vdc = 1.0 / v->Vdc;
		Dx = 1.5 * Vx * v->inv_Vdc;					// Maximum available voltage = 2.0*Vdc/3.0 
		Dy = 1.5 * Vy * v->inv_Vdc;						
		Dxy = Dx + Dy;
	}else{
		Dx = 1.5 * Vx * INV_VDC_MIN;				// Maximum available voltage = 2.0*Vdc/3.0 
		Dy = 1.5 * Vy * INV_VDC_MIN;						
		Dxy = Dx + Dy;
	}


	if(Dxy >= 1.0)
	{
		if (Dx > Dy)
		{
			Dx = Dx / Dxy;
			Dy = 1.0 - Dx;
		}else{
			Dy = Dy / Dxy;
			Dx = 1.0 - Dy;
		}
		Dxy = 1.0;
		Dz = 0;
	}else
	 	Dz = 0.5 * (1.0 - Dxy);

	// 섹터별로 듀티값을 계산한다.
	switch (v->Sector)
	{
		case 1	:
				{	
					v->DutyU = Dxy + Dz;
					v->DutyV = Dy + Dz;
					v->DutyW = Dz;
					break;
				}
		case 2	:
				{
					v->DutyU = Dx + Dz;
					v->DutyV = Dxy + Dz;
					v->DutyW = Dz;
				}
					break;
		
		case 3	:
				{
					v->DutyU = Dz;
					v->DutyV = Dxy + Dz;
					v->DutyW = Dy + Dz;
				}
					break;
		
		case 4	:
				{
					v->DutyU = Dz;
					v->DutyV = Dx + Dz;
					v->DutyW = Dxy + Dz;
				}
					break;
		
		case 5	:
				{
					v->DutyU = Dy + Dz;
					v->DutyV = Dz;
					v->DutyW = Dxy + Dz;
				}
					break;
		
		case 6	:
				{
					v->DutyU = Dxy + Dz;
					v->DutyV = Dz;
					v->DutyW = Dx + Dz;
				}
					break;
	}
}	
	
/*																	
// Inverse clarke transformation
    Va = v->Ubeta;
    Vb = -0.5*v->Ubeta + 0.8660254*v->Ualpha;  // 0.8660254 = sqrt(3)/2 
    Vc = -0.5*v->Ubeta - 0.8660254*v->Ualpha;  // 0.8660254 = sqrt(3)/2

// 60 degree Sector determination
    if (Va>0)
       Sector = 1; 
    if (Vb>0)
       Sector = Sector + 2;
    if (Vc>0)   
       Sector = Sector + 4;
       
// X,Y,Z (Va,Vb,Vc) calculations
//    Va = v->Ubeta;                                                       // X = Va 
//    Vb = 0.5*v->Ubeta + 0.8660254*v->Ualpha;   // Y = Vb 
//    Vc = 0.5*v->Ubeta - 0.8660254*v->Ualpha;   // Z = Vc
    
    if(v->Vdc > VDC_MIN)
    {
    	// 최대 출력 전압 및 PU로 계산하기 위해서
    	temp = 1.5*(1/v->Vdc);					// Maximum available voltage = 2.0*Vdc/3.0 
    	// X,Y,Z (Va,Vb,Vc) calculations
    	Va = v->Ubeta*temp;                             // X = Va 
    	Vb = 0.5*v->Ubeta*temp + 0.8660254*v->Ualpha*temp;   // Y = Vb 
    	Vc = 0.5*v->Ubeta*temp - 0.8660254*v->Ualpha*temp;   // Z = Vc
    }else{
    	temp = VDC_MIN;					// Maximum available voltage = 2.0*Vdc/3.0 
    	// X,Y,Z (Va,Vb,Vc) calculations
    	Va = v->Ubeta*temp;                             // X = Va 
    	Vb = 0.5*v->Ubeta*temp + 0.8660254*v->Ualpha*temp;   // Y = Vb 
    	Vc = 0.5*v->Ubeta*temp - 0.8660254*v->Ualpha*temp;   // Z = Vc
	}
    
    if (v->Vdc > Vdc_MINIMUM)
	{
		inv_Vdc = 1.0 / Vdc;
		Dx = 1.5 * Vx * inv_Vdc;					// Maximum available voltage = 2.0*Vdc/3.0 
		Dy = 1.5 * Vy * inv_Vdc;						
		Dxy = Dx + Dy;
	}
	else
	{
		Dx = 1.5 * Vx * inv_Vdc_MINIMUM;				// Maximum available voltage = 2.0*Vdc/3.0 
		Dy = 1.5 * Vy * inv_Vdc_MINIMUM;						
		Dxy = Dx + Dy;
	}


	if (Dxy >= 1.0)
	{
		if (Dx > Dy)
		{
			Dx = Dx / Dxy;
			Dy = 1.0 - Dx;
		}
		else
		{
			Dy = Dy / Dxy;
			Dx = 1.0 - Dy;
		}

		Dxy = 1.0;
		Dz = 0;
	}
	else
	 	Dz = 0.5 * (1.0 - Dxy);
    
    
    if (Sector==0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
    {
       v->Ta = 0.5;
       v->Tb = 0.5;
       v->Tc = 0.5;
    }
    if (Sector==1)  // Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
    {
       t1 = Vc;
       t2 = Vb;
       tz = 0.5*(1-t1-t2);			// zero vector
       v->Ta = t1+tz;           	// taon = tbon+t1
       v->Tb = tz;      			// tbon = (1-t1-t2)/2
       v->Tc = t1+t2+tz;           	// tcon = taon+t2
    }
    else if (Sector==2)  // Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
    {
       t1 = Vb;
       t2 = -Va;
       tz = 0.5*(1-t1-t2);			// zero vector
       v->Ta = tz;      // taon = (1-t1-t2)/2
       v->Tb = t1+t2+tz;           // tbon = tcon+t2
       v->Tc = t1+tz;           // tcon = taon+t1
    }      
    else if (Sector==3)  // Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
    {
       t1 = -Vc;
       t2 = Va;
       tz = 0.5*(1-t1-t2);			// zero vector
       v->Ta = tz;      // taon = (1-t1-t2)/2
       v->Tb = t1+tz;           // tbon = taon+t1
       v->Tc = t1+t2+tz;           // tcon = tbon+t2
    }   
    else if (Sector==4)  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
    {
       t1 = -Va;
       t2 = Vc;
       tz = 0.5*(1-t1-t2);			// zero vector
 	   v->Ta = t1+t2+tz;           // taon = tbon+t2	
       v->Tb = t1+tz;           // tbon = tcon+t1
       v->Tc = tz;      // tcon = (1-t1-t2)/2
       
    }   
    else if (Sector==5)  // Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
    {
       t1 = Va;
       t2 = -Vb;
       tz = 0.5*(1-t1-t2);			// zero vector
       v->Ta = t1+t2+tz;           // taon = tcon+t2
       v->Tb = tz;      // tbon = (1-t1-t2)/2
       v->Tc = t1+tz;           // tcon = tbon+t1
    }   
    else if (Sector==6)  // Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
    {
       t1 = -Vb;
       t2 = -Vc;
       tz = 0.5*(1-t1-t2);			// zero vector
       v->Ta = t1+tz;            // taon = tcon+t1
       v->Tb = t1+t2+tz;            // tbon = taon+t2 
       v->Tc = tz;      // tcon = (1-t1-t2)/2
    }
    
// Convert the unsigned GLOBAL_Q format (ranged (0,1)) -> signed GLOBAL_Q format (ranged (-1,1))
//    v->Ta = _IQmpy(_IQ(2.0),(v->Ta-_IQ(0.5)));
//   v->Tb = _IQmpy(_IQ(2.0),(v->Tb-_IQ(0.5)));
//    v->Tc = _IQmpy(_IQ(2.0),(v->Tc-_IQ(0.5)));        
*/


