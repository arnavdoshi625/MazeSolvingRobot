/*
 * pid.c
 *
 *  Created on: Sep 27, 2020
 *      Author: Tyler Price
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"
#include "pid.h"

int16_t angleError = 0;
int16_t oldAngleError = 0;
float angleCorrection;
float distanceCorrection;
float distanceError = 0;
float oldDistanceError = 0;
const float kPw = 0.1;
const float kDw = 0.1;
const float kPx = 0.2;
const float kDx = 0.1;
int16_t leftEncoder = 0;
int16_t rightEncoder = 0;
int16_t goalA = 0;
int16_t goalD = 0;
int8_t done = 0;
int16_t count = 0;

void resetPID()
{
	/*
	 * For assignment 3.1: This function does not need to do anything
	 * For assignment 3.2: This function should reset all the variables you define in this file to help with PID to their default
	 *  values. You should also reset your motors and encoder counts (if you tell your rat to turn 90 degrees, there will be a big
	 * difference in encoder counts after it turns. If you follow that by telling your rat to drive straight without first
	 * resetting the encoder counts, your rat is going to see a huge angle error and be very unhappy).
	 *
	 * You should additionally set your distance and error goal values (and your oldDistanceError and oldAngleError) to zero.
	 */
	angleError = 0;
	oldAngleError = 0;
	distanceError = 0;
	oldDistanceError = 0;
	leftEncoder = 0;
	rightEncoder = 0;
	resetMotors();
	resetEncoders();
	done = 0;
	count = 0;
	goalA = 0;
	goalD = 0;

}

void updatePID()
{
	/*
	 * This function will do the heavy lifting PID logic. It should do the following: read the encoder counts to determine an error,
	 * use that error along with some PD constants you determine in order to determine how to set the motor speeds, and then actually
	 * set the motor speeds.
	 *
	 * For assignment 3.1: implement this function to get your rat to drive forwards indefinitely in a straight line. Refer to pseudocode
	 * example document on the google drive for some pointers
	 *
	 * TIPS (assignment 3.1): Create kPw and kDw variables, use a variable to store the previous error for use in computing your
	 * derivative term. You may get better performance by having your kDw term average the previous handful of error values instead of the
	 * immediately previous one, or using a stored error from 10-15 cycles ago (stored in an array?). This is because systick calls so frequently
	 * that the error change may be very small and hard to operate on.
	 *
	 * For assignment 3.2: implement this function so it calculates distanceError as the difference between your goal distance and the average of
	 * your left and right encoder counts. Calculate angleError as the difference between your goal angle and the difference between your left and
	 * right encoder counts. Refer to pseudocode example document on the google drive for some pointers.
	 */

	leftEncoder = getLeftEncoderCounts();
	rightEncoder = getRightEncoderCounts();

	angleError = goalA + (rightEncoder - leftEncoder);
	angleCorrection = (kPw * angleError) + kDw * (angleError - oldAngleError);
	oldAngleError = angleError;
	if (angleCorrection < -0.3)
		angleCorrection = -0.3;
	else if (angleCorrection > 0.3)
		angleCorrection = 0.3;

	distanceError = goalD - (rightEncoder + leftEncoder)/2;
	distanceCorrection = kPx * distanceError + kDx * (distanceError - oldDistanceError);
	oldDistanceError = distanceError;
	if (distanceCorrection < -0.5)
		distanceCorrection = -0.5;
	else if (distanceCorrection > 0.5)
		distanceCorrection = 0.5;

	float leftPower = distanceCorrection + angleCorrection;
	float rightPower = distanceCorrection - angleCorrection;
	setMotorLPWM(leftPower);
	setMotorRPWM(rightPower);

	if (angleError <= 10 && angleError >= -10
			&& distanceError <= 10 && distanceError >= -10)
	{
		count++;
		if (count >= 50)
			done = 1;
		else
			done = 0;
	}
	else
		count = 0;
}

void setPIDGoalD(int16_t distance)
{
	/*
	 * For assignment 3.1: this function does not need to do anything.
	 * For assignment 3.2: this function should set a variable that stores the goal distance.
	 */
	goalD = distance;
}

void setPIDGoalA(int16_t angle)
{
	/*
	 * For assignment 3.1: this function does not need to do anything
	 * For assignment 3.2: This function should set a variable that stores the goal angle.
	 */
	goalA = angle;
}

int8_t PIDdone(void)
{
	/*
	 * For assignment 3.1: this function does not need to do anything (your rat should just drive straight indefinitely)
	 * For assignment 3.2:This function should return true if the rat has achieved the set goal. One way to do this by having updatePID() set some variable when
	 * the error is zero (realistically, have it set the variable when the error is close to zero, not just exactly zero). You will have better results if you make
	 * PIDdone() return true only if the error has been sufficiently close to zero for a certain number, say, 50, of SysTick calls in a row.
	 */

	return done;
}
