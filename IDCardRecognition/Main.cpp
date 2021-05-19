/************************************************************ 
  FileName: main.cpp 
  Author: chenhuan
  Email: 245077022@qq.com
  Version :v1.0.0
  Date: 2021.05.08
  Description:
  1. 从info.txt中提取出所有正确且不重复的18位身份证号码，在控制台输出。
  2. 把挑选出来的身份证号码按性别进行分组显示，在控制台输出。
  3. 把挑选出来的身份证号码按年龄从小到大进行排序，在控制台输出。
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