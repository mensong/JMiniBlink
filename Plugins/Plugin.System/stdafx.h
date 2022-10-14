// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <string>
#include <map>
#include <vector>

#include <Convertor.h>
#include <MixedHelper.h>
#include <FileHelper.h>
#include <pystring.h>
#include <pystring.h>


// TODO:  在此处引用程序需要的其他头文件
void MByteToWChar(LPCSTR lpcszStr, DWORD cbMultiByte, std::vector<wchar_t>* out, UINT codePage);
void WCharToMByte(LPCWSTR lpWideCharStr, DWORD cchWideChar, std::vector<char>* out, UINT codePage);

std::wstring AnsiToUnicode(const std::string& Ansi);
std::string UnicodeToAnsi(const std::wstring& Unicode);

std::string ReadText(const wchar_t * path);