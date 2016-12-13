// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Motor.h"


class Motor{
public:
	Motor(char id, int dir1Pin, int dir2Pin, int speedPin);
	void forward(int speed);
	void backward(int speed);
	void stop();
private:
	char _id;
	int _speed;
	int _dir1Pin;
	int _dir2Pin;
	int _speedPin;
	void comprint(char c, int speed);

};

#endif

