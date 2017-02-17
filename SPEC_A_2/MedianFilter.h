// MedianFilter.h - Moritz Hauff - 14.02.2017

#ifndef _MEDIANFILTER_h
#define _MEDIANFILTER_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define MAXARRAYLENGTH 20

///////////////////////////////////////////////////////////////////////////
///MedianFilterClass
/* Die MedianFilter-Class verwaltet Messwerte
und wendet auf diese einen eindimensionalen MedianFilter an.*/
class MedianFilterClass
{
protected:
	int TotalNumberOfValues;   // wie viele Werte insgesamt gespeichert werden.
	int measurements[MAXARRAYLENGTH];
	int currentpos;
	bool ready;

public:
	/* Initialize a KalmanFilter with standard properties, which need to be specified for satifying results.*/
	MedianFilterClass();
	/* Initialize the correct formulas calculating the measurement update.*/
	void Init(int TotalNumberOfValues);

	/* Add a new measurement.*/
	void Update(int measurement);

	/* Get the current value and delete the last measurments.*/
	int GetValue();

	/* Print current properties to serial terminal.*/
	void PrintProps();
};

#endif
