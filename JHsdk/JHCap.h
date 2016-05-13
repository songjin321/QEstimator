// --------------------------------------------------------------------------
// Header file for the camera.
// Copyright 2011-2012 JingHang Electronics Corporation.
// Company proprietary.
// --------------------------------------------------------------------------
//******************************************************************************
// --------------------------------------------------------------------------
//******************************************************************************
/*
 * \file      JHCap.h
 * \brief     Defines the API for the JingHang'camera DLL application
 * \author     Winyio
 * \version    \$ Revision: 0.1 \$
 * \arg      first implemetation
 * \date      2011/11/08 10:10:00
 */

#ifndef JHCAP_H
#define JHCAP_H

//#if defined(JHCAP_LIBRARY)
//# define DLL_EXPORT __declspec( dllexport )
//#else
//# define DLL_EXPORT __declspec( dllimport )
//#endif

#define DLL_EXPORT 
#define __stdcall //LINUX TODO

typedef int API_STATUS;
//typedef int ( CALLBACK * CAPTURE_FRAME_PROC)(unsigned char *pImageBuffer, int width, int height, int format);

enum {API_OK=0, API_ERROR};

#define CAMERA_IMAGE_RAW8  0x1
#define CAMERA_IMAGE_GRAY8  0x2
#define CAMERA_IMAGE_RGB24  0x4
#define CAMERA_IMAGE_BGR   0x100
#define CAMERA_IMAGE_QUAD  0x200
#define CAMERA_IMAGE_SYNC    0x10000
#define CAMERA_IMAGE_TRIG    0x20000
#define CAMERA_IMAGE_STRETCH 0x1000000

#define CAMERA_IMAGE_BMP   (CAMERA_IMAGE_RGB24 | CAMERA_IMAGE_BGR)

#define CAMERA_IO_IN  0x02
#define CAMERA_IO_OUT  0x80

#define RESOLUTION_CROPPING 0x00
#define RESOLUTION_SKIPPING    0x01
#define RESOLUTION_BINNING 0x02

#define CAMERA_SNAP_CONTINUATION 0x01
#define CAMERA_SNAP_TRIGGER 0x02

#define CAMERA_RESOLUTION_CROPPING 0x0
#define CAMERA_RESOLUTION_SKIPPING 0x1
#define CAMERA_RESOLUTION_BINNING 0x2

#define CAMERA_INTERPOLATION_BILINEAR 0x0
#define CAMERA_INTERPOLATION_ALGO1  0x1
#define CAMERA_INTERPOLATION_ALGO2  0x2

/*****************************************************************************
 *****************************************************************************
 **                      Core API Functions                                 **
 *****************************************************************************
 *****************************************************************************/
/*==============================================================
Name: CameraGetVersion
Description: Get the version of JHCap2.lib
Parameters: *major, contains the year/month information
            *minor, contains version information
Return: always returns API_OK.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT int __stdcall CameraGetVersion(int *major, int *minor);


/*==============================================================
Name: CameraGetCount
Description: Get the number of cameras that connected to the PC
Parameters: *count,  return the number of cameras
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetCount(int *count);


/*==============================================================
Name: CameraGetName
Description: Get the camera name according to the index
Parameters: index, the index of camera to query
            *name, the camera full name
            *model, the model information of the camera name
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/


extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetName(int index, char *name, char *model);


/*==============================================================
Name: CameraGetID
Description: Get the camera name according to the index
Parameters: index, the index of MultiCamera
            *modeID, the model ID of the camera name
            *productID, the product ID of the camera name
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetID(int index,int *modeID, int *productID);


/*==============================================================
Name: CameraInit
Description: Initialize the camera.
Parameters: index, the camera need to initialize
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraInit(int device_id);


/*==============================================================
Name: CameraFree
Description: Free resources related to openned camera.
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraFree(int device_id);

/*==============================================================
Name: CameraSetOption
Description: Set the image format
Parameters:device_id, the camera index
		   format, options are CAMERA_IMAGE_RAW8 / CAMERA_IMAGE_GRAY8 /
                                CAMERA_IMAGE_RGB24 / CAMERA_IMAGE_BGR /
                                CAMERA_IMAGE_BMP  /  CAMERA_IMAGE_STRETCH
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the default image format is CAMERA_IMAGE_BMP
 --------------------------------------------------------------*/

//extern "C" DLL_EXPORT  API_STATUS __stdcall CameraSetOption(int device_id,int format);


/*==============================================================
Name: CameraGetOption
Description: Set the image format
Parameters: device_id, the camera index
			format, options are CAMERA_IMAGE_RAW8 / CAMERA_IMAGE_GRAY8 /
                                CAMERA_IMAGE_RGB24 / CAMERA_IMAGE_BGR /
                                CAMERA_IMAGE_BMP  /  CAMERA_IMAGE_STRETCH
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the default image format is CAMERA_IMAGE_BMP (CAMERA_IMAGE_RGB24 | CAMERA_IMAGE_BGR)
 --------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetOption(int device_id,int *format);


/*==============================================================
Name: CameraGetResolutionCount
Description: Get the number of resolution that the camera supported
Parameters: device_id, the camera index
            *count, return the number of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetResolutionCount(int device_id, int *count);


/*==============================================================
Name: CameraGetResolutionMax
Description: Get the information of the max resolution that the camera can solved
Parameters: device_id, the camera index
            *width, the width of resolution
            *height, the height of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT  API_STATUS __stdcall CameraGetResolutionMax(int device_id, int *width, int *height);

/*==============================================================
Name: CameraGetResolutionMode
Description: Get the information of the max resolution that the camera can solved
Parameters: device_id, the camera index
            *mode, return the resolution mode
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:The return value is RESOLUTION_CROPPING,RESOLUTION_SKIPPING or RESOLUTION_BINNING

 --------------------------------------------------------------*/

extern "C" DLL_EXPORT  API_STATUS __stdcall CameraGetResolutionMode(int device_id, int *mode);


/*==============================================================
Name: CameraSetResolutionMode
Description: Get the information of the max resolution that the camera can solved
Parameters: device_id, the camera index
            mode, resolution mode includes RESOLUTION_CROPPING / RESOLUTION_SKIPPING / RESOLUTION_BINNING

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the mode can equal to RESOLUTION_CROPPING,RESOLUTION_SKIPPING or RESOLUTION_BINNING
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT  API_STATUS __stdcall CameraSetResolutionMode(int device_id, int mode);


/*==============================================================
Name: CameraGetResolution
Description: Get the information of the resolution according to the resolution index
Parameters: device_id, the camera index
            index, the index of camera
            *width, the width of resolution
            *height, the height of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetResolution(int device_id,int index,int *width, int *height);


/*==============================================================
Name: CameraSetResolution
Description: Set the resolution index
Parameters: device_id, the camera index
            index, the index of camera
            *width, the width of resolution
            *height, the height of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetResolution(int device_id,int index,int *width, int *height);


/*==============================================================
Name: CameraSetROI
Description: Set region of interested(ROI) area
Parameters: device_id, the camera index
            offset_width, row offset
            offset_height, column offset
            width, ROI width
            height, ROI height
Return: returns API_OK on success , otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/


extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetROI(int device_id,int offset_width, int offset_height, int width, int height);


/*==============================================================
Name: CameraSetGain
Description: Set camera gain
Parameters: device_id, the camera index
            gain, the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: gain ranges from 1-255
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetGain(int device_id,int gain);


/*==============================================================
Name: CameraGetGain
Description: Get camera gain
Parameters: device_id, the camera index
            *gain, return the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 1-255
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetGain(int device_id,int *gain);

/*==============================================================
Name: CameraSetAGC
Description: Set auto gain flag
Parameters: device_id, the camera index
            agc, set auto gain control, true for auto gian and false for manual gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetAGC(int device_id,bool agc);

/*==============================================================
Name: CameraGetAGC
Description: Get auto gain control
Parameters: device_id, the camera index
            *agc, return auto gain control status, true for auto gain and false for manual gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetAGC(int device_id,bool *agc);

/*==============================================================
Name: CameraSetWBGain
Description: Set white balance(WB) digital gain of the camera
Parameters: device_id, the camera index
            rg, set the value of red gain
            gg, set the value of green gain
            bg,  set the value of blue gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/


extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetWBGain(int device_id,double rg, double gg, double bg);

/*==============================================================
Name: CameraGetWBGain
Description: get white balance digital gain of the camera
Parameters: device_id, the camera index
            *rg, return the current red gain
            *gg, return the current green gain
            *bg, return the current blue gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetWBGain(int device_id,double *rg, double *gg, double *bg);


/*==============================================================
Name: CameraSetExposure
Description: Set Exposure
Parameters: device_id, the camera index
            exposure, the value of the exposure
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetExposure(int device_id,int exposure);

/*==============================================================
Name: CameraGetExposure
Description: Set exposure
Parameters: device_id, the camera index
            *exposure, return the value of the exposure
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetExposure(int device_id,int *exposure);

/*==============================================================
Name: CameraSetAEC
Description: Set auto exposure
Parameters: device_id, the camera index
            aec, set the auto exposure status(true for auto exposure and false for manual exposure)
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetAEC(int device_id,bool aec);

/*==============================================================
Name: CameraGetAEC
Description: Get auto exposure
Parameters: device_id, the camera index
            *aec, return the auto exposure status(true for auto exposure and false for manual exposure)
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetAEC(int device_id,bool *aec);

/*==============================================================
Name: CameraSetGamma
Description: set GAMMA
Parameters: device_id, the camera index
            gamma, the value of the gamma
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0.3-2
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetGamma(int device_id,double gamma);


/*==============================================================
Name: CameraGetGamma
Description: Read GAMMA
Parameters: device_id, the camera index
            *gamma, return the gamma
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: gamma value ranges from 0.3-3
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetGamma(int device_id,double *gamma);

/*==============================================================
Name: CameraSetContrast
Description: Set Contrast
Parameters: device_id, the camera index
            contrast, the contrast

Return: returns API_OK on success, otherwise returns API_ERROR.
Note: contrast value ranges from 0.3-2
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetContrast(int device_id,double contrast);


/*==============================================================
Name: CameraGetContrast
Description: Get Contrast
Parameters: device_id, the camera index
            *contrast, return the contrast

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0.3-2
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetContrast(int device_id,double *contrast);


/*==============================================================
Name: CameraSetSaturation
Description: Set saturation
Parameters: device_id, the camera index
            saturation, the saturation
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-2
--------------------------------------------------------------*/
extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetSaturation(int device_id,double saturation);

/*==============================================================
Name: CameraGetSaturation
Description: Get saturation
Parameters: device_id, the camera index
            *saturation, return the saturation

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-2
--------------------------------------------------------------*/
extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetSaturation(int device_id,double *saturation);


/*==============================================================
Name: CameraSetBlackLevel
Description: set black level of the sensor, normally it should be 0, range [0,255]
Parameters: device_id, the camera index
            black, the black level
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-255
--------------------------------------------------------------*/

extern "C"   DLL_EXPORT  API_STATUS __stdcall CameraSetBlackLevel(int device_id,int black);


/*==============================================================
Name: CameraGetBlackLevel
Description: Read Black level
Parameters: device_id, the camera index
            *black, return the black level
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-255
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetBlackLevel(int device_id,int *black);


/*==============================================================
Name: CameraSetAETarget
Description: AE target is a important parameter for auto exposure.
             normally it should be neutural, range [0,255]
Parameters: device_id, the camera index
            target, the auto exposure target value

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-255
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetAETarget(int device_id,int target);


/*==============================================================
Name: CameraGetAETarget
Description: Read AE target
Parameters: device_id, the camera index
            *target, return the ae target

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-255
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetAETarget(int device_id,int *target);


/*==============================================================
Name: CameraOnePushWB
Description: manually white balance
Parameters: device_id, the camera index

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraOnePushWB(int device_id);



/*****************************************************************************

Get the image information and image frame

 *****************************************************************************/

/*==============================================================
Name: CameraGetImageSize
Description: Get the size of the image that the camera capture
Parameters: device_id, the camera index
            *width, the width of the image
            *height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetImageSize(int device_id,int *width, int *height);

/*==============================================================
Name: CameraGetImageBufferSize
Description: Get the size of buffer to hold a image
Parameters: device_id, the camera index
            *size, return the width of the image
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetImageBufferSize(int device_id,int *size, int option);

/*==============================================================
Name: CameraQueryImage
Description: Get the informatin that the camera gathered
Parameters: device_id, the camera index
            *imgbuf, return the image data
            *length, return the image data size
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraQueryImage(int device_id,unsigned char *imgbuf, int *length, int option);


/*==============================================================
Name: CameraQueryImage2
Description: Get the informatin that the camera through trigger gathered
Parameters: device_id, the camera index
            *imgbuf, return the image data
            *length, return the image data size
            *meta, return the flag of the trigger
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraQueryImage2(int device_id,unsigned char *imgbuf, int *length, int *meta,int option);


/*==============================================================
Name: CameraCaptureImage
Description: Low resolution preview, high resolution capture
Parameters: device_id, the camera index
            index, the index of the resolution to capture 
            *imgbuf, return the image data
            *length, return the image data size
            option, the format of the image
Return: returns API_OK on success and the image information from imgbuf
Note:Capture resolution must be greater than the preview resolution

--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraCaptureImage(int device_id,int index,unsigned char *imgbuf, int *length, int option);

/*==============================================================
Name: CameraSetMirrorX
Description: Set the horizontal mirror state of the output images.
Parameters: device_id, the camera index
            mx, horizontal mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetMirrorX(int device_id,bool mx);


/*==============================================================
Name: CameraSetMirrorY
Description: Set the vertical mirror state of the output images.
Parameters: device_id, the camera index
            my, vertical mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetMirrorY(int device_id,bool my);


/*==============================================================
Name: CameraSetRotate
Description: Set image rotated
Parameters: device_id, the camera index
            rotate, the angle of rotation 
               //the value must equal to 360,270,180,90
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the angle must equal to 360,270,180,90
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetRotate(int device_id,int rotate);


/*==============================================================
Name: CameraGetMirrorX
Description: Query the horizontal mirror state of the output images.
Parameters: device_id, the camera index
            mx, the current horizontal mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetMirrorX(int device_id,bool *mx);

/*==============================================================
Name: CameraGetMirrorY
Description: Query the vetical mirror state of the output images.
Parameters: device_id, the camera index
            my, the current vertical mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetMirrorY(int device_id,bool *my);


/*==============================================================
Name: CameraReadSerialNumber
Description: Read the Camera's GUID
Parameters: device_id, the camera index
            id[], the array to the GUID, the length must bigger than 12 byte.
            length, GUID's size,the size must equal to 12
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraReadSerialNumber(int device_id,char id[],int length);


/*==============================================================
Name: CameraWriteUserData
Description: Wrete the user data
Parameters: device_id, the camera index
            data[], the array to the data, the length must less than 64 byte.
            length, the size of the user data ,the value must equal to 64
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraWriteUserData(int device_id,char data[],int length);

/*==============================================================
Name: CameraReadUserData
Description: Read the user data 
Parameters: device_id, the camera index
            data[], the array to the data, the length must less than 64 byte.
            length, the size of the user data ,the value must equal to 64
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraReadUserData(int device_id,char data[],int length);


/*==============================================================
Name: CameraSaveParameter
Description: Save parameter
Parameters: device_id, the camera index
            group_no, the index of parameters group
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: group_no can equal to 0,1,2 and 3, the default group is group 0.
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSaveParameter(int device_id,int group_no);


/*==============================================================
Name: CameraLoadParameter
Description: Load the stored parameter
Parameters: device_id, the camera index
            group_no, the index of parameters group
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraLoadParameter(int device_id,int group_no);


/*==============================================================
Name: CameraEnableStrobe
Description: Enable strobe
Parameters: device_id, the camera index
            en, true for enable strobe output, false for disable strobe output
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraEnableStrobe(int device_id, bool en);

/*==============================================================
Name: CameraSetTriggerPolarity
Description: Set trigger polarity
Parameters: device_id, the camera index
            high, the trigger polarity, true for positive pole and false for negative pole
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetTriggerPolarity(int device_id, bool high);

/*==============================================================
Name: CameraSetStrobePolarity
Description: Set the strobe output polarity
Parameters: device_id, the camera index
            high, the strobe polarity, true for positive pole and false for negative pole
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetStrobePolarity(int device_id, bool high);

/*==============================================================
Name: CameraGetSnapMode
Description: get snap mode
Parameters: device_id, the camera index
            *snap_mode, return the image capture mode
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetSnapMode(int device_id, int *snap_mode);

/*==============================================================
Name: CameraSetSnapMode
Description: set snap mode
Parameters: device_id, the camera index
            snap_mode, the snap mode includes CAMERA_SNAP_TRIGGER or CAMERA_SNAP_CONTINUATION
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetSnapMode(int device_id, int snap_mode);

/*==============================================================
Name: CameraTriggerShot
Description: Shot a image with software trigger
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraTriggerShot(int device_id);


/*==============================================================
Name: CameraGetGPIO
Description: get GPIO
Parameters: device_id, the camera index
            *val, return the value of the GPIO
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetGPIO(int device_id,int *val);

/*==============================================================
Name: CameraSetGPIO
Description: set GPIO
Parameters: device_id, the camera index
            mask, the mask only valid for CAMERA_IO_IN or CAMERA_IO_OUT
            val, GPIO value
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetGPIO(int device_id,int mask, int val);

/*==============================================================
Name: CameraSetDelay
Description: set frame delay so as to decrease the frame speed
Parameters: device_id, the camera index
            delay, the time of delay 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetDelay(int device_id,int delay);


/*==============================================================
Name: CameraGetDelay
Description: set frame delay so as to decrease the frame speed
Parameters: device_id, the camera index
            *delay, return the time of delay 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetDelay(int device_id,int *delay);


/*==============================================================
Name: CameraSetAntiFlicker
Description: set the value of flicker so as to decrease the flicker
Parameters: device_id, the camera index
            flicker, set the type of anti-flicker 0 for off, 1 for 50Hz, 2 for 60Hz
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetAntiFlicker(int device_id,int flicker);


/*==============================================================
Name: CameraGetAntiFlicker
Description: get the value of flicker
Parameters: device_id, the camera index
            *flicker, return the type of anti-flicker 0 for off, 1 for 50Hz, 2 for 60Hz
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetAntiFlicker(int device_id,int *flicker);

/*==============================================================
Name: CameraSetEnhancement
Description: Set the color enhancement
Parameters: device_id, the camera index
            enhance, the enhancement state, true for enable and false for disable
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetEnhancement(int device_id,bool enhance);

/*==============================================================
Name: CameraGetEnhancement
Description: Get the color enhancement
Parameters: device_id, the camera index
            *enhance, return the state of enhancement, true for enabled and false for disabled. 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetEnhancement(int device_id,bool *enhance);

/*==============================================================
Name: CameraSetAWB
Description: Set auto white balance(AWB) state
Parameters: device_id, the camera index
            awb, set AWB state, true for AWB and false for turn off AWB.
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetAWB(int device_id,bool awb);

/*==============================================================
Name: CameraGetAWB
Description: Get auto white balance(AWB) state
Parameters: device_id, the camera index
            *awb, return the state of AWB,true for AWB and false for AWB off.
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetAWB(int device_id,bool *awb);

/*==============================================================
Name: CameraSetInterpolation
Description: Set interpolation algorithm
Parameters: device_id, the camera index
            interpolation, interpolation algorithm to interpolate color images
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetInterpolation(int device_id,int interpolation);

/*==============================================================
Name: CameraGetInterpolation
Description: Get interpolation algorithm
Parameters: device_id, the camera index
            *interpolation, return the interpolation mode                
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetInterpolation(int device_id,int *interpolation);

/*==============================================================
Name: CameraSetHighSpeed
Description: Set data transfer speed
Parameters: device_id, the camera index
            high, true for high speed 48Mhz clock and false for low speed with 24Mhz or 32Mhz clock
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraSetHighspeed(int device_id, bool high);

/*==============================================================
Name: CameraGetHighSpeed
Description: Get data transfer speed
Parameters: device_id, the camera index
            *high, return the current data transfer speed, true for high and false for low                
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C"  DLL_EXPORT  API_STATUS __stdcall CameraGetHighspeed(int device_id, bool *high);

/*==============================================================
Name: CameraReconnect
Description: reconnect the camera as if the camera re-plug to the computer physically
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
--------------------------------------------------------------*/

extern "C"   DLL_EXPORT API_STATUS __stdcall CameraReconnect(int device_id);

/*****************************************************************************
 *****************************************************************************
 **                     Utility Functions                                   **
 *****************************************************************************
 *****************************************************************************/

/*==============================================================
Name: CameraSaveHBITMAP
Description: Create a HBITMAP handle by capturing data from camera
Parameters: device_id, the camera index
            *hBitmap, the handle of the BITMAP 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: using DeleteObject to release the HBITMAP
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveHBITMAP(int device_id, HBITMAP* hBitmap);

/*==============================================================
Name: CameraSaveBMP
Description: Create a named 32bit BMP file from the buffer
Parameters: fileName, the path and name that the image will save as
            *buf, the color information(RGB or GBR or YUV)
            width, the width of the image
            height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMP(char *fileName,BYTE * buf,UINT width,UINT height);

/*==============================================================
Name: CameraSaveBMP8
Description: Create a named 8bit BMP file from the buffer
Parameters: fileName, the path and name that the image will save as
            *buf, the color information(RAW or GRAY)
            width, the width of the image
            height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:this function only convenient for 8bit format
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMP8(char *fileName,BYTE * buf,UINT width,UINT height);

/*==============================================================
Name: CameraSaveBMPB
Description: Create a named BMP file by capturing data from camera
Parameters: device_id, the camera index
            fileName, the path and name that the image will save as
            width, the width of the image
            height, the height of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:this function convenient for 24bits and 8bits format
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMPB(int device_id,char *fileName);


/*==============================================================
Name: CameraSaveJpeg
Description: Create a named Jpeg file from the buffer
Parameters: fileName, the path and name that the image will save as
            dataBuf, RGB Buffer
            widthPix, the width of the image
            height, the height of the image
            color, TRUE = RGB  FALSE = Grayscale
            quality, 0-100
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: the dataBuf format is RGB not BGR
--------------------------------------------------------------*/
//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveJpeg(char *fileName, //path
//                             BYTE *dataBuf,  //RGB Buffer
//                             UINT widthPix,UINT height, BOOL color,int quality);

/*==============================================================
Name: CameraSaveJpegB
Description: Create a named Jpeg file by capturing data from camera
Parameters: device_id, the camera index
            fileName, the path and name that the image will save as
            color, TRUE = RGB  FALSE = Grayscale
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: the dataBuf format is RGB not BGR,besides the file is 24bits
--------------------------------------------------------------*/
//extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveJpegB(int device_id,char *fileName,BOOL color);  // TRUE = RGB  FALSE = Grayscale

/*==============================================================
Name: CameraShowImage
Description: Show image in a control with capturing data from camera, callback function will be invoked if valid.
Parameters: device_id, the camera index
            hWnd, the handle of the window
            x, left scene
            y, top scene
            cx, the width of scene
            cy, the height of scene
            proc, the callback function
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
//extern "C" DLL_EXPORT API_STATUS __stdcall CameraShowImage(int device_id,HDC hdc,int x,int y,int cx,int cy, CAPTURE_FRAME_PROC proc);


/*==============================================================
Name: CameraShowBufferImage
Description: Show image in a control with image data and other informations.
Parameters:  hWnd, the handle of the window
             buf, the image data
             width, the width of the image
             height, the height of the image
             color, true/RGB false/Gray
             showStretchMode, true/stretch false/no stretch
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Recommendation showStretchMode is set to true.
 
--------------------------------------------------------------*/
/*extern "C" DLL_EXPORT API_STATUS __stdcall CameraShowBufferImage(HWND hWnd,unsigned char *buf,int width,int height,bool color,bool showStretchMode);*///color :true/RGB false/Gray


/*==============================================================
Name: CameraPlayWithoutCallback
Description: Show the picture
Parameters: device_id, the camera index
            hWnd, the display control's handle
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:This function apply to situation without using the callback function. 
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraPlayWithoutCallback(int device_id,HWND hWnd);

/*==============================================================
Name: CameraPlay
Description: Show image stream in the control, invoke the callback function with every captured image.
Parameters: device_id, the camera index
            hWnd, the display control's handle
            proc, the callback function
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraPlay(int device_id,HWND hWnd,CAPTURE_FRAME_PROC proc);


/*==============================================================
Name: CameraStop
Description: Stop show the image stream started by CameraPlay
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

//extern "C" DLL_EXPORT API_STATUS __stdcall CameraStop(int device_id);

#endif // JHCAP_H

