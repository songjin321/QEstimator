#pragma once

//extract feature elements of an image
//get edge by 'canny',
//use hough transform to detect line, circle .etc

#include "common.h"
#include <vector>
#include "Criterion.h"
enum FeatureType
{
    Feature_Line,
    Feature_Circle
};

class FeatureExtractor
{
public:
    FeatureExtractor();
    ~FeatureExtractor();

    void extract(const cv::Mat& srcImg);
    void extractBlindHole(const cv::Mat& srcImg);
    void detect(Criterion* criterion,const cv::Mat& srcImg);
    void getCircles(std::vector<Round>& circlesOut){
        circlesOut.assign(rounds.begin(),rounds.end());
    }
    void getSegments(std::vector<Segment>& linesOut){
        linesOut.assign(lines.begin(),lines.end());
    }

private:
    //result
    const int detectCircleMinDis=10;
    const int detectLineMinDis=20;
    std::vector<Segment> lines;
    std::vector<Round> rounds;
    void detectCircle(std::vector<cv::Point2f> Points);
    void fitLinedetect(Segment detectedline, std::vector<cv::Point2f> Points);
    double changeAngle(int y1, int y2,int x1,int x2, double SlopeCurrent);
};
