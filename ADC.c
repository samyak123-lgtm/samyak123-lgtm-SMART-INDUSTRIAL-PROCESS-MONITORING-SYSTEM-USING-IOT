#include <LPC214x.h>

#include "types.h"
#include "defines.h"
#include "adc_defines.h"
#include "delay.h"

#define ADC_FUNC 0x01 
s32 temp1;
void Init_ADC(void)
{
  PINSEL1 |= 0x15400000;//(ADC_FUNC<<28);  //configure P0.28 as ADC input
  AD0CR=PDN_BIT|CLKDIV|CHANNEL_SEL;	
}

f32 Read_ADC(u8 chNo)
{
    u32 adcVal=0;
	f32 eAR;
	WRITEBYTE(AD0CR,0,chNo);
	SETBIT(AD0CR,ADC_START_BIT);
	delay_us(3);
	while(!READBIT(AD0GDR,DONE_BIT));
	CLRBIT(AD0CR,ADC_START_BIT);
	adcVal=(AD0GDR>>6)&0x3FF;
	eAR=((adcVal*3.3)/1023);
	return eAR;
}

s32 LM35_read(void)
{
	temp1 =Read_ADC(CH1)*100;
	return temp1;
}
