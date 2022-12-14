#include <opencv2/opencv.hpp>
#include "RTSPcam.h"

//#define GSTREAMER
//#define FFMEP
#define SIMPLE_FFMPEG

/// 192.168.178.129:8554/test is my local RTSP streamer. Replace the address to the one you like to receive.
int main()
{
    cv::Mat frame;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
#ifdef GSTREAMER
    std::string pipeline = "rtspsrc location=rtsp://192.168.178.129:8554/test/ latency=0 ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! appsink";

    RTSPcam cam(pipeline, cv::CAP_GSTREAMER);
#endif // GSTREAMER

#ifdef FFMEP
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
    const std::string RTSP_URL = "rtsp://192.168.178.129:8554/test/";

    RTSPcam cam(RTSP_URL, cv::CAP_FFMPEG);
#endif //FFMPEG

#ifdef SIMPLE
    RTSPcam cam("rtsp://192.168.178.129:8554/test/"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
#endif //SIMPLE_FFMPEG

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
        if(!cam.GetLatestFrame(frame)){
            std::cout << "Capture read error" << std::endl;
            break;
        }

        ///place here your time consuming algorithms

        //show frame
        cv::imshow("Camera",frame);

        char esc = cv::waitKey(1);
        if(esc == 27) break;
    }

    cv::destroyAllWindows() ;

    return 0;
}


