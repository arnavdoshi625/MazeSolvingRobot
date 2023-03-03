/*
 * controller.h
 *
 *  Created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "main.h"

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;


void moveCells(int8_t n);
void turn(int8_t n);
Heading changeHeading(Heading h, int n);
Heading getHeading();
void setHeading(Heading h);

#endif /* INC_CONTROLLER_H_ */
