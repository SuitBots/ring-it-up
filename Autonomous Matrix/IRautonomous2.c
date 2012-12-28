#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
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
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "Autonomous_Base.h"

/// @returns 0 for the left column, 1 for middle, 2 for right
int FindTheColumnThatTheIRBeaconIsOn() {
	// TODO
	return 0;
}

void DeadReckoningDriveForward(long amount) {
	// TODO
}

/// @returns the amount we should drive forward in phase 1 for each column
long Forward1Amount(int column) {
	// TODO
	return 100;
}

/// @returns the amount we should back up from our column to get to the dispensor
long BackUpAmount (int column) {
	// TODO
	return -100;
}

/// @returns the amount we should drive forward in phase 2 for each column
long Forward2Amount(int column) {
	// TODO
	return 100;
}


/// @returns the amount we should drive forward to address the column
long Forward3Amount(int column) {
	// TODO
	return 100;
}

void TurnLeftThisManyDegrees (float degrees) {
	// TODO
}

// Raises the scissor lift to the correct level for peg 1
// Also tilts the hand forward
void RaiseTheScissorToPeg1Level () {
	// TODO
}

void DropScissorLift () {
	// TODO
}

/// @returns the power reading from the left IR sensor
int LeftIRPower () {
	// TODO
	return 100;
}

/// @returns the power reading from the right IR sensor
int RightIRPower () {
	// TODO
	return 100;
}

bool ApproximiateProximitySensorUsingIRSensors () {
	int PROXIMITY_POWER_CUTOFF = 60;
	int left_power = LeftIRPower ();
	int right_power = RightIRPower ();

	return left_power == 0
		|| right_power == 0
		|| (left_power < PROXIMITY_POWER_CUTOFF
		   && right_power < PROXIMITY_POWER_CUTOFF);
}

// Returns true when we're close enough to the peg to stop
bool ProximitySensorSaysStop () {
	// TODO for an actual proximity sensor
	return ApproximiateProximitySensorUsingIRSensors ();
}


void MoveForwardWithSpeeds(int left, int right) {

}

// Uses IR Sensors to make sure we're lined up left-to-right
// Uses a proximity sensor to know when to stop
void GuidedDriveForward () {
	int DEFAULT_MOTOR_SPEED = 100;
	int POWER_DIFF_THRESHOLD = 10;
	while(! ProximitySensorSaysStop ()) {
		int left_power = LeftIRPower ();
		int right_power = RightIRPower ();
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
	int column = FindTheColumnThatTheIRBeaconIsOn();
	DeadReckoningDriveForward(Forward1Amount(column));
	TurnLeftThisManyDegrees(45.0);
	DeadReckoningDriveForward(Forward2Amount(column));
	RaiseTheScissorToPeg1Level ();
	GuidedDriveForward ();
	DropScissorLift ();
	DeadReckoningDriveForward(BackUpAmount(column));
	TurnLeftThisManyDegrees(135.0);
	DeadReckoningDriveForward(Forward3Amount(column));
}

task main() {

}