#ifndef HARRIS_H
#define HARRIS_H
#include "opencv2/opencv.hpp"

class harris
{
private:
    cv::Mat  cornerStrength;  //opencv harris�����������Ҳ����ÿ�����صĽǵ���Ӧ����ֵ
    cv::Mat cornerTh; //cornerStrength��ֵ���Ľ��
    cv::Mat localMax; //�ֲ����ֵ���
    int neighbourhood; //���򴰿ڴ�С
    int aperture;//sobel��Ե��ⴰ�ڴ�С��sobel��ȡ�����ص�x��y����ĻҶȵ�����
    double k;
    double maxStrength;//�ǵ���Ӧ�������ֵ
    double threshold;//��ֵ��ȥ��ӦС��ֵ
    int nonMaxSize;//�������Ĭ�ϵ�3���������ֵ���Ƶ����򴰿ڴ�С
    cv::Mat kernel;//���ֵ���Ƶĺˣ�����Ҳ���������õ��ĺ�
public:
    harris():neighbourhood(3),aperture(3),k(0.01),maxStrength(0.0),threshold(0.01),nonMaxSize(3){

    };

    void setLocalMaxWindowsize(int nonMaxSize){
        this->nonMaxSize = nonMaxSize;
    };

    //����ǵ���Ӧ�����Լ������ֵ����
    void detect(const cv::Mat &image){
            //opencv�Դ��Ľǵ���Ӧ�������㺯��
            cv::cornerHarris (image,cornerStrength,neighbourhood,aperture,k);
            double minStrength;
            //���������С��Ӧֵ
            cv::minMaxLoc (cornerStrength,&minStrength,&maxStrength);

            cv::Mat dilated;
            //Ĭ��3*3�����ͣ�����֮�󣬳��˾ֲ����ֵ���ԭ����ͬ�������Ǿֲ����ֵ�㱻
            //3*3�����ڵ����ֵ��ȡ��
            cv::dilate (cornerStrength,dilated,cv::Mat());
            //��ԭͼ��ȣ�ֻʣ�º�ԭͼֵ��ͬ�ĵ㣬��Щ�㶼�Ǿֲ����ֵ�㣬���浽localMax
            cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
    }

    //��ȡ�ǵ�ͼ
    cv::Mat getCornerMap(double qualityLevel) {
            cv::Mat cornerMap;
            // ���ݽǵ���Ӧ���ֵ������ֵ
            threshold= qualityLevel*maxStrength;
            cv::threshold(cornerStrength,cornerTh,
            threshold,255,cv::THRESH_BINARY);
            // תΪ8-bitͼ
            cornerTh.convertTo(cornerMap,CV_8U);
            // �;ֲ����ֵͼ�룬ʣ�½ǵ�ֲ����ֵͼ��������ɷ����ֵ����
            cv::bitwise_and(cornerMap,localMax,cornerMap);
            return cornerMap;
    }

    void getCorners(std::vector<cv::Point> &points,
            double qualityLevel) {
            //��ȡ�ǵ�ͼ
            cv::Mat cornerMap= getCornerMap(qualityLevel);
            // ��ȡ�ǵ�
            getCorners(points, cornerMap);
    }

    // ����ȫͼ����ýǵ�
    void getCorners(std::vector<cv::Point> &points,
    const cv::Mat& cornerMap) {

            for( int y = 0; y < cornerMap.rows; y++ ) {
                    const uchar* rowPtr = cornerMap.ptr<uchar>(y);
                    for( int x = 0; x < cornerMap.cols; x++ ) {
                    // �������ǽǵ�
                          if (rowPtr[x]) {
                                points.push_back(cv::Point(x,y));
                          }
                     }
                }
          }

    //��ȦȦ��ǽǵ�
    void drawOnImage(cv::Mat &image,
    const std::vector<cv::Point> &points,
            cv::Scalar color= cv::Scalar(255,255,0),
            int radius=3, int thickness=2) {
                    std::vector<cv::Point>::const_iterator it=points.begin();
                    while (it!=points.end()) {
                    // �ǵ㴦��Ȧ
                    cv::circle(image,*it,radius,color,thickness);
                    ++it;
            }
    }

};

#endif // HARRIS_H
