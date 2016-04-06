/*
 * This file contains the function declarations needed to identify victims
 * in a color image. The detectVictim function will be called by the
 * navigation function once the robot is in position. If the robot sees
 * a red or yellow victim at that time it should return true. 
 */
#ifndef _VISION_FUNCTIONS_HPP_
#define _VISION_FUNCTIONS_HPP_

#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

/*
 * This function cleans colored patches that are too small from the image
 * and grows those that are sufficiently large.
 */
void cleanThresholdedImage (Mat& thresholdImage);

/*
 * This function determines which of the rectangles generated around
 * colored objects is the largest. It then returns that rectangle.
 */
Rect largestRectInFrame( vector<Rect> rects);

/*
 * This function takes a frame from the camera and detects the wanted
 * colors in the image. If the colors are detected it will return true.
 */
bool detectVictim ();
#endif
