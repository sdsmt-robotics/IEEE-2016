#include "../include/vision.h"

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

void clear_objects_in_frame()
{
    red_object_seen = false;
    yellow_object_seen = false;
}

bool red_object_in_frame()
{
	return red_object_seen;
}

bool yellow_object_in_frame()
{
	return yellow_object_seen;
}


bool init_camera()
{
	camera.open( 1 );

	if( !camera.isOpened() )
	{
		cout << "Cannot open video1 (laptop), trying video0 (webcam)\n";
		camera.open( 0 );
		return 1;
	}
	return 0;
}

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

bool grabFrame()
{	
	Mat cap, frame, red, red_lower, red_upper, yellow;
	bool frame_available;

	frame_available = camera.read(cap);
	if(!frame_available)
	{
		cout << "Unable to grab frame from camera, might not be initialized or maybe unpluged?\n";
		return 1;
	}
	cvtColor(cap, frame, CV_BGR2HSV); //convert to HSV from RGB

	inRange(frame, lowLowerRed, highLowerRed, red_lower);	
	inRange(frame, lowUpperRed, highUpperRed, red_upper);
	inRange(frame, lowYellow, highYellow, yellow);

	cleanThresholdedImage(red_lower);
	cleanThresholdedImage(red_upper);
	cleanThresholdedImage(yellow);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Rect> bounding_rects;

	//same kit as above for the yellow
	findContours(yellow, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for( int i = 0; i < contours.size(); i++ )
	{
		bounding_rects.push_back( boundingRect( Mat(contours[i]) ) );
	}
	if(bounding_rects.size() > 0)
	{
		Rect largest = largestRectInFrame(bounding_rects);
		rectangle( yellow, largest, Scalar(150, 127, 200), 1, 8);
		if(largest.area() > AREA_THRESHOLD)
		{	
			cout << "Yellow object center at: (" << (largest.x + largest.width/2) 
			<< ", " << (largest.y + largest.height/2) << ")" << endl;
			yellow_object_seen = true;
		}
		else
		{
			cout << "There's somehting yellow there, but not big enough." << endl;
			yellow_object_seen = false;
		}
	}
	else
	{
		cout << "Nothing yellow, fam." << endl;
		yellow_object_seen = false;
	}

	contours.clear();
	hierarchy.clear();
	bounding_rects.clear();

	//same thing for the red

	red = red_lower + red_upper;
	findContours(red, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for( int i = 0; i < contours.size(); i++ )
	{
		bounding_rects.push_back( boundingRect( Mat(contours[i]) ) );
	}
	if(bounding_rects.size() > 0)
	{
		Rect largest = largestRectInFrame(bounding_rects);
		rectangle( red, largest, Scalar(150, 127, 200), 1, 8);
		if(largest.area() > AREA_THRESHOLD)
		{
			cout << "Red object center at: (" << (largest.x + largest.width/2)
			<< ", " << (largest.y + largest.height/2) << ")" << endl;
			red_object_seen = true;
		}
		else
		{
			cout << "There's somehting red there, but not big enough." << endl;
			red_object_seen = false;
		}
	}
	else
	{
		cout << "No red, fam." << endl;
		red_object_seen = false;
	}

	contours.clear();
	hierarchy.clear();
	bounding_rects.clear();

	return 0;
}

int checkColor()
{
    for(int i = 0; i < 20; i++)
    {
        grabFrame();
    }
    if(yellow_object_seen)
        return YELLOW;
    else if(red_object_seen)
        return RED;
    else
        return UNKNOWN_COLOR;
}