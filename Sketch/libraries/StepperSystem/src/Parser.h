#pragma once

////////////////////////////////////////////////////////

#include "StreamReader.h"

////////////////////////////////////////////////////////

typedef float expr_t;

////////////////////////////////////////////////////////
//
// Base class for command parser/Scanner
//
class CParser
{
public:

	CParser(CStreamReader* reader)							{ _reader = reader; _error = NULL; _OkMessage = NULL; };

	virtual void Parse() = 0;

	bool IsError()											{ return _error != NULL; };
	const __FlashStringHelper * GetError()					{ return _error; }

	typedef void(*PrintOKMessage)(void);
	PrintOKMessage	GetOkMessage()							{ return _OkMessage; }

	CStreamReader* GetReader()								{ return _reader; }

protected:

	////////////////////////////////////////////////////////

	bool CheckError();
	virtual char SkipSpacesOrComment()						{ return _reader->SkipSpaces(); }
	void SkipCommentSingleLine()							{ _reader->MoveToEnd(); };

	bool ExpectEndOfCommand();
	void ErrorNotImplemented()								{ Error(MESSAGE_PARSER_NotImplemented); }
	void ErrorNotANumber()									{ Error(MESSAGE_PARSER_NotANumber); };
	void ErrorNumberRange()									{ Error(MESSAGE_PARSER_OverrunOfNumber); };

	////////////////////////////////////////////////////////

	void ErrorAdd(const __FlashStringHelper * error)		{ if (!IsError()) Error(error); }
	void Error(const __FlashStringHelper * error)			{ _error = error; _reader->MoveToEnd(); }
	void Info(const __FlashStringHelper* s1)				{ StepperSerial.print(MESSAGE_INFO);  StepperSerial.println(s1); }
	void Warning(const __FlashStringHelper* s1)				{ StepperSerial.print(MESSAGE_WARNING);  StepperSerial.println(s1); }

	CStreamReader*					_reader;
	const __FlashStringHelper *		_error;
	PrintOKMessage					_OkMessage;

	long GetInt32Scale(long minvalue, long maxvalue, unsigned char scale, unsigned char maxscale);	// get "float" e.g. 1.234 => 1234 or 12 => 12000, limit with scale
	expr_t GetDouble();

	unsigned char GetUInt8();
	unsigned short GetUInt16();
	unsigned long GetUInt32();
	char GetInt8();
	short GetInt16();
	long GetInt32();
	sdist_t GetSDist();

	static bool IsUInt(char ch)				{ return CStreamReader::IsDigit(ch); }
	static bool IsInt(char ch)				{ return CStreamReader::IsMinus(ch) || CStreamReader::IsDigit(ch); }

	//////////////////////////////////////////////////////
	// Textsearch  (=Tokens)

	// Content of "b": 
	//	  \001	=> any digit (last command char)
	//	  \002	=> space or end or not digit (last comand char), e.g. g0\002 will find g0 and not g01
	//	  \003	=> \000 (end of line)

	bool IsToken(const __FlashStringHelper * b, bool expectdel, bool ignorecase);
	bool TryToken(const __FlashStringHelper * b, bool expectdel, bool ignorecase)									{ return TryToken(_reader->GetBuffer(), b, expectdel, ignorecase); }
	bool TryToken(const char* buffer, const __FlashStringHelper * b, bool expectdel, bool ignorecase);	// scan from different location, but do not remove it

	bool TryToken(const char* buffer, const __FlashStringHelper * b, bool ignorecase);					// same as stricmp (with Progmem)	

	//////////////////////////////////////////////////////

private:

	static const char unsigned ConvertChar(const char ch, bool ignorecase)	{ return ignorecase ? CStreamReader::Toupper(ch) : ch; }

	bool TryToken(const char* buffer, const __FlashStringHelper * b, bool expectdel, bool ignorecase, const char*&nextchar);		// scan, but do not remove it

	////////////////////////////////////////////////////////

private:

	template<class T> T GetUInt()			{
		if (!CStreamReader::IsDigit(_reader->GetChar())) { ErrorNotANumber();	return 0; }
		T value = 0;
		while (CStreamReader::IsDigit(_reader->GetChar()))
		{
			T old = value;
			value *= (T)10;
			value += _reader->GetChar() - '0';
			if (old > value)	{ ErrorNumberRange();	return 0; }
			_reader->GetNextChar();
		}
		return value;
	}
	template<class T> T GetInt()			{
		bool negativ;
		if ((negativ = CStreamReader::IsMinus(_reader->GetChar()))!=0)
			_reader->GetNextChar();
		T value = (T)GetUInt<T>();
		return negativ ? -value : value;
	}

	template<class T> T GetUInt(T minvalue, T maxvalue)	{
		T value = GetUInt<T>();
		if (value < minvalue)		Error(MESSAGE_PARSER_ValueLessThanMin);
		else if (value > maxvalue)	Error(MESSAGE_PARSER_ValueGreaterThanMax);
		return value;
	}
	template<class T> T GetInt(T minvalue, T maxvalue)	{
		T value = GetInt<T>();
		if (value < minvalue)		Error(MESSAGE_PARSER_ValueLessThanMin);
		else if (value > maxvalue)	Error(MESSAGE_PARSER_ValueGreaterThanMax);
		return value;
	}

};

////////////////////////////////////////////////////////