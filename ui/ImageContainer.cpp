#include "ImageContainer.h"
#include <QDebug>
#include "CriterionEditor.h"
#include <algorithm>
#include "Pretreatment.h"
#include "FeatureExtractor.h"
#include <cfloat>

using namespace cv;

ImageContainer::ImageContainer(QWidget *parent)
    :QWidget(parent),
      pFather(nullptr),
      _dispFeature(false),
      _state(Wait),
      _dispSepcial(false),
      _selectingIndex(-1),
      _selectingType(Feature_Line)
{
    drawBlindBox =   false;
    fPoint  =   lPoint  =   QPoint(0,0);
}

void ImageContainer::paintEvent(QPaintEvent* e){
    QPainter painter(this);

    painter.fillRect(rect(),Qt::lightGray);
    if(!_img.isNull()){
        painter.drawPixmap(rect(),_img);
        if(_dispFeature)
            drawFeatures(painter);
    }

    if(_dispSepcial)
        drawSelectingFeature(painter);

    //paint select rect
    if(drawBlindBox){
        painter.save();

        painter.setPen(Qt::blue);
        painter.drawRect(QRect(fPoint,lPoint));

        painter.restore();
    }
}

void ImageContainer::drawFeatures(QPainter &painter){
    painter.save();

    //lines
    painter.setPen(QPen(QBrush(QColor(255,0,0)),4));
    if(lines.size()){
        std::for_each(lines.begin(),lines.end(),
                      [&](const Segment& s){
            painter.drawLine(pic2widget(QPoint(s.sPoint.x,s.sPoint.y)),
                             pic2widget(QPoint(s.ePoint.x,s.ePoint.y)));
        });
    }

    //circles
    painter.setPen(QPen(QBrush(QColor(0,255,0)),4));
    if(circles.size()){
        std::for_each(circles.begin(),circles.end(),
                      [&](const Round& c){
            painter.drawEllipse(pic2widget(QPoint(c.center.x,c.center.y)),
                    int(c.radius/_img.width()*width()),int(c.radius/_img.height()*height()));
        });
    }

    painter.restore();
}

void ImageContainer::drawSelectingFeature(QPainter &painter){
    if(_selectingIndex  < 0)
        return;

    painter.save();
    painter.setPen(QPen(QBrush(QColor(255,255,255,100)),6));

    switch(_selectingType){
    case Feature_Line:
        painter.drawLine(pic2widget(QPoint(lines[_selectingIndex].sPoint.x,lines[_selectingIndex].sPoint.y)),
                         pic2widget(QPoint(lines[_selectingIndex].ePoint.x,lines[_selectingIndex].ePoint.y)));
        break;
    case Feature_Circle:
        painter.drawEllipse(pic2widget(QPoint(circles[_selectingIndex].center.x,circles[_selectingIndex].center.y)),
                            int(circles[_selectingIndex].radius/_img.width()*width()),
                            int(circles[_selectingIndex].radius/_img.height()*height()));
        break;
    default:
        break;
    }

    painter.restore();
}

void ImageContainer::setImage(QPixmap img, bool dispFeature){
    _img    =   img;
    _dispFeature    =   dispFeature;
    circles.clear();
    lines.clear();
    if(dispFeature){
        //extract features here...
        Mat mat =   fromQImage2Mat(img.toImage());
        Pretreatment pre;
        pre.run(mat,mat);
        imwrite("pre.jpg",mat);
        qDebug()<<"run!";
        FeatureExtractor fe;        
        fe.extract(mat);
        qDebug()<<"extract";
        fe.getCircles(circles);
        fe.getSegments(lines);
        //can NOT display edge image directly!
        //Canny(mat,mat,2,6,3);
        //_img    =   QPixmap::fromImage(fromMat2QImage(mat));
        pFather->statusLabel->setText(QString("selectable element: "
                                              "lines: %1\tcircles: %2").arg(lines.size()).arg(circles.size()));
    }
    update();
}

//user interact
void ImageContainer::mousePressEvent(QMouseEvent* e){
    if(e->buttons()&Qt::LeftButton){
        qDebug()<<"pressed";
        Segment s;
        switch(_state){
        case Wait:
            break;
        case SetBlindBox:
            drawBlindBox =   true;
            lPoint  =   fPoint  =   e->pos();
            update();
            break;
        case SelectLine:
            //emit signal only if select okay
            if(_selectingIndex >= 0)
                emit lineSelected(lines[_selectingIndex]);
            break;
        case SelectCircle:
            if(_selectingIndex >= 0)
                emit circleSelected(circles[_selectingIndex]);
            break;
        default:
            break;
        }
    }
}

void ImageContainer::mouseMoveEvent(QMouseEvent* e){
    switch(_state){
    case Wait:
        break;
    case SetBlindBox:
        lPoint  =   e->pos();
        update();
        break;
    case SelectLine:
        _selectingIndex =   getNearestFeature(e->pos(),Feature_Line);
        update();
        break;
    case SelectCircle:
        _selectingIndex =   getNearestFeature(e->pos(),Feature_Circle);
        update();
        break;
    default:
        break;
    }
}

void ImageContainer::mouseReleaseEvent(QMouseEvent* e){
    if(e->button()==Qt::LeftButton){
        switch(_state){
        case Wait:
            break;
       case SetBlindBox:
        {

            drawBlindBox =   false;
            QRect rect=QRect(widget2pic(fPoint),widget2pic(lPoint));
            cv::Rect r;
            cv::Mat BlindHoleImage;
            cv::Mat mat =   fromQImage2Mat(_img.toImage());
            r.x =   rect.left();
            r.y =   rect.top();
            r.width =   rect.width();
            r.height    =   rect.height();
            BlindHoleImage=mat(r);
            Pretreatment pre;
            imwrite("pref.jpg",BlindHoleImage);
            pre.run(BlindHoleImage,BlindHoleImage);
            imwrite("prel.jpg",BlindHoleImage);
            FeatureExtractor fe;
            fe.extractBlindHole(BlindHoleImage);
            fe.getCircles(_circles_);
          //qDebug()<<circles.size()<<endl;

            if(_circles_.size()==1)
            {
                circles.push_back(_circles_[0]);
                circles.back().center.x+=r.x;
                circles.back().center.y+=r.y;
                circles.back().box=r;
                qDebug()<<"Blind hole    x:"<<circles.back().center.x<<" "<<"y:"<<circles.back().center.y<<"r:"<<circles.back().radius<<endl;
                update();
            }
            else
            {
                qDebug()<<"get blind hole failed!";
            }
            break;
        }
        default:
            break;
        }
    }
}

const float DisThreshold  =   80.0f;
//
int ImageContainer::getNearestFeature(const QPoint& pos,FeatureType ft){
    int reIndex =   -1;

    float minDis    =   FLT_MAX;
    float curDis    =   0;

    Point2f posOnPic(widget2pic(pos).x(),widget2pic(pos).y());

    //for
    //  line, use distance to judge
    //  circle, use distance to the nearest point on it, means: radius minus distance to centorid

    switch(ft){
    case Feature_Line:
        if(lines.size()){
            for(size_t i=0;i<lines.size();++i){
                Segment& s    =   lines[i];
#if 0//direction is not supportted
                s.direction =   (s.ePoint-s.sPoint)/static_cast<float>(length2D(s.ePoint-s.sPoint));
#endif
                curDis  =   qAbs(crossProduct2D(s.direction,posOnPic-s.sPoint));

                if(curDis < minDis){
                    minDis  =   curDis;
                    reIndex =   i;
                }
            }
        }
        break;
    case Feature_Circle:
        if(circles.size()){
            for(size_t i=0;i<circles.size();++i){
                const Round& c  =   circles[i];
                curDis  =   std::abs(c.radius-length2D(posOnPic-c.center));
                if(curDis < minDis){
                    minDis  =   curDis;
                    reIndex =   i;
                }
            }
        }
        break;
    default:
        break;
    }

    reIndex =   minDis<DisThreshold?reIndex:-1;
    //qDebug()<<"current nearest feature is: "<<reIndex<<" with distace: "<<minDis;

    return reIndex;
}

void ImageContainer::setState(State s){
    if(s == _state)
        return;

    _state  =   s;
    _dispSepcial    =   false;
    this->setMouseTracking(false);
    //do something necessary to initialize state
    switch(_state){
    case SelectLine:
        _dispSepcial    =   true;
        _selectingType  =   Feature_Line;
        _selectingIndex =   -1;
        this->setMouseTracking(true);
        break;
    case SelectCircle:
        _dispSepcial    =   true;
        _selectingType  =   Feature_Circle;
        _selectingIndex =   -1;
        this->setMouseTracking(true);
        break;
    default:
        break;
    }
}
