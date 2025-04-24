#include <LPC21xx.H>
#include<stdio.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "keypad.h"
#include "delay.h"
#include "rtc_defines.h"

extern int temp;
extern int gas_data;

int hour,min,sec;
int day,mon,year,date;

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
void Init_RTC(void)
{
	CCR=1<<1;
	PREINT=PREINT_Val;
	PREFRAC=PREFRAC_Val;
	//CCR=1<<0;
	CCR=RTC_CCR_EN | RTC_CCR_CLKSRC;
}
void GetRTCTime(s32 *hr,s32 *mi,s32 *se)
{
	*hr=HOUR;
	*mi=MIN;
	*se=SEC;
}
void DisplayRTCTime(u32 hour,u32 min,u32 sec)
{
    SetCursor(1,0);
	Write_DAT_LCD((hour/10)+48);
	Write_DAT_LCD((hour%10)+48);
	Write_DAT_LCD(':');
	Write_DAT_LCD((min/10)+48);
	Write_DAT_LCD((min%10)+48);
	Write_DAT_LCD(':');
	Write_DAT_LCD((sec/10)+48);
	Write_DAT_LCD((sec%10)+48);
}
void GetRTCDate(s32 *date,s32 *mon,s32 *year)
{
	*date=DOM;
	*mon=MONTH;
	*year=YEAR;
}
void DisplayRTCDate(u32 date,u32 mon,u32 year)
{
	SetCursor(2,0);
	Write_DAT_LCD((date/10)+48);
	Write_DAT_LCD((date%10)+48);
	Write_DAT_LCD('/');
	Write_DAT_LCD((mon/10)+48);
	Write_DAT_LCD((mon%10)+48);
	Write_DAT_LCD('/');
	Write_int_LCD(year);
}
void GetRTCDay(s32 *day)
{
	*day=DOW;
}
void DisplayRTCDay(u32 day)
{
	SetCursor(1,10);
	Write_str_LCD(week[day]);
}
void setRTCTime(u32 hour,u32 min,u32 sec)
{
	HOUR=hour;
	MIN=min;
	SEC=sec;
}
void setRTCDate(u32 day,u32 mon,u32 year)
{
	DOM=day;
	MONTH=mon;
	YEAR=year;
}
void setRTCDay(u32 day)
{
	DOW=day;
}
void SetRTCHour(u32 houron)
{
	HOUR=houron;
}
void SetRTCMin(s32 minon)
{
	MIN=minon;
}
void SetRTCSec(s32 secon)
{
	SEC=secon;
}
/*void SetRTChour(s32 houroff)
{
	HOUR=houroff;
}
void SetRTCmin(s32 minoff)
{
	MIN=minoff;
}
void SetRTCsec(s32 secoff)
{
	SEC=secoff;
}
void SetRTCHOUR(s32 hourk)
{
	HOUR=hourk;
}
void SetRTCMIN(s32 mink)
{
	MIN=mink;
}
void SetRTCSEC(s32 seck)
{
	SEC=seck;
}*/
void SetRTCDATE(s32 datek)
{
	DOM=datek;
}
void SetRTCMONTH(s32 monthk)
{
	MONTH=monthk;
}
void SetRTCYEAR(s32 yeark)
{
	YEAR=yeark;
}
/*void SetRTCDAY(s32 dayk)
{
	DOW=dayk;
}*/

void edit(void)
{
	u32 key,data;
	Write_CMD_LCD(0x01);
	while(1)
	{
	SetCursor(1,0);
	Write_str_LCD("1.H 2.M 3.S 7.Day");
	SetCursor(2,0);
	Write_str_LCD("4.d 5.M 6.Y 8.E ");
	key=KeyDetect();
	Write_CMD_LCD(0x01);
		if(key==8)
		{
			break;
		}
	switch(key)
	{
		case 1:
					Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("HOUR:");
					data=ReadNum();
					if(data>24)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCHour(data);
					break;
		case 2:
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("MIN:");
					data=ReadNum();
					if(data>59)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCMin(data);
					break;
		case 3:
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("SEC:");
					data=ReadNum();
					if(data>59)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCSec(data);
					break;
		case 4:     
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("DATE:");
					data=ReadNum();
					if(data>31)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCDATE(data);
					break;
		case 5: 
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
				    //Write_str_LCD("MONTH:");
					data=ReadNum();
					if(data>12)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCMONTH(data);
					break;
		case 6:     
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("YEAR:");
					data=ReadNum();
					if(data>2026)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					SetRTCYEAR(data);
					break;
		case 7:
		            Write_CMD_LCD(DSP_ON_CUR_BLINK);
					//Write_str_LCD("DAY:");
					data=ReadNum();
					if(data>6)
					{
						SetCursor(2,0);
						Write_str_LCD("INVALID INPUT");
						delay_ms(150);
					}
					setRTCDay(data);
					break;
		case 8:     break;
		default:
		                Write_CMD_LCD(0x01);
						Write_str_LCD("INVALID");
						delay_ms(100);
	}
	}
}

void display(void)
{
    char buffer[20];

    Write_CMD_LCD(0x0C); // Display ON, cursor off

    // 1st Line: HH:MM:SS + T:33°C
    GetRTCTime(&hour, &min, &sec);
    sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
    SetCursor(1, 0);
    Write_str_LCD(buffer);

    sprintf(buffer, "T:%d", temp);
    SetCursor(1, 10);
    Write_str_LCD(buffer);
    Write_DAT_LCD(0xDF); // Degree symbol
    Write_DAT_LCD('C');

    // 2nd Line: DD/MM/YY + Day + Gas
    GetRTCDate(&date, &mon, &year);
    sprintf(buffer, "%02d/%02d/%02d", date, mon, year % 100); // Display last 2 digits of year
    SetCursor(2, 0);
    Write_str_LCD(buffer);

    // Day (e.g., MON) — Centered if possible
    GetRTCDay(&day);
    SetCursor(2, 9);  // Adjust to center between date and gas
    Write_str_LCD(week[day]);

    // Gas data
    SetCursor(2, 13); // Adjusted to fit 16 chars
    if (gas_data == 0)
	{
        Write_str_LCD("G:");
		Write_DAT_LCD(1);
	}
    else
	{
        Write_str_LCD("G:");
		Write_DAT_LCD('0');
	 }
		
}






	
