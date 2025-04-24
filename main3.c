#include <LPC214X.H>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"
#include "esp01.h"
#include "keypad.h"
#include "adc.h"
#include "rtc.h"
#include "defines.h"
#include"types.h"

#define MQ2 25

void eint1_isr(void) __irq;
void Enable_EINT1(void);

int temp, gas_data;
int last_sent_min = -1;
int gas_flag = -1;  // Use -1 to indicate that we haven't sent anything yet

u8 filled_zero[8]={ 0X0E,0X1F,0X1B,0X1B,0X1B,0X1B,0X1F,0X0E};
u8 solid_circle[8]={ 0X0E,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X0E};

void BuildCGRAM()
{
    u32 i;
    Write_CMD_LCD(0x40);  // Set CGRAM address
    for(i=0;i<8;i++)
	{
		Write_DAT_LCD(filled_zero[i]);
	}
	Write_CMD_LCD(0x48);
	for(i=0;i<8;i++)
	{
		Write_DAT_LCD(solid_circle[i]);
	}
    Write_CMD_LCD(0x80);  // Return to DDRAM
}

int main()
{
    Init_UART0();
    LCD_Init();  
    Init_KPM();
    Init_ADC();
    Init_RTC();
    Enable_EINT1();
		BuildCGRAM();
			
		Write_CMD_LCD(0x0C);
    Write_CMD_LCD(0x80);
    Write_str_LCD("PROJECT DISPLAY");
    delay_ms(1000);  

    setRTCTime(12, 12, 00);
    setRTCDate(22, 4, 2025);
    setRTCDay(2);
        
    Write_CMD_LCD(0x01);
    Write_CMD_LCD(0x80);
    Write_str_LCD("Connect ESP01");
    delay_ms(1000);
    esp01_connectAP();
    delay_ms(1000);

    while(1)
    {
        temp = LM35_read();
        gas_data = ((IOPIN0 >> MQ2) & 1);
        
        Write_CMD_LCD(0x01);
		//Write_str_LCD("                  ");
        display();
        //delay_ms(2000);
        
        if (last_sent_min == -1 || (MIN - last_sent_min + 60) % 60 >= 3)
        {
            Write_CMD_LCD(0x01);
            Write_CMD_LCD(0x80);
            Write_str_LCD(" sending temp");
            //delay_ms(1000);
            esp01_sendToThingspeak(temp, 1);
            delay_ms(16000);  // wait to prevent overlapping with gas
            last_sent_min = MIN;
        }
        
        // Initially check if gas is not detected and send the "no smoke" signal (0) first
        if (gas_flag == -1)
        {
            if (gas_data == 1) // No smoke detected
            {
                esp01_sendToThingspeak(0, 2); // Send 0 to indicate no smoke
                delay_ms(16000);
                gas_flag = 0; // Mark that no-smoke was sent
            }
            else if (gas_data == 0) // Smoke detected
            {
                esp01_sendToThingspeak(1, 2); // Send 1 to indicate smoke
                delay_ms(16000);
                gas_flag = 1; // Mark that smoke was sent
            }
        }
        else if (gas_data == 0 && gas_flag != 1)
        {
            // Smoke detected
            esp01_sendToThingspeak(1, 2); // Send 1 to indicate smoke
            delay_ms(16000);
            gas_flag = 1; // Mark that smoke was sent
        }
        else if (gas_data == 1 && gas_flag != 0)
        {
            // No smoke, and we haven't sent '0' yet
            esp01_sendToThingspeak(0, 2); // Send 0 to indicate no smoke
            delay_ms(16000);
            gas_flag = 0; // Mark that no-smoke was sent
        }
        
        delay_ms(500);  
    }
}

void Enable_EINT1(void)
{
    PINSEL0 |= (3 << 6);
    SSETBIT(VICIntEnable, 15);
    VICVectCntl1 = 0x20 | 15;
    VICVectAddr1 = (unsigned long)eint1_isr;
    
    // Enable EINT0 
    SSETBIT(EXTINT, 1);  // default 0
}  

void eint1_isr(void) __irq
{
    Write_CMD_LCD(0x01);
    Write_CMD_LCD(0x80);
    Write_str_LCD("RESET");
    delay_ms(1000);

    edit();
    last_sent_min = -1;
    				   
    EXTINT = 1 << 1;
    VICVectAddr = 0;
}




