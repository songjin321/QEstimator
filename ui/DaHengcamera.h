#ifndef DAHENGCAMERA_H
#define DAHENGCAMERA_H
#include "CaptureDevice.h"
#include <GxIAPI.h>
class DaHengCamera:public CaptureDevice
{
public:
    DaHengCamera();
    ~DaHengCamera();

    virtual void beginCapture();
    virtual void endCapture();

    virtual QImage captureImage();

private:
    GX_DEV_HANDLE hDevice;
    GX_OPEN_PARAM stOpenParam;
    uint32_t nDeviceNum;
    GX_FRAME_DATA stFrameData;
    GX_STATUS status;
};

#endif // DAHENGCAMERA_H
