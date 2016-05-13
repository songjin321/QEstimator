#include "WorkpieceEstimator.h"
#include "Pretreatment.h"
#include "FeatureExtractor.h"
#include "CriterionManager.h"
#include <QDebug>
#include <math.h>\
#include "ui/MainWindow.h"
#include "common.h"
//#include "masterthread.h"
using namespace cv;
WorkpieceEstimator::WorkpieceEstimator()
    :PPM(1.0f),distanceOrder(0),radiusOrder(0)
{}

WorkpieceEstimator::~WorkpieceEstimator(){

}
bool WorkpieceEstimator::judgeTest(){
    bool qualified  =   true;

    Criterion* criterion    =   nullptr;

#if 0
    //here, init current criterion by code, which avoids redundant operations
//    0 check
//        line1: 1526,1247 -> 2102,708
//        line2: 1908,1660 -> 2486,1118
//        dis  : -1 to -1
//        angle: -1 to -1
//    0 check
//        line1: 1526,1247 -> 2102,708
//        line2: 2487,1112 -> 2110,709
//        dis  : -1 to -1
//        angle: -1 to -1
    criterion   =   Criterion::create();
    criterion->header.name  =   "debug";

    TwoLineCriterion* tlc   =   new TwoLineCriterion;
    tlc->_fLine.sPoint  =   Point(1526,1247);   tlc->_fLine.ePoint  =   Point(2102,708);
    tlc->_sLine.sPoint  =   Point(1908,1660);   tlc->_sLine.ePoint  =   Point(2486,1118);
    tlc->_fLine.direction   =   (tlc->_fLine.ePoint-tlc->_fLine.sPoint)/length2D(tlc->_fLine.ePoint-tlc->_fLine.sPoint);
    tlc->_sLine.direction   =   (tlc->_sLine.ePoint-tlc->_sLine.sPoint)/length2D(tlc->_sLine.ePoint-tlc->_sLine.sPoint);
    tlc->_distance.lower    =   tlc->_distance.upper    =   100.0f;
    tlc->_angle.lower   =   tlc->_angle.upper   =   -1;
    criterion->criterionList.push_back(tlc);

    tlc =   new TwoLineCriterion;
    tlc->_fLine.sPoint  =   Point(1526,1247);   tlc->_fLine.ePoint  =   Point(2102,708);
    tlc->_sLine.sPoint  =   Point(2487,1112);   tlc->_sLine.ePoint  =   Point(2110,709);
    tlc->_fLine.direction   =   (tlc->_fLine.ePoint-tlc->_fLine.sPoint)/length2D(tlc->_fLine.ePoint-tlc->_fLine.sPoint);
    tlc->_sLine.direction   =   (tlc->_sLine.ePoint-tlc->_sLine.sPoint)/length2D(tlc->_sLine.ePoint-tlc->_sLine.sPoint);
    tlc->_distance.lower    =   tlc->_distance.upper    =   -1;
    tlc->_angle.lower   =   tlc->_angle.upper   =   90.0f;
    criterion->criterionList.push_back(tlc);distanceOrder++;

    if(!CriterionManager::getInstance()->addCriterion(criterion)){
        qDebug()<<"failed to add criterion";
    }
    CriterionManager::getInstance()->setCurrentCriterion(criterion->header.name);
    //delete criterion;   criterion   =   nullptr;
    //cannot delete it!
#endif

    criterion    =   CriterionManager::getInstance()->getCurrentCriteion();
    if(!criterion){
        qDebug()<<"estimate quit because invalid criterion!";
        return false;
    }
    basePoint=criterion->header.basePoint;
    PPM=criterion->header.PPM;
    extractFeature(criterion);
    //begin judge

    qDebug()<<"begin judging\n"<<criterion->criterionList.size();
    for(size_t i=0;i<criterion->criterionList.size();++i){
        qDebug()<<"criterion index: "<<i;
        CriterionBase* singleRule  =   criterion->criterionList[i];
        bool judgeResult;

        switch(singleRule->getType()){
        case twoLineRelation:
            //qualified   =   qualified && judgeTwoLineRelation(singleRule);
                // func call would be skip            
            judgeResult   =   judgeTwoLineRelation(singleRule,distanceOrder);
            distanceOrder++;
            break;
        case circleCharacter:
            //qualified   =   qualified && judgeCircleCharacter(singleRule);
            judgeResult =   judgeCircleCharacter(singleRule,radiusOrder);
            radiusOrder++;
            break;
        default:
            qDebug()<<"not the expected type!";
            break;
        }

        qualified   =   qualified && judgeResult;
    }

    return qualified;
}

void WorkpieceEstimator::extractFeature(Criterion* criterion){
    imwrite("../_img.jpg",_img);
    //pretreatment and extract features
    qDebug()<<"pretreament okay";

    FeatureExtractor* fe    =   new FeatureExtractor;
    fe->detect(criterion,_img);
    qDebug()<<"extracted";
    fe->getCircles(currentCircles);
    fe->getSegments(currentLines);
    delete fe;  fe  =   nullptr;
    qDebug()<<"feature extracted.";
}
float WorkpieceEstimator::calcPPM(Segment& fLine,Segment& sLine,double stdDistance){
        float disInPixels    =   qAbs(crossProduct2D(sLine.direction,(fLine.sPoint-sLine.sPoint)))+
                        qAbs(crossProduct2D(sLine.direction,(fLine.ePoint-sLine.sPoint)));
        disInPixels    /=  2.0f;
        return   disInPixels/stdDistance;
    }
float WorkpieceEstimator::calcPPM(Round &r, double stdDistance){
    return r.radius*2/stdDistance;
}

int WorkpieceEstimator::judgeTwoLineRelation(CriterionBase *cb,int order){
    qDebug()<<"two line relation judge";
    TwoLineCriterion* tlc   =   dynamic_cast<TwoLineCriterion*>(cb);
    if(!tlc){
        qDebug()<<"WorkpieceEstimator: dynamic cast failed!";
        return false;
    }

    float distance  =   -1.0f;
    float angle =   -1.0f;
    do{
        int fLineIndex  =   findMatchLine(tlc->_fLine);
        int sLineIndex  =   findMatchLine(tlc->_sLine);
        if(fLineIndex < 0 || sLineIndex < 0){
            qDebug()<<"WorkpieceEstimator: no match line found!";
            break;
        }
        Segment& fLine  =   currentLines[fLineIndex];
        Segment& sLine  =   currentLines[sLineIndex];
//        qDebug()<<fLine.sPoint.x<<" "<<fLine.sPoint.y<<" "<<fLine.ePoint.x<<" "<<fLine.ePoint.y;
//        qDebug()<<sLine.sPoint.x<<" "<<sLine.sPoint.y<<" "<<sLine.ePoint.x<<" "<<sLine.ePoint.y;


        //clac distance and angle if necessary
            distance    =   qAbs(crossProduct2D(sLine.direction,(fLine.sPoint-sLine.sPoint)))
                            +qAbs(crossProduct2D(fLine.direction,(fLine.sPoint-sLine.sPoint)));
            distance    /=  2.0f;
            //qDebug()<<"disance="<<distance;
            FrameDistances[order].push_back(distance);
            qDebug()<<detecttimes;
            if(detecttimes==DTIMES)
            {
                int i=0;
                do{
                    distance=getAverageValue(FrameDistances[i]);
                    distance/=PPM;
                    qDebug()<<"FinallyDistance:"<<distance<<"mm";
                    i++;
                }while(FrameDistances[i].data());
                detecttimes=0;
                distanceOrder=0;

            }

        if(tlc->_angle.lower > 0){
            angle   =   qAbs(std::asin(crossProduct2D(sLine.direction,fLine.direction)))/PI*180.0f;
            qDebug()<<angle;
        }
    }while(false);

    //if need check strictly, consider range
    if(tlc->needCheck()){
        for(int i=0;i<tlc->disRankNum;i++)
        {
            if(tlc->_distances[i].lower>0){
             if(distance>=tlc->_distances[i].lower && distance<=tlc->_distances[i].upper)
             {
                 //serialPort.transaction(QString::number(i));
             }
            }
        }
        if(tlc->_angle.lower>0){
            if(tlc->_angle.lower<=angle && angle<=tlc->_angle.upper)
            {
                //serialPort.transaction(QString::number(angle));
            }
        }
    }


    return true;
}

int WorkpieceEstimator::findMatchLine(Segment &line){
    //since the detection algorithm of each line do NOT assiocate with the others,
    //probably mismatch

    //1. included angle must under a threshold
    //2. the line with minimum distance is most probably considered as the aimed segment
    //      use the midpoint to calc distance
    //3. if it's hard to tell by DISTANCE, consider LENGTH

    const float angleThreshold  =   10.0f;  //degrees

    int re  =   -1;

    float minDis    =   FLT_MAX;
    float dis   =   minDis;
    float reAngle   =   90.0f;
    cv::Point2f midpoint ;
    midpoint.x=   (line.sPoint.x+line.ePoint.x)/2.0f;
    midpoint.y=   (line.sPoint.y+line.ePoint.y)/2.0f;
    for(size_t i=0;i<currentLines.size();++i){
        Segment& seg    =   currentLines[i];
        //1. ANGLE
        float angle =   qAbs(std::asin(crossProduct2D(line.direction,seg.direction)))/PI*180.0f;
        if(angle > angleThreshold)
            continue;

        //2. DISTANCE
        dis =   qAbs(crossProduct2D(seg.direction,(midpoint-seg.sPoint)));
        if(dis < minDis){
            minDis  =   dis;
            reAngle =   angle;
            re  =   i;
        }
    }

    //qDebug()<<"match line: "<<re<<" dis: "<<minDis<<" angle: "<<reAngle;

    return re;
}


bool WorkpieceEstimator::judgeCircleCharacter(CriterionBase *cb, int order){
    qDebug()<<"order="<<order;
    CircleCriterion* cc =   dynamic_cast<CircleCriterion*>(cb);
    if(!cc){
        qDebug()<<"dynamic cast failed!";
        return false;
    }
    Round r;
    r.center    =   cc->_center;
    r.radius    =   cc->_radiu;
    int circleIndex =   findMatchCircle(r);
    if(circleIndex < 0){
        qDebug()<<"WorkpieceEstimator: no match circle found!";
        return false;
    }
    qDebug()<<"detecttimes"<<detecttimes;
    float radius    =   currentCircles[circleIndex].radius/PPM;
    Point2f realCenter;
    realCenter.x =(currentCircles[circleIndex].center.x-basePoint.x)/PPM;
    realCenter.y=(currentCircles[circleIndex].center.y-basePoint.y)/PPM;
    FrameRadius[order].push_back(radius);
    if(detecttimes==DTIMES)
    {
        int i=0;
        do{
            radius=getAverageValue(FrameRadius[i]);
            qDebug()<<"FinallyDiameter:"<<radius*2<<"mm";
            qDebug()<<"realCenterX:"<<realCenter.x<<"mm";
           // serialPort.transaction(QString::number(realCenter.x));
            qDebug()<<"realCenterY:"<<realCenter.y<<"mm";
           // serialPort.transaction(QString::number(realCenter.y));
            i++;
        }while(FrameRadius[i].data());
        radiusOrder=0;
        detecttimes=0;
    }
    if(cc->needCheck()){
        return cc->_radius.lower <= radius && radius <= cc->_radius.upper;
    }
    return true;
}

int WorkpieceEstimator::findMatchCircle(const Round &circle){
    //1. center
    //2. radius
    float posThreshold    =   circle.radius*0.5f;

    int re  =   -1;
    float minDiffer =   FLT_MAX;
    float difference  =   FLT_MAX;
    for(size_t i=0;i<currentCircles.size();++i){
        const Round& r  =   currentCircles[i];

        if(length2D(r.center - circle.center) > posThreshold)
            continue;

        difference  =   qAbs(r.radius-circle.radius);
        if(difference < minDiffer){
            minDiffer   =   difference;
            re  =   i;
        }
    }
    qDebug()<<"match circle: "<<re<<" difference: "<<minDiffer;

    return re;
}
float WorkpieceEstimator::getAverageValue(std::vector<float>& values)
{
    float sumDis=0.0f;
    float max=FLT_MIN;
    float min=FLT_MAX;
    for(int i=0;i<values.size();i++)
    {
        float dis=values[i];
        if(dis<min)
            min=dis;
        if(dis>max)
            max=dis;
    }
    for(int i=0;i<values.size();i++)
    {
        float dis=values[i];
        if(dis>min&&dis<max)
            sumDis+=dis;
    }
    float ret=sumDis/(values.size()-2);
    values.clear();
    return ret;
}






