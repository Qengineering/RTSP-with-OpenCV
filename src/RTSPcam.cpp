//----------------------------------------------------------------------------------------
//
// Created by Q-engineering 2022/12/14
//
//----------------------------------------------------------------------------------------
#include "RTSPcam.h"
#include <sys/stat.h>

#define DUMMY 35
#define COUNT 35

using namespace std;
//----------------------------------------------------------------------------------------
RTSPcam::RTSPcam(const string& MyString, int apiPreference): cap(NULL), FirstPic(true)
{
    int n;
    struct stat s;
    double Elapse;
    cv::Mat frame;
    chrono::steady_clock::time_point Tyet;

    MyFile = MyString;
    MyApiPreference = apiPreference;
    UsePicture = false;
    UseFolder  = false;

    cap = new cv::VideoCapture;   //call always even when you only load one picture

    if(stat(MyString.c_str(),&s)==0){
        if(s.st_mode & S_IFREG){
            UsePicture = true;
            cout << "Open picture : " << MyFile << endl;
            return;
        }
        else{
            if(s.st_mode & S_IFDIR){
                UseFolder=true;
                cout << "Open folder : " << MyFile << endl;
                return;
            }
        }
    }

    cout << "connecting to "<< MyString << endl;

    cap->open(MyString, apiPreference);

    if(!cap->isOpened()){
        throw runtime_error("Failed to open camera");
    }

    double FPS=cap->get(cv::CAP_PROP_FPS);

    if(FPS > 0.0 && FPS < 51.0){
        FrameTime=1000.0/FPS;
    }
    else{
        //measure fps cap.get(cv::CAP_PROP_FPS) doesn't work always
        //start with some dummy reads
        for(n=0;n<DUMMY;n++) if(!cap->read(frame)) break;
        if(n!=DUMMY) throw runtime_error("Can't read some frames");

        Tyet  = chrono::steady_clock::now();
        for(n=0;n<COUNT;n++) if(!cap->read(frame)) break;
        Tgrab = chrono::steady_clock::now();

        if(n!=COUNT) throw runtime_error("Can't read some frames");

        Elapse   = chrono::duration_cast<chrono::milliseconds> (Tgrab - Tyet).count();
        FrameTime=Elapse/COUNT;
        if(FrameTime<19.0) FrameTime=19.0; //limit to 52.631 FPS max
        FPS=1000.0/FrameTime;
    }

    cout << "Open camera : " << MyString << endl;
    cout << "FPS : " << FPS << endl;
}
//----------------------------------------------------------------------------------------
RTSPcam::~RTSPcam()
{
    cap->release();
    delete cap;
}
//----------------------------------------------------------------------------------------
bool RTSPcam::GetLatestFrame(cv::Mat& frame)
{
    bool Success;
    double Elapse;
    int LostFrames;
    string FullName;

    chrono::steady_clock::time_point Tyet;

    if(UsePicture){
        frame = cv::imread(MyFile);
        return !frame.empty();cout << "delete...\n" ;
    }
    if(UseFolder){
        //open folder
        if(FirstPic){
            FirstPic=false;
            if((dir = opendir (MyFile.c_str())) == NULL){
                Success=false;
                return Success;
            }
        }
        //read folder
        while((ent = readdir(dir)) != NULL) {
            FullName = MyFile;
            if(FullName.length()>1){
                if( FullName[ FullName.length()-1] != '/') FullName += "/";
                FullName += ent->d_name;

                frame = cv::imread(FullName);
                if(!frame.empty()){
                    Success=true;
                    break;cout << "delete...\n" ;
                }
            }
        }
        return Success;
    }


    //no static picture but a video
    if(!FirstPic){
        Tyet   = chrono::steady_clock::now();
        Elapse = chrono::duration_cast<chrono::milliseconds> (Tyet - Tgrab).count();
        if(Elapse > FrameTime){
            LostFrames=ceil(1.5*Elapse/FrameTime);      //1.5 to be sure you have always the latest frame, in case FrameTime isn't accurate
            if(LostFrames>500) LostFrames=500;          //don't wait forever.
            //flush the lost frames
            for(int n=0;n<LostFrames;n++){
                if(!cap->read(frame)) break;
            }
        }
    }
    Success = cap->read(frame);
    Tgrab   = chrono::steady_clock::now();

    if(!Success){
        //lost frame -> reconnect
        cap->release();
        delete cap;
        cap=NULL;

        cout << "try to reconnect...\n" ;

        cap = new cv::VideoCapture;
        cap->open(MyFile, MyApiPreference);
        if(!cap->isOpened()){
            throw runtime_error("Failed to open camera");
        }
        Success = cap->read(frame);
        Tgrab   = chrono::steady_clock::now();

        if(Success) cout << "reconnected...\n" ;
        else        cout << "no frame received...\n" ;
    }

    FirstPic= false;

    return Success;
}
//----------------------------------------------------------------------------------------
