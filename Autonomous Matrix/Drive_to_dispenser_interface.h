//
// Drive_to_dispenser_interface.h
//
// Copyright (c) 2012 Suit Bots 4628
//

#include "Autonomous_Base.h"

void initialize_screen() { eraseDisplay(); }

// Draw the words "Red" and "Blue"
void draw_the_text() {
	int level = externalBattery;
	nxtDisplayCenteredBigTextLine(3, "Red     Blue");
	nxtDisplayCenteredTextLine(7, "%d", level);
}

// read events from the arrow NXT buttons, increment or deincrement values based on that
int read_the_buttons(int button_current_state) {
	int i = button_current_state;
	if (nNxtButtonPressed == 1)
		++i;
	else if (nNxtButtonPressed == 2)
		--i;
	return i;
}

void draw_the_square(int square_position) {
	int square_pos = square_position % 2;
	nxtDrawRect(20 * square_pos, 20 * square_pos, 40 * square_pos, 40 * square_pos); // todo: make it so that the square will go aroud the words
}

side_t determine_side() {
	if (read_the_buttons(i) == 0)
		return RED;
	else if (read_the_buttons(i) == 1)
		return BLUE;
	else return UNDETERMINED;
}

side_t select_side() {
	if (nNxtButtonPressed == 3)
		return determine_side;
	else return UNDETERMINED;
}

void autonomous_ready_screen() {
	initialize_screen();
	int level = externalBattery;
	nxtDisplayCenteredTextLine(1, "Autonomous Ready");
	nxtDisplayCenteredTextLine(2, "Waiting for start");
	nxtDisplayCenteredTextLine(4, "Side Selected:");
	if (determine_side() == RED) nxtDisplayCenteredTextLine(4, "RED");
	else if (determine_side() == BLUE) nxtDisplayCenteredTextLine(4, "BLUE");
	else nxtDisplayCenteredTextLine(4, "UNDETERMINED");
	nxtDisplayCenteredTextLine(7, "%d", level);
}

void autonomous_running_screen() {
	initialize_screen();
	int level = externalBattery;
	nxtDisplayCenteredTextLine(1, "Autonomous Running");
	nxtDisplayCenteredTextLine(4, "Side Selected:");
	if (determine_side() == RED) nxtDisplayCenteredTextLine(4, "RED");
	else if (determine_side() == BLUE) nxtDisplayCenteredTextLine(4, "BLUE");
	else nxtDisplayCenteredTextLine(4, "UNDETERMINED");
	nxtDisplayCenteredTextLine(7, "%d", level);
}

void teleOp_running_screen() {
	initialize_screen();
	int level = externalBattery;
	nxtDisplayCenteredTextLine(1, "TeleOp Running");
	nxtDisplayCenteredTextLine(7, "%d", level);
}
