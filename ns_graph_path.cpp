#include<stdio.h>
#include<stdlib.h>
#define MAX 30


int graph[30][30],path[20],current_node,path_length,path_counter;

int obstacle_found = 0, strt_node;


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
	printf("\n\n");
	for (i = 0; i < 28; i++)
	{
		for (int k = 0; k < 28; k++)
		{
			if (distance[k] == j)
			{
				printf("%d ", k);
				printf("  parent=%d  ", previous[k]);
			}
		}
		j++;
		printf("\n\n");
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


int main()
{
	create_adjacency_matrix(29);
	breadth_first_search(23,0);
	
	printf("\n%d\n\n",path_length);
	return 0;
}
