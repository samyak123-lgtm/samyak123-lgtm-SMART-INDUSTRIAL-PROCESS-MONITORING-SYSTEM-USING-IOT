#include"types.h"

void Write_CMD_LCD(char);
void Write_DAT_LCD(char);
void Write_LCD(char);
void SetCursor(u8 ,u8 );
void LCD_Init(void);
void Write_str_LCD(char *);
void Write_int_LCD(signed int);//signed int(2 byte)-> -32768 to 32767
void Write_float_LCD(float f,char );


