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
	bool getActive() //判断规则是否应用过
	{
		return active;
	}

	void addPend(string attr,string val) //添加判定条件
	{
		pend[attr] = val;
	}

	void setResult(string attr,string val) //设置判定成功添加的证据
	{
		res_attr = attr;
		res_val = val;
	}

	void pendRule() //进行规则判定
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