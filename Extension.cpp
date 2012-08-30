#include "Common.h"

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr) : rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr),
																	Mode(Neither), lfrom(0), rawsize(0), rawdata(0)
{
	LinkCondition(0, OnError);
	LinkCondition(1, OnElement);
	LinkCondition(2, OnElement);
	LinkCondition(3, OnElement);
	LinkCondition(4, OnElement);
	LinkCondition(5, OnElement);
	LinkCondition(6, OnElement);
	LinkCondition(7, OnElement);
	LinkCondition(8, OnElement);
	LinkCondition(9, IsSaving);
	LinkCondition(10,OnElement);
	LinkCondition(11,OnElement);
	LinkCondition(12,OnElement);
	LinkCondition(13,OnElement);
	LinkCondition(14,OnElement);
	LinkCondition(15,OnElement);
	LinkCondition(16,OnElement);
	LinkCondition(17,OnElement);
	LinkCondition(18,IsLoading);
	LinkCondition(19,OnElement);
	LinkCondition(20,OnElement);

	LinkAction(0, CreateFormat);
	LinkAction(1, SaveFormat);
	LinkAction(2, LoadFormat);
	LinkAction(3, AddSubFormat);
	LinkAction(4, AddByte);
	LinkAction(5, AddShort);
	LinkAction(6, AddInt);
	LinkAction(7, AddLong);
	LinkAction(8, AddFloat);
	LinkAction(9, AddDouble);
	LinkAction(10,AddString);
	LinkAction(11,TraverseFormat);
	LinkAction(12,TraverseFile);
	LinkAction(13,SetFormatTimes);
	LinkAction(14,SetByteVal);
	LinkAction(15,SetShortVal);
	LinkAction(16,SetIntVal);
	LinkAction(17,SetLongVal);
	LinkAction(18,SetFloatVal);
	LinkAction(19,SetDoubleVal);
	LinkAction(20,SetStringVal);
	LinkAction(21,SetStringSize);
	LinkAction(22,AddRaw);
	LinkAction(23,SetRawSize);

	LinkExpression(0, GetError);
	LinkExpression(1, GetSubFormatLoopIndex);
	LinkExpression(2, GetByte);
	LinkExpression(3, GetUByte);
	LinkExpression(4, GetShort);
	LinkExpression(5, GetUShort);
	LinkExpression(6, GetInt);
	LinkExpression(7, GetUInt);
	LinkExpression(8, GetLong);
	LinkExpression(9, GetULong);
	LinkExpression(10,GetFloat);
	LinkExpression(11,GetDouble);
	LinkExpression(12,GetString);
	LinkExpression(13,GetRawMemAddr);
	LinkExpression(14,GetRawSize);
}
Extension::~Extension()
{
	for(Formats_t::iterator it = Formats.begin(); it != Formats.end(); ++it)
	{
		delete (*it);
	}
}


short Extension::Handle()
{
	return REFLAG_ONESHOT;
}
short Extension::Display()
{
	return 0;
}

short Extension::Pause()
{
	return 0;
}
short Extension::Continue()
{
	return 0;
}

bool Extension::Save(HANDLE File)
{
#ifndef VITALIZE

	unsigned long written;
	unsigned long const Version (0);
	if(!WriteFile(File, &Version, sizeof(Version), &written, 0) || written != sizeof(Version)) return false;
	Formats_t::size_type nf = Formats.size();
	if(!WriteFile(File, &nf, sizeof(nf), &written, 0) || written != sizeof(nf)) return false;
	for(Formats_t::const_iterator it = Formats.begin(); it != Formats.end(); ++it)
	{
		if(!((*it)->Serialize(File))) return false;
	}

	return true;
#endif
	return false;
}
bool Extension::Load(HANDLE File)
{
#ifndef VITALIZE

	unsigned long written;
	unsigned long version;
	if(!ReadFile(File, &version, sizeof(version), &written, 0) || written != sizeof(version)) return false;
	if(version == 0)
	{
		Formats_t::size_type nf;
		if(!ReadFile(File, &nf, sizeof(nf), &written, 0) || written != sizeof(nf)) return false;
		for(Formats_t::size_type i = 0; i < nf; ++i)
		{
			String n;
			char c;
			for(;; n += c)
			{
				if(!ReadFile(File, &c, sizeof(c), &written, 0) || written != sizeof(c)) return false;
				if(!c) break;
			}
			if(!ReadFile(File, &c, sizeof(c), &written, 0) || written != sizeof(c)) return false;
			if(c != Format::ItemGroup::TypeID) return false;
			try
			{
				Formats.push_back(new Format::ItemGroup(File, n));
			}
			catch(Format::Item *)
			{
				return false;
			}
		}
	}
	else return false;

	return true;
#endif
	return false;
}
Format::ItemGroup::ItemGroup(HANDLE File, const String &nam) : Item(nam)
{
	unsigned long written;
	String n;
	char c;
	items_t::size_type size;
	if(!ReadFile(File, &size, sizeof(size), &written, 0) || written != sizeof(size)) throw this;
	for(items_t::size_type i = 0; i < size; ++i)
	{
		for(n.clear();; n += c)
		{
			if(!ReadFile(File, &c, sizeof(c), &written, 0) || written != sizeof(c)) throw this;
			if(!c) break;
		}
		if(!ReadFile(File, &c, sizeof(c), &written, 0) || written != sizeof(c)) throw this;
		if(c == ItemGroup::TypeID)
		{
			items.push_back(new ItemGroup(File, n));
		}
		else if(c == Integer::TypeID)
		{
			items.push_back(new Integer(File, n));
		}
		else if(c == Float::TypeID)
		{
			items.push_back(new Float(File, n));
		}
		else if(c == Str::TypeID)
		{
			items.push_back(new Str(File, n));
		}
		else if(c == Raw::TypeID)
		{
			items.push_back(new Raw(File, n));
		}
		else
		{
			throw this;
		}
	}
}

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{
	//
}
long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{
	return false;
}
long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{
	return long("");
}

