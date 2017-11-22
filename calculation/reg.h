#ifndef REG
#define REG
#include <reg52.h>
#define INTEGRITY_EXP 11
#define RESULT_EXP 5
#define VALID		0
#define ERROR_INVALID	1
#define ERROR_OVERFLOW	2

/*c51 special*/

	   
    /*
    0   valid
    1   invalid
    2   overflow
    */

extern void LcdWaitReady();
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
extern void LcdSetCursor(unsigned char x, unsigned char y);
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
extern void InitLcd1602();

extern char add_value_from_button(char val);
extern void process_error(char val);
extern void find_operator();
extern char process_button();
extern void clear();

extern void find_operator();
extern char process_express();

extern void game_main();
extern char Integrity_exp[INTEGRITY_EXP];
extern char Result[RESULT_EXP];
typedef struct POS_TYPE
{
    char x;
    char y;
}POS;
extern POS pos;
extern unsigned char code Button_Map[4][5];
#endif