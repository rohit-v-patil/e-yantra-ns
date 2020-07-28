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

#include "NS_Task_1_Predef.h"
extern unsigned int color_sensor_pulse_count;
/*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the nodes specified
* Example Call: forward_wls(2) //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node);


/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/
void left_turn_wls(void);


/*
*
* Function Name: sharp_left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_left_turn_wls(); //Turns right until black line is encountered
*
*/
void sharp_left_turn_wls(void);


/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void right_turn_wls(void);


/*
*
* Function Name: sharp_right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void sharp_right_turn_wls(void);


/*
*
* Function Name: read_line_sensor
* Input: void
* Output: void
* Logic: Reads values of all three(left, middle and right) line sensors and stores in variables l,m,r respectively
* Example Call: read_line_sensors();
*
*/
void read_line_sensor(void);


/*
*
* Function Name: line_follow
* Input: void
* Output: void
* Logic: Use this function to make the robot follow the black line on the arena
* Example Call: line_follow();
*/
void line_follow(void);


/*
*
* Function Name: Square
* Input: void
* Output: void
* Logic: Use this function to make the robot trace a square path on the arena
* Example Call: Square();
*/
void Square(void);


/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.1 logic
* Example Call: Task_1_1();
*/
void Task_1_1(void);


/*
*
* Function Name: Task_1_2
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.2 logic
* Example Call: Task_1_2();
*/
void Task_1_2(void);