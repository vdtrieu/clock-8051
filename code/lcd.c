#include "lcd.h"

void delay(unsigned int time){
	int i,j;
	for (i = 0; i < time; i++){
		for(j = 0; j < 123; j++);
	}
}

void LCD_cmd(unsigned char cmd){
	
	LCD_RS = 0;			//command mode
	LCD_DATA = cmd; 
	LCD_EN = 1;			//generate a pulse to allow LCD get data
	LCD_EN = 0;
	if(cmd < 0x02)
		delay(2);
	else
		delay(1);
}

void LCD_print_char(char ch){
	LCD_RS = 1;			//data mode
	LCD_DATA = ch;
	LCD_EN = 1;
	LCD_EN = 0;
	delay(1); 
}

void LCD_print(char *str){
	unsigned int index = 0;
	
	while(str[index] != 0){
		LCD_print_char(str[index]);
		index++;
	}
}

void LCD_init(){
	LCD_cmd(0x30);	
	delay(5);
	LCD_cmd(0x30);
	delay(1);
	LCD_cmd(0x30);	//according to manufacturer's instructions
	LCD_cmd(0x38); 	//2 line, font 5x8
	LCD_cmd(0x01); 	//delete content
	LCD_cmd(0x0c); 	//turn on display, no pointer
}

void LCD_goto(char row, char col){
	char pos = 0x80;
	if (row & 1)
		pos = pos | ((col & 0x0f) | 0x40);
	else
		pos = pos | (col & 0x0f);
	LCD_cmd(pos);
}

void LCD_print_number(int numb){
	char str[4];
	if(numb < 100){
		str[0] = digit[numb / 10];
		str[1] = digit[numb % 10];
		LCD_print(str);
	}
	else{
		str[0] = digit[numb / 1000];
		str[1] = digit[numb % 1000 / 100];
		str[2] = digit[numb % 100 / 10];
		str[3] = digit[numb % 1000];
		LCD_print(str);
	}
}
void LCD_print_time(char hour, char minute, char second, bit _24h_mode_){
	char str[10];
	LCD_cmd(0x84);
	if(~_24h_mode_){
		if(hour < 12) {
			str[8] = 'a';
			str[9] = 'm';
		}
		else{
			str[8] = 'p';
			str[9] = 'm';
		}
		
		if(hour>=13 && hour<=23){
			hour -= 12;
		}
		else if(hour == 0)
			hour = 12;
	}
	else{
		str[8] = ' ';
		str[9] = ' ';
	}
	str[0] = digit[hour / 10];
	str[1] = digit[hour % 10];
	str[2] = ':';
	str[3] = digit[minute / 10];
	str[4] = digit[minute % 10];
	str[5] = ':';
	str[6] = digit[second / 10];
	str[7] = digit[second % 10];
	str[10] = 0;
	LCD_print(str);
}

void LCD_print_date(char week_day, char day, char month, int year){
	char str[14];
	str[14] = 0;
	LCD_cmd(0xc1);
	str[0] = weekday[week_day][0];
	str[1] = weekday[week_day][1];
	str[2] = weekday[week_day][2];
	str[3] = ' ';
	str[4] = digit[day / 10];
	str[5] = digit[day % 10];
	str[6] = '/';
	str[7] = digit[month / 10];
	str[8] = digit[month % 10];
	str[9] = '/';
	str[10] = digit[year / 1000];
	str[11] = digit[year % 1000 / 100];
	str[12] = digit[year % 100 / 10];
	str[13] = digit[year % 10];
	
	LCD_print(str);
}