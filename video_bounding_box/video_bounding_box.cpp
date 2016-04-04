#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	VideoCapture cam( 0 );
	if( !cam.isOpened() )
	{
		cout << "Cannot open the camera\n";
	}

	int low_value = 0;
	namedWindow("Threshold Adjustment", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Low Binary Threshold:", "Threshold Adjustment", &low_value, 254);

	Mat frame, threshold_frame;
	bool frame_available;

	while(1)
	{
		frame_available = cam.read(frame);
		if(!frame_available)
		{
			cout << "You broke some stuff, cutting out\n";
			return 1;
		}

		cvtColor(frame, threshold_frame, CV_BGR2GRAY, 0);
	
		threshold(threshold_frame, threshold_frame, low_value, 255, THRESH_BINARY); //threshold the image to B/W
	
		std::vector<Point> white_points;
		Mat_<uchar>::iterator it = threshold_frame.begin<uchar>();
		Mat_<uchar>::iterator end = threshold_frame.end<uchar>();
	
		for(it = it; it != end; it++)
			if(*it)
				white_points.push_back(it.pos());
		if( !white_points.empty() )
		{	
			RotatedRect rect = minAreaRect(Mat(white_points));
	
			/*Point2f corners[4];
			rect.points(corners);
			for(int i = 0; i < 4; i++)
			line(frame, corners[i], corners[(i + 1) % 4], Scalar(255, 0, 0), 2);
		
			Rect target;
			target.x = rect.center.x - (rect.size.width/2);
			target.y = rect.center.y - (rect.size.height/2);
		
			target.width = rect.size.width;
			target.height = rect.size.height;
		
			rectangle(frame, target, Scalar(0, 255, 0));
			*/
			Rect bounds = rect.boundingRect();
			rectangle(frame, bounds, Scalar( 0, 0, 255));	
			rectangle(threshold_frame, bounds, Scalar( 255, 255, 255));
			cout << "target X: " << rect.center.x	<< "  target Y: " << rect.center.y << endl;
		}
		else
			cout << "No white points found!\n";

			imshow("Frame Image", frame);
			imshow("Threshold Image", threshold_frame);
			if(waitKey(30) == 27)
				return 0;
					
	}

	return 0;
}




