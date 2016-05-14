#include <QtWidgets/QApplication>
#include "ui/MainWindow.h"
#include <QDebug>
#include <ctime>


#include <pylon/PylonIncludes.h>

using namespace Pylon;

using namespace std;

int main(int argc, char** argv){
    QApplication app(argc, argv);

    MainWindow* mw  =   new MainWindow;
    mw->show();
    return app.exec();
}
