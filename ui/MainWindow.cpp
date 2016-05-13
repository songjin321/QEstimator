#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QtWidgets>
#include "../EstimateSystem.h"
#include "PylonCamera.h"
int detecttimes=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frame(nullptr),
    _system(nullptr),
    isSystemRunning(false),
    isRun(false),
    sleep_time(9*1000.0f),
    detect_time(100.0f),
    screenShotTimes(0)

{
    ui->setupUi(this);

    //prepare for camera
    scene   =   new QGraphicsScene(this);
    ui->gvMain->setScene(scene);
    ptrCamera  =   new PylonCamera;
    refreshTimer    =   new QTimer;
    refreshTimer->setInterval(33);//set fixed fps, may change by slot
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(updateFrame()));

    //init list
    _criterionListWidget    =   CriterionListWidget::getInstance();
    _criterionListWidget->setMainWindow(this);
    connect(ui->actionCriterionList,SIGNAL(toggled(bool)),_criterionListWidget,SLOT(setVisible(bool)));
    connect(_criterionListWidget,SIGNAL(createNewCriterion(QString)),this,SLOT(on_createNewCriterion(QString)));
    emit ui->actionCriterionList->toggled(true);
    ui->actionCriterionList->setChecked(true);

    //init editor
    _criterionEditor    =   new CriterionEditor;
    connect(ui->actionCiterionEditor,SIGNAL(toggled(bool)),_criterionEditor,SLOT(setVisible(bool)));
    connect(_criterionEditor,SIGNAL(refreshImage()),this,SLOT(on_RefreshImage()));

    //
    //currentFrame.load("../QualityEstimator/source.jpg");

    //connet judge, called when system active
    judgeTimer  =   new QTimer;
    judgeTimer->setInterval(sleep_time);
    connect(judgeTimer,SIGNAL(timeout()),this,SLOT(beginJudge()));


    detectTimer =   new QTimer;
    detectTimer->setInterval(detect_time);
    connect(detectTimer,SIGNAL(timeout()),this,SLOT(judgeCurrentPiece()));

}

MainWindow::~MainWindow()
{
    if(_system)
        delete _system;
    if(ptrCamera)
        delete ptrCamera;

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    //close all widgets
    _criterionListWidget->close();
    _criterionEditor->close();

    QMainWindow::closeEvent(e);

}
void MainWindow::stopUpdateFrame()
{

}

void MainWindow::on_actionRun_triggered()
{
        qDebug()<<"system run"<<endl;
#if 0
    if(!_system)
        _system =   new EstimateSystem;

    _system->calcPixelsPerMilimeter(fromQImage2Mat(currentFrame));
    _system->runWithPicture(fromQImage2Mat(currentFrame));
//    WorkpieceEstimator* we  =   new WorkpieceEstimator;
//    bool re =   we->judgeTest();
//    qDebug()<<(re?"qualified!":"unqualified...");

#else
    if(!isRun)
    {
        ptrCamera->beginCapture();
        refreshTimer->start();
        //Modify fit the camera BY SJ
        currentFrame    =   ptrCamera->captureImage();
        if(currentFrame.isNull())
        {
            return;
        }
        float rw    =   float(ui->gvMain->width())/currentFrame.width();
        float rh    =   float(ui->gvMain->height())/currentFrame.height();
        float r =   rw>rh?rh:rw;
        qDebug()<<"ui.width="<<ui->gvMain->width()<<"ui.height="<<ui->gvMain->height();
        qDebug()<<"rw="<<rw<<"rh="<<rh;
        ui->gvMain->scale(r,r);
        isRun=true;
    }
#endif
}
void MainWindow::on_actionDetect_triggered()
{
    if(!_system)
        _system =   new EstimateSystem;

    isSystemRunning =   !isSystemRunning;
    if(isSystemRunning){
        beginJudge();
        judgeTimer->start();
    }else{
        judgeTimer->stop();
    }
}

void MainWindow::beginJudge(){
    qDebug()<<"now judging";
    if(currentFrame.isNull()){
        //qDebug()<<"current frame invalid!";
        QMessageBox::warning(nullptr,tr("warning"),tr("invalid frame!"));
        return;
    }
    detectTimer->start();

    //imwrite("../current_notInput.jpg",currentFrame);

}
void MainWindow::judgeCurrentPiece(){

    detecttimes++;
    if(detecttimes==DTIMES)
    {
        detectTimer->stop();

    }    
    _system->runWithPicture(fromQImage2Mat(currentFrame));
}

void MainWindow::updateFrame(){
    //static int i= 0;
    //qDebug()<<"updateing"<<++i;
    currentFrame    =   ptrCamera->captureImage();
    if(!currentFrame.isNull()){
        if(frame){
            scene->removeItem(frame);
            delete frame;
        }
        frame   =   scene->addPixmap(QPixmap::fromImage(currentFrame));
        scene->setSceneRect(0,0,currentFrame.width(),currentFrame.height());
    }else
        qDebug()<<"frame get failed!";
}

void MainWindow::on_createNewCriterion(QString name){
    //load local contour here
    _criterionEditor->setImage(QPixmap::fromImage(currentFrame),true);

    _criterionEditor->setCriterionName(name);
    _criterionEditor->show();
    _criterionEditor->raise();

}

void MainWindow::on_RefreshImage(){
    if(!frame){
        QMessageBox::information(this,tr("warnning"),tr("no frame captured!"),QMessageBox::Ok);
        return;
    }

    _criterionEditor->setImage(frame->pixmap());

}



void MainWindow::on_actionZoon_in_triggered()
{
    ui->gvMain->scale(1/1.2,1/1.2);
    qDebug()<<ui->gvMain->width()<<" "<<ui->gvMain->height();
}

void MainWindow::on_actionZoom_out_triggered()
{
    ui->gvMain->scale(1.2,1.2);
    qDebug()<<ui->gvMain->width()<<" "<<ui->gvMain->height();
}

void MainWindow::on_actionSelect_Camare_triggered()
{

}

void MainWindow::on_actionScreen_shot_triggered()
{
    screenShotTimes++;
    cv::Mat currentMatFrame;
    currentMatFrame=fromQImage2Mat(currentFrame);
    char name[10];
    std::sprintf(name,"%d.jpg",screenShotTimes);// = screenShotTimes
    imwrite(name,currentMatFrame);
}

void MainWindow::grabStop(){
    refreshTimer->stop();
}

void MainWindow::grabStart(){
    refreshTimer->start();
}

void MainWindow::on_btnSysRun_clicked(){
    qDebug()<<"on push button"<<endl;
    emit ui->actionRun->triggered();
}
