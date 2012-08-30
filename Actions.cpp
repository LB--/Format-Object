#include "Common.h"

void Extension::CreateFormat(const Char *Name)
{
	if(FindFormat(Name) != Formats.end())
	{
		Runtime.GenerateEvent(Trigger::Error);
		return;
	}
	Formats.push_back(new Format::ItemGroup(Name));
}
static const String ID (TS"Format");
static const unsigned FileVersion (0);
void Extension::SaveFormat(const Char *Name, const Char *File)
{
	Formats_t::iterator it = FindFormat(Name);
	if(!Assert(it != Formats.end(), String(TS"No such format: ")+Name)) return;
	std::ofstream f (File, std::ios::out|std::ios::binary|std::ios::trunc);
	if(!Assert(f.is_open(), String(TS"Failed to open: ")+File)) return;
	(*it)->Serialize((f << ID << char(FileVersion), f));
}
void Extension::LoadFormat(const Char *File, const Char *Name)
{
	Formats_t::iterator it = FindFormat(Name);
	if(!Assert(it == Formats.end(), String(TS"Format already exists: ")+Name)) return;
	std::ifstream f (File, std::ios::in|std::ios::binary);
	if(!Assert(f.is_open(), String(TS"Failed to open: ")+File)) return;
	char id[7] = {0};
	f.read(id, 6);
	if(!Assert(id == ID, String(TS"Not a valid format file: ")+File)) return;
	unsigned FileVer (unsigned(f.get()));
	if(!Assert(FileVer <= FileVersion, String(TS"Newer or unkown version: "))+File) return;
	String name; for(int c; c = f.get(); name += char(c));
	if(!Assert(f.get() == 0, String(TS"Invalid format file: ")+File)) return;
	try
	{
		Formats.push_back(new Format::ItemGroup(f, name));
	}
	catch(Format::ItemGroup *)
	{
		GenerateError("Unkown format element");
	}
}
Format::ItemGroup::ItemGroup(std::ifstream &from, const String &nam) : Item(nam) //Deserialize
{
	String n;
	int c;
	items_t::size_type size;
	from.read((char*)&size, sizeof(size));
	for(items_t::size_type i = 0; i < size; ++i)
	{
		for(n.clear(); c = from.get(); n += char(c));
		if((c = from.get()) == ItemGroup::TypeID)
		{
			items.push_back(new ItemGroup(from, n));
		}
		else if(c == Integer::TypeID)
		{
			items.push_back(new Integer(from, n));
		}
		else if(c == Float::TypeID)
		{
			items.push_back(new Float(from, n));
		}
		else if(c == Str::TypeID)
		{
			items.push_back(new Str(from, n));
		}
		else if(c == Raw::TypeID)
		{
			items.push_back(new Raw(from, n));
		}
		else
		{
			throw this;
		}
	}
}

void Extension::AddSubFormat(const Char *Name)
{
	Formats_t::iterator it = FindFormat(Name);
	if(!Assert(it != Formats.end(), String(TS"No such format: ")+Name)) return;
	Format::ItemGroup *g = (*it)->Clone();
	g->name = Name;
	Formats.back()->items.push_back(g);
}
void Extension::AddByte(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Integer(Name, 1));
}
void Extension::AddShort(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Integer(Name, 2));
}
void Extension::AddInt(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Integer(Name));
}
void Extension::AddLong(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Integer(Name, 8));
}
void Extension::AddFloat(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Float(Name, false));
}
void Extension::AddDouble(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Float(Name));
}
void Extension::AddString(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Str(Name));
}
void Extension::AddRaw(const Char *Name)
{
	if(!Assert(!Formats.empty(), TS"No format exists")) return;
	Formats.back()->items.push_back(new Format::Raw(Name));
}

void Extension::TraverseFormat(const Char *Name, const Char *File)	//	//	//	//	//	//	//
{
	Formats_t::iterator it = FindFormat(Name);
	if(!Assert(it != Formats.end(), String(TS"No such format: ")+Name)) return;
	std::ofstream f (File, std::ios::out|std::ios::binary|std::ios::trunc);
	if(!Assert(f.is_open(), String(TS"Failed to open: ")+File)) return;
	Mode = Serializing;
	OfFormat.clear();
	(*it)->Traverse(f, *this);
	Mode = Neither;
}
void Format::ItemGroup::Traverse(std::ofstream &to, Extension &ext) const
{
	ext.FormatTimes = 1;
	if(ext.OfFormat.length())
	{
		ext.ElemName = name;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::SubFormat);
	}
	for(unsigned i = 0, k = ext.FormatTimes; i < k; ++i)
	{
		for(items_t::const_iterator it = items.begin(); it != items.end(); ++it)
		{
			ext.FormatTimes = i;
			ext.OfFormat = name;
			ext.ElemName = (*it)->name;
			(*it)->Traverse(to, ext);
			if(!ext.Assert(to.good(), String(TS"An error occured while writing the file after: ")+(*it)->name)) return;
		}
	}
}
void Format::Integer::Traverse(std::ofstream &to, Extension &ext) const
{
	if(size == 1)
	{
		ext.ByteValue = 0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Byte);
		to.write((char*)&ext.ByteValue, sizeof(ext.ByteValue));
	}
	else if(size == 2)
	{
		ext.ShortValue = 0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Short);
		to.write((char*)&ext.ShortValue, sizeof(ext.ShortValue));
	}
	else if(size == 4)
	{
		ext.IntValue = 0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Int);
		to.write((char*)&ext.IntValue, sizeof(ext.IntValue));
	}
	else if(size == 8)
	{
		ext.LongValue = 0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Long);
		to.write((char*)&ext.LongValue, sizeof(ext.LongValue));
	}
	else
	{
		ext.GenerateError(TS"Unkown Size; not Byte, Short, Int, or Long");
	}
}
void Format::Float::Traverse(std::ofstream &to, Extension &ext) const
{
	if(doub)
	{
		ext.DoubleValue = 0.0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Double);
		to.write((char*)&ext.DoubleValue, sizeof(ext.DoubleValue));
	}
	else
	{
		ext.FloatValue = 0.0;
		ext.Runtime.GenerateEvent(Extension::Trigger::Save::Float);
		to.write((char*)&ext.FloatValue, sizeof(ext.FloatValue));
	}
}
void Format::Str::Traverse(std::ofstream &to, Extension &ext) const
{
	ext.NullT = true;
	ext.StrValue.clear();
	ext.Runtime.GenerateEvent(Extension::Trigger::Save::String);
	to << ext.StrValue;
	if(ext.NullT) to << ends;
}
void Format::Raw::Traverse(std::ofstream &to, Extension &ext) const
{
	ext.rawsize = 0;
	ext.Runtime.GenerateEvent(Extension::Trigger::Save::Raw);
	if(ext.rawdata) to.write(ext.rawdata, ext.rawsize); //Lazily allow the expression to allocate the memory so the size can be modifed
	delete[] ext.rawdata;
	ext.rawdata = 0;
}
void Extension::TraverseFile(const Char *File, const Char *Name)	//	//	//	//	//	//	//
{
	std::ifstream f (File, std::ios::in|std::ios::binary);
	if(!Assert(f.is_open(), String(TS"Failed to open: ")+File)) return;
	Formats_t::iterator it = FindFormat(Name);
	if(!Assert(it != Formats.end(), String(TS"No such format: ")+Name)) return;
	Mode = Deserializing;
	OfFormat.clear();
	(*it)->Traverse(f, *this);
	Mode = Neither;
}
void Format::ItemGroup::Traverse(std::ifstream &from, Extension &ext) const
{
	ext.FormatTimes = 1;
	if(ext.OfFormat.length())
	{
		ext.ElemName = name;
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::SubFormat);
	}
	for(unsigned i = 0, k = ext.FormatTimes; i < k; ++i)
	{
		for(items_t::const_iterator it = items.begin(); it != items.end(); ++it)
		{
			ext.FormatTimes = i;
			ext.OfFormat = name;
			ext.ElemName = (*it)->name;
			(*it)->Traverse(from, ext);
			if(!ext.Assert(from.good(), String(TS"An error occured while reading the file after: ")+(*it)->name)) return;
		}
	}
}
void Format::Integer::Traverse(std::ifstream &from, Extension &ext) const
{
	if(size == 1)
	{
		from.read((char*)&ext.ByteValue, sizeof(ext.ByteValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Byte);
	}
	else if(size == 2)
	{
		from.read((char*)&ext.ShortValue, sizeof(ext.ShortValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Short);
	}
	else if(size == 4)
	{
		from.read((char*)&ext.IntValue, sizeof(ext.IntValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Int);
	}
	else if(size == 8)
	{
		from.read((char*)&ext.LongValue, sizeof(ext.LongValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Long);
	}
	else
	{
		ext.GenerateError(TS"Unkown Size; not Byte, Short, Int, or Long");
	}
}
void Format::Float::Traverse(std::ifstream &from, Extension &ext) const
{
	if(doub)
	{
		from.read((char*)&ext.DoubleValue, sizeof(ext.DoubleValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Double);
	}
	else
	{
		from.read((char*)&ext.FloatValue, sizeof(ext.FloatValue));
		ext.Runtime.GenerateEvent(Extension::Trigger::Load::Float);
	}
}
void Format::Str::Traverse(std::ifstream &from, Extension &ext) const
{
	ext.NullT = true;
	ext.StrValue.clear();
	ext.lfrom = &from; //Leave it to the expression to use 'lazy initialization' >v
	ext.Runtime.GenerateEvent(Extension::Trigger::Load::String); //to allow for changing from null terminated to length terminated
	ext.lfrom = 0;
}
void Format::Raw::Traverse(std::ifstream &from, Extension &ext) const
{
	ext.rawsize = 0;
	ext.lfrom = &from;
	ext.Runtime.GenerateEvent(Extension::Trigger::Load::Raw); //allow expression to lazily initialize & load
	delete ext.rawdata;
	ext.rawdata = 0;
}

void Extension::SetFormatTimes(int times)
{
	FormatTimes = times > 0 ? times : 0;
}
void Extension::SetByteVal(int val)
{
	ByteValue = char(val);
}
void Extension::SetShortVal(int val)
{
	ShortValue = short(val);
}
void Extension::SetIntVal(int val)
{
	IntValue = int(val);
}
void Extension::SetLongVal(const Char *val)
{
	LongValue = ConvTo<long long>::f(val);
}
void Extension::SetFloatVal(float val)
{
	FloatValue = val;
}
void Extension::SetDoubleVal(const Char *val)
{
	DoubleValue = ConvTo<double>::f(val);
}
void Extension::SetStringVal(const Char *val, int nullt)
{
	StrValue = val;
	NullT = nullt?false:true;
}

void Extension::SetStringSize(int size)
{
	NullT = false;
	StrValue.clear();
	StrValue.resize(size, '\0');
}
void Extension::SetRawSize(int size)
{
	rawsize = unsigned(size);
}
