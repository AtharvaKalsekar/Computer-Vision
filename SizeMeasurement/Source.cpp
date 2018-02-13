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
int refContourNo;
bool isRefContourSet = false;
double PPM=0;


vector<vector<Point>> findContours(Mat &img);
void drawings(vector< vector<Point> > contours, vector<RotatedRect> sortedRect, Mat &img);
vector<RotatedRect> getRotatedRect(vector< vector<Point> > contours,Mat &img);
void setPPM(vector<RotatedRect> sortedRect, int refContourNo);
vector<RotatedRect> sortRects(vector<RotatedRect> minRect);
void printRects(vector<RotatedRect> sortedRect);
void setRefContourNo(vector<RotatedRect> sortedRect);

void main()
{
	VideoCapture capture;
	VideoCapture capture1;
	VideoCapture capture2;
//	capture.open("http://192.168.43.147:8080/?action=stream"); //arm-cam
	capture.open(0);
//	capture1.open("http://192.168.43.91:8080/?action=stream");
//	capture2.open("http://192.168.43.91:8081/?action=stream");
	if (!capture.isOpened())
	{
		printf("Unable to open");
		getchar();
		return;
	}
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	Mat img;
	Mat imgC1;
	Mat imgC2;
	vector< vector<Point> > contours;
	vector<RotatedRect> minRect;
	vector<RotatedRect> sortedRect;
	while (1)
	{
		capture.read(img);
//		capture1.read(imgC1);
//		capture2.read(imgC2);
		imshow("Video Stream", img);
//		imshow("CAM - 1", imgC1);
//		imshow("CAM - 2", imgC2);
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
//	img = imread("test4.jpg");
	printf("width=%d , height=%d\n", img.cols, img.rows);
	resize(img, img, Size(640, 360));  // change size of window
	imshow("refImg", img);
	contours=findContours(img);
	minRect=getRotatedRect(contours, img);
	sortedRect = sortRects(minRect);
	printRects(sortedRect);
	setRefContourNo(sortedRect);
	setPPM(sortedRect, refContourNo);
	drawings(contours, sortedRect, img);
	capture.release();
	waitKey(0);
}

vector< vector<Point> > findContours(Mat &img)
{
//Pre-processing for finding contours
	Mat edges;
	Mat imgCopy;
	img.copyTo(imgCopy);
	cvtColor(imgCopy, imgCopy, CV_BGR2GRAY);
//	threshold(img, edges, 100, 255, THRESH_BINARY);
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
		for (int index = 0; index < contours.size(); index ++)
		{
			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;
			printf("Area = %f , Contour no. = %d\n", area, index);

		}
//	}
	return contours;
}

vector<RotatedRect> getRotatedRect(vector< vector<Point> > contours,Mat &img)
{
	Point2f rect_points[4];
	vector<RotatedRect> minRect(contours.size());
	printf("Rotated Rect co-ordis ------------- \n");
	for (int i = 0; i < contours.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(contours[i]));
		minRect[i].points(rect_points);
		printf("(x=%f ,y=%f)\n",rect_points[1].x,rect_points[1].y);
	}
	
	return minRect;

}

vector<RotatedRect> sortRects(vector<RotatedRect> minRect)
{
	Point2f rect_points[4];
	RotatedRect temp;
	float x1, x2;
	for (int i = 0; i < minRect.size(); i++)
	{
		for (int j = 0; j < minRect.size()-1; j++)
		{
			minRect[j].points(rect_points);
			x1 = rect_points[1].x;
			minRect[j+1].points(rect_points);
			x2 = rect_points[1].x;
			if (x1>x2)
			{
				temp = minRect[j];
				minRect[j] = minRect[j + 1];
				minRect[j + 1] = temp;
			}
		}
	}


	return minRect;
}

void drawings(vector< vector<Point> > contours, vector<RotatedRect> sortedRect,Mat &img)
{
//	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	float refLength1, refLength2,finalRefLength;
	double length, breadth,temp;
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// contour
//		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		
		// rotated rectangle bottomleft=0 , topleft=1,topright=2,bottomright=3
		Point2f rect_points[4];
//		Point2f rect_mpoints[4];
		sortedRect[i].points(rect_points);
		
/*		for (int k = 0; k < 4; k++)
		{
			rect_mpoints[k].x = (rect_points[k].x + rect_points[(k + 1) % 4].x) / 2;
			rect_mpoints[k].y = (rect_points[k].y + rect_points[(k + 1) % 4].y) / 2;
		}*/
		if (sortedRect[i].size.area() > 1000) {
			for (int j = 0; j < 4; j++)
			{
				//corner points
				circle(img, Point(rect_points[j].x, rect_points[j].y), 4, Scalar(0, 0, 255), -1);

				//midpoints
	//			circle(img, Point(rect_mpoints[j].x, rect_mpoints[j].y), 3, Scalar(255, 0, 0), -1);

				//line for rectangle
				line(img, rect_points[j], rect_points[(j + 1) % 4], color, 2, 8);

				//line for mpoints
	//			line(img, rect_mpoints[j], rect_mpoints[(j + 2) % 4], color, 1, 8);
			}

			if (isRefContourSet && PPM != 0)
			{
				length = powf(powf(rect_points[0].x - rect_points[1].x, 2) + powf(rect_points[0].y - rect_points[1].y, 2), 0.5) / PPM;
				breadth = powf(powf(rect_points[0].x - rect_points[3].x, 2) + powf(rect_points[0].y - rect_points[3].y, 2), 0.5) / PPM;
				if (length < breadth)
				{
					temp = length;
					length = breadth;
					breadth = temp;
				}
				stringstream stream;
				stream << fixed << "(L=" << setprecision(2) << length << ",B=" << setprecision(2) << breadth << ")";
				String s = stream.str();

				printf("contourno=%d ; length=%f ; breadth=%f\n", i, length, breadth);
				putText(img, s, rect_points[3], 1, 1, Scalar(0, 0, 255));
			}
		}

	}

	printf("\nsize of contours=%d", contours.size());
	imshow("Measurements", img);

}

void setPPM(vector<RotatedRect> sortedRect,int refContourNo)
{
	Point2f rect_points[4];
	sortedRect[refContourNo].points(rect_points);
	float refLength1, refLength2, finalRefLength;
	refLength1 = powf(powf(rect_points[0].x - rect_points[1].x, 2) + powf(rect_points[0].y - rect_points[1].y, 2), 0.5);
	refLength2 = powf(powf(rect_points[0].x - rect_points[3].x, 2) + powf(rect_points[0].y - rect_points[3].y, 2), 0.5);
	if (refLength1 > refLength2)
	{
		finalRefLength = refLength1;
	}
	else
	{
		finalRefLength = refLength2;
	}
	printf("refLength1=%f ; refLenght2=%f ;  finalRefLength=%f \n", refLength1, refLength2, finalRefLength);
	PPM = finalRefLength / 3.1;//measured diameter of blue cap 
}

void printRects(vector<RotatedRect> sortedRect)
{
	Point2f rect_points[4];
	double s;
	printf("\nSorted rects are --------\n");
	for (int k = 0; k < sortedRect.size(); k++)
	{
		sortedRect[k].points(rect_points);
		s = sortedRect[k].size.area();
		printf("(x=%f , y=%f) area = %f \n", rect_points[1].x, rect_points[1].y,s);
	}
}

void setRefContourNo(vector<RotatedRect> sortedRect)
{
	double area;
	for (int k = 0; k < sortedRect.size(); k++)
	{
		area = sortedRect[k].size.area();
		if (area > 1000)
		{
			refContourNo = k;
			isRefContourSet = true;
			printf("refContourNo = %d \n", refContourNo);
			break;
		}
	}
	return;
}