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
    RTSPcam(void);
    virtual ~RTSPcam();

    //used with Buster or Bullseye+Legacy Video Stack (0 opens RaspiCam)
    void Open(const int Value);
    //GStreamer or FFmpeg pipeline
    void Open(const std::string& MyString, const int apiPreference = cv::CAP_ANY);

    //get the lateste frame without (almost no letency)
    bool GetLatestFrame(cv::Mat& frame);

    double FPS;                     // FPS
    bool UsePicture;                // true when a jpg, png or bmp picture is loaded.
    bool UseFolder;                 // true when a only folder name is loaded.
    std::string CurrentFileName;    // get the name of the picture or frame number
protected:
    cv::VideoCapture* cap;
    bool FirstPic;
    size_t FrameCnt;
    std::string MyFile;
private:
    DIR *dir;
    struct dirent *ent;
    double FrameTime;               // 1/FPS
    void ProcessOpen(void);
    void NiceString(const size_t Cnt);
    void NiceString(const std::string Str);
    std::chrono::steady_clock::time_point Tgrab;
};
#endif // RTSPCAM_H
