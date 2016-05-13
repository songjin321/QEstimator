#include "LogContainer.h"
#include <ctime>

using namespace std;

LogContainer::LogContainer(){
    init();
}

LogContainer::~LogContainer(){}

void LogContainer::exportLog(){
}

void LogContainer::clear(){

}

string LogContainer::getLocalTimeStr(){
    time_t t    =   time(0);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y/%m/%d %X",localtime(&t));
    return string(tmp);
}

void LogContainer::init(){
    logFile =   "../log.txt";
    esstialInformation  =   {
        "",
        0,
        0,
        "unknow",
        "unknow"
    };
}
