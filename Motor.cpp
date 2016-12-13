// 
// 
// 
#include "Arduino.h"
#include "Motor.h"

Motor::Motor(char id,int dir1Pin, int dir2Pin, int speedPin)
{
	_dir1Pin = dir1Pin;
	_dir2Pin = dir2Pin;
	_speedPin = speedPin;
	_id = id;

	//Define L298N Dual H-Bridge Motor Controller Pins

	pinMode(_dir1Pin, OUTPUT);
	pinMode(_dir2Pin, OUTPUT);
	pinMode(_speedPin, OUTPUT);

}

void Motor::forward(int speed)
{
	comprint('F', speed);

	digitalWrite(_dir1Pin, LOW);
	digitalWrite(_dir2Pin, HIGH);
	analogWrite(_speedPin, speed);//Sets speed variable via PWM 
}

void Motor::backward(int speed)
{
	comprint('B', speed);
	
	digitalWrite(_dir1Pin, HIGH);
	digitalWrite(_dir2Pin, LOW);
	analogWrite(_speedPin, speed);//Sets speed variable via PWM 
}



void Motor::stop()
{
	comprint('X', 0);
	
	digitalWrite(_dir1Pin, LOW);
	digitalWrite(_dir2Pin, LOW);
	analogWrite(_speedPin, 0);//Sets speed variable via PWM 
}



void Motor::comprint(char c, int speed) {
	Serial.print(" -> Motor ");
	Serial.print(_id);
	Serial.print(" ");
	Serial.print(c);
	Serial.print(" ");
	Serial.println(speed);
}
