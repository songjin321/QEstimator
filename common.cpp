#include "common.h"
#include <memory>
#include <iostream>

using namespace cv;
cv::Point2f operator/ (const cv::Point2f &p1,float& p2){
    Point2f p;
    p.x=p1.x/p2;
    p.y=p1.y/p2;\
    return p;
}
Mat fromQImage2Mat(const QImage &img){
    Mat mat;
    switch(img.format()){
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat =   Mat(img.height(),img.width(),CV_8UC4,
                    (void*)(img.constBits()),img.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat =   Mat(img.height(),img.width(),CV_8UC3,
                    (void*)(img.constBits()),img.bytesPerLine());
        cvtColor(mat,mat,CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
    //case QImage::Format_Grayscale8:
        mat =   Mat(img.height(),img.width(),CV_8UC1,
                    (void*)(img.constBits()),img.bytesPerLine());
        break;
    default:
        std::cout<<"not the expected type!"<<std::endl;
        mat = Mat::zeros(img.height(),img.width(),CV_8UC1);
        break;
    }

    return mat;
}

QImage fromMat2QImage(const Mat &mat){
    //8 bits unsigned, 1 channel
    if(mat.type() == CV_8UC1){
        QImage image(mat.cols,mat.rows,QImage::Format_Indexed8);
        //set color table
        image.setColorCount(256);
        for(int i=0;i<256;++i)
            image.setColor(i,qRgb(i,i,i));
        //copy
        uchar* pSrc =   mat.data;
        for(int r = 0;r<mat.rows;++r){
            uchar* pDest    =   image.scanLine(r);
            memcpy(pDest,pSrc,mat.cols);
            pSrc    +=  mat.step;
        }
        return image;
    }

    //3 channels
    else if (mat.type() == CV_8UC3){
        const uchar* pSrc   =   static_cast<const uchar*>(mat.data);
        QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_RGB888);
        return image.rgbSwapped();//from BGR to RGB
    }
    //4 channels
    else if(mat.type() == CV_8UC4){
        const uchar* pSrc   =   static_cast<const uchar*>(mat.data);
        QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_ARGB32);
        return image.copy();
    }
    else{
        //not the expected type
        return QImage();
    }
}

float crossProduct2D(const cv::Point2f& p1, const cv::Point2f& p2){
    return p1.x*p2.y-p1.y*p2.x;
}

float length2D(const cv::Point2f& p){
    return std::sqrt(p.x*p.x+p.y*p.y);
}
void IMSHOW(const std::string &winName, const Mat &mat){
    const Size maxSize(640,480);

    Size size;
    if(maxSize.width*mat.rows > mat.cols*maxSize.height){
        size.height =   maxSize.height;
        size.width  =   size.height*mat.cols/static_cast<float>(mat.rows);
    }else{
        size.width  =   maxSize.width;
        size.height =   size.width*mat.rows/static_cast<float>(mat.cols);
    }

    Mat cache;
    resize(mat,cache,size);
    imshow(winName,cache);
}
