#ifndef IOXML_H
#define IOXML_H
#include "common.h"
#include "Criterion.h"
#include "tiXml/tinystr.h"
#include "tiXml/tinyxml.h"
class IoXml
{
public:
    IoXml();
    bool outputFileAsXML(const std::string& fileName, Criterion* criterion);
    void inputFromXML(const std::string& fileName,Criterion* criterion);
    bool GetNodeByName(const std::string& Name, TiXmlElement *&node, TiXmlElement *rootEle);
    void outputSegment(Segment &s, TiXmlElement *pLine);
    void getSegment(Segment& s,TiXmlElement *pLine);
};

#endif // IOXML_H
