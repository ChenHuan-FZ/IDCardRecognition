#ifndef _IDCARD_H_
#define _IDCARD_H_

/* �Ա�ö�����Ͷ��� */
typedef enum
{
	SEXTYPE_INVALID = -1,
	SEXTYPE_MAN,
	SEXTYPE_WOMAN
} SEX_TYPE;

/**
 * \brief  ���֤ʶ����
 */
class CIDCardRecognition {

public:
	CIDCardRecognition();
	virtual ~CIDCardRecognition();

	/**
	 * \brief  �����ļ��е����֤��Ϣ
	 * \param  szFileName -- �ļ����·��
	 * \return 0 -- �ɹ�\n <0 -- ����ʧ��
	 */
	int praseFile(const std::string &strFileName);

	/**
	 * \brief  �����ʾ���֤��Ϣ
	 * \param
	 */
	void show();

	/**
	 * \brief  ��ָ���Ա�������ʾ���֤��Ϣ
	 * \param  eType -- �Ա�����
	 */
	void show(const SEX_TYPE eType);

	/**
	 * \brief  �����������ս�������
	 * \param
	 */
	void sort();

protected:

private:
	// ���֤��Ϣ
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