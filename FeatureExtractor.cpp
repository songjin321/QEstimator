#include "FeatureExtractor.h"
#include "Pretreatment.h"
#include "common.h"
#include <algorithm>
#include <QDebug>
using namespace cv;
using namespace std;
FeatureExtractor::FeatureExtractor(){}
FeatureExtractor::~FeatureExtractor(){}
void FeatureExtractor::extract(const Mat& srcImg){
    //check input type
    qDebug()<<"channels="<<srcImg.channels();
    if(srcImg.channels() == 1)
        ;
    //start
    vector<Point> polygonPoints;
    vector<Point> circlePoints;
    const double minNumberRatio=1,maxNumberRatio=300;
    const int minPointNumber=srcImg.cols*srcImg.rows*minNumberRatio*0.00001,
              maxPointNumber=srcImg.cols*srcImg.rows*maxNumberRatio*0.00001;
    qDebug()<<"minPointNumber="<<minPointNumber<<" "<<"maxPointNumber="<<maxPointNumber;
    const double circleThreshold=0.2;//circle angle threshold
    const int angleThreshold=10,disThreshold=50;//判断斜率
    int contoursSize=0;
    vector< vector<Point> > contours;
    Point startPoint,endPoint;
    findContours(srcImg,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
    qDebug()<<"contours.size()="<<contours.size()<<endl;
    //if the number of contours much ->canny

    //
        for(size_t i=0;i<contours.size();i++)
        {
           if((contours[i].size()>minPointNumber)&&(contours[i].size()<maxPointNumber))
           {
               bool isCircle=false;
               //area And length to divide
               double areaContour=contourArea(contours[i]);
               Point2f center;
               float radius;
               minEnclosingCircle(contours[i],center,radius);
               double areaCircle=PI*radius*radius;
               double ErrorPercent=abs(areaCircle-areaContour)/areaContour;
               qDebug()<<areaContour<<" "<<areaCircle<<" "<<"圆度误差"<<ErrorPercent;
               if(ErrorPercent<circleThreshold)
               {
                   isCircle=true;
               }
               else
               {
                   isCircle=false;
               }
               if(!isCircle)
               {
                    polygonPoints=contours[i];
                    //将斜率相近的作为一个点
                    startPoint=polygonPoints[0];
                    endPoint=polygonPoints[disThreshold];
                    for(vector<Point>::iterator it=polygonPoints.begin();it<polygonPoints.end()-disThreshold*2;it+=disThreshold)
                    {

                        double Slope0=180/PI*atan(1.0*(startPoint.y-endPoint.y)/(startPoint.x-endPoint.x));
                        Slope0=changeAngle(startPoint.y,endPoint.y,startPoint.x,endPoint.x,Slope0);
                        int sX=(it+disThreshold)->x,sY=(it+disThreshold)->y,
                            eY=(it+disThreshold*2)->y,eX=(it+disThreshold*2)->x;
                        double SlopeCurrent=180/PI*atan(1.0*(sY-eY)/(sX-eX));
                        SlopeCurrent=changeAngle(sY,eY,sX,eX,SlopeCurrent);
                        double angle=abs(Slope0-SlopeCurrent);
                        if(angle>180)
                        {
                            angle=360-angle;
                        }
                        if(angle<angleThreshold)
                        {
                            endPoint=*(it+disThreshold*2);
                        }
                        else
                        {
                            Segment tem;
                            tem.sPoint=startPoint;
                            tem.ePoint=endPoint;
                            lines.push_back(tem);
                            startPoint=*(it+disThreshold);
                            endPoint=*(it+disThreshold*2);
                        }
                    }
                    //将最后一条与第一个点连接
                    Segment tem;
                    tem.sPoint=startPoint;
                    tem.ePoint=polygonPoints[0];
                    lines.push_back(tem);
                    //here, calculate the direction of each segment
                    for_each(lines.begin(),lines.end(),
                             [&](Segment& s){
                        s.direction.x =   (s.ePoint.x-s.sPoint.x)/length2D(s.ePoint-s.sPoint);
                        s.direction.y=   (s.ePoint.y-s.sPoint.y)/length2D(s.ePoint-s.sPoint);
                    });
               }
               else if(isCircle)
               {
                   circlePoints=contours[i];
                   //Point Hough/
                   int PointNumber=static_cast<int>(circlePoints.size()/3-1);
                   int SumNumber=0;
                   Point2f Point1;
                   Point2f Point2;
                   Point2f Point3;
                   double k12,k23,d12,d23;
                   double Sumac=0,Sumbc=0,Sumrc=0;
                   Round tempRound;
                   for(size_t i=0;i<PointNumber;i++)
                   {
                       Point1=circlePoints[i];
                       Point2=circlePoints[i+PointNumber];
                       Point3=circlePoints[i+2*PointNumber];
                       if((Point1.y!=Point2.y)&&(Point2.y!=Point3.y))
                       {
                           k12=-(Point1.x-Point2.x)/(Point1.y-Point2.y);
                           d12=(Point1.y+Point2.y-k12*(Point1.x+Point2.x))/2;
                           k23=-(Point2.x-Point3.x)/(Point2.y-Point3.y);
                           d23=(Point2.y+Point3.y-k23*(Point2.x+Point3.x))/2;
                           double ac=-(d12-d23)/(k12-k23);
                           double bc=k23*ac+d23;
                           double rc=sqrt((Point1.x-ac)*(Point1.x-ac)+(Point1.y-bc)*(Point1.y-bc));
                           //Average value
                           Sumac+=ac;
                           Sumbc+=bc;
                           Sumrc+=rc;
                           SumNumber+=1;
                       }
                   }
                   tempRound.center=Point2f(Sumac/SumNumber,Sumbc/SumNumber);
                   tempRound.radius=Sumrc/SumNumber;
                   rounds.push_back(tempRound);
               }
               contoursSize++;
           }
        }
        qDebug()<<"The Number Of Contours "<<contoursSize<<endl;
    }
void FeatureExtractor::detect(Criterion* criterion,const cv::Mat& srcImg)
{
    Mat predSrc;
    Mat srcImg0=srcImg.clone();
    Pretreatment pre;
    pre.run(srcImg,predSrc);
    Canny(predSrc,predSrc,3,6,3);
    imwrite("canny.jpg",predSrc);
    vector<Point2f> Points;
    for(int i=10;i<predSrc.rows-10;i++)
    {
        const uchar* data=predSrc.ptr<uchar>(i);
        for(int j=10;j<predSrc.cols-10;j++)
        {
            if(data[j]==255)
            {
                Points.push_back(Point2f(j,i));
            }
        }
    }
    //
    qDebug()<<criterion->criterionList.size()<<endl;
    for(size_t i=0;i<criterion->criterionList.size();++i)
    {
        CriterionBase* singleRule  =   criterion->criterionList[i];
        switch(singleRule->getType()){
        case twoLineRelation:
        {
            TwoLineCriterion* tlc=dynamic_cast<TwoLineCriterion*> (singleRule);
            fitLinedetect(tlc->_fLine,Points);
           //line(srcImg0,tlc->_fLine.sPoint,tlc->_fLine.ePoint,Scalar(0,255,0),3,8,0);
            line(srcImg0,lines[0].sPoint,lines[0].sPoint+1000*lines[0].direction,Scalar(0,0,255),3,8,0);
            fitLinedetect(tlc->_sLine,Points);
          //line(srcImg0,tlc->_sLine.sPoint,tlc->_sLine.ePoint,Scalar(0,255,0),3,8,0);
            line(srcImg0,lines[1].sPoint,lines[1].sPoint+1000*lines[1].direction,Scalar(0,0,255),3,8,0);
            imwrite("srcImg.jpg",srcImg0);
        }
            break;
        case circleCharacter:
        {
            CircleCriterion* cc=dynamic_cast<CircleCriterion*> (singleRule);
            vector<Point2f> detectPoints;
            qDebug()<<cc->_radiu;
            for(int j=0;j<Points.size();j++)//the diffrent i j;
            {
                if(abs((length2D(Points[j]-cc->_center)-cc->_radiu))<detectCircleMinDis)
                {
                    detectPoints.push_back(Points[j]);
                    //circle(srcImg0,Points[j],3,Scalar(0,255,0),-1,8,0);
                }
            }
            detectCircle(detectPoints);
        }
            break;
        default:
            qDebug()<<"detect failed";
            break;
        }
    }
}
void FeatureExtractor::fitLinedetect(Segment detectedline,vector<Point2f> Points)
{
    vector<Point2i> detectPoints;
    Segment s;
    Vec4f detectLine;
    float middleY=(detectedline.sPoint+detectedline.ePoint).y/2;
    for(int i=0;i<Points.size();i++)
    {
       //use triangle three lines to judge

        float linef1=length2D(Points[i]-detectedline.sPoint);
        float linef2=length2D(Points[i]-detectedline.ePoint);
        float linef0=length2D(detectedline.sPoint-detectedline.ePoint);
        float Dis=qAbs(crossProduct2D(detectedline.direction,(Points[i]-detectedline.sPoint)));
       //I should just get the middle of the line

        if((Dis<detectLineMinDis)&&(linef1>linef0/6)&&(linef2>linef0/6))
        {
            detectPoints.push_back(Points[i]);
        }

    }
    cv::fitLine(detectPoints,detectLine,CV_DIST_L1,0,0.01,0.01);
    s.direction=Point2f(detectLine[0],detectLine[1]);
    float middleX=(middleY-detectLine[3])/(detectLine[1]/detectLine[0])+detectLine[2];
    s.sPoint=Point2f(middleX,middleY);
    lines.push_back(s);
}
void FeatureExtractor::detectCircle(std::vector<Point2f> Points)
{
           Round r;
           for(int i=0;i<1;i++)
           {
               double sumX=0,sumX2=0,sumY=0,sumXY=0,sumY2=0,
                   sumX3=0,sumY3=0,sumX2Y=0,sumXY2=0,
                   C,D,E,G,H;
                int N=Points.size();
                for_each(Points.begin(),Points.end(),
                         [&](const Point2f& P){
                    sumX+=P.x;
                    sumX2+=P.x*P.x;
                    sumY+=P.y;
                    sumXY+=(P.x*P.y);
                    sumX3+=P.x*P.x*P.x;
                    sumY3+=P.y*P.y*P.y;
                    sumX2Y+=P.x*P.x*P.y;
                    sumXY2+=P.x*P.y*P.y;
                    sumY2+=P.y*P.y;
                });
                C=N*sumX2-sumX*sumX;
                D=N*sumXY-sumX*sumY;
                E=N*sumX3+N*sumXY2-(sumX2+sumY2)*sumX;
                G=N*sumY2-sumY*sumY;
                H=N*sumX2Y+N*sumY3-(sumX2+sumY2)*sumY;
                double a=(H*D-E*G)/(C*G-D*D);
                double b=(H*C-E*D)/(D*D-C*G);
                double c=(sumX2+sumY2+a*sumX+b*sumY)/(-N);
                r.center.x=a/(-2);
                r.center.y=b/(-2);
                r.radius=sqrt(a*a+b*b-4*c)/2;
                vector<Point2f>::iterator it=Points.begin();
                while(it!=Points.end())
                {
                    double distance=sqrt(pow(((*it).x-r.center.x),2)+pow(((*it).y-r.center.y),2));
                    if(abs(distance-r.radius)>5)
                    {
                        it=Points.erase(it);
                    }
                    else
                    {
                        it++;
                    }
                }
           }
           rounds.push_back(r);
}
void FeatureExtractor::extractBlindHole(const Mat &srcImg)
{
        Canny(srcImg,srcImg,3,6,3);
        vector<Point2f> Points;
        for(size_t i=10;i<srcImg.rows-10;i++)
        {

            const uchar* data=srcImg.ptr<uchar>(i);
            for(size_t j=10;j<srcImg.cols-10;j++)
            {
                if(data[j]==255)
                {
                    Points.push_back(Point2f(j,i));
                   // qDebug()<<Points.back().x<<" "<<Points.back().y<<endl;
                }

            }
        }
        //
        qDebug()<<srcImg.rows*srcImg.cols<<"Points Numbers"<<Points.size()<<endl;
        //test
        detectCircle(Points);

}
double FeatureExtractor::changeAngle(int y1, int y2, int x1, int x2, double SlopeCurrent)
{

    if((y1<y2)&&(SlopeCurrent>0))
    {
        SlopeCurrent=SlopeCurrent;
    }
    else if((y1<y2)&&(SlopeCurrent<0))
    {
        SlopeCurrent+=180;
    }
    else if((y1>y2)&&(SlopeCurrent>0))
    {
        SlopeCurrent+=180;
    }
    else if((y1>y2)&&(SlopeCurrent<0))
    {
        SlopeCurrent+=360;
    }
    else if((y1==y2)&&(x1>=x2))
    {
        SlopeCurrent=180;
    }
    else if((y1==y2&&x1<x2))
    {
        SlopeCurrent=0;
    }
    else
    {
            qDebug()<<"Chang Slope failed!";
    }
    if((x1==x2)&&(y1<y2))
    {
        SlopeCurrent=90;
    }
    if((x1==x2)&&(y1>y2))
    {
        SlopeCurrent=270;
    }
    return SlopeCurrent;
}
