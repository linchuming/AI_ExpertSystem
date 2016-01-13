#ifndef _EXPERTS_H
#define _EXPERTS_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "Evidence.h"
#include "Rule.h"
using namespace std;

#define BUF_LEN 1024

vector<rule*> rules;


int readRule(char* filename) //读取知识库并添加规则
{
	fstream f(filename,ios::in);
	string str;
	char buf[BUF_LEN];
	if(!f) {
		cout<<"read file error!"<<endl;
		return 1;
	}
	string attr,val;
	rule* new_rule;
	while(f.getline(buf,BUF_LEN)) {
		//f.getline(buf,1024);
		str = buf;
		if(str.length()==0) continue;
		if(str.find("IF")!=string::npos) {
			attr = str.substr(3,str.find("is")-4);
			val = str.substr(str.find("is")+3,BUF_LEN);
			evd[attr] = "";
			new_rule = new rule();
			rules.push_back(new_rule);
			new_rule->addPend(attr,val);

		} else if(str.find("AND")!=string::npos) {
			attr = str.substr(4,str.find("is")-5);
			val = str.substr(str.find("is")+3,BUF_LEN);
			evd[attr] = "";
			new_rule->addPend(attr,val);
		} else if(str.find("THEN")!=string::npos) {
			attr = str.substr(5,str.find("is")-6);
			val = str.substr(str.find("is")+3,BUF_LEN);
			evd[attr] = "";
			new_rule->setResult(attr,val);
		}
	}
	f.close();
	return 0;
}

#endif