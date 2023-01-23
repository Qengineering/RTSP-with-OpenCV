# RTSP - UDP - TCP streams in OpenCV (with neglectable latency)
It is a known issue with RTSP streams and time-consuming algorithms such as deep learning frameworks. You're getting out of sync if individual frames take longer than your stream's frame rate to process. There is an increasing delay between reality and the captured images.<br>
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
Use only a ***wired*** Ethernet connection for your RTSP stream. Wi-Fi can be unstable.<br>
Because the RTSP protocol is sensitive to even just one missing frame, the stream can easily crash.<br><br>
If you are using the stream in a deep learning app, adjust your resolution and frame rate to the requirements of the deep learning model.<br>
It is not a good idea to send a 1280x960 stream at 30 FPS if your model has a 416x416 input and takes 200 mSec to process a single frame.<br>
It only costs extra memory and processing power.<br><br>
If you want to stream UDP or TCP, are sure the streams work with the command line prompt beforehand. If not, they certainly won't work in OpenCV. Often errors are caused by the coding in the pipeline, the addresses or missing modules. If you need to install additional GStreamer modules, you'll need to rebuild your OpenCV also! For more information, see our [website](https://qengineering.eu/install-gstreamer-1.18-on-raspberry-pi-4.html).

------------

## Running the app.
To run the application load the project file RTSPcam.cbp in Code::Blocks.<br><br> 
If you are using a **Jetson Nano**, you have to change the location where OpenCV has stored its header files to `/usr/include/opencv4`<br><br>
At line 16 in main.cpp the stream is opened.
#### RTSP
```
cam.Open("rtsp://192.168.178.129:8554/test/");
```
#### UDP
```
cam.Open("udpsrc port=5200 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink", cv::CAP_GSTREAMER);
```
Sender: RaspiCam with Raspberry Pi **Buster** OS<br>
`gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=-1 ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=192.168.178.84 port=5200`<br><br>
Sender: RaspiCam with Raspberry Pi **Bullseye** OS<br>
`gst-launch-1.0 -v libcamerasrc ! video/x-raw, width=640, height=480, framerate=30/1 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=192.168.178.84 port=5200`<br><br>
Note, `host=192.168.178.84` being the IP address of the receiver.

#### TCP
```
cam.Open("tcpclientsrc host=192.168.178.129 port=5000 ! jpegdec ! videoconvert ! appsink", cv::CAP_GSTREAMER);
```
Sender: RaspiCam with Raspberry Pi **Buster** OS<br>
`gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=-1 ! video/x-raw,width=640,height=480, framerate=30/1 ! videoconvert ! jpegenc ! tcpserversink  host=192.168.178.32 port=5000`<br><br>
Sender: RaspiCam with Raspberry Pi **Bullseye** OS<br>
`gst-launch-1.0 -v libcamerasrc ! video/x-raw,width=640,height=480, framerate=30/1 ! videoconvert ! jpegenc ! tcpserversink  host=192.168.178.32 port=5000`<br><br>
Note, `host=192.168.178.32` being the IP address of the sender.

#### RaspiCam (only Buster or Bullseye legacy stack)
```
cam.Open(0);
```
#### Webcam (only Buster OS or Bullseye legacy stack)
```
cam.Open(0); //if RaspiCam is not connected
cam.Open(1); //if RaspiCam is connected
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
![output image]( https://qengineering.eu/github/RTSPstream.webp )<br><br>
[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 
