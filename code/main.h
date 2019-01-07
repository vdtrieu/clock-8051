#ifndef _MAIN_H_
#define _MAIN_H_

#include <REGX52.H>
#include "lcd.h"
#include "LunarCalendar.h"

#define FREQ_OSC	12000000ul
#define DEBOUNCE	100
#define BLINK			200
#define LCD_DATA	P2
#define LCD_RS  	P3_0
#define LCD_EN 	 	P3_1
#define BTN_SET		P3_4

typedef struct{
	char hour;
	char minute;
	char second;
	char weekday;
	char day;
	char month;
	int	 year;
	char lunarday;
	char lunarmonth;
	int  lunaryear;
} Time;

void init_time();
char day_of_week(char day, char month, int year);
void increase_second();

#endif