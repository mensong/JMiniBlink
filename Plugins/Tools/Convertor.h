#pragma once
#include <string>

namespace GL
{

	//ת������
	// out : ����ת��
	// pIn : ����
	// tocode : Ŀ�����(�鿴win_iconv.c�ļ�)
	// fromcode : Դ����(�鿴win_iconv.c�ļ�)
	bool Convert(std::string &out, const char *pIn, const char *tocode, const char *fromcode);
	std::string Convert(const char *pIn, const char *tocode, const char *fromcode);

	//unicode תΪ ansi
	bool WideByte2Ansi(std::string &out, const wchar_t* wstrcode, const char* locale = "");
	std::string WideByte2Ansi(const wchar_t* wstrcode, const char* locale = "");

	//ansi ת Unicode
	bool Ansi2WideByte(std::wstring& out, const char* strAnsi, const char* locale = "");
	std::wstring Ansi2WideByte(const char* strAnsi, const char* locale = "");

	//Unicode ת UTF8
	bool WideByte2Utf8(std::string& out, const wchar_t* src, const char* locale = "");
	std::string WideByte2Utf8(const wchar_t* src, const char* locale = "");

	//UTF8 ת Unicode
	bool Utf82WideByte(std::wstring& out, const char* src, const char* locale = "");
	std::wstring Utf82WideByte(const char* src, const char* locale = "");

	//ansiתUTF8
	bool Ansi2Utf8(std::string& out, const char* strAnsi, const char* locale = "");
	std::string Ansi2Utf8(const char* strAnsi, const char* locale = "");

	//UTF8תansi
	bool Utf82Ansi(std::string& out, const char* strUtf8, const char* locale = "");
	std::string Utf82Ansi(const char* strUtf8, const char* locale = "");

	std::string StrToHex(const char* strData);
	std::string HexToStr(const char* strHex);
	std::wstring StrToHex(const wchar_t* strData);
	std::wstring HexToStr(const wchar_t* strHex);

}
