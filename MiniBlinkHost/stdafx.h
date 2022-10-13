// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <map>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

void MByteToWChar(LPCSTR lpcszStr, DWORD cbMultiByte, std::vector<wchar_t>* out, UINT codePage);
void WCharToMByte(LPCWSTR lpWideCharStr, DWORD cchWideChar, std::vector<char>* out, UINT codePage);

std::wstring AnsiToUnicode(const std::string& Ansi);
std::string UnicodeToAnsi(const std::wstring& Unicode);

std::string ReadText(const wchar_t * path);