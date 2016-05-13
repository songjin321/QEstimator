#include <QtWidgets/QApplication>
#include "ui/MainWindow.h"
#include <QDebug>
#include <ctime>


#include <pylon/PylonIncludes.h>

using namespace Pylon;

using namespace std;

int main(int argc, char** argv){
    QApplication app(argc, argv);

    auto c  =   clock();
    for(int i=0;i>0;--i){
        qDebug()<<i<<endl;

        while(clock()-c < CLOCKS_PER_SEC){}

        c   =   clock();
    }
    MainWindow* mw  =   new MainWindow;
    mw->show();

    mw->on_actionRun_triggered();

    return app.exec();
}
