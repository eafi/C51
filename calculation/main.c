#include "reg.h"

void delay_ms(unsigned int ms)
{
	unsigned int x,y;
	for(x=ms;x>0;x--)
		for(y=110;y>0;y--);
}
int main()
{
	InitLcd1602();

//	LcdShowStr(0,0,"asdyhtytuytd");
	process_button();
	LcdWriteCmd(0x01);
	LcdShowStr(0,0,"woo,bug.....");
	LcdShowStr(0,1,"u did sth wrong.");
	delay_ms(1000);
 
	LcdWriteCmd(0x01);
	LcdShowStr(0,0,"welcm to real");
	LcdShowStr(0,1,"press '0 || 1'");
	delay_ms(1000);
	LcdWriteCmd(0x01);
	game_main();
//	game_main();
	while(1);			
	//return 0;
}
