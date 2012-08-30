#include "Common.h"

const Char *Extension::GetError()
{
	return Runtime.CopyString(ErrorMessage.c_str());
}

int Extension::GetSubFormatLoopIndex()
{
	return FormatTimes;
}
int Extension::GetByte()
{
	return ByteValue;
}
int Extension::GetUByte()
{
	return unsigned char(ByteValue);
}
int Extension::GetShort()
{
	return ShortValue;
}
int Extension::GetUShort()
{
	return unsigned short(ShortValue);
}
int Extension::GetInt()
{
	return IntValue;
}
const Char *Extension::GetUInt()
{
	return Runtime.CopyString(ConvTo<String>::f(unsigned int(IntValue)).c_str());
}
const Char *Extension::GetLong()
{
	return Runtime.CopyString(ConvTo<String>::f(LongValue).c_str());
}
const Char *Extension::GetULong()
{
	return Runtime.CopyString(ConvTo<String>::f(unsigned long long(LongValue)).c_str());
}
float Extension::GetFloat()
{
	return FloatValue;
}
const Char *Extension::GetDouble()
{
	return Runtime.CopyString(ConvTo<String>::f(DoubleValue).c_str());
}
const Char *Extension::GetString()
{
	if(lfrom)
	{
		if(NullT)
		{
			std::getline(*lfrom, StrValue, '\0');
		}
		else
		{
			for(String::iterator it = StrValue.begin(); it != StrValue.end(); ++it)
			{
				*it = Char(lfrom->get());
			}
		}
		lfrom = 0;
	}
	return Runtime.CopyString(StrValue.c_str());
}
int Extension::GetRawMemAddr()
{
	if(!rawdata)
	{
		rawdata = new char[rawsize];
		if(lfrom)
		{
			lfrom->read(rawdata, rawsize);
			lfrom = 0;
		}
		else
		{
			memset(rawdata, 0, rawsize);
		}
	}
	return int(rawdata);
}
int Extension::GetRawSize()
{
	return int(rawsize);
}
