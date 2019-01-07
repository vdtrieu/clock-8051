#include "main.h"

// time parameter
volatile unsigned int timer_counter = 20;
Time time;
bit cur_sec_stat 			= 0;
bit last_sec_stat 		= 0;
bit cur_24h_stat			= 1;
bit last_24h_stat			= 1;
bit cur_cal_stat 			= 0;
bit last_cal_stat 		= 0;
char settings					= 1;

void main(){
	TMOD &= 0xF0; // set timer0
	TMOD |= 0x01; // set mode 1 time0
	TH0 	= 0x3C;		// initial value count 50.000us = 50ms
	TL0 	= 0xB0;
	ET0 	= 1;			// enable timer0 interrupt
	IT0 	= 1;			// enable falling edge on INT0/BTN1
	IT1 	= 1; 			///enable falling edge on INT1/BTN2
	EX0 	= 1;			// enable external interrupt 0
	EX1 	= 1;			// enable external interrupt 1
	EA 		= 1;				// enable all interrupt
	TR0 	= 1;			// start timer0
	
	init_time();
	LCD_init();
	
	SETTINGS:{
		while(1){
			if(settings == 1){ 
				LCD_cmd(0x84);
				LCD_print("  ");
			}
			else if(settings == 2){ 
				LCD_cmd(0x87);
				LCD_print("  ");
			}
			else if(settings == 3){					
				LCD_cmd(0x8a);
				LCD_print("  ");
			}
			else if(settings == 4){ 
				LCD_cmd(0xcb);
				LCD_print("    ");
			}
			else if(settings == 5){ 
				LCD_cmd(0xc8);
				LCD_print("  ");
			}
			else if(settings == 6){ 
				LCD_cmd(0xc5);
				LCD_print("  ");
			}
			else{
				time.weekday = day_of_week(time.day,time.month,time.year);
				Solar2Lunar(time.day,time.month,time.year,&time.lunarday, &time.lunarmonth, &time.lunaryear);
				goto NORMAL;
			}
			
			delay(BLINK);
			if(~BTN_SET){
				delay(DEBOUNCE);
				settings = (settings+1)%7;
			}
			
			if(time.year%4==0 && time.month==2 && time.day>29)
				time.day = 29;
			else if(time.year%4!=0 && time.month==2 && time.day>28)
				time.day = 28;
			else if((time.month==4 || time.month==6 || time.month==9 || time.month==11) && time.day>30)
				time.day = 30;
			
			LCD_print_time(time.hour,time.minute,time.second,1);
			LCD_print_date(OFF_WEEKDAY,time.day,time.month,time.year);
			
			delay(BLINK);
			if(~BTN_SET){
				delay(DEBOUNCE);
				settings = (settings+1)%7;
			}
		}	
	}
	
	NORMAL:
	LCD_print_time(time.hour,time.minute,time.second,cur_24h_stat);
	LCD_print_date(time.weekday,time.day,time.month,time.year);	
	while(1){
		if(cur_sec_stat != last_sec_stat){
			increase_second();
			last_sec_stat = cur_sec_stat;
			LCD_print_time(time.hour,time.minute,time.second,cur_24h_stat);
		}
		if(cur_24h_stat != last_24h_stat){
			last_24h_stat = cur_24h_stat;
			LCD_print_time(time.hour,time.minute,time.second,cur_24h_stat);
		}
		if(cur_cal_stat != last_cal_stat){
			last_cal_stat = cur_cal_stat;
			if(~cur_cal_stat) // not lunar calendar
				LCD_print_date(time.weekday,time.day,time.month,time.year);
			else
				LCD_print_date(LUNAR,time.lunarday,time.lunarmonth,time.lunaryear);
		}
		if(~BTN_SET){
			delay(DEBOUNCE);
			settings++;
			goto SETTINGS;
		}
	}
}

//-------------------------------------END-OF-MAIN----------------------------------------------------//

void ISR_1ms_timer0(void) interrupt 1 {
	TH0 = 0x3C;
	TL0 = 0xB0;
	timer_counter--;
	if (timer_counter == 0){
		timer_counter = 20;
		cur_sec_stat = ~cur_sec_stat;
	}
}

void ISR_btn1(void) interrupt 0 {
	delay(DEBOUNCE);
	if(settings == 0)
		cur_24h_stat = ~cur_24h_stat;
	else if(settings == 1)
		time.hour = (time.hour>0) ? time.hour - 1 : 23;
	else if(settings == 2)
		time.minute = (time.minute>0) ? time.minute - 1 : 59;
	else if(settings == 3)
		time.second = (time.second>0) ? time.second - 1 : 59;
	else if(settings == 4)
		time.year -= 1;
	else if(settings == 5)
		time.month = (time.month>1) ? time.month - 1 : 12;
	else{
		if(time.day==1){
			if(time.year%4==0 && time.month==2)
				time.day = 29;
			else if(time.year%4!=0 && time.month==2)
				time.day = 28;
			else if(time.month==4 || time.month==6 || time.month==9 || time.month==11)
				time.day = 30;
			else time.day = 31;
		}
		else time.day -= 1;
	}
}

void ISR_btn2(void) interrupt 2 {
	delay(DEBOUNCE);
	if(settings == 0)
		cur_cal_stat = ~cur_cal_stat;
	else if(settings == 1)
		time.hour = (time.hour<23) ? time.hour + 1 : 0;
	else if(settings == 2)
		time.minute = (time.minute<59) ? time.minute + 1 : 0;
	else if(settings == 3)
		time.second = (time.second<59) ? time.second + 1 : 0;
	else if(settings == 4)
		time.year += 1;
	else if(settings == 5)
		time.month = (time.month<12) ? time.month + 1 : 1;
	else {
			if(time.year%4==0 && time.month==2 && time.day>=29)
				time.day = 1;
			else if(time.year%4!=0 && time.month==2 && time.day>=28)
				time.day = 1;
			else if((time.month==4 || time.month==6 || time.month==9 || time.month==11)&& time.day>=30)
				time.day = 1;
			else if(time.day==31)
				time.day = 1;
			else
				time.day+=1;
	}
}

void init_time(){
	time.hour = 0;
	time.minute = 0;
	time.second = 0;
	time.weekday = 0;
	time.day = 1;
	time.month = 1;
	time.year = 2018;
}

void increase_second(){
	time.second += 1;
	
	if(time.second == 60){
		time.minute += 1;
		time.second = 0;
		if(time.minute == 60){
			time.hour += 1;
			time.minute = 0;
			if(time.hour == 24){
				time.weekday += 1;
				time.day += 1;
				time.hour = 0;
				if(time.weekday == 7){
					time.weekday = 0;
				}
				if(time.month == 2){
					if ((time.year % 4) == 0){ 
						if (time.day == 30){
							time.day = 1;
							time.month += 1;
						}
					}
					else{
						if (time.day == 29){
							time.day = 1;
							time.month += 1;
						}				
					}
				}
				else if(time.month == 12){
					if(time.day == 32){
						time.day = 1;
						time.month = 1;
						time.year += 1;
					}
				}
				else if(time.month == 1 || time.month == 3 || time.month == 5 || time.month == 7 || time.month == 8 || time.month == 10){
					if(time.day == 32){
						time.day = 1;
						time.month += 1;
					}
				}
				else{
					if(time.day == 31){
						time.day = 1;
						time.month += 1;
					}
				}
				Solar2Lunar(time.day,time.month,time.year,&time.lunarday, &time.lunarmonth, &time.lunaryear);
			}
		}
	}
}

char day_of_week(char day,char month,int year){
	unsigned int total_day = day - 1;
	char i;
	char res;
	
	for(i=1; i<month; i++){
		if(i==1 || i==3 || i==5 || i==7|| i==8|| i==10 || i==12)
			total_day+=31;
		else{
			if(i==2){
				if(year%4==0)
					total_day+=29;
				else if(year%4!=0)
					total_day+=28;
			}
			else
				total_day+=30;
		}
	}
	total_day += (year-2000)*365 + (year-2001)/4 + 1;
	res = (char)((total_day+5)%7);
	return res;
}


