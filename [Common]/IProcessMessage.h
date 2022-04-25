//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once

#include "CommInc.h"

class IProcessMessage {
public:
	enum EFlag : DWORD {
		ProcessingAll	= 0,
		RemoveMessage	= 1,
		NoYield			= 2,
		Loop			= 4,
		ProcessingInput = 8,
		ProcessingPaint = 16,
		ProcessingSends = 32,
		ProcessingPosts = 64,
	};
	typedef BOOL(FIXAPI* CloseHandlerT)(VOID);
public:
	static BOOL FIXAPI CloseHandlerDefault(VOID);
public:
	static VOID FIXAPI ProcessMessage(HWND hWindow, DWORD dwMsgFilterMin, DWORD dwMsgFilterMax, DWORD flag,
		CloseHandlerT CloseHandler = CloseHandlerDefault);
	static VOID FIXAPI ProcessMessageAsync(HWND hWindow, DWORD dwMsgFilterMin, DWORD dwMsgFilterMax, DWORD flag,
		CloseHandlerT CloseHandler = CloseHandlerDefault);
};