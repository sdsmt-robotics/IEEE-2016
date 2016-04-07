#include "../include/visionFunctions.hpp"
#include "../include/robot_defines.h"

void cleanThresholdedImage(Mat& thresholdImage)
{
    //morphological opening (remove small objects from the foreground)
    erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

    //morphological closing (fill small holes in the foreground)
    dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    erode( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
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

/* 
int lowHue = 17;
int highHue = 40;
        
int lowSaturation = 81;
int highSaturation = 255;
        
int lowValue = 70;
int highValue = 199;
*/
      



//This function will determine if a red or yellow victim is in the picture.
//if there is a victim, the function will return true, else returns false.
int detectVictim ()
{
    bool red_detected = false;
    bool yellow_detected = false;    

    //Copied from the video_color file.
    //Opens the camera device.
    VideoCapture cam(1);
    if( !cam.isOpened() )
    {
        cout << "Cannot open the external camera, trying the internal\n" << endl;
        cam.open (0);
    }
      

  
    Scalar lowRed(0, 0, 0);
    Scalar highRed(15, 255, 255);

    Scalar lowRed2(165, 0, 0);
    Scalar highRed2(180, 255, 255);
        
    //Scalar lowYellow(13, 89, 194);
    //Scalar highYellow(30, 204, 255);
    
    Scalar lowYellow(23, 70, 97);
    Scalar highYellow(40, 255, 255);   
  
    //Scalar lowBlue(98, 0, 165);
    //Scalar highBlue(118, 255, 255);

    //Mat cap, frame, threshold, red, blue, yellow;
    Mat cap, frame, /* threshold,*/ red, red2, yellow;
    bool frame_available;

	/*
	namedWindow("Control", CV_WINDOW_AUTOSIZE);

	//Create Track bars in window
	cvCreateTrackbar("Low Hue", "Control", &lowHue, 180); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Control", &highHue, 180);
	cvCreateTrackbar("Low Saturation", "Control", &lowSaturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Control", &highSaturation, 255);
	cvCreateTrackbar("Low Value", "Control", &lowValue, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Control", &highValue, 255);
	*/
	
	
    frame_available = cam.read(cap);
    if(!frame_available)
    {
        cout << "You broke some stuff, cutting out\n";
        return 1;
    }
    cvtColor(cap, frame, CV_BGR2HSV); //convert to HSV from RGB

    //inRange(frame, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), threshold); //threshold that thang for the "threshold" debug window 
    inRange(frame, lowRed, highRed, red);
    inRange(frame, lowRed2, highRed2, red2);
    //inRange(frame, lowBlue, highBlue, blue);
    inRange(frame, lowYellow, highYellow, yellow);

    //cleanThresholdedImage(threshold);
    //cleanThresholdedImage(blue);
    cleanThresholdedImage(red);
    cleanThresholdedImage(red2);
    cleanThresholdedImage(yellow);
    
    red += red2;

    vector<vector<Point> > contours_red;
    vector<Vec4i> hierarchy_red;
    //Mat debug = red.clone() + yellow;
    
    vector<vector<Point> > contours_yellow;
    vector<Vec4i> hierarchy_yellow;

    Mat temp_red = red.clone();
    Mat temp_yellow = yellow.clone();

    findContours(temp_red, contours_red, hierarchy_red, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    findContours(temp_yellow, contours_yellow, hierarchy_yellow, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    //drawContours(frame, contours, -1, Scalar(150, 127, 200), 1, 8);
    //drawContours(temp, contours, -1, Scalar(150, 127, 200), 1, 8);

    vector<Rect> bounding_rects_red( contours_red.size() );
    vector<Rect> bounding_rects_yellow(contours_yellow.size() );

    for( int i = 0; i < contours_red.size(); i++ )
    {
        bounding_rects_red[i] = boundingRect( Mat(contours_red[i]) );
        //rectangle( temp, bounding_rects[i], Scalar(150, 127, 200), 1, 8);
    }
    for( int i = 0; i < contours_yellow.size(); i++)
    {
        bounding_rects_yellow[i] = boundingRect( Mat(contours_yellow[i]));
    }
    

    //imshow( "Debug", debug);
    //imshow( "Detection", temp);
    //imshow( "Frame capture", cap);
    //imshow( "Test threshold", threshold);

    //waitKey(1);

    if( bounding_rects_red.size() > 0 )
    {
        //TODO: implement a check to make sure the red detected is the right size        

        //rectangle( temp, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
        //rectangle( frame, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
        //rectangle( cap, largestRectInFrame(bounding_rects), Scalar(160, 200, 200), 2, 8);
        red_detected = true;
    }
    if ( bounding_rects_yellow.size() > 0 )
    {
        //TODO: implement a check to make sure the yellow detected is the right size        

        yellow_detected = true;
    }
    if ( red_detected && yellow_detected || !red_detected && !yellow_detected)
    {
        return UNKNOWN_COLOR;
    }
    else if ( red_detected )
    {
        return RED;
    }
	
    return YELLOW; 
}
