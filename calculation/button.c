#include "reg.h"
sbit KEY_IN_1 = P1^3;
sbit KEY_IN_2 = P1^4;
sbit KEY_IN_3 = P1^5;
sbit KEY_IN_4 = P1^6;
sbit KEY_IN_5 = P1^7;
sbit KEY_OUT_1 = P2^4;	
sbit KEY_OUT_2 = P2^5;
sbit KEY_OUT_3 = P2^6;
sbit KEY_OUT_4 = P2^7;


POS pos;

char add_value_from_button(char val);
void process_error(char val);
void find_operator();
char process_button();
void clear();
char fun();
void game();
char error_type = 0;
/*
    {
        7,8,9,A,Clear
        4,5,6,B,+
        1,2,3,C,-
        0,F,E,D,=
    }
*/

unsigned char code Button_Map[4][5] = { 
    {'7', '8', '9', 'A','G' },
    {'4', '5', '6', 'B', '+'},
    {'1', '2', '3', 'C', '-'},
    {'0', 'F', 'E', 'D', '='}};

char code error_overflow[] = {"overflow!"};
char code error_invalid[] = {"exp invalid!"};
char code error_press_clear[] = {"press clear..."};
char code str_fun[] = {"BABA"};
char code str_got[] = {"code got! 'press ='"};
static char pos_cnt = 0;
bit select_timer = 0;
char process_button()
{
		char x;
		char same_flag = 0;

//    unsigned char i, j;   
    	EA = 1; //ä½¿èƒ½æ€»ä¸­æ–­
    	TMOD = 0x10; //è®¾ç½® T0 ä¸ºæ¨¡å¼ 1

		TH1 = 0xFC; //ä¸º T0 èµ‹åˆå€¼ 0xFC67ï¼Œå®šæ—¶ 1ms
    	TL1 = 0x67;
    	ET1 = 1; //ä½¿èƒ½ T0 ä¸­æ–­
    	TR1 = 1; //å¯åŠ¨ T0

		//char down = '1';
    	while (1)
    	{
			if(pos.y != 0 && pos.x != 0  )
			{
				x = Button_Map[pos.x-1][pos.y-1];
				//LcdShowStr(pos_cnt++,1,&x);
				if(add_value_from_button(x))
				{
				 	return 1;	
				}
				pos.x = 0;pos.y = 0;		
			}								
    	}
}

char add_value_from_button(char val)
{
    
    if(pos_cnt >= INTEGRITY_EXP)
    {
        LcdShowStr(0,0,error_overflow);
        LcdShowStr(0,1,error_press_clear);
        pos_cnt = 0;
    }
    else 
    {
        if(val == '=')
        {
			if(!fun())
			{
				Integrity_exp[pos_cnt] = '#';
            	find_operator();
            	process_error(process_express());
			}else
			{
				return 1;
			}

        }
        else if(val == 'G')
        {
            clear();
        }
        else 
        {
            Integrity_exp[pos_cnt++] = val;
			LcdWriteCmd(0x01);
            LcdShowStr(0,0,Integrity_exp);
        }
    }
	return 0;
}

char fun()
{
	char i=0;
	for(;i<4;++i)
	{
		if(Integrity_exp[i] != str_fun[i])
			return 0;
	} return 1;
}

void interupt_button3() interrupt 3
{
	char i = 0;	
	

	static unsigned char keyout = 0; //¾ØÕó°´¼üÉ¨ÃèÊä³öË÷Òý
	//TH1 = 0xFC; //ÖØÐÂ¼ÓÔØ³õÖµ
	//TL = 0x67;
 	if(!KEY_IN_1)
		{
			pos.x = keyout+1;
			pos.y = 1;
		}
 	else if(!KEY_IN_2)
		{
			pos.x = keyout+1;
			pos.y = 2;
		}
	else if(!KEY_IN_3)
		{
			pos.x = keyout+1;
			pos.y = 3; 
		}
	else if(!KEY_IN_4)
		{
			pos.x = keyout+1;
			pos.y = 4;
		}
	else if(!KEY_IN_5)
		{
			pos.x = keyout+1;
			pos.y = 5;
		}


//Ö´ÐÐÏÂÒ»´ÎµÄÉ¨ÃèÊä³ö
keyout++; //Êä³öË÷ÒýµÝÔö
keyout = keyout & 0x03; //Ë÷ÒýÖµ¼Óµ½ 4 ¼´¹éÁã
switch (keyout) //¸ù¾ÝË÷Òý£¬ÊÍ·Åµ±Ç°Êä³öÒý½Å£¬À­µÍÏÂ´ÎµÄÊä³öÒý½Å
{
case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
default: break;
}

}

void process_error(char val)
{
    switch(val)
    {
        case 2:
            LcdShowStr(0,0,error_overflow);
            LcdShowStr(0,1,error_press_clear);
            break;
        case 1:
            LcdShowStr(0,0,error_invalid);
            LcdShowStr(0,1,error_press_clear);
            break;
        case 0:
            LcdShowStr(0,1,Result);
            break;
    }
}

void clear()
{
    char i = 0;
    for(;i<INTEGRITY_EXP;++i)
    {
        Integrity_exp[i] = '\0';
        if(i<RESULT_EXP)
        {
            Result[i] = '\0';
        }
    }
	pos_cnt = 0;
    LcdWriteCmd(0x01);
}



