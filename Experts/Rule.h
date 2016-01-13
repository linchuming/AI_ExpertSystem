#ifndef _RULE_H
#define _RULE_H

#include <iostream>
#include <map>
#include <string>
#include "Evidence.h"
using namespace std;

class rule
{
	bool active;
	map<string,string> pend;
	string res_attr,res_val;
public:
	rule()
	{
		active = false;
	}
	~rule()
	{
	}
	bool getActive() //�жϹ����Ƿ�Ӧ�ù�
	{
		return active;
	}

	void addPend(string attr,string val) //����ж�����
	{
		pend[attr] = val;
	}

	void setResult(string attr,string val) //�����ж��ɹ���ӵ�֤��
	{
		res_attr = attr;
		res_val = val;
	}

	void pendRule() //���й����ж�
	{
		map<string,string>::iterator it;
		bool flag = true;
		for(it=pend.begin();it!=pend.end();it++) {
			if(it->second != evd[it->first]) {
				flag = false;
				break;
			}
		}
		if(flag) {
			active = true;
			evd[res_attr] = res_val;
		}
	}


};
#endif