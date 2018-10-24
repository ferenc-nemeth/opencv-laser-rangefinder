# opencv-laser-rangefinder
How to measure distance with a webcam, a laser pointer and OpenCV.

### Table of content
- [Introduction](#introduction)
- [How it works](#how-it-works)
- [How to use it](#how-to-use-it)
- [References](#references)


### Introduction
Many (about 10 or more) years ago I have found an interesting project: a distance measurement system with a webcam and a laser pointer. It's probably long gone, but after these years, I have decided to recreate it.

### How it works
[See my demostration video on youtube.](https://www.youtube.com/watch?v=bvDV_c-mpdM)

<img src="https://raw.githubusercontent.com/ferenc-nemeth/opencv-laser-rangefinder/master/Design/output.jpg" > <br>
*Figure 1. The measured distance in cm.*

<img src="https://raw.githubusercontent.com/ferenc-nemeth/opencv-laser-rangefinder/master/Design/device.jpg" > <br>
*Figure 2. The device.*

If the object (and the laser point) is further, then the dot's position is also different. This is what the rangefinder uses to get the distance.

The computer vision part is simple. It is created in OpenCV. The video stream [[1]](#references) is turned into grayscale with cvtColor() [[2]](#references). After that, it is turned into black and white with binary threshold() [[3]](#references). Now only the laser is visible, as a white dot, everything else is black. In the black and white picture, it is possible to find the contours with findContours() [[4]](#references) and get the moments [[5]](#references) from it. With the help of moments, we can calculate the center of the laser dot.

To calculate the distance, I made some measurements of the relation between the laser dot's position and the object distance, as you can see it in Figure 3. 

<br><img src="https://raw.githubusercontent.com/ferenc-nemeth/opencv-laser-rangefinder/master/Design/measurement.png" > <br>
*Figure 3. The measured values.*

From these data, I created a look-up table and used linear interpolation [[6]](#references) to calculate the values in between the measured values.

Finally, the system draws a circle [[7]](#references) on the laser dot and writes a text [[8]](#references) about the distance in cm.

<br><img src="https://raw.githubusercontent.com/ferenc-nemeth/opencv-laser-rangefinder/master/Design/activity.png" > <br>
*Figure 4. Workflow of the laser rangefinder.*

### How to use it
To build it, you need a GCC version, that supports c++14. For OpenCV I had the latest version, I am not sure if it works or not with others.
```
$ g++ --version
g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609
$ pkg-config --modversion opencv
4.0.0
```
If you have everything, you can clone it.
```
$ git clone https://github.com/ferenc-nemeth/opencv-laser-rangefinder.git
$ cd opencv-laser-rangefinder
$ cd Src
```
Inside this folder, you have to modify the magic constants inside laser.cpp: "cam_number"; "cam_width"; "cam_height". Later, you might need to modify the "threshold_min" value too. After you are done, you can build it.
```
$ make
$ ./laser
```
Now you can use it, but the distance is going to be wrong. You have to take measurements for different distances. You have to write down the values shown in the terminal window (see figure 3). After you took enough, you have to write them inside "pixel" and "cm" vectors.

You are done, enjoy.

### References
[1] [OpenCV docs - cv::VideoCapture Class Reference](https://docs.opencv.org/master/d8/dfe/classcv_1_1VideoCapture.html) <br>
[2] [OpenCV docs - cvtColor()](https://docs.opencv.org/master/d7/d1b/group__imgproc__misc.html#ga397ae87e1288a81d2363b61574eb8cab) <br>
[3] [OpenCV docs - threshold()](https://docs.opencv.org/master/d7/d1b/group__imgproc__misc.html#gae8a4a146d1ca78c626a53577199e9c57) <br>
[4] [OpenCV docs - findContours()](https://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#ga17ed9f5d79ae97bd4c7cf18403e1689a) <br>
[5] [OpenCV docs - moments()](https://docs.opencv.org/master/d3/dc0/group__imgproc__shape.html#ga556a180f43cab22649c23ada36a8a139) <br>
[6] [Wikipedia - Linear interpolation](https://en.wikipedia.org/wiki/Linear_interpolation) <br>
[7] [OpenCV docs - circle()](https://docs.opencv.org/master/d6/d6e/group__imgproc__draw.html#gaf10604b069374903dbd0f0488cb43670) <br>
[8] [OpenCV docs - putText()](https://docs.opencv.org/master/d6/d6e/group__imgproc__draw.html#ga5126f47f883d730f633d74f07456c576)
