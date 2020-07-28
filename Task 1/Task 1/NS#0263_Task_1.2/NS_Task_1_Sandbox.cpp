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



#include "NS_Task_1_Sandbox.h"
#include<stdio.h>
#include<stdlib.h>
#define MAX 30

unsigned char left_value, middle_value, right_value, current_nut_color='W',ir;

int graph[30][30],path[20],current_node,path_length,path_counter;

int clear, red, green, blue, obstacle_found = 0, strt_node;


//structure for queue to implement graph
typedef struct QUEUE
{
	int data[MAX];
	int front;
	int rear;
}queue;

/*
*
* Function Name: init
* Input: queue reference
* Output: void
* Logic: initialize front and rear of queue
* Example Call: init(&q);
*
*/
void init(queue *q)
{
	q->front = -1;
	q->rear = -1;
}

/*
*
* Function Name: enqueue
* Input: queue reference , element to insert in queue(int)
* Output: void
* Logic: inserts an element in queue
* Example Call: enqueue(&q,data)
*
*/
void enqueue(queue *q, int data)
{
	if (q->rear == MAX - 1)
		return;
	else if (q->rear == -1 && q->front == -1)
	{
		q->rear = q->front = 0;
		q->data[q->rear] = data;
	}
	else
	{
		q->rear++;
		q->data[q->rear] = data;
	}
}

/*
*
* Function Name: dequeue
* Input: queue reference
* Output: int (returns the element deleted)
* Logic: deletes an element from queue
* Example Call: dequeue(&q)
*
*/
int dequeue(queue *q)
{
	int temp;
	if (q->rear == -1 && q->front == -1)
		return -1;
	else if (q->front == q->rear)
	{
		temp = q->data[q->front];
		q->front = -1;
		q->rear = -1;
	}
	else
	{
		temp = q->data[q->front];
		q->front++;
	}
	return temp;
}

/*
*
* Function Name: isEmpty
* Input: queue reference
* Output: int (returns 1 if empty and 0 if not empty)
* Logic: checks if front and rear of queue bot are -1 if true queue is empty else not
* Example Call: isEmpty(&q)
*
*/
int isEmpty(queue *q)
{
	if (q->rear == -1 && q->front == -1)
		return 1;
	else
		return 0;
}

/*
*
* Function Name: create_adjacency_matrix
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/

/*
*
* Function Name: create_adjacency_matrix
* Input: int (number of nodes)
* Output: void
* Logic: represents graphical arena in a matrix. If there is an edge between node 1 and node 2 then graph[1][2]=1 and graph[2][1]=1 and if no edge then 0
* Example Call: create_adjacency_matrix(nodes)
*
*/

void create_adjacency_matrix(int no_of_node)
{
	int i, j;
	for (i = 0; i < no_of_node; i++)
	{
		for (j = 0; j < no_of_node; j++)
		{
			graph[i][j] = 0;	
		}
	}


	graph[0][1] = 1;
	graph[1][2] = 1;
	graph[2][3] = 1;
	graph[2][4] = 1;
	graph[2][14] = 1;
	graph[4][5] = 1;
	graph[14][15] = 1;
	graph[5][15] = 1;
	graph[5][6] = 1;
	graph[15][16] = 1;
	graph[6][7] = 1;
	graph[16][17] = 1;
	graph[7][8] = 1;
	graph[17][18] = 1;
	graph[4][9] = 1;
	graph[14][19] = 1;
	graph[9][10] = 1;
	graph[19][20] = 1;
	graph[7][10] = 1;
	graph[17][20] = 1;
	graph[19][22] = 1;
	graph[9][12] = 1;
	graph[10][13] = 1;
	graph[20][23] = 1;
	graph[3][11] = 1;
	graph[11][12] = 1;
	graph[12][13] = 1;
	graph[3][21] = 1;
	graph[21][22] = 1;
	graph[22][23] = 1;


	graph[1][0] = 1;
	graph[2][1] = 1;
	graph[3][2] = 1;
	graph[4][2] = 1;
	graph[14][2] = 1;
	graph[5][4] = 1;
	graph[15][14] = 1;
	graph[15][5] = 1;
	graph[6][5] = 1;
	graph[16][15] = 1;
	graph[7][6] = 1;
	graph[17][16] = 1;
	graph[8][7] = 1;
	graph[18][17] = 1;
	graph[9][4] = 1;
	graph[19][14] = 1;
	graph[10][9] = 1;
	graph[20][19] = 1;
	graph[10][7] = 1;
	graph[20][17] = 1;
	graph[22][19] = 1;
	graph[12][9] = 1;
	graph[13][10] = 1;
	graph[23][20] = 1;
	graph[11][3] = 1;
	graph[12][11] = 1;
	graph[13][12] = 1;
	graph[21][3] = 1;
	graph[22][21] = 1;
	graph[23][22] = 1;


	/*printf("\nAdj. Mat. -->\n\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}*/
}

/*
*
* Function Name: breadth_first_search
* Input: int, int (source and destination node)
* Output: void
* Logic: uses breadth first search algorithm to find path between the given nodes and stores path in path[] array
* Example Call: breadth_first_search(source,dest)
*
*/
void breadth_first_search(int source, int dest)
{
	int no_nodes=24,path1[20];
	queue q;
	init(&q);
	int distance[30], previous[30], u, v, i, j = 0, f = 0;
	char color[30];
	for (v = 0; v < no_nodes; v++)	//initialise distance of all nodes=-1 , color='w' and preious=-1
	{
		distance[v] = -1;
		color[v] = 'w';
		previous[v] = -1;
	}
	distance[source] = 0;					//distance of source node is 0
	color[source] = 'g';					//color is made grey as it is visited
	previous[source] = source;				//previous of source is source
	enqueue(&q, source);					//source is enqueued in queue
	//printf("%d is enqueued\n", source);
	while (!isEmpty(&q))			
	{
		u = dequeue(&q);					//dequeue front element of queue
		//printf("%d is dequeued\n", u);
		for (v = 0; v < no_nodes; v++)			//loop to process all nodes
		{
			if (graph[u][v] == 1)				//check adjacents nodes of current dequeued node
			{
				if (color[v] == 'w')				//if it is unvisited then visit it and enqueue it in queue and increment distance by 1
				{
					color[v] = 'g';
					distance[v] = distance[u] + 1;
					previous[v] = u;
					enqueue(&q, v);
					//printf("%d is enqueued\n", v);
				}
			}
		}
		color[u] = 'b';							//color of dequeued node is made b which indicates it is processed
	}
	/*printf("\n\n");
	for (i = 0; i < n; i++)
	{
		for (k = 0; k < n; k++)
		{
			if (d[k] == j)
			{
				printf("%d ", k);
				printf("  parent=%d  ", prev[k]);
			}
		}
		j++;
		//printf("\n\n");
	}*/


	i = dest;
	path[0] = dest;
	path_length = 1;
	do {										//store calculated path in path[] array
		path[path_length++] = previous[i];
		i = path[path_length - 1];
	} while (i != source);		

	for (i = 0,j= path_length -1; j>= 0; j--,i++)
	{
		path1[i] = path[j];
	}

	path[0] = -1;

	if (obstacle_found == 1)
	{
		path[0] = strt_node;
	}

	for (i = 0; i < path_length; i++)
	{
		path[i+1] = path1[i];
	}
	printf("\n");
	for (i = 0; i <= path_length; i++)
	{
		printf("%d ", path[i]);
	}
	printf("\n");

}

/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified and also check for obstacle and if obstacle is present
		 stop the bot and return control an dmake value of flag obstacle_found=1
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int nodes_covered = 0;			//counter for nodes
	while (true)
	{

		if (check_object() == 1)			// check if object is present in path
		{
			stop();
			_delay_ms(1000);
			obstacle_found = 1;
			return;
		}

		forward();
		velocity(250, 250);
		_delay_ms(1);
		
		//read values of line sensor
		read_line_sensor();

		//check for node, if node is found increment no. of nodes covered(nodes_covered) by 1 and go a little forward
		if ((left_value != 0 && middle_value != 0 && right_value != 0))
		{
			nodes_covered++;
			printf("covered= %d\n\n", nodes_covered);
			forward();
			velocity(250, 250);
			_delay_ms(320);
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
		velocity(150, 150);
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
		velocity(150, 150);
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
* Function Name: travel_path
* Input: void
* Output: void
* Logic: this function specifies for each node that bot should turn in which direction based on the previous, current node and next node 
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void travel_path()
{
	int i;

	current_node = path[1];
	obstacle_found = 0;

	for (i = 0; i < path_length; i++)
	{
		
		path_counter = i;

		if (current_node == 0)
		{
			if (path[i + 1] == 1)
				forward_wls(1);
		}

		else if (current_node == 1)
		{
			if (path[i + 1] == 2 || path[i+1]==0)
				forward_wls(1);
		}

		else if (current_node == 2)
		{
			if (path[i + 1] == 3 && path[i-1]==1)
				forward_wls(1);

			else if (path[i + 1] == 4 && path[i - 1] == 1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 3)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 3)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 4)
			{
				left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 4)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 4)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 14)
			{
				right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 14)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 14)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}

		}

		else if (current_node == 3)
		{
			if (path[i + 1] == 11 && path[i - 1] == 2)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 2)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 11)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 11)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 21)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == 21)
			{
				forward_wls(1);
			}
		}

		else if (current_node == 4)
		{
			if (path[i + 1] == 9 && path[i - 1] == 2)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 2)
			{
				right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 5)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 5)
			{
				left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 9)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
		}

		else if (current_node == 5)
		{
		if (path[i + 1] == 6 && path[i - 1] == 4)
		{
			sharp_left_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 15 && path[i - 1] == 4)
		{
			right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 6 && path[i - 1] == 15)
		{
			sharp_right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 4 && path[i - 1] == 15)
		{
			left_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 4 && path[i - 1] == 6)
		{
			sharp_right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 15 && path[i - 1] == 6)
		{
			sharp_left_turn_wls();
			forward_wls(1);
		}
		}

		else if (current_node == 6)
		{
			if (path[i + 1] == 5 && path[i-1]==7)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == 5)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}

		}

		else if (current_node == 7)
		{
		if (path[i + 1] == 8 && path[i - 1] == 6)
		{
			sharp_right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 10 && path[i - 1] == 6)
		{
			sharp_left_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 8 && path[i - 1] == 10)
		{
			//right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 6 && path[i - 1] == 10)
		{
			sharp_right_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 6 && path[i - 1] == 8)
		{
			sharp_left_turn_wls();
			forward_wls(1);
		}
		else if (path[i + 1] == 10 && path[i - 1] == 8)
		{
			forward_wls(1);
		}

		}

		else if (current_node == 8)
		{
		if (path[i + 1] == 7 && path[i - 1] == -1)
		{
			sharp_left_turn_wls();
			forward_wls(1);
		}
		}

		else if (current_node == 9)
		{
			if (path[i + 1] == 10 && path[i - 1] == 4)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 4)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 10 && path[i - 1] == 12)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 12)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 10)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 10)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
		}

		else if (current_node == 10)
		{
			if (path[i + 1] == 13 && path[i - 1] == 7)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 7)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == 9)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 13)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == 13)
			{
				forward_wls(1);
			}
		}

		else if (current_node == 11)
		{
			if (path[i + 1] == 12 && path[i-1]==3)
				forward_wls(1);

			else if (path[i + 1] == 3 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 12)
			{
				forward_wls(1);
			}
		}

		else if (current_node == 12)
		{
			if (path[i + 1] == 13 && path[i - 1] == 11)
				forward_wls(1);
			else if (path[i + 1] == 9 && path[i - 1] == 11)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == 9)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == 13)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 13)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			

		}

		else if (current_node == 13)
		{
			if (path[i + 1] == 10 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 10)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 10 && path[i - 1] == 12)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
		}

		else if (current_node == 14)
		{
			if (path[i + 1] == 19 && path[i - 1] == 2)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i - 1] == 2)
			{
				left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 15)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 15)
			{
				right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}

		}

		else if (current_node == 15)
		{
			if (path[i + 1] == 16 && path[i - 1] == 14)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 14)
			{
				left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 16 && path[i - 1] == 5)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 5)
			{
				right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 16)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 16)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
		}

		else if (current_node == 16)
		{
			if (path[i + 1] == 15 && path[i - 1] == 17)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == 15)
			{
				forward_wls(1);
			}

		}

		else if (current_node == 17)
		{
			if (path[i + 1] == 18 && path[i - 1] == 16)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 16)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 18 && path[i - 1] == 20)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 16 && path[i - 1] == 20)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 16 && path[i - 1] == 18)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 18)
			{
				forward_wls(1);
			}

			}

		else if (current_node == 18)
		{
			if (path[i + 1] == 17 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}

		}

		else if (current_node == 19)
		{
			if (path[i + 1] == 22 && path[i - 1] == 14)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 14)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == 20)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 20)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}

			else if (path[i + 1] == 20 && path[i - 1] == 22)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 22)
			{
				forward_wls(1);
			}
		}

		else if (current_node == 20)
		{
			if (path[i + 1] == 23 && path[i - 1] == 17)
			{
				forward_wls(1);
				left();
				_delay_ms(5);
				stop();
			}
			else if (path[i + 1] == 19 && path[i - 1] == 17)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 23)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == 23)
			{
				forward_wls(1);
			}
		}

		else if (current_node == 21)
		{
			if (path[i + 1] == 3 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 22)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == 3)
			{
				forward_wls(1);
			}
			

		}

		else if (current_node == 22)
		{
			if (path[i + 1] == 21 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == 21)
				forward_wls(1);
			else if (path[i + 1] == 21 && path[i - 1] == 23)
				forward_wls(1);
			else if (path[i + 1] == 23 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 23)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 21)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
		}

		else if (current_node == 23)
		{
			if (path[i + 1] == 22 && path[i - 1] == 20)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 22)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				forward_wls(1);
			}
		}

		if (obstacle_found == 1)	// if obstacle was found while travlling from one node to other then return
		{
			return;
		}				
		obstacle_found = 0;
		current_node = path[i + 1];			//update value of current_node when reached next node
		printf("\ncurrent node = %d", current_node);
	}


}

/*
*
* Function Name: check_object
* Input: void
* Output: int (1 if object is present in front else 0)
* Logic: uses front ir sensor to detect any object present in front
* Example Call: check_object();
*
*/
int check_object()
{
	ir = ADC_Conversion(4);
	if (ir > 5 && ir < 60)
	{
		printf("\n\nObstacle Detected\n\n");
		return 1;
	}
	else 
	{
		//printf("\nIR= %d\n", ir);
		return 0;
	}
}

/*
*
* Function Name: check_color
* Input: void
* Output: void
* Logic: uses color sensor and predefined functions to store value of red,green and blue filters and stores in red, green and blue variables
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void check_color()
{

	filter_red();
	red = color_sensor_pulse_count;

	filter_green();
	green = color_sensor_pulse_count;

	filter_blue();
	blue = color_sensor_pulse_count;


	printf("\nred= %d green= %d blue= %d \n\n" ,red, green, blue);
}

/*
*
* Function Name: check_nut
* Input: void
* Output: void
* Logic: if in pickup node then it checks the color of nut and stores in current_nut_color variable and picks it else current_nut_color is white(W)
* Example Call: sharp_right_turn_wls(); //Turns right until black line is encountered
*
*/
void check_nut()
{
	current_nut_color = 'W';

	if (current_node == 11)
	{
		
		if (path[path_length - 1] == 3)
			sharp_left_turn_wls();
		else if (path[path_length - 1] == 12)
			sharp_right_turn_wls();
		stop();

		if (check_object() == 0)
			return;

		_delay_ms(1000);
		check_color();
		_delay_ms(1000);

		 if ( red > green && red > blue)
		{
			current_nut_color = 'R';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
		else if ( green > red && green > blue)
		{
			current_nut_color = 'G';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
	}

	else if (current_node == 12)
	{
		if (path[path_length - 1] == 11)
			sharp_left_turn_wls();
		else if(path[path_length - 1] == 13)
			sharp_right_turn_wls();
		stop();

		if (check_object() == 0)
			return;

		_delay_ms(1000);
		check_color();
		_delay_ms(1000);

		if (red > green && red > blue)
		{
			current_nut_color = 'R';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
		else if (green > red && green > blue)
		{
			current_nut_color = 'G';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
	}

	else if (current_node == 13)
	{
		if (path[path_length - 1] == 12)
			sharp_left_turn_wls();
		
		stop();

		if (check_object() == 0)
			return;

		_delay_ms(1000);
		check_color();
		_delay_ms(1000);

		if (red > green && red > blue)
		{
			current_nut_color = 'R';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
		else if (green > red && green > blue)
		{
			current_nut_color = 'G';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
	}

	else if (current_node == 21 )
	{
		if (path[path_length - 1] == 3)
			sharp_right_turn_wls();
		else
			sharp_left_turn_wls();
		stop();

		if (check_object() == 0)
			return;

		_delay_ms(1000);
		check_color();
		_delay_ms(1000);

		if (red > green && red > blue)
		{
			current_nut_color = 'R';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
		else if (green > red && green > blue)
		{
			current_nut_color = 'G';
			printf("\ncolor = %c", current_nut_color);
			pick();
		}
	}

	else if (current_node == 22)
	{
	if (path[path_length - 1] == 21)
		sharp_right_turn_wls();
	else if(path[path_length - 1] == 23)
		sharp_left_turn_wls();
	stop();

	if (check_object() == 0)
		return;

	_delay_ms(1000);
	check_color();
	_delay_ms(1000);

	if (red > green && red > blue)
	{
		current_nut_color = 'R';
		printf("\ncolor = %c", current_nut_color);
		pick();
	}
	else if (green > red && green > blue)
	{
		current_nut_color = 'G';
		printf("\ncolor = %c", current_nut_color);
		pick();
	}
	}

	else if (current_node == 23)
	{
	if (path[path_length - 1] == 22)
		sharp_right_turn_wls();
	stop();

	if (check_object() == 0)
		return;

	_delay_ms(1000);
	check_color();
	_delay_ms(1000);

	if (red > green && red > blue)
	{
		current_nut_color = 'R';
		printf("\ncolor = %c", current_nut_color);
		pick();
	}
	else if (green > red && green > blue)
	{
		current_nut_color = 'G';
		printf("\ncolor = %c", current_nut_color);
		pick();
	}
	}
	
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
	int pickup_count = 0, pickup_nodes[6] = { 11,12,13,21,22,23 } ;  //array to store nodes which are at pickup zone
	int red_count = 0, deposit_node_red[2] = { 16, 18 } ;			 //array to store nodes at red deposit zone 
	int green_count = 0, deposit_node_green[2] = { 6, 8 } ;			 //array to store nodes at green deposit zone

	create_adjacency_matrix(24);		//form matrix of arena
	
	breadth_first_search(0, pickup_nodes[0]);		// find path between start and first pickup node
	travel_path();									//travel the path
	stop();

	if (obstacle_found == 1)						//if obstacle is found in path
	{
		while (obstacle_found != 0)					//while no obstacle is present in path
		{
			
			sharp_left_turn_wls();					//make reverse turn
			forward_wls(1);							//go forward until next node
			strt_node = path[path_counter + 1];		//make new starting node
			graph[current_node][strt_node] = 0;		//make the edge with obstacle as 0 in graph[][]
			graph[strt_node][current_node] = 0;		//make the edge with obstacle as 0 in garph[][]
			breadth_first_search(current_node, pickup_nodes[0]);	//calculate new path
			obstacle_found = 0;
			travel_path();							//travel the path
			stop();
			
		}
	}

	while (1)
	{

		check_nut();	//check if nut is present

		if (current_nut_color == 'W')	//if no nut
		{
			breadth_first_search(pickup_nodes[pickup_count], pickup_nodes[pickup_count + 1]);	//calculate path for next pickup node
			pickup_count++;	//increment counter of pickup nodes by 1
			travel_path();//go to next pickup node

			if (obstacle_found == 1)		//for dealing with obstacle
			{
				while (obstacle_found != 0)
				{
					
					sharp_left_turn_wls();
					forward_wls(1);
					strt_node = path[path_counter + 1];
					graph[current_node][strt_node] = 0;
					graph[strt_node][current_node] = 0;
					breadth_first_search(current_node, pickup_nodes[pickup_count]);
					obstacle_found = 0;
					travel_path();
					stop();
				}
			}
		}

		else if (current_nut_color == 'R')		//if red nut is present
		{
			breadth_first_search(current_node, deposit_node_red[red_count]);//find path for red deposit zone node
			travel_path();//go to red deposit zone node
			stop();

			if (obstacle_found == 1)			//for dealing with obstacles 
			{
				while (obstacle_found != 0)
				{
					
					sharp_left_turn_wls();
					forward_wls(1);
					strt_node = path[path_counter + 1];
					graph[current_node][strt_node] = 0;
					graph[strt_node][current_node] = 0;
					breadth_first_search(current_node, deposit_node_red[red_count]);
					obstacle_found = 0;
					travel_path();
					stop();
				}
			}


			if (red_count == 0)
			{
				if (path[path_length - 1] == 15)
					sharp_left_turn_wls();
				else
					sharp_right_turn_wls();
			}

			stop();
			_delay_ms(1000);
			place();					//place the nut
			_delay_ms(1000);
			red_count++;				//increment red counter by 1

			if ((red_count > 1 || green_count > 1) && pickup_count + 1 > 5)//if both green and both red nuts are placed and all pickup nodes are covered then finish by breaking out of loop
				break;

			breadth_first_search(current_node, pickup_nodes[pickup_count + 1]);		//find path to next pickup node
			pickup_count++;	
			travel_path();//go to next pickup node
			stop();

			if (obstacle_found == 1)//for dealing with obstacle
			{
				while (obstacle_found != 0)
				{
					sharp_left_turn_wls();
					forward_wls(1);
					strt_node = path[path_counter + 1];
					graph[current_node][strt_node] = 0;
					graph[strt_node][current_node] = 0;
					breadth_first_search(current_node, pickup_nodes[pickup_count]);
					obstacle_found = 0;
					travel_path();
					stop();
				}
			}
		}

		else if (current_nut_color == 'G')			//if green nut is present
		{
			breadth_first_search(current_node, deposit_node_green[green_count]);//find path for red deposit zone node
			travel_path();//go to red deposit zone node

			if (obstacle_found == 1)
			{
				while (obstacle_found != 0)			//for dealing with obstacles 
				{
					sharp_left_turn_wls();
					forward_wls(1);
					strt_node = path[path_counter + 1];
					graph[current_node][strt_node] = 0;
					graph[strt_node][current_node] = 0;
					breadth_first_search(current_node, deposit_node_green[green_count]);
					obstacle_found = 0;
					travel_path();
					stop();
				}
			}

			if (green_count == 0)
			{
				if (path[path_length - 1] == 5)
					sharp_right_turn_wls();
				else
					sharp_left_turn_wls();
			}
			

			stop();
			_delay_ms(1000);
			place();				//place the nut
			_delay_ms(1000);
			green_count++;			//increment green counter by 1

			if ((red_count > 1 && green_count > 1) || pickup_count+1 > 5)//if both green and both red nuts are placed and all pickup nodes are covered then finish by breaking out of loop
				break;

			breadth_first_search(current_node, pickup_nodes[pickup_count + 1]);//find path to next pickup node
			pickup_count++;
			travel_path();//go to next pickup node
			stop();

			if (obstacle_found == 1)//for dealing with obstacle
			{
				while (obstacle_found != 0)
				{
					
					sharp_left_turn_wls();
					forward_wls(1);
					strt_node = path[path_counter + 1];
					graph[current_node][strt_node] = 0;
					graph[strt_node][current_node] = 0;
					breadth_first_search(current_node, pickup_nodes[pickup_count]);
					obstacle_found = 0;
					travel_path();
					stop();
				}
			}
		}

		
	}

	breadth_first_search(current_node, 0);//find path upto start node(node 0)
	travel_path();//go to start node

	if (obstacle_found == 1)//for dealing with obstacles
	{
		while (obstacle_found != 0)
		{
			
			sharp_left_turn_wls();
			forward_wls(1);
			strt_node = path[path_counter + 1];
			graph[current_node][strt_node] = 0;
			graph[strt_node][current_node] = 0;
			breadth_first_search(current_node, 0);
			obstacle_found = 0;
			travel_path();
			stop();
		}
	}

	left();//turn reverse
	_delay_ms(950);

	stop();//stop the bot
	_delay_ms(3000);



	
	

}
