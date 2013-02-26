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

bool FailSafeTargetReached (long max_fwd) {
  int left = nMotorEncoder[ML];
  int right = nMotorEncoder[ML];
  float test_val = max_fwd * DRIVE_SCALE_FACTOR;
  return left > test_val && right > test_val;
}

// Uses IR Sensors to make sure we're lined up left-to-right
void GuidedDriveForward (long max_fwd) {
  nMotorEncoder[ML] = nMotorEncoder[MR] = 0;
  // TUNEABLE PARAMETERS
  const int DEFAULT_MOTOR_SPEED = 90;
  const int WAIT_BETWEEN_SAMPLES_MS = 20;
  // END TUNEABLE PARAMETERS

  while (! FailSafeTargetReached (max_fwd)) {
    int left_power = IRmax_sig(ir);
    int right_power = IRmax_sig(ir1);
    int right_motor_speed = DEFAULT_MOTOR_SPEED;
    int left_motor_speed = DEFAULT_MOTOR_SPEED;
    
    int power_diff = left_power - right_power;
    // Make sure that the far motor is always at DEFAULT_MOTOR_SPEED
    // and the slow motor is at DEFAULT_MOTOR_SPEED - power_diff
    left_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) - power_diff;
    right_motor_speed = DEFAULT_MOTOR_SPEED - abs(power_diff) + power_diff;
    
    motor[ML] = -left_motor_speed;
    motor[MR] = -right_motor_speed;
    wait1Msec (WAIT_BETWEEN_SAMPLES_MS);
  }
  motor[ML] = motor[MR] = 0;
}

void DropHand () {
  servo[hand_vertical] = 180;
}

void RaiseHand () {
  servo[hand_vertical] = 10;
  nSyncedMotors = synchAB;
}

void DriveToPegLeft (bool use_guided)
{ GoForward (125);
  TurnRight (57);
  if (use_guided)
    GuidedDriveForward (320);
  else
    GoForward (60);
}

void DriveToPegMiddle (bool use_guided)
{ GoForward (40);
  TurnRight (57);
  GoForward (73);
  if (use_guided)
    GuidedDriveForward (42);
  else
    GoForward (42);
}

void DriveToPegRight (bool use_guided)
{ GoForward (5);
  TurnRight (50);
  GoForward (34);
  TurnLeft (24);
  GoForward (45);
  if (use_guided)
    GuidedDriveForward (60);
  else
    GoForward (60);
}

void DriveToPeg (peg_t column)
{ bool middle_guided = false;
  switch (column) {
  case LEFT:
    DriveToPegLeft(true);
    break;
  case RIGHT:
    DriveToPegRight(true);
    break;
  case MIDDLE:
    middle_guided = true;
  default: // also: middle
    DriveToPegMiddle(middle_guided);
    break;
  }

}

void DriveToDispensor (peg_t column, side_t color)
{ // todo
}

void IRAutonomous () {
  RaiseHand();
  peg_t column = FindTheColumnThatTheIRBeaconIsOn();
  DriveToPeg (column);
  DropHand ();
  GoBackwards (10);
  DriveToDispensor (column, RED); // todo: replace red with the correct side
}

task main() {
  initializeRobot();
  waitForStart();
  IRAutonomous();
}
