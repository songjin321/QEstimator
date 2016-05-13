#include "CriterionManager.h"
#include <algorithm>
#include <QMessageBox>
#include "ioxml.h"
using namespace std;

CriterionManager* CriterionManager::_instance   =   nullptr;

CriterionManager::~CriterionManager(){
    clear();
}

void CriterionManager::clear(){
//    for_each(criterionMap.begin(),criterionMap.end(),
//             [&](/*pair<string, Criterion*>& p*/type){
//                if(p.second)
//                    delete p.second;
//            });
    for(auto &ele : criterionMap){
        if(ele.second)
            delete ele.second;
    }
    criterionMap.clear();
}

bool CriterionManager::addCriterion(Criterion *c){
    if(c->header.name.empty() ||
            criterionMap.find(c->header.name) != criterionMap.end())
        return false;//no key for map || key already exist
    //simply insert
    bool isInsert= criterionMap.insert(make_pair(c->header.name,c)).second;
    return isInsert;
}

void CriterionManager::removeCriterion(const string& name){
    criterionMap.erase(name);
}

Criterion* CriterionManager::getCurrentCriteion(){
    return currentCriterion;
}

bool CriterionManager::setCurrentCriterion(const string &name){
    if(criterionMap.find(name) == criterionMap.end())
        return false;

    currentCriterion    =   criterionMap[name];
    return true;
}

//output
void CriterionManager::outputToFile(const string &file,string name){
    IoXml ioXml;
    string criterionsfile="../criterionsfile/"+file+".xml";
    auto theCriterion   =   criterionMap.find(name);
    if(theCriterion == criterionMap.end()){
        for(auto& item : criterionMap)
           // item.second->output(criterionsfile);
            ioXml.outputFileAsXML(criterionsfile,item.second);
    }else{
        //theCriterion->second->output(criterionsfile);
        ioXml.outputFileAsXML(criterionsfile,theCriterion->second);
    }
}
#if 0
void CriterionManager::outputToFile(ofstream &fout,string name){
    auto theCriterion   =   criterionMap.find(name);
    if(theCriterion == criterionMap.end()){
        for(auto& item : criterionMap)
            item.second->output(fout);
    }else{
        theCriterion->second->output(fout);
    }
}
#endif
//input
void CriterionManager::inputFromFile(const std::string& file,Criterion* criterion)
{
    IoXml ioXml;
    ioXml.inputFromXML(file,criterion);
}







