#ifndef VISION_H
#define VISION_H

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <unistd.h>
#include "robot_defines.h"

#define AREA_THRESHOLD 1200

bool red_object_in_frame();

bool yellow_object_in_frame();

bool init_camera();

void cleanThresholdedImage(Mat& thresholdImage);

Rect largestRectInFrame(vector<Rect> rects);

bool grabFrame();

void set_global_colors();

void clear_global_colors();


#endif