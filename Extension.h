#include <typeinfo>
#include "StringConvert.hpp"
#include "EditData.hpp"
#include "Format.hpp"

class Extension
{
public:
	LPRDATA rdPtr;
	LPRH	rhPtr;
	Edif::Runtime Runtime;
	static const int MinimumBuild = 254;
	static const int Version = 1;
	static const int OEFLAGS = OEFLAG_VALUES|OEFLAG_RUNBEFOREFADEIN|OEFLAG_NEVERKILL;
	static const int OEPREFS = 0;
	static const int WindowProcPriority = 100;

	Extension(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr);
	~Extension();

	typedef std::vector<Format::ItemGroup*> Formats_t;
	Formats_t Formats;
	String ErrorMessage;

	enum Mode_t
	{
		Serializing,	//Saving
		Deserializing,	//Loading
		Neither,		//Neither
	} Mode; //Whether we are saving or loading
	String OfFormat; //The format to which the current element belongs
	String ElemName; //The name of the current element
	union
	{
		unsigned FormatTimes;	//Number of times to loop a format (if SubFormat)
		char ByteValue;			//The value to use (if Integer)
		short ShortValue;		//The value to use (if Integer)
		int IntValue;			//The value to use (if Integer)
		long long LongValue;	//The value to use (if Integer)
		float FloatValue;		//The value to use (if Float)
		double DoubleValue;		//The value to use (if Float)
		bool NullT;				//Whether to use a null terminator (if Str)
	};	String StrValue;		//The String to use (if Str)
	std::ifstream *lfrom;		//For loading (if Str)
	char *rawdata;				//For raw data (if Raw)
	unsigned rawsize;			//For raw data (if Raw)

	struct Trigger //holds enums for triggerable condition IDs
	{
		enum TriggerID
		{
			Error = 0,
		};
		struct Save //triggerable save condition IDs
		{
			enum SaveID
			{
				SubFormat = 1,
				Byte,
				Short,
				Int,
				Long,
				Float,
				Double,
				String,
				Raw = 19,
			};
		private:Save();Save(const Save&);Save&operator=(const Save&);~Save();
		};
		struct Load //triggerable load condition IDs
		{
			enum LoadID
			{
				SubFormat = 10,
				Byte,
				Short,
				Int,
				Long,
				Float,
				Double,
				String,
				Raw = 20,
			};
		private:Load();Load(const Load&);Load&operator=(const Load&);~Load();
		};
	private:Trigger();Trigger(const Trigger&);Trigger&operator=(const Trigger&);~Trigger();
	};

	//Conditions
	bool OnError();

	bool OnElement(const Char *Format, const Char *Name);

	bool IsSaving();
	bool IsLoading();

	//Actions
	void CreateFormat(const Char *Name);
	void SaveFormat(const Char *Name, const Char *File);
	void LoadFormat(const Char *File, const Char *Name);

	void AddSubFormat(const Char *Name);
	void AddByte(const Char *Name);
	void AddShort(const Char *Name);
	void AddInt(const Char *Name);
	void AddLong(const Char *Name);
	void AddFloat(const Char *Name);
	void AddDouble(const Char *Name);
	void AddString(const Char *Name);
	void AddRaw(const Char *Name);

	void TraverseFormat(const Char *Name, const Char *File);
	void TraverseFile(const Char *File, const Char *Name);

	void SetFormatTimes(int times);
	void SetByteVal(int val);
	void SetShortVal(int val);
	void SetIntVal(int val);
	void SetLongVal(const Char *val); //MMF2 cannot transmit 64-bit integers
	void SetFloatVal(float val);
	void SetDoubleVal(const Char *val); //MMF2 cannot transmit 64-bit floats
	void SetStringVal(const Char *val, int nullt);

	void SetStringSize(int size);
	void SetRawSize(int size);

	//Expressions
	const Char *GetError();

	int GetSubFormatLoopIndex();
	int GetByte();
	int GetUByte();
	int GetShort();
	int GetUShort();
	int GetInt();
	const Char *GetUInt(); //MMF2 cannot receive 32-bit unsigned integers
	const Char *GetLong(); //MMF2 cannot receive 64-bit integers
	const Char *GetULong();
	float GetFloat();
	const Char *GetDouble(); //MMF2 cannot receive 64-bit floats
	const Char *GetString();
	int GetRawMemAddr();
	int GetRawSize();


	void Action(int ID, LPRDATA rdPtr, long param1, long param2);
	long Condition(int ID, LPRDATA rdPtr, long param1, long param2);
	long Expression(int ID, LPRDATA rdPtr, long param);

	short Handle();
	short Display();

	short Pause();
	short Continue();

	bool Save(HANDLE File);
	bool Load(HANDLE File);

	void GenerateError(const String &msg){ Assert(false, msg); }
	bool Assert(bool exp, const String &msg)
	{
		if(!exp)
		{
			ErrorMessage = msg;
			Runtime.GenerateEvent(Trigger::Error);
		}
		return exp;
	}
protected:
	Formats_t::iterator FindFormat(const String &name)
	{
		for(Formats_t::iterator it = Formats.begin(); it != Formats.end(); ++it)
		{
			if((*it)->name == name && typeid(**it) == typeid(Format::ItemGroup))
			{
				return it;
			}
		}
		return Formats.end();
	}
};
