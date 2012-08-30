#include "Common.h"

#ifndef RUN_ONLY
namespace Debug
{
	enum
	{
		None,
	};
}
WORD DebugTree[] =
{
	DB_END
};

#endif

/*
cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
{
	return NULL;
}
LPSMASK WINAPI DLLExport GetRunObjectCollisionMask(LPRDATA rdPtr, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0 )	// Note: only if your object has the OEPREFS_INKEFFECTS option
		return NULL;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->m_pColMask;
	if ( pMask == NULL )
	{
		if ( rdPtr->m_pSurface != NULL )
		{
			DWORD dwMaskSize = rdPtr->m_pSurface->CreateMask(NULL, lParam);
			if ( dwMaskSize != 0 )
			{
				pMask = (LPSMASK)calloc(dwMaskSize, 1);
				if ( pMask != NULL )
				{
					rdPtr->m_pSurface->CreateMask(pMask, lParam);
					rdPtr->m_pColMask = pMask;
				}
			}
		}
	}
	return pMask;
}
*/

void WINAPI DLLExport StartApp(mv _far *mV, CRunApp* pApp)
{
}
void WINAPI DLLExport EndApp(mv _far *mV, CRunApp* pApp)
{
}

void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
}
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
}
/*
LPRDATA GetRdPtr(HWND hwnd, LPRH rhPtr)
{
	return (LPRDATA)GetProp(hwnd, (LPCSTR)rhPtr->rh4.rh4AtomRd);
}

LRESULT CALLBACK DLLExport WindowProc(LPRH rhPtr, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	LPRDATA rdPtr = NULL;

	switch (nMsg) {

	// Example
	case WM_CTLCOLORSTATIC:
		{
			// Get the handle of the control
			HWND hWndControl = (HWND)lParam;

			// Get a pointer to the RUNDATA structure (see GetRdptr function above for more info)
			rdPtr = GetRdPtr(hWndControl, rhPtr);

			// Check if the rdPtr pointer is valid and points to an object created with this extension
			if ( rdPtr == NULL || rdPtr->rHo.hoIdentifier != IDENTIFIER )
				break;

			// OK, intercept the message
			HDC hDC = (HDC)wParam;
			SetBkColor(hDC, rdPtr->backColor);
			SetTextColor(hDC, rdPtr->fontColor);
			rhPtr->rh4.rh4KpxReturn = (long)rdPtr->hBackBrush;
			return REFLAG_MSGRETURNVALUE;
		}
		break;
	}

	return 0;
}
*/
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#ifndef RUN_ONLY
	return DebugTree;
#endif
	return NULL;
}

void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#ifndef RUN_ONLY
	Extension &ext (*rdPtr->pExtension);
//	switch(id)
//	{
		//
//	}
#endif
}
void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#ifndef RUN_ONLY
	Extension &ext (*rdPtr->pExtension);
//	switch (id)
//	{
		//
//	}
#endif
}
