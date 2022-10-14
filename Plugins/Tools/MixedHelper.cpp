#include "stdafx.h"
#include "MixedHelper.h"
#include <cmath>

bool MixedHelper::IsDoubleEqual(const double &d1, const double &d2, const double &dTol /*= 0.0001*/)
{
	return fabs(d1 - d2) < (dTol == 0.0 ? 0.000001 : dTol);
}

bool MixedHelper::IsDoubleMoreThan(const double &d1, const double &d2, const double &dTol /*= 0.0001*/)
{
	if (IsDoubleEqual(d1, d2, dTol))
	{
		return false;
	}

	return d1 > d2;
}

bool MixedHelper::IsDoubleLessThan(const double &d1, const double &d2, const double &dTol /*= 0.0001*/)
{
	if (IsDoubleEqual(d1, d2, dTol))
	{
		return false;
	}

	return d1 < d2;
}

double MixedHelper::TakeDoubleBy(double d, unsigned int uTakeDecimalDigits, unsigned int uMinCarry /*= 5*/)
{
	double dPow = (double)(pow((float)10, (int)uTakeDecimalDigits));
	double dCarry = 1.001 - ((double)uMinCarry / 10.0);

	if (0 > d)
	{
		dCarry *= -1;
	}

	d *= dPow;
	d += dCarry;
	d = (double)(long long)d;
	d /= dPow;

	return d;
}

double MixedHelper::CarryMaxInteger(const double &d, unsigned int uTakeDecimalDigits)
{
	return TakeDoubleBy(d, uTakeDecimalDigits, 1);
}

double MixedHelper::GetDoubleTol(unsigned int uTakeDecimalDigits)
{
	switch (uTakeDecimalDigits)
	{
	case 0: return 0.1;
	case 1: return 0.01;
	case 2: return 0.001;
	case 3: return 0.0001;
	case 4: return 0.00001;
	case 5: return 0.000001;
	case 6: return 0.0000001;
	case 7: return 0.00000001;
	case 8: return 0.000000001;
	case 9: return 0.0000000001;
	}

	return (double)1 / pow((double)10, (int)uTakeDecimalDigits + 1);
}

unsigned long long MixedHelper::highRounding(unsigned long long uVal)
{
	unsigned long long nCount = 1;
	for (;;)
	{
		if (uVal >= 0 && uVal <= 9)
			break;
		uVal /= 10;
		nCount *= 10;
	}
	return uVal*nCount;
}

int MixedHelper::GetDoubleDotNum(double dVal)
{
	char szBuff[50];
	sprintf_s(szBuff, sizeof(szBuff)/sizeof(char), "%f", dVal);
	int idxLast = -1;
	int idxDot = -1;
	for (int i = strlen(szBuff) - 1; i >= 0; --i)
	{
		if (szBuff[i] == '.')
		{
			idxDot = i;
			break;
		}
		else if (szBuff[i] != '0' && idxLast == -1)
		{
			idxLast = i;
		}
	}

	if (idxLast == -1 || idxDot == -1)
		return 0;

	return idxLast - idxDot;
}

std::string MixedHelper::GetFractionFormat(double dVal, const int &nMaxDenoMinator /*= MAX_DENOMINATOR*/, 
	const double &dTol /*= 0.001*/, double dMax /*= 0.05*/, int nNumerator /*= 1*/)
{
	if (fabs(dVal) > dMax)
	{//不进行分数处理
		//处理小数点后几位
		double dTemp = fabs(dTol);
		int nNum = 0;
		while (dTemp < 1 && nNum <= 10)
		{
			dTemp *= 10;
			++nNum;
		}

		char szFormater[30];
		sprintf_s(szFormater, sizeof(szFormater) / sizeof(char), "%%.%df", nNum);

		char szRet[50];
		sprintf_s(szRet, sizeof(szRet) / sizeof(char), szFormater, dVal);
		return szRet;
	}
	else if (fabs(dVal) > 1)
	{//大于1的分数处理
		//处理小数点后几位
		double dTemp = fabs(dTol);
		int nNum = 0;
		while (dTemp < 1 && nNum <= 10)
		{
			dTemp *= 10;
			++nNum;
		}

		char szFormater[30];
		sprintf_s(szFormater, sizeof(szFormater) / sizeof(char), "%%.%df/1", nNum);

		char szRet[50];
		sprintf_s(szRet, sizeof(szRet) / sizeof(char), szFormater, dVal);
		return szRet;
	}
	else
	{//进行分子为1的分数处理
		if (IsDoubleEqual(dVal, 0.0, dTol))
		{
			return ("0");
		}

		char temp[256];
		_itoa_s(nNumerator, temp, 10);
		std::string sRet;
		sRet += temp;
		sRet += ("/");
		long long nDenominator = (long long)(nNumerator / dVal);

		//特殊的处理
		if (nDenominator > nMaxDenoMinator)
			nDenominator = nMaxDenoMinator;

		char szFormat[20];
		sprintf_s(szFormat, sizeof(szFormat) / sizeof(char), "%lld", nDenominator);
		sRet += szFormat;

		return sRet;
	}
}

std::string MixedHelper::GetEScientificNotation(const double &d, unsigned int uTakeDecimalDigits /*= 3*/)
{
	std::string sRet;

	double da = fabs(d);
	double d1 = da;

	int nE = 0;
	char c = '+';

	if (da > 1)
	{
		while (d1 >= 10)
		{
			++nE;
			d1 /= 10;
		}

		c = '+';
	}
	else
	{
		while (d1 < 1)
		{
			++nE;
			d1 *= 10;
		}

		c = '-';
	}

	double dRes = TakeDoubleBy(d1, uTakeDecimalDigits, 5);

	char szFormater[100];
	sprintf_s(szFormater, sizeof(szFormater) / sizeof(char), "%%.0%dfe%c%%0%dd", uTakeDecimalDigits, c, uTakeDecimalDigits);
	char szRet[100];
	sprintf_s(szRet, sizeof(szRet) / sizeof(char), szFormater, dRes, nE);
	sRet = szRet;

	if (d < 0)
		sRet = '-' + sRet;

	return sRet;
}

double MixedHelper::FilterZeroMinus(double dVal, double dTol /*= 0.00000001*/)
{
	double dAbs = fabs(dVal);
	if (dAbs < dTol)
	{
		return dAbs;
	}

	return dVal;
}

std::string MixedHelper::Double2String(double d, int uTakeDecimalDigits/*=-1*/)
{
	int nDotNum = uTakeDecimalDigits;
	if (uTakeDecimalDigits < 0)
	{//小送点后有多少则输出多少
		nDotNum = GetDoubleDotNum(d);
	}
	else
	{//
		d = TakeDoubleBy(d, uTakeDecimalDigits);//先四舍五入
	}

	char szFormater[20];
	sprintf_s(szFormater, sizeof(szFormater) / sizeof(char), "%%.%df", nDotNum);

	char szBuff[50];
	sprintf_s(szBuff, sizeof(szBuff) / sizeof(char), szFormater, d);
	return szBuff;
}

int MixedHelper::DoubleToInt(double dbl, double dRounding/*=0.5*/)
{
	int nRet = (int)dbl;
	if (dbl - nRet >= dRounding)
		++nRet;
	return nRet;
}

std::string MixedHelper::Double2String0(double d)
{
	std::string sFormat = std::to_string(d);
	
	for (int i = sFormat.length() - 1; i >= 0; --i)
	{
		char c = sFormat[i];
		if (c == '.')
		{
			sFormat[i] = '\0';
			break;
		}

		if (c == '0')
			sFormat[i] = '\0';
		else
			break;
	}

	return  sFormat;
}

bool MixedHelper::StringReplaceA(std::string& strBase, const std::string& strSrc, const std::string& strDes)
{
	bool b = false;

	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
		b = true;
	}

	return b;
}

bool MixedHelper::StringReplaceW(std::wstring& strBase, const std::wstring& strSrc, const std::wstring& strDes)
{
	bool b = false;

	std::wstring::size_type pos = 0;
	std::wstring::size_type srcLen = strSrc.size();
	std::wstring::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::wstring::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
		b = true;
	}

	return b;
}

void MixedHelper::StringSplitA(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters/* = " "*/)
{
	std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
	std::string::size_type pos = s.find_first_of(delimiters, lastPos);
	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}

void MixedHelper::StringSplitW(const std::wstring& s, std::vector<std::wstring>& tokens, const std::wstring& delimiters /*= " "*/)
{
	std::wstring::size_type lastPos = s.find_first_not_of(delimiters, 0);
	std::wstring::size_type pos = s.find_first_of(delimiters, lastPos);
	while (std::wstring::npos != pos || std::wstring::npos != lastPos) 
	{
		tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}
