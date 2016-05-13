#include "Criterion.h"
#include <algorithm>
#include "ioxml.h"
#include <QDebug>
using namespace std;

Criterion::~Criterion(){
    for_each(criterionList.begin(),criterionList.end(),
             [&](CriterionBase*& cb){
                if(cb)
                    delete cb;
            });
}

/*
 * CriterionBase child
*/

//two line relation

TwoLineCriterion::TwoLineCriterion(bool check)
    :CriterionBase(twoLineRelation,check),
      _fLine(),
      _sLine()
{
    _angle  =   {-1,-1};
}
void TwoLineCriterion::outputAsXML(TiXmlElement *pRootEle)
{
    IoXml ioXml;
    TiXmlElement *pTwoLineCirterion=new TiXmlElement("");
    if(!ioXml.GetNodeByName("TwoLineCirterion",pTwoLineCirterion,pRootEle))
    {
        pTwoLineCirterion->SetValue("TwoLineCirterion");
        pRootEle->LinkEndChild(pTwoLineCirterion);
    }
    TiXmlElement *pTlc=new TiXmlElement("Tlc");
    pTwoLineCirterion->LinkEndChild(pTlc);


    TiXmlElement *pFLine=new TiXmlElement("fLine");
    pTlc->LinkEndChild(pFLine);
    ioXml.outputSegment(_fLine,pFLine);

    TiXmlElement *pSLine=new TiXmlElement("sLine");
    pTlc->LinkEndChild(pSLine);
    ioXml.outputSegment(_sLine,pSLine);

    TiXmlElement *pAngle=new TiXmlElement("angle");
    pTlc->LinkEndChild(pAngle);
    pAngle->SetDoubleAttribute("upperAngle",_angle.upper);
    pAngle->SetDoubleAttribute("lowerAngle",_angle.lower);

    TiXmlElement *pDistances=new TiXmlElement("distances");
    pTlc->LinkEndChild(pDistances);
    for(int i=0;i<disRankNum;i++)
    {
        char name[10];
        std::sprintf(name,"distance%d",i);
        TiXmlElement *pDistance=new TiXmlElement(name);
        pDistance->SetDoubleAttribute("upper",_distances[i].upper);
        pDistance->SetDoubleAttribute("lower",_distances[i].lower);
        pDistances->LinkEndChild(pDistance);
    }

    TiXmlElement *pDisRankNum=new TiXmlElement("disRankNum");
    pTlc->LinkEndChild(pDisRankNum);
    pDisRankNum->SetAttribute("value",disRankNum);
}

//circle
CircleCriterion::CircleCriterion(bool check)
    :CriterionBase(circleCharacter,check)
{
    _radius =   {-1,-1};
}

void CircleCriterion::outputAsXML(TiXmlElement *pRootEle)
{
    IoXml ioXml;
    TiXmlElement *pCircleCirterion=new TiXmlElement("");
    if(!ioXml.GetNodeByName("CircleCirterion",pCircleCirterion,pRootEle))
    {
        pCircleCirterion->SetValue("CircleCirterion");
        pRootEle->LinkEndChild(pCircleCirterion);
    }
    TiXmlElement *pCc=new TiXmlElement("Cc");
    pCircleCirterion->LinkEndChild(pCc);


    TiXmlElement *pCircle=new TiXmlElement("circle");
    pCc->LinkEndChild(pCircle);
    pCircle->SetDoubleAttribute("centerX",_center.x);
    pCircle->SetDoubleAttribute("centerY",_center.y);
    pCircle->SetDoubleAttribute("radiu",_radiu);

    TiXmlElement *pRadiusRange=new TiXmlElement("radiusRange");
    pCc->LinkEndChild(pRadiusRange);
    pRadiusRange->SetDoubleAttribute("upper",_radius.upper);
    pRadiusRange->SetDoubleAttribute("lower",_radius.lower);

}
