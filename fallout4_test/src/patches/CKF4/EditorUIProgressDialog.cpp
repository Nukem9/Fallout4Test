#include "../../common.h"
#include "UIBaseWindow.h"
#include "UIProgressDialog.h"
#include "EditorUI.h"
#include "EditorUIProgressDialog.h"
#include "CellViewWindow.h"
#include "ObjectWindow.h"
#include "RenderWindow.h"
#include "MainWindow.h"
#include "LogWindow.h"

#include "TESCellViewScene_CK.h"

namespace EditorUI
{
	namespace sys = Core::Classes::UI;

	BOOL FIXAPI hk_CallLoadFile(TESDataFileHandler_CK* io_handler, INT32 _zero_only)
	{
		// get and save F4FileDataHandler from CK
		// FileHandler = io_handler; 0x6D67960 and doesn't change

		// Replacing Tips with a progress Bar
		if (bReplaceTips)
		{
			Assert(!sys::ProgressDialog);
			// show Progress
			sys::ProgressDialog = new sys::CUIProgressDialog(&MainWindow::GetWindowObj(), 3238);
			Assert(sys::ProgressDialog);
			sys::ProgressDialog->Create();
			// set Text 
			sys::ProgressDialog->MessageText = "Loading Files...";
		}

		// load
		return io_handler->Load(_zero_only);
	}

	VOID FIXAPI hk_EndLoadFile(VOID)
	{
		// Replacing Tips with a progress Bar
		if (bReplaceTips)
		{
			// close Progress
			delete sys::ProgressDialog;
			sys::ProgressDialog = NULL;
		}

		RenderWindow::GetWindowObj().Foreground();
		CellViewWindow::GetWindowObj().Foreground();

		ObjectWindow::OBJWNDS Wnds = ObjectWindow::GetAllWindowObj();

		for each (auto Wnd in Wnds)
		{
			if (Wnd.second->ObjectWindow.Visible)
				Wnd.second->ObjectWindow.Foreground();
		}
	}

	VOID FIXAPI hk_StepItProgress(LPCSTR* str)
	{
		Assert(sys::ProgressDialog);

		// set position 0..95%
		std::string s(MainWindow::GetWindowObj().GetTextToStatusBarA(3));

		s.assign(s.begin() + s.find('%') - 2, s.end());
		sys::ProgressDialog->Position = strtol(s.c_str(), NULL, 10);
	}

	BOOL FIXAPI hk_UpdateProgress(LPVOID __this, INT32 __1)
	{
		// Fallout CK is generally cut out.....

		static FLOAT lastPercent = 0.0f;

		// Only update every quarter percent, rather than every single form load
		FLOAT newPercent = ((FLOAT) * (UINT32*)OFFSET(0x6D6A7D8, 0) / (FLOAT) * (UINT32*)OFFSET(0x6D6A7D4, 0)) * 100.0f;

		if (abs(lastPercent - newPercent) > 0.25f)
			Core::Classes::UI::CUIMainWindow::ProcessMessages();

		return ((BOOL(__fastcall*)(LPVOID __this, INT32 __1))OFFSET(0x8027C0, 0))(__this, __1);
	}

	VOID FIXAPI hk_SetTextAndSendStatusBar(UINT32 index, LPCSTR message)
	{
		// send Status Bar
		((VOID(__stdcall*)(UINT32, LPCSTR))OFFSET(0x5FDFE0, 0))(index, message);
		// set Text 
		sys::ProgressDialog->MessageText = message;
		Core::Classes::UI::CUIMainWindow::ProcessMessages();
	}

	VOID FIXAPI hk_SendFromCellViewToRender(LPVOID Unknown1, TESForm_CK* View, INT32 Unknown3)
	{
		Assert(!sys::ProgressDialog);
		// show Progress
		sys::ProgressDialog = new sys::CUIProgressDialog(&MainWindow::GetWindowObj(), 3238);
		Assert(sys::ProgressDialog);
		sys::ProgressDialog->Create();

		sys::ProgressDialog->Marquee = TRUE;
		sys::ProgressDialog->MessageText = "Please wait while requested cell loads ...";

		// send
		((VOID(__fastcall*)(LPVOID, TESForm_CK*, INT32))OFFSET(0x45FE60, 0))(Unknown1, View, Unknown3);
	}

	VOID FIXAPI hk_EndSendFromCellViewToRender(VOID)
	{
		// Replacing Tips with a progress Bar
		if (bReplaceTips)
		{
			// close Progress
			delete sys::ProgressDialog;
			sys::ProgressDialog = NULL;
		}

		// enabled all markers
		Core::Classes::UI::CUIMenuItem MenuItem = MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(15);
		if (!MenuItem.Checked)
			MenuItem.Click();

		// enabled light markers
		MenuItem = MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(16);
		if (!MenuItem.Checked)
			MenuItem.Click();

		// enabled sound markers
		MenuItem = MainWindow::GetMainMenuObj().GetSubMenuItem(2).GetItemByPos(17);
		if (!MenuItem.Checked)
			MenuItem.Click();

		RenderWindow::GetWindowObj().Foreground();
	}
}
