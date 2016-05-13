#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CriterionListWidget.h"
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QTimer>
#include "CriterionEditor.h"
#include "common.h"

namespace Ui {
class MainWindow;
}

class EstimateSystem;

class CaptureDevice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void grabStop();
    void grabStart();

protected:
    void closeEvent(QCloseEvent* e);

public slots:
    void judgeCurrentPiece();
    void beginJudge();
    void stopUpdateFrame();
    void on_actionRun_triggered();
    void on_actionDetect_triggered();

    void on_createNewCriterion(QString name);

    //refresh editor's image, deliver current frame
    void on_RefreshImage();

    //timeout callback
    void updateFrame();

    //camera viewport

    void on_actionZoon_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionSelect_Camare_triggered();

    void on_actionScreen_shot_triggered();

    void on_btnSysRun_clicked();

public:
//private:
    Ui::MainWindow *ui;

    QGraphicsPixmapItem* frame;
    QGraphicsScene* scene;//display frame
    CaptureDevice* ptrCamera;
    QTimer* refreshTimer;
    QTimer* judgeTimer;
    QTimer* detectTimer;
    //the only instance of list
    CriterionListWidget* _criterionListWidget;
    CriterionEditor* _criterionEditor;

    EstimateSystem* _system;

    //test img
    //once the camera ready, this should equivalent to expression: frame->pixmap().toImage();
    QImage currentFrame;

    bool isSystemRunning;
    bool isRun;

    //
    float sleep_time;
    float detect_time;
    //
    int screenShotTimes;
    //

};

#endif // MAINWINDOW_H
