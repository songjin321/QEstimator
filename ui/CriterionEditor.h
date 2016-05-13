#ifndef CRITERIONEDITOR_H
#define CRITERIONEDITOR_H

#include <QMainWindow>
#include "common.h"

class Criterion;
class QLabel;
class ImageContainer;
class CriterionBase;

namespace Ui {
class CriterionEditor;
}

class CriterionEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit CriterionEditor(QWidget *parent = 0);
    ~CriterionEditor();

    //set input image
    void setImage(QPixmap img, bool dispFeature = true);
    //make it easier to get some data
    friend class ImageContainer;
public slots:
    //set criterion
    void setCriterionName(QString name);

private slots:
    void onCircleSelected(Round r);

    void onSelectingTwoLineRelation(Segment s);

    void onSettingRuleCriterion(Segment s);

    void onSettingRuleCriterion(Round r);

    void on_actionRule_triggered();

    void on_actionRefresh_triggered();

    void on_actionTwoLine_triggered();

    void on_actionDone_Insert_triggered();

    void on_actionCircle_triggered();
    void on_actionBlind_hole_2_triggered();

    void on_actionBasic_circle_triggered();

    void on_actiontwo_line_triggered();

    void on_actionCircle_rule_triggered();

signals:
    void refreshImage();

private:
    Ui::CriterionEditor *ui;

    QLabel* statusLabel;

    Criterion* currentCriterion;
    bool shallRelease;

    bool isBaseCircle;
    //current editing regulation
    CriterionBase* singleRule;

};

#endif // CRITERIONEDITOR_H
