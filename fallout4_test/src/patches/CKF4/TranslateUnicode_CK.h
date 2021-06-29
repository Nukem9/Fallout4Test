#pragma once

//#define __DBG_TRAN_UNICODE_READ
//#define __DBG_TRAN_UNICODE_WRITE

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include "..\..\Common.h"

#include <list>
#include <string>

namespace Core
{
	namespace Patch
	{
		namespace CreationKit
		{
			class CTranslateUnicodeMediator
			{
			private:
				BOOL m_Mode;
				LPSTR m_preaddres;
				std::list<std::string> heap;
#ifdef __DBG_TRAN_UNICODE_WRITE
				std::ofstream out;
#elif __DBG_TRAN_UNICODE_READ
				std::ofstream out;
#endif // __DBG_TRAN_UNICODE
			private:
				BOOL IsValid(LPCSTR addr, const BOOL no_ansi = FALSE);
				LPSTR Utf8ToWinCP(LPCSTR src);
				LPSTR WinCPToUtf8(LPCSTR src);
			public:
				VOID SetMode(const BOOL mode);
				LPSTR Translate(LPCSTR src);
			public:
				CTranslateUnicodeMediator(VOID);
				~CTranslateUnicodeMediator(VOID);
			};

			extern CTranslateUnicodeMediator Translator;
		}
	}
}

namespace Experimental
{
	INT FIXAPI BeginPluginSave(VOID);
	LPVOID FIXAPI EndPluginSave(LPVOID __arg1);
	LPSTR FIXAPI Translate(LPSTR str);

	BOOL WINAPI hk_SetDlgItemTextA(HWND hDlg, INT32 nIDDlgItem, LPCSTR lpString);
	LRESULT WINAPI hk_SendDlgItemMessageA(HWND hDlg, INT32 nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam);
}