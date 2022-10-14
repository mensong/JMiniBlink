#pragma once
#include <string>
#include <vector>

class FileHelper
{
public:
	/*
	 * 读文件
	 * outContent ： 返回文件内容
	 * in_outLen ： in : outContent的内存长度 out:已读取到outContent的内容长度
	 * start ： 读取开始位置
	 * 返回：文件长度
	 */
	static size_t ReadFile(const char* path, char outContent[], size_t& in_outLen, size_t start = 0);
	static size_t ReadFile(const char* path, std::vector<char>& data, size_t start = 0);
	/*
	 * 读取文件
	 * 返回字符串
	 */
	static std::string ReadText(const char* path);

	/*
	 * 写文件
	 * path : 文件路径
	 * writeContent ： 写内容
	 * inLen ： writeContent的内存长度
	 * writenLen: 已写内容长度
	 * start ： 开始写的位置  -1:在当前文件的末尾开始写  当start大于当前文件长度，则文件空白处使用'\0'填充
	 * bInsert ： 当start小于当前文件长度时，bInsert为true时，插入的方式；bInsert为false时，覆盖的方式
	 * bDelTail : 是否删除末尾内容。当bInsert为false时才有效
	 * 返回：写入后文件总长度
	 */
	static size_t WriteFile(const char* path, const char *writeContent, size_t inLen, 
		size_t* writenLen = NULL, int start = -1, bool bInsert = true, bool bDelTail = true);

	static void ListFilesA(const char* lpPath, std::vector<std::string>* vctSubFiles = NULL, std::vector<std::string>* vctSubDirs = NULL, 
		const char* filter = "*.*", bool bSubDir = true, bool bAppendPath = true);
	static void ListFilesW(const wchar_t* lpPath, std::vector<std::wstring>* vctSubFiles = NULL, std::vector<std::wstring>* vctSubDirs = NULL,  
		const wchar_t* filter = L"*.*", bool bSubDir = true, bool bAppendPath = true);

	static bool IsSamePathA(const char* path1, const char* path2);
	static bool IsSamePathW(const wchar_t* path1, const wchar_t* path2);

	static std::string GetDirFromPathA(const std::string& sPath);
	static std::wstring GetDirFromPathW(const std::wstring& sPath);
	static std::string GetFileNameFromPathA(const std::string& sPath);
	static std::wstring GetFileNameFromPathW(const std::wstring& sPath);
	static std::string GetFileNameWithoutExtensionFromPathA(const std::string& sPath);
	static std::wstring GetFileNameWithoutExtensionFromPathW(const std::wstring& sPath);
	//跟据文件路径获得扩展名，不带“.”
	static std::string GetFileExtensionFromPathA(const std::string& sPath);
	static std::wstring GetFileExtensionFromPathW(const std::wstring& sPath);

	//目录是否存在的检查：
	static bool IsFolderExistA(const std::string &strPath);
	static bool IsFolderExistW(const std::wstring &strPath);
	//文件是否存在
	static bool IsFileExitstA(const std::string &filePath);
	static bool IsFileExitstW(const std::wstring &filePath);

	//创建文件夹（可创建多重文件夹）
	static bool CreateMultipleDirectoryA(const char* szPath);
	static bool CreateMultipleDirectoryW(const wchar_t* szPath);

	static bool RemoveDirA(const char* dirPath);
	static bool RemoveDirW(const wchar_t* dirPath);

	//检查字符串是否可做为文件（夹）名
	static std::string ConvertLegalFileNameA(const char* szStr);
	static std::wstring ConvertLegalFileNameW(const wchar_t* szStr);
};

