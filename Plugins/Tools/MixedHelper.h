#pragma once
#include <string>
#include <vector>

#define MAX_DENOMINATOR 999999

class  MixedHelper
{
public:
	//判断double相等
	static bool IsDoubleEqual(const double &d1, const double &d2, const double &dTol = 0.0001);
	//double是否大于
	static bool IsDoubleMoreThan(const double &d1, const double &d2, const double &dTol = 0.0001);
	//double是否小于
	static bool IsDoubleLessThan(const double &d1, const double &d2, const double &dTol = 0.0001);

	//判断两数是否相等
	template <class T>
	static bool isEqual(const T &v1, const T &v2, T dTol = 0.0001)
	{
		return fabs(v1 - v2) <= dTol;
	}

	/**
	* @brief     	获得保留小数点后的double
	* @param        d - 输入的double数
	* @param     	uTakeDecimalDigits - 要保留的小数点位数
	* @param     	uMinCarry - （uMinCarry-1）舍去，uMinCarry进位，默认为四舍五入
	* @author    	LMS
	* @date      	14/5/2014
	* @retval    	转换后的数值
	* @note
	*/
	static double  TakeDoubleBy(double d, unsigned int uTakeDecimalDigits, unsigned int uMinCarry = 5);

	/**
	* @brief     	进整
	* @param        d - 输入的double数
	* @param     	uTakeDecimalDigits - 要保留的小数点位数
	* @author    	LMS
	* @date      	23/5/2014
	* @retval    	转换后的数值
	* @note      	例如 d=1.231 uTakeDecimalDigits=2，则返回1.24
						 d=1.230 uTakeDecimalDigits=2，则返回1.23
						 d=1.1 uTakeDecimalDigits=0，则返回2
						 d=1.01 uTakeDecimalDigits=0，则返回1
	*/
	static double  CarryMaxInteger(const double &d, unsigned int uTakeDecimalDigits);

	/**
	* @brief     	根据保留的小数点位数，获得容差
	* @param        -
	* @author    	LMS
	* @date      	24/9/2014
	* @retval
	* @note
	*/
	static double  GetDoubleTol(unsigned int uTakeDecimalDigits);

	/**
	* @brief		高位取整
	* @note        Ex: 0-->0    3-->3   45-->40  739-->700   158679-->10000
	*/
	static unsigned long long  highRounding(unsigned long long uVal);

	/**
	* @brief     	获得小数点后的位数
	* @param        d - 要转化的数
	* @author    	LMS
	* @date      	12/6/2014
	* @retval
	* @note
	*/
	static int  GetDoubleDotNum(double dVal);

	/**
	* @brief     	把小数转化为字符串格式的分数
	* @param        d - 要转化的数
	* @param     	nNumerator - 分子
	* @param     	dMax - 表示d大于这个数则使用小数的形式表示
	* @author    	LMS
	* @date      	12/6/2014
	* @retval
	* @note
	*/
	static std::string  GetFractionFormat(double dVal, const int &nMaxDenoMinator = MAX_DENOMINATOR, 
		const double &dTol = 0.001, double dMax = 0.05, int nNumerator = 1);

	/**
	* @brief     	e科学计数法
	* @param        -
	* @author    	LMS
	* @date      	17/6/2015
	* @retval
	* @note
	*/
	static std::string  GetEScientificNotation(const double &d, unsigned int uTakeDecimalDigits = 3);

	/**
	* @brief     	过滤-0的中的负号，把-0改为0
	* @param        -
	* @author    	LMS
	* @date      	5/9/2014
	* @retval
	* @note
	*/
	static double  FilterZeroMinus(double dVal, double dTol = 0.00000001);

	/**
	 * @brief     	double 转 string
	 * @param       d - 要转换的浮点数
	 * @param       uTakeDecimalDigits - 保留小数点位数（-1则输出所有小数位）
	 * @author    	Gergul
	 * @date      	2018/10/08
	 * @retval		
	 * @note        取小数点位会四舍五入
	 */
	static std::string Double2String(double d, int uTakeDecimalDigits=-1);

	/**	double转int
	*	@param  dbl - double数
	*	@param  dRounding - 向个位进位的阈值
	*	@author  Gergul
	*   @date
	*   @retval  -
	*   @note    -
	*   求解步聚：-
	*/
	static int DoubleToInt(double dbl, double dRounding = 0.5);
	/**	double转字符串，并把
	*	@param  dbl - double数，并把小数点后，末尾的0都去除掉
	*	@author  Gergul
	*   @date
	*   @retval  -
	*   @note    -
	*   求解步聚：-
	*/
	static std::string Double2String0(double d);

	static bool StringReplaceA(std::string& strBase, const std::string& strSrc, const std::string& strDes);
	static bool StringReplaceW(std::wstring& strBase, const std::wstring& strSrc, const std::wstring& strDes);

	static void StringSplitA(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ");
	static void StringSplitW(const std::wstring& s, std::vector<std::wstring>& tokens, const std::wstring& delimiters = L" ");
};

