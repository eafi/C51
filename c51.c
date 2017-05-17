#include <reg52.h>

#define MOTOR_STOP      'e'
#define MOTOR_RIGHT     'd' 
#define MOTOR_LEFT      'a' 
#define MOTOR_BACK      's'
#define MOTOR_GO        'w' 
#define BAUD            9600

#define TIMER0_TIME     100
/*********** timer 0 settings ************/
#define TH0_VALUE       0xff;
#define TL0_VALUE       0xa3;       // 0.1ms

/*********** Trig Circle***************/
#define TRIG_CIRCLE     600     //60m：s

#define BARRIER         5

/*
*    0 for right 

*/
sbit ECHO_0 = P0 ^ 1;
sbit ECHO_1 = P0 ^ 3;
sbit TRIG_0 = P0 ^ 0;
sbit TRIG_1 = P0 ^ 2;

/*
* main.c文件
*/


void Motor_Stop();
void Motor_Go();
void Motor_Back();
void Motor_Left();
void Motor();
void Config_UART(unsigned int);
void Config_Timer0();   //0.1ms
void Ultrasonic();
void Decision_Direction();
void Motor_SelfStop();
void Send_Trig();

char    cMotorState = 'e';      //小车运动姿态
unsigned int uiEcho0_cnt;
unsigned int uiEcho1_cnt;
unsigned int ultrasonic_80ms_cnt;
unsigned int uiTrig_cnt;
unsigned int uiSelfStop_cnt;
bit bCarIsMove = 0;
char    cBufFromUART;
char    cModel = '0';   //0 for auto

int main()
{
    Config_UART(BAUD);
    Config_Timer0();
    while (1)
    {
        if (cModel == '0')
        {
            Motor();
            Decision_Direction();
            Send_Trig();
            if(cBufFromUART == '1')
                cModel = '1';

        }
        else if(cModel == '1')
        {           
            cMotorState = cBufFromUART;
            if(cMotorState == '0')
            {
                cModel = '0';
                continue;
            }       
            Motor_SelfStop();
            Send_Trig();

        }
        else continue;
    }
}
void Motor_Stop()
{
    P1 &= 0xf0;     //11110000
    P1 |= 0x00;
}
void Motor_Go()
{
    P1 &= 0xf0;     //11110000
    P1 |= 0x05;     //00000101
}
void Motor_Back()
{
    P1 &= 0xf0;
    P1 |= 0x0a;
}
void Motor_Right()
{
    P1 &= 0xf0;
    P1 |= 0x09;
}
void Motor_Left()
{
    P1 &= 0xf0;
    P1 |= 0x06;
}

void Motor()
{
    switch (cMotorState)
    {
    case 'w':
        Motor_Go();     
        break;
    case 's':
        Motor_Back();
        break;
    case 'a':
        Motor_Left();
        break;
    case 'd':
        Motor_Right();
        break;
    case 'e':
        Motor_Stop();
        break;
    default:
        break;
    }
}

void Motor_SelfStop()
{
    switch (cMotorState)
    {
    case 'w':
        Motor_Go(); bCarIsMove = 1;
        break;
    case 's':
        Motor_Back();bCarIsMove = 1;
        break;
    case 'a':
        Motor_Left();bCarIsMove = 1;
        break;
    case 'd':
        Motor_Right();  bCarIsMove = 1;
        break;
    case 'e':
        Motor_Stop(); 
        break;
    default:
        break;
    }
}
void Config_UART(unsigned int baud)
{
    EA = 1;
    SCON = 0x50;
    TMOD = 0x21;
    TH1 = 256 - (11059200 / 12 / 32) / baud;
    TL1 = TH1;
    ET1 = 0;
    ES = 1;
    TR1 = 1;
}

void Config_Timer0()
{
    TH0 = TH0_VALUE;
    TL0 = TL0_VALUE;
    ET0 = 1;
    TR0 = 1;
}

void Ultrasonic()
{
    ++uiTrig_cnt;
    if (++ultrasonic_80ms_cnt <= 800)
        return;
    ultrasonic_80ms_cnt = 0;
    if (uiTrig_cnt >= TRIG_CIRCLE)
    {
        TRIG_0 = 1; TRIG_1 = 1;
    }
    if (uiTrig_cnt >= TRIG_CIRCLE+2)
    {
        TRIG_0 = 0; TRIG_1 = 0; uiTrig_cnt = 0;
    }
    if (!ECHO_0)
    {
        uiEcho0_cnt = 0;
    }
    if (!ECHO_1)
    {
        uiEcho1_cnt = 0;
    }

}

void Decision_Direction()
{
    if (uiEcho0_cnt <= BARRIER && uiEcho1_cnt <= BARRIER&&uiEcho0_cnt&&uiEcho1_cnt)
    {
        cMotorState = 's';
    }
    else if (uiEcho0_cnt <= BARRIER && uiEcho1_cnt > BARRIER )
    {
         cMotorState = 'a';
    }else if (  uiEcho0_cnt > BARRIER && uiEcho1_cnt <= BARRIER)
    {
         cMotorState = 'd';
    }
    else if  (uiEcho0_cnt > BARRIER && uiEcho1_cnt > BARRIER)   
        cMotorState = 'w';
    else  if(uiEcho0_cnt >= uiEcho1_cnt) cMotorState = 'd';
    else   cMotorState = 'a';
}

void Send_Trig()
{
    //if (uiEcho0_cnt <= BARRIER&&uiEcho0_cnt)
    //{
    //  SBUF = uiEcho0_cnt;
    char tmp = 0;
        if (uiEcho0_cnt <= BARRIER&&uiEcho0_cnt)
    {
        tmp |= (char)uiEcho0_cnt;
        tmp |= 0x30;
        SBUF = tmp;
    }
    if (uiEcho1_cnt <= BARRIER&&uiEcho1_cnt)
    {
        tmp |= (char)uiEcho0_cnt;
        tmp |= 0x10;
        SBUF = tmp;
    }
}
//串口开启
void Interrupt_UART()   interrupt   4
{
    if (RI)
    {
        RI = 0;
        cBufFromUART = SBUF;
        //SBUF = cMotorState;
    }
    if (TI)
    {
        TI = 0;
    }
}

//0.1ms
void Interrupt_Timer0() interrupt 1
{
    TH0 = TH0_VALUE;
    TL0 = TL0_VALUE;
    if (ECHO_0)
    {
        if (++uiEcho0_cnt >=65500 )
        {
            uiEcho0_cnt = 0;
        }
    }

    if (ECHO_1)
    {
        if (++uiEcho1_cnt >= 65500)
        {
            uiEcho1_cnt = 0;
        }
    }
    Ultrasonic();
    if (++uiSelfStop_cnt>=5000 &&bCarIsMove )
    {
            //Motor_Stop();
        TR1 = 0;
        //cBufFromUART = 'e';
        Motor_Stop();
        cBufFromUART = ' '; 
        bCarIsMove = 0;
        uiSelfStop_cnt = 0; 
        TR1 = 1;    
    }
}

