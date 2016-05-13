#pragma once

//thread for grabing frames by camera
//simply copy from "capturethread" in QtDemo

#include <QThread>
#include <QImage>

#if 1

class CaptureThread:    public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QObject* parent  =   0);
};

#else
class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(QObject *parent = 0){}

public:
    virtual void run();
    void setIndex(int index);
    void stream();
    void pause();
    void stop();
signals:
    void captured(QImage img, unsigned char * buffer);
private:
    int index;
    bool work;
    bool term;


    QVector<QRgb> grayColourTable;

public slots:

};

#endif

