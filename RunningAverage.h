// RunningAverage.h

//
//    FILE: RunningAverage.h
//  AUTHOR: Rob dot Tillaart at gmail dot com
// VERSION: 0.2.12
//    DATE: 2016-dec-01
// PURPOSE: RunningAverage library for Arduino
//     URL: http://arduino.cc/playground/Main/RunningAverage
// HISTORY: See RunningAverage.cpp
//
// Released to the public domain
//
// backwards compatibility
// clr()   clear()
// add(x)  addValue(x)
// avg()   getAverage()

#ifndef _RUNNINGAVERAGE_h
#define _RUNNINGAVERAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class RunningAverage
{
public:
	RunningAverage(const uint8_t size);

	void clear();
	void addValue(const double);
	void fillValue(const double, const uint8_t);

	double getAverage() const;      // does iterate over all elements.
	double getFastAverage() const;  // reuses previous values.

									// return statistical characteristics of the running average
	double getStandardDeviation() const;

	// returns min/max added to the data-set since last clear
	double getMin() const { return _min; };
	double getMax() const { return _max; };

	// returns min/max from the values in the internal buffer
	double GetMinInBuffer() const;
	double GetMaxInBuffer() const;

	// return true if buffer is full
	bool BufferIsFull() const;

	double getElement(uint8_t idx) const;

	uint8_t getSize() const { return _size; }
	uint8_t getCount() const { return _cnt; }



protected:
	uint8_t _size;
	uint8_t _cnt;
	uint8_t _idx;
	double _sum;
	double * _ar;
	double _min;
	double _max;
};


#endif

