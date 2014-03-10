#include "Common.h"

#ifndef RUN_ONLY
namespace Prop
{
	enum
	{
		zzzunused = PROPID_EXTITEM_CUSTOM_FIRST,
		Version,
		Help,
	};
}

#define MKRS (UINT_PTR)LPCSTR
PropData Properties[] =
{
	PropData_StaticString_Opt(Prop::Version, MKRS("Version"), MKRS("The current version of Format Object"), PROPOPT_BOLD),
	PropData_Button(Prop::Help, MKRS("Click for:"), MKRS("Click here for a breif help dialog (just until I make a help file)"), MKRS("Help")),
	PropData_End()
};
#endif

BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	mvInsertProps(mV, edPtr, Properties, PROPID_TAB_GENERAL, TRUE);
#endif
	return TRUE;
}
void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
#endif
}

LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
//	switch(nPropID)
//	{
		//
//	}
#endif
	return NULL;
}
void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif
}

LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	switch(nPropID)
	{
	case Prop::Version:
		return new CPropDataValue("Release 2");
	}
#endif
	return NULL;
}
BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
//	switch(nPropID)
//	{
		//
//	}
#endif
	return 0;
}

void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	CPropValue* pValue = (CPropValue*)lParam;

//	switch (nPropID)
//	{
		//
//	}
#endif
}
void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
//	switch(nPropID)
//	{
		//
//	}
#endif
}
BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	switch(nPropID)
	{
	case Prop::Help:
		{
			String msgs[] =
			{
				_T("With the Format Object, you can create customized formats for any task and then use those formats to save and load files."),
				_T("To start, you need to create all the formats you will need in reverse order.\r\nFor example, if format A constains an integer, format B, and a string, you would need to create format B first, so that you can later add it into format A."),
				_T("The 'add' actions affect the last format you created, which is why you need to create the format in reverse order. Once you've added a format into another, you no longer need the one you added."),
				_T("This means you only need to save the final resulting format to a format file to load it and use it - the other formats you made were just placeholders."),
				_T("Saving and Loading using Formats should be almost intuitive; just remember that unless you specify otherwise, strings will be loaded with null terminators.\r\nYou must specify a size BEFORE using the expression to get the string."),
				_T("Some number types have to be dealt with as strings due to technical limitations in either MMF2 itself or the way MMF2 communicates with extensions.\r\nFor example, MMF2 internally uses 64-bit double precision floats, but it can only transmit 32-bit floats to/from extensions."),
				_T("The Raw Data type is meant for useage with the Memory Object and other objects which give/take memory addresses for their operations.\r\nYou should set the size of the Raw Data BEFORE using the expression to get its memory address."),
				_T("I hope this explains enough - if you need more information feel free to post and ask."),
				_T("")
			};
			for(unsigned i = 0; msgs[i].length(); ++i)
			{
				MessageBox(mV->mvHEditWin, msgs[i].c_str(), _T("Help"), MB_OK);
			}
		}
		break;
	}
#endif
	return FALSE;
}

BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	EditData ed (edPtr);
	switch(nPropID)
	{
	case Prop::Version:
		return FALSE;
	case Prop::Help:
		return TRUE;
	}
#endif
	return TRUE;
}


int WINAPI DLLExport MakeIconEx(mv _far *mV, cSurface* pIconSf, LPTSTR lpName, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	pIconSf->Delete();
	pIconSf->Clone(*SDK->Icon);

	pIconSf->SetTransparentColor(RGB(255, 0, 255));
#endif
   return 0;
}

int WINAPI DLLExport CreateObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	if(IS_COMPATIBLE(mV))
	{
		Edif::Init(mV, edPtr);

		EditData().Serialize(mV, edPtr);

		return 0;
	}
#endif
	return -1;
}
void WINAPI	DLLExport CreateFromFile (LPMV mV, LPTSTR fileName, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif
}
void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif
}
void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}
void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	if(0 == cpt)
	{
		Edif::Free(edPtr);
	}
#endif
}

BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{
	
	}
#endif // !defined(RUN_ONLY)
	return FALSE;
}
/*
BOOL WINAPI SetEditSize(LPMV mv, LPEDATA edPtr, int cx, int cy)
{
#ifndef RUN_ONLY
	edPtr->swidth = cx;
	edPtr->sheight = cy;
#endif
	return TRUE;
}
*/

void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	rc->right = rc->left + SDK->Icon->GetWidth();
	rc->bottom = rc->top + SDK->Icon->GetHeight();
#endif
	return;
}

void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY

	LPSURFACE Surface = WinGetSurface((int) mV->mvIdEditWin);

	if(!Surface)
		return;

	SDK->Icon->Blit(*Surface, rc->left, rc->top, BMODE_TRANSP, BOP_COPY, 0);

#endif
}

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy)
{
#ifndef RUN_ONLY
	//
#endif
	return FALSE;
}

void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY
#endif
}

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved)
{
#ifndef RUN_ONLY
#endif
	return FALSE;
}

BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPTSTR fileName)
{
#ifndef RUN_ONLY
#endif
	return FALSE;
}


DWORD WINAPI DLLExport GetTextCaps(mv _far *mV, LPEDATA edPtr){ return 0; }
BOOL WINAPI DLLExport GetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPTSTR pStyle, UINT cbSize){ return TRUE; }
BOOL WINAPI DLLExport SetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPCSTR pStyle){ return TRUE; }
COLORREF WINAPI DLLExport GetTextClr(mv _far *mV, LPEDATA edPtr){ return 0; }
void WINAPI DLLExport SetTextClr(mv _far *mV, LPEDATA edPtr, COLORREF color){}
DWORD WINAPI DLLExport GetTextAlignment(mv _far *mV, LPEDATA edPtr){ return 0; }
void WINAPI DLLExport SetTextAlignment(mv _far *mV, LPEDATA edPtr, DWORD dwAlignFlags){}


void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
#ifndef RUN_ONLY
	//
#endif
}
/*
#ifndef RUN_ONLY
BOOL CALLBACK DLLExport SetupProc(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt*			pExt;

	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog

			// Save edptr
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt=(paramExt*)lParam;

			SetDlgItemText(hDlg, IDC_EDIT, pExt->pextData);
			return TRUE;

		case WM_COMMAND: // Command

			// Retrieve edptr
			pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);

			switch (wmCommandID)
			{
			case IDOK:	// Exit
				GetDlgItemText(hDlg, IDC_EDIT, pExt->pextData, 500);
				pExt->pextSize=sizeof(paramExt)+strlen(pExt->pextData)+1;
				EndDialog(hDlg, TRUE);
				return TRUE;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}
#endif // !defined(RUN_ONLY)
*/void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#ifndef RUN_ONLY
	//DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_TRYPARAM), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)pExt);
#endif
}

void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#if !defined(RUN_ONLY)
	//
#endif
}
