//
// Autonomous_Base.h
//
// Copyright (c) 2012 Suit Bots
//

#include "hitechnic-irseeker-v2.h"

void initializeRobot(char DrivL, char DrivR, char HndL, char HndR, char scsrL, char scsrR) {
	nMotorEncoder[HndL] = 0;
	nMotorEncoder[HndR] = 0;
	nMotorEncoder[DrivL] = 0;
	nMotorEncoder[DrivR] = 0;
	nMotorEncoder[scsrL] = 0;
	nMotorEncoder[scsrR] = 0;
}

void rememberHandPosition (char hand) {
	long hand_position = nMotorEncoder[hand];
	AddToDatalog(0, hand_position);
}

//int irDirection (char sensor) {
//	int _dirAC = 0;
//	int acS1, acS2, acS3, acS4, acS5 = 0;


//	tHTIRS2DSPMode _mode = DSP_1200;

//	if (HTIRS2setDSPMode(sensor, _mode) == 0) {

//		eraseDisplay();
//		nxtDisplayCenteredTextLine(0, "ERROR!");
//		nxtDisplayCenteredTextLine(2, "Init failed!");
//		nxtDisplayCenteredTextLine(3, "Connect sensor properly");


//		PlaySound(soundBeepBeep);

//		wait10Msec(300);
//		return 0;
//	}

//	_dirAC = HTIRS2readACDir(sensor);

//	if (_dirAC < 0) {
//		writeDebugStreamLine("Read dir ERROR!");
//		return 0;
//	}

//	if (!HTIRS2readAllACStrength(sensor, acS1, acS2, acS3, acS4, acS5 )) {
//		writeDebugStreamLine("Read sig ERROR!");
//		return 0;
//	}
//	return _dirAC;
//}

//int reversed_ir_sensor(int sensorVal) {
//	int x = sensorVal * -1 + 10;
//	return x;
//}

typedef enum {left = 0, middle, right} peg_t;

// Dear Hunter,
//  This isn't that complicated.  I'll explain it to you if you ask.
// for now, I can give you an example that might clear it up:
//
//   typedef enum {true = 0, false} bool;
//
//  Sincerely,
//   Fletcher
//

void forward(short power, long distance, bool direction, char DrivL, char DrivR) {  //direction should be True for forward and False for backward
	int directional_power = (-2 * direction + 1) * power;

	while ((nMotorEncoder[DrivL] < distance
		      && nMotorEncoder[DrivR] < distance)
	       || (nMotorEncoder[DrivL] > distance * -1
	        && nMotorEncoder[DrivR] > distance * -1))  {
		motor[DrivL] = directional_power;
		motor[DrivR] = directional_power;
	}
}

void swingTurn(short power, int degree, bool direction, char DrivL, char DrivR) {  //True for left, False for right
	float rotations_ratio = degree / 360;  //This ratio will need to be adjusted

	motor[DrivL] = (! direction) * power;
	motor[DrivR] = direction * power;

	nMotorEncoderTarget[DrivL] = rotations_ratio;
	nMotorEncoderTarget[DrivR] = rotations_ratio;

}

void pivotTurn(short power, int degree, bool direction, char DrivL, char DrivR) { //True for left, False for right
	float rotations_ratio = degree / 360;  //This ratio will need to be adjusted
	int realPower = power;

	if (direction) {
		realPower += 0;
	}
	else {
		realPower *= -1;
	}

	motor[DrivL] = realPower;
	motor[DrivR] = realPower * -1;
	if (direction) {
		nMotorEncoderTarget[DrivL] = rotations_ratio;
		nMotorEncoderTarget[DrivR] = rotations_ratio;
	}
}

int IRSensorRegion (char sensorName, bool reversed) {  // include these commented lines when applied in the code
	int _dirAC = 0;
	//int acS1, acS2, acS3, acS4, acS5 = 0;
	//int maxSig = 0;    // the max signal strength from the seeker.
	//int val = 0;       // the translated directional value.

	//tHTIRS2DSPMode _mode = DSP_1200;


	// read the current modulated signal direction
	_dirAC = HTIRS2readACDir(sensorName);
	if (_dirAC < 0)
	{
		// error! - write to debug stream and then break.
		writeDebugStreamLine("Read dir ERROR!");

	}
	if (reversed) {
		_dirAC = _dirAC * -1 + 10;
	}
	return _dirAC;
}

peg_t dondePeg(char sensor1, char sensor2) {
	// This assumes the edge of the robot is 21" from the edge of field, like this
	//
  // |                   +-----+         +---------+
	// | |---   21"   ---| |Robot|         |Dispenser|
	// +-------------------+-----+---~ ~---+---------+--
	//

	peg_t peg;

	int sensor1state = IRSensorRegion(sensor1, false);
	int sensor2state = IRSensorRegion(sensor2, false);
	if (sensor1state == 5
		  && sensor2state == 5) {
		peg = right;
	}
	if (sensor1state == 6
		  && sensor2state == 6) {
		peg = middle;
	}
	if (sensor1state == 7
		  && sensor2state == 7) {
		peg = left;
	}
	else peg = middle;

	return peg;
}



#define COLOR_BLACK 1    //This, Hunter, is crazy stuff
#define COLOR_WHITE 2    //PS: I didn't wright it
#define COLOR_NEITHER 4  //PSS: I don't understand it fully

/*int black_white_colortrans(char color_sensor) {
static int color_state = COLOR_NEITHER;
int sColor;
while (true) {
int prev_color = color_state;
switch (SensorValue[color_sensor])
{
case 0:
sColor = "Black";
color_state = COLOR_BLACK;
break;
case 17:
sColor = "White";
color_state = COLOR_WHITE;
if (COLOR_BLACK == prev_color) {
return 1;
}
break;
default:
sColor = "n/a";
color_state = COLOR_NEITHER;
break;
}
}
}

int donde_IRBeacon () {

}*/
