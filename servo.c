// soupbot servo.c usb 2018-10-08 hazel l <rose.hazel@protonmail.ch>
//                                skillet <kilerskyman@gmail.com>

#include "oi.c"

task armServ_init(){
	while(true){
		motor[clawServ] = oi_armServ_getPos();
	}
}
