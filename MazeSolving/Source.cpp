
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
//#include <Bits.h>
//#include <stdc++.h>
#include <cstring>
#include <atlstr.h>
//#include <sstream>
#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
//#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

static int map[5000][5000];
static int closed_nodes_map[5000][5000]; 
static int open_nodes_map[5000][5000]; 
static int dir_map[5000][5000]; 

const int dir = 8;
static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int x_coordinate[2], y_coordinate[2], c = 0;

bool WriteComPort(CString PortSpecifier, CString data)
{
	DCB dcb;
	DWORD byteswritten;
	//printf("insidewrite");
	HANDLE hPort = CreateFile(

		PortSpecifier,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (!GetCommState(hPort, &dcb))
		return false;
	dcb.BaudRate = CBR_115200; //9600 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop
	if (!SetCommState(hPort, &dcb))
		return false;
	bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
	CloseHandle(hPort); //close the handle
	return retVal;
}

void callbackfunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		x_coordinate[c] = y;
		y_coordinate[c] = x;
		c++;
		//	cout << "\n" << xs << " " << ys << "\n";
	}
	//	count++;
	//
}



class node
{
	int xPos,yPos,level,priority;  

	public:
	node(int xp, int yp, int d, int p)
	{
		xPos = xp; yPos = yp; level = d; priority = p;
	}
	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }

	void updatePriority(const int & xDest, const int & yDest)
	{
		priority = level + estimate(xDest, yDest) * 10;
	}

	void nextLevel(const int & i) // i: direction
	{
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	// Fn to calculate remaining distance to endpoint
	const int & estimate(const int & xDest, const int & yDest) const
	{
		static int xd, yd, d;
		xd = xDest - xPos;
		yd = yDest - yPos;
		d = static_cast<int>(sqrt(xd*xd + yd*yd));
		return(d);
	}
};

bool operator<(const node & a, const node & b)
{
	return a.getPriority() > b.getPriority();
}

// A* algo------------------------------------------------------------------------------------------
//returns string of direction digits
string pathFind(const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish, int n, int m)
{
	static priority_queue<node> pq[2]; 
	static int pqi; 
	static node* n0;
	static node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	for (y = 0; y<m; y++)
	{
		for (x = 0; x<n; x++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); 

	while (!pq[pqi].empty())
	{
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); 
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		if (x == xFinish && y == yFinish)
		{
			// generates path from finish to start by following the directions
			string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			delete n0;
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || map[xdx][ydy] == 0
				|| closed_nodes_map[xdx][ydy] == 1))
			{
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority())
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop();

					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; 
			}
		}
		delete n0; 
	}
	return ""; // no route found
}

//---------------------------------------------------------------------------------------------------




int main(int argc, char** argv)
{
	//	srand(time(NULL));																	1
	Mat img, cpy,skel;
	img = imread(argv[1], 1);
	//	cv::Size newsize = cv::Size(img.cols / 2, img.rows / 2);
	//	resize(img, img, newsize);
	img.copyTo(skel);
	cvtColor(skel, skel, COLOR_BGR2GRAY);
	namedWindow("Original image", WINDOW_NORMAL);
	imshow("Original image", img);
//	GaussianBlur(cpy, cpy, Size(7, 7), 0);									    //Actual image
	threshold(skel, skel, 150, 255, CV_THRESH_BINARY_INV);
	namedWindow("Skeleton", WINDOW_FREERATIO);
	Mat element = cv::getStructuringElement(cv::MORPH_RECT, Size(5, 5));		//Real image(5,5)

	erode(skel, skel, element, Point(0, 0), 4);									//Real image

//	imshow("Binary inverse image", cpy);

/*	Mat element = cv::getStructuringElement(cv::MORPH_RECT, Size(7, 7));		//Actual image(7,7)
	erode(cpy, cpy, element, Point(0, 0), 2);									//Acual image

	bool done;
	Mat eroded;
	Mat skel(cpy.size(), CV_8UC1, cv::Scalar(0));
	Mat temp(cpy.size(), CV_8UC1);
	do
	{
		erode(cpy, eroded, element);
		dilate(eroded, temp, element);
		subtract(cpy, temp, temp);
		bitwise_or(skel, temp, skel);
		eroded.copyTo(cpy);
		done = (cv::countNonZero(cpy) == 0);
	} while (!done);
	//erode(cpy, eroded,element,Point(-1,-1),7);
	Mat element1 = cv::getStructuringElement(cv::MORPH_RECT, Size(3, 3));		//Actual Image
	dilate(skel, skel, element1);												//Actual Image
	erode(skel, skel, element1);												//Actual image
	namedWindow("Skeleton", WINDOW_KEEPRATIO);
*/	
	Mat Path;
	skel.copyTo(Path);


	int row = skel.rows;
	int col = skel.cols;
	for (int i = 0; i < skel.rows; i++)
		for (int j = 0; j < skel.cols; j++)
		{
			map[i][j] = skel.at<uchar>(i, j);
		}



	int xA, yA, xB, yB;

	setMouseCallback("Skeleton", callbackfunc, NULL);

	imshow("Skeleton", skel);
	imwrite("../data/skeleton.jpg", skel);
	waitKey(0);
	xA = x_coordinate[0];
	yA = y_coordinate[0];
	xB = x_coordinate[1];
	yB = y_coordinate[1];

	//	cin >> xA >> yA >> xB >> yB;

	cout << "Map Size (X,Y): " << skel.rows << "," << skel.cols << endl;
	cout << "Start: " << xA << "," << yA << endl;
	cout << "Finish: " << xB << "," << yB << endl;
	// get the route
	//	clock_t start = clock();															2

	string route = pathFind(xA, yA, xB, yB, row, col);
	if (route == "") cout << "An empty route generated!" << endl;
	//	clock_t end = clock();																3
	//	double time_elapsed = double(end - start);											4
	//	cout << "Time to calculate the route (ms): " << time_elapsed << endl;				5
	cout << "Route:" << endl;
	cout << route << endl << endl;

	//  Converting route into direction array---------------------------------------------------
	char path[2000];
	int p = 0;
	int pos = 0;
	if (route.at(p) % 2 == 0)
		path[p] = route.at(p);
	else
		path[p] = route.at(p + 1);
	p++;
	char directions[2000];
	/*	for (int i = 0; i < row; i++)
	for (int j = 0; j < col; j++)
	if(map[i][j] ==255)
	cout<<i<<" "<<j<<"\n";
	*/
	// follow the route on the map and display it 
	if (route.length() > 0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		map[x][y] = 2;
		for (int i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
			//*******************************************************
			if (i >= 1)
			{
				if ((route.at(i) != route.at(i - 1)) && route.at(i) % 2 == 0)
				{
					path[p] = route.at(i);

					//p++;
					if (path[p - 1] == '6')
					{
						if (path[p] == '0')
						{
							directions[pos] = 'L';
							p++;
							pos++;
						}
						else if (path[p] == '4')
						{
							directions[pos] = 'R';
							p++;
							pos++;
						}
						//	else if(path[p]=='6')
					}
					else if (path[p - 1] == '0')
					{
						if (path[p] == '6')
						{
							directions[pos] = 'R';
							p++;
							pos++;
						}
						else if (path[p] == '2')
						{
							directions[pos] = 'L';
							p++;
							pos++;
						}
					}
					else if (path[p - 1] == '2')
					{
						if (path[p] == '4')
						{
							directions[pos] = 'L';
							p++;
							pos++;
						}
						else if (path[p] == '0')
						{
							directions[pos] = 'R';
							p++;
							pos++;
						}
					}
					else if (path[p - 1] == '4')
					{
						if (path[p] == '6')
						{
							directions[pos] = 'L';
							p++;
							pos++;
						}
						else if (path[p] == '2')
						{
							directions[pos] = 'R';
							p++;
							pos++;
						}
					}
				}
				//Accessing array as arr[y][x]
				else if (route.at(i) == route.at(i - 1) && route.at(i) % 2 == 0)
				{
					if (route.at(i) == '6' || route.at(i) == '2')
					{
						if ((map[x + 1][y] == 255 && map[x + 4][y] == 255 && map[x + 7][y] == 255 && map[x + 10][y] == 255) || (map[x - 1][y] == 255 && map[x - 4][y] == 255 && map[x - 7][y] == 255 && map[x - 10][y] == 255))
						{
							directions[pos] = 'S';
							pos++;
						}
						else if (directions[pos - 1] == '0')
						{
						}
						else
						{
							directions[pos] = '0';
							pos++;
						}
					}
					else if (route.at(i) == '0' || route.at(i) == '4')
					{
						if ((map[x][y + 1] == 255 && map[x][y + 4] == 255 && map[x][y + 7] == 255 && map[x][y + 10] == 255) || (map[x][y - 1] == 255 && map[x][y - 4] == 255 && map[x][y - 7] == 255 && map[x][y - 10] == 255))
						{
							directions[pos] = 'S';
							pos++;
						}
						else if (directions[pos - 1] == '0')
						{
						}
						else
						{
							directions[pos] = '0';
							pos++;
						}
					}
				}
			}
			//*******************************************************
		}
		map[x][y] = 4;
	}

	// display the map with the route
	for (int y = 0; y<col; y++)
	{
		for (int x = 0; x < row; x++)
		{
			if (!(map[x][y] == 2 || map[x][y] == 3 || map[x][y] == 4))
			{
				//Path.at<uchar>(x, y) = 0;
				if (skel.at<uchar>(x, y) == 255)
					skel.at<uchar>(x, y) = 70;
			}
			else
			{
				//cout << "(" << x << "," << y << ")\n";
				Path.at<uchar>(x, y) = 255;
				//skel.at<uchar>(x, y) = 127;
			}

		}
		//			cout << endl;
	}
	cout << "\n\n\n";


	for (int t = 0; t < pos; t++)
		cout << directions[t] << " ";
	cout << "\n";

	char finalpath[100];
	int k = 0;

	for (int t = 0; t < pos; t++)
	{
		if (directions[t] == '0')
		{
		}
		else if (directions[t] == 'L' || directions[t] == 'R')
		{
			finalpath[k] = directions[t];
			k++;
		}
		else if (directions[t] == 'S')
		{
			if (directions[t - 1] != 'S')
			{
				finalpath[k] = 'S';
				k++;
			}
		}
	}
	cout << "\n\n\n";

	for (int i = 0; i < k; i++)
	{
		cout << finalpath[i] << " ";
		WriteComPort("COM4", finalpath[i]);
	}

	namedWindow("Path--", WINDOW_FREERATIO);
	imshow("Path--", skel);

	//	namedWindow("Original image with Path", CV_WINDOW_FREERATIO);
	//	imshow("Original image with Path", img);

	waitKey(0);
	return(0);
}