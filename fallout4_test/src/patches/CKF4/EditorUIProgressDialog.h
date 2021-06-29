#pragma once

#include "../../common.h"
#include "TESDataFileHandler_CK.h"
#include "TESForm_CK.h"

namespace EditorUI
{
	BOOL FIXAPI hk_CallLoadFile(TESDataFileHandler_CK* io_handler, INT32 _zero_only);
	VOID FIXAPI hk_EndLoadFile(VOID);
	VOID FIXAPI hk_StepItProgress(LPCSTR* str);
	BOOL FIXAPI hk_UpdateProgress(LPVOID __this, INT32 __1);
	VOID FIXAPI hk_SetTextAndSendStatusBar(UINT32 index, LPCSTR message);
	VOID FIXAPI hk_SendFromCellViewToRender(LPVOID Unknown1, TESForm_CK* View, INT32 Unknown3);
	VOID FIXAPI hk_EndSendFromCellViewToRender(VOID);
}