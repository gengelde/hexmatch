#include "tm1637.h"
#include <signal.h>
#include <stdlib.h>

void signaux(int sigtype)
	{
	TMclear();
	exit(0);
	}

int main(void)
	{
	int i;
	signal(SIGINT,signaux);
	signal(SIGTERM,signaux);
	TMsetup(29,28);
	TMsetBrightness(7);

	while (true)
		{
		for (i=0;i<10000;i++)
			{
			TMshowNumber(i,0,false,4,0);
			delay(1000);			
			}
		}
	return 0;
	}
