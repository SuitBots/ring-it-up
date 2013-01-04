#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     ir,             sensorI2CCustom)
#pragma config(Sensor, S4,     ir1,            sensorI2CCustom)
#pragma config(Motor,  motorA,          HandL,         tmotorNXT, openLoop, reversed, encoder)
#pragma config(Motor,  motorB,          HandR,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          Scoop,         tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     ML,            tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     MR,            tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     ScissorL1,     tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     ScissorR1,     tmotorMatrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     ScissorL2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     ScissorR2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     fork,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "Autonomous_Base.h"

void initializeRobot() {
	return;
}

/// @returns 0 for the left column, 1 for middle, 2 for right
peg_t FindTheColumnThatTheIRBeaconIsOn() {
	peg_t peg = dondePeg(ir, ir1);
	return peg;
}

void DeadReckoningDriveForwardCM(long amount) {
	float scale_factor = 68.8973;
	bool direction = amount > 0;
	amount = abs(amount);
	//motor[ML] = motor[MR] = 100;
	//wait1Msec(1000);
	forward(100, amount * scale_factor, direction, ML, MR);
}

/// @returns the amount we should drive forward in phase 1 for each column
long InitialForward(peg_t column) {
	if (column == left) {
		return 150; // tune it
	}
	if (column == middle) {
		return 20; // tune it
	}
	if (column == right) {
		return 5; // tune it
	}
	else return -5; // tune it
}

/// @returns the amount we should back up from our column to get to the dispensor
long BackUpAmount (peg_t column) {
	if (column == left) {
		return -300; // tune it
	}
	if (column == middle) {
		return -200; // tune it
	}
	if (column == right) {
		return -100; // tune it
	}
	else return -200; // tune it
}

/// @returns the amount we should drive forward in phase 2 for each column
long SecondForward(peg_t column) {
	if (column == left) {
		return 300; // tune it
	}
	if (column == middle) {
		return 200; // tune it
	}
	if (column == right) {
		return 100; // tune it
	}
	else return 200; // tune it
}


/// @returns the amount we should drive forward to address the column
long ThirdForward(peg_t column) {
	if (column == left) {
		return 300; // tune it
	}
	if (column == middle) {
		return 200; // tune it
	}
	if (column == right) {
		return 100; // tune it
	}
	else return 200; // tune it
}

void TurnLeftThisManyDegrees (int degrees) {
	float scale_factor = 37;
	int abs_degrees = abs(degrees);
	swingTurn(100, abs_degrees * scale_factor, degrees > 0, ML, MR);
}

// Raises the scissor lift to the correct level for peg 1
// Also tilts the hand forward
void RaiseTheScissorToPeg1Level () {
	forward(100, 300000, false, ScissorL1, ScissorR1); //tune the 300000 in both of these
	forward(100, 300000, false, ScissorL2, ScissorR2);
}

void DropScissorLift () {
	forward(100, 300000, true, ScissorL1, ScissorR1); //tune the 300000 in both of these
	forward(100, 300000, true, ScissorL2, ScissorR2);
}

// Returns true when we're close enough to the peg to stop
bool ProximitySensorSaysStop () {
	int PROXIMITY_POWER_CUTOFF = 60;
	int left_power = IRmax_sig(ir);
	int right_power = IRmax_sig(ir1);

	if (left_power == 0
		|| right_power == 0
		|| (left_power < PROXIMITY_POWER_CUTOFF
		   && right_power < PROXIMITY_POWER_CUTOFF))
		writeDebugStreamLine("STOP");

	return left_power == 0
		|| right_power == 0
		|| (left_power < PROXIMITY_POWER_CUTOFF
		   && right_power < PROXIMITY_POWER_CUTOFF);
}


void MoveForwardWithSpeeds(int left, int right) {
	motor[ML] = left;
	motor[MR] = right;
}

// Uses IR Sensors to make sure we're lined up left-to-right
// Uses a proximity sensor to know when to stop
void GuidedDriveForward () {
	int DEFAULT_MOTOR_SPEED = 100;
	int POWER_DIFF_THRESHOLD = 10;
	while(! ProximitySensorSaysStop ()) {
		int left_power = IRmax_sig(ir);
		int right_power = IRmax_sig(ir1);
		int left_motor_speed = DEFAULT_MOTOR_SPEED;
		int right_motor_speed = DEFAULT_MOTOR_SPEED;

		int power_diff = left_power - right_power;
		if(abs(power_diff) > POWER_DIFF_THRESHOLD) {
			left_motor_speed -= power_diff;
			right_motor_speed += power_diff;
		}

		MoveForwardWithSpeeds(left_motor_speed, right_motor_speed);
	}
}

void IRAutonomous () {
	peg_t column = FindTheColumnThatTheIRBeaconIsOn();
  DeadReckoningDriveForwardCM(InitialForward(column));
	//DeadReckoningDriveForwardCM(90);
	//TurnLeftThisManyDegrees(-45);
	//DeadReckoningDriveForward(SecondForward(column));
	//RaiseTheScissorToPeg1Level ();
	//GuidedDriveForward ();
	//DropScissorLift ();
	//DeadReckoningDriveForward(BackUpAmount(column));
	//TurnLeftThisManyDegrees(135.0);
	//DeadReckoningDriveForward(ThirdForward(column));
}

task main() {
	initializeRobot();
	waitForStart();
	IRAutonomous();
}
