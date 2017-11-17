#include "reg.h"

unsigned char idata KeySta[4][5] = { 
{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1 }, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}};
char error_type = 0;
/*
    {
        7,8,9,A,Clear
        4,5,6,B,+
        1,2,3,C,-
        0,F,E,D,=
    }
*/

unsigned char idata Button_Map[4][5] = { 
    {'7', '8', '9', 'A','G' },
    {'4', '5', '6', 'B', '+'},
    {'1', '2', '3', 'C', '-'},
    {'0', 'F', 'E', 'E', '='}};

char idata error_overflow[] = {"overflow!"};
char idata error_invalid[] = {"exp invalid!"};
char idata error_press_clear[] = {"press clear..."};
void add_value_from_button(char val);
void process_error(char val);
void clear();
void process_button()
{
    unsigned char i, j;
    unsigned char backup[4][5] = { //按键值备份，保存前一次的值
    {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1 }, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}};
    
    	EA = 1; //使能总中断
    	TMOD = 0x01; //设置 T0 为模式 1
    	TH0 = 0xFC; //为 T0 赋初值 0xFC67，定时 1ms
    	TL0 = 0x67;
    	ET0 = 1; //使能 T0 中断
    	TR0 = 1; //启动 T0
    	while (1)
    	{
    		for (i=0; i<4; i++) //循环检测 4*4 的矩阵按键
    		{
    			for (j=0; j<5; j++)
    			{
    				if (backup[i][j] != KeySta[i][j]) //检测按键动作
    				{
    					if (backup[i][j] != 0) //按键按下时执行动作
    					{
    						//add_value_from_button(*Button_Map[i*4+j]); //将编号显示到数码管

							LcdShowStr(0,0,"asd");
    					}
    				backup[i][j] = KeySta[i][j]; //更新前一次的备份值
    				}
    			}
    		}
    	}
}

void add_value_from_button(char val)
{
    static int pos = 0;
    if(pos >= INTEGRITY_EXP)
    {
        LcdShowStr(0,0,error_overflow);
        LcdShowStr(1,0,error_press_clear);
        pos = 0;
    }
    else 
    {
        if(val == '=')
        {
            find_operator();
            process_error(process_express());
        }
        else if(val == 'G')
        {
            clear();
        }
        else 
        {
            Integrity_exp[pos++] = val;
            LcdShowStr(0,0,Integrity_exp);
        }
    }
}

void interupt_button()
{
unsigned char i;
static unsigned char keyout = 0; //矩阵按键扫描输出索引
static unsigned char keybuf[4][5] = { //矩阵按键扫描缓冲区
{0xff, 0xff, 0xff, 0xff,0xff}, {0xff, 0xff, 0xff, 0xff,0xff},
{0xff, 0xff, 0xff, 0xff,0xff}, {0xff, 0xff, 0xff, 0xff,0xff}
};
TH0 = 0xFC; //重新加载初值
TL0 = 0x67;

//将一行的 4 个按键值移入缓冲区
keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;
keybuf[keyout][4] = (keybuf[keyout][4] << 1) | KEY_IN_5;
//消抖后更新按键状态
	for (i=0; i<5; i++) //每行 4 个按键，所以循环 4 次
	{
		if ((keybuf[keyout][i] & 0x0F) == 0x00)
		{ //连续 4 次扫描值为 0，即 4*4ms 内都是按下状态时，可认为按键已稳定的按下
			KeySta[keyout][i] = 0;
		}
		else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
		{ //连续 4 次扫描值为 1，即 4*4ms 内都是弹起状态时，可认为按键已稳定的弹起
			KeySta[keyout][i] = 1;
		}
	}
//执行下一次的扫描输出
	keyout++; //输出索引递增
	keyout = keyout & 0x03; //索引值加到 4 即归零
	switch (keyout) //根据索引，释放当前输出引脚，拉低下次的输出引脚
	{
		case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
		case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
		case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
		case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
		default: break;
	}
}

void interupt() interrupt 1
{
    interupt_button();
}

void process_error(char val)
{
    switch(val)
    {
        case 2:
            LcdShowStr(0,0,error_overflow);
            LcdShowStr(1,0,error_press_clear);
            break;
        case 1:
            LcdShowStr(0,0,error_overflow);
            LcdShowStr(1,0,error_press_clear);
            break;
        case 0:
            LcdShowStr(1,0,Result);
            break;
    }
}

void clear()
{
    char i = 0;
    for(;i<INTEGRITY_EXP;++i)
    {
        Integrity_exp[i] = '0';
        if(i<RESULT_EXP)
        {
            Result[i] = '0';
        }
    }
    LcdWriteCmd(0x01);
}

