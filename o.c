#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
typedef struct tm tm_t;
typedef struct timeb timeb_t;
//Battery
const char* GET_BATTERY_STAT = "cat /sys/class/power_supply/BAT1/status";
const char* GET_BATTERY_FULL = "cat /sys/class/power_supply/BAT1/energy_full";
const char* GET_BATTERY_NOW = "cat /sys/class/power_supply/BAT1/energy_now";

//Brightness
const char* GET_BRIGHTNESS = "xrandr --verbose | grep Brightness";

//Volume
const char* GET_VOLUME = "amixer get -c 0 Master | grep '[[0-9%]]'";
const char* VOLUME_UP = "pactl set-sink-volume 0 +10%";
const char* VOLUME_DOWM = "pactl set-sink-volume 0 -10%";

int BatteryInit=1;
char Result[1000];
void ExecCommand(const char* commandStr)
{
	FILE* fp;
	memset(Result,0,1000);
	fp=popen(commandStr, "r");
	fgets(Result, 1000, fp);
	pclose(fp);
}
char mon[10];
char day[10];
char hour[10];
char min[10];
char sec[10];
char Weekday[7][5]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
tm_t CurrentTime;
short ms;
char Buffer[100];
void DateFormat(int num,char *s)
{
	if(num>=10)sprintf(s,"%d",num);
	else sprintf(s,"0%d",num);
}
void GetDateTime()
{
	timeb_t t;
	ftime(&t);
	ms=t.millitm;
	CurrentTime=*localtime(&t.time);
}
int Charging = 0;
double BatteryFull = 0.0;
double BatteryNow = 0.0;
double Charge = 0.0;
char BatteryIcon[5];
char ChargeIcon[5];
char Icons[7][4]={"","","","",""," "};
void SetIcon(int index,char* target)
{
	for(int i=0;i<4;i++)
	{
		target[i]=Icons[index][i];
	}
}
void GetBatteryStat()
{
	if(BatteryInit)
	{
		ExecCommand(GET_BATTERY_FULL);
		BatteryInit=0;
		BatteryFull = (double)atoi(Result);
	}
	ExecCommand(GET_BATTERY_STAT);
	if(Result[0]=='D')Charging=0;
	else Charging=1;
	ExecCommand(GET_BATTERY_NOW);
	BatteryNow = (double)atoi(Result);
	Charge = BatteryNow / BatteryFull;
	if(Charge<=0.125)SetIcon(0,BatteryIcon);
	if(Charge>0.125&&Charge<=0.375)SetIcon(1,BatteryIcon);
	if(Charge>0.375&&Charge<=0.625)SetIcon(2,BatteryIcon);
	if(Charge>0.625&&Charge<=0.875)SetIcon(3,BatteryIcon);
	if(Charge>0.875&&Charge<=1.000)SetIcon(4,BatteryIcon);
	if(Charging)SetIcon(5,ChargeIcon);
	else memset(ChargeIcon,0,5);
}
void Wait()
{
	GetDateTime();
	while(ms!=0)
	{
		GetDateTime();
	}
}
char DateTime[100];
int Volume = 0;
void GetVolume()
{
	ExecCommand(GET_VOLUME);
	int flag=0;
	int j = 0;
	char vol[10];
	memset(vol,0,10);
	Volume = 0;
	for(int i=0;i<500;i++)
	{	
		if (Result[i]=='%')break;
		if (flag)
		{
		       	vol[j] = Result[i];
			j++;
		}
		if (Result[i]=='[') flag = 1;
	}
	if (vol[2]) Volume = 100;
	else if(!vol[1]) Volume = (int)vol[0]-48;
	else 
	{
		Volume += ((int)vol[0]-48)*10;
		Volume += ((int)vol[1]-48);
	}

}
int main()
{
	Wait();
	while(1)
	{
		GetDateTime();
		DateFormat(CurrentTime.tm_mon+1,mon);
		DateFormat(CurrentTime.tm_mday,day);
		DateFormat(CurrentTime.tm_hour,hour);
		DateFormat(CurrentTime.tm_min,min);
		DateFormat(CurrentTime.tm_sec,sec);
		sprintf(DateTime," %s-%s %s %s:%s:%s",
				mon,
				day,
				Weekday[CurrentTime.tm_wday],
				hour,
				min,
				sec);
		GetBatteryStat();
		GetVolume();
		sprintf(Buffer,"xsetroot -name \"%s | VOL %d% | %s%s %d%\"",DateTime,Volume,ChargeIcon, BatteryIcon, (int)(100.0 * Charge));
		ExecCommand(Buffer);
		Wait();
	}
}
