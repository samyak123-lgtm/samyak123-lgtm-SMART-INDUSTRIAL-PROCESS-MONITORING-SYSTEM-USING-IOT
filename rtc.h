//#include"header.h"
#ifndef _RTC_H
#define _RTC_H
#include "types.h"



void Init_RTC(void);
void GetRTCTime(s32 *,s32 *,s32 *);
void DisplayRTCTime(u32,u32,u32 );
void GetRTCDate(s32 *, s32 *,s32 *);
void DisplayRTCDate(u32,u32,u32);
void GetRTCDay(s32 *);
void DisplayRTCDay(u32);
void setRTCTime(u32,u32,u32);
void setRTCDate(u32,u32,u32);
void setRTCDay(u32);
void SetRTCHour(s32);
void SetRTCMin(s32);
void SetRTCSec(s32);
void SetRTCDATE(s32);
void SetRTCMONTH(s32);
void SetRTCYEAR(s32);
void setRTCDay(u32);

void edit(void);
void display(void);

#endif

