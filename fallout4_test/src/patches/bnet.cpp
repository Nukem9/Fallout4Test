//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
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

#include "../common.h"

size_t FIXAPI BNetConvertUnicodeString(char *Destination, size_t DestSize, const wchar_t *Source, size_t SourceSize)
{
	if (Destination)
		memset(Destination, 0, DestSize);

	size_t outLen;
	errno_t result = wcsrtombs_s(&outLen, Destination, DestSize, &Source, SourceSize, nullptr);

	if (result != 0)
	{
		// Try a fall back instead of normally returning -1
		outLen = WideCharToMultiByte(CP_UTF8, 0, Source, SourceSize, Destination, DestSize, nullptr, nullptr);

		if (Destination && DestSize > 0)
			Destination[DestSize - 1] = '\0';

		AssertMsg(outLen != 0, "Unicode conversion failed");
	}

	return outLen - 1;
}