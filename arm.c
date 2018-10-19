// soupbot arm.c usb 2018-09-26 hazel l  <rose.hazel@protonmail.ch>

#include "lib/PidLib.c"
#include "oi.c"

pidController *arm_main_pid;
pidController *arm_opp_pid;

task arm_control() {
	while (true) {
		if (arm_main_pid != NULL) {
			// update pid controller
			PidControllerUpdate(arm_main_pid);

			// drive the motor
			motor[armMain] = arm_main_pid->drive_cmd;
		}
		if (arm_opp_pid != NULL) {
			// update pid controller
			PidControllerUpdate(arm_opp_pid);

			// drive the motor
			motor[armOpp] = arm_opp_pid->drive_cmd;
		}
		wait1Msec(25);
	}
}

void arm_setpos(short pos) {
	if (arm_main_pid != NULL) {
		arm_main_pid->target_value = pos;
		arm_opp_pid->target_value = pos;
	}
}

short arm_waitpos(short timeout = 3000) {
	// default 3s
	short count = timeout;

	while (count >= 0) {
		wait1Msec(50); // nobody knows why this works. but if we remove it, it stops working
		               // therefore, here it will stay
		// TODO; don't know if this works
		if ((abs(arm_main_pid->error) < 50) && (abs(arm_opp_pid->error) < 50)) {
			return 1;
		}

		count -= 50;
	}

	return 0;
}

task arm_init() {
	// Kp, Ki, Kd
	arm_main_pid = PidControllerInit(0.004, 0.0, 0.01, armEnc, 0);
	arm_opp_pid = PidControllerInit(0.004, 0.0, 0.01, armOppEnc, 0);

	startTask(arm_control);

	while (true) {
		arm_setpos(oi_arm_getPosition());
	}
}
