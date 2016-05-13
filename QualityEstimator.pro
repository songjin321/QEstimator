#create 20150918
#/home/ubuntu/Desktop/QEpro/QualityEstimator/

QT  +=  widgets



HEADERS += ui/MainWindow.h \
    Criterion.h \
    common.h \
    CriterionManager.h \
    ui/CriterionListWidget.h \
    WorkpieceEstimator.h \
    LogContainer.h \
    EstimateSystem.h \
    ui/ImageContainer.h \
    Pretreatment.h \
    FeatureExtractor.h \
    ui/CriterionEditor.h \
    ui/UtilityWidgets.h \
    ui/CaptureDevice.h \
    ui/PylonCamera.h \
    tiXml/tinystr.h \
    tiXml/tinyxml.h \
    ioxml.h

SOURCES +=\
    ui/MainWindow.cpp \
    Criterion.cpp \
    CriterionManager.cpp \
    ui/CriterionListWidget.cpp \
    EstimateSystem.cpp \
    WorkpieceEstimator.cpp \
    LogContainer.cpp \
    ui/ImageContainer.cpp \
    Pretreatment.cpp \
    FeatureExtractor.cpp \
    common.cpp \
    ui/CriterionEditor.cpp \
    ui/UtilityWidgets.cpp \
    ui/CaptureDevice.cpp \
    ui/PylonCamera.cpp \
    tiXml/tinystr.cpp \
    tiXml/tinyxml.cpp \
    tiXml/tinyxmlerror.cpp \
    tiXml/tinyxmlparser.cpp \
    ioxml.cpp \
    main.cpp

FORMS += \
    ui/MainWindow.ui \
    ui/CriterionListWidget.ui \
    ui/CriterionEditor.ui

CONFIG  +=  c++11

#opencv required

INCLUDEPATH += F:/Opencv/opencv/build/include\
                F:/Opencv/opencv/build/include/opencv \
                F:/Opencv/opencv/build/include/opencv/opencv2
INCLUDEPATH +=  ./ui

LIBS += -LF:/Opencv/opencv/build/x64/vc12/lib\
        -l/opencv_world300d\

#pylon camera required
INCLUDEPATH +=  F:/Vision_Detect/include\
                F:/Vision_Detect/include/pylon\
                F:/Vision_Detect/include/pylon/usb\

LIBS += -LF:/Vision_Detect/lib/x64\
        -l/GCBase_MD_VC120_v3_0_Basler_pylon_v5_0.lib\
        -l/GenApi_MD_VC120_v3_0_Basler_pylon_v5_0.lib\
        -l/PylonBase_MD_VC120_v5_0.lib\
        -l/x64/PylonC_MD_VC120.lib\
        -l/PylonGUI_MD_VC120_v5_0.lib\
        -l/PylonUtility_MD_VC120_v5_0.lib\



