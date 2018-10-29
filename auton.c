// soupbot auton.c usb 2018-10-05 hazel l <rose.hazel@protonmail.ch>

#define DRV_TICKS_TO_IN   27 // enc ticks / distance moved
#define DRV_H_TICKS_TO_IN 27
#define abs(X) ((X < 0) ? -1 * X : X)

#include "oi.c"
#include "drive.c"

// TODO: these two functions... don't work...
void auton_driveUntilEncReached(int ticks, int power) {
	drive_zeroEnc();
	drive_masterPwr = power;
	drive_slavePwr = power;
	while (abs(SensorValue[leftEnc]) < ticks) {}
	drive_masterPwr = 0;
	drive_slavePwr = 0;
}

void auton_driveHUntilEncReached(int ticks, int power) {
	drive_zeroEnc();

	stopTask(basicDrive);
	//stopTask(drivePIDLoop);
	//startTask(drivePIDPos);

	drive_masterPwr = 0;
	drive_slavePwr = 0;
	motor[hDrive] = power;

	while (abs(SensorValue[hEnc]) < ticks){}
	drive_hPwr = 0;

	//stopTask(drivePIDPos);
	startTask(basicDrive);
}

// left starting position, go forward, then right
task autonomous() {
	stopTask(drivePIDLoop);
	startTask(basicDrive);
	SensorValue[led] = 1;
	oi_armServ_setPos(120);
	wait1Msec(400);
	auton_driveUntilEncReached(100, 127);
	wait1Msec(400);
	oi_arm_setPosition(100);
	wait1Msec(700);
	auton_driveUntilEncReached(425, -127);
	wait1Msec(400);
	auton_driveHUntilEncReached(435, 127);
	wait1Msec(400);
	auton_driveUntilEncReached(220, -127);
	wait1Msec(400);
  oi_arm_setPosition(1270);
  wait1Msec(2000);
  oi_armServ_setPos(-60);
	SensorValue[led] = 0;
	stopTask(basicDrive);
	drive_masterPwr = 0;
	drive_slavePwr = 0;
	drive_hPwr = 0;
}
