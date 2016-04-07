#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/*
* Yellow: 
* low Hue: 13
* high Hue: 30
* Low Saturation: 89
* High Saturation: 204
* Low Value: 194
* High Value: 255
*
* Red:
* low Hue: 0
* high Hue: 10
* Low Saturation: 201   / 150
* High Saturation: 255
* Low Value: 194        / 100
* High Value: 255
* may need to add second range of hues for red
* low hue: 170
* high hue: 180
*
* Blue:
* low Hue: 110
* High Hue: 134
* Low Saturation: 128
* High Saturation: 200
* Low Value: 87
* High Value: 217
*/
// Compile with: g++ color_detection.cpp -o color -lopencv_core -lopencv_highgui -lopencv_flann -lopencv_imgproc

// Use namespace for opencv
using namespace cv;

using namespace std;

void cleanThresholdedImage(Mat& thresholdImage)
{
	//morphological opening (remove small objects from the foreground)
	dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		
	//morphological closing (fill small holes in the foreground)
	dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );	
}


int main( int argc, char** argv)
{
	VideoCapture cap( 1 ); //capture from camera
	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	
	int lowHue = 0;
	int highHue = 0;
	
	int lowSaturation = 0;
	int highSaturation = 0;
	
	int lowValue = 0;
	int highValue = 0;
	
	Scalar lowRed(0, 201, 194);
	Scalar highRed(10, 255, 255);
	
	Scalar lowYellow(13, 89, 194);
	Scalar highYellow(30, 204, 255);
	
	Scalar lowBlue(110, 128, 87);
	Scalar highBlue(134, 200, 217);

	//Create Track bars in window
	cvCreateTrackbar("Low Hue", "Control", &lowHue, 179); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Control", &highHue, 179);
	cvCreateTrackbar("Low Saturation", "Control", &lowSaturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Control", &highSaturation, 255);
	cvCreateTrackbar("Low Value", "Control", &lowValue, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Control", &highValue, 255);

	while (true)
	{
		Mat origImage;
		
		bool bSuccess = cap.read(origImage);
		
		if (!bSuccess)
		{
			cout << "Cannot read a frame from the video stream" << endl;
			break;
		}
		Mat hsvImage;
		Mat thresholdImage;
		Mat blueImage;
		Mat redImage;
		Mat yellowImage;
		//Convert from Blue Green Red to Hue Saturation Value
		cvtColor(origImage, hsvImage, COLOR_BGR2HSV);
	
		inRange(hsvImage, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), thresholdImage); //Threshold the image
		inRange(hsvImage, lowBlue, highBlue, blueImage);
		inRange(hsvImage, lowRed, highRed, redImage);
		inRange(hsvImage, lowYellow, highYellow, yellowImage);
		cleanThresholdedImage(blueImage);
		cleanThresholdedImage(redImage);
		cleanThresholdedImage(yellowImage);
			
		imshow("Thresholded Image", thresholdImage); //show the thresholded image	
		imshow("Original Image", origImage); 
		imshow("Game Image", blueImage + redImage + yellowImage);
		
		if (waitKey(30) == 27) //wait for 'esc' key for 30 ms
		{
			cout << "Exiting..." << endl;
			break;
		}
	}
	
	return 0;
}
