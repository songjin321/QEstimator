#pragma once
//ImageContainer, inherit from QWidget
//prepare for CriterionEditor
//provide pick-up function, line or circle

#include <QtWidgets>
#include <vector>
#include "FeatureExtractor.h"
#include "Pretreatment.h"
#include "common.h"

class CriterionEditor;
class ImageContainer:public QWidget
{
    Q_OBJECT
public:
    ImageContainer(QWidget* parent = nullptr);

    //operate state
    enum State{
        Wait,
        SetBlindBox,
        SelectLine,
        SelectCircle
    };
    void setImage(QPixmap img, bool dispFeature);

    void setState(State s);

    //make it easier to access some data
    friend class CriterionEditor;
    CriterionEditor* pFather;

signals:
    //print information to parent's status bar
    void showInfo(QString);
    void lineSelected(Segment);
    void circleSelected(Round);
    void basicCircleSelected(Round);

protected:
    //virtual
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    //selectable elements
    std::vector<Round> circles;
    std::vector<Segment> lines;
    std::vector<Round> _circles_;
    //window state, mark of operation
    State _state;

    //paint control
    QPixmap _img;

    //draw selectable features
    bool _dispFeature;
    void drawFeatures(QPainter& painter);

    //draw special features
    bool _dispSepcial;
    //show the user which feature is being selected
    int _selectingIndex;
    FeatureType _selectingType;
    void drawSelectingFeature(QPainter& painter);

    //draw stretch box
    bool drawBlindBox;
    QPoint fPoint, lPoint;

    //match coordinate
    QPoint pic2widget(const QPoint& pos){
        return QPoint(pos.x()/static_cast<float>(_img.width())*this->width(),
                       pos.y()/static_cast<float>(_img.height())*this->height());
    }
    QPoint widget2pic(const QPoint& pos){
        return QPoint(pos.x()/static_cast<float>(width())*_img.width(),
                       pos.y()/static_cast<float>(height())*_img.height());
    }

    //get the nearest feature, which may be the user want to select
    //return -1 or the index of element in vector
    int getNearestFeature(const QPoint &pos,FeatureType ft);

};
//! 2015 09 30
//! now I think struct circle and segment should have inherited form a basic class,
//! which would make it easier to manager them, as well as paint function

