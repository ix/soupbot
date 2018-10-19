// soupbot drive.c usb 2018-09-18 hazel l <rose.hazel@protonmail.ch>

#include "drive.h"
#include "oi.c"

struct drive_PCtrl drv;
int motorPos = 1;

void drive_zeroEnc() {
	nMotorEncoder[leftDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[hDrive] = 0;
	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;
	SensorValue[hEnc] = 0;
}

task drive_init() {
	drv.kp = 3;
	drv._delay = 100;
	drv.deadzone = 10;
	drv.error = 0;

	drive_zeroEnc();
}

task basicDrive(){
	while(true){
		motor[leftDrive]  = drive_masterPwr;
		motor[rightDrive] = drive_slavePwr;
		motor[hDrive] = drive_hPwr;
	}
}

task drivePIDLoop(){
	while (true) {
		motor[leftDrive]  = oi_drive_getPower(false);
		motor[rightDrive] = oi_drive_getPower(true);
		motor[hDrive] = oi_drive_getPower(true, true);

		if (!(vexRT[Ch4] > drv.deadzone || vexRT[Ch4] < -drv.deadzone)) { // don't even bother if we're turning
			drv.error = nMotorEncoder[leftDrive] - nMotorEncoder[rightDrive];

			drive_slavePwr += drv.error / drv.kp; // TODO: don't access this explicitly

			wait1Msec(drv._delay);
	  }

	  drive_zeroEnc();
	}
}

task drivePIDPos(){
	SensorValue[rightEnc] = 1;
	while (true) {
		drive_slavePwr = -127 * (((SensorValue[rightEnc] - motorPos) / motorPos));
		wait1Msec(drv._delay);
	}
}
