#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>

using namespace std;
using namespace cv;
Mat img;
vector<vector<int>> mycolor{ {0,0,0,179,54,54} //Black };
vector<Scalar> toshow{ {0,0,0} };
vector<vector<int>> drawpoint;

Point getcont(Mat mask)
{
	vector<vector<Point>> contour;
	vector<Vec4i> her;

	findContours(mask, contour, her, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> ContPoint(contour.size());
	vector<Rect> bound(contour.size());
	Point mypoint(0, 0);
	for (int i = 0; i < contour.size(); ++i)
	{
		int area = contourArea(contour[i]);
		if (area > 1000)
		{
			float peri = arcLength(contour[i], true);
			approxPolyDP(contour[i], ContPoint[i], 0.01 * peri, true);
			bound[i] = boundingRect(ContPoint[i]);

			mypoint.x = bound[i].x + bound[i].width / 2;
			mypoint.y = bound[i].y;

		}
	}
	return mypoint;
}
void getcolor(Mat img)
{
	Mat img2, mask;
	cvtColor(img, img2, COLOR_BGR2HSV);
	for (int i = 0; i < mycolor.size(); ++i)
	{
		Scalar lower(mycolor[i][0], mycolor[i][1], mycolor[i][2]);
		Scalar upper(mycolor[i][3], mycolor[i][4], mycolor[i][5]);
		inRange(img2, lower, upper, mask);
		Point newpoint = getcont(mask);
		if (newpoint.x != 0 && newpoint.y != 0)
		{
			drawpoint.push_back({ newpoint.x,newpoint.y,i });
		}
	}
}

void draw()
{
	for (int i = 0; i < drawpoint.size(); ++i)
	{
		circle(img, Point(drawpoint[i][0], drawpoint[i][1]), 10, toshow[drawpoint[i][2]], FILLED);
	}
}
int main()
{
	VideoCapture cap(0);
	while (true)
	{
		cap.read(img);
		getcolor(img);
		draw();
		imshow("image", img);
		waitKey(1);
	}
}