
#include <RunningStatistics.h>
#include <FloatDefine.h>
#include <FilterTwoPole.h>
#include <Filters.h>
#include <FilterOnePole.h>
#include <FilterDerivative.h>
#include "Motor.h"
#include "Door.h"
#include "RunningAverage.h"
#include <adk.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

// ADK Config
USB Usb;
ADK adk(&Usb, "floating_in_the_clouds", // Manufacturer Name
              "DoggyDoorOpener", // Model Name
              "Sketch for the USB Host Shield", // Description (user-visible string)
              "1.0", // Version
              "http://www.floatingintheclouds.com", // URL (web page to visit if no installed apps support the accessory)
              "123456789"); // Serial Number (optional)

uint32_t timer;
bool connected;

// Door definition
Door* door;


//door commands
// assigned to 'command' variable
#define STOP_DOOR 'm'
#define DOOR_MOTOR_OFF 't'
#define CLOSE_DOOR 'c'
#define OPEN_DOOR 'o'
#define STATUS 's'
#define REPEAT_LAST_COMMAND 'r'


// define MOTOR pins
const int IN1 = 10;
const int IN2 = 9;
const int ENA = 11;// Needs to be a PWM pin to be able to control motor spee
				 
// connect momentary buttons (both to one pin):
const int PIN_ENDSTOP_CLOSE = 8;
const int PIN_ENDSTOP_OPEN = 12;

const int PIN_MECHANICAL_CLOSE_BTN =22;
const int PIN_MECHANICAL_OPEN_BTN =24;

// current sensor into analog.
const int PIN_CURRENTSENSOR = A1;



//**************************************************************************
//[ START-OF MAIN ARDUINO METHODS]

void setup()
{

  // Serial Communication
  Serial.begin(57600);
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  
  if (Usb.Init() == -1) {
    Serial.print("\r\nOSCOKIRQ failed to assert");
    while (1); // halt
  }
 //Usb.Task();

	// create Door instance.
  Serial.println("Initilizing door,");
  door = new Door(IN1, IN2, ENA, PIN_ENDSTOP_CLOSE, PIN_ENDSTOP_OPEN);
  door->setMechControlButtons(PIN_MECHANICAL_CLOSE_BTN, PIN_MECHANICAL_OPEN_BTN);
  door->setCurrentSensor(PIN_CURRENTSENSOR);
  Serial.println("Door okay,");
  
}

void loop()
{

	
	boolean hasUsbControl = usbControl();

	// Check switches to see if door fully opened or closed.
	door->checkEndStops();

	//Check if mechnical open/close buttons have been pressed.
	door->checkMechanicalButtons();

	door->checkForAutoAssist();
	/**
	* If the door staus has changed, send it down usb.
	*/
	boolean doorStateChanged = door->stateChange(true);

	if (doorStateChanged) {
		Serial.print("USB Sending new door state: ");
		Serial.println(door->state());
		usbSendDoorState();
	}


} // end of MAIN LOOP
byte usbSendDoorState() {
	return usbSendDoorState(door->state());
}

byte usbSendDoorState(char c) {
	byte msg[2] = { DOOR_PROTOCOL_SYNC, c };
	byte rcode = adk.SndData(strlen((char*)msg), msg);
	if (rcode && rcode != hrNAK) {

	}
	else {

	}
	return rcode;
}

boolean usbControl() {
	//ADK Communication
	Usb.Task();
	if (adk.isReady()) {
		if (!connected) {
			connected = true;
			Serial.print(F("\r\nConnected to accessory"));
		}
		//  if (millis() - timer_comm >= 1000 / COMM_FREQ) {
		//   timer_comm = millis();

		byte msg[3]; // read data into msg variable
		uint16_t len = sizeof(msg);
		byte rcode = adk.RcvData(&len, msg);
		if (rcode && rcode != hrNAK) {
			Serial.print(F("\r\nData rcv: "));
			Serial.print(rcode, HEX);
		}
		else if (len > 0) {
			if (msg[0] == DOOR_PROTOCOL_SYNC) {
				switch (msg[1]) {
				case OPEN_DOOR:
					Serial.print(msg[2]);
					door->open(demodulate(msg[2]));
					break;
				case CLOSE_DOOR:
					Serial.print(msg[2]);
					door->close(demodulate(msg[2]));
					break;
				case STOP_DOOR:
				case DOOR_MOTOR_OFF:
					door->powerOff();
					break;
				case STATUS:
					usbSendDoorState();
					break;
				case REPEAT_LAST_COMMAND:
				default:
					usbSendDoorState(REPEAT_LAST);
					break;
				}
			}
			else {
				Serial.print("Unknown protocol");
			}
		}
		return true;

	}
	else { // end of adk ready
		if (connected) {
			connected = false;
			Serial.print(F("\r\nDisconnected from accessory"));
			//door->close(255);
		}
		return false;
	}// end of adk not-ready


}



float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int demodulate(byte b) {
	int i = (int)b + 128;
	return  constrain(i, 0, 255);
}
