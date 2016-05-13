#pragma once
#include <vector>
#include "common.h"
#include "Criterion.h"
//#include "masterthread.h"
//WorkpieceEstimator, use criterion from CriterionManager
//estimate each workpiece by designated criterion

class WorkpieceEstimator
{
public:
    WorkpieceEstimator();
    ~WorkpieceEstimator();

    //test for designated picture
    //this function would be deprecated once the hardware camera ready
    bool judgeTest();
    void setImage(const cv::Mat& img){
        _img    =   img.clone();
        currentCircles.clear();
        currentLines.clear();
    }
    //void extractFeature();
    void extractFeature(Criterion*);
    static float calcPPM(Segment& fLine, Segment& sLine, double stdDistance);
    static float calcPPM(Round& r,double stdDistance);
    std::vector<Segment> currentLines;
    std::vector<Round> currentCircles;

    //How to get average?
    int distanceOrder;
    int radiusOrder;
private:

    int judgeTwoLineRelation(CriterionBase* cb, int order);
    bool judgeCircleCharacter(CriterionBase* cb,int order);
    //search for matching feature to estimate, return -1 if failed
    int findMatchLine(Segment& line);
    int findMatchCircle(const Round& circle);
    //
    float getAverageValue(std::vector<float> &values);
    //
    cv::Mat _img;//current image
    float PPM;//pixels per millimeter
    cv::Point2f basePoint;
    std::vector<float> FrameDistances[10];
    std::vector<float> FrameRadius[10];
    //MasterThread serialPort;
};
