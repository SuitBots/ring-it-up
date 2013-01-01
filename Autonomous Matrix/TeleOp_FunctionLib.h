//
// TeleOp_FuntionLib.h
//
// Copyright (c) 2012 Suit Bots
//

void Arcade_Drive(char DrivL, char DrivR, int joy1, int joy2)  {
	motor[DrivL] = joy1 - joy2;
	motor[DrivR] = joy1 + joy2;
}

void Tank_Drive(char DrivL, char DrivR, int joy1, int joy2)  {
	motor[DrivL] = joy1;
	motor[DrivR] = joy2;
}

//void Operate_Hand(bool motorORservo,

int J1X1() { return joystick.joy1_x1; }       //These lines make pretty variables for the joysticks
int J1X2() { return joystick.joy1_x2; }       //J is which logitech controller is being used
int J1Y1() { return joystick.joy1_y1; }       //X or Y blah is which direction the joystick will go
int J1Y2() { return joystick.joy1_y2; }
int J2X1() { return joystick.joy2_x1; }
int J2X2() { return joystick.joy2_x2; }
int J2Y1() { return joystick.joy2_y1; }
int J2Y2() { return joystick.joy2_y2; }

void SetMotor(char name, int x) {
	motor[name] = x;
}

void syncMotors (char Motor1, char Motor2) {
	while (nMotorEncoder[Motor1] > nMotorEncoder[Motor2])
		motor[Motor2] += 1;
	while (nMotorEncoder[Motor1] < nMotorEncoder[Motor2])
		motor[Motor1] += 1;
}
