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
#pragma config(Servo,  srvo_S1_C3_5,    HandLR,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_6,    hand_vertical,        tServoStandard)

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

static const float DRIVE_SCALE_FACTOR = 68.8973;

void DeadReckoningDriveForwardCM(long amount) {
  bool direction = amount > 0;
  amount = abs(amount);
  forward(100, amount * DRIVE_SCALE_FACTOR, direction, ML, MR);
}

void TurnLeftThisManyDegrees (int degrees) {
  const float SCALE_FACTOR = 75;
  int abs_degrees = abs(degrees);
  swingTurn(100, abs_degrees * SCALE_FACTOR, degrees > 0, ML, MR);
}

void GoForward (const long cm) { DeadReckoningDriveForward (cm); }
void GoBackwards (const long cm) { DeadReckoningDriveForward (-cm); }
void TurnLeft (const int degrees) { TurnLeftThisManyDegrees (degrees); }
void TurnRight (const int degrees) { TurnLeftThisManyDegrees (-degrees); }

int AveragePower () {
  return (IRmax_sig (ir) + IRmax_sig(ir1)) / 2;
}

// Returns true when we're close enough to the peg to stop
static int beginning_power_reading = 0;
bool ProximitySensorSaysStop () {
  int PROXIMITY_POWER_CUTOFF = beginning_power_reading / 4;
  int left_power = IRmax_sig(ir);
  int right_power = IRmax_sig(ir1);

  return left_power == 0
    || right_power == 0
    || (left_power < PROXIMITY_POWER_CUTOFF
        && right_power < PROXIMITY_POWER_CUTOFF);
}

bool FailSafeTargetReached (long max_fwd) {
  float left = nMotorEncoder[DrivL];
  float right = nMotorEncoder[DrivL];
  float test_val = max_fwd * DRIVE_SCALE_FACTOR;
  return left > test_val && right > test_val;
}

// Uses IR Sensors to make sure we're lined up left-to-right
void GuidedDriveForward (long max_fwd) {
  nMotorEncoder[DrivL] = nMotorEncoder[DriveR] = 0;
  int DEFAULT_MOTOR_SPEED = 100;
  int POWER_DIFF_THRESHOLD = 10;
  while(! (ProximitySensorSaysStop () || FailSafeTargetReached (max_fwd)) ) {
    int left_power = IRmax_sig(ir);
    int right_power = IRmax_sig(ir1);
    int left_motor_speed = DEFAULT_MOTOR_SPEED;
    int right_motor_speed = DEFAULT_MOTOR_SPEED;

    int power_diff = left_power - right_power;
    if(abs(power_diff) > POWER_DIFF_THRESHOLD) {
      // Make sure that the far motor is always at DEFAULT_MOTOR_SPEED
      // and the slow motor is at DEFAULT_MOTOR_SPEED - power_diff
      left_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) - power_diff;
      right_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) + power_diff;
    }

    motor[ML] = left_motor_speed;
    motor[MR] = right_motor_speed;
  }
  motor[ML] = motor[MR] = 0;
}

void DropHand () {
  servo[hand_vertical] = 180.0;
}

static bool USE_GUIDED_FWD = false;

void DriveToPegLeft ()
{ GoForward (129);
  TurnRight (25);
  if (USE_GUIDED_FWD)
    GuidedDriveForward (60);
  else
    GoForward (60);
  DropHand ();
  GoBackwards (10);
}

void DriveToPegMiddle ()
{ GoForward (45);
  TurnRight (24);
  if (USE_GUIDED_FWD)
    { GoForward (55);
      GuidedDriveForward (60);
    }
  else
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
      GuidedDriveForwrard (60);
    }
  else
    GoForward (105);
  DropHand ();
  GoBackwards (10.0);
}

void DriveToPeg (peg_t column)
{ if (LEFT == column)
    DriveToPegLeft ();
  else if (MIDDLE == column)
    DriveToPegMiddle ();
  else if (RIGHT == column)
    DriveToPegRight ();
}

void DriveToDispensor (peg_t column)
{ // todo
}

void IRAutonomous () {
  beginning_power_reading = AveragePower ();
  peg_t column = FindTheColumnThatTheIRBeaconIsOn();
  DriveToPeg (column);
  DriveToDispensor (column);
}

task main() {
  initializeRobot();
  waitForStart();
  IRAutonomous();
}
