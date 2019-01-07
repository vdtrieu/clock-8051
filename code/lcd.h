#ifndef _LCD_H_
#define _LCD_H_
#include "main.h"
#define OFF_WEEKDAY	7
#define LUNAR				8
static char code digit[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
static char code weekday[9][3] = { {'M','O','N'}, 
															{'T','U','E'}, 
															{'W','E','D'}, 
															{'T','H','U'}, 
															{'F','R','I'}, 
															{'S','A','T'},
															{'S','U','N'},
															{'-','-','-'},
															{'L','N','R'}};

void delay(unsigned int time);
void LCD_init();
void LCD_cmd(unsigned char cmd);
void LCD_print_char(char ch);
void LCD_print(char *str);
void LCD_print_number(int numb);
void LCD_print_time(char hour, char minute, char second, bit _24h_mode_);
void LCD_print_date(char week_day, char day, char month, int year);
void LCD_goto(char row, char col);

#endif