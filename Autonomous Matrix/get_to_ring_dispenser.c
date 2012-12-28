#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          Scoop,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_3,    HandLR,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_4,    HandOp,               tServoStandard)

#include "JoystickDriver.c"
#include "Autonomous_Base.h"

void resetMotors(char Motor1, char Motor2) {
	nMotorEncoder[Motor1] = 0;
	nMotorEncoder[Motor2] = 0;
}

task main()
{
	initializeRobot(ML, MR, HandL, HandR, ScissorL1, ScissorR1);
	waitForStart();

	while (nMotorEncoder[ML] < 4000 && nMotorEncoder[MR] < 4000) { // Forward a bit
		motor[ML] = 75;
		motor[MR] = 75;
	}
	resetMotors(ML, MR);

	while (nMotorEncoder[ML] < 3333 && nMotorEncoder[MR] > -3333) { // Turn right 90 degrees
		motor[ML] = 75;
		motor[MR] = -75;
	}
	resetMotors(ML, MR);

	while (nMotorEncoder[ML] < 4000 && nMotorEncoder[MR] < 4000) {
		motor[ML] = 75;
		motor[MR] = 75;
	}
	resetMotors(ML, MR);

	while (nMotorEncoder[ML] < 3333 && nMotorEncoder[MR] > -3333) { // Turn right 90 degrees
		motor[ML] = 75;
		motor[MR] = -75;
	}
	resetMotors(ML, MR);

	while (nMotorEncoder[ML] < 4000 && nMotorEncoder[MR] < 4000) {
		motor[ML] = 75;
		motor[MR] = 75;
	}

}
