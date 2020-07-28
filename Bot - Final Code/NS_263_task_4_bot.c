/*
*
*Team Id: 0263
*Author List: Rohit Vasant Patil, Brijesh Kumar Patra, Kunal Kishore Sahu, Debasish Panigrahy
*Filename: NS_263_task_4_bot.c
*Theme: Nutty Squirrel
*Functions: create_adjacency_matrix, forward_wls, left_turn_wls, right_turn_wls, sharp_left_turn_wls, sharp_right_turn_wls,
*			breadth_first_search, travel_path, check_object, adjust_for_pickup, adjust_for_place, adjust_after_pickup, adjust_after_place,
*			check_nut, deal_with_obstacle, Task_4
*Global Variables: current_nut_color, total_nodes, pickup_nodes[], deposit_node_red[], deposit_node_green[], deposit_node_blue[],
*				   graph[][], path[], current_node, path_length, path_counter, prev_node, obstacle_found, start_node, obs_cnt 
*
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include "lcd.h"
#include "ns_header.h"
#define MAX 30


unsigned char current_nut_color='W',total_nodes=0;
int pickup_nodes[6] = { 11, 12, 13, 22, 21, 23 } ;  //array to store nodes which are at pickup zone
int deposit_node_red[2] = { 6, 8} ;					//array to store nodes at red deposit zone 
int deposit_node_green[2] = { 18, 16 } ;			//array to store nodes at green deposit zone
int deposit_node_blue[2] = { 26, 27 };				//array to store nodes at blue deposit zone

int graph[30][30],path[20],current_node,path_length,path_counter,prev_node;

int obstacle_found = 0, strt_node, obs_cnt=0;


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


	graph[0][2] = 1;
	graph[2][3] = 1;
	graph[2][4] = 1;
	graph[2][14] = 1;
	graph[4][5] = 1;
	graph[14][15] = 1;
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
	graph[15][1] = 1;
	graph[5][1] = 1;
	graph[1][24] = 1;
	graph[24][25] = 1;
	graph[25][26] = 1;
	graph[25][27] = 1;
	 


	graph[2][0] = 1;
	graph[3][2] = 1;
	graph[4][2] = 1;
	graph[14][2] = 1;
	graph[5][4] = 1;
	graph[15][14] = 1;
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
	graph[1][15] = 1;
	graph[1][5] = 1;
	graph[24][1] = 1;
	graph[25][24] = 1;
	graph[26][25] = 1;
	graph[27][25] = 1;
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
	int no_nodes=28,path1[20];
	queue q;
	init(&q);
	int distance[30], previous[30], u, v, i, j = 0; //f = 0;
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

	/*if (obstacle_found == 1)
	{
		path[0] = strt_node;
	}*/

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
		 stop the bot and return control and make value of flag obstacle_found=1
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int nodes_covered = 0;		//counter for nodes
	int obj=0;					//stores value returned by check_object
	while (1)
	{
		
		if ( path[path_counter+1] < 24 && obs_cnt < 1)		// check if object is present in path; don't check in upper part of arena
		   obj = check_object();
		   
		if ( obj == 1 )			// if obstacle is found make obstacle_found flag 1, increase obstacle couunt and return
		{
			stop();
			obs_cnt++;
			_delay_ms(500);
			obstacle_found = 1;
			obj=0;
			return;
		}
		
		_delay_ms(5);
		
		//read values of line sensor
		read_line_sensor();
		
		_delay_ms(5);
		
		//print values of line sensor
		//print_line_sensor();
		
		if(middle_line_sensor>=THRESHOLD && left_line_sensor<=THRESHOLD && right_line_sensor<=THRESHOLD) // Center on black line-010 ; go foward
		{	
			velocity(93,95); 
			forward();
		}

		else if((left_line_sensor>=THRESHOLD && middle_line_sensor<THRESHOLD) ) //left sensor on black line and middle out of line take left turn to get back on black line
		{
			left_turn_wls();
		}

		else if((right_line_sensor>=THRESHOLD && middle_line_sensor<=THRESHOLD)) //right sensor on black line and middle out of line take right turn to get back on black line
		{	
			
			right_turn_wls();
		}
		
		else if((right_line_sensor<=0x0F && middle_line_sensor<=0x0F && left_line_sensor<=0x0F)) //sometimes all three sensors detect white line and bot stops at one place so make it go slight left or right to bring on line
		{	
			if((current_node == 23 && path[path_counter+1]==20) || (current_node == 5 && path[path_counter+1]==1) || (current_node == 1 && path[path_counter+1]==15) || (current_node == 17 && path[path_counter+1]==20) || (current_node == 10 && path[path_counter+1]==7) || (current_node == 4 && path[path_counter+1]==5) || (current_node == 15 && path[path_counter+1]==14))
			{
				right();
				_delay_ms(80);
				stop();
			}
			else
			{
				left();
				_delay_ms(80);
				stop();
			}				
			
		}
		
		//node detection		
		
		if(path[path_counter+1]!=8 && path[path_counter+1]!=18 && path[path_counter+1]!=26 && path[path_counter+1]!=27)
		{
			
			if(middle_line_sensor>THRESHOLD && left_line_sensor>THRESHOLD && right_line_sensor<THRESHOLD)//110
			{
				forward();
				velocity(90,90);
				_delay_ms(430);
				stop();
				nodes_covered++;
				//return;
			}
			else if(middle_line_sensor<THRESHOLD && left_line_sensor>THRESHOLD && right_line_sensor>THRESHOLD)//101
			{
				forward();
				velocity(90,90);
				_delay_ms(430);
				stop();
				nodes_covered++;
				//return;
			}
			else if(middle_line_sensor>THRESHOLD && left_line_sensor>THRESHOLD && right_line_sensor>THRESHOLD)//111
			{
				forward();
				velocity(90,90);
				_delay_ms(430);
				stop();
				nodes_covered++;
				//return;
			}
			else if(middle_line_sensor>THRESHOLD && left_line_sensor<THRESHOLD && right_line_sensor>THRESHOLD)//011
			{
				forward();
				velocity(90,90);
				_delay_ms(430);
				stop();
				nodes_covered++;
				//return;
			}
		}
		 
		else if(middle_line_sensor >=0x76 && (left_line_sensor >= 0x09 || right_line_sensor >= 0x09))
		{
			
			PORTB |= 0x0F; //first making all bits high to turn off led
			PORTB &= 0xFB; //glow blue
			_delay_ms(50);
			PORTB |= 0x0F;
			forward();
			velocity(90,90);
			_delay_ms(430);
			stop();
			nodes_covered++;
			
		}
		

		//if nodes covered is equal to given no. of nodes then stop bot and return control to calling function 
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
		left();
		velocity(85,85);
		_delay_ms(10);
		stop();
		if (ADC_Conversion(2) >= THRESHOLD)
		{
			stop();
			break;
		}			
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
		right();
		velocity(85,85);
		_delay_ms(10);
		stop();
		if (ADC_Conversion(2) >= THRESHOLD)
		{
			stop();
			break;
		}			
	}
}



/*
*
* Function Name: sharp_left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered even if already on black line
* Example Call: left_turn_wls(); //Turns left until black line is encountered
*
*/
void sharp_left_turn_wls(void)
{
	left();
	velocity(95,95);
	_delay_ms(500);
	while (1)
	{
		left();
		velocity(95,95);
		_delay_ms(30);
		stop();
		if (ADC_Conversion(2) >= THRESHOLD)
		{
			stop();
			break;
		}			
	}
}

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered even if already on balck line
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void sharp_right_turn_wls(void)
{
	right();
	velocity(95,95);
	_delay_ms(500);
	while (1)
	{
		right();
		velocity(95,95);
		_delay_ms(30);
		stop();
		if (ADC_Conversion(2) >= THRESHOLD)
		{
			stop();
			break;
		}			
	}
}


/*
*
* Function Name: travel_path
* Input: void
* Output: void
* Logic: this function specifies for each node that bot should turn in which direction based on the previous, current node and next node 
* Example Call: travel_path();
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
		
		//print current and next node on lcd
		lcd_print(2,14,abs(path[i+1]),2);
		lcd_print(1,15,current_node,2);
		

		if (current_node == 0)
		{
			if (path[i + 1] == 2)
				forward_wls(1);
		}

		else if (current_node == 1)
		{
			if (path[i + 1] == 24 && path[i-1]==5)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 24 && path[i-1]==15)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i-1]==24)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i-1]==24)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i-1]==5)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i-1]==15)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 2)
		{
			if (path[i + 1] == 3 && path[i-1]==0)
				forward_wls(1);

			else if (path[i + 1] == 4 && path[i - 1] == 0)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 0)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 0 && path[i - 1] == 3)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 3)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 3)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 4)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 4)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 0 && path[i - 1] == 4)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 14)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 3 && path[i - 1] == 14)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 0 && path[i - 1] == 14)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}

		}

		else if (current_node == 3)
		{
			if (path[i + 1] == 11 && path[i - 1] == 2)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 2)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 11)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 11)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 21)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 2)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 5)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 5)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 9)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 5)
		{
			if (path[i + 1] == 6 && path[i - 1] == 4)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 4)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 6 && path[i - 1] == 1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 1)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 6)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 6)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}

		}

		else if (current_node == 7)
		{
		if (path[i + 1] == 8 && path[i - 1] == 6)
		{
			sharp_right_turn_wls();
			stop();
			_delay_ms(100);
			forward_wls(1);
			
			forward();
			_delay_ms(370);
			stop();
		}
		else if (path[i + 1] == 10 && path[i - 1] == 6)
		{
			sharp_left_turn_wls();
			stop();
			_delay_ms(100);
			forward_wls(1);
		}
		else if (path[i + 1] == 8 && path[i - 1] == 10)
		{
			forward();
			_delay_ms(100);
			stop();
			left_turn_wls();
			stop();
			_delay_ms(100);
			forward_wls(1);
			
			forward();
			_delay_ms(370);
			stop();
		}
		else if (path[i + 1] == 6 && path[i - 1] == 10)
		{
			sharp_right_turn_wls();
			stop();
			_delay_ms(100);
			forward_wls(1);
		}
		else if (path[i + 1] == 6 && path[i - 1] == 8)
		{
			sharp_left_turn_wls();
			stop();
			_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i - 1] == 7)
			{
				forward();
				velocity(87,90);
				_delay_ms(300);
				stop();
				
			}
		}

		else if (current_node == 9)
		{
			if (path[i + 1] == 10 && path[i - 1] == 4)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 4)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 10 && path[i - 1] == 12)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 12)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 4 && path[i - 1] == 10)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 10)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 10)
		{
			if (path[i + 1] == 13 && path[i - 1] == 7)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 7)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == 9)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 13)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 7 && path[i - 1] == 13)
			{
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == 9)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == 9)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == 13)
			{
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == 13)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 9 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 11 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 13 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			

		}

		else if (current_node == 13)
		{
			if (path[i + 1] == 10 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 12 && path[i - 1] == 10)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 10 && path[i - 1] == 12)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 14)
		{
			if (path[i + 1] == 19 && path[i - 1] == 2)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i - 1] == 2)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 15)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 15)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 2 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 15 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}

		}

		else if (current_node == 15)
		{
			if (path[i + 1] == 16 && path[i - 1] == 14)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 14)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 16 && path[i - 1] == 1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 1)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 16)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 5 && path[i - 1] == 1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 16)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
				
				forward();
				_delay_ms(370);
				stop();
				
			}
			else if (path[i + 1] == 20 && path[i - 1] == 16)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 18 && path[i - 1] == 20)
			{
				forward();
				_delay_ms(100);
				stop();
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
				
				forward();
				_delay_ms(370);
				stop();
			}
			else if (path[i + 1] == 16 && path[i - 1] == 20)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 16 && path[i - 1] == 18)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i - 1] == 17)
			{
				forward();
				velocity(87,90);
				_delay_ms(300);
				stop();
				
			}

		}

		else if (current_node == 19)
		{
			if (path[i + 1] == 22 && path[i - 1] == 14)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 14)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == 20)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 20)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}

			else if (path[i + 1] == 20 && path[i - 1] == 22)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 14 && path[i - 1] == 22)
			{
				left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 20)
		{
			if (path[i + 1] == 23 && path[i - 1] == 17)
			{
				right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 17)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 23)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);				
				forward_wls(1);
			}
			else if (path[i + 1] == 17 && path[i - 1] == 23)
			{
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 21)
		{
			if (path[i + 1] == 3 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
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
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 23 && path[i - 1] == 21)
				forward_wls(1);
			else if (path[i + 1] == 21 && path[i - 1] == 23)
				forward_wls(1);
			else if (path[i + 1] == 23 && path[i - 1] == 19)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 21 && path[i - 1] == 19)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 23)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 19 && path[i - 1] == 21)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 23)
		{
			if (path[i + 1] == 22 && path[i - 1] == 20)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == 22)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 20 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 22 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}

		else if (current_node == 24)
		{
			
			if (path[i + 1] == 25 && path[i - 1] == 1)
			{
				_delay_ms(14000);
				forward_wls(1);
			}
			else if (path[i + 1] == 1 && path[i - 1] == 25)
			{
				_delay_ms(13000);
				forward_wls(1);
			}
			
		}
		
		else if (current_node == 25)
		{
			if (path[i + 1] == 26 && path[i - 1] == 24)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
				
				forward();
				_delay_ms(370);
				stop();
			}
			else if (path[i + 1] == 27 && path[i - 1] == 24)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
				
				forward();
				_delay_ms(370);
				stop();
			}
			else if (path[i + 1] == 24 && path[i - 1] == 27)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			else if (path[i + 1] == 24 && path[i - 1] == 26)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
		}
		
		
		else if (current_node == 27)
		{
			if (path[i + 1] == 25 && path[i - 1] == -1)
			{
				sharp_right_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			
		}
		
		else if (current_node == 26)
		{
			if (path[i + 1] == 25 && path[i - 1] == -1)
			{
				sharp_left_turn_wls();
				stop();
				_delay_ms(100);
				forward_wls(1);
			}
			
		}
		
		
		if (obstacle_found == 1)	//if obstacle was found while traveling from one node to other then return
		{
			return;
		}	
		obstacle_found = 0;
		current_node = path[i + 1];			//update value of current_node when reached next node
	}


}

/*
*
* Function Name: check_object
* Input: void
* Output: int (1 if object is present in front else 0)
* Logic: uses front distance sensor to detect any object present in front
* Example Call: check_object();
*
*/
int check_object()
{
	sharp = ADC_Conversion(8);
	dist = Sharp_GP2D12_estimation(sharp);
	lcd_print(2,6,dist,3);
	if (dist <= 100)	//if distance is less than 10 cm
	{
		stop();
		PORTB |= 0x0F; //first making all bits high to turn off led
		PORTB &= 0xFB; //glow blue
		_delay_ms(2000);
		PORTB |= 0x0F;
		
		return 1;
	}
	return 0;
}



/*
*
* Function Name: adjust_for_pickup
* Input: void
* Output: void
* Logic: adjusts the position of bot for picking nut according to present node
* Example Call: adjust_for_pickup();
*
*/
void adjust_for_pickup()
{
	if(current_node == 23 && path[path_counter-1] == 20)
	{
		stop();
		forward();
		_delay_ms(520);
		stop();
		_delay_ms(250);
		right_turn_wls();
		stop();
	}
	else if(current_node == 23 && path[path_counter-1] == 22)
	{
		stop();
		forward();
		_delay_ms(400);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 13 && path[path_counter-1] == 10)
	{
		stop();
		forward();
		_delay_ms(400);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 13 && path[path_counter-1] == 12)
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 21 && path[path_counter-1] == 22)
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 12 && path[path_counter-1] == 11)
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 11 && path[path_counter-1] == 3)
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
		left_turn_wls();
		stop();
		_delay_ms(250);
	}
	else if(current_node == 11 && path[path_counter-1] == 12)
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
		right_turn_wls();
		stop();
		_delay_ms(250);
	}
	else
	{
		stop();
		forward();
		_delay_ms(300);
		stop();
		_delay_ms(250);
	}		
}


/*
*
* Function Name: adjust_for_place
* Input: void
* Output: void
* Logic: adjusts the position of bot for placing nut according to present node
* Example Call: adjust_for_place();
*
*/
void adjust_for_place()
{
	if(current_node == 6)
	{
		stop();
		forward();
		_delay_ms(220);
		stop();
		_delay_ms(100);
		left_turn_wls();
		stop();
		_delay_ms(100);
	}
	else if(current_node == 16)
	{
		stop();
		forward();
		_delay_ms(260);
		stop();
		_delay_ms(100);
		left_turn_wls();
		stop();
		_delay_ms(100);
	}
	
	
}


/*
*
* Function Name: adjust_after_pickup
* Input: void
* Output: void
* Logic: adjusts the position of bot after picking the nut according to present node
* Example Call: adjust_after_pickup();
*
*/
void adjust_after_pickup()
{
	_delay_ms(100);
	backward();
	_delay_ms(300);
	stop();
	_delay_ms(100);
}



/*
*
* Function Name: adjust_after_palce
* Input: void
* Output: void
* Logic: adjusts the position of bot after placing the nut according to present node
* Example Call: adjust_after_place();
*
*/
void adjust_after_place()
{
	_delay_ms(100);
	backward();
	_delay_ms(300);
	stop();
	_delay_ms(100);
}




/*
*
* Function Name: check_nut
* Input: void
* Output: void
* Logic: if in pickup node then it checks the color of nut and stores in current_nut_color variable and picks it else current_nut_color is white(W)
* Example Call: check_nut(); 
*
*/
void check_nut()
{
	char c='W';
	current_nut_color = 'W';

	if (current_node == 11)
	{
		
		if (path[path_length - 1] == 3)
		{
			sharp_left_turn_wls();
			_delay_ms(100);	
			left_turn_wls();
		}			
		else if (path[path_length - 1] == 12)
			sharp_right_turn_wls();
			
		_delay_ms(250);
			
		adjust_for_pickup();
		
		left_turn_wls();

		//if (check_object() == 0)
		//	return;

		/*_delay_ms(1000);
		ckeck_color();
		_delay_ms(1000);*/
		//servo_color_pos();
		c=check_color();
		 if ( c=='R')
		{
			current_nut_color = 'R';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if ( c=='G')
		{
			current_nut_color = 'G';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if ( c=='B' )
		{
			current_nut_color = 'B';
			pick();
			adjust_after_pickup();
		}
		else
		{
			adjust_after_pickup();
		}
	}

	else if (current_node == 12)
	{
		if (path[path_length - 1] == 11)
			sharp_left_turn_wls();	
		else if(path[path_length - 1] == 13)			
			sharp_right_turn_wls();	
			
		adjust_for_pickup();

		
		c=check_color();
		if (c=='R')
		{
			current_nut_color = 'R';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
			
			
		}
		else if (c=='G')
		{
			current_nut_color = 'G';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if ( c=='B' )
		{
			current_nut_color = 'B';
			pick();
			adjust_after_pickup();
		}
		else
		{
			adjust_after_pickup();
		}
	}

	else if (current_node == 13)
	{
		if (path[path_length - 1] == 12)
			sharp_left_turn_wls();
		else if(path[path_length - 1] == 10)
			left_turn_wls();
		
		adjust_for_pickup();

		//if (check_object() == 0)
		//	return;

		/*_delay_ms(1000);
		ckeck_color();
		_delay_ms(1000);*/

		//servo_color_pos();
		c=check_color();
		
		if (c=='R')
		{
			current_nut_color = 'R';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if (c=='G')
		{
			current_nut_color = 'G';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if ( c=='B' )
		{
			current_nut_color = 'B';
			pick();
			adjust_after_pickup();
		}
		else
		{
			adjust_after_pickup();
		}
	}

	else if (current_node == 21 )
	{
		if (path[path_length - 1] == 3)
			sharp_right_turn_wls();
		else
			sharp_left_turn_wls();
		
		adjust_for_pickup();

		//if (check_object() == 0)
		//	return;

		/*_delay_ms(1000);
		ckeck_color();
		_delay_ms(1000);*/
		//servo_color_pos();
		c=check_color();
		if (c=='R')
		{
			current_nut_color = 'R';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if (c=='G')
		{
			current_nut_color = 'G';
			//printf("\ncolor = %c", current_nut_color);
			pick();
			
			adjust_after_pickup();
		}
		else if ( c=='B' )
		{
			current_nut_color = 'B';
			pick();
			adjust_after_pickup();
		}
		else
		{
			adjust_after_pickup();
		}
	}

	else if (current_node == 22)
	{
			if (path[path_length - 1] == 21)
				sharp_right_turn_wls();
			else if(path[path_length - 1] == 23)
				sharp_left_turn_wls();
	
	
			adjust_for_pickup();

			//if (check_object() == 0)
				//return;

			/*_delay_ms(1000);
				ckeck_color();
				_delay_ms(1000);*/

			//servo_color_pos();
			c=check_color();
			if (c=='R')
			{
				current_nut_color = 'R';
				//printf("\ncolor = %c", current_nut_color);
				pick();
		
				adjust_after_pickup();
			}
			else if (c=='G')
			{
				current_nut_color = 'G';
				//printf("\ncolor = %c", current_nut_color);
				pick();
		
				adjust_after_pickup();
			}
			else if ( c=='B' )
			{
				current_nut_color = 'B';
				pick();
				adjust_after_pickup();
			}
			else
			{
				adjust_after_pickup();
			}
	}

	else if (current_node == 23)
	{
			if (path[path_length - 1] == 22)
				sharp_right_turn_wls();
			else if(path[path_length - 1] == 20)
				right_turn_wls();
	
			adjust_for_pickup();

			c=check_color();
	
			if (c=='R')
			{
				current_nut_color = 'R';
				//printf("\ncolor = %c", current_nut_color);
				pick();
		
				adjust_after_pickup();
			}
			else if (c=='G')
			{
				current_nut_color = 'G';
				//printf("\ncolor = %c", current_nut_color);
				pick();
		
				adjust_after_pickup();
			}
			else if ( c=='B' )
			{
				current_nut_color = 'B';
				pick();
				adjust_after_pickup();
			}
			else
			{
				adjust_after_pickup();
			}
	}
	
}



/*
*
* Function Name: deal_with_obstacle
* Input: destination node
* Output: void
* Logic: finds new path and follows it if any obstacle is found
* Example Call: deal_with_obstacle();
*
*/
void deal_with_obstacle(int dest)
{
	if( current_node == 16 || path[path_counter+1] == 16 )
	{
		if(current_node == 16 && path[path_counter+1] == 17)
			sharp_right_turn_wls();				
		else if(current_node == 16 && path[path_counter+1] == 15)
			sharp_left_turn_wls();
		else if(current_node == 15 && path[path_counter+1] == 16)
		{
			sharp_right_turn_wls();
			right();
			_delay_ms(800);
		}
		else if(current_node == 17 && path[path_counter+1] == 16)
		{
			sharp_left_turn_wls();
			left();
			_delay_ms(800);
		}
		
		
		_delay_ms(500);								//go forward until next node
		strt_node = current_node;					//make new starting node
		prev_node = path[path_counter+1];
		graph[current_node][prev_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		graph[prev_node][current_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		breadth_first_search(current_node, dest);	//calculate new path
		obstacle_found = 0;
		path[0]=prev_node;							//the node which was next in the old path is made as first node in new path so as to determine orientation of bot
		travel_path();								//travel the path
		stop();
	}
	else if( current_node==6 || path[path_counter+1] == 6 )
	{
		
		if(current_node==6 && path[path_counter+1] == 7)
			sharp_left_turn_wls();								//make reverse turn
		else if(current_node==6 && path[path_counter+1] == 5)
			sharp_right_turn_wls();
		else if(current_node == 5 && path[path_counter+1] == 6)
		{
			sharp_right_turn_wls();
			right();
			_delay_ms(800);
			stop();
		}
		else if(current_node == 7 && path[path_counter+1] == 6)
		{
			sharp_right_turn_wls();
			right();
			_delay_ms(800);
			stop();
		}
		
		
		
		//forward_wls(1);
		_delay_ms(500);								//go forward until next node
		strt_node = current_node;					//make new starting node
		prev_node = path[path_counter+1];
		graph[current_node][prev_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		graph[prev_node][current_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		breadth_first_search(current_node, dest);	//calculate new path
		obstacle_found = 0;
		path[0]=prev_node;							//the node which was next in the old path is made as first node in new path so as to determine orientation of bot
		travel_path();								//travel the path
		stop();
	}
	else
	{
		sharp_left_turn_wls();						//make reverse turn
		forward_wls(1);								//go forward until next node
		_delay_ms(500);						
		strt_node = current_node;					//make new starting node
		prev_node = path[path_counter+1];
		graph[current_node][prev_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		graph[prev_node][current_node] = 0;			//make the edge with obstacle as 0 in graph[][]
		breadth_first_search(current_node, dest);	//calculate new path
		obstacle_found = 0;
		path[0]=prev_node;							//the node which was next in the old path is made as first node in new path so as to determine orientation of bot
		travel_path();								//travel the path
		stop();
	}
	
}

/*
*
* Function Name: Task_4
* Input: void
* Output: void
* Logic: contains full theme implementation logic
* Example Call: Task_4();
*/
void Task_4(void)
{
	int pickup_count = 0;		//counter for pickup nodes 		
	int red_count = 0;			//counter for red nuts
	int green_count = 0;		//counter for green nuts
	int blue_count = 0;			//counter for blue nuts
	
	create_adjacency_matrix(29);					//form matrix of arena
	
	breadth_first_search(0, pickup_nodes[0]);		// find path between start and first pickup node
	travel_path();									//travel the path
	stop();

	if (obstacle_found == 1)						//if obstacle is found in path
	{
		while (obstacle_found != 0)					//while obstacle is present in path
		{
			deal_with_obstacle(pickup_nodes[0]);	//find new path up to first pickup node
		}
	}

	while (1)
	{
		check_nut();	//check if nut is present

		if (current_nut_color == 'W')	//if no nut
		{
			breadth_first_search(pickup_nodes[pickup_count], pickup_nodes[pickup_count + 1]);	//calculate path for next pickup node
			travel_path();//go to next pickup node

			if (obstacle_found == 1)		//for dealing with obstacle
			{
				while (obstacle_found != 0)
				{					
					deal_with_obstacle(pickup_nodes[pickup_count+1]);	//find new path up to next pickup node
				}
			}
			pickup_count++;	//increment counter of pickup nodes by 1
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
					deal_with_obstacle(deposit_node_red[red_count]);	//find new path for red deposit zone node
				}
			}


			if ( red_count == 0 )
			{
				if (path[path_length - 1] == 7)
					sharp_left_turn_wls();
				else
					sharp_right_turn_wls();
			}

			adjust_for_place();			//adjust for placing the nut
			
			/*if(current_node == 8)
			{
				forward();
				_delay_ms(400);
				stop();
			}*/
			
			stop();
			_delay_ms(1000);
			place();					//place the nut
			_delay_ms(1000);
			
			adjust_after_place();
			
			red_count++;				//increment red counter by 1

			if (pickup_count + 1 > 5)//if all pickup nodes are covered then finish by breaking out of loop
				break;

			breadth_first_search(current_node, pickup_nodes[pickup_count + 1]);		//find path to next pickup node
				
			travel_path();//go to next pickup node
			stop();

			if (obstacle_found == 1)//for dealing with obstacle
			{
				while (obstacle_found != 0)				//until obstacle is present
				{
					deal_with_obstacle(pickup_nodes[pickup_count+1]);	//find new path to next pickup node
				}
			}
			pickup_count++;							//increment pickup counter after reaching pickup node
		}

		else if (current_nut_color == 'G')			//if green nut is present
		{
			breadth_first_search(current_node, deposit_node_green[green_count]);//find path for green deposit zone node
			travel_path();//go to green deposit zone node

			if (obstacle_found == 1)
			{
				while (obstacle_found != 0)			//for dealing with obstacles 
				{
					deal_with_obstacle(deposit_node_green[green_count]);
					
				}
			}

			if (green_count == 1)
			{
				if (path[path_length - 1] == 15)
					sharp_left_turn_wls();
				else
					sharp_right_turn_wls();
			}
					
			adjust_for_place();
			
			
			
			stop();
			_delay_ms(1000);
			place();				//place the nut
			_delay_ms(1000);
			
			adjust_after_place();
			
			green_count++;			//increment green counter by 1

			if (pickup_count+1 > 5)//if both green and both red nuts are placed and all pickup nodes are covered then finish by breaking out of loop
				break;

			breadth_first_search(current_node, pickup_nodes[pickup_count + 1]);//find path to next pickup node
			
			travel_path();//go to next pickup node
			stop();

			if (obstacle_found == 1)				//for dealing with obstacle
			{
				while (obstacle_found != 0)			//until obstacle is present in path	
				{
					
					deal_with_obstacle(pickup_nodes[pickup_count+1]);	//find new path to next pickup node
					
				}
			}
			pickup_count++;				//increment pickup counter after reaching pickup node
		}
		
		
		else if (current_nut_color == 'B')			//if blue nut is present
		{
			breadth_first_search(current_node, deposit_node_blue[blue_count]);//find path for blue deposit zone node
			travel_path();//go to blue deposit zone node

			if (obstacle_found == 1)
			{
				while (obstacle_found != 0)			//for dealing with obstacles
				{
					deal_with_obstacle(deposit_node_blue[blue_count]);
					
				}
			}
			
			
			
			adjust_for_place();
			
			/*if(current_node == 26 || current_node == 27)
			{
				forward();
				_delay_ms(400);
				stop();
			}*/
			
			stop();
			_delay_ms(1000);
			place();				//place the nut
			_delay_ms(1000);
			
			adjust_after_place();
			
			blue_count++;			//increment green counter by 1

			if (pickup_count+1 > 5)//if all pickup nodes are covered then finish by breaking out of loop
				break;

			breadth_first_search(current_node, pickup_nodes[pickup_count + 1]);//find path to next pickup node
			
			travel_path();//go to next pickup node
			stop();

			if (obstacle_found == 1)//for dealing with obstacle
			{
				while (obstacle_found != 0)				//
				{
					
					deal_with_obstacle(pickup_nodes[pickup_count+1]);
					
				}
			}
			pickup_count++;					//increment pickup counter after reaching pickup node
		}

		
	}

	breadth_first_search(current_node, 0);		//find path up to start node(node 0)
	travel_path();//go to start node

	if (obstacle_found == 1)//for dealing with obstacles
	{
		while (obstacle_found != 0)			//until obstacle is present
		{
			
			deal_with_obstacle(0);			// find new path up to starting node
			
		}
	}

	left_turn_wls();//turn reverse
	_delay_ms(300);

	stop();//stop the bot
	_delay_ms(2000);
	
	return;


	
	

}



int main(void)
{
	init_devices();
	
	
	lcd_cursor(2,1);
	lcd_string("Dist:");
	
	lcd_cursor(2,11);
	lcd_string("Nxt:");
	
	lcd_cursor(1,13);
	lcd_string("C:");
		
	Task_4();
	buzzer_on();
	
		
	
	
	while(1)
	{
		
		
		
		
	}
}