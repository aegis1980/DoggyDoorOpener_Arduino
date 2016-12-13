// 
// 
// 

#include "Arduino.h"
#include "ACSCurrentSensor.h"
#include "Filters.h"
#include "RunningAverage.h"

ACSCurrentSensor::ACSCurrentSensor(int pinAnalog)
{
	_pinAnalog = pinAnalog;
}


void ACSCurrentSensor::task(boolean updateCorrection) {
	float rawValue = (float) analogRead(_pinAnalog);
	_currentLowPassFilter.input(rawValue);
	float uncorrectedMamps = 1000 * ((.0264 * _currentLowPassFilter.output()) - 13.51);
	_mamps = uncorrectedMamps -_correctionStats.getAverage();

	if (updateCorrection) {
		_correctionStats.addValue(uncorrectedMamps);
		if (_correctionStats.BufferIsFull()) {
			// Reset running average.
			double lastAvg  = _correctionStats.getAverage();
			_correctionStats.clear();
			// quarter fill buffer so get smooth reset
			_correctionStats.fillValue(lastAvg, _correctionStats.getSize() / 5);
		}
	}
	
	if ((unsigned long)(millis() - _previousMillis) >= _updatePeriod) {
		_previousMillis = millis();   // update time

									 // display current values to the screen
		//Serial.print("\n");
		// output sigma or variation values associated with the inputValue itsel
		//Serial.print("\tsigma: "); Serial.print(inputStats.sigma());
		// convert signal sigma value to current in amps
		
	//	Serial.print("\tamps: ");
	//	Serial.println(_mamps);
	}
	//Serial.println(rawValue);
	//_voltage = (rawValue / 1024.0) * 5000.0; // Gets you mV
	//_amps = ((_voltage - ACS_OFFSET) / MVOLTS_PER_AMP);
}

double ACSCurrentSensor::getVoltage()
{
	return _voltage;
}

double ACSCurrentSensor::getCurrent()
{
	//Serial.println(_mamps);
	return _mamps;

}

boolean ACSCurrentSensor::isCurrentNominallyZero() {
	double a = abs(_mamps);
	double b = 10.;
	//Serial.println(a);
	//Serial.println(b);
	if (a >= b) { // more that 2.5 sds from mean -> significent
		return false;
	} else {
		return true;
	}
}

