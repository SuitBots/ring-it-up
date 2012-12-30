#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     fork_prop,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_4,    fork_drop,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_6,    hand_vertical,        tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

void initializeRobot()  {
	nMotorEncoder[ScissorL1] = 0;  //This may be the solution to our problems
	nMotorEncoder[ScissorR1] = 0;
	//servo[HandLR] = 127;
	//servo[HandOp] = 0;
}

void syncMotors (char Motor1, char Motor2) {
	while (nMotorEncoder[Motor1] > nMotorEncoder[Motor2])
		motor[Motor2] += 1;
	while (nMotorEncoder[Motor1] < nMotorEncoder[Motor2])
		motor[Motor1] += 1;
}

void SetMotor(char name, int x) {             //makes a void with argument like in SetServo
	motor[name] = x;                            //The motor will be set to x (specified when the function is called
}


int J1X1 = joystick.joy1_x1;       //These lines make pretty variables for the joysticks
int J1X2 = joystick.joy1_x2;       //J is which logitech controller is being used
int J1Y1 = joystick.joy1_y1;       //X or Y blah is which direction the joystick will go
int J1Y2 = joystick.joy1_y2;
int J2X1 = joystick.joy2_x1;
int J2X2 = joystick.joy2_x2;
int J2Y1 = joystick.joy2_y1;
int J2Y2 = joystick.joy2_y2;



void drive() {
	int crawlRate = 25;  // adjust this for a different crawl rate
	if (joy1Btn(6)){
		SetMotor(ML, (J1Y2 / crawlRate + J1X2 / crawlRate));  // crawl rate means that it'll go slowly if the driver
		SetMotor(MR, (J1Y2 / crawlRate - J1X2 / crawlRate));  // pushes a button
	} else {
		SetMotor(ML, (J1Y2 + J1X2));
		SetMotor(MR, (J1Y2 - J1X2));
	}
}


void operateLR() {

	if (joy2Btn(5)) {
		servo[HandLR] = 0;
	}
	else if (joy2Btn(6)) {
		servo[HandLR] = 256;
	}
	else {
		servo[HandLR] = 127;
	}
}

void limit_hand_motion() {
	int distance_limit = 180; //placeholder
	if (! joy2Btn(8)) {
		if (nMotorEncoder[HandL] > distance_limit
			  || nMotorEncoder[HandL] < distance_limit * -1)
			motor[HandL] = 0;
	}
}


void HandOpe(char name, int x) {
	servo[name] = x;
}

void operate_scissor() {
	SetMotor(ScissorL1, -J2Y1);
	SetMotor(ScissorR1, -J2Y1);
	SetMotor(ScissorL2, -J2Y1);
	SetMotor(ScissorR2, -J2Y1);

	if (joy2Btn(3))	{ // B button
		SetMotor(ScissorL1, 75);
		SetMotor(ScissorL2, 75);
	}

	if (joy2Btn(4)) { // Y button
		SetMotor(ScissorR1, 75);
		SetMotor(ScissorR2, 75);
	}
}

void operate_hand_vertical() {
	if (joystick.joy2_TopHat == 0) // up on D-pad
		servo[hand_vertical] = 255; //tune it

	if (joystick.joy2_TopHat == 2) // right on D-pad
		servo[hand_vertical] = 125; //tune it

	if (joystick.joy2_TopHat == 4) // down on D-pad
		servo[hand_vertical] = 0; //tune it
}

void operate_fork_assembly() {
		if (joy2Btn(9)) // start button
		servo[fork_drop] = 100; //tune it

	if (joy2Btn(6)) // left trigger
		motor[fork_prop] = 100;
}


task movement()
{
	drive();
	operateLR();
}


task accessories()
{
	nSyncedMotors = synchAB;  //IMPORTANT (to Hunter): the hand motors are plugged in to A and B (not B and C)
	nSyncedTurnRatio = 100;
	nMotorEncoder[HandL] = 0;

	operateLR();

	SetMotor(HandL, -J2Y2 / 2.75);
	limit_hand_motion();


}


task main()
{
	initializeRobot();

	waitForStart();

	while (true) {
		StartTask(movement);
		StartTask(accessories);
	}
}
