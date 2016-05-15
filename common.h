#pragma once

//declar&define commons
//some utility function also declar here

#ifndef PI  //avoid macro repeatition
const float PI  =   3.1415926f;
#endif
#ifndef DTIMES  //
const int DTIMES = 3;
#endif
//opencv head file
#include <opencv2/opencv.hpp>
//#include <unistd.h>
//some basic data stuct
#include <string>
#ifndef DETECTTIMES
extern int detecttimes;
#endif
//line segment
struct Segment
{
    Segment():sPoint(0,0),ePoint(0,0),direction(0,0){}
    //regard as valid if end point not origin;
    bool isValid(){ return ePoint.x || ePoint.y; }

    cv::Point2f sPoint;
    cv::Point2f ePoint;
    cv::Point2f direction;
};
union Range{
    struct{
        float upper;
        float lower;
    };
    float bound[2];
};
struct Round
{
    Round():center(0,0),radius(-1.0f),box(cv::Point(0,0),cv::Point(0,0)){}
    cv::Point2f center;
    cv::Rect box;
    float radius;
};
extern cv::Point2f operator/ (const cv::Point2f &p1,float& p2);
//way to convert QImage and cv::Mat
#include <QImage>

extern cv::Mat fromQImage2Mat(const QImage& img);
extern QImage fromMat2QImage(const cv::Mat& mat);

//simple vector function
//assist for distance
extern float crossProduct2D(const cv::Point2f& p1, const cv::Point2f& p2);
extern float length2D(const cv::Point2f& p);

//display cv::Mat for by cv::imshow for debug
void IMSHOW(const std::string& winName,const cv::Mat&  mat);



