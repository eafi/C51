#ifndef REG
#define REG

#include <reg52.h>

#define INTEGRITY_EXP   11      //0x0000 0x0 0x0000 $ \0
#define RESULT_EXP      6
#define ERROR_OVERFLOW  2
#define ERROR_INVALID   1
#define VALID     0	
/*c51 special*/

	   
    /*
    0   valid
    1   invalid
    2   overflow
    */

extern char error_type;



sbit KEY_IN_1 = P1^3;
sbit KEY_IN_2 = P1^4;
sbit KEY_IN_3 = P1^5;
sbit KEY_IN_4 = P1^6;
sbit KEY_IN_5 = P1^7;
sbit KEY_OUT_1 = P2^4;
sbit KEY_OUT_2 = P2^5;
sbit KEY_OUT_3 = P2^6;
sbit KEY_OUT_4 = P2^7;

void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdWriteDat(unsigned char dat);
void LcdSetCursor(unsigned char x, unsigned char y);
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void InitLcd1602();

void process_button();

void find_operator();
char process_express();
char hex_add_hex(char *exp,char s_pos,char e_pos);
char hex_minus_hex(char *exp,char s_pos,char e_pos);
void reverse_answer(char* exp);

extern char Integrity_exp[INTEGRITY_EXP];
extern char Result[RESULT_EXP];

#endif