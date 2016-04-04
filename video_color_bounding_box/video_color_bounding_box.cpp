#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;

void cleanThresholdedImage(Mat& thresholdImage)
{
        //morphological opening (remove small objects from the foreground)
		erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)) );
        dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        

        //morphological closing (fill small holes in the foreground)
        dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );   
}

Rect largestRectInFrame(vector<Rect> rects)
{ 
	int largest_index = 0;

	for(int i = 0; i < rects.size(); i++)
	{
		if( rects[i].area() > rects[largest_index].area() )
		{
			largest_index = i;
		}
	}
	//cout << largest_index << endl;
	return rects[largest_index];
}

int main(int argc, char **argv)
{

	VideoCapture cam( 1 );
	if( !cam.isOpened() )
	{
		cout << "Cannot open the external camera, trying the internal\n";
		cam.open( 0 );
	}

	
	
	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	
	int lowHue = 17;
	int highHue = 40;
	
	int lowSaturation = 81;
	int highSaturation = 255;
	
	int lowValue = 70;
	int highValue = 199;
	
	Scalar lowRed(0, 201, 194);
	Scalar highRed(10, 255, 255);
	
	Scalar lowYellow(13, 89, 194);
	Scalar highYellow(30, 204, 255);
	
	Scalar lowBlue(98, 0, 165);
	Scalar highBlue(118, 255, 255);

	//Create Track bars in window
	cvCreateTrackbar("Low Hue", "Control", &lowHue, 179); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Control", &highHue, 179);
	cvCreateTrackbar("Low Saturation", "Control", &lowSaturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Control", &highSaturation, 255);
	cvCreateTrackbar("Low Value", "Control", &lowValue, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Control", &highValue, 255);

	Mat cap, frame, threshold, red, blue, yellow;
	bool frame_available;

	while(1)
	{
		frame_available = cam.read(cap);
		if(!frame_available)
		{
			cout << "You broke some stuff, cutting out\n";
			return 1;
		}
		cvtColor(cap, frame, CV_BGR2HSV); //convert to HSV from RGB
	
		inRange(frame, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), threshold); //threshold that thang for the "threshold" debug window	
		inRange(frame, lowRed, highRed, red);	
		inRange(frame, lowBlue, highBlue, blue);
		inRange(frame, lowYellow, highYellow, yellow);

		cleanThresholdedImage(threshold);
		cleanThresholdedImage(blue);
		cleanThresholdedImage(red);
		cleanThresholdedImage(yellow);
		
		//imshow("frame", frame);
		//imshow("threshold", threshold);
		//imshow("red", red);		
		//imshow("blue", blue);
		//imshow("yellow", yellow);


		
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		Mat temp = threshold.clone();

		findContours(temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		drawContours(frame, contours, -1, Scalar(150, 127, 200), 1, 8);
		drawContours(temp, contours, -1, Scalar(150, 127, 200), 1, 8);

		vector<Rect> bounding_rects( contours.size() );

		for( int i = 0; i < contours.size(); i++ )
		{
			bounding_rects[i] = boundingRect( Mat(contours[i]) );
			rectangle( temp, bounding_rects[i], Scalar(150, 127, 200), 1, 8);
		}

		if( bounding_rects.size() > 0 )
		{
			rectangle( temp, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
			rectangle( frame, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
			rectangle( cap, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
		}

		imshow("HSV", frame);
		imshow("RGB", cap);
		imshow("threshold", threshold);
		imshow("temp", temp);

		waitKey(17);
		//Mat_<uchar>::iterator it = threshold_frame.begin<uchar>();
		//Mat_<uchar>::iterator end = threshold_frame.end<uchar>();
	}

	return 0;
}




