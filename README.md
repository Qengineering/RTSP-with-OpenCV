# RTSP with OpenCV (with neglectable latency)
It is a known issue with RTSP streams and time-consuming algorithms such as deep learning frameworks. You're getting out of sync if individual frames take longer than your stream's frame rate to process. There is an increasing delay between reality and the captured images.<br>
There are two possible solutions to this problem.<br>
You could continuously grabbing images in a seperated thread.<br>
Or determine the missed images and flush them before grabbing a new frame. The latter solution is implemented here.<br><br>
The code speaks for itself.<br>
You can either use GStreamer of FFmpeg to open the stream.<br>
RTSP streams usually are compressed with H264. Therefore your decompression is sensitive for timing issues, like threads halted for some time.
