/** S_Sense.cpp
***
*** Dieser State erfasst nach jeder Aktion das aktuelle Feld und
*** beendet sich nach 5 Ticks wieder.
***
*** Moritz Hauff, 21.04.2017
**/

///////////////////////////////////////////////////////////////////////////
/// Copyright (C) {2017}  {Moritz Hauff}
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// If you start using parts of this code please write a short message to: 
/// license@vierradroboter.de
///
/// If you have any questions contact me via mail: admin@vierradroboter.de
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///Includes
#include "S_Idle.h"

void S_IdleClass::Init()
{
	// Variablen zurücksetzen.
	status = Running;
	counter = 0;

	spectator->serialBuffer.AddMsg(C_WandVorne, spectator->GetWand(R_VORNE));
	spectator->serialBuffer.AddMsg(C_WandRechts, spectator->GetWand(R_RECHTS));
	spectator->serialBuffer.AddMsg(C_WandHinten, spectator->GetWand(R_HINTEN));
	spectator->serialBuffer.AddMsg(C_WandLinks, spectator->GetWand(R_LINKS));
	spectator->serialBuffer.AddMsg(C_OpferLinks, spectator->OpferKontroller.OpferLinks());
	spectator->serialBuffer.AddMsg(C_OpferVorne, spectator->OpferKontroller.OpferVorne());
	spectator->serialBuffer.AddMsg(C_OpferRechts, spectator->OpferKontroller.OpferRechts());
	spectator->serialBuffer.Flush();

	spectator->serialBuffer.AddMsg(C_AktuelleRichtung, spectator->AktRichtung);
	spectator->serialBuffer.AddMsg(C_GeradeRampeBefahren, spectator->GeradeRampeBefahren);
	spectator->serialBuffer.AddMsg(C_GeradeSchwarzesFeldBefahren, spectator->GeradeSchwarzesFeldBefahren);
	spectator->serialBuffer.AddMsg(C_GeradeSonstWieNichtVorangekommen, spectator->GeradeSonstWieNichtVorangekommen);
	spectator->serialBuffer.Flush();

	Serial.println("DemandNextStep");

	// Resete die Werte
	spectator->GeradeRampeBefahren = false;
	spectator->GeradeSchwarzesFeldBefahren = false;
	spectator->GeradeSonstWieNichtVorangekommen = false;
	//spectator->OpferKontroller.Reset();  // only reset when your really move to another field.
}

///////////////////////////////////////////////////////////////////////////
///Funktionen
void S_IdleClass::Sense()
{
	//spectator->UpdateLaser();

	spectator->UpdateMPU();

	spectator->UpdateLDR();

	//spectator->serialBuffer.Flush();
	spectator->serialBuffer.Clear();
}

void S_IdleClass::Think()
{
	counter++;
	if (counter > S_Idle_NumberOfTicks)
	{
		status = Finished;
	}
}

void S_IdleClass::Act()
{
	spectator->Motoren.SetMotoren(0, 0);  // Stop any movement.
}

void S_IdleClass::ShiftTimers(unsigned long ShiftAmount)
{
	// do nothing.
}
