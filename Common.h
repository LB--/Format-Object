#pragma once

#include <string>
#ifdef UNICODE
	typedef std::wstring String;
	#define TS L
	typedef wchar_t Char;
#else
	typedef std::string String;
	#define TS
	typedef char Char;
#endif

// #define TGFEXT	   // TGF2, MMF2, MMF2 Dev
   #define MMFEXT	   // MMF2, MMF2 Dev
// #define PROEXT	   // MMF2 Dev only

#include	"Edif.h"
#include	"Resource.h"

struct EDITDATA
{
	extHeader		eHeader;

	char data[1];
};

class Extension;

struct RUNDATA
{
	headerObject rHo;

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//	rCom			rc;				// Common structure for movements & animations
//	rMvt			rm;				// Movements
//	rSpr			rs;				// Sprite (displayable objects)
	rVal			rv;				// Alterable values

	Extension * pExtension;
};

#include "Extension.h"
