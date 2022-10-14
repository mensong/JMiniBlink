#pragma once
#include <string>
#include <vector>

#define MAX_DENOMINATOR 999999

class  MixedHelper
{
public:
	//�ж�double���
	static bool IsDoubleEqual(const double &d1, const double &d2, const double &dTol = 0.0001);
	//double�Ƿ����
	static bool IsDoubleMoreThan(const double &d1, const double &d2, const double &dTol = 0.0001);
	//double�Ƿ�С��
	static bool IsDoubleLessThan(const double &d1, const double &d2, const double &dTol = 0.0001);

	//�ж������Ƿ����
	template <class T>
	static bool isEqual(const T &v1, const T &v2, T dTol = 0.0001)
	{
		return fabs(v1 - v2) <= dTol;
	}

	/**
	* @brief     	��ñ���С������double
	* @param        d - �����double��
	* @param     	uTakeDecimalDigits - Ҫ������С����λ��
	* @param     	uMinCarry - ��uMinCarry-1����ȥ��uMinCarry��λ��Ĭ��Ϊ��������
	* @author    	LMS
	* @date      	14/5/2014
	* @retval    	ת�������ֵ
	* @note
	*/
	static double  TakeDoubleBy(double d, unsigned int uTakeDecimalDigits, unsigned int uMinCarry = 5);

	/**
	* @brief     	����
	* @param        d - �����double��
	* @param     	uTakeDecimalDigits - Ҫ������С����λ��
	* @author    	LMS
	* @date      	23/5/2014
	* @retval    	ת�������ֵ
	* @note      	���� d=1.231 uTakeDecimalDigits=2���򷵻�1.24
						 d=1.230 uTakeDecimalDigits=2���򷵻�1.23
						 d=1.1 uTakeDecimalDigits=0���򷵻�2
						 d=1.01 uTakeDecimalDigits=0���򷵻�1
	*/
	static double  CarryMaxInteger(const double &d, unsigned int uTakeDecimalDigits);

	/**
	* @brief     	���ݱ�����С����λ��������ݲ�
	* @param        -
	* @author    	LMS
	* @date      	24/9/2014
	* @retval
	* @note
	*/
	static double  GetDoubleTol(unsigned int uTakeDecimalDigits);

	/**
	* @brief		��λȡ��
	* @note        Ex: 0-->0    3-->3   45-->40  739-->700   158679-->10000
	*/
	static unsigned long long  highRounding(unsigned long long uVal);

	/**
	* @brief     	���С������λ��
	* @param        d - Ҫת������
	* @author    	LMS
	* @date      	12/6/2014
	* @retval
	* @note
	*/
	static int  GetDoubleDotNum(double dVal);

	/**
	* @brief     	��С��ת��Ϊ�ַ�����ʽ�ķ���
	* @param        d - Ҫת������
	* @param     	nNumerator - ����
	* @param     	dMax - ��ʾd�����������ʹ��С������ʽ��ʾ
	* @author    	LMS
	* @date      	12/6/2014
	* @retval
	* @note
	*/
	static std::string  GetFractionFormat(double dVal, const int &nMaxDenoMinator = MAX_DENOMINATOR, 
		const double &dTol = 0.001, double dMax = 0.05, int nNumerator = 1);

	/**
	* @brief     	e��ѧ������
	* @param        -
	* @author    	LMS
	* @date      	17/6/2015
	* @retval
	* @note
	*/
	static std::string  GetEScientificNotation(const double &d, unsigned int uTakeDecimalDigits = 3);

	/**
	* @brief     	����-0���еĸ��ţ���-0��Ϊ0
	* @param        -
	* @author    	LMS
	* @date      	5/9/2014
	* @retval
	* @note
	*/
	static double  FilterZeroMinus(double dVal, double dTol = 0.00000001);

	/**
	 * @brief     	double ת string
	 * @param       d - Ҫת���ĸ�����
	 * @param       uTakeDecimalDigits - ����С����λ����-1���������С��λ��
	 * @author    	Gergul
	 * @date      	2018/10/08
	 * @retval		
	 * @note        ȡС����λ����������
	 */
	static std::string Double2String(double d, int uTakeDecimalDigits=-1);

	/**	doubleתint
	*	@param  dbl - double��
	*	@param  dRounding - ���λ��λ����ֵ
	*	@author  Gergul
	*   @date
	*   @retval  -
	*   @note    -
	*   ��ⲽ�ۣ�-
	*/
	static int DoubleToInt(double dbl, double dRounding = 0.5);
	/**	doubleת�ַ���������
	*	@param  dbl - double��������С�����ĩβ��0��ȥ����
	*	@author  Gergul
	*   @date
	*   @retval  -
	*   @note    -
	*   ��ⲽ�ۣ�-
	*/
	static std::string Double2String0(double d);

	static bool StringReplaceA(std::string& strBase, const std::string& strSrc, const std::string& strDes);
	static bool StringReplaceW(std::wstring& strBase, const std::wstring& strSrc, const std::wstring& strDes);

	static void StringSplitA(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ");
	static void StringSplitW(const std::wstring& s, std::vector<std::wstring>& tokens, const std::wstring& delimiters = L" ");
};

