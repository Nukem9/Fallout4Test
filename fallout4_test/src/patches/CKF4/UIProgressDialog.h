#pragma once

#include "..\..\Common.h"
#include "UIBaseWindow.h"
#include "TESDataFileHandler_CK.h"

#define UI_PROGRESS_CMD_STEPIT	(UI_CUSTOM_MESSAGE + 5)

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			class CUIProgressDialog : public CUICustomDialog
			{
			private:
				ULONG_PTR m_OldStyles;
				BOOL m_Marquee;
				INT32 SafePos;
				UINT32 m_Min, m_Max;
				std::string tmp_str;
				CUIBaseControl m_Progress;
				CUIBaseControl m_Label;
			public:
				LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
				VOID SetMinAndMax(const UINT16 min, const UINT16 max);
			public:
				UINT16 GetMin(VOID);
				VOID SetMin(const UINT16 value);
				UINT16 GetMax(VOID);
				VOID SetMax(const UINT16 value);
				UINT16 GetIncrement(VOID);
				VOID SetIncrement(const UINT16 value);
				UINT16 GetPosition(VOID);
				VOID SetPosition(const UINT16 value);
				VOID SetMarquee(const BOOL value);
				BOOL GetMarquee(VOID);
				std::string GetMessageText(VOID);
				VOID SetMessageText(const std::string &str);
				VOID Step(VOID);
				static VOID FIXAPI ProcessMessages(VOID);
				static VOID FIXAPI ProcessMessagesOnlyLoadCellWorld(VOID);
			public:
				PROPERTY(GetMin, SetMin) UINT16 Min;
				PROPERTY(GetMax, SetMax) UINT16 Max;
				PROPERTY(GetIncrement, SetIncrement) UINT16 Increment;
				PROPERTY(GetPosition, SetPosition) UINT16 Position;
				PROPERTY(GetMarquee, SetMarquee) BOOL Marquee;
				PROPERTY(GetMessageText, SetMessageText) std::string MessageText;
			public:
				CUIProgressDialog(CUICustomWindow* parent, const UINT res_id);
				~CUIProgressDialog(VOID);
			};

			extern CUIProgressDialog* ProgressDialog;
		}
	}
}