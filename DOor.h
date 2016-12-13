#ifndef _DOOR_h
#define _DOOR_h


// the debounce time for endstop buttons; increase if the output flickers for switches


// door doorStatuses
// assigned to 'doorstatus' variable
#define DOOR_PROTOCOL_SYNC (char)'d'
#define CLOSING (char)'0'
#define CLOSE_ASSIST (char)'1'
#define CLOSED (char)'2'
#define OPENING (char)'4'
#define OPEN_ASSIST (char)'5'
#define OPEN (char)'6'
#define STALLED_OPENING (char)'7'
#define STALLED_CLOSING (char)'8'
#define REPEAT_LAST (char)'9'

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Motor.h"
#include "ACSCurrentSensor.h"

class Door
{
public:

	// construcutor with (3) motor pin allocations.
	Door(int in1, int in2, int ens);
	// construcutor with (3) motor pin allocations & open/ close switch pins & current sensor pin
	Door(int in1, int in2, int en, int openEndStopPin, int closedEndStopPin);
	void open(int speed);
	void close(int speed);
	void setMechControlButtons(int closeBtnPin, int openBtnPin);
	void setCurrentSensor(int pinAnalog);
	void open();
	void close();
	void powerOff();
	char state();
	int lastState();
	boolean stateChange(boolean reset);
	void checkEndStops();
	void checkMechanicalButtons();
	void checkForAutoAssist();
	boolean isMoving();
private:
	#define ENDSTOP_BUTTON_DEBOUNCE_DELAY (long)100
	#define AUTOASSIST_CURRENT_THRESHOLD (double)100.05

	Motor* _motor;
	ACSCurrentSensor* _currentSensor;

	int _openEndStopPin;
	int _closedEndStopPin;

	int _closeBtnPin = -1;
	int	_openBtnPin = -1;


	int _buttonState[4];             // the current reading from the input pin
	int _lastButtonState[4];   // the previous reading from the input pin

									 // the following variables are long's because the time, measured in miliseconds,
									 // will quickly become a bigger number than can be stored in an int.
	long _lastDebounceTime[4];


	char _state;
	int _lastState;

	int checkSwitch(int buttonPin, int i);
	



};


#endif
