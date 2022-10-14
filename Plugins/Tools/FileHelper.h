#pragma once
#include <string>
#include <vector>

class FileHelper
{
public:
	/*
	 * ���ļ�
	 * outContent �� �����ļ�����
	 * in_outLen �� in : outContent���ڴ泤�� out:�Ѷ�ȡ��outContent�����ݳ���
	 * start �� ��ȡ��ʼλ��
	 * ���أ��ļ�����
	 */
	static size_t ReadFile(const char* path, char outContent[], size_t& in_outLen, size_t start = 0);
	static size_t ReadFile(const char* path, std::vector<char>& data, size_t start = 0);
	/*
	 * ��ȡ�ļ�
	 * �����ַ���
	 */
	static std::string ReadText(const char* path);

	/*
	 * д�ļ�
	 * path : �ļ�·��
	 * writeContent �� д����
	 * inLen �� writeContent���ڴ泤��
	 * writenLen: ��д���ݳ���
	 * start �� ��ʼд��λ��  -1:�ڵ�ǰ�ļ���ĩβ��ʼд  ��start���ڵ�ǰ�ļ����ȣ����ļ��հ״�ʹ��'\0'���
	 * bInsert �� ��startС�ڵ�ǰ�ļ�����ʱ��bInsertΪtrueʱ������ķ�ʽ��bInsertΪfalseʱ�����ǵķ�ʽ
	 * bDelTail : �Ƿ�ɾ��ĩβ���ݡ���bInsertΪfalseʱ����Ч
	 * ���أ�д����ļ��ܳ���
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
	//�����ļ�·�������չ����������.��
	static std::string GetFileExtensionFromPathA(const std::string& sPath);
	static std::wstring GetFileExtensionFromPathW(const std::wstring& sPath);

	//Ŀ¼�Ƿ���ڵļ�飺
	static bool IsFolderExistA(const std::string &strPath);
	static bool IsFolderExistW(const std::wstring &strPath);
	//�ļ��Ƿ����
	static bool IsFileExitstA(const std::string &filePath);
	static bool IsFileExitstW(const std::wstring &filePath);

	//�����ļ��У��ɴ��������ļ��У�
	static bool CreateMultipleDirectoryA(const char* szPath);
	static bool CreateMultipleDirectoryW(const wchar_t* szPath);

	static bool RemoveDirA(const char* dirPath);
	static bool RemoveDirW(const wchar_t* dirPath);

	//����ַ����Ƿ����Ϊ�ļ����У���
	static std::string ConvertLegalFileNameA(const char* szStr);
	static std::wstring ConvertLegalFileNameW(const wchar_t* szStr);
};

