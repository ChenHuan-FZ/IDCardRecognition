/************************************************************ 
  FileName: main.cpp 
  Author: chenhuan
  Email: 245077022@qq.com
  Version :v1.0.0
  Date: 2021.05.08
  Description:
  1. ��info.txt����ȡ��������ȷ�Ҳ��ظ���18λ���֤���룬�ڿ���̨�����
  2. ����ѡ���������֤���밴�Ա���з�����ʾ���ڿ���̨�����
  3. ����ѡ���������֤���밴�����С������������ڿ���̨�����
***********************************************************/ 
#include <vector>
#include <iostream>

#include "IDCardRecognition.h"

using namespace std;

#define FILE_PATH "C:\\share\\test\\info.txt"

int main(int argc, char *argv[])
{
	int nRlt = 0;

	CIDCardRecognition objIDCardRecognition;

    nRlt = objIDCardRecognition.praseFile(FILE_PATH);
	if (nRlt != 0) {
		cout << "Oget IDCard faild errcode: " << nRlt << endl;
		return -1;
	}

	objIDCardRecognition.show();

	getchar();
    return 0;
}