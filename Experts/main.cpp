#include <iostream>
#include <opencv2\opencv.hpp>
#include "Experts.h"
#include "Rule.h"
#include "Hough.h"
#include "Features.h"
#include "Harris.h"

using namespace std;
using namespace cv;


void doRule() //进行推理
{
	vector<rule*>::iterator it;
	
	bool flag = false;
	do {
		flag = false;
		for(it=rules.begin();it!=rules.end();it++) {
			if(!(**it).getActive()) {
				(**it).pendRule();
				if((**it).getActive()) flag = true;
			}
		}
	}while(flag);
	
}

int main()
{
	while(1) {
	readRule("rule.txt");
	
	cout<<"input image:";
	string filename;
	cin>>filename;
	Mat image = imread(filename);
	//Mat blank = imread("0.jpg");
	if(image.empty()) {
		cout<<"read image error!"<<endl;
		return 0;
	}
	
	Mat result;
	cvtColor(image,result,CV_BGRA2GRAY);

	/*
	harris Harris;
	Harris.detect(result);
	vector<Point> pts;

	Harris.getCorners(pts,0.01);
	goodFeaturesToTrack(result,pts,20,0.7,20);

	Harris.drawOnImage(image,pts);

	//imshow("harris",image);
	//waitKey(0);
	imwrite("harris\\"+filename,image);
	}
	*/
	


	Mat contours;

	Canny(result,contours,125,350);
	LineFinder finder;
	finder.setMinVote(80);
	finder.setLineLengthAndGap(50,20);
	vector<Vec4i> tmp; 
	tmp = finder.findLines(contours);
	getFeatures(tmp); //获取图片特征信息
	finder.drawDetectedLines(image);
	//imwrite("hough\\"+filename,image);
	doRule();
	cout<<"Result: "<<evd["shape"]<<endl;
	}
	
	//imshow("hough",image);
	//waitKey(0);
	
	return 0;
}