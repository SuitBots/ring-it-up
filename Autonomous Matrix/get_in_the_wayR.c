//
// get_in_the_warR.c
//
// Copyright (c) 2013 Suit Bots 4628
//

#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorB,          HandL,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          HandR,         tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DriveL,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DriveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR,      tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    HandOp,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Autonomous_Base.h"
#include "JoystickDriver.c"

// This autonomous' goal is to get to the opponent's platform and get in the way
//
// +----------------------+
// | \                  / |     R - Starting point
// |   \       ......./.R |     . - Robot Path
// |     +----.-----+     |     S - Raise Scissor
// |     |   .      |     |     / or \ - team boundries
// |     |  S       |     |     + or - - borders
// |     |          |     |
// |     |          |     |
// |     +----------+     |
// |   /              \   |
// | /                  \ |
// +----------------------+
//

void resetMotors (char name, char name1) {
	nMotorEncoder[name] = 0;
	nMotorEncoder[name1] = 0;
}

task main()  { // the Trues and Falses might have to be flipped
	initializeRobot(HandL, HandR, DriveL, DriveR, ScissorL, ScissorR);

	waitForStart();
	while (nMotorEncoder[DriveL] < 14000 && nMotorEncoder[DriveR] < 14000) {
		motor[DriveL] = 75;
		motor[DriveR] = 75;
	}
	resetMotors(DriveL, DriveR);

	while (nMotorEncoder[DriveL] > -3333 && nMotorEncoder[DriveR] < 3333) {
		motor[DriveL] = -75;
		motor[DriveR] = 75;
	}

	resetMotors(DriveL, DriveR);

	while (nMotorEncoder[DriveL] < 4250 && nMotorEncoder[DriveR] < 4250) {
		motor[DriveL] = 75;
		motor[DriveR] = 75;
	}
	resetMotors(DriveL, DriveR);

	servo[HandOp] = 0;

	//while (nMotorEncoder[ScissorL] < 20000 && nMotorEncoder[ScissorR] < 20000) {
	//	motor[ScissorL] = 100;
	//	motor[ScissorR] = 100;
	//}

	//forward(75, 10000, true, DriveL, DriveR);  //1 is a placeholder, calibrate actual distance

	//pivotTurn(75, 3000, true, DriveL, DriveR);  //1 " " ", claibrate actual degree turn

	//forward(100, 20000, true, ScissorL, ScissorR);

}
