// soupbot oi.c usb 2018-09-12 hazel l <rose.hazel@protonmail.ch>

int drive_masterPwr = 0;
int drive_slavePwr = 0;
int drive_hPwr = 0;

int arm_position = 0; // initially horizontal, eh
bool arm_isUpdating = false;

int armServ_pos = 0;

task oi_arm_updatePos() {
	while (true) {
		if (vexRT[Btn5U] == 1) {
			arm_position++;
			arm_isUpdating = true;
		} else if ((vexRT[Btn5D] == 1) && (arm_position > 0)) { // no negative position, eh
			arm_position--;
			arm_isUpdating = true;
		} else {
			arm_isUpdating = false;
		}
		wait1Msec(100); // unsure if this is needed, it totally is needed, eh
	}
}

task oi_armServ_updatePos(){
	while(true){
		if ((vexRT[Btn6U] == 1) && (armServ_pos <= 127)){
			armServ_pos++;
		} else if ((vexRT[Btn6D] == 1) && (armServ_pos >= -127)){
			armServ_pos--;
		}
		wait1Msec(100);
	}
}

/* left = forward - turn
	 right = forward + turn
	 H = sideways */
task oi_vexNetMap() {
	while (true) {
		// DRIVE
		// left master, right slave
		drive_masterPwr = vexRT[Ch2] + vexRT[Ch4];
		drive_slavePwr = vexRT[Ch2] - vexRT[Ch4];
		drive_hPwr = vexRT[Ch1];

		// ARM
		startTask(oi_arm_updatePos);
		startTask(oi_armServ_updatePos);
	}
}

// false: left, true: right
// oi_getDrivePower(true, true) -> H
// this does not apply politically
int oi_drive_getPower(bool mtr, bool mtr_h = false) {
	if (mtr_h) {
		return drive_hPwr;
	} else if (mtr) {
		return drive_slavePwr;
	} else {
		return drive_masterPwr;
	}
	// NOTREACHED
	return 0;
}

void oi_drive_setPower(int value, bool mtr, bool mtr_h = false) {
	if (mtr_h) {
		drive_hPwr = value;
	} else if (mtr) {
		drive_slavePwr = value;
	} else {
		drive_masterPwr = value;
	}
}

// don't use this function, it, for whatever reason, applies an absolute value to your powers
// we don't know why, we can't figure it out, it just does this, so yeah
// or maybe it doesn't, I don't know, it just doesn't work with auton
void oi_drive_setDrivePwr(int value) {
	oi_drive_setPower(value, false);
	oi_drive_setPower(value, true);
}

int oi_arm_getPosition() {
	return arm_position;
}

void oi_arm_setPosition(int value) {
	arm_position = value;
}

bool oi_arm_isUpdating() {
	return arm_isUpdating;
}

int oi_armServ_getPos() {
	return armServ_pos;
}

void oi_armServ_setPos(int value) {
	armServ_pos = value;
}
