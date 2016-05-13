#pragma once

//LogContainer, reserve debug log and output result
    //1 esstial information
    //2 current criterion, get from manager
    //3 estimate result
        //may hard code

#include <string>

struct LogContainer
{
    LogContainer();
    ~LogContainer();

    static std::string getLocalTimeStr();
    void exportLog();
    void clear();

    //file
    std::string logFile;

    //data
    struct{
        std::string statement;
        int goodsTotal;
        int perfectCount;
        std::string startTime;
        std::string endTime;
    }esstialInformation;



    void init();
};
