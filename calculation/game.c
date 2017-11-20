#include "reg.h"

unsigned char frame_lock = 0;

void game_main()
{
	frame_lock = 0 ;
	while(1)
	{
		if(frame_lock >= 30)
		{
			frame_lock = 0;
			//LcdWriteCmd(0x01);
			LcdShowStr(0,0,"asdyhtytuytd");	
		}
	}

}
