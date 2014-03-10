#pragma once

#include <string>
#ifdef UNICODE
	typedef std::wstring String;
	typedef wchar_t Char;
#else
	typedef std::string String;
	typedef char Char;
#endif

// #define TGFEXT	   // TGF2, MMF2, MMF2 Dev
   #define MMFEXT	   // MMF2, MMF2 Dev
// #define PROEXT	   // MMF2 Dev only

#include	"Edif.h"
#include	"Resource.h"

#include "Extension.h"
