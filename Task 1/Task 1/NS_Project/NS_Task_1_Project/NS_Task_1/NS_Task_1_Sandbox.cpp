/*
*
*Team Id: 0263
*Author List: Rohit Vasant Patil
*Filename: NS_Task_1_Sandbox.cpp
*Theme: Nutty Squirrel
*Functions: forward_wls, left_turn_wls, right_turn_wls, sharp_left_turn_wls, sharp_right_turn_wls, read_line_sensor,Square,line_follow, Task_1_1,Task_1_2  
*Global Variables: left_value, middle_value, right_value
*
*/



#include "NS_Task_1_Sandbox.h"
unsigned char left_value, middle_value, right_value;

/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int nodes_covered = 0;			//counter for nodes
	while (true)
	{

		forward();
		velocity(150, 150);
		_delay_ms(1);

		//read values of line sensor
		read_line_sensor();

		//check for node, if node is found increment no. of nodes covered(nodes_covered) by 1 and go a little forward
		if ((left_value != 0 && middle_value != 0 && right_value != 0))
		{
			nodes_covered++;
			printf("covered= %d\n\n", nodes_covered);
			forward();
			velocity(150, 150);
			_delay_ms(350);
		}


		//read_line_sensor();
		// if middle and right sensors go out of line then turn left until black line is in middle
		if ((left_value != 0 && middle_value == 0 && right_value == 0))
		{
			left_turn_wls();
		}

		//read_line_sensor();
		// if middle and left sensors go out of line then turn right until black line is in middle
		if ((left_value == 0 && middle_value == 0 && right_value != 0))
		{
			right_turn_wls();
		}



		//if nodes covered is equal to given no. of nodes then stop bot and return control to calling funtion 
		if (nodes_covered == node)	
		{
			stop();
			return;
		}
	}
}

/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/
void left_turn_wls(void)
{
	while (1)
	{
		soft_left();
		_delay_ms(1);
		if (ADC_Conversion(2) != 0)
			break;
	}
}

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void right_turn_wls(void)
{
	while (1)
	{
		soft_right();
		_delay_ms(1);
		if (ADC_Conversion(2) != 0)
			break;
	}
}


/*
*
* Function Name: read_line_sensor
* Input: void
* Output: void
* Logic: Reads values of all three(left, middle and right) line sensors and stores in variables l,m,r respectively
* Example Call: read_line_sensors();
*
*/
void read_line_sensor() 
{
	left_value = ADC_Conversion(1);
	middle_value = ADC_Conversion(2);
	right_value = ADC_Conversion(3);
}


/*
*
* Function Name: sharp_left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_left_turn_wls(); //Turns right until black line is encountered 
*
*/
void sharp_left_turn_wls(void)
{
	
	left();
	_delay_ms(250);

	while (1)
	{
		left();
		_delay_ms(5);
		if (ADC_Conversion(2) != 0)
			break;
	}
}



/*
*
* Function Name: sharp_right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered 
*
*/
void sharp_right_turn_wls(void)
{
	right();
	_delay_ms(250);

	while (1)
	{
		right();
		_delay_ms(5);
		if (ADC_Conversion(2) != 0)
			break;
	}
}




/*
*
* Function Name: Square
* Input: void
* Output: void
* Logic: Use this function to make the robot trace a square path on the arena
* Example Call: Square();
*/
void Square(void)
{
	
	forward();
	_delay_ms(1000);

	left();
	_delay_ms(500);
	
	forward();
	_delay_ms(1000);

	left();
	_delay_ms(500);
	
	forward();
	_delay_ms(1000);
	
	left();
	_delay_ms(500);
	
	forward();
	_delay_ms(1000);

	left();
	_delay_ms(500);


	stop();
	_delay_ms(3000);

		
}

/*
*
* Function Name: line_follow
* Input: void
* Output: void
* Logic: Use this function to make the robot follow the black line on the arena
* Example Call: line_follow();
*/
void line_follow()
{
	while (1) 
	{
		
		forward();
		velocity(150, 150);	//decrease velocity of bot to half
		_delay_ms(1);
		

		read_line_sensor();
		// if middle and right sensors go out of line then turn left until black line is in middle
		if ((left_value != 0 && middle_value == 0 && right_value == 0))	
		{
			left_turn_wls();
		}

		//read_line_sensor();
		// if middle and left sensors go out of line then turn right until black line is in middle
		if ((left_value == 0 && middle_value == 0 && right_value != 0))	
		{
			right_turn_wls();
		}

		//read_line_sensor();
		if ((left_value == 0 && middle_value == 0 && right_value == 0))
		{
			break;
		}

	}
	//printf("\n\nSuccess!!!\n\n");
}


/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.1 logic
* Example Call: Task_1_1();
*/
void Task_1_1(void)
{
	forward_wls(1);

	sharp_right_turn_wls();

	forward_wls(1);

	sharp_left_turn_wls();

	forward_wls(2);

	sharp_left_turn_wls();

	forward_wls(1);

	sharp_left_turn_wls();

	forward_wls(1);

	sharp_right_turn_wls();

	forward_wls(1);

	sharp_right_turn_wls();

	forward_wls(2);

	sharp_left_turn_wls();

	forward_wls(1);

	sharp_right_turn_wls();

	forward_wls(1);
	
	sharp_left_turn_wls();
	
	forward_wls(1);

	stop();
	
	_delay_ms(2000);


}

/*
*
* Function Name: Task_1_2
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.2 logic
* Example Call: Task_1_2();
*/
void Task_1_2(void)
{

	//printf("\nir= %d\n", ADC_Conversion(4));
	
	filter_clear();
	_delay_ms(1000);
	//printf("\nclear = %d\n", color_sensor_pulse_count);

	filter_red();
	_delay_ms(1000);
	printf("\nred = %d\n", color_sensor_pulse_count);

	filter_clear();

	filter_green();
	_delay_ms(1000);
	printf("\ngreen = %d\n", color_sensor_pulse_count);

	_delay_ms(3000);
	/*
	forward_wls(2);
	sharp_right_turn_wls();
	forward_wls(1);
	sharp_left_turn_wls();
	forward_wls(2);
	stop();
	_delay_ms(1000);
	pick();
	sharp_right_turn_wls();
	sharp_right_turn_wls();
	forward_wls(2);
	sharp_right_turn_wls();
	forward_wls(4);
	//line_follow();
	sharp_right_turn_wls();
	forward_wls(1);
	sharp_right_turn_wls();
	stop();
	_delay_ms(1000);
	place();
	sharp_right_turn_wls();
	forward_wls(1);
	stop();
	_delay_ms(2000);
	*/


}
