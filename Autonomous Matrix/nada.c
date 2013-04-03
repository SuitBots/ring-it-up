//
// nada.c
//
// Copyright (c) 2013 Suit Bots 4628
//

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
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)

#include "JoystickDriver.c"

task main() {
	waitForStart();

	nMotorEncoderTarget[ML] = 1000;
	nMotorEncoderTarget[MR] = 1000;
	motor[ML] = 75;
	motor[MR] = 75;
}
