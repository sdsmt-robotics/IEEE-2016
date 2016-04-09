#ifndef VISION_H
#define VISION_H

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <unistd.h>
#include "robot_defines.h"

#define AREA_THRESHOLD 1200

using namespace std;
using namespace cv;

VideoCapture camera;

bool yellow_object_seen = false;
bool red_object_seen    = false;

Scalar lowLowerRed(0, 119, 117);
Scalar highLowerRed(12, 255, 255);

Scalar lowUpperRed(165, 119, 117);
Scalar highUpperRed(179, 255, 255);
    
Scalar lowYellow(15, 50, 118);
Scalar highYellow(45, 255, 255);
    
Scalar lowBlue(98, 0, 165);
Scalar highBlue(118, 255, 255);

bool red_object_in_frame();

bool yellow_object_in_frame();

bool init_camera();

void cleanThresholdedImage(Mat& thresholdImage);

Rect largestRectInFrame(vector<Rect> rects);

bool grabFrame();

void set_global_colors();

void clear_global_colors();


#endif