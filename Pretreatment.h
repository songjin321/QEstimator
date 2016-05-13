#pragma once

//pretreatment before feature extraction
#include "common.h"

class Pretreatment
{
public:
    Pretreatment(){}
    ~Pretreatment(){}

    //start pretreatment
    void run(const cv::Mat& srcImg, cv::Mat& dstImg);

private:
    //intensify its edge
    void sharpeImage(const cv::Mat& srcImg, cv::Mat& res);

    //some mutable parameters

};
