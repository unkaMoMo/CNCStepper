////////////////////////////////////////////////////////
/*
  This file is part of CNCLib - A library for stepper motors.

  Copyright (c) 2013-2014 Herbert Aitenbichler

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

//////////////////////////////////////////

class CMm1000
{
private:
	mm1000_t	_value;

public:

	CMm1000()	{ _value = 0; }
	CMm1000(mm1000_t v)	{ _value = v; }

	static char* ToString(mm1000_t v, char*tmp, unsigned char precision, unsigned char scale);	// right aligned
	static char* ToString(mm1000_t v, char*tmp, unsigned char scale);

	char*ToString(char*tmp, unsigned char precision, unsigned char scale)	{ return ToString(_value, tmp, precision, scale); } // right aligned
	char*ToString(char*tmp, unsigned char scale)							{ return ToString(_value, tmp, scale); }
};

////////////////////////////////////////////////////////

class CSDist
{
private:
	sdist_t	_value;

public:

	CSDist()	{ _value = 0; }
	CSDist(sdist_t v)	{ _value = v; }

	static char* ToString(sdist_t v, char*tmp, unsigned char precision);		// right aligned
};

//////////////////////////////////////////