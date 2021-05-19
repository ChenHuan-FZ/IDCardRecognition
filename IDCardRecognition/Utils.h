#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>

bool IsValidateDate(const int nYear, const int nMonth, const int nDay);

int converCharToInt(const char *pSrt, const int nSrtLen, int *pDst, int nDstLen);

#endif /* _UTILS_H_ */