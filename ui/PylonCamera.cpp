#include "PylonCamera.h"
#include "QDebug"

using namespace Pylon;

PylonCamera::PylonCamera()
{
     PylonInitialize();
    try{
        IPylonDevice* pDevice   =   CTlFactory::GetInstance().CreateFirstDevice();
        camera.Attach(pDevice);
        //camera(CTlFactory::GetInstance().CreateFirstDevice());
        qDebug()<<"camera okay";
    }catch(GenICam::GenericException& e){
        qDebug()<<"error: \n\t"<<e.what();
    }
}

PylonCamera::~PylonCamera(){
    camera.StopGrabbing();
    PylonTerminate();
}

void PylonCamera::beginCapture(){
    if(!camera.IsGrabbing())
    camera.StartGrabbing();
}

void PylonCamera::endCapture(){
    camera.StopGrabbing();
}

QImage PylonCamera::captureImage(){
    if(camera.IsGrabbing()){
        camera.RetrieveResult(5000,ptrGrabResult);
        if(ptrGrabResult->GrabSucceeded()){
            return QImage((uchar*)ptrGrabResult->GetBuffer(),
                          ptrGrabResult->GetWidth(),
                          ptrGrabResult->GetHeight(),
                          QImage::Format_Indexed8);
        }
        qDebug()<<"grab failed";
    }else
        qDebug()<<"camera not grabbing";
    return QImage();
}
