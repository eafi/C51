#include "reg.h"
#include <stdlib.h>
#define ENEMY_NUM	3
#define SCREEN	16
unsigned int frame_lock = 300;
unsigned int frame = 0;
unsigned int seed = 0;
char game_state = 1;		
/*
	0	die
*/
void draw_frame();
void init_game();
void process();
void collison();
typedef struct ENEMY
{
	char x;
	char y;
}Enemy;
bit player;
Enemy enemy[ENEMY_NUM];

char raw_up[SCREEN]= {0,};
char raw_down[SCREEN]= {0,};
char *game_over = "game over ;)";



void game_main()
{
	TMOD = 0x11; //设置 T0 为模式 1
	EA = 1;
	TH0 = 0xFC; //为 T0 赋初值 0xFC67，定时 1ms
    TL0 = 0x67;
    ET0 = 1; //使能 T0 中断
    TR0 = 1; //启动 T0
	init_game();
	while(1)
	{
			frame = 0;
			//LcdShowStr(0,1,"fghfh");
			
			process();
			collison();
			//selecting 
			switch(game_state)
			{
			 	case 0:
					LcdShowStr(0,1,game_over);		
				break;
				case 1:
					draw_frame();	
				break;
			}
				
			while(frame < frame_lock);
			LcdWriteCmd(0x01);
			
	}

}

void collison()
{
		char i;
		if(player)
			{
				if(raw_down[1] == 35)
					game_state = 0;		
			}
		else
			{
				if(raw_up[1] == 35)
				    game_state = 0;
			} 
//	for(i = 0;i<ENEMY_NUM;++i)
//	{	
//		if(enemy[i].x == 0 && enemy[i].y == player)
	//			game_state = 0;			
	//}
}		 
void process()
{
	char i;
	char x;
	for(i=0;i<ENEMY_NUM;++i)
	{
		if(enemy[i].x < 0)
		{
			enemy[i].x = 6;
			srand(seed);
			enemy[i].y = rand()%2 - 1;
		}
		else
		{
			--enemy[i].x;
		}
	}
	//player button detecting
	x = Button_Map[pos.x-1][pos.y-1];
	if(x == '0')
	{
		player = 1;
	}else if(x == '1')
	{
		player = 0;
	}	
}

void draw_frame()
{
	char i;
	for(i = 0;i<SCREEN;++i)
	{
		raw_up[i] = 32;
		raw_down[i] = 32;
	}
	for(i=0;i<ENEMY_NUM;++i)
	{
		if(enemy[i].y == 0)		//raw up	
		{
			raw_up[enemy[i].x] = 35;
		}else
			raw_down[enemy[i].x] = 35;
		

	}
	if(player)
	{
		raw_down[0] = 120;	
	}
	else
	{
		raw_up[0] = 120;
	} 
	LcdShowStr(0,0,raw_up);
	LcdShowStr(0,1,raw_down);
}

void init_game()
{

	enemy[0].x = 6;
	enemy[0].y = 0;
	enemy[1].x = 8;
	enemy[1].y = 1;
	enemy[2].x = 11;
	enemy[2].y = 0;
}

void interupt_button1() interrupt 1
{
	TH0 = 0xFC; //���¼��س�ֵ
	TL0 = 0x67;
	++frame;
	++seed;
}
