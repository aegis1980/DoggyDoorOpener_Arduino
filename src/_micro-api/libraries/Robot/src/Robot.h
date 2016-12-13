// Robot.h

#ifndef _ROBOT_h
#define _ROBOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class Robot
{
public:
	// construcutor with left and right motor pin allocations.
	Robot(int in1L, int in2L, int enL, int in1R, int in2R, int enR);
private:
	//Motor _motorLeft;
//	Motor _motorRight;
};


#endif

