#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>
#include "controller.h"

#define QUEUESIZE 256

typedef enum Action {LEFT, FORWARD, RIGHT, IDLE} Action;

bool horizWalls[7][6];
bool vertWalls[6][7];
int distances[6][6];
int mouseX;
int mouseY;

void initialize();
Action solver();
Action floodFill();

typedef struct {
	int x;
	int y;
} Coord;

typedef struct {
	Coord queue[QUEUESIZE];
	int front;
	int back;
	int size;
} Queue;

void push(Queue* q, Coord c);
Coord pop(Queue* q);
bool empty(Queue* q);


#endif
