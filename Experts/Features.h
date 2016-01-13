#ifndef _FEATURES_H
#define _FEATURES_H

#include <opencv2\opencv.hpp>
#include "Hough.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

struct tline
{
	Point a,b;
	int cover;
	tline() 
	{
		cover = 0;
	}
};
vector<tline> lines;

inline int sqr(int x)
{
	return x*x;
}

void mergeLine(vector<Vec4i> tmp)
{
	lines.clear();
	vector<Vec4i>::iterator itc;
	for(itc=tmp.begin();itc!=tmp.end();itc++) {
		Point pt1((*itc)[0],(*itc)[1]);
		Point pt2((*itc)[2],(*itc)[3]);
		tline l;
		l.a = pt1; l.b = pt2;
		lines.push_back(l);
	}
	//先去除重复线段
	int gap; //允许的误差
	vector<tline>::iterator it;
	it=lines.begin();
	gap = (sqr(it->a.x - it->b.x) + sqr(it->a.y - it->b.y)) * 0.05;
	//cout<<gap<<endl;
	vector<tline>::iterator it2;
	int ins1,ins2;
	for(it=lines.begin();it!=lines.end();it++) {
		for(it2=lines.begin();it2!=lines.end();) {
			if(it==it2) {
				it2++;
				continue;
			}
			ins1 = sqr(it->a.x - it2->a.x) + sqr(it->a.y - it2->a.y);
			ins2 = sqr(it->b.x - it2->b.x) + sqr(it->b.y - it2->b.y);
			if(ins1 <= gap && ins2 <= gap) {
				it2 = lines.erase(it2);
				continue;
			}
			ins1 = sqr(it->a.x - it2->b.x) + sqr(it->a.y - it2->b.y);
			ins2 = sqr(it->b.x - it2->a.x) + sqr(it->b.y - it2->a.y);
			if(ins1 <= gap && ins2 <= gap) {
				it2 = lines.erase(it2);
				continue;
			}
			it2++;
		}
	}

	//合并线段顶点
	for(it=lines.begin();it!=lines.end();it++) {
		for(it2=lines.begin();it2!=lines.end();it2++) {
			if(it==it2) continue;
			ins1 = sqr(it->a.x - it2->a.x) + sqr(it->a.y - it2->a.y);
			ins2 = sqr(it->b.x - it2->b.x) + sqr(it->b.y - it2->b.y);
			if(ins1 <= gap) {
				int x = (it->a.x + it2->a.x) / 2;
				int y = (it->a.y + it2->a.y) / 2;
				it->a.x = it2->a.x = x;
				it->a.y = it2->a.y = y;
				it->cover++;
				it2->cover++;
			}
			if(ins2 <= gap) {
				int x = (it->b.x + it2->b.x) / 2;
				int y = (it->b.y + it2->b.y) / 2;
				it->b.x = it2->b.x = x;
				it->b.y = it2->b.y = y;
				it->cover++;
				it2->cover++;
			}
			ins1 = sqr(it->a.x - it2->b.x) + sqr(it->a.y - it2->b.y);
			ins2 = sqr(it->b.x - it2->a.x) + sqr(it->b.y - it2->a.y);
			if(ins1 <= gap) {
				int x = (it->a.x + it2->b.x) / 2;
				int y = (it->a.y + it2->b.y) / 2;
				it->a.x = it2->b.x = x;
				it->a.y = it2->b.y = y;
				it->cover++;
				it2->cover++;
			}
			if(ins2 <= gap) {
				int x = (it->b.x + it2->a.x) / 2;
				int y = (it->b.y + it2->a.y) / 2;
				it->b.x = it2->a.x = x;
				it->b.y = it2->a.y = y;
				it->cover++;
				it2->cover++;
			}
		}
	}

	for(it=lines.begin();it!=lines.end();) {
		//cout<<it->a<<" "<<it->b<<" "<<it->cover<<endl;
		if(it->cover!=4) {
			it = lines.erase(it);
		} else {
			it++;
		}
	}

}

void getFeatures(vector<Vec4i> tmp) //获取图片中的形状特征信息
{

	//求顶点个数
	mergeLine(tmp);
	char buf[128];
	evd["node number"] = itoa(lines.size(),buf,10);
	//cout<<evd["node number"]<<endl;
	//cout<<evd["node number"].length()<<endl;

	//求相等边数
	vector<tline>::iterator it;
	vector<int> lens;
	for(it=lines.begin();it!=lines.end();it++) {
		lens.push_back(sqr(it->a.x - it->b.x)+sqr(it->a.y - it->b.y));
	}
	sort(lens.begin(),lens.end());
	int max_num = 1,cur_num = 1;
	double gap = 0.05; //允许的误差
	vector<int>::iterator p;
	for(p=lens.begin();p!=lens.end();p++) {
		if(p+1==lens.end()) break;
		//cout<<*p<<endl;
		if(abs(*p - *(p+1)) / (double)*p <= gap) {
			cur_num++;
			if(cur_num > max_num) max_num = cur_num;
		} else {
			cur_num = 1;
		}
	}
	//cout<<max_num<<endl;

	evd["equal sides number"] = itoa(max_num,buf,10);
	//cout<<evd["equal sides number"]<<endl;

	//求夹角为直角的个数
	vector<tline>::iterator it2;
	gap = 0.001; //允许的误差
	int right = 0;
	for(it=lines.begin();it!=lines.end();it++) {
		for(it2=lines.begin();it2!=lines.end();it2++) {
			if(it==it2) continue;
			if(it->a == it2->a || it->a == it2->b || it->b == it2->a || it->b == it2->b) {
				Point v1,v2;
				v1 = it->a - it->b;
				v2 = it2->a - it2->b;
				double m_v1 = sqr(v1.x) + sqr(v1.y);
				double m_v2 = sqr(v2.x) + sqr(v2.y);
				double val = (double)sqr(v1.x*v2.x + v1.y*v2.y)/m_v1/m_v2;
				if(val<gap) {
					right++;
				}
			}
		}
	}
	right/=2;
	evd["right angle number"] = itoa(right,buf,10);
	//cout<<evd["right angle number"]<<endl;


}
#endif