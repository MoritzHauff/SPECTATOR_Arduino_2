/*
	SharpIR

	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK

	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico

	https://github.com/guillaume-rico/SharpIR

*/

#ifndef SharpIR_h
#define SharpIR_h

#define NB_SAMPLE 10

#ifdef ARDUINO
  #include "Arduino.h"
#elif defined(SPARK)
  #include "Particle.h"
#endif

#include "MedianFilter.h"

class SharpIR
{
private:

	MedianFilterClass median = MedianFilterClass();

	uint8_t _ir_Pin;
	uint8_t _numberOfValues;

public:

	SharpIR (uint8_t ir_Pin, uint8_t NumberOfValues);
    
	/*Gibt den median-Wert der letzten Updates zur�ck.*/
	int GetValue();

	/*F�hrt die angegebene Anzahl an Messungen durch und gibt dann den berechneten Wert zur�ck. Zeitkritisch!*/
	int CalculateValue();

	/*F�hrt eine einzelne Messung durch.*/
	void Update();


};

#endif
