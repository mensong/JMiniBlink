// stdafx.cpp : 只包括标准包含文件的源文件
// MiniBlinkHost.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

std::string ReadText(const wchar_t * path)
{
	std::vector<char> vctSZ;
	WCharToMByte(path, wcslen(path), &vctSZ, CP_ACP);
	if (vctSZ.size() < 1)
		return "";
	vctSZ.push_back('\0');

	FILE *f = NULL;
	long sz;

	if (!path)
	{
		return "";
	}

	std::string sRet;

	f = fopen(vctSZ.data(), "rb");
	if (!f)
	{
		return "";
	}

	do
	{
		if (fseek(f, 0, SEEK_END) < 0)
		{
			break;
		}

		sz = ftell(f);
		if (sz < 0)
		{
			break;
		}

		if (fseek(f, 0, SEEK_SET) < 0)
		{
			break;
		}

		sRet.resize((size_t)sz + 1, '\0');

		if ((size_t)fread(const_cast<char*>(sRet.c_str()), 1, (size_t)sz, f) != (size_t)sz)
		{
			sRet = "";
			break;
		}
	} while (0);

	fclose(f);

	return sRet;
}

void MByteToWChar(LPCSTR lpcszStr, DWORD cbMultiByte, std::vector<wchar_t>* out, UINT codePage)
{
	out->clear();

	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(codePage, 0, lpcszStr, cbMultiByte, NULL, 0);
	if (0 == dwMinSize)
		return;

	out->resize(dwMinSize);

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar(codePage, 0, lpcszStr, cbMultiByte, &out->at(0), dwMinSize);
}

void WCharToMByte(LPCWSTR lpWideCharStr, DWORD cchWideChar, std::vector<char>* out, UINT codePage)
{
	out->clear();

	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(codePage, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, FALSE);
	if (0 == dwMinSize)
		return;

	out->resize(dwMinSize);

	// Convert headers from ASCII to Unicode.
	WideCharToMultiByte(codePage, 0, lpWideCharStr, cchWideChar, &out->at(0), dwMinSize, NULL, FALSE);
}

std::wstring AnsiToUnicode(const std::string& Ansi)
{
	std::wstring Result(L"");

	if (Ansi.empty())
	{
		return Result;
	}

	int nChar = MultiByteToWideChar(CP_ACP, 0, Ansi.c_str(), -1, NULL, 0);

	int nSize = nChar * sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(CP_ACP, 0, Ansi.c_str(), -1, pwchBuffer, nSize);
	if (nChar == 0)
	{
		delete[] pwchBuffer;
		return Result;
	}
	pwchBuffer[nChar] = 0;
	Result = pwchBuffer;

	delete[] pwchBuffer;

	return Result;
}

std:: string UnicodeToAnsi(const std::wstring& Unicode)
{
	std::string Result("");

	if (Unicode.empty())
	{
		return Result;
	}

	int nChar = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, 0, 0, 0, 0);
	int nSize = nChar + 1;

	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, pchBuffer, nSize, 0, 0);
	if (nChar == 0)
	{
		delete[] pchBuffer;
		return Result;
	}

	pchBuffer[nChar] = 0;
	Result = pchBuffer;

	delete[] pchBuffer;

	return Result;
}