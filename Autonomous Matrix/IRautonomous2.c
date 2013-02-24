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

/// @returns 0 for the left column, 1 for middle, 2 for right
peg_t FindTheColumnThatTheIRBeaconIsOn() {
	peg_t peg = dondePeg(ir, ir1);
	return peg;
}

static const float DRIVE_SCALE_FACTOR = 68.8973;

void DeadReckoningDriveForwardCM(long amount) {
	bool direction = amount > 0;
	amount = abs(amount);
	forward(100, amount * DRIVE_SCALE_FACTOR, direction, ML, MR);
}

void TurnLeftThisManyDegrees (int degrees) {
	const float SCALE_FACTOR = 30;
	int abs_degrees = abs(degrees);
	swingTurn(100, abs_degrees * SCALE_FACTOR, degrees > 0, ML, MR);
}

void GoForward (const long cm) { DeadReckoningDriveForwardCM (cm); }
void GoBackwards (const long cm) { DeadReckoningDriveForwardCM (-cm); }
void TurnLeft (const int degrees) { TurnLeftThisManyDegrees (-degrees); }
void TurnRight (const int degrees) { TurnLeftThisManyDegrees (degrees); }

int AveragePower () {
	return (IRmax_sig (ir) + IRmax_sig(ir1)) / 2;
}

//typedef struct {
//	int power0;
//	int power1;
//}  powers_t;

//void GetPowers (powers_t *ret) {  // this function will get two data points.  For it we'll have to find the maximum speed of the robot
//	ret -> power0 = AveragePower();
//	int time_diff = 10; // this ought to equal the distance we want it to check divided by the speed of the robot

//	static bool good_to_stop = false;

//	wait10Msec(time_diff);

//	ret -> power1 = AveragePower();

//	int POWER_DIFF_THRESHOLD = 20;
//	if ((power0 - 20) > power1)
//		good_to_stop = true;
//}

// Returns true when we're close enough to the peg to stop
static int beginning_power_reading = 0;
bool ProximitySensorSaysStop (int *last_power, bool check_power) {
	int PROXIMITY_POWER_CUTOFF = beginning_power_reading / 4;

	// TODO: Re-enable the proximity stop.
	return false;

	int left_power = IRmax_sig(ir);
	int right_power = IRmax_sig(ir1);
	int cur_power = (left_power + right_power) / 2;

	if (check_power) {
		int POWER_DIFF_THRESHOLD = 7000;
		if (*last_power - POWER_DIFF_THRESHOLD > cur_power)
			return true;
  	*last_power = cur_power;
  }

	return left_power == 0
	|| right_power == 0
	|| (left_power < PROXIMITY_POWER_CUTOFF
	&& right_power < PROXIMITY_POWER_CUTOFF);
}

bool FailSafeTargetReached (long max_fwd) {
	int left = nMotorEncoder[ML];
	int right = nMotorEncoder[ML];
	float test_val = max_fwd * DRIVE_SCALE_FACTOR;
	return left > test_val && right > test_val;
}



// Uses IR Sensors to make sure we're lined up left-to-right
void GuidedDriveForward (long max_fwd) {
	nMotorEncoder[ML] = nMotorEncoder[MR] = 0;
	int DEFAULT_MOTOR_SPEED = 70;
	int POWER_DIFF_THRESHOLD = 4;
	int last_power = AveragePower ();
	int tenths_to_wait = 0;
	ClearTimer(T1);
	while(! (ProximitySensorSaysStop (&last_power, time100[T1] > tenths_to_wait) || FailSafeTargetReached (max_fwd)) ) {
		int left_power = IRmax_sig(ir) / 2;
		int right_power = IRmax_sig(ir1) / 2;
		int right_motor_speed = DEFAULT_MOTOR_SPEED;
		int left_motor_speed = DEFAULT_MOTOR_SPEED;

		int power_diff = left_power - right_power;
		if(abs(power_diff) > POWER_DIFF_THRESHOLD) {
			// Make sure that the far motor is always at DEFAULT_MOTOR_SPEED
			// and the slow motor is at DEFAULT_MOTOR_SPEED - power_diff
			left_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) - power_diff;
			right_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) + power_diff;
		}

		motor[ML] = -left_motor_speed;
		motor[MR] = -right_motor_speed;

		if (time100[T1] > tenths_to_wait)
			ClearTimer(T1);
	}
	motor[ML] = motor[MR] = 0;
}

void DropHand () {
	servo[hand_vertical] = 180;
}

void RaiseHand () {
	servo[hand_vertical] = 10;
	nSyncedMotors = synchAB;
	//while (nMotorEncoder[HandL] < 50) {
		//motor[HandL] = 75;
	//}
}

static bool USE_GUIDED_FWD = false;

void DriveToPegLeft ()
{ GoForward (125);
	TurnRight (57);
	if (USE_GUIDED_FWD)
		GuidedDriveForward (320);
	else
		GoForward (60);
	DropHand ();
	GoBackwards (10);
}

void DriveToPegMiddle ()
{ GoForward (40);
	TurnRight (57);
	if (USE_GUIDED_FWD) {
		GoForward (73);
		GuidedDriveForward (42);
	} else
		GoForward (115);

	DropHand ();
	GoBackwards (10);
}

void DriveToPegRight ()
{ GoForward (5);
	TurnRight (50);
	GoForward (34);
	TurnLeft (24);
	if (USE_GUIDED_FWD)
	{ GoForward (45);
		GuidedDriveForward (60);
	}
	else
		GoForward (105);
	DropHand ();
	GoBackwards (10.0);
}

void DriveToPeg (peg_t column)
{
	switch (column)
	{
		case LEFT:
			DriveToPegLeft();
			break;
		case MIDDLE:
			DriveToPegMiddle();
			break;
		case RIGHT:
			DriveToPegRight();
			break;
		default:
			DriveToPegMiddle();
			break;
	}

	//if (LEFT == column)
	//	DriveToPegLeft ();
	//else if (MIDDLE == column)
	//	DriveToPegMiddle ();
	//else if (RIGHT == column)
	//	DriveToPegRight ();
}

void DriveToDispensor (peg_t column, side_t color)
{ // todo
}

void IRAutonomous () {
	RaiseHand();
	beginning_power_reading = AveragePower ();
	peg_t column = FindTheColumnThatTheIRBeaconIsOn();
	DriveToPeg (column);
	DriveToDispensor (column, RED); // todo: replace red with the correct side
}

task main() {
	initializeRobot();
	waitForStart();
	IRAutonomous();
}
