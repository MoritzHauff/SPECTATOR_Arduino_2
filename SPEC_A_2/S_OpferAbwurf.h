// S_OpferAbwurf.h - Moritz Hauff - 04.05.2017

#ifndef _S_OPFERABWURF_h
#define _S_OPFERABWURF_h

///////////////////////////////////////////////////////////////////////////
///Includes
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "State.h"

///////////////////////////////////////////////////////////////////////////
///State-Class
/// <summary>
/// Dieser State überwacht den Abwurf von RescueKits.
/// </summary>
class S_OpferAbwurfClass : public StateClass
{
protected:
	bool dropped;

public:
	S_OpferAbwurfClass(SPECTATORClass *Spectator, const char Name[]) : StateClass(Spectator, Name)
	{ }

	void Init();
	void Sense();
	void Think();
	void Act();

	void ShiftTimers(unsigned long ShiftAmount);

	/// <summary>
	/// Gibt an ob rechts 'r' oder links 'l' ein Opfer abgeworfen werden soll.
	/// </summary>
	char RechtsLinks;
};


#endif
