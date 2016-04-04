#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat input_image = imread(argv[1]); //load the image
	Mat threshold_image;
	Mat converted_image;

	namedWindow("Threshold Image", WINDOW_NORMAL);
	namedWindow("Input Image", WINDOW_NORMAL);

	cvtColor(input_image, converted_image, CV_BGR2GRAY, 0);

	threshold(converted_image, threshold_image, 250, 255, THRESH_BINARY); //threshold the image to B/W

	std::vector<Point> white_points;
	Mat_<uchar>::iterator it = threshold_image.begin<uchar>();
	Mat_<uchar>::iterator end = threshold_image.end<uchar>();

	for(it = it; it != end; it++)
		if(*it)
			white_points.push_back(it.pos());

	RotatedRect rect = minAreaRect(Mat(white_points));

	Point2f corners[4];
	rect.points(corners);
	for(int i = 0; i < 4; i++)
		line(input_image, corners[i], corners[(i + 1) % 4], Scalar(255, 0, 0), 2);

	Rect target;
	target.x = rect.center.x - (rect.size.width/2);
	target.y = rect.center.y - (rect.size.height/2);

	target.width = rect.size.width;
	target.height = rect.size.height;

	rectangle(input_image, target, Scalar(0, 255, 0));

	Rect bounds = rect.boundingRect();
	rectangle(input_image, bounds, Scalar( 0, 0, 255));	

	imshow("Input Image", input_image);
	imshow("Threshold Image", threshold_image);
	waitKey(0);

	return 0;
}




