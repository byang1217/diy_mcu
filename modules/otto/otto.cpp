//-- Otto Library
#include "otto.h"
#include <OttoARM.h>
Otto Otto;  //This is Otto!

int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int modeId=0;            //Number of mode
int moveSize=15;         //Asociated with the height of some movements

unsigned long previousMillis=0;

int otto_init(void)
{
	Otto.init(2,3,4,5,6,7,false,8,9,10,11);
	Otto.home();
	previousMillis = millis();
}

int otto_shell_handle(int argc, char **argv)
{
	static int init;

	if (!init) {
		otto_init();
		init = 1;
	}
}
