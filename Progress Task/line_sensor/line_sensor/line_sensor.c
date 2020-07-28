/*
 * line_sensor.c
 *
 * Created: 18-01-2019 18:14:54
 *  Author: Rohit
 */ 

#define THRESHOLD 50
#define THRESHOLD_MIDDLE 75
#define THRESHOLD_SIDE 12
#define THRESHOLD_NODE 123
#define VMAX 130
#define VMIN 90
#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>  
#include <avr/interrupt.h>
#include "lcd.h"

float BATT_Voltage, BATT_V;
unsigned char ADC_Value;
unsigned char left_line_sensor=0;
unsigned char middle_line_sensor=0;
unsigned char right_line_sensor=0;
unsigned char flag = 0;

void lcd_port_config(void)
{
	DDRC |= 0xFF;
	PORTC &= 0x80; 
}

unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	
	ADMUX= (ADMUX & 0xF8) | Ch;
	
	ADCSRA |= (1 << ADSC); 		//Set start conversion bit
	
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	
	a=ADCH;
	
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	
	ADCSRB = 0x00;
	
	return a;
}

void print_sensor(char row, char column, unsigned char channel)
{
	ADC_Value=ADC_Conversion(channel);
	lcd_print(row,column,ADC_Value,3);
}

void adc_init()
{
	ADMUX = (1<<REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	DIDR0 = (1 << ADC0D);
}

/*void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x60;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}*/


//Function to configure ports to enable robot's motion
void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}


// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}



//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 		// removing upper nibble for the protection
	PortARestore = PORTA; 		// reading the PORTA original status
	PortARestore &= 0xF0; 		// making lower direction nibble to 0
	PortARestore |= Direction; // adding lower nibble for forward command and restoring the PORTA status
	PORTA = PortARestore; 		// executing the command
}



void forward (void)
{
	motion_set (0x06);
}

void stop (void)
{
	motion_set (0x00);
}

void reverse(void)
{
	motion_set(0x09);
}

void soft_left(void)
{
	motion_set(0x04);
}


void soft_right(void)
{
	motion_set(0x02);
}

void sharp_left(void)
{
	motion_set(0x05);
}

void sharp_right(void)
{
	motion_set(0x0A);
}


void about_turn_right(void)
{
	velocity(90,120);
	sharp_right();
	_delay_ms(500);
	
	//right_line_sensor=ADC_Conversion(1);
	middle_line_sensor=ADC_Conversion(2);
	//left_line_sensor=ADC_Conversion(3);
	print_sensor(2,5,2);	//Prints Value of White Line Sensor3
	
	while((middle_line_sensor=ADC_Conversion(2)) < 15)
	{
		
		//print_sensor(2,5,2);	//Prints Value of White Line Sensor3

	}
	stop();
	_delay_ms(1000);
}


int main(void)
{
	cli();
	
	motion_pin_config();
	timer5_init();
	//sei();
	lcd_port_config();
	adc_init();
	lcd_init();
	
	//unsigned char speed=150;
	//forward();
	
	lcd_cursor(1,1);
	lcd_string("L");
	
	lcd_cursor(1,6);
	lcd_string("M");
	
	lcd_cursor(1,10);
	lcd_string("R");

	//lcd_string("CAPTAIN BRIJESH!!");
    while(1)
    {
		
	
		
		left_line_sensor = ADC_Conversion(3);	//Getting data of Left WL Sensor
		middle_line_sensor = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_line_sensor = ADC_Conversion(1);	//Getting data of Right WL Sensor

		flag=0;

		print_sensor(2,1,3);	//Prints value of White Line Sensor1
		print_sensor(2,5,2);	//Prints Value of White Line Sensor2
		print_sensor(2,9,1);	//Prints Value of White Line Sensor3
		
		
			if(middle_line_sensor>THRESHOLD)
			{
				flag=1;
				forward();
				velocity(VMAX,VMAX+10);
			
			}
			
			
			


			if((left_line_sensor > THRESHOLD_SIDE) && (flag==0))
			{
				forward();
				
				velocity(110,VMAX+30);
			}

			


			if((right_line_sensor > THRESHOLD_SIDE) && (flag==0))
			{
				forward();
				velocity(VMAX+20,110);
			}


			
			if((middle_line_sensor > THRESHOLD_NODE) && ((left_line_sensor > THRESHOLD_SIDE) || (right_line_sensor > THRESHOLD_SIDE)))
			{
				stop();
				_delay_ms(2000);
				
				about_turn_right();
				
			}
			
			if(((right_line_sensor > THRESHOLD_NODE) || (left_line_sensor > THRESHOLD_NODE)) && (middle_line_sensor > THRESHOLD_SIDE))
			{
				stop();
				_delay_ms(2000);
				
				about_turn_right();
			}
			/*if((right_line_sensor > 8) && (middle_line_sensor> 8) && (left_line_sensor > 8))
			{
				stop();
				_delay_ms(2000);
				
				about_turn_right();
			}*/
				
    }
}