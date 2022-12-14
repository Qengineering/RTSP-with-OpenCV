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
    double FrameTime;   // 1/FPS
protected:
    cv::VideoCapture cap;
private:
    std::chrono::steady_clock::time_point Tgrab;
};

#endif // RTSPCAM_H
