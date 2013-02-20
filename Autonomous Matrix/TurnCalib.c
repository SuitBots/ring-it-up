#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     ir,             sensorI2CCustom)
#pragma config(Sensor, S4,     ir1,            sensorI2CCustom)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     fork,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_6,    hand_vertical,        tServoStandard)

#include "JoystickDriver.c"
#include "Autonomous_Base.h"
//#include "Drive_to_dispenser_interface.h"


void initializeRobot() {
	return;
}

void TurnLeftThisManyDegrees (int degrees) {
	const float SCALE_FACTOR = 30;
	int abs_degrees = abs(degrees);
	swingTurn(100, abs_degrees * SCALE_FACTOR, degrees > 0, ML, MR);
}

void IRAutonomous() {
	TurnLeftThisManyDegrees(45);
}

task main() {
	initializeRobot();
	waitForStart();
	IRAutonomous();
}
