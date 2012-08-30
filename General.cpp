#include "Common.h"

HINSTANCE hInstLib;
BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		// DLL is attaching to the address space of the current process.
		case DLL_PROCESS_ATTACH:
			
			hInstLib = hDLL; // Store HINSTANCE
			break;

		// A new thread is being created in the current process.
		case DLL_THREAD_ATTACH:
			break;

		// A thread is exiting cleanly.
		case DLL_THREAD_DETACH:
			break;

		// The calling process is detaching the DLL from its address space.
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}


extern "C" int WINAPI DLLExport Initialize(mv _far *mV, int quiet)
{
	return Edif::Init(mV);

	return 0;
}
extern "C" int WINAPI DLLExport Free(mv _far *mV)
{
	Edif::Free(mV);

	return 0;
}

int	WINAPI DLLExport LoadObject(mv _far *mV, LPCSTR fileName, LPEDATA edPtr, int reserved)
{
	Edif::Init(mV, edPtr);

	return 0;
}
void WINAPI DLLExport UnloadObject(mv _far *mV, LPEDATA edPtr, int reserved)
{
}

HGLOBAL WINAPI DLLExport UpdateEditStructure(mv __far *mV, void __far * OldEdPtr)
{
	LPEDATA edPtr = (LPEDATA)OldEdPtr;
	EditData(edPtr).Serialize(mV, edPtr);
	return 0;
}
void WINAPI DLLExport UpdateFileNames(mv _far *mV, LPSTR appName, LPEDATA edPtr, void (WINAPI * lpfnUpdate)(LPSTR, LPSTR))
{
}

/*
int WINAPI DLLExport EnumElts (mv __far *mV, LPEDATA edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{  
	int error = 0;

	// Replace wImgIdx with the name of the WORD variable you create within the edit structure
  
	// Enum images  
	if ( (error = enumProc(&edPtr->wImgIdx, IMG_TAB, lp1, lp2)) != 0 )
	{
		// Undo enum images	  
		undoProc (&edPtr->wImgIdx, IMG_TAB, lp1, lp2);	
	}  

	return error;
}
*/
