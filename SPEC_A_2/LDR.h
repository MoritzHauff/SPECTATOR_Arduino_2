// LDR.h - Moritz Hauff - 21.04.2017

#ifndef LDR_h
#define LDR_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////
///SPECMotorenClass
/*Verwaltet einen Helligkeitssensor.*/
class LDR
{
private:
	uint8_t _LDR_Pin;
	int _lastValue;

public:

	LDR(uint8_t LDR_Pin);
	
	/*Setzt die richtigen PinModes und erm�glicht somit eine Messung.*/
	void Init();

	/*Gibt den aktuellen Wert des LDR zur�ck.*/
	int GetValue();

	/*F�hrt eine einzelne Messung durch.*/
	void Update();


};

#endif
