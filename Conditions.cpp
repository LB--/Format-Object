#include "Common.h"

bool Extension::OnError()
{
	return true;
}

bool Extension::OnElement(const Char *Format, const Char *Name) //Generic Triggerable for All Types
{
	return OfFormat == Format && ElemName == Name;
}

bool Extension::IsSaving()
{
	return Mode == Serializing;
}
bool Extension::IsLoading()
{
	return Mode == Deserializing;
}
