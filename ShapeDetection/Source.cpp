#include<stdio.h>
#include<math.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<iomanip>
#include<sstream>

using namespace cv;
using namespace std;

int FRAME_WIDTH = 640;
int FRAME_HEIGHT = 360;
RNG rng(12345);
vector<vector<Point>> findContours(Mat &img);
Mat drawings(vector< vector<Point> > contours,  Mat &img);
vector< vector<Point> > getColourAndShape(vector< vector<Point> > contours, vector< vector<Point> > contoursApprox);
//void findContours2();
double perimeter;

void main()
{
	VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		printf("Unable to open");
		getchar();
		return;
	}
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	Mat camerafeed;
	Mat contourDrawing;
	vector< vector<Point> > contours;
	vector< vector<Point> > contoursApprox;
	while (1)
	{
		capture.read(camerafeed);
		imshow("eyes", camerafeed);
		blur(camerafeed, camerafeed, Size(9, 9));
		imshow("blur", camerafeed);
		cvtColor(camerafeed, camerafeed, COLOR_BGR2GRAY);
		threshold(camerafeed, camerafeed, 250, 255, THRESH_BINARY);
		imshow("Video Stream", camerafeed);
		char c = (char)waitKey(25);
		if (c == 'a')
		{
			
			break;
		}
		if (c == 27)
		{
			capture.release();
			return;
		}
	}
	imshow("refImg", camerafeed);
	contours = findContours(camerafeed);
	contourDrawing=drawings(contours, camerafeed);
	contoursApprox=getColourAndShape(contours, contoursApprox);
/*	contoursApprox.resize(contours.size());
	
	for (size_t k = 0; k < contours.size(); k++)
	{
		approxPolyDP(Mat(contours[k]), contoursApprox[k], 50, true);
		printf("size= %d \n", contoursApprox[k].size());
		switch (contoursApprox[k].size())
		{
		case 3:printf("Trinagle \n");
			break;
		case 4:printf("Square \n");
			break;
		case 5:printf("Pentagon \n");
			break;
		default:printf("Circle \n");
			break;
		}
	}*/
	contourDrawing = drawings(contoursApprox,camerafeed);
	imshow("Approx poly", contourDrawing);

//	capture.release();
	waitKey(0);
}

vector< vector<Point> > findContours(Mat &img)
{
	//Pre-processing for finding contours
	Mat edges;
	Mat imgCopy;
	img.copyTo(imgCopy);
//	cvtColor(imgCopy, imgCopy, CV_BGR2GRAY);
//	threshold(img, edges, 200, 255, THRESH_BINARY);
	GaussianBlur(imgCopy, imgCopy, Size(7, 7), 0);
	Canny(imgCopy, edges, 10, 60);      //Canny(imgCopy, edges, 50, 100);
	dilate(edges, edges, noArray());
	erode(edges, edges, noArray());
	imshow("edges", edges);

	//Finding contours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//	if (hierarchy.size() > 0) 
	//	{
	//	for (int index = 0; index >= 0; index = hierarchy[index][0])
	for (int index = 0; index < contours.size(); index++)
	{
		Moments moment = moments((cv::Mat)contours[index]);
		double area = moment.m00;
		printf("Area = %f , Contour no. = %d\n", area, index);

	}
	//	}
	return contours;
}

Mat drawings(vector< vector<Point> > contours, Mat &img)
{
	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}
	return drawing;
	imshow("contours", drawing);
}

vector< vector<Point> > getColourAndShape(vector< vector<Point> > contours, vector< vector<Point> > contoursApprox)
{
	contoursApprox.resize(contours.size());

	for (size_t k = 0; k < contours.size(); k++)
	{
		approxPolyDP(Mat(contours[k]), contoursApprox[k], 50, true);
		printf("size= %d \n", contoursApprox[k].size());
		switch (contoursApprox[k].size())
		{
		case 3:printf("Trinagle \n");
			break;
		case 4:printf("Square \n");
			break;
		case 5:printf("Pentagon \n");
			break;
		default:printf("Circle \n");
			break;
		}
	}
	return contoursApprox;
}

/*
void findContours2(Mat &image)
{
	IplImage* img;
//	img = &image;
	IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);

	//thresholding the grayscale image to get better results
	cvThreshold(imgGrayScale, imgGrayScale, 128, 255, CV_THRESH_BINARY);

	CvSeq* contours;  //hold the pointer to a contour in the memory block
	CvSeq* result;   //hold sequence of points of a contour
	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours

												   //finding all contours in the image
	cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
}*/