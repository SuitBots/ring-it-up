#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ir,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     ir1,            sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     fork,          tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_6,    hand_vertical,        tServoStandard)


#include "JoystickDriver.c"
#include "TeleOp_FunctionLib.h"

int counter = 0;

void initializeRobot()  {
	nMotorEncoder[HandL] = 0;
}
	//SetMotor(ML, (J1Y2() - J1X2()));
	//SetMotor(MR, (J1Y2() + J1X2()));



void operateLR() {
	int left = 0;
	int right = 256;
	int stopped = 127;

	if (joy2Btn(5)) { // Left Button
		servo[HandLR] = left;
	}
	else if (joy2Btn(6)) { // Right Button
		servo[HandLR] = right;
	}
	else {
		servo[HandLR] = stopped;
	}
}

void limit_hand_motion() {
	int distance_limit = 180; //placeholder

	if (joy2Btn(8)) // Select Button
		nMotorEncoder[HandL] = 0;
	if (! joy2Btn(0)) { // X Button
		if (nMotorEncoder[HandL] > distance_limit
			  || nMotorEncoder[HandL] < distance_limit * -1)
			motor[HandL] = 0;
	}
}


void HandOpe(char name, int x) {
	servo[name] = x;
}

void operate_scissor() {
	SetMotor(ScissorL1, -J2Y1());
	SetMotor(ScissorR1, -J2Y1());
	SetMotor(ScissorL2, -J2Y1());
	SetMotor(ScissorR2, -J2Y1());

	if (joy2Btn(3))	{ // B button
		SetMotor(ScissorL1, 90);
		SetMotor(ScissorL2, 90);
	}

	if (joy2Btn(4)) { // Y button
		SetMotor(ScissorR1, 90);
		SetMotor(ScissorR2, 90);
	}
}

void operate_hand_vertical() {
	if (joystick.joy2_TopHat == 0) // up on D-pad
		servo[hand_vertical] = 255 / 2; //tune it

	if (joystick.joy2_TopHat == 2) // right on D-pad
		servo[hand_vertical] = 255 / 4; //tune it

	if (joystick.joy2_TopHat == 4) // down on D-pad
		servo[hand_vertical] = 0; //tune it
}

void fork_go_up_finite_amount() {
	long counter1 = 0;

		while (counter1 < 1000)
		{
			motor[fork] = 100;
			++counter1;
		}
}

void fork_go_down() {
	motor[fork] = -100;
}

void operate_fork () {

  if (joy2Btn(8)
      && counter == 0) {
    fork_go_up_finite_amount ();
    ++counter;
    fork_go_down ();
  }
}

task movement()
{
	expDrive_Crawl(ML, MR, J1Y2(), J1X2());
	operateLR();
}


task accessories()
{
	nSyncedMotors = synchAB;  //IMPORTANT (to Hunter): the hand motors are plugged in to A and B (not B and C)
	nSyncedTurnRatio = 100;
	nMotorEncoder[HandL] = 0;

	operateLR();

	int hand_ritard = 2.75;

	SetMotor(HandL, -J2Y2() / hand_ritard);
	//limit_hand_motion();

	operate_scissor();
	operate_hand_vertical();
	operate_fork();
}


task main()
{
	initializeRobot();

	waitForStart();

	while (true) {
		StartTask(movement);
		StartTask(accessories);
		getJoystickSettings(joystick);
	}
}
