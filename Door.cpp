#include "Door.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/**
constructor with left and right motor pin allocations.
*/
Door::Door(int in1, int in2, int en)
{
	_motor = new Motor('D', in1, in2, en);
}

Door::Door(int in1, int in2, int en, int openEndStopPin, int closedEndStopPin) : Door(in1, in2, en)
{
	_openEndStopPin = openEndStopPin;
	_closedEndStopPin = closedEndStopPin;



	// Momentary buttons that detect fully close and open door
	pinMode(_closedEndStopPin, INPUT);
	pinMode(_openEndStopPin, INPUT);
}


void Door::close(int speed)
{
	Serial.println("Door close");
	_motor->forward(speed);
	_state = CLOSING;

}

void Door::setMechControlButtons(int closeBtnPin, int openBtnPin) {
	_closeBtnPin = closeBtnPin;
	_openBtnPin = openBtnPin;

	pinMode(_closeBtnPin, INPUT);
	pinMode(_openBtnPin, INPUT);
}

void Door::setCurrentSensor(int pinAnalog)
{
	_currentSensor = new ACSCurrentSensor(pinAnalog);
}

void Door::open()
{
	open(255);
}

void Door::close()
{
	close(255);

}


void Door::open(int speed)
{
	Serial.println("Door open");
	_motor->backward(speed);
	_state = OPENING;
}


void Door::powerOff()
{
	_motor->stop();
}

char Door::state()
{
	return _state;
}

int Door::lastState()
{
	return _lastState;
}

boolean Door::stateChange(boolean reset)
{
	boolean changed = (_state != _lastState);

	if (reset) {
		_lastState = _state;
	}
	return changed;
}

boolean Door::isMoving() {
	return (_state != CLOSED && _state != OPEN && _state != STALLED_CLOSING && _state != STALLED_OPENING);
}

void Door::checkEndStops()
{

	// Check switches to see if door fully opened or closed.
	if (_state == CLOSING) {
		if (checkSwitch(_closedEndStopPin, 0) == HIGH) {

			Serial.println("Close button High");
			powerOff();
			_state = CLOSED;
		}
	}
	else if (_state == OPENING) {
		if (checkSwitch(_openEndStopPin, 1) == HIGH) {
			Serial.println("Open button high");
			powerOff();
			_state = OPEN;
		}
	}
}

void Door::checkMechanicalButtons() {
	if (checkSwitch(_openBtnPin, 2) == HIGH) {
		if (_state != CLOSING && _state!=CLOSED && _state!=CLOSE_ASSIST) {
			close();
		}
	}
	else if (checkSwitch(_closeBtnPin,3)==HIGH) {
		if (_state != OPENING && _state != OPEN && _state!=OPEN_ASSIST) {
			open();
		}
	}
}

void Door::checkForAutoAssist() {
	if (!isMoving()) {
		_currentSensor->task(true);

		if (!_currentSensor->isCurrentNominallyZero()) {
			//Serial.println("Current Threshold");
		}
		else {
			//open();
		}
	}
}

/**
* Returns if one of the buttons is high, or both are low.
*/
int Door::checkSwitch(int buttonPin, int i) {

	/*
	* Variables for the button switches
	* For recording the button doorStatus and debounce functions
	* in checkButtonState()
	*/

 // the last time the output pin was toggled

									  // read the doorStatus of the switch into a local variable:
	int reading = digitalRead(buttonPin);


	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != _lastButtonState[i]) {
		// reset the debouncing timer
		_lastDebounceTime[i] = millis();
	}

	if ((millis() - _lastDebounceTime[i]) > ENDSTOP_BUTTON_DEBOUNCE_DELAY) {
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current doorStatus:

		// if the button doorStatus has changed:
		if (reading != _buttonState[i]) {
			_buttonState[i] = reading;
		}
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	_lastButtonState[i] = reading;

	return _buttonState[i];
}
