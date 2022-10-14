#include "stdafx.h"
#include "Convertor.h"
#include "iconv.h"
#if USE_WINDOWS
#include <Windows.h>
#else
#include <stdlib.h>
#endif

namespace GL
{

bool Convert(std::string &out, const char *pIn, const char *tocode, const char *fromcode)
{
	if (pIn == NULL)
		return false;

	bool bRet = false;

	if ((size_t)pIn >= (size_t)(out.c_str()) && (size_t)pIn <= (size_t)(out.c_str()) + ((size_t)(out.size()) + sizeof(char)))//out.c_str() != pIn
	{//输出和输入是同一块内存
		std::string sIn = pIn;
		iconv_t cd = iconv_open(tocode, fromcode);
		if (cd != (iconv_t)-1)
		{
			size_t srcLen = sIn.length() + 1;
			size_t destLen = (srcLen - 1) * 4;
			const char *pInTemp = sIn.c_str();
			out.resize(destLen, '\0');
			char *pOut = const_cast<char *>(out.c_str());
			if (iconv(cd, &pInTemp, &srcLen, &pOut, &destLen) != -1)
			{
				out.resize(strlen(out.c_str()));
				bRet = true;
			}
			else
			{
				printf("errno=%d (%s)\n", errno, strerror(errno));
			}
			iconv_close(cd);
		}
	}
	else
	{//输出和输入不是同一块内存
		iconv_t cd = iconv_open(tocode, fromcode);
		if (cd != (iconv_t)-1)
		{
			size_t srcLen = strlen(pIn) + 1;
			size_t destLen = (srcLen - 1) * 4;
			out.resize(destLen, '\0');
			char *pOut = const_cast<char *>(out.c_str());
			const char *pInx = pIn;//必须的
			if (iconv(cd, &pInx, &srcLen, &pOut, &destLen) != -1)
			{
				out.resize(strlen(out.c_str()));
				bRet = true;
			}
			else
			{
				printf("errno=%d (%s)\n", errno, strerror(errno));
			}
			iconv_close(cd);
		}
	}

	return bRet;
}

std::string Convert(const char *pIn, const char *tocode, const char *fromcode)
{
	std::string s;
	Convert(s, pIn, tocode, fromcode);
	return s;
}

//unicode 转为 ansi
bool WideByte2Ansi(std::string &out, const wchar_t* wstrcode, const char* locale/* = ""*/)
{
	if (wstrcode == NULL)
		return false;

#if USE_WINDOWS
	int ansiSize = WideCharToMultiByte(CP_OEMCP, 0, wstrcode, -1, NULL, 0, NULL, NULL);
	if (ansiSize == ERROR_NO_UNICODE_TRANSLATION)
	{
		//throw std::exception("Invalid UTF-8 sequence.");
		return false;
	}
	if (ansiSize == 0)
	{
		//throw std::exception("Error in conversion.");
		return false;
	}

	out.resize(ansiSize);
	int convresult = WideCharToMultiByte(CP_OEMCP, 0, wstrcode, -1, const_cast<char *>(out.c_str()), ansiSize, NULL, NULL);
	if (convresult != ansiSize)
	{
		//throw std::exception("La falla!");
		return false;
	}

	return true;

#else

	size_t nLen = wcslen(wstrcode) * 2;
	if (nLen == 0)
	{
		out.resize(0);
		return true;
	}

	setlocale(LC_ALL, locale);
	char* ar = new char[nLen + 1];
	memset(ar, 0, nLen + 1);
	size_t read = wcstombs(ar, wstrcode, nLen);
	if (read == (size_t)-1)
		out.resize(out.length());
	else
	{
		ar[read] = '\0';
		out = ar;
		out.resize(read);
	}

	delete[] ar;
	setlocale(LC_ALL, "");

	return (read != (size_t)-1);

#endif
}

std::string WideByte2Ansi(const wchar_t* wstrcode, const char* locale /*= ""*/)
{
	std::string s;
	WideByte2Ansi(s, wstrcode, locale);
	return s;
}

//ansi 转 Unicode
bool Ansi2WideByte(std::wstring& out, const char* strAnsi, const char* locale/* = ""*/)
{
	if (strAnsi == NULL)
		return false;

#if USE_WINDOWS
	int widesize = MultiByteToWideChar(CP_ACP, 0, strAnsi, -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		//throw std::exception("Invalid UTF-8 sequence.");
		return false;
	}
	if (widesize == 0)
	{
		//throw std::exception("Error in conversion.");
		return false;
	}

	out.resize(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, strAnsi, -1, const_cast<wchar_t*>(out.c_str()), widesize);
	if (convresult != widesize)
	{
		//throw std::exception("La falla!");
		return false;
	}

	return true;
#else

	size_t nLen = strlen(strAnsi);
	if (nLen == 0)
	{
		out.resize(0);
		return true;
	}

	setlocale(LC_ALL, locale);
	wchar_t* ar = new wchar_t[nLen + 1];
	memset(ar, 0, nLen + 1);
	size_t read = mbstowcs(ar, strAnsi, nLen);
	if (read == (size_t)-1)
		out.resize(out.length());
	else
	{
		ar[read] = '\0';
		out = ar;
		out.resize(read);
	}

	delete[] ar;
	setlocale(LC_ALL, "");

	return (read != (size_t)-1);

#endif
}

std::wstring Ansi2WideByte(const char* strAnsi, const char* locale /*= ""*/)
{
	std::wstring ws;
	Ansi2WideByte(ws, strAnsi, locale);
	return ws;
}

//Unicode 转 UTF8
bool WideByte2Utf8(std::string& out, const wchar_t* src, const char* locale/* = ""*/)
{
	WideByte2Ansi(out, src, locale);
	return Convert(out, out.c_str(), "UTF-8", "GBK");
}

std::string WideByte2Utf8(const wchar_t* src, const char* locale /*= ""*/)
{
	std::string s;
	WideByte2Utf8(s, src, locale);
	return s;
}

//UTF8 转 Unicode
bool Utf82WideByte(std::wstring& out, const char* src, const char* locale/* = ""*/)
{
	std::string s;
	bool b = Convert(s, src, "GBK", "UTF-8");
	Ansi2WideByte(out, s.c_str(), locale);
	return b;
}

std::wstring Utf82WideByte(const char* src, const char* locale /*= ""*/)
{
	std::wstring ws;
	Utf82WideByte(ws, src, locale);
	return ws;
}

bool Ansi2Utf8(std::string& out, const char* strAnsi, const char* locale/* = ""*/)
{
	std::wstring ws;
	Ansi2WideByte(ws, strAnsi, locale);
	return WideByte2Utf8(out, ws.c_str(), locale);
}

std::string Ansi2Utf8(const char* strAnsi, const char* locale /*= ""*/)
{
	std::string s;
	Ansi2Utf8(s, strAnsi, locale);
	return s;
}

bool Utf82Ansi(std::string& out, const char* strUtf8, const char* locale/* = ""*/)
{
	std::wstring ws;
	Utf82WideByte(ws, strUtf8, locale);
	return WideByte2Ansi(out, ws.c_str(), locale);
}

std::string Utf82Ansi(const char* strUtf8, const char* locale /*= ""*/)
{
	std::string s;
	Utf82Ansi(s, strUtf8, locale);
	return s;
}

std::string StrToHex(const char* strData)
{
	if (strData == NULL)
		return "";

	//初始化
	std::string strTemp = "";
	int iLenth = strlen(strData);
	int nNewSize = (iLenth + 1) * 2;
	char* cArrTemp = new char[nNewSize];
	memset(cArrTemp, 0, nNewSize);

	for (int i = 0; i < iLenth; i++)
	{
		if (strData[i] == 0)
		{
			break;
		}
		sprintf(cArrTemp + (i * 2), "%02X", (unsigned char)strData[i]);
	}

	strTemp = cArrTemp;
	strTemp.resize(iLenth * 2);
	delete[] cArrTemp;

	return strTemp;
}

std::string HexToStr(const char* strHex)
{
	if (strHex == NULL)
		return "";

	std::string strTemp = "";
	int iLenth = strlen(strHex);
	char *cArrTemp = new char[iLenth + 1];
	memset(cArrTemp, 0, iLenth + 1);

	int iOcx;
	for (int i = 0; i < iLenth / 2; i++)
	{
		if (1 != sscanf(strHex + (i * 2), "%2x", (unsigned int*)&iOcx))
			break;
		cArrTemp[i] = (char)iOcx;
	}

	strTemp = cArrTemp;
	strTemp.resize(iLenth / 2);

	delete[] cArrTemp;

	return strTemp;
}

std::wstring StrToHex(const wchar_t* strData)
{
	if (strData == NULL)
		return L"";

	std::string ansi;
	WideByte2Ansi(ansi, strData);
	ansi = StrToHex(ansi.c_str());
	std::wstring ws;
	Ansi2WideByte(ws, ansi.c_str());
	return ws;
}

std::wstring HexToStr(const wchar_t* strHex)
{
	if (strHex == NULL)
		return L"";

	std::string ansi;
	WideByte2Ansi(ansi, strHex);
	ansi = HexToStr(ansi.c_str());
	std::wstring ws;
	Ansi2WideByte(ws, ansi.c_str());
	return ws;
}

}