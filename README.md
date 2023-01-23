# RTSP - UDP - TCP streams in OpenCV (with neglectable latency)
It is a known issue with streams and time-consuming algorithms such as deep learning frameworks. You're getting out of sync if individual frames take longer than your stream's frame rate to process. There is an increasing delay between reality and the captured images.<br>
There are two possible solutions to this problem.<br>
You could continuously grabbing images in a seperated thread.<br>
Or determine the missed images and skip them before grabbing a new frame. The latter solution is implemented here.<br><br>
The code speaks for itself.<br>
You can either use GStreamer of FFmpeg to open the stream.<br>
RTSP streams usually are compressed with H264. Therefore your decompression is sensitive for timing issues, like threads halted for some time.

------------

## Dependencies.
To run the application, you have to:
- OpenCV installed [32-bit](https://qengineering.eu/install-opencv-4.5-on-raspberry-pi-4.html) or [64-bit](https://qengineering.eu/install-opencv-4.5-on-raspberry-64-os.html) OS<br/>
- Code::Blocks installed. (```$ sudo apt-get install codeblocks```)

------------

## Tips.
Use only a ***wired*** Ethernet connection for your RTSP stream.<br>
Wi-Fi can be unstable. And because the RTSP protocol is sensitive to even just one missing frame, the stream can easily crash.<br><br>
If you are using the stream in a deep learning app, adjust your resolution and frame rate to the requirements of the deep learning model.<br>
It is not a good idea to send a 1280x960 stream at 30 FPS if your model has a 416x416 input and takes 200 mSec to process a single frame.<br>
It only costs extra memory and processing power.<br>

------------

## Running the app.
To run the application load the project file RTSPcam.cbp in Code::Blocks.<br><br> 
If you are using a Jetson Nano, you have to change the location where OpenCV has stored its header files to `/usr/include/opencv4`<br><br>
At line 16 in main.cpp the stream is opened.
#### RTSP
```
cam.Open("rtsp://192.168.178.129:8554/test/");
```
#### UDP
```
cam.Open("rtsp://192.168.178.129:8554/test/");
```
#### TCP
```
cam.Open("rtsp://192.168.178.129:8554/test/");
```
#### MP4
```
cam.Open("James.mp4");
```
#### Folder
```
cam.Open("/home/pi/Pictures/Plants");
```
#### Single file
```
cam.Open("/home/pi/Pictures/Garden.jpg");
```
### Retrieve the stream
```cpp
int main()
{
    cv::Mat frame;
    RTSPcam cam;

    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);

    cam.Open("rtsp://192.168.178.129:8554/test/"); //you can dump anything OpenCV eats. (cv::CAP_ANY) BTW,OpenCV first tries FFmpeg

    while(true)
    {
        if(!cam.GetLatestFrame(frame)){
            cout << "Capture read error" << endl;
            break;
        }
        //place here your time consuming algorithms
//        cout << cam.CurrentFileName << endl;
        //show frame
        cv::imshow("Camera",frame);
        char esc = cv::waitKey(2);
        if(esc == 27) break;
    }
    cv::destroyAllWindows() ;
    return 0;
}
```



------------

[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 
