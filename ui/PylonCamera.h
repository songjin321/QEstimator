#pragma once

#include "CaptureDevice.h"
#include <pylon/PylonIncludes.h>

class PylonCamera:  public CaptureDevice
{
public:
    PylonCamera();
    ~PylonCamera();

    virtual void beginCapture();
    virtual void endCapture();

    virtual QImage captureImage();

private:
    Pylon::CInstantCamera camera;
    Pylon::CGrabResultPtr ptrGrabResult;
};
