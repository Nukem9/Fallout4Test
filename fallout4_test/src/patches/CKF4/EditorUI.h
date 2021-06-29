#pragma once

#include "../../common.h"

namespace EditorUI
{
	extern BOOL bReplaceTips;

	VOID FIXAPI Initialize(VOID);

	BOOL   FIXAPI ListViewCustomSetItemState(HWND ListViewHandle, WPARAM Index, UINT Data, UINT Mask);
	VOID   FIXAPI ListViewSelectItem(HWND ListViewHandle, INT32 ItemIndex, bool KeepOtherSelections);
	VOID   FIXAPI ListViewFindAndSelectItem(HWND ListViewHandle, LPVOID Parameter, bool KeepOtherSelections);
	LPVOID FIXAPI ListViewGetSelectedItem(HWND ListViewHandle);
	VOID   FIXAPI ListViewDeselectItem(HWND ListViewHandle, LPVOID Parameter);
	VOID   FIXAPI TabControlDeleteItem(HWND TabControlHandle, uint32_t TabIndex);

	LRESULT FIXAPI hk_SetSettingsPartStatusBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	VOID FIXAPI hk_SpamFPSToStatusBar(INT64 a1);
}