////////////////////////////////////////////////////////
/*
  This file is part of CNCLib - A library for stepper motors.

  Copyright (c) 2013-2017 Herbert Aitenbichler

  CNCLib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  CNCLib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  http://www.gnu.org/licenses/
*/
////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////

#define STEPPERTYPE 6		// CStepperMash6050S

////////////////////////////////////////////////////////

#define USBBAUDRATE 115200

#define MYNUM_AXIS 3

////////////////////////////////////////////////////////

#define X_MAXSIZE 800000				// in mm1000_t
#define Y_MAXSIZE 500000 
#define Z_MAXSIZE 100000 
#define A_MAXSIZE 360000 
#define B_MAXSIZE 360000 
#define C_MAXSIZE 360000 

////////////////////////////////////////////////////////

// PIN AS Ramps 1.4 
#define STEPSPERROTATION	200
#define MICROSTEPPING		16
#define SCREWLEAD			5.0

#define X_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)
#define Y_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)
#define Z_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)
#define A_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)
#define B_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)
#define C_STEPSPERMM ((STEPSPERROTATION*MICROSTEPPING)/SCREWLEAD)

#define ConversionToMm1000 CMotionControlBase::ToMm1000_5_3200
#define ConversionToMachine CMotionControlBase::ToMachine_5_3200

////////////////////////////////////////////////////////

#define CNC_MAXSPEED 30000        // steps/sec
#define CNC_ACC  350
#define CNC_DEC  400

////////////////////////////////////////////////////////
// NoReference, ReferenceToMin, ReferenceToMax

#define X_USEREFERENCE	EReverenceType::ReferenceToMin
#define Y_USEREFERENCE	EReverenceType::ReferenceToMin
#define Z_USEREFERENCE	EReverenceType::ReferenceToMax
#define A_USEREFERENCE	EReverenceType::NoReference
#define B_USEREFERENCE  EReverenceType::NoReference
#define C_USEREFERENCE  EReverenceType::NoReference

#define REFMOVE_1_AXIS  Z_AXIS
#define REFMOVE_2_AXIS  Y_AXIS
#define REFMOVE_3_AXIS  X_AXIS
#define REFMOVE_4_AXIS  255
#define REFMOVE_5_AXIS  255
#define REFMOVE_6_AXIS  255

////////////////////////////////////////////////////////

#if STEPPERTYPE==6
#include "Configuration_Mash6050S.h"
#endif

////////////////////////////////////////////////////////

#define GO_DEFAULT_STEPRATE		((steprate_t) CConfigEeprom::GetConfigU32(offsetof(CConfigEeprom::SCNCEeprom, maxsteprate)))	// steps/sec
#define G1_DEFAULT_STEPRATE		(CNC_MAXSPEED/2)			// steps/sec
#define G1_DEFAULT_MAXSTEPRATE	((steprate_t) CConfigEeprom::GetConfigU32(offsetof(CConfigEeprom::SCNCEeprom, maxsteprate)))	// steps/sec

////////////////////////////////////////////////////////

#if defined(__SAM3X8E__)
#define MESSAGE_MYCONTROL_Starting					F("KK1000S(HA) due is starting ... (" __DATE__ ", " __TIME__ ")")
#else
#define MESSAGE_MYCONTROL_Starting					F("KK1000S(HA) Mega is starting ... (" __DATE__ ", " __TIME__ ")")
#endif
