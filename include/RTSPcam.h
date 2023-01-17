//----------------------------------------------------------------------------------------
//
// Created by Q-engineering 2022/12/14
//
//----------------------------------------------------------------------------------------
#ifndef RTSPCAM_H
#define RTSPCAM_H

#include "dirent.h"
#include <opencv2/opencv.hpp>

class RTSPcam
{
public:
    RTSPcam(const std::string& MyString, int apiPreference = cv::CAP_ANY);
    virtual ~RTSPcam();
    bool GetLatestFrame(cv::Mat& frame);
    bool UsePicture;    // true when a jpg, png or bmp picture is loaded.
    bool UseFolder;     // true when a only folder name is loaded.
    double FrameTime;   // 1/FPS
protected:
    cv::VideoCapture* cap;
    bool FirstPic;
    std::string MyFile;
    int MyApiPreference;
private:
    DIR *dir;
    struct dirent *ent;
    std::chrono::steady_clock::time_point Tgrab;
};

#endif // RTSPCAM_H
