#ifndef CRITERIONLISTWIDGET_H
#define CRITERIONLISTWIDGET_H

//user interface to acess CriterionManager
//singleton parttern

#include <QWidget>

class Criterion;

namespace Ui {
class CriterionListWidget;
}

class MainWindow;

class CriterionListWidget : public QWidget
{
    Q_OBJECT
    explicit CriterionListWidget(QWidget *parent = 0);

    static CriterionListWidget* _instance;
public:
    ~CriterionListWidget();

    static CriterionListWidget* getInstance(){
        if(!_instance)
            _instance   =   new CriterionListWidget;
        return _instance;
    }

    bool InsertCriterion(Criterion* c);

    void setMainWindow(MainWindow* mw){ _mainWindow = mw; }

private slots:
    void cmbChanged(QString name);

    void on_btnCreate_clicked();

    void on_btnExport_clicked();
    
    void on_btnRemove_clicked();

    void on_btnLoad_clicked();

signals:
    void createNewCriterion(QString);

private:
    Ui::CriterionListWidget *ui;

    MainWindow* _mainWindow;
};

#endif // CRITERIONLISTWIDGET_H
