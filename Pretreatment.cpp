#include "Pretreatment.h"
#include<QDebug>

using namespace cv;

void Pretreatment::run(const Mat &srcImg, Mat &dstImg){
    qDebug()<<"now running";
    //designated parameters
    const Size Ksize(5,5);
    const double sigmaX =   0;
    const double sigmaY =   0;
    const double threshold  =   80;
    const double max_value  =   255;
  // const int threshold_type    =   0;

    //processing
    Mat img;
    //convert to single channel
    //std::cout<<srcImg.channels()<<std::endl;
    switch(srcImg.channels()){
    case 4:
        cvtColor(srcImg,img,COLOR_BGRA2GRAY);
        break;
    case 3:
        cvtColor(srcImg,img,COLOR_BGR2GRAY);
        break;
    default:
        img =   srcImg.clone();
        break;
    }

    GaussianBlur(img,img,Ksize,sigmaX,sigmaY);
    //intensify edge
    sharpeImage(img,img);
    //convert to binary
    cv::threshold(img,dstImg,threshold,max_value,CV_THRESH_BINARY|CV_THRESH_OTSU);
}

void Pretreatment::sharpeImage(const Mat &srcImage, Mat &result)
{
    //创建并初始化滤波模板
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    result.create(srcImage.size(), srcImage.type());
    //对图像进行滤波
    cv::filter2D(srcImage, result, srcImage.depth(), kernel);
}
