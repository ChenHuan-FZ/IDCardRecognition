#ifndef _IDCARD_H_
#define _IDCARD_H_

/* 性别枚举类型定义 */
typedef enum
{
	SEXTYPE_INVALID = -1,
	SEXTYPE_MAN,
	SEXTYPE_WOMAN
} SEX_TYPE;

/**
 * \brief  身份证识别类
 */
class CIDCardRecognition {

public:
	CIDCardRecognition();
	virtual ~CIDCardRecognition();

	/**
	 * \brief  解析文件中的身份证信息
	 * \param  szFileName -- 文件存放路径
	 * \return 0 -- 成功\n <0 -- 解析失败
	 */
	int praseFile(const std::string &strFileName);

	/**
	 * \brief  输出显示身份证信息
	 * \param
	 */
	void show();

	/**
	 * \brief  按指定性别类型显示身份证信息
	 * \param  eType -- 性别类型
	 */
	void show(const SEX_TYPE eType);

	/**
	 * \brief  按出生年月日降序排序
	 * \param
	 */
	void sort();

protected:

private:
	// 身份证信息
	std::vector<std::string> m_vecIDCard;

private:
	int loadContentIDCard(const std::string &strContent);

	int verifyDateTime(const std::string &strIDCard);

	char getCheckCodeBySum(int nParitySum);

	int checkProvince(const std::string &strIDCard);

	int getPowerSum(int *pParityBit, int nLen);

	int verifyIDCard(const std::string &strIDCard);

	int checkParity(const std::string &strIDCard);

};

#endif /* _IDCARD_H_ */