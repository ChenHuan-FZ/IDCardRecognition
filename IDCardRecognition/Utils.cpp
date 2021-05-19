#include "Utils.h"

// 判断年月日是否合法
bool IsValidateDate(const int nYear, const int nMonth, const int nDay)
{
    int nMonth_Day[12]  = { 31,((nYear % 4 == 0 && nYear % 100 != 0 )|| nYear % 400 == 0) ? 29 : 28,
        31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return nMonth >= 1 && nMonth <= 12 && nDay >= 1 && nDay <= nMonth_Day[nMonth - 1];
}

// 将数字字符数组转为整型数组
int converCharToInt(const char *pSrt, const int nSrtLen, int *pDst, int nDstLen)
{
	int nIndex = 0;

	if (NULL == pSrt || NULL == pDst) {
	    return -1;
	}

	if (nSrtLen <= 0 || nDstLen <= 0) {
		return -2;
	}

	for (nIndex = 0 ; nIndex < nSrtLen; nIndex++) {

		if (nIndex >= nDstLen) {
			break;
		}

		pDst[nIndex] = (int)pSrt[nIndex] - (int)('0');
	}

	return nIndex;
}
