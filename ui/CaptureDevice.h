#pragma once

#include <QImage>

class CaptureDevice
{
public:
    CaptureDevice(){}
    virtual ~CaptureDevice(){}

    virtual void beginCapture(){}
    virtual void endCapture(){}

    virtual QImage captureImage()   =   0;
};
