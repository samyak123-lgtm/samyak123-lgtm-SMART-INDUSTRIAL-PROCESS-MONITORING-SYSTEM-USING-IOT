#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"
#include <stdio.h>


extern char buff[200];
extern unsigned char i;

void esp01_connectAP()
{
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT");
	delay_ms(1000);
	UART0_Tx_str("AT\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK"))
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("ATE0");
	delay_ms(1000);
	UART0_Tx_str("ATE0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK"))
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CIPMUX");
	delay_ms(1000);
	UART0_Tx_str("AT+CIPMUX=0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK"))
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CWQAP");
	delay_ms(1000);
	UART0_Tx_str("AT+CWQAP\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(1500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"OK"))
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD("AT+CWJAP");
	delay_ms(1000);
	//need to change the wifi network name and password
	UART0_Tx_str("AT+CWJAP=\"Samyak\",\"123456789\"\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(2500);
	buff[i] = '\0';
	Write_CMD_LCD(0x01);
	Write_CMD_LCD(0x80);
	Write_str_LCD(buff);
	delay_ms(2000);
	if(strstr(buff,"WIFI CONNECTED"))
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("OK");
		delay_ms(1000);		
	}
	else
	{
		Write_CMD_LCD(0xC0);
		Write_str_LCD("ERROR");
		delay_ms(1000);		
		return;
	}
	
}

void esp01_sendToThingspeak(int val, int id)
{
    char request[100], send_cmd[30];
    int len;

    Write_CMD_LCD(0x01);
    Write_CMD_LCD(0x80);
    Write_str_LCD("Connecting TS");
    delay_ms(1000);

    UART0_Tx_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(3000);  // wait for "CONNECT"
    if (strstr(buff, "CONNECT") || strstr(buff, "ALREADY CONNECTED"))
    {
        Write_CMD_LCD(0xC0);
        Write_str_LCD("Conn OK");
        delay_ms(1000);

        // Prepare the GET request
        if (id == 1)  // Temperature
        {
            len = sprintf(request, "GET /update?api_key=AW2BIMVFK6IL3ZW2&field1=%d\r\n\r\n", val);
            Write_CMD_LCD(0x01);
            Write_str_LCD("Sending Temp");
        }
        else if (id == 2)  // Gas
        {
            len = sprintf(request, "GET /update?api_key=AW2BIMVFK6IL3ZW2&field2=%d\r\n\r\n", val);
            Write_CMD_LCD(0x01);
            Write_str_LCD("Sending Gas");
        }

        delay_ms(1500);

        // Send AT+CIPSEND with calculated length
        sprintf(send_cmd, "AT+CIPSEND=%d\r\n", len);
        UART0_Tx_str(send_cmd);

        i = 0; memset(buff, 0, sizeof(buff));
        while (!strstr(buff, ">"));  // Wait for prompt
        delay_ms(500);

        // Send the actual GET request
        UART0_Tx_str(request);

        i = 0; memset(buff, 0, sizeof(buff));
        delay_ms(4000);  // wait for response

        if (strstr(buff, "SEND OK"))
        {
            Write_CMD_LCD(0x01);
            Write_str_LCD("DATA UPDATED");
        }
        else
        {
            Write_CMD_LCD(0x01);
            Write_str_LCD("UPDATE FAILED");
        }
    }
    else
    {
        Write_CMD_LCD(0x01);
        Write_str_LCD("CIPSTART FAIL");
    }

    delay_ms(2000);
}

