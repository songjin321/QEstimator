#include "EstimateSystem.h"
#include <QDebug>
#include "Pretreatment.h"
#include "FeatureExtractor.h"

using namespace cv;

EstimateSystem::EstimateSystem()
    :workpiece_estimator(nullptr)
     {}

EstimateSystem::~EstimateSystem(){
    if(workpiece_estimator)
        delete workpiece_estimator;
}

void EstimateSystem::runWithPicture(const Mat& img){
    if(!workpiece_estimator)
        workpiece_estimator =   new WorkpieceEstimator;
    workpiece_estimator->setImage(img);
    workpiece_estimator->judgeTest();
    workpiece_estimator->distanceOrder=0;
    workpiece_estimator->radiusOrder=0;
}
//void EstimateSystem::calcPixelsPerMilimeter(const Mat &dImg){
//    if(PPMgot)
//        return;
//    PPMgot  =   true;
//    //since this shoudnt called for much times, freature detection was called again.
//    if(!workpiece_estimator)
//        workpiece_estimator =   new WorkpieceEstimator;

//    workpiece_estimator->setImage(dImg);
//    PPM =   workpiece_estimator->calcPPM();
//    qDebug()<<"PPM: "<<PPM;
//}

