//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
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

#include "..\common.h"
#include "TES/BSTArray.h"

namespace Experimental {
	DWORD FIXAPI QSIMDFastSearchArrayItemOffsetDWORD(BSTArray<DWORD>& _array, DWORD& _target, DWORD _start_index);
	DWORD FIXAPI QSIMDFastSearchArrayItemOffsetQWORD(BSTArray<UINT64>& _array, UINT64& _target, DWORD _start_index);

	inline DWORD FIXAPI QSIMDFastSearchArrayItemDWORD(BSTArray<DWORD>& _array, DWORD& _target) {
		return QSIMDFastSearchArrayItemOffsetDWORD(_array, _target, 0);
	}

	inline DWORD FIXAPI QSIMDFastSearchArrayItemQWORD(BSTArray<UINT64>& _array, UINT64& _target) {
		return QSIMDFastSearchArrayItemOffsetQWORD(_array, _target, 0);
	}
}