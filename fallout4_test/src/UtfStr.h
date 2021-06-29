#pragma once

#include "config.h"

#include <atomic>
#include <string>

#define UTILAPI WINAPI

namespace XUtil
{
	namespace Conversion
	{
		typedef std::basic_string<char> utf8string;
		typedef std::basic_string<wchar_t> utf16string;

#if FALLOUT4_LAZ_UNICODE_PLUGIN
		bool UTILAPI LazUnicodePluginInit(void);
#endif // !FALLOUT4_LAZ_UNICODE_PLUGIN

		bool UTILAPI IsUtf8Valid(const utf8string &str);
		utf8string UTILAPI Utf16ToUtf8(const utf16string &str);
		utf16string UTILAPI Utf8ToUtf16(const utf8string &str);
		std::string UTILAPI Utf8ToAnsi(const utf8string &str);
		utf8string UTILAPI AnsiToUtf8(const std::string &str);
		std::string UTILAPI Utf16ToAnsi(const utf16string &str);
		utf16string UTILAPI AnsiToUtf16(const std::string &str);
		std::string UTILAPI WideToAnsi(const std::wstring &str);
	    std::wstring UTILAPI AnsiToWide(const std::string &str);
	}
}
