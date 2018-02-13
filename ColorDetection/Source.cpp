#include<stdio.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<Windows.h>
#include<opencv\cxcore.h>

using namespace cv;
using namespace std;

struct colourConfidence
{
	double totalArea = 0;
};

double getArea(Mat outImg,Mat outGray,Mat outBin);
Rect getRect(Mat &img);
char getColour(Mat &crop,Mat &outImg,Mat &outGray,Mat &outBin);
int getMaxAreaIndex(double area[]);
int getMaxTotalArea();
void mouseEvent(int evt, int x, int y, int flags, void* param);
int FRAME_WIDTH = 640;
int FRAME_HEIGHT = 360;
char colourSet[4] = { 'r','b','a','n' };
int COLOURINDEX;
IplImage* img2;

colourConfidence resultColours[3];
void main()
{
	Mat inImg; //for getting the input image
	Mat outImg;//final output i.e. the extracted coloured and the ROI
	Mat outGray, outBin;
	Mat image;
	Rect rect;
	char colour='n';
	char samples[5];
//	int sampleOccurance[4] = { 0,0,0,0 };
//	int maxSampleOccurance;
//	int maxSampleOccuranceIndex;
	String crops[5] = { "Crop 1","Crop 2" ,"Crop 3" ,"Crop 4","Crop 5" };
	VideoCapture capture;
	capture.open(1);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	while (1) {
		while (1)
		{
			capture.read(inImg);

			if (inImg.empty())
			{
				break;
			}
			rect = getRect(inImg);
			imshow("eyes", inImg);
			img2 = cvCloneImage(&(IplImage)inImg);
			char c = (char)waitKey(25);

			if (c == 27)
			{ 
				capture.release();
				return;
			}
	//		cvSetMouseCallback("eyes", mouseEvent, (void*)img2);
			if (c == 'a')
			{
				//extracting the colours from the crop img
				for (int i = 0; i < 5; i++)
				{
					capture.read(inImg);
					//bilateralFilter(inImg, inImg, 15, 80, 80,4);
					img2 = cvCloneImage(&(IplImage)inImg);
					GaussianBlur(inImg, inImg, Size(5, 5), 3);
					//blur(inImg, inImg, Size(5, 5));
					inImg.copyTo(image);

					//Getiting the rectangular region to focus on
					rect = getRect(inImg);

					//Making the ROI into Mat
					Mat crop = image(rect);
					imshow(crops[i], crop);
					cvSetMouseCallback("Crop 1", mouseEvent, (void*)img2);
					colour = getColour(crop, outImg, outGray, outBin);
					printf("For crop %d ----------------",i );
//					for (int j = 0; i < crop.rows; j++)
//					{
//						for (int k = 0; j < crop.cols; k++) 
//						{
//							Point3_<uchar>* p = crop.ptr<Point3_<uchar> >(j, k);
//							printf("[%d , %d ,%d]\n", p->x, p->y, p->z);
//						}
//					}
					printf("------------------------------------ \n");
//					samples[i] = colour;
//					printf("sample[%d] = %c \n", i, samples[i]);
					printf("sample[%d] = %c \n", i, colour);
					Sleep(1000);
				}
			//			}
				cvSetMouseCallback("Crop 1", mouseEvent, (void*)img2);
				//getting the colours for all samples
/*				for(int i=0;i<5;i++)
				{
					char temp = samples[i];
					for (int j = 0; j < 5; j++)
					{
						if (temp == samples[j])
						{
							switch (temp)
							{
								case 'r':
									sampleOccurance[0]++;
									break;
								case 'b':
									sampleOccurance[1]++;
									break;
								case 'a':
									sampleOccurance[2]++;
									break;
								case 'n':
									sampleOccurance[3]++;
									break;
							}
						}
				
					}
				}

				//getting the max no. of colour occurances and its index refering to colourSet
				for (int i = 0; i < 4; i++)
				{
					maxSampleOccurance = sampleOccurance[i];
					for (int j = 0; j < 4; j++)
					{
						if (maxSampleOccurance < sampleOccurance[j])
						{
							maxSampleOccurance = sampleOccurance[j];
							maxSampleOccuranceIndex = j;
						 }
					 }
				}*/

				//reseting The sample colour occurances
				for (int i = 0; i < 4; i++)
				{
	//				sampleOccurance[i] = 0;
					resultColours[i].totalArea = 0;
				}
				printf("\n ****************** colour = %c \n", colourSet[COLOURINDEX]);
				break;
			}
		}
	}


	waitKey(0);
}

double getArea(Mat outImg,Mat outGray,Mat outBin)
{
	cvtColor(outImg, outGray, COLOR_BGR2GRAY);
	threshold(outGray, outBin, 50, 255, CV_THRESH_BINARY);
//	imshow("crop Gray", outGray);
//	imshow("crop Bin", outBin);
	
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(outBin, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	if (hierarchy.size() > 0) 
	{
		for (int index = 0; index >= 0; index = hierarchy[index][0])
		{
			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;
			if (area > 200)
			{
			//	printf("Area = %f\n", area);
				return area;
			}
		}
	}
	else
	{
		return 0;
	}
}

Rect getRect(Mat &img)
{
//	line(img, Point(img.cols / 2 - 80, 0), Point(img.cols / 2 - 80, img.rows), Scalar(255, 255, 255), 3);
//	line(img, Point(img.cols / 2 + 80, 0), Point(img.cols / 2 + 80, img.rows), Scalar(255, 255, 255), 3);
//	line(img, Point(0, img.rows / 2 - 80), Point(img.cols, img.rows / 2 - 80), Scalar(255, 255, 255), 3);
//	line(img, Point(0, img.rows / 2 +80), Point(img.cols, img.rows / 2 + 80), Scalar(255, 255, 255), 3);
	Rect rect = Rect(img.cols / 2 - 60, img.rows / 2 - 60, 120, 120);
//	Vec3f pixel = img.at<Vec3f>(img.rows / 2, img.cols / 2);
//	Point3_<uchar>* p = img.ptr<Point3_<uchar> >(img.rows / 2, img.cols / 2);
//	printf("[%d , %d ,%d]\n", p->x, p->y, p->z);
//	printf("%f \n", img.at<Vec3f>(img.rows / 2, img.cols / 2)[0]);
	rectangle(img, rect, Scalar(255, 0, 0));
	return rect;
}

char getColour(Mat &crop,Mat &outImg,Mat &outGray,Mat &outBin)
{
	char colour;
	Mat outImgCopy;
	int maxAreaIndex;
	double area[4] = { 0 };
	String colors[3] = {"RED extract","BLUE extract","GOLD extract" };
	Mat mask;
	Scalar a[3][2] = { { Scalar(30, 28, 116) , Scalar(75, 112, 255)} ,//Scalar(30, 28, 116) , Scalar(109, 137, 255)//Scalar(17, 15, 100) , Scalar(79, 85, 255) //Red *Scalar(17, 15, 100) , Scalar(50, 56, 255)
					   { Scalar(144, 127, 45) , Scalar(255, 237, 139) },// Scalar(67, 124, 60) , Scalar(255, 237, 139) //Scalar(137, 92, 16) , Scalar(239, 216, 177)  //Blue ** Scalar(137, 92, 16) , Scalar(255, 239, 210)||*Scalar(137, 92, 16) , Scalar(252, 200, 112)|| Scalar(86, 31, 4) , Scalar(220, 88, 50); 2.Scalar(128, 97, 12) , Scalar(250, 206, 83);
					   { Scalar(0,105,124),Scalar(146,240,255) },//Gold Scalar(0,105,124),Scalar(119,238,255)//Scalar(0,0,0),Scalar(1,1,1)//Green  *Scalar(6, 80, 11) , Scalar(78, 250, 89) ||,Scalar(2, 80, 11) , Scalar(127, 247,114)
					 };//Green Scalar(93,110,110),Scalar(221,216,209)//Scalar(0,105,124),Scalar(204,255,255) //Gold *Scalar(0,105,124),Scalar(204,255,255)||Scalar(0,115,156),Scalar(161,220,242) || Scalar(0,115,156),Scalar(205,255,255) || *Scalar(0,105,124),Scalar(141,228,235)
	for (int i = 0; i < 3; i++)
	{
		inRange(crop, a[i][0], a[i][1], mask);
		bitwise_and(crop, crop, outImg, mask);
		imshow(colors[i], outImg);
//		area[i] = getArea(outImg, outGray, outBin);
		resultColours[i].totalArea+= getArea(outImg, outGray, outBin);
		outImg.release();
	//	printf("colour = %c : area = %f \n",colourSet[i],area[i]);
		printf("colour = %c : area = %f \n", colourSet[i], resultColours[i].totalArea);
	}
//	maxAreaIndex = getMaxAreaIndex(area);
	maxAreaIndex = getMaxTotalArea();
//	printf("maxarea index=%d \n", maxAreaIndex);
	switch (maxAreaIndex)
	{
		case 0:	printf("colour for this sample = RED \n");
				colour = 'r';
				break;
		case 1: printf("colour for this sample = BLUE \n");
				colour = 'b';
				break;
		case 2: printf("colour for this sample = GOLD \n");
				colour = 'a';
				break;
		default:printf("no colour detected \n");
				colour = 'n';
				break;
	}
	return colour;
}

/*int getMaxAreaIndex(double area[])
{
	double max = area[0];
//	printf("max=%f\n", max);
	int index = -1;
	for (int i = 0; i < 4; i++)
	{
	//	printf("max = %f : area[%d] = %f\n", max,i, area[i]);
		if (area[i] >= max)
		{
			max = area[i];
			index = i;
	//		printf("index assigned = %d : i = %d\n", index, i);
		}
	//	printf("index = %d : i = %d\n", index, i);
	}
	if (index != -1 && area[index] > 0)
	{
//		printf("index assigned to return = %d\n", index);
		return index;
	}
	else
	{
		return -1;
	}
}*/

int getMaxTotalArea()
{
	int colourIndex = -1;
//	double maxTotalArea[3] = { a[0].totalArea,a[1].totalArea,a[2].totalArea };
	double maxTotalArea[3] = { resultColours[0].totalArea,resultColours[1].totalArea,resultColours[2].totalArea };
	double test = 0;
	for (int i = 0; i < 3; i++)
	{
		if (test < maxTotalArea[i])
			{
				test = maxTotalArea[i];
				colourIndex = i;
			}	
	}
	COLOURINDEX = colourIndex;
	return colourIndex;
}

void mouseEvent(int evt, int x, int y, int flags, void* param)
{
	IplImage* rgb = (IplImage*)param;
	Mat RGB = cvarrToMat(rgb);
//	if (evt == CV_EVENT_LBUTTONDOWN)
	if (evt == CV_EVENT_MOUSEMOVE)
	{
		//char* data = rgb->imageData + rgb->widthStep * y + x * 3;
		//printf("Current Position: x= %d y= %d B=%d G=%d R=%d\n", x, y, data[0], data[1], data[2]);
		Point3_<uchar>* p = RGB.ptr<Point3_<uchar> >(y, x);
		printf("[%d , %d ,%d]\n", p->x, p->y, p->z);
	}
}