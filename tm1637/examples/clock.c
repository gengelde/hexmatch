#include "../tm1637.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void signaux(int sigtype)
	{
	TMclear();
	exit(0);
	}

int main(void)
	{
	time_t t;
	struct tm tv;
	bool b=0;
	signal(SIGINT,signaux);
	signal(SIGTERM,signaux);
	TMsetup(29,28);
	TMsetBrightness(6);
	while(1)
		{		t=time(NULL);
    	localtime_r (&t,&tv);
		TMshowNumber(tv.tm_hour*100+tv.tm_min,b*0b01000000,false,4,0);
		b=1-b;
		delay(1000);
		}
	}
