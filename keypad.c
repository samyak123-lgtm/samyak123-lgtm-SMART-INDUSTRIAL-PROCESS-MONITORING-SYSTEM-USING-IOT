#include <LPC21xx.H>

#include "defines.h"
#include "types.h"
#include "keypad_defines.h"
#include "lcd.h"
#include "lcd_defines.h"

/*
//keypad lookup table1
unsigned char keypad_lut[4][4] = {1,2,3,4,
																	5,6,7,8,
																	9,10,11,12,
																	13,14,15,16};
*/


//keypad lookup table2
unsigned char keypad_lut[4][4] ={ 1,2,3,'/',
																	4,5,6,'x',
																	7,8,9,'-',
																	'C',0,'=','+'};



/*
//keypad lookup table3
unsigned char keypad_lut[4][4] = {'7','8','9','/',
																	'4','5','6','x',
																	'1','2','3','-',
																	'C','0','=','+'};

*/

//variables declaration
unsigned char row_val,col_val;
unsigned int c0,c1,c2,c3;

void Init_KPM()
{
	//configuring the rows port pins as output port pins
	//IODIR1 = ((IODIR1&~(15<<ROW0))|(15<<ROW0));
	WRITENIBBLE(IODIR1,ROW0,15);
	
	//initially making all rows to zero 
	//initially making all columns to one
}	

//read the all columns status and any one of the column 
//is 0 then switch is pressed. if switch is pressed return 0 
//otherwise return 1																
u32 ColScan(void)
{
	//return ((IOPIN1>>COL0)&15)<15?0:1;
	return READNIBBLE(IOPIN1,COL0)<15?0:1;
}

//row identification process
u32 RowCheck(void)
{
	u32 rNo;
	
	for(rNo=0;rNo<=3;rNo++)
	{
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rNo));
		if(ColScan() == 0)
			break;	
	}	
	return rNo;
}

//columns identification process
//if col0 status is low then column value is 0
//else if col1 status is low then column value is 1
//else if col2 status is low then column value is 2
//else if col3 status is low then column value is 3
u32 ColCheck(void)
{
	u32 cNo;
	
	for(cNo=0;cNo<=3;cNo++)
	{
		if(READBIT(IOPIN1,(COL0+cNo)) == 0)
			break;
	}
	return cNo;
}	

u32 KeyDetect(void)
{
	//making all rows to zero 
	IOCLR1 = (1<<ROW0 | 1<<ROW1 | 1<<ROW2 | 1<<ROW3);
	//waiting for switch press
	while(ColScan());
		
  //row identification process
	row_val = RowCheck();

	//columns identification process
	col_val = ColCheck();
		
	//waiting for switch release
	while(!ColScan());

	//collect the pressed switch equivalent value from defined keypad lookup table and display it on any o/p device		
	return keypad_lut[row_val][col_val];	
}

u32 ReadNum(void)
{
	s32 keyC,num=0;
	s32 pos=0;
	
	while(1)
	{
		keyC = KeyDetect();
		
		if(keyC>=0 && keyC<=9)
		{
			num = (num*10) + keyC;
			Write_DAT_LCD(keyC+48);
			pos++;
		}
		else if(keyC == 'C')
		{
			pos--;
			
			if(pos>=0)
			{
				Write_CMD_LCD(0x80+pos);
				Write_DAT_LCD(' ');
				Write_CMD_LCD(0x80+pos);
				num=num/10;
			}
			else
			{
				pos=0;
			}
		}
		
		else if(keyC == '=')
		{
			break;
		}
		
	}
	return num;
}

