#pragma once

//back-stage management, hold all procedures

#include "WorkpieceEstimator.h"
#include "common.h"

class EstimateSystem
{
    EstimateSystem(const EstimateSystem&){}
    EstimateSystem& operator = (const EstimateSystem&){ return *this; }
public:
    EstimateSystem();
    ~EstimateSystem();

    void runWithPicture(const cv::Mat& img);
    //void calcPixelsPerMilimeter(const cv::Mat& dImg);
    // void recalcPPM(const cv::Mat& dImg){
    // PPMgot = false;
    // calcPixelsPerMilimeter(dImg);
   //}

//private:
//    bool PPMgot;
//    float PPM;

    WorkpieceEstimator* workpiece_estimator;

};
