#include "DaHengcamera.h"
#include <QDebug>
DaHengCamera::DaHengCamera():
    hDevice(NULL),
    nDeviceNum(0)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    status = GXInitLib();
    if (status != GX_STATUS_SUCCESS)
    {
        qDebug()<<"Init DaHeng lib failed!";
    }
    status = GXUpdateAllDeviceList(&nDeviceNum,1000);
    if (status != GX_STATUS_SUCCESS&&nDeviceNum<1)
    {
        qDebug()<<"getDevice failed!";
    }
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode   = GX_OPEN_INDEX;
    stOpenParam.pszContent = "1";
    status = GXOpenDevice(&stOpenParam,&hDevice);
    int64_t nPayLoadSize=0;
    if(GXGetInt(hDevice,GX_INT_PAYLOAD_SIZE,&nPayLoadSize)==GX_STATUS_SUCCESS
       && nPayLoadSize>0)
    {

        stFrameData.pImgBuf =malloc((size_t)nPayLoadSize);
    }
}
DaHengCamera::~DaHengCamera()
{

}
void DaHengCamera::beginCapture()
{
    if(GXSendCommand(hDevice,GX_COMMAND_ACQUISITION_START)!=GX_STATUS_SUCCESS)
    {
        qDebug()<<"begin capture failed!";

    }
}
void DaHengCamera::endCapture()
{
    GXSendCommand(hDevice,GX_COMMAND_ACQUISITION_STOP);
    free(stFrameData.pImgBuf);
    status= GXCloseDevice(hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        qDebug()<<"Close DaHeng Device failed!";
    }
    status = GXCloseLib();
    if (status != GX_STATUS_SUCCESS)
    {
        qDebug()<<"Close DaHeng lib failed!";
    }
}
QImage DaHengCamera::captureImage()
{
    while(GXGetImage(hDevice,&stFrameData,100)!=GX_STATUS_SUCCESS)
    {
        qDebug()<<"DaHeng camera can't get Image";
    }
    if(stFrameData.nStatus==GX_FRAME_STATUS_SUCCESS)
    {
        return QImage((uchar*)stFrameData.pImgBuf,
                      stFrameData.nWidth,
                      stFrameData.nHeight,
                      QImage::Format_Indexed8);
    }
    else
    {
        qDebug()<<"get DaHeng Image failed!";
    }
}
