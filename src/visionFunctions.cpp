#include "../include/visionFunctions.hpp"

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

//This function will determine if a red or yellow victim is in the picture.
//if there is a victim, the function will return true, else returns false.
bool detectVictim ()
{
    //Copied from the video_color file.
    //Opens the camera device.
    VideoCapture cam(0);
    if( !cam.isOpened() )
    {
        cout << "Cannot open the external camera, trying the internal\n" << endl;
        cam.open (0);
    }
      

   
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

    //Mat cap, frame, threshold, red, blue, yellow;
    Mat cap, frame, threshold, red, yellow;
    bool frame_available;


    frame_available = cam.read(cap);
    if(!frame_available)
    {
        cout << "You broke some stuff, cutting out\n";
        return 1;
    }
    cvtColor(cap, frame, CV_BGR2HSV); //convert to HSV from RGB

    inRange(frame, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), threshold); //threshold that thang for the "threshold" debug window 
    inRange(frame, lowRed, highRed, red);
    //inRange(frame, lowBlue, highBlue, blue);
    inRange(frame, lowYellow, highYellow, yellow);

    cleanThresholdedImage(threshold);
    //cleanThresholdedImage(blue);
    cleanThresholdedImage(red);
    cleanThresholdedImage(yellow);

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
        return true;
    }

    cvNamedWindow( "Image", CV_WINDOW_AUTOSIZE);
    cvShowImage( "Image", cap);
    cvWaitKey(100);
    cvReleaseImage( &cap );
    cvDestroyWindow( "Image" );

    return false; 
}
