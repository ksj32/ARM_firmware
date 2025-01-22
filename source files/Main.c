#include "./Images/BG2.H"
#include "./Images/START.H"
#include "./Images/RUN.H"
#include "./Images/SLIDE.H"
#include "./Images/ttt.h"
#include "./Images/tt.h"

//#include "iis.c"
#include "sdi.c"
#include "buzzer.c"
#include "2450addr.h"
#include "my_lib.h"
#include "global_var.h"

#define UART0  0    

//16 color
#define BLACK	0x0000
#define WHITE	0xFFFF
#define BLUE	0x001F		
#define GREEN	0x03E0		
#define RED	    0x7C00		

int random1 = 0;			//ù ��° ��Ʈ ���� ��  
int random2 = 0;			//�� ��° ��Ʈ ���� ��

int position = 0;		
int score = 0;				//���� ���ھ�  

unsigned short (* Fb_ptr)[480];	//lcd0 buffer

int start_x1 = 480;			//��� ���� ��ġ(x��ǥ) 
int start_x2 = 0;			//��� ���� ��ġ(y��ǥ) 

int hit_check1 = 0;			//��� ��Ʈüũ1 
int hit_check2 = 0;			//��� ��Ʈüũ2

int Miss = 0; 		//��Ʈ�� ��ģ ��� 
int Perfect = 0; 	//��Ʈ�� ���� ��� 
int my_clear; 		//Ŭ���� 

int temp_start_x1 = 0;		//block1 & block2 distance store var
int temp_start_x2 = 0;

int level = 0;		//current game level var

char ptr[10][10] = {"PERFECT","MISS   ","       "};		//���¸� �����ϴ� �迭 

extern unsigned int fullscreen_buffer[272][480];
extern unsigned int clear_buffer[272][100];

void Main(void)
{	
	/* driver init */
	MMU_Init();						
	Uart_Init(115200);							//115200�� Serial ��� �ӵ�(Baud rate)			
	Graphic_Init();
	SWITCH_Port_Init();
	Timer_Init();	
	Touch_Init();
	Buzzer_Init();

	Lcd_Select_Frame_Buffer(1); 
	Lcd_Draw_BMP(0, 0, tt); 					//����ȭ�鿡 �̹��� �����ֱ�(Draw) 
	Lcd_Copy(1, 0);
	
	Lcd_Select_Frame_Buffer(0);
	Lcd_Make_fullscreen_Buffer(0, 0, ttt); 		//���� ȭ�� �� �̹��� �����ֱ�  
	Lcd_Display_Frame_Buffer(0);	
}

void game_over(void)  	//40�� ��� �Ǵ� �� ������ -100���� ���  
{
	static int first_score = 0;
	static int second_score = 0;
	static int third_score = 0;

	rTCON &= ~(0x1);			//timer stop
	rTCON &= ~(0x1<<8);
	rTCON &= ~(0x1<<12);
	
	/*����ũ �����*/
	rINTSUBMSK |= (0x1<<9);	
	rINTMSK1 |= (0x1<<31);		
	
	if(my_clear == 0)
		Lcd_Puts_big(168,40,0xffff,fullscreen_buffer,"GAME OVER");
	else
		Lcd_Puts_big(168,60,0x0000,fullscreen_buffer," CLEAR ");
		Lcd_Printf(198,126,0xFFFF,fullscreen_buffer,"PERFECT : %d", Perfect); 
		Lcd_Printf(198,152,0xFFFF,fullscreen_buffer,"MISS : %d", Miss);
		Lcd_Printf(198,178,0xFFFF,fullscreen_buffer,"Score : %d", score);

		Miss = 0;
		Perfect = 0;
}
