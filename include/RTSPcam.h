//----------------------------------------------------------------------------------------
//
// Created by Q-engineering 2022/12/14
//
//----------------------------------------------------------------------------------------
#ifndef RTSPCAM_H
#define RTSPCAM_H

#include <opencv2/opencv.hpp>


class RTSPcam
{
public:
    RTSPcam(const std::string& filename, int apiPreference = cv::CAP_ANY);
    virtual ~RTSPcam();
    bool GetLatestFrame(cv::Mat& frame);
    bool Picture;       // true when a jpg or png picture is loaded.
    double FrameTime;   // 1/FPS
protected:
    cv::VideoCapture cap;
    std::string MyFile;
private:
    std::chrono::steady_clock::time_point Tgrab;
};

#endif // RTSPCAM_H
