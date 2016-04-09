#ifndef VISION_H
#define VISION_H

#include <iostream>
#include <vector>
#include <unistd.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../include/serial.h"
#include "../include/robot_defines.h"
#include "../include/logger.h"
#include "../include/navigation.h"
#include "../include/locomotion.h"
#include "../include/sensors.h"
#include "../include/vision.h"

#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define AREA_THRESHOLD 1200

void clear_objects_in_frame();
bool red_object_in_frame();
bool yellow_object_in_frame();
void cleanThresholdedImage(cv::Mat& thresholdImage);
cv::Rect largestRectInFrame(std::vector<cv::Rect> rects);
bool grabFrame();
int checkColor();




#endif

