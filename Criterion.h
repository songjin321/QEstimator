#pragma once

//define criterions struct
//these class should be handled by POINTER
    //CriterionBase memory release by Criterion, whose memory release by CriterionManager

#include "common.h"
#include <string>
#include <vector>
#include "tiXml/tinystr.h"
#include "tiXml/tinyxml.h"
//CriterionBase: supper class for criterion elements

enum CriterionType{
    twoLineRelation,
    circleCharacter
};

class CriterionBase
{
protected:
    //construction ban
    CriterionBase(CriterionType type, bool check = true)
        :_type(type),_check(check){}

public:
    virtual ~CriterionBase(){}

    //discribe float range of value


    virtual void outputAsXML(TiXmlElement* ){}

    CriterionType getType(){ return _type; }
    bool needCheck(){ return _check; }
    void setCheck(bool check){ _check = check; }

protected:
    CriterionType _type;
    bool _check;
};

//single criterion
class Criterion
{
    //ban to create an object, use pointer instead!
    Criterion(){ header.PPM = 1.0f; }
public:
        static Criterion* create(){
        return new Criterion;
    }

    ~Criterion();//CriterionBase memory release here

    struct{
        Segment fLine,sLine;
        float PPM;
        cv::Point2f basePoint;
        std::string name;
    }header;
    std::vector<CriterionBase* > criterionList;
};

//! 20151004
//! now I wonder if I should have set ~Criterion() as a private attribute of Criterion
//!


/*
 * specific criterion element describe follows:
*/

class TwoLineCriterion:  public CriterionBase
{
public:
    TwoLineCriterion(bool check = true);

    //line discribe by its direction and an anchorpoint
    Segment _fLine;
    Segment _sLine;
    std::vector<Range> _distances;
    Range _angle;
    int disRankNum;
    void outputAsXML(TiXmlElement *pRootEle) override;

//    void output(std::ofstream& fout){
//        CriterionBase::output(fout);
//        fout<<_fLine.sPoint.x<<" "<<_fLine.sPoint.y<<" "<<_fLine.ePoint.x<<" "<<_fLine.ePoint.y<<"\n"
//            <<_sLine.sPoint.x<<" "<<_sLine.sPoint.y<<" "<<_sLine.ePoint.x<<" "<<_sLine.ePoint.y<<"\n"
//            //<<_distances.lower<<" "<<_distances.upper<<"\n"
//            <<_angle.lower<<" "<<_angle.upper<<"\n";
//    }
};

class CircleCriterion:  public CriterionBase
{
public:
    CircleCriterion(bool check = true);

    //circle
    cv::Point2f _center;
    Range _radius;
    float _radiu;
    cv::Rect _box;
    void outputAsXML(TiXmlElement *pRootEle);
//    void output(std::ofstream &fout){
//        CriterionBase::output(fout);
//        fout<<_center.x<<" "<<_center.y<<"\n"
//            <<_radius.lower<<" "<<_radius.upper<<"\n";
//    }
};
