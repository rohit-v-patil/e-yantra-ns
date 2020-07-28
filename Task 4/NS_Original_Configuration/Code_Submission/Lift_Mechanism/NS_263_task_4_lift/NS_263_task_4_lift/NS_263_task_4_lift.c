/*
 * Team Id: NS#263
 * Author List: Rohit V. Patil
 * Filename: NS_263_task_4_lift.c
 * Theme: Nutty Squirrel
 * Functions: init_devices, lift_up, lift_down, motor_stop, main
 */


#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h> 



/*
 * Function Name:init_devices
 * Input: void
 * Output: void
 * Logic: used to initialize the input and output pins
 * Example Call: init_devices()
*/
void init_devices(void)
{
	DDRB |= 0xFF;	//PORT B is set as output for motor
	PORTB &= 0x00;
		
	DDRD &= 0x00;	//PORT D is set as input for IR Sensor and Limit Switches
	PORTD |= 0xFF;
	}


/*
 * Function Name:lift_up
 * Input: void
 * Output: void
 * Logic: used to rotate motor in clockwise direction
 * Example Call: lift_up()
*/
void lift_up(void)
{
	PORTB &= 0x00;
	PORTB |= 0x18;	//PB4 and PB3 is made high
}


/*
 * Function Name:lift_down
 * Input: void
 * Output: void
 * Logic: used to rotate motor in anti-clockwise direction
 * Example Call: lift_down
*/
void lift_down(void) 
{
	PORTB &= 0x00;
	PORTB |= 0x14;	//PB4 and PB2 is made high
}


/*
 * Function Name:motor_stop
 * Input: void
 * Output: void
 * Logic: used to stop the motor
 * Example Call: motor_stop()
*/
void motor_stop(void)
{
	PORTB &= 0x00;
}



int main(void)
{
	unsigned int ir=1, limit_switch_1=1, limit_switch_2=1; // variables to store status of IR Sensor and Limit switches
	unsigned int up=0;
	init_devices();			//initialized all devices
	motor_stop();			
    while(1)
    {	
		
		ir=1; limit_switch_1=1; limit_switch_2=1;
				
		ir = PIND & 0x04;				//reading status of IR Sensor
		limit_switch_1 = PIND & 0x10;	//reading status of Limit Switch 1 (bottom limit switch)
		limit_switch_2 = PIND & 0x08;	//reading status of Limit Switch 2 (upper limit switch)
		
		
		
		if( ir == 0 )									//check if bot is present on lift			
		{
			if( limit_switch_1 == 0 )					//check if lift is at ground level
			{
				_delay_ms(4000);						//wait for 4 sec. and check again if bot is present on the lift
				ir = PIND & 0x04;
				while( limit_switch_2 != 0 && ir == 0)	//move the lift up until upper limit switch is pressed
				{
					lift_up();
					limit_switch_2 = PIND & 0x08;
				}
				motor_stop();
			}	
			
			if( limit_switch_2 == 0 )					//check if lift is at upper position
			{
				_delay_ms(4000);						//wait for 4 sec. and check again if bot is present on the lift
				ir = PIND & 0x04;
				while( limit_switch_1 !=0 && ir == 0)	//move the lift down until bottom limit switch is pressed
				{
					lift_down();
					limit_switch_1 = PIND & 0x10;
				}		
				motor_stop();			
			}		
				
		}			
		
			
		
	}		
}