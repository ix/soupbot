// soupbot drive.h usb 2018-09-24 hazel l  <rose.hazel@protonmail.ch>
//                                skyler c <kilerskyman@gmail.com>

#ifndef __DRIVE_CONST_H
#define __DRIVE_CONST_H

// concurrent tasks don't seem to have function definitions
void drive_zeroEnc();
void drive_setSpeed(int left, int right, int hulkhogan);

typedef struct {
	int kp;
	int _delay;
	int deadzone;

	int error;
} drive_PCtrl;

#endif /* __DRIVE_CONST_H */
