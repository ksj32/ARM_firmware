#include "2450addr.h"
#include "my_lib.h"
#include "global_var.h"

#define DURATION_1SEC	1000

void Touch_Init(void);

extern unsigned int HandleADC;
extern unsigned int fullscreen_buffer[272][480];
extern unsigned int cookie_buffer[44][44];
extern unsigned int slide_buffer[22][56]; 

void Touch_Init(void)
{
	rADCDLY = (50000);		/* ADC start or interval delay */
	
	rADCCON |= (0x1<<14);	/* ADC Control */
	rADCCON |= (39<<6);
	rADCCON &= ~(0x1<<3);
	rADCCON &= ~(0x1<<2);
	rADCCON &= ~(0x1<<1);
	rADCCON &= ~(0x1);

	rADCTSC = (0x0d3);	//interrupt mode & detect stylus down (Waiting for interrupt mode)
		
	HandleADC = (unsigned int)Touch_ISR;
}

volatile int ADC_x, ADC_y;
                                                          
void Touch_ISR()
{
	/* 인터럽트 마스킹 */
	rINTSUBMSK |= (0x1<<9);
	rINTMSK1 |= (0x1<<31);	
	
	/* 펜딩 클리어 */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	rSUBSRCPND |= (0x1<<9);
	rSRCPND1 |= (0x1<<31);
	rINTPND1 |= (0x1<<31);

	if(rADCTSC & 0x100) 	//안 누른 경우
	{
		rADCTSC &= (0xff); 			//ADC touch screen control
		position = 0;
	}
	
	else 	//누른 경우 
	{
		rADCTSC &=~(0x1<<8); //detect stylus down
		
		rADCTSC &= ~(0x3);
		rADCTSC |= (0x1<<2);
		
		rADCCON |= (0x1); //ENABLE_START 
	
		/* wait until End of A/D Conversion */
		while(!(rADCCON & (1<<15)));
			rADCCON &= ~(0x1); //Stop Conversion (= No operation)
		
		ADC_x = (rADCDAT0 & 0x3ff);
		ADC_y = (rADCDAT1 & 0x3ff);
	
		if((ADC_x >= 760) && (ADC_x <= 840) && (ADC_y >= 578) && (ADC_y <= 640)) //첫 번째 버튼 터치 감지 범위 
		{
			position = 1;
			Buzzer_Beep(C1, 20);
		}
		if((ADC_x >= 760) && (ADC_x <= 840) && (ADC_y >= 488) && (ADC_y <= 562)) //두 번째 버튼 터치 감지 범위 
		{
			position = 2;
			Buzzer_Beep(D1, 20);
		}
		if((ADC_x >= 760) && (ADC_x <= 840) && (ADC_y >= 400) && (ADC_y <= 465)) //세 번째 버튼 터치 감지 범위 
		{
			position = 3;
			Buzzer_Beep(E1, 20);
		}
		if((ADC_x >= 760) && (ADC_x <= 840) && (ADC_y >= 320) && (ADC_y <= 380)) //네 번째 버튼 터치 감지 범위 
		{
			position = 4;
			Buzzer_Beep(F1, 20);	
		}
		
		/*if(press_slide == 0 && press_jump < 2 && ADC_x >= 650 && ADC_y <= 440)
		{
			CLEAR_COOKIE
			press_jump++;
		}
		else if(press_jump == 0 && ADC_x <= 360 && ADC_y <= 440)
		{
			CLEAR_COOKIE
			press_slide = 1;
		}*/
		
		rADCTSC = (0x1d3);		//interrupt mode & detect stylus up 
	}
	rINTSUBMSK &= ~(0x1<<9);	//마스킹 해제(인터럽트 허용) 
	rINTMSK1 &= ~(0x1<<31); 
}
