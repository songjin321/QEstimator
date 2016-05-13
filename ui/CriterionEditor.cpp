#include "CriterionEditor.h"
#include "ui_CriterionEditor.h"
#include "Criterion.h"
#include "ImageContainer.h"
#include <QtWidgets>
#include "common.h"
#include "UtilityWidgets.h"
#include "CriterionListWidget.h"
#include "WorkpieceEstimator.h"
CriterionEditor::CriterionEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CriterionEditor),
    singleRule(nullptr),
    isBaseCircle(false)
{
    ui->setupUi(this);

    statusLabel =   new QLabel(tr("ready"));
    ui->statusbar->addWidget(statusLabel);

    connect(ui->imgContainer,SIGNAL(showInfo(QString)),statusLabel,SLOT(setText(QString)));

    currentCriterion    =   Criterion::create();
    shallRelease    =   true;
    ui->ledtName->setText(QString::fromStdString(currentCriterion->header.name));
    ui->ledtRule->setText("0,0 0x0");

    //imageContainer
    ui->imgContainer->pFather   =   this;

    connect(ui->ledtName,SIGNAL(textChanged(QString)),this,SLOT(setCriterionName(QString)));
}

CriterionEditor::~CriterionEditor()
{
    delete ui;

    if(shallRelease && currentCriterion)
        delete currentCriterion;
}

void CriterionEditor::setImage(QPixmap img, bool dispFeature){
    ui->imgContainer->setImage(img,dispFeature);
}

void CriterionEditor::setCriterionName(QString name){
    ui->ledtName->setText(name);
    currentCriterion->header.name   =   name.toStdString();
}



void CriterionEditor::on_actionRule_triggered()
{
#if 0
    ui->imgContainer->setState(ImageContainer::SetBlindBox);

    statusLabel->setText(tr("set rubber box"));
#else
    if(ui->imgContainer->lines.size() < 2){
        QMessageBox::information(nullptr,tr("Operation FAILED"),
                                 tr("operation DECLINE because less than 2 lines was detected! consider refresh"));
        return;
    }

    ui->imgContainer->setState(ImageContainer::SelectLine);
    statusLabel->setText(tr("chose a line"));

    currentCriterion->header.fLine.ePoint   =  cv:: Point2f(0.0f,0.0f);//make invalid
    connect(ui->imgContainer,SIGNAL(lineSelected(Segment)),this,SLOT(onSettingRuleCriterion(Segment)));

#endif
}
void CriterionEditor::on_actionRefresh_triggered()
{
    emit refreshImage();

    statusLabel->setText(tr("update workpiece image"));
}

void CriterionEditor::on_actionTwoLine_triggered()
{
    if(ui->imgContainer->lines.size() < 2){
        QMessageBox::information(nullptr,tr("Operation FAILED"),
                                 tr("operation DECLINE because less than 2 lines was detected!"));
        return;
    }
    ui->imgContainer->setState(ImageContainer::SelectLine);
    statusLabel->setText(tr("chose a line"));

    //avoid memory leak due to operator switch
    if(singleRule)
        delete singleRule;

    singleRule  =   new TwoLineCriterion;
    connect(ui->imgContainer,SIGNAL(lineSelected(Segment)),this,SLOT(onSelectingTwoLineRelation(Segment)));
}

void CriterionEditor::on_actionCircle_triggered()
{
    if(ui->imgContainer->circles.size() < 1){
        QMessageBox::information(nullptr,tr("Operation FAILED"),tr("operation DECLINE because no circles detected!"));
        return;
    }
    isBaseCircle=false;
    ui->imgContainer->setState(ImageContainer::SelectCircle);
    statusLabel->setText(tr("chose a circle"));

   if(singleRule)
       delete singleRule;

   singleRule   =   new CircleCriterion;
   connect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onCircleSelected(Round)));
}

void CriterionEditor::on_actionDone_Insert_triggered(){
    if(!CriterionListWidget::getInstance()->InsertCriterion(currentCriterion)){
        return;
    }

    //reset all state
    currentCriterion    =   Criterion::create();
        //manager will release the memory
    ui->ledtName->setText(QString::fromStdString(currentCriterion->header.name));
    ui->ledtRule->setText("0,0 0x0");
    ui->listWidget->clear();
    ui->imgContainer->setState(ImageContainer::Wait);
}


//custom slots
void CriterionEditor::onSelectingTwoLineRelation(Segment s){
    if(singleRule->getType() == twoLineRelation){
        TwoLineCriterion* wlc  =   dynamic_cast<TwoLineCriterion*>(singleRule);
        if(!wlc->_fLine.isValid()){
            wlc->_fLine =   s;
            statusLabel->setText(tr("first line got, now select the other line"));
            return;
        }else{
            wlc->_sLine =   s;
            statusLabel->setText(tr("second line got"));

            //two line already got, reset ImageContainer' state
            disconnect(ui->imgContainer,SIGNAL(lineSelected(Segment)),this,SLOT(onSelectingTwoLineRelation(Segment)));
            ui->imgContainer->setState(ImageContainer::Wait);

            //ask for extra data
#if 0
            RangeDialog dlgDis(&(wlc->_distance.lower),&(wlc->_distance.upper),
                               nullptr,tr("distance"),tr("input DISTANCE range: "));
            dlgDis.exec();
            RangeDialog dlgAngle(&(wlc->_angle.lower),&(wlc->_angle.upper),
                                 nullptr,tr("angle"),tr("input ANGLE range: "));
            dlgAngle.exec();
#endif
            //new method to extra data           
            wlc->disRankNum =   QInputDialog::getInt(nullptr, tr("rank number"),tr("input the rank number: "));

            for(int i=0;i<static_cast<int>(wlc->disRankNum);i++)
            {
                Range ra;
                char name[50];
                std::sprintf(name,"input DISTANCE%d range: ",i);
                RangeDialog dlgDis(&(ra.lower),&(ra.upper),
                                   nullptr,tr("distance"),tr(name));
                dlgDis.exec();
                wlc->_distances.push_back(ra);
                qDebug()<<wlc->_distances.back().lower<<" "<<wlc->_distances.back().upper;

            }//add into list
            RangeDialog dlgAngle(&(wlc->_angle.lower),&(wlc->_angle.upper),
                                 nullptr,tr("angle"),tr("input ANGLE range: "));
            dlgAngle.exec();
            currentCriterion->criterionList.push_back(singleRule);
            singleRule  =   nullptr;//release no longer relate to himself

            //show in list, NOT humanized now
            QListWidgetItem* item   =   new QListWidgetItem(tr("two line"));
            ui->listWidget->addItem(item);

            qDebug()<<"two line relation:"
                   <<"\n\t"<<wlc->_fLine.sPoint.x<<","<<wlc->_fLine.sPoint.y<<" -> "<<wlc->_fLine.ePoint.x<<","<<wlc->_fLine.ePoint.y
                   <<"\n\t"<<wlc->_sLine.sPoint.x<<","<<wlc->_sLine.sPoint.y<<" -> "<<wlc->_sLine.ePoint.x<<","<<wlc->_sLine.ePoint.y
                   //<<"\n\tdistance: "<<wlc->_distance.lower<<" to "<<wlc->_distance.upper
                   <<"\n\tangle: "<<wlc->_angle.lower<<" to "<<wlc->_angle.upper;
        }
    }
}

void CriterionEditor::onCircleSelected(Round r){
    qDebug()<<isBaseCircle;
    if(isBaseCircle)
    {
        currentCriterion->header.basePoint=r.center;
        disconnect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onCircleSelected(Round)));
        ui->imgContainer->setState(ImageContainer::Wait);
        QMessageBox::information(nullptr,tr("BaseCircle"),tr("get base circle"));
    }
    else
    {
        CircleCriterion* cc =   dynamic_cast<CircleCriterion*>(singleRule);
        cc->_center =r.center;
        cc->_radiu=r.radius;
        cc->_box=r.box;
        //circle center ready
        disconnect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onCircleSelected(Round)));
        ui->imgContainer->setState(ImageContainer::Wait);

        //radius
        RangeDialog dlgRadius(&(cc->_radius.lower),&(cc->_radius.upper),
                              nullptr,tr("radius"),tr("input RADIUS range: "));
        dlgRadius.exec();
        currentCriterion->criterionList.push_back(singleRule);
        singleRule  =   nullptr;

        QListWidgetItem* item   =   new QListWidgetItem(tr("circle"));
        ui->listWidget->addItem(item);

        qDebug()<<"circle"
                <<"\n\t"<<cc->_center.x<<","<<cc->_center.y
                <<"\n\t"<<cc->_radius.lower<<" to "<<cc->_radius.upper;
    }
}

void CriterionEditor::onSettingRuleCriterion(Segment s){
    if(!currentCriterion->header.fLine.isValid()){
        currentCriterion->header.fLine  =   s;
        statusLabel->setText(tr("first line got, now select the other line"));
        return;
    }else{
        currentCriterion->header.sLine  =   s;
        statusLabel->setText(tr("second line got"));

        disconnect(ui->imgContainer,SIGNAL(lineSelected(Segment)),this,SLOT(onSettingRuleCriterion(Segment)));
        ui->imgContainer->setState(ImageContainer::Wait);
        double stdDistance  =   QInputDialog::getDouble(nullptr,
                                                        tr("Input"),
                                                        tr("Input the standard length:"),
                                                        -1);
        currentCriterion->header.PPM=WorkpieceEstimator::calcPPM(currentCriterion->header.fLine,
                                                                 currentCriterion->header.sLine,
                                                                 stdDistance);
    }
}
void CriterionEditor::onSettingRuleCriterion(Round r)
{
    statusLabel->setText(tr("circle got"));

    disconnect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onSettingRuleCriterion(Round)));
    ui->imgContainer->setState(ImageContainer::Wait);
    double stdDistance  =   QInputDialog::getDouble(nullptr,
                                                    tr("Input"),
                                                    tr("Input the standard radius:"),
                                                    -1);
    currentCriterion->header.PPM=WorkpieceEstimator::calcPPM(r,stdDistance);
}

void CriterionEditor::on_actionBlind_hole_2_triggered()
{
    isBaseCircle=false;
    ui->imgContainer->setState(ImageContainer::SetBlindBox);
    statusLabel->setText(tr("chose the Box of blind hole"));
}

void CriterionEditor::on_actionBasic_circle_triggered()
{

    if(ui->imgContainer->circles.size()<1)
    {
        QMessageBox::information(nullptr,tr("Operation FAILED"),tr("operation DECLINE because no circles detected!"));
        return;
    }
    isBaseCircle=true;
    ui->imgContainer->setState(ImageContainer::SelectCircle);
    statusLabel->setText(tr("chose a basic circle"));
    connect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onCircleSelected(Round)));
}
void CriterionEditor::on_actiontwo_line_triggered()
{

    if(ui->imgContainer->lines.size() < 2){
        QMessageBox::information(nullptr,tr("Operation FAILED"),
                                 tr("operation DECLINE because less than 2 lines was detected! consider refresh"));
        return;
    }

    ui->imgContainer->setState(ImageContainer::SelectLine);
    statusLabel->setText(tr("chose a line"));

    currentCriterion->header.fLine.ePoint   =  cv:: Point2f(0.0f,0.0f);//make invalid
    connect(ui->imgContainer,SIGNAL(lineSelected(Segment)),this,SLOT(onSettingRuleCriterion(Segment)));

}

void CriterionEditor::on_actionCircle_rule_triggered()
{

    if(!ui->imgContainer->circles.size()){
        QMessageBox::information(nullptr,tr("Operation FAILED"),
                                 tr("operation DECLINE because less than 2 lines was detected! consider refresh"));
        return;
    }

    ui->imgContainer->setState(ImageContainer::SelectCircle);
    statusLabel->setText(tr("chose a circle"));

    connect(ui->imgContainer,SIGNAL(circleSelected(Round)),this,SLOT(onSettingRuleCriterion(Round)));
}
