#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/bgsegm.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;


Mat frame; 
Mat fgMaskMOG2; 
Ptr<BackgroundSubtractor> pMOG2; 
char keyboard;
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT =360;

void processVideo();
Point2i locate(Mat thold,Mat* output);
Rect drawRect(Mat* frame,Point2i center);


int main()
{
	pMOG2 = createBackgroundSubtractorMOG2();
	processVideo();
	destroyAllWindows();
	return 0;
}

void processVideo() {
	Mat firstFrame;
	int c = 0;
	Mat gray;
	Mat binary;
	vector<Vec3f> circles;
	Point2i centerOfObject;
	Rect recAroundOject;
	
	VideoCapture capture(1);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	if (!capture.isOpened()) {
		printf("Unable to open camerafeed \n");
		return;
	}

	keyboard = 0;
	while (keyboard != 'q' && keyboard != 27) {
		if (!capture.read(frame)) {
			printf("Unable to read next frame \n");
			return;
		}
		pMOG2->apply(frame, fgMaskMOG2);
		erode(fgMaskMOG2, binary, Mat(), Point(-1, -1), 2);
		dilate(binary, binary, Mat(), Point(-1, -1), 2);
		centerOfObject = locate(binary, &frame);
		recAroundOject = drawRect(&frame, centerOfObject);
		if (c == 0)
		{
			fgMaskMOG2.copyTo(firstFrame);
			c++;
		}
		imshow("Frame", frame);
		imshow("FG Mask MOG 2.2", binary);
		keyboard = (char)waitKey(30);

	}
	capture.release();
}
Point2i locate(Mat thold,Mat* output)
{
	Point2i center;
	Mat temp;
	int x, y;
	thold.copyTo(temp);
	//threshold(temp, temp, 100, 255, CV_THRESH_BINARY);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	if (hierarchy.size()>0) 
	{
		for (int index = 0; index >= 0; index = hierarchy[index][0])
		{
			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;
			if (area > 300)
			{
				x = moment.m10 / area;
				y = moment.m01 / area;
				
			}
			printf("Area = %f ; x=%d ; y=%d\n", area, x, y);
		}
	}

	if (x > 0 && y > 0)
	{
		circle(*output, Point(x, y), 2, Scalar(0, 0, 255), -1);
		center.x = x;
		center.y = y;
		return center;
	}
	else
	{
		center.x = 0;
		center.y = 0;
		return center;
	}
}

Rect drawRect(Mat* frame, Point2i center)
{
	Rect rec=Rect(0,0,0,0);
	if (center.x > 0 && center.y > 0)
	{
		rec=Rect(Point2i(center.x - 20, center.y - 20), Point2i(center.x + 20, center.y + 20));
		rectangle(*frame, rec, Scalar(0, 0, 255));
	}
	return rec;
}
