#include "solver.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "irs.h"

bool initOrNot = true;

void push(Queue* q, Coord c)
{
    q->queue[q->back] = c;
    q->back--;
    if (q->back <= -1)
        q->back = 255;
    q->size++;
}

Coord pop(Queue* q)
{
    Coord cur = {-1, -1};
    if (q->front != q->back)
    {
        cur = q->queue[q->front];
        //q->queue[q->front] = NULL;
        q->front--;
        if (q->front <= -1)
            q->front = 255;
        q->size--;
        return cur;
    }
    else
        return cur;
}

bool empty(Queue* q)
{
    if (q->size == 0)
        return true;
    else
        return false;
}


// This function redirects function calls from mouse.c to the desired maze solving algorithm
Action solver()
{
    //printf("ok programs running\n");
    // This can be changed to call other maze solving algorithms
	Action flood = floodFill();
	switch(flood)
	    {
	        // Update position and goal movement depending on heading
	        case FORWARD:
	            if(getFrontReading())
	            {
	                printf("Error: mouse attempted to move through wall\nPress s to resume\n");
	                //running = 0;
	                return IDLE;
	            }
	            else
	            {
	                switch(getHeading())
	                {
	                    case NORTH:
	                        mouseY++;
	                        break;

	                    case EAST:
	                        mouseX++;
	                        break;

	                    case SOUTH:
	                        mouseY--;
	                        break;

	                    case WEST:
	                        mouseX--;
	                        break;
	                }
	                break;
	            }

	        // Update heading and set goal rotation amount
	        case LEFT:
	            break;

	        // Update heading and set goal rotation amount
	        case RIGHT:
	            break;

	        // Do nothing
	        case IDLE:
	            break;
	    }

    return flood;
}


void initialize()
{
	mouseX = 0;
	mouseY = 0;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            horizWalls[i][j] == false;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        horizWalls[0][i] = true;
        horizWalls[6][i] = true;
    }

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            vertWalls[i][j] == false;
        }
    }
    for (int i = 0; i < 6; i++)
    {
        vertWalls[i][0] = true;
        vertWalls[i][6] = true;
    }

    // Initializes default manhattan distance array given maze dimensions
    /*for (int i = 0; i < mL; i++)
    {
        for (int j = 0; j < mL; j++)
        {
            if (mL % 2 == 0)
            {
                distances[i][j] = abs(mL / 2 - i - ((i < mL / 2) ? 1 : 0)) + abs(mL / 2 - j - ((j < mL / 2) ? 1 : 0));
            }
            else
            {
                distances[i][j] = abs(floor(mL / 2) - i) + abs(floor(mL / 2) - j);
            }
        }
    }*/


    distances[0][0] = 10;
    distances[0][1] = 9;
    distances[0][2] = 8;
    distances[0][3] = 7;
    distances[0][4] = 6;
    distances[0][5] = 5;
	distances[1][0] = 9;
	distances[1][1] = 8;
	distances[1][2] = 7;
	distances[1][3] = 6;
	distances[1][4] = 5;
	distances[1][5] = 4;
	distances[2][0] = 8;
	distances[2][1] = 7;
	distances[2][2] = 6;
	distances[2][3] = 5;
	distances[2][4] = 4;
	distances[2][5] = 3;
	distances[3][0] = 7;
	distances[3][1] = 6;
	distances[3][2] = 5;
	distances[3][3] = 4;
	distances[3][4] = 3;
	distances[3][5] = 2;
	distances[4][0] = 6;
	distances[4][1] = 5;
	distances[4][2] = 4;
	distances[4][3] = 3;
	distances[4][4] = 2;
	distances[4][5] = 1;
	distances[5][0] = 5;
	distances[5][1] = 4;
	distances[5][2] = 3;
	distances[5][3] = 2;
	distances[5][4] = 1;
	distances[5][5] = 0;



	/*
    //fill bottom left quadrant 
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < x; y++)
        {
            distances[x][y] = distances[y][x];
        }
    }

    //reflect into top left quadrant
    for (int x = 0; x < 8; x++)
    {
        for (int y = 8; y < 16; y++)
        {
            distances[x][y] = distances[x][16 - y];
        }
    }

    //reflect into bottom right quadrant
    for (int x = 8; x < 16; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            distances[x][y] = distances[16 - x][y];
        }
    }

    //reflect into top right quadrant
    for (int x = 8; x < 16; x++)
    {
        for (int y = 8; y < 16; y++)
        {
            distances[x][y] = distances[x][16 - y];
        }
    }*/

    /*for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%02d ", distances[i][j]);
        }
        printf("\n");
    }*/
}

Action floodFill()
{
    
    if (initOrNot)
    {
        initialize();
        initOrNot = false;
    }

    if (mouseX == 5 && mouseY == 5)
    	return IDLE;

    Heading dir = getHeading();
    
    int xx = mouseX;
    int yy = mouseY;

    if (getFrontReading() == true)
    {
        if (dir == NORTH)
        {
            horizWalls[yy + 1][xx] = true;
        }
        else if (dir == SOUTH)
        {
            horizWalls[yy][xx] = true;
        }
        else if (dir == EAST)
        {
            vertWalls[yy][xx + 1] = true;
        }
        else if (dir == WEST)
        {
            vertWalls[yy][xx] = true;
        }
    }
    if (getLeftReading() == true)
    {
        if (dir == NORTH)
        {
            vertWalls[yy][xx] = true;
        }
        else if (dir == SOUTH)
        {
            vertWalls[yy][xx + 1] = true;
        }
        else if (dir == EAST)
        {
            horizWalls[yy + 1][xx] = true;
        }
        else if (dir == WEST)
        {
            horizWalls[yy][xx] = true;
        }
    }
    if (getRightReading() == true)
    {
        if (dir == NORTH)
        {
            vertWalls[yy][xx + 1] = true;
        }
        else if (dir == SOUTH)
        {
            vertWalls[yy][xx] = true;
        }
        else if (dir == EAST)
        {
            horizWalls[yy][xx] = true;
        }
        else if (dir == WEST)
        {
            horizWalls[yy + 1][xx] = true;
        }
    }

    /*printf("horiz\n");
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 16; j++)
            printf("%d ", ((horizWalls[i][j]) ? 1 : 0));
        printf("\n");
    }

    printf("vert\n");
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 17; j++)
            printf("%d ", ((vertWalls[i][j]) ? 1 : 0));
        printf("\n");
    }*/

    int steps = distances[mouseX][mouseY];
    if (steps == 0)
    {
        return IDLE;
    }

    if (dir == NORTH)
    {
        if (getFrontReading() == false && distances[xx][yy + 1] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx - 1][yy] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx + 1][yy] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == SOUTH)
    {
        if (getFrontReading() == false && distances[xx][yy - 1] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx + 1][yy] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx - 1][yy] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == EAST)
    {
        if (getFrontReading() == false && distances[xx + 1][yy] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx][yy + 1] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx][yy - 1] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == WEST)
    {
        if (getFrontReading() == false && distances[xx - 1][yy] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx][yy - 1] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx][yy + 1] < steps)
        {
            return RIGHT;
        }
    }


    Queue cellQueue;
    Queue* q = &cellQueue;
    q->front = 255;
    q->back = 255;
    q->size = 0;

    printf("initializes queue\n");

    //add current cell to queue
    Coord c = {mouseX, mouseY};
    //c.x = mouse->x;
    //c.y = mouse->y;
    push(q, c);

    printf("add cur to queue\n");
    
    //while queue is not empty
    while (!empty(q))
    {
        printf("top of while loop\n");
        //take front cell in queue 'out of line' for consideration
        Coord cur = pop(q);
        //get front cell's minimum value amongst accessible neighbors
        int min = 10000000;
        printf("this: %d\n", distances[cur.x][cur.y]);
        //east
        if (vertWalls[cur.y][cur.x + 1] == false && distances[cur.x + 1][cur.y] < min)
        {
            min = distances[cur.x + 1][cur.y];
            printf("east: %d\n", distances[cur.x + 1][cur.y]);
        }
        //west
        if (vertWalls[cur.y][cur.x] == false && distances[cur.x - 1][cur.y] < min)
        {
            min = distances[cur.x - 1][cur.y];
            printf("west: %d\n", distances[cur.x - 1][cur.y]);
        }
        //north
        if (horizWalls[cur.y + 1][cur.x] == false && distances[cur.x][cur.y + 1] < min)
        {
            min = distances[cur.x][cur.y + 1];
            printf("north: %d\n", distances[cur.x][cur.y + 1]);
        }
        //south
        if (horizWalls[cur.y][cur.x] == false && distances[cur.x][cur.y - 1] < min)
        {
            min = distances[cur.x][cur.y - 1];
            printf("south: %d\n", distances[cur.x][cur.y - 1]);
        }
        //if front cell's value <= minimum of its neighbors, set front cell's value to minimum + 1 and add all accessible neighbors to queue

        printf("current cell: %d min = %d\n", distances[cur.x][cur.y], min);

        if (distances[cur.x][cur.y] <= min)
        {
            printf("front cell's value is less than min of neighbors\n");
            distances[cur.x][cur.y] = min + 1;
            //east
            Coord n1 = {cur.x + 1, cur.y};
            if (cur.x + 1 < 16 && vertWalls[cur.y][cur.x + 1] == false)
            {
                push(q, n1);
                printf("pushing cell (%d, %d) \n", n1.x, n1.y);
            }
            //west
            Coord n2 = {cur.x - 1, cur.y};
            if (cur.x - 1 >= 0 && vertWalls[cur.y][cur.x] == false)
            {
                push(q, n2);
                printf("pushing cell (%d, %d) \n", n2.x, n2.y);
            }
            //north
            Coord n3 = {cur.x, cur.y + 1};
            if (cur.y + 1 < 16 && horizWalls[cur.y + 1][cur.x] == false)
            {
                push(q, n3);
                printf("pushing cell (%d, %d) \n", n3.x, n3.y);
            }
            //south
            Coord n4 = {cur.x, cur.y - 1};
            if (cur.y - 1 >= 0 && horizWalls[cur.y][cur.x] == false)
            {
                push(q, n4);
                printf("pushing cell (%d, %d) \n", n4.x, n4.y);
            }
        }
    }

    /*for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            printf("%02d ", distances[i][j]);
        }
        printf("\n");
    }*/


    if (dir == NORTH)
    {
        if (getFrontReading() == false && distances[xx][yy + 1] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx - 1][yy] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx + 1][yy] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == SOUTH)
    {
        if (getFrontReading() == false && distances[xx][yy - 1] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx + 1][yy] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx - 1][yy] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == EAST)
    {
        if (getFrontReading() == false && distances[xx + 1][yy] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx][yy + 1] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx][yy - 1] < steps)
        {
            return RIGHT;
        }
    }
    else if (dir == WEST)
    {
        if (getFrontReading() == false && distances[xx - 1][yy] < steps)
        {
            return FORWARD;
        }
        if (getLeftReading() == false && distances[xx][yy - 1] < steps)
        {
            return LEFT;
        }
        if (getRightReading() == false && distances[xx][yy + 1] < steps)
        {
            return RIGHT;
        }
    }

    return LEFT;
}
