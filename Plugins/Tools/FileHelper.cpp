#include "stdafx.h"
#include "FileHelper.h"
#include <Windows.h>
#include <algorithm>
#include <set>
#include <direct.h>
#include "MixedHelper.h"
#include <io.h>

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif

size_t FileHelper::ReadFile(const char * path, char outContent[], size_t & in_outLen, size_t start/*=0*/)
{
	FILE *f = NULL;
	long sz;
	size_t nFileSize = 0;

	if (!path)
	{
		return 0;
	}
	
	f = fopen(path, "rb");
	if (!f)
	{
		return 0;
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
		nFileSize = (size_t)sz;

		if (fseek(f, start, SEEK_SET) < 0)
		{
			break;
		}

		size_t nToRead = in_outLen;
		if (nToRead == 0)
		{
			nToRead = (size_t)sz;
		}

		size_t nReaded = fread(outContent, 1, nToRead, f);
		if (nReaded > nToRead)
		{
			break;
		}

		in_outLen = nReaded;

	} while (0);

	fclose(f);

	return nFileSize;
}

size_t FileHelper::ReadFile(const char* path, std::vector<char>& data, size_t start /*= 0*/)
{
	FILE *f = NULL;
	long sz;
	size_t nToRead = 0;
	size_t nFileSize = 0;

	if (!path)
	{
		return 0;
	}

	f = fopen(path, "rb");
	if (!f)
	{
		return 0;
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
		nFileSize = (size_t)sz;
		if (start >= nFileSize)
		{
			break;
		}

		if (fseek(f, start, SEEK_SET) < 0)
		{
			break;
		}

		nToRead = nFileSize - start;
		data.resize(nToRead);
		
		size_t nReaded = fread(&(data[0]), 1, nToRead, f);
		if (nReaded > nToRead)
		{
			nToRead = nReaded;
			break;
		}
	} while (0);

	fclose(f);

	return nFileSize;
}

std::string FileHelper::ReadText(const char * path)
{
	FILE *f = NULL;
	long sz; 

	if (!path)
	{
		return "";
	}

	std::string sRet;

	f = fopen(path, "rb");
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

size_t FileHelper::WriteFile(const char* path, const char *writeContent, size_t inLen,
	size_t* writenLen/* = NULL*/, int start/* = -1*/, bool bInsert/* = true*/, bool bDelTail/* = true*/)
{
	if (!path)
	{
		return -1;
	}

	FILE *f = NULL;
	if ((f = fopen(path, "rb+")) == NULL)
	{
		//文件不存在，则新建一个空的
		if ((f = fopen(path, "wb")) == NULL)
		{
			return -1;
		}
		fclose(f);

		if ((f = fopen(path, "rb+")) == NULL)
			return -1;
	}

	do
	{
		int nFileSize = 0;
		if (fseek(f, 0, SEEK_END) != 0)
		{
			nFileSize = 0;
		}
		else
		{
			nFileSize = ftell(f);
		}
		if (nFileSize < 0)
		{
			break;
		}

		char* pOldStart = NULL;
		char* pOldEnd = NULL;
		if (start > -1)
		{
			if (nFileSize < start)
			{//插入处比原有的文件大，则在原文件尾到插入处空白的这些位置填充NULL
				int nSpace = start - nFileSize + 1;
				char *pSpace = new char[nSpace];
				memset(pSpace, 0, nSpace);
				fwrite(pSpace, nSpace, 1, f);
				delete[] pSpace;
			}
			else if (bInsert && (nFileSize > start))
			{//在中间插入要写的内容
				//读取插入处到原文件结尾处的内容，以回写
				int nRead = (nFileSize - start);
				fseek(f, start, SEEK_SET);
				pOldEnd = new char[nRead];
				fread(pOldEnd, 1, nRead, f);
			}
			else if (bDelTail && (nFileSize > start) && (inLen < nFileSize - start))
			{//回写插入处前面的内容，而丢弃插入处后面的内容
				//读取插入处前面的内容
				if (start > 0)
				{
					fseek(f, 0, SEEK_SET);
					pOldStart = new char[start];
					fread(pOldStart, 1, start, f);
				}			

				//删除原有的文件，新建一个空的
				fclose(f);
				remove(path);
				if ((f = fopen(path, "wb")) == NULL)
				{
					return -1;
				}
				//回写插入处前面的内容，而丢弃插入处后面的内容
				if (start > 0)
					fwrite(pOldStart, 1, start, f);
			}

			if (fseek(f, start, SEEK_SET) != 0)
			{
				if (pOldEnd)
				{
					delete[] pOldEnd;
				}
				break;
			}
		}

		size_t writen = fwrite(writeContent, 1, inLen, f);
		if (writenLen)
			*writenLen = writen;

		if (pOldEnd)
		{
			fwrite(pOldEnd, 1, (nFileSize - start), f);
			delete[] pOldEnd;
		}

	} while (0);

	int nFileSize = 0;
	if (fseek(f, 0, SEEK_END) != 0)
	{
		nFileSize = -1;
	}
	else
	{
		nFileSize = ftell(f);
	}

	fclose(f);
	return nFileSize;
}

void FileHelper::ListFilesA(const char* lpPath, std::vector<std::string>* vctSubFiles/*=NULL*/, std::vector<std::string>* vctSubDirs/*=NULL*/, const char* filter /*= "*.*"*/, bool bSubDir /*= true*/, bool bAppendPath /*= true*/)
{
	char szFind[MAX_PATH] = { 0 };
	lstrcpyA(szFind, lpPath);
	char ec = lpPath[strlen(lpPath) - 1];
	if (ec != '/' && ec != '\\')
		lstrcatA(szFind, "\\");
	lstrcatA(szFind, filter);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		//int n = GetLastError();
		return;
	}

	char szFile[MAX_PATH] = { 0 };
	while (TRUE)
	{
		szFile[0] = '\0';
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				if (bAppendPath)
				{
					lstrcpyA(szFile, lpPath);
					char ec = lpPath[strlen(lpPath) - 1];
					if (ec != '/' && ec != '\\')
						lstrcatA(szFile, "\\");
					lstrcatA(szFile, FindFileData.cFileName);
				}
				else
				{
					lstrcpyA(szFile, FindFileData.cFileName);
				}
				if (vctSubDirs)
					vctSubDirs->push_back(szFile);

				if (bSubDir)
				{
					ListFilesA(szFile, vctSubFiles, vctSubDirs, filter, bSubDir, bAppendPath);
				}
			}
		}
		else
		{
			if (bAppendPath)
			{
				lstrcpyA(szFile, lpPath);
				char ec = lpPath[strlen(lpPath) - 1];
				if (ec != '/' && ec != '\\')
					lstrcatA(szFile, "\\");
				lstrcatA(szFile, FindFileData.cFileName);
			}
			else
			{
				lstrcpyA(szFile, FindFileData.cFileName);
			}
			if (vctSubFiles)
				vctSubFiles->push_back(szFile);
		}

		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

void FileHelper::ListFilesW(const wchar_t* lpPath, std::vector<std::wstring>* vctSubFiles/*=NULL*/, std::vector<std::wstring>* vctSubDirs/*=NULL*/, const wchar_t* filter /*= L"*.*"*/, bool bSubDir /*= true*/, bool bAppendPath /*= true*/)
{
	wchar_t szFind[MAX_PATH];
	lstrcpyW(szFind, lpPath);
	wchar_t ec = lpPath[wcslen(lpPath) - 1];
	if (ec != '/' && ec != '\\')
		lstrcatW(szFind, L"\\");
	lstrcatW(szFind, filter);

	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = ::FindFirstFileW(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	wchar_t szFile[MAX_PATH];
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				if (bAppendPath)
				{
					lstrcpyW(szFile, lpPath);
					wchar_t ec = lpPath[wcslen(lpPath) - 1];
					if (ec != '/' && ec != '\\')
						lstrcatW(szFile, L"\\");
					lstrcatW(szFile, FindFileData.cFileName);
				}
				else
				{
					lstrcpyW(szFile, FindFileData.cFileName);
				}
				if (vctSubDirs)
					vctSubDirs->push_back(szFile);

				if (bSubDir)
				{
					ListFilesW(szFile, vctSubFiles, vctSubDirs, filter, bSubDir, bAppendPath);
				}
			}
		}
		else
		{
			if (bAppendPath)
			{
				lstrcpyW(szFile, lpPath);
				wchar_t ec = lpPath[wcslen(lpPath) - 1];
				if (ec != '/' && ec != '\\')
					lstrcatW(szFile, L"\\");
				lstrcatW(szFile, FindFileData.cFileName);
			}
			else
			{
				lstrcpyW(szFile, FindFileData.cFileName);
			}
			if (vctSubFiles)
				vctSubFiles->push_back(szFile);
		}

		if (!FindNextFileW(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

bool FileHelper::IsSamePathA(const char * path1, const char * path2)
{
	std::string s1 = path1;
	std::string s2 = path2;
	std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

	MixedHelper::StringReplaceA(s1, "\\", "/");
	MixedHelper::StringReplaceA(s1, "//", "/");
	MixedHelper::StringReplaceA(s2, "\\", "/");
	MixedHelper::StringReplaceA(s2, "//", "/");

	if (s1.size() > 0 && s1[s1.size() - 1] == '/')
		s1[s1.size() - 1] = '\0';
	if (s2.size() > 0 && s2[s2.size() - 1] == '/')
		s2[s2.size() - 1] = '\0';

	return (strcmp(s1.c_str(), s2.c_str()) == 0);
}

bool FileHelper::IsSamePathW(const wchar_t * path1, const wchar_t * path2)
{
	std::wstring s1 = path1;
	std::wstring s2 = path2;
	std::transform(s1.begin(), s1.end(), s1.begin(), ::towupper);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::towupper);

	MixedHelper::StringReplaceW(s1, L"\\", L"/");
	MixedHelper::StringReplaceW(s1, L"//", L"/");
	MixedHelper::StringReplaceW(s2, L"\\", L"/");
	MixedHelper::StringReplaceW(s2, L"//", L"/");

	if (s1.size() > 0 && s1[s1.size() - 1] == '/')
		s1[s1.size() - 1] = '\0';
	if (s2.size() > 0 && s2[s2.size() - 1] == '/')
		s2[s2.size() - 1] = '\0';

	return (wcscmp(s1.c_str(), s2.c_str()) == 0);
}

std::string FileHelper::GetDirFromPathA(const std::string & sPath)
{
	std::string path = sPath;
	char c = path[path.size() - 1];
	if (c == '\\' || c == '/')
	{
		path = path.substr(0, path.size() - 1);
	}

	size_t split1 = path.find_last_of('/');
	size_t split2 = path.find_last_of('\\');
	size_t split = 0;
	if (split1 == std::string::npos)
		split = split2;
	else if (split2 == std::string::npos)
		split = split1;
	else
		split = max(split1, split2);

	if (std::string::npos != split)
	{
		path = path.substr(0, split);
	}

	return path.c_str();
}

std::wstring FileHelper::GetDirFromPathW(const std::wstring & sPath)
{
	std::wstring path = sPath;
	wchar_t c = path[path.size() - 1];
	if (c == '\\' || c == '/')
	{
		path = path.substr(0, path.size() - 1);
	}

	size_t split1 = path.find_last_of('/');
	size_t split2 = path.find_last_of('\\');
	size_t split = 0;
	if (split1 == std::wstring::npos)
		split = split2;
	else if (split2 == std::wstring::npos)
		split = split1;
	else
		split = max(split1, split2);

	if (std::wstring::npos != split)
	{
		path = path.substr(0, split);
	}

	return path.c_str();
}

std::string FileHelper::GetFileNameFromPathA(const std::string & sPath)
{
	size_t split1 = sPath.find_last_of('/');
	size_t split2 = sPath.find_last_of('\\');
	size_t split = 0;
	if (split1 == std::string::npos)
		split = split2;
	else if (split2 == std::string::npos)
		split = split1;
	else
		split = max(split1, split2);

	if (std::string::npos == split)
		return sPath;

	return sPath.substr(split + 1).c_str();
}

std::wstring FileHelper::GetFileNameFromPathW(const std::wstring & sPath)
{
	size_t split1 = sPath.find_last_of('/');
	size_t split2 = sPath.find_last_of('\\');
	size_t split = 0;
	if (split1 == std::wstring::npos)
		split = split2;
	else if (split2 == std::wstring::npos)
		split = split1;
	else
		split = max(split1, split2);

	if (std::wstring::npos == split)
		return sPath;

	return sPath.substr(split + 1).c_str();
}

std::string FileHelper::GetFileNameWithoutExtensionFromPathA(const std::string & sPath)
{
	std::string fileName = GetFileNameFromPathA(sPath).c_str();

	size_t dot = fileName.find_last_of('.');
	if (dot == 0)
		return "";
	else if (dot == std::string::npos)
		return fileName.c_str();
	else
		return fileName.substr(0, dot).c_str();

	return "";
}

std::wstring FileHelper::GetFileNameWithoutExtensionFromPathW(const std::wstring & sPath)
{
	std::wstring fileName = GetFileNameFromPathW(sPath).c_str();

	size_t dot = fileName.find_last_of('.');
	if (dot == 0)
		return L"";
	else if (dot == std::wstring::npos)
		return fileName.c_str();
	else
		return fileName.substr(0, dot).c_str();

	return L"";
}

std::string FileHelper::GetFileExtensionFromPathA(const std::string & sPath)
{
	std::string fileName = GetFileNameFromPathA(sPath);

	size_t dot = fileName.find_last_of('.');
	if (dot == std::string::npos)
		return "";
	else
		return fileName.substr(dot + 1).c_str();

	return "";
}

std::wstring FileHelper::GetFileExtensionFromPathW(const std::wstring & sPath)
{
	std::wstring fileName = GetFileNameFromPathW(sPath);

	size_t dot = fileName.find_last_of('.');
	if (dot == std::wstring::npos)
		return L"";
	else
		return fileName.substr(dot + 1).c_str();

	return L"";
}

bool FileHelper::IsFolderExistA(const std::string &strPath)
{
	if (_access(strPath.c_str(), 0) == 0)
		return true;
	return false;
}

bool FileHelper::IsFolderExistW(const std::wstring &strPath)
{
	if (_waccess(strPath.c_str(), 0) == 0)
		return true;
	return false;
}

bool FileHelper::IsFileExitstA(const std::string & filePath)
{
	if (_access(filePath.c_str(), 0) == 0)
		return true;
	return false;
}

bool FileHelper::IsFileExitstW(const std::wstring & filePath)
{
	if (_waccess(filePath.c_str(), 0) == 0)
		return true;
	return false;
}

bool FileHelper::CreateMultipleDirectoryA(const char* szPath)
{
	std::string strDir(szPath);//存放要创建的目录字符串
	MixedHelper::StringReplaceA(strDir, "/", "\\");//把“/”转为“\”

	//确保以'\'结尾以创建最后一个目录
	if (strDir[strDir.length() - 1] != '\\')
	{
		strDir += '\\';
	}
	std::vector<std::string> vPath;//存放每一层目录字符串
	std::string strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
						  //遍历要创建的字符串
	for (int i = 0; i < strDir.length(); ++i)
	{
		if (strDir[i] != '\\')
		{//如果当前字符不是'\\'
			strTemp += strDir[i];
		}
		else
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp += '\\';
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<std::string>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectoryA(vIter->c_str(), NULL) ? true : false;
	}

	return bSuccess;
}

bool FileHelper::CreateMultipleDirectoryW(const wchar_t* szPath)
{
	std::wstring strDir(szPath);//存放要创建的目录字符串
	MixedHelper::StringReplaceW(strDir, L"/", L"\\");//把“/”转为“\”

	//确保以'\'结尾以创建最后一个目录
	if (strDir[strDir.length() - 1] != '\\')
	{
		strDir += '\\';
	}
	std::vector<std::wstring> vPath;//存放每一层目录字符串
	std::wstring strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i = 0; i < strDir.length(); ++i)
	{
		if (strDir[i] != '\\')
		{//如果当前字符不是'\\'
			strTemp += strDir[i];
		}
		else
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp += '\\';
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<std::wstring>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectoryW(vIter->c_str(), NULL) ? true : false;
	}

	return bSuccess;
}

bool FileHelper::RemoveDirA(const char* dirPath)
{
	std::vector<std::string> dirs;
	std::vector<std::string> files;
	ListFilesA(dirPath, &files, &dirs, "*", false, true);
	for (int i = 0; i < files.size(); ++i)
	{
		DeleteFileA(files[i].c_str());
	}

	for (int i = 0; i < dirs.size(); ++i)
	{
		RemoveDirA(dirs[i].c_str());
		rmdir(dirs[i].c_str());
	}

	return (0 == rmdir(dirPath));
}

bool FileHelper::RemoveDirW(const wchar_t* dirPath)
{
	std::vector<std::wstring> dirs;
	std::vector<std::wstring> files;
	ListFilesW(dirPath, &files, &dirs, L"*", false, true);
	for (int i = 0; i < files.size(); ++i)
	{
		DeleteFileW(files[i].c_str());
	}

	for (int i = 0; i < dirs.size(); ++i)
	{
		RemoveDirW(dirs[i].c_str());
		_wrmdir(dirs[i].c_str());
	}

	return (0 == _wrmdir(dirPath));
}

std::string FileHelper::ConvertLegalFileNameA(const char* szStr)
{
	std::string sRet;
	int nLen = strlen(szStr);
	for (int i=0; i<nLen; ++i)
	{
		char c = szStr[i];
		if (c == '<' || c == '>' || c == ':' || c == '"' || c == '/' 
			|| c == '\\' || c == '|' || c == '?' || c == '*'
			)//<>:"/\|?*
		{
			continue;
		}
		sRet += c;
	}

	std::string sTmp = sRet;
	size_t idx = sRet.find_last_of('.');
	if (idx != std::string::npos)
	{
		sTmp = sTmp.substr(0, idx);
	}

	std::transform(sTmp.begin(), sTmp.end(), sTmp.begin(), ::toupper);

	static std::set<std::string> g_inlegalFileNames;
	if (g_inlegalFileNames.size() == 0)
	{
		g_inlegalFileNames.insert("CON");
		g_inlegalFileNames.insert("PRN");
		g_inlegalFileNames.insert("AUX");
		g_inlegalFileNames.insert("NUL");
		g_inlegalFileNames.insert("COM0");
		g_inlegalFileNames.insert("COM1");
		g_inlegalFileNames.insert("COM2");
		g_inlegalFileNames.insert("COM3");
		g_inlegalFileNames.insert("COM4");
		g_inlegalFileNames.insert("COM5");
		g_inlegalFileNames.insert("COM6");
		g_inlegalFileNames.insert("COM7");
		g_inlegalFileNames.insert("COM8");
		g_inlegalFileNames.insert("COM9");
		g_inlegalFileNames.insert("LPT0");
		g_inlegalFileNames.insert("LPT1");
		g_inlegalFileNames.insert("LPT2");
		g_inlegalFileNames.insert("LPT3");
		g_inlegalFileNames.insert("LPT4");
		g_inlegalFileNames.insert("LPT5");
		g_inlegalFileNames.insert("LPT6");
		g_inlegalFileNames.insert("LPT7");
		g_inlegalFileNames.insert("LPT8");
		g_inlegalFileNames.insert("LPT9");
	}

	if (g_inlegalFileNames.find(sTmp) != g_inlegalFileNames.end())
		return "";

	if (sRet.size() >= 248)//windows下完全限定文件名必须少于260个字符，目录名必须小于248个字符。
		return "";

	return sRet;
}

std::wstring FileHelper::ConvertLegalFileNameW(const wchar_t* szStr)
{
	std::wstring sRet;
	int nLen = wcslen(szStr);
	for (int i=0; i<nLen; ++i)
	{
		wchar_t c = szStr[i];
		if (c == '<' || c == '>' || c == ':' || c == '"' || c == '/' 
			|| c == '\\' || c == '|' || c == '?' || c == '*'
			)//<>:"/\|?*
		{
			continue;
		}
		sRet += c;
	}

	std::wstring sTmp = sRet;
	size_t idx = sRet.find_last_of('.');
	if (idx != std::string::npos)
	{
		sTmp = sTmp.substr(0, idx);
	}

	std::transform(sTmp.begin(), sTmp.end(), sTmp.begin(), ::towupper);

	static std::set<std::wstring> g_inlegalFileNames;
	if (g_inlegalFileNames.size() == 0)
	{
		g_inlegalFileNames.insert(L"CON");
		g_inlegalFileNames.insert(L"PRN");
		g_inlegalFileNames.insert(L"AUX");
		g_inlegalFileNames.insert(L"NUL");
		g_inlegalFileNames.insert(L"COM0");
		g_inlegalFileNames.insert(L"COM1");
		g_inlegalFileNames.insert(L"COM2");
		g_inlegalFileNames.insert(L"COM3");
		g_inlegalFileNames.insert(L"COM4");
		g_inlegalFileNames.insert(L"COM5");
		g_inlegalFileNames.insert(L"COM6");
		g_inlegalFileNames.insert(L"COM7");
		g_inlegalFileNames.insert(L"COM8");
		g_inlegalFileNames.insert(L"COM9");
		g_inlegalFileNames.insert(L"LPT0");
		g_inlegalFileNames.insert(L"LPT1");
		g_inlegalFileNames.insert(L"LPT2");
		g_inlegalFileNames.insert(L"LPT3");
		g_inlegalFileNames.insert(L"LPT4");
		g_inlegalFileNames.insert(L"LPT5");
		g_inlegalFileNames.insert(L"LPT6");
		g_inlegalFileNames.insert(L"LPT7");
		g_inlegalFileNames.insert(L"LPT8");
		g_inlegalFileNames.insert(L"LPT9");
	}

	if (g_inlegalFileNames.find(sTmp) != g_inlegalFileNames.end())
		return L"";

	if (sRet.size() >= 248)//windows下完全限定文件名必须少于260个字符，目录名必须小于248个字符。
		return L"";

	return sRet;
}