#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <set>

#include "IDCardRecognition.h"
#include "Utils.h"

using namespace std;

#define CONTENT_BUFFER_MAX_LEN      (1024 * 8)
#define CONTENT_CUTOUT_LEN          17
#define CONTENT_CHECK_PARITY_LEN    17
#define IDCARD_NUMBER_LEN           18

CIDCardRecognition::CIDCardRecognition()
{
    m_vecIDCard.clear();
}

CIDCardRecognition::~CIDCardRecognition()
{
	m_vecIDCard.clear();
	vector<string>().swap(m_vecIDCard);
}

// 检验年月日是否合法
int CIDCardRecognition::verifyDateTime(const string &strIDCard)
{
	int nYear, nMonth, nDay;

	if (strIDCard.length() != IDCARD_NUMBER_LEN) {
		return -1;
	}

	nYear = atoi(strIDCard.substr(6,4).c_str());
	nMonth = atoi(strIDCard.substr(10,2).c_str());
	nDay = atoi(strIDCard.substr(12,2).c_str());

	if (!IsValidateDate(nYear, nMonth, nDay)) {
		return -2;
	}

    return 0;
}

// 检验省份
int CIDCardRecognition::checkProvince(const string &strIDCard)
{
	int nIndex = 0;
	int nCityProvince = 0;
	int nCityCodeCount = 0;

    // 省份检验码
	const int nCityCode[] = { 11, 12, 13, 14, 15, 21, 22, 23, 31, 32,
							  33, 34, 35, 36, 37, 41, 42, 43, 44, 45,
							  46, 50, 51, 52, 53, 54, 61, 62, 63, 64,
							  65, 71, 81, 82, 91 };


	if (strIDCard.length() != IDCARD_NUMBER_LEN) {
		return -1;
	}

	nCityCodeCount = sizeof(nCityCode) / sizeof(nCityCode[0]);
	nCityProvince = atoi(strIDCard.substr(0,2).c_str());

	for (nIndex = 0; nIndex < nCityCodeCount; nIndex++) {
		if (nCityCode[nIndex] == nCityProvince) {
            return 0;
		}
	}

	return -2;
}

// 将身份证的每位和对应位的加权因子相乘之后，再得到和值
int CIDCardRecognition::getPowerSum(int *pParityBit, int nLen) 
{
    int nSum = 0;
	int nIndex = 0;
	int nPowerNum = 0;

	// 每位加权因子
	const int nPower[] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };

	if (NULL == pParityBit) {
	    return -1;
	}

	nPowerNum = (sizeof(nPower) / sizeof(nPower[0]));
	if (nLen > nPowerNum) {
		return -2;
	}

	for (nIndex = 0; nIndex < nLen; nIndex++) {
	    nSum += pParityBit[nIndex] * nPower[nIndex];
	}

	return nSum;
}

// 将和值与11取模得到余数进行校验码判断
char CIDCardRecognition::getCheckCodeBySum(int nParitySum) 
{
	char chCheckCode = -1;
	switch (nParitySum % 11) {
		case 10:
			chCheckCode = '2';
			break;
		case 9:
			chCheckCode = '3';
			break;
		case 8:
			chCheckCode = '4';
			break;
		case 7:
			chCheckCode = '5';
			break;
		case 6:
			chCheckCode = '6';
			break;
		case 5:
			chCheckCode = '7';
			break;
		case 4:
			chCheckCode = '8';
			break;
		case 3:
			chCheckCode = '9';
			break;
		case 2:
			chCheckCode = 'x';
			break;
		case 1:
			chCheckCode = '0';
			break;
		case 0:
			chCheckCode = '1';
			break;
	}

	return chCheckCode;
}

// 检验位合法性检验
int CIDCardRecognition::checkParity(const string &strIDCard)
{
	char szTmp[IDCARD_NUMBER_LEN] = {0};
	int nParityBit[IDCARD_NUMBER_LEN] = {0};
	int nParitySum;
	char chCheckCode;
	int nRlt = -1;

	if (strIDCard.length() != IDCARD_NUMBER_LEN) {
		return -1;
	}

	// 获取前17位
	strncpy_s(szTmp, strIDCard.substr(0, strIDCard.length() - 1).c_str(), CONTENT_CHECK_PARITY_LEN);

	// 获取整形数组
	nRlt = converCharToInt(szTmp, CONTENT_CHECK_PARITY_LEN, nParityBit, CONTENT_CHECK_PARITY_LEN);
	if (nRlt < 0) {
		return -2;
	}

	// 将身份证的每位和对应位的加权因子相乘之后，再得到和值
    nParitySum = getPowerSum(nParityBit, CONTENT_CHECK_PARITY_LEN);
	if (nParitySum) {
		return -3;
	}
	
	// 将和值与11取模得到余数进行校验码判断, 将身份证的第IDCARD_NUMBER_LEN位与算出来的校码进行匹配，不相等就为假
	chCheckCode = getCheckCodeBySum(nParitySum);
	if (chCheckCode == strIDCard[CONTENT_CHECK_PARITY_LEN] || ('x' == chCheckCode && 'X' == strIDCard[CONTENT_CHECK_PARITY_LEN])) {
		return 0;
	}

    return -4;
}

// 比较出生日，true为降序/flase为降序
static bool compare(const string &strIDCard, const string &strComp)
{
	string strDateTime;
	string strCompTime;

	if (strIDCard.length() != IDCARD_NUMBER_LEN || strComp.length() != IDCARD_NUMBER_LEN) {
		return false;
	}

	strDateTime = strIDCard.substr(6, 6);
	strCompTime = strComp.substr(6, 6);

	return (strDateTime > strCompTime);
}

// 按出生日降序排序
void CIDCardRecognition::sort()
{
	::sort(m_vecIDCard.begin(), m_vecIDCard.end(), compare);
}

// 检验身份证合法性
int CIDCardRecognition::verifyIDCard(const string &strIDCard)
{
	// 检验长度
	if (strIDCard.length() != IDCARD_NUMBER_LEN) {
        return -1;
	}

	// 检验身份证是否已存在
	vector<string>::iterator it = find(m_vecIDCard.begin(), m_vecIDCard.end(), strIDCard);
	if (it != m_vecIDCard.end()) {
		return -2;
	}

	// 检验省份
	if (checkProvince(strIDCard) != 0) {
		return -3;
	}
	
	// 检验日期
	if (verifyDateTime(strIDCard) != 0) {
		return -4;
	}

	// 检验位是否合法
	if (checkParity(strIDCard) != 0) {
		return -5;
	}

    return 0;
}

// 遍历文件内容，获取文件内容中所有的身份证信息
int CIDCardRecognition::loadContentIDCard(const string &strContent)
{
	smatch matchRlt;
	string::const_iterator iterStart;
	string::const_iterator iterEnd;
	regex regexPattern("[1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])\\d{3}([\\d|x|X]{1})");

	if (strContent.empty()) {
	    return -1;
	}

	iterStart = strContent.begin();
	iterEnd = strContent.end();
	while (regex_search(iterStart, iterEnd, matchRlt, regexPattern)) {
		string strTmp = matchRlt[0];
		if (verifyIDCard(strTmp)) {
			m_vecIDCard.push_back(strTmp);
		}

		cout << strTmp << endl;
		iterStart = matchRlt[0].second;	//更新搜索起始位置,搜索剩下的字符串
	}

	return 0;
}

// 读取文件信息，获取文件中所有身份证信息
int CIDCardRecognition::praseFile(const string &strFileName)
{
	string strContent;
	string strLastContent;
	int nLoadContentRlt;
	int nReadLen;
	char szContentBuffer[CONTENT_BUFFER_MAX_LEN] = {0};

	if (strFileName.empty()) {
	    return -1;
	}

	ifstream infile(strFileName, ios::in|ios::binary);
    if (!infile.is_open()) {
		cout << "Open file failure " << strFileName << endl;
		return -2;
	}
	
	strLastContent.clear();
	while (1) {

		if (!infile.read(szContentBuffer, CONTENT_BUFFER_MAX_LEN)) {
			break;
		}

		nReadLen = infile.gcount();
		if (strLastContent.length() < CONTENT_CUTOUT_LEN) {
			strContent = strLastContent + szContentBuffer;
		} else {
		    strContent = strLastContent.substr(nReadLen - CONTENT_CUTOUT_LEN, CONTENT_CUTOUT_LEN) + szContentBuffer;
		}
		strLastContent = szContentBuffer;

		nLoadContentRlt = loadContentIDCard(strContent);
		if (nLoadContentRlt != 0) {
            cout << "Load Content Failure Error Code:" << nLoadContentRlt << endl;
		}
		memset(szContentBuffer, 0x00, sizeof(szContentBuffer));
    }
	
	infile.close();

	return 0;
}

// 打印所有有效的身份证信息
void CIDCardRecognition::show()
{
    vector<string>::iterator it;

	for (it = m_vecIDCard.begin(); it != m_vecIDCard.end(); it++) {
		cout << "IDCARD:" << *it << endl;
    }

}

// 按性别进行分组打印身份证信息
void CIDCardRecognition::show(SEX_TYPE eType)
{
	int nFlag;
	vector<string>::iterator it;

	for(it = m_vecIDCard.begin(); it != m_vecIDCard.end(); it++) {
		nFlag = atoi(((string)*it).substr(16, 1).c_str());
		if ((nFlag % 2) == eType) {
			cout << "IDCARD:" << *it << endl;
		}
    }

}