#ifndef _ADC_H_
#define _ADC_H_

#include "types.h"

void Init_ADC(void);
f32 Read_ADC(u8 chNo);
s32 LM35_read(void);

#endif

