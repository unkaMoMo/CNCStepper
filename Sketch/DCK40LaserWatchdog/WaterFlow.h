
/*
  This file is part of CNCLib - A library for stepper motors.

  Copyright (c) 2013-2018 Herbert Aitenbichler

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

////////////////////////////////////////////////////////////

#include <avr\interrupt.h>

#define SAMPELCOUNT 16
#define SAMPELTIME  200

class WaterFlow
{
public:
	WaterFlow()
	{
		for (uint8_t i = 0; i < SAMPELCOUNT; i++)
		{
			_count[i] = 0;
			_countTime[i] = 0;
		}
	}

	void Init(uint8_t pin)
	{
		_countIdx = SAMPELCOUNT - 1;
		_countTime[_countIdx] = millis();
		Next();

		_ISRInstance = this;
		pinMode(pin, INPUT);
		attachInterrupt(digitalPinToInterrupt(pin), StaticISRPinRising, RISING);
	}

	unsigned int AvgCount(unsigned int timediff = 1000)
	{
		{
			uint8_t s = SREG;
			cli();					//  DisableInterrups();
			TestSampleTime();
			SREG = s;
		}

		// no disable interrupts => cache

		unsigned char countIdx = _countIdx;
		unsigned long sumtime = 0;
		unsigned int  sumcount = 0;

		for (unsigned char diff = 1; diff < SAMPELCOUNT && sumtime < timediff; diff++)
		{
			unsigned char idx = PrevIndex(countIdx, diff);

			sumtime += _countTime[idx];
			sumcount += _count[idx];
		}

		if (sumtime <= SAMPELTIME)
			return 0;

		return ScaleCount(sumcount, sumtime, timediff);
	}

private:

	static WaterFlow* _ISRInstance;

	static unsigned int ScaleCount(unsigned int count, unsigned long totaltime, unsigned long scaletotime)
	{
		return (unsigned long)count * scaletotime / totaltime;
	}

	static unsigned char NextIndex(unsigned char idx, unsigned char count)
	{
		return (unsigned char)((idx + count)) % (SAMPELCOUNT);
	}

	static unsigned char PrevIndex(unsigned char idx, unsigned char count)
	{
		return (idx >= count) ? idx - count : (SAMPELCOUNT)-(count - idx);
	}

	void TestSampleTime()
	{
		if (millis() > _countUntil)
			Next();
	}

	static void StaticISRPinRising()
	{
		_ISRInstance->ISRPinRising();
	}

	void ISRPinRising()
	{
		TestSampleTime();
		_count[_countIdx]++;
	}

	void Next()
	{
		// prev
		_countTime[_countIdx] = millis() - _countTime[_countIdx];

		// set next (no cli => cache)
		uint8_t idxnext = (_countIdx + 1) % SAMPELCOUNT;
		_count[idxnext] = 0;
		_countTime[idxnext] = millis();
		_countUntil = _countTime[idxnext] + SAMPELTIME;
		_countIdx = idxnext;
	}

	unsigned long _countUntil;
	volatile unsigned char _countIdx = 0;
	volatile int _count[SAMPELCOUNT];
	unsigned long _countTime[SAMPELCOUNT];
};

////////////////////////////////////////////////////////////

