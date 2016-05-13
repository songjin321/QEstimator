#include "ioxml.h"
#include "tiXml/tinystr.h"
#include "tiXml/tinyxml.h"
#include "common.h"
#include "QDebug"
IoXml::IoXml()
{

}
bool IoXml::outputFileAsXML(const std::string& fileName, Criterion* criterion)
{
    TiXmlDocument *pDoc=new TiXmlDocument;
    TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","0","0");
    pDoc->LinkEndChild(pDeclaration);
    TiXmlElement *pRootEle=new TiXmlElement("criterion");
    pDoc->LinkEndChild(pRootEle);
    TiXmlElement *pHeader=new TiXmlElement("Header");
    pRootEle->LinkEndChild(pHeader);
    pHeader->SetAttribute("name",criterion->header.name);
    pHeader->SetDoubleAttribute("PPM",criterion->header.PPM);

    TiXmlElement *pHFLine=new TiXmlElement("fLine");
    pHeader->LinkEndChild(pHFLine);
    outputSegment(criterion->header.fLine,pHFLine);

    TiXmlElement *pHSLine=new TiXmlElement("sLine");
    pHeader->LinkEndChild(pHSLine);
    outputSegment(criterion->header.sLine,pHSLine);

    TiXmlElement *pBasePoint=new TiXmlElement("basePoint");
    pHeader->LinkEndChild(pBasePoint);
    pBasePoint->SetDoubleAttribute("BasePointX",criterion->header.basePoint.x);
    pBasePoint->SetDoubleAttribute("BasePointY",criterion->header.basePoint.y);
    for(CriterionBase* cb : criterion->criterionList)
    {
       qDebug()<<"TwoLine"<<endl;
       qDebug("%p", cb);
       qDebug("type:%d", cb->getType());
       // TwoLineCriterion* tlc=static_cast<TwoLineCriterion*> (cb);
       cb->outputAsXML(pRootEle);
   }
    pDoc->SaveFile(fileName);
    return true;
}
void IoXml::inputFromXML(const std::string &fileName, Criterion *criterion)
{
    TiXmlDocument *pDoc=new TiXmlDocument();
    if(!pDoc->LoadFile(fileName))
    {
        qDebug()<<"get xml file failed!";
    }
    TiXmlElement *pRootEle=pDoc->RootElement();
    TiXmlHandle hRoot(pRootEle);
    //header
    {
        TiXmlElement *pHeader=hRoot.FirstChildElement("Header").Element();
        qDebug()<<pHeader->Value();
        pHeader->QueryFloatAttribute("PPM",&(criterion->header.PPM));
        pHeader->QueryStringAttribute("name",&(criterion->header.name));
        TiXmlElement *pHFLine=pHeader->FirstChildElement("fLine");
        getSegment(criterion->header.fLine,pHFLine);
        TiXmlElement *pHSLine=pHeader->FirstChildElement("sLine");
        getSegment(criterion->header.sLine,pHSLine);
        TiXmlElement *pHBasePoint=pHeader->FirstChildElement("basePoint");
        pHBasePoint->QueryFloatAttribute("BasePointX",&(criterion->header.basePoint.x));
        pHBasePoint->QueryFloatAttribute("BasePointY",&(criterion->header.basePoint.y));
    }
    //two line
    {
        TiXmlElement *pTlc=hRoot.FirstChildElement("TwoLineCirterion").FirstChildElement().Element();
        for(;pTlc;pTlc=pTlc->NextSiblingElement())
        {
            TwoLineCriterion* tlc   =   new TwoLineCriterion;
            TiXmlElement *pTFLine=pTlc->FirstChildElement("fLine");
            getSegment(tlc->_fLine,pTFLine);
            TiXmlElement *pTSLine=pTlc->FirstChildElement("sLine");
            getSegment(tlc->_sLine,pTSLine);
            TiXmlElement *pTAngle=pTlc->FirstChildElement("angle");
            pTAngle->QueryFloatAttribute("upperAngle",&(tlc->_angle.upper));
            pTAngle->QueryFloatAttribute("lowerAngle",&(tlc->_angle.lower));
            TiXmlElement *pTDistances=pTlc->FirstChildElement("distances");
            TiXmlElement *pTDistance=pTDistances->FirstChildElement();
            for(;pTDistance;pTDistance=pTDistance->NextSiblingElement())
            {
                Range r;
                pTDistance->QueryFloatAttribute("upper",&(r.upper));
                pTDistance->QueryFloatAttribute("lower",&(r.lower));
                tlc->_distances.push_back(r);
            }
            TiXmlElement *pTDisRankNum=pTlc->FirstChildElement("disRankNum");
            pTDisRankNum->QueryIntAttribute("value",&(tlc->disRankNum));
            criterion->criterionList.push_back(tlc);
        }
    }
    //circle
    {
        TiXmlElement *pCc=hRoot.FirstChildElement("CircleCirterion").FirstChildElement().Element();
        for(;pCc;pCc=pCc->NextSiblingElement())
        {
            CircleCriterion* Cc=new CircleCriterion;
            TiXmlElement *pCircle=pCc->FirstChildElement("circle");
            pCircle->QueryFloatAttribute("centerX",&(Cc->_center.x));
            pCircle->QueryFloatAttribute("centerY",&(Cc->_center.y));
            pCircle->QueryFloatAttribute("radiu",&(Cc->_radiu));

            TiXmlElement *pRadiusRange=pCc->FirstChildElement("radiusRange");
            pRadiusRange->QueryFloatAttribute("upper",&(Cc->_radius.upper));
            pRadiusRange->QueryFloatAttribute("lower",&(Cc->_radius.lower));
            criterion->criterionList.push_back(Cc);
        }
    }
}
bool IoXml::GetNodeByName(const std::string &Name, TiXmlElement* &node, TiXmlElement * rootEle)
{
    if(rootEle->Value()==Name)
    {
        node=rootEle;
        return true;
    }
    TiXmlElement* pEle=rootEle;
    for(pEle=rootEle->FirstChildElement();pEle;pEle=pEle->NextSiblingElement())
    {
        if(GetNodeByName(Name,node,pEle))
        {
                return true;
        }
    }
    return false;
}
void IoXml::outputSegment(Segment& s,TiXmlElement *pLine)
{
    pLine->SetDoubleAttribute("sPointX",s.sPoint.x);
    pLine->SetDoubleAttribute("sPointY",s.sPoint.y);
    pLine->SetDoubleAttribute("ePointX",s.ePoint.x);
    pLine->SetDoubleAttribute("ePointY",s.ePoint.y);
    pLine->SetDoubleAttribute("directionX",s.direction.x);
    pLine->SetDoubleAttribute("directionY",s.direction.y);
}
void IoXml::getSegment(Segment& s,TiXmlElement *pLine)
{
    pLine->QueryFloatAttribute("sPointX",&(s.sPoint.x));
    pLine->QueryFloatAttribute("sPointY",&(s.sPoint.y));
    pLine->QueryFloatAttribute("ePointX",&(s.ePoint.x));
    pLine->QueryFloatAttribute("ePointY",&(s.ePoint.y));
    pLine->QueryFloatAttribute("directionX",&(s.direction.x));
    pLine->QueryFloatAttribute("directionY",&(s.direction.y));
}

















