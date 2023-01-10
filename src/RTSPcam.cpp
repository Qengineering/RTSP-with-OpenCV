//----------------------------------------------------------------------------------------
//
// Created by Q-engineering 2022/12/14
//
//----------------------------------------------------------------------------------------
#include "RTSPcam.h"

#define DUMMY 35
#define COUNT 35
//----------------------------------------------------------------------------------------
RTSPcam::RTSPcam(const std::string& filename, int apiPreference)
{
    int n;
    cv::Mat frame;
    std::chrono::steady_clock::time_point Tyet;

    MyFile=filename;

    Picture=(filename.find("jpg")!=std::string::npos || filename.find("png")!=std::string::npos);

    if(Picture){
        std::cout << "Open picture : " << filename << std::endl;
        return;
    }

    cap.open(filename, apiPreference);

    if(!cap.isOpened()) {
        throw std::runtime_error("Failed to open camera");
    }
    double FPS=cap.get(cv::CAP_PROP_FPS);

    if(FPS > 0.0 && FPS < 51.0){
        FrameTime=1000.0/FPS;
    }
    else{
        //measure fps cap.get(cv::CAP_PROP_FPS) doesn't work always
        //start with some dummy reads
        for(n=0;n<DUMMY;n++){
            if(!cap.read(frame)) break;
        }
        if(n!=DUMMY){
            throw std::runtime_error("Can't read some frames");
        }
        Tyet   = std::chrono::steady_clock::now();
        for(n=0;n<COUNT;n++){
            if(!cap.read(frame)) break;
        }
        if(n!=COUNT){
            throw std::runtime_error("Can't read some frames");
        }
        Tgrab = std::chrono::steady_clock::now();
        double Elapse = std::chrono::duration_cast<std::chrono::milliseconds> (Tgrab - Tyet).count();
        FrameTime=Elapse/COUNT;
        if(FrameTime<19.0) FrameTime=19.0; //limit to 52.631 FPS max
        FPS=1000.0/FrameTime;
    }

    std::cout << "Open camera : " << filename << std::endl;
    std::cout << "FrameTime : " << FrameTime << std::endl;
    std::cout << "FPS : " << FPS << std::endl;
}
//----------------------------------------------------------------------------------------
RTSPcam::~RTSPcam()
{
    cap.release();
}
//----------------------------------------------------------------------------------------
bool RTSPcam::GetLatestFrame(cv::Mat& frame)
{
    bool Success;
    double Elapse;
    int LostFrames;
    std::chrono::steady_clock::time_point Tyet;

    if(Picture){
        frame = cv::imread(MyFile);
        return !frame.empty();
    }

    //no static picture but a video
    Tyet   = std::chrono::steady_clock::now();
    Elapse = std::chrono::duration_cast<std::chrono::milliseconds> (Tyet - Tgrab).count();
    if(Elapse > FrameTime){
        LostFrames=ceil(1.5*Elapse/FrameTime);      //1.5 to be sure you have always the latest frame, in case FrameTime isn't accurate
        if(LostFrames>500) LostFrames=500;  //don't wait forever.
        //flush the lost frames
        for(int n=0;n<LostFrames;n++){
            if(!cap.read(frame)) break;
        }
    }
    Success = cap.read(frame);
    Tgrab   = std::chrono::steady_clock::now();
    return Success;
}
//----------------------------------------------------------------------------------------
