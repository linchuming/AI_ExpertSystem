#ifndef _HOUGH_H
#define _HOUGH_H
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
#define PI 3.1415926

class LineFinder{
private:
        // ֱ�߶�Ӧ�ĵ��������
        std::vector<cv::Vec4i> lines;
        //����
        double deltaRho;
        double deltaTheta;
        // �ж���ֱ�ߵ���СͶƱ��
        int minVote;
        // �ж���ֱ�ߵ���С����
        double minLength;
        // ͬһ��ֱ���ϵ�֮��ľ������̶�
        double maxGap;
public:
        //��ʼ��
        LineFinder() : deltaRho(1), deltaTheta(PI/180),
        minVote(10), minLength(0.), maxGap(0.) {}
        // ���ò���
        void setAccResolution(double dRho, double dTheta) {
                deltaRho= dRho;
                deltaTheta= dTheta;
        }
        // ������СͶƱ��
        void setMinVote(int minv) {
                minVote= minv;
        }
        // ������С�߶γ��Ⱥ��߶μ�����̶�
        void setLineLengthAndGap(double length, double gap) {
                minLength= length;
                maxGap= gap;
        }

        //Ѱ���߶�
		std::vector<cv::Vec4i> findLines(cv::Mat& binary) {
                lines.clear();
                cv::HoughLinesP(binary,lines, deltaRho, deltaTheta, minVote,minLength, maxGap);
				return lines;
        }

        // ���߶�
        void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(0,255,255)) {
        std::vector<cv::Vec4i>::const_iterator it2=lines.begin();
        while (it2!=lines.end()) {
                cv::Point pt1((*it2)[0],(*it2)[1]);
				//cout<<(*it2)[0]<<" "<<(*it2)[1]<<endl;
                cv::Point pt2((*it2)[2],(*it2)[3]);
				//cout<<(*it2)[2]<<" "<<(*it2)[3]<<endl;
                cv::line( image, pt1, pt2, color);
                ++it2;
              }
        }
};

#endif