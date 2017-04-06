/** State.cpp
***
*** todo
***
*** Moritz Hauff, 17.03.2017
**/

///////////////////////////////////////////////////////////////////////////
///Includes
#include "State.h"

///////////////////////////////////////////////////////////////////////////
///Konstruktoren
StateClass::StateClass(SPECTATORClass *Spectator, const char Name[])
{
	spectator = Spectator;
	
	name = String(Name);

	status = Running;
}

///////////////////////////////////////////////////////////////////////////
///Functions
String StateClass::GetName()
{
	return name;
}

StateStatus StateClass::GetStatus()
{
	return status;
}
