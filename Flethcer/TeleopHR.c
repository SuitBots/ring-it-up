#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ir,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     ir1,            sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     fork,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    hand,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    slide,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_6,    hand_vertical,        tServoStandard)
#include "JoystickDriver.c"

void initializeRobot()  {
	//nMotorEncoder[HandL] = 0;
}
/*
typedef enum {GOOD, BAD} motorstate_t;

motorstate_t limit_motor(tMotor motorn) {
if (nMotorEncoder[motorn] <= 0)
return BAD;
else return GOOD;
}
*/

/* motorstate_t limit_hand_motor(tMotor motorn) {
if (nMotorEncoder[motorn] >= 70 || nMotorEncoder[motorn] <= 0)
return BAD;
else return GOOD;
}
*/

void syncMotors (char Motor1, char Motor2) {
	while (nMotorEncoder[Motor1] > nMotorEncoder[Motor2])
		motor[Motor2] += 1;
	while (nMotorEncoder[Motor1] < nMotorEncoder[Motor2])
		motor[Motor1] += 1;
}

void SetMotor(char name, int x) {             // makes a void with argument like in SetServo
	motor[name] = x;                            // The motor will be set to x (specified when the function is called
}


int J1X1()  { return joystick.joy1_x1; }       // These lines make pretty variables for the joysticks
int J1X2()  { return joystick.joy1_x2; }       // J is which logitech controller is being used
int J1Y1()  { return joystick.joy1_y1; }       // X or Y is the axis on which the compiler will read values
int J1Y2()  { return joystick.joy1_y2; }       // 1 or 2 is which joystick values will be read from
int J2X1()  { return joystick.joy2_x1; }
int J2X2()  { return joystick.joy2_x2; }
int J2Y1()  { return joystick.joy2_y1; }
int J2Y2()  { return joystick.joy2_y2; }



void drive() {
	SetMotor(ML, (J1Y2() - J1X2()));
	SetMotor(MR, (J1Y2() + J1X2()));

}

/*void limit_hand_motion() {
if (limit_hand_motor(HandL) == BAD) {
while (nMotorEncoder[HandL] < 5) {
motor[HandL] = 50;
}
}

if (limit_hand_motor(HandR) == BAD) {
while (nMotorEncoder[HandR] < 5) {
motor[HandR] = 50;
}
}
}
*/

void HandOpe(char name, int x) {
	servo[name] = x;
}
void operate_hand_vertical() {
	if (joystick.joy2_TopHat == 4) // down on D-pad
		servo[hand_vertical] = 180; //tune it //180

	if (joystick.joy2_TopHat == 0) // up on D-pad
		servo[hand_vertical] = 60; //tune it  //60
}
void servohand() {
	while (J2Y2() < -10){
		servoChangeRate[hand] = 100000;
		servo[hand] = servo[hand] + 2;
		wait10Msec(1000);
	}
	while (J2Y2() > 10){
		servoChangeRate[hand] = 100000;
		servo[hand] = servo[hand] - 2;
		wait10Msec(1000);
	}
}
void slider() {
	int out = 0;
	int in = 256;
	int stopped = 127;

	if (joy2Btn(5) == 1) { // Left Button
		servoChangeRate[hand] = 100000;
		servo[slide] = out;
		wait10Msec(1);
	}
	if (joy2Btn(6) == 1) {
		servoChangeRate[hand] = 100000;
		servo[slide] = in;
		wait10Msec(1);
	}
	else{
		servoChangeRate[hand] = 100000;
		servo[slide] = stopped;
		wait10Msec(1);
	}
}

void operate_fork()
{
	if (joy2Btn(7) == 1)
	{
		SetMotor(fork, J2X2());
	}
	else {
		SetMotor(HandL, -J2Y2() / 2.75);
		SetMotor(fork, 0);
	}

}
/*void operate_nhand()
{
if(abs(J2Y1()) > 10 && joy2Btn(7) == 0)
{
SetMotor(HandL, -J2Y2() / 2.75);
}
}*/
void ZeroScissorEncoders(){
	if (joy1Btn(2))	{   //The A Button
		nMotorEncoder[ScissorL1] = 0;
		nMotorEncoder[ScissorR1] = 0;
	}
}
void ZeroHandEncoders(){
	if (joy1Btn(1))	{   //The X Button
		nMotorEncoder[ScissorL1] = 0;
		nMotorEncoder[ScissorR1] = 0;
	}
}
void operate_scissor() {
	ZeroScissorEncoders();

	SetMotor(ScissorL1, -J2Y1());
	SetMotor(ScissorR1, -J2Y1());
	SetMotor(ScissorL2, -J2Y1());
	SetMotor(ScissorR2, -J2Y1());
}

void adj() {
	while (joy2Btn(3))	{ // B button
		SetMotor(ScissorL1, 100);
		SetMotor(ScissorL2, 100);

	}

	while (joy2Btn(4)) { // Y button
		SetMotor(ScissorR1, 100);
		SetMotor(ScissorR2, 100);

	}
}



task movement()
{
	drive();
	adj();
	operate_fork();
	slider();
}


task accessories()
{
	nSyncedMotors = synchAB;  //IMPORTANT (to Hunter): the hand motors are plugged in to A and B (not B and C)
	nSyncedTurnRatio = 100;
	nMotorEncoder[HandL] = 0;
	operate_scissor();
	operate_hand_vertical();
	servohand();
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
