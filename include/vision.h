#ifndef VISION_H
#define VISION_H

#include <iostream>
#include "robot_defines.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <unistd.h>

#define AREA_THRESHOLD 1200

// using namespace std;
//using namespace cv;

void clear_objects_in_frame();
bool red_object_in_frame();
bool yellow_object_in_frame();
void cleanThresholdedImage(cv::Mat& thresholdImage);
cv::Rect largestRectInFrame(std::vector<cv::Rect> rects);
bool grabFrame();
int checkColor();




#endif

