// Functions.h - Moritz Hauff - 17.02.2017
// see Functions.c

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

#ifndef _FUNCTIONS_h
#define _FUNCTIONS_h

///////////////////////////////////////////////////////////////////////////
///Includes
#include <avr/io.h>

//Initialisiert den Analog-Digital-Wandler mit einer genaueren Einstallbarkeit als bei analogRead().
extern void init_adc(void);

//Etwas besser einstellbares analogRead().
//Bevor diese Funktion verwendet werden kann muss init_adc() aufgerufen werden.
extern int16_t get_adc(uint8_t channel);
// für weitere Informationen siehe: https://bennthomsen.wordpress.com/arduino/peripherals/analogue-input/

#endif

