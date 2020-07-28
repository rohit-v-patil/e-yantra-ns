/*
*
*Team Id: 0263
*Author List: Rohit Vasant Patil, Brijesh Kumar Patra, Kunal Kishore Sahu, Debasish Panigrahy
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

/*
*
* Function Name: create_adjacency_matrix
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void create_adjancey_matrix();

/*
*
* Function Name: breadth_first_search
* Input: int, int (source and destination node)
* Output: void
* Logic: uses breadth first search algorithm to find path between the given nodes and stores path in path[] array
* Example Call: breadth_first_search(source,dest)
*
*/
void breadth_first_search(int, int);

/*
*
* Function Name: travel_path
* Input: void
* Output: void
* Logic: this function specifies for each node that bot should turn in which direction based on the previous, current node and next node 
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void travel_path(void);

/*
*
* Function Name: check_object
* Input: void
* Output: int (1 if object is present in front else 0)
* Logic: uses front ir sensor to detect any object present in front
* Example Call: check_object();
*
*/
int check_object(void);

/*
*
* Function Name: check_color
* Input: void
* Output: void
* Logic: uses color sensor and predefined functions to store value of red,green and blue filters and stores in red, green and blue variables
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void check_color(void);

/*
*
* Function Name: check_nut
* Input: void
* Output: void
* Logic: if in pickup node then it checks the color of nut and stores in current_nut_color variable and picks it else current_nut_color is white(W)
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void check_nut(void);

