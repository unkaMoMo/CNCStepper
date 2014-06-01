#pragma once

////////////////////////////////////////////////////////

#include "Stepper.h"
#include "Parser.h"
#include "Configuration.h"
#include "Lcd.h"

////////////////////////////////////////////////////////
// Control: read from USB (or File) and pass string to the Parser

class CControl : public CSingleton<CControl>
{
public:

	CControl();

	virtual void Run();				// run Controller => do not return
	virtual void Kill();			// stop everyting => Emergency Stop
	virtual void Resurrect();		// Call after Kill to restart again

	bool IsKilled()												{ return CStepper::GetInstance()->IsEmergencyStop(); }

	void Pause();					// pause for FileReadAndExecuteCommand
	void Continue();				// continue from pause

	bool IsPause()												{ return _pause; }

	void Delay(unsigned long ms);	// delay with idle processing

	//////////////////////////////////////////

	enum EIOTools
	{
		// Output
		Spindel,
		Coolant,
		ControllerFan,

		// input
		Probe				// Probe for tool lenght
	};

	virtual void IOControl(unsigned char tool, unsigned short level)	{ tool;level;};
	virtual unsigned short IOControl(unsigned char tool)				{ tool; return 0; };

	//////////////////////////////////////////

	virtual void GoToReference() {};							// Goto Refernce during Initialisation
	virtual void GoToReference(axis_t axis) { axis; };

	//////////////////////////////////////////

	void StartPrintFromSD()				{ _printFromSDFile = true; }
	void ClearPrintFromSD()				{ _printFromSDFile = false; }
	bool PrintFromSDRunnding()			{ return _printFromSDFile; }

	//////////////////////////////////////////

	const char* GetBuffer()				{ return _buffer; }

protected:

	bool SerialReadAndExecuteCommand();							// read from serial an execut command, return true if command pending (buffer not empty)
	void FileReadAndExecuteCommand(Stream* stream);				// read command until "IsEndOfCommandChar" and execute command (NOT Serial)

	virtual void Init();
	virtual void Initialized();									// called if Init() is done

	virtual void Parse() = 0;									// abstract: specify Parser
	virtual void Command(char* xbuffer);						// execute Command (call parser)
	virtual void Idle(unsigned int idletime);					// called after TIMEOUTCALLIDEL in idle state
	virtual bool IsEndOfCommandChar(char ch);					// override default End of command char, default \n \r
	virtual void ReadAndExecuteCommand();						// read and execute commands from other source e.g. SD.File

	virtual void TimerInterrupt();								// called from timer (timer0 on AVR) 

	void ParseAndPrintResult(CParser* parser);

	virtual bool OnStepperEvent(CStepper*stepper, EnumAsByte(CStepper::EStepperEvent) eventtype, unsigned char addinfo);

	bool IsResurrectCommand(const char*buffer)					{ return buffer[0] == '!' && buffer[1] == '!' && buffer[2] == '!' && buffer[3] == 0; }

private:

	void ReadAndExecuteCommand(Stream* stream, bool filestream);	// read command until "IsEndOfCommandChar" and execute command (Serial or SD.File)

	void CheckIdle();											// check idle time and call Idle every 100ms


	int				_bufferidx;									// read Buffer index

	unsigned long	_lasttime;									// time last char received
	unsigned long	_timeBlink;									// time to change blink state

	CStepper::StepperEvent	_oldStepperEvent;
	void*					_oldStepperEventParam;

	bool			_pause;										// see gcode m01 & m02
	bool			_printFromSDFile;

	char			_buffer[SERIALBUFFERSIZE];					// serial input buffer

	static void HandleInterrupt()								{ GetInstance()->TimerInterrupt(); }

	static bool MyStepperEvent(CStepper*stepper, void* param, EnumAsByte(CStepper::EStepperEvent) eventtype, unsigned char addinfo) { return ((CControl*)param)->OnStepperEvent(stepper, eventtype, addinfo); }

protected:

	CStreamReader		_reader;

};

////////////////////////////////////////////////////////