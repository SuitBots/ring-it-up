//
// DistCalib.c
//
// Copyright (c) 2013 Suit Bots 4628
//

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

static const float DRIVE_SCALE_FACTOR = 68.8973;

void DeadReckoningDriveForwardCM(long amount) {
	bool direction = amount > 0;
	amount = abs(amount);
	forward(100, amount * DRIVE_SCALE_FACTOR, direction, ML, MR);
}

void IRAutonomous() {
	DeadReckoningDriveForwardCM(100);
}


task main() {
	initializeRobot();
	waitForStart();
	IRAutonomous();
}
