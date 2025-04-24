Smart Industrial Process Monitoring System (LPC2148 ARM7)


This project is a Smart Industrial Process Monitoring System developed using the LPC2148 microcontroller (ARM7TDMI-S). It is designed to continuously monitor critical industrial parameters like temperature and gas levels, and to transmit data to the cloud via ESP01 (WiFi module) for real-time analysis and logging. The project also includes a Real-Time Clock (RTC) and LCD display for local monitoring, along with an interrupt-based keypad interface for setting time parameters.

✨ Features
📡 Temperature & Gas Level Monitoring using ADC.

⏰ RTC-based timekeeping with display and edit capability via external interrupt.

📲 IoT Integration: Sends temperature data every 3 minutes to ThingSpeak using ESP01.

🚨 Gas Leak Detection: Sends alert only on change (from no-leak to leak or vice versa).

📟 16x2 LCD Display: Shows real-time temperature, gas status, date, and time.

🔘 Keypad Interrupt: Used to edit RTC time values.

🧩 Hardware Used
LPC2148 ARM7 microcontroller

MQ gas sensor

LM35 temperature sensor

ESP01 (WiFi module)

RTC DS1307

4x4 Matrix Keypad

16x2 LCD Display

Power Supply Circuit

📁 Project Structure
main3.c: Final version of main logic with RTC, LCD, gas and temperature handling.

rtc.c, lcd.c, delay.c, esp01.c, adc.c, keypad.c: Modular code for each component.

.uvproj, .uvopt: Keil project files.

.hex files: Compiled firmware ready for flashing.

🛠️ How it Works
System initializes peripherals and continuously displays sensor values.

RTC triggers a 3-minute interrupt to upload temperature data.

On gas level change, it immediately sends status update.

Time can be updated by the user using keypad and interrupt.

🚀 Getting Started
Open the Keil .uvproj file.

Compile the code (main3.c as entry point).

Flash the generated .hex file to LPC2148 using Flash Magic.

Connect sensors and ESP01 module as per circuit diagram.

Monitor data on LCD and ThingSpeak.
