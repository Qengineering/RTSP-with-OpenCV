#include <opencv2/opencv.hpp>
#include "RTSPcam.h"
#include <chrono>
#include <thread>

using namespace std;

/// 192.168.178.129:8554/test is my local RTSP streamer. Replace the address to the one you like to receive.
int main()
{
    cv::Mat frame;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

    RTSPcam cam("rtsp://192.168.178.129:8554/test/"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    RTSPcam cam("/home/pi/Pictures/plant.jpg"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    RTSPcam cam("/home/pi/Pictures/Storm"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg

    while(true)
    {
        if(!cam.GetLatestFrame(frame)){
            cout << "Capture read error" << endl;
            break;
        }
        ///place here your time consuming algorithms

        //show frame
        cv::imshow("Camera",frame);

        char esc = cv::waitKey(100);
        if(esc == 27) break;
    }

    cv::destroyAllWindows() ;

    return 0;
}

/*


void wait(void)
{
    double A=1.001;

    #define T 5000

    cout << "A " << A << "\n" ;

    for(int i=0;i<T;i++){
        for(int j=0;j<T;j++){
            A+=log(A);
        }
    }

    cout << "A " << A << "\n" ;
}

int main()
{
    int Cnt=0;
    cv::Mat frame;
    cv::VideoCapture *cap;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

//    for(int z=0;z<30;z++){
        cout << "Open...\n" ;

//        cap = new cv::VideoCapture;
        cap->open("rtsp://192.168.178.108:8554/test/");

        if(cap->isOpened()){
            while(++Cnt<300){
                if(cap->read(frame)){
                    if(!frame.empty()){
                        //show frame
                        cv::imshow("Camera",frame);

                        char esc = cv::waitKey(5);
                        if(esc == 27) break;
                    }
                }
            }
            cap->release();
        }

        cout << "delete...\n" ;
        delete cap;

//    }


        cout << "Open...\n" ;

        cap = new cv::VideoCapture;
        cap->open("rtsp://192.168.178.108:8554/test/");

        if(cap->isOpened()){
            while(++Cnt<600){
                if(cap->read(frame)){
                    if(!frame.empty()){
                        //show frame
                        cv::imshow("Camera",frame);

                        char esc = cv::waitKey(5);
                        if(esc == 27) break;
                    }
                }
            }
            cap->release();
        }

        cout << "delete...\n" ;
        delete cap;


    return 0;
}


int main()
{
    int MissedFrames;
    cv::Mat frame;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

    MissedFrames=0;

    RTSPcam cam("rtsp://192.168.178.108:8554/test/"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    RTSPcam cam("/home/pi/Pictures/plant.jpg"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg
//    RTSPcam cam("/home/pi/Pictures/Storm"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg

    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
        if(!cam.GetLatestFrame(frame)){
            //don't quit the app just after one missing frame
            MissedFrames++;
            std::cout << "Missed : " << MissedFrames << std::endl;

            std::cout << "wait 2 Sec" << std::endl;
            wait();
            std::cout << "reopen the stream" << std::endl;
            cam.cap.open("rtsp://192.168.178.108:8554/test/");


            if(cam.cap.isOpened()){
                std::cout << "Camera open again" << std::endl;
                if(!cam.cap.read(frame)){
                    std::cout << "still in error " << std::endl;
                }
                else{
                    cam.Tgrab = std::chrono::steady_clock::now();
                    std::cout << "we gaan door... " << std::endl;
                }
            }
            else{
                std::cout << "Camera gaat niet open. stoppen maar" << std::endl;
                MissedFrames=100;
            }

            if(MissedFrames>20){
                //missed 20 frames in a row. The stream has probably stopped.
                std::cout << "Missed : " << MissedFrames << std::endl;
                std::cout << "Capture read error" << std::endl;
                break;
            }
        }
//        else{
            MissedFrames=0;
            ///place here your time consuming algorithms

            //show frame
            cv::imshow("Camera",frame);

            char esc = cv::waitKey(100);
            if(esc == 27) break;
//        }
    }

    cv::destroyAllWindows() ;

    return 0;
}

*/
