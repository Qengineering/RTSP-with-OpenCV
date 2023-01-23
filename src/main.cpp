#include <opencv2/opencv.hpp>
#include "RTSPcam.h"
#include <chrono>

using namespace std;

/// 192.168.178.129:8554/test is my local RTSP streamer. Replace the address to the one you like to receive.
int main()
{
    cv::Mat frame;
    RTSPcam cam;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

//    cam.Open(0); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
    cam.Open("rtsp://192.168.178.129:8554/test/"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    cam.Open("/home/pi/Pictures/plant.jpg"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    cam.Open("/home/pi/Pictures/Storm"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg

    while(true)
    {
        if(!cam.GetLatestFrame(frame)){
            cout << "Capture read error" << endl;
            break;
        }
        ///place here your time consuming algorithms

//        cout << cam.CurrentFileName << endl;

        //show frame
        cv::imshow("Camera",frame);
        char esc = cv::waitKey(2);
        if(esc == 27) break;
    }

    cv::destroyAllWindows() ;

    return 0;
}
