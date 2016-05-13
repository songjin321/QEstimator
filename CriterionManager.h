#pragma once

//declar CriterionManager.
//manage all criterions may used, singleton partten

#include "common.h"
#include "Criterion.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>

class CriterionManager
{
    static CriterionManager* _instance;
    CriterionManager():currentCriterion(nullptr){}

public:
    ~CriterionManager();

    void clear();
    bool addCriterion(Criterion* c);
    void removeCriterion(const std::string& name);
    Criterion* getCurrentCriteion();
    bool setCurrentCriterion(const std::string& name);

    static CriterionManager* getInstance(){
        if(!_instance)
            _instance   =   new CriterionManager;
        return _instance;
    }

    //if criterion with 'name' do not exist, all criterions will be exported
    //void outputToFile(std::ofstream& fout,std::string name = "");
    void outputToFile(const std::string& file,std::string name = "");


    //get criteron for file
    void inputFromFile(const std::string& file,Criterion* criterion);

private:
    std::map<std::string, Criterion*> criterionMap;
    //std::string currentName;//current used criterion's name
    Criterion* currentCriterion;

    //

};
