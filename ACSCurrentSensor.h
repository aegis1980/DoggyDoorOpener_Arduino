// CurrentSensor.h

#ifndef _ACSCURRENTSENSOR_h
#define _ACSCURRENTSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Filters.h"
#include "RunningAverage.h"

class ACSCurrentSensor {
public:
	ACSCurrentSensor(int pinAnalog);
	void task(boolean updateCorrection);
	double getVoltage();
	double getCurrent();
	boolean isCurrentNominallyZero();
private:
	int _pinAnalog;
	FilterOnePole _currentLowPassFilter = FilterOnePole(LOWPASS,2.0);
	RunningAverage _correctionStats = RunningAverage(1000);

	// Track time in milliseconds since last reading
	unsigned long  _previousMillis = 0;
	unsigned long _updatePeriod = 500;  // in milliseconds
	double _voltage;
	double _mamps;

	uint32_t _samples = 0;

	// Use 66 for 30A ACS712 module, 100 for 20A one & 185 for 5A one. 
	const double MVOLTS_PER_AMP = 66;
	const double ACS_OFFSET = 2500;
};


#endif

