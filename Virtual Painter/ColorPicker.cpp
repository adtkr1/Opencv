#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
int hmin=0, smin=0, vmin=0, hmax=179, smax=255, vmax=255;
int main()
{
	VideoCapture cap(0);
	Mat img1, img2, img;
	namedWindow("Taskbar", (700, 700));
	createTrackbar("Hmin", "Taskbar", &hmin, 179);
	createTrackbar("Hmax", "Taskbar", &hmax, 179);
	createTrackbar("smin", "Taskbar", &smin, 255);
	createTrackbar("smax", "Taskbar", &smax, 255);
	createTrackbar("vmin", "Taskbar", &vmin, 255);
	createTrackbar("vmax", "Taskbar", &vmax, 255);	
	while (true)
	{
		cap.read(img);
		cvtColor(img, img1, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(img1, lower, upper, img2);
		cout << hmin << smin << vmin << hmax << smax << vmax << endl;
		imshow("image1", img2);
		imshow("image", img);
		waitKey(1);
	}
	return 0;
}