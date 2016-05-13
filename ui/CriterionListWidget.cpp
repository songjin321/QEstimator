#include "CriterionListWidget.h"
#include "ui_CriterionListWidget.h"
#include <QtWidgets>
#include <QDebug>
#include "../CriterionManager.h"
#include "ui/MainWindow.h"
CriterionListWidget* CriterionListWidget::_instance =   nullptr;

CriterionListWidget::CriterionListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CriterionListWidget)
{
    ui->setupUi(this);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(cmbChanged(QString)));

}

CriterionListWidget::~CriterionListWidget()
{
    delete ui;
}

bool CriterionListWidget::InsertCriterion(Criterion *c){
    if(!CriterionManager::getInstance()->addCriterion(c)){
        QMessageBox::warning(this,tr("operation failed"),
                             QString("FAILED to insert: %1!").arg(c->header.name.empty()?"invalid name":"name repetition"));
        return false;
    }

    QListWidgetItem* item   =   new QListWidgetItem(QString::fromStdString(c->header.name));
    ui->listWidget->addItem(item);
    ui->comboBox->addItem(QString::fromStdString(c->header.name));

    return true;
}

void CriterionListWidget::cmbChanged(QString name){
    if(!CriterionManager::getInstance()->setCurrentCriterion(name.toStdString()))
        qDebug()<<"CriteroinListWidget: something unexpected happended";
}

void CriterionListWidget::on_btnCreate_clicked()
{
    QString name    =   QInputDialog::getText(this,
                                              tr("create new criterion"),
                                              tr("criterion name: "));
    if(name.isEmpty())
        return;

    emit createNewCriterion(name);
}

void CriterionListWidget::on_btnExport_clicked()
{
    std::string name=CriterionManager::getInstance()->getCurrentCriteion()->header.name;
    std::cout<<name<<endl;
    //
    CriterionManager::getInstance()->outputToFile(name,name);
    QMessageBox::information(this,tr("done"),tr("export okay."));
}

void CriterionListWidget::on_btnRemove_clicked()
{
    ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
    CriterionManager::getInstance()->removeCriterion(ui->comboBox->currentText().toStdString());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}

void CriterionListWidget::on_btnLoad_clicked()
{
    qDebug()<<"clicked";
    _mainWindow->grabStop();

    qDebug()<<"stopped";
    QFileDialog *fileDialog=new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("XML (*.xml)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setDirectory(tr("../criterionsfile"));
    QString path;
    if(fileDialog->exec()==QDialog::Accepted){
        path=fileDialog->selectedFiles()[0];
        Criterion *loadCriterion =   Criterion::create();
        CriterionManager::getInstance()->inputFromFile(path.toStdString(),loadCriterion);
        CriterionManager::getInstance()->addCriterion(loadCriterion);
        QListWidgetItem* item   =   new QListWidgetItem(QString::fromStdString(loadCriterion->header.name));
        ui->listWidget->addItem(item);
        ui->comboBox->addItem(QString::fromStdString(loadCriterion->header.name));
    }else{
        QMessageBox::information(nullptr,tr("path"),tr("You didn't select any file."));
    }
    qDebug()<<"continue...";

    _mainWindow->grabStart();
}

