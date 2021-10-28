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

#pragma once

#include "../../api/BSTList.h"

union SETTING_VALUE
{
	const char *str;
	int i;
	unsigned int u;
	float f;
	bool b;
	char c;
	unsigned char h;

	struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} rgba;
};

class Setting
{
public:
	enum SETTING_TYPE
	{
		ST_BINARY = 0x0,
		ST_CHAR = 0x1,
		ST_UCHAR = 0x2,
		ST_INT = 0x3,
		ST_UINT = 0x4,
		ST_FLOAT = 0x5,
		ST_STRING = 0x6,
		ST_RGB = 0x7,
		ST_RGBA = 0x8,
		ST_NONE = 0x9,
	};

	static SETTING_TYPE DataType(const char *Prefix);

	void GetAsString(char *Buffer, size_t BufferLen);
	bool SetFromString(const char *Input);

	virtual ~Setting();
	virtual bool IsPrefSetting();

	SETTING_VALUE uValue;
	const char *pKey;

	void operator=(const char *StringValue)
	{
		AutoFunc(void(__fastcall *)(Setting *, const char *), sub_140D282D0, 0xD282D0);
		sub_140D282D0(this, StringValue);
	}
};
static_assert_offset(Setting, uValue, 0x8);
static_assert_offset(Setting, pKey, 0x10);

template<typename T>
struct SettingT : public Setting
{
};

template<typename T>
class SettingCollection
{
protected:
	char pSettingFile[260];
	void *pHandle;
};

template<typename T>
class SettingCollectionList : public SettingCollection<T>
{
public:
	BSSimpleList<T *> SettingsA;

public:
	virtual ~SettingCollectionList();
	virtual void AddSetting(Setting *S);
	virtual void RemoveSetting(Setting *S);
	virtual bool WriteSetting(Setting *S) = 0;
	virtual bool ReadSetting(Setting *S) = 0;
	virtual bool Open(bool OpenDuringRead);
	virtual bool Close();
	virtual bool ReadSettingsFromProfile();
	virtual bool WriteSettings();
	virtual bool ReadSettings();
};
static_assert_offset(SettingCollectionList<Setting>, SettingsA, 0x118);

class INISettingCollection : public SettingCollectionList<Setting>
{
public:
	constexpr static uint32_t MAX_KEY_LENGTH = 64;
	constexpr static uint32_t MAX_SUBKEY_LENGTH = 512;

	bool hk_ReadSetting(Setting *S);
	bool hk_Open(bool OpenDuringRead);
	bool hk_Close();

	Setting *FindSetting(const char *Key) const;
	void DumpSettingIDAScript(FILE *File) const;

	void MainKey(const Setting *S, char *Buffer) const;
	void SubKey(const Setting *S, char *Buffer) const;
};

class INIPrefSettingCollection : public INISettingCollection
{
public:
};

AutoPtr(INISettingCollection *, INISettingCollectionSingleton, 0x3043758);
AutoPtr(INIPrefSettingCollection *, INIPrefSettingCollectionSingleton, 0x2F91A08);
#define DefineIniSetting(Name, Category) static SettingResolverHack Name(#Name ":" #Category)

//
// This doesn't exist in the game itself but I need a way to statically init things. DLLs
// have their static constructors called before the game does.
//
class SettingResolverHack
{
private:
	const char *m_Key;
	Setting *m_Ptr;

public:
	SettingResolverHack(const char *Key);
	Setting *operator ->();
};