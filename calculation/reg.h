#include <reg52.h>

/*c51 special*/
#define LCD1602_DB  P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;

void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdWriteDat(unsigned char dat);
void LcdSetCursor(unsigned char x, unsigned char y);
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void InitLcd1602();

