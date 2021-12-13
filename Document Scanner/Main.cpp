#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>
#include<opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

Mat img,gimg,bimg,cimg,dimg,fimg;

vector<Point> biggestarea,finalpoint;

float h = 500, w = 500;

void fcont()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hei;
	int MAXAREA = -1;
	findContours(dimg, contours, hei, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> ConPoly(contours.size());
	for (int i = 0; i < contours.size(); ++i)
	{
		int area = contourArea(contours[i]);
		if (area > 200)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], ConPoly[i], 0.02 * peri, true);
			cout << area << endl;
			if (area > MAXAREA && ConPoly[i].size() == 4)
			{
				biggestarea = { ConPoly[i][0], ConPoly[i][1], ConPoly[i][2], ConPoly[i][3] };
				MAXAREA = area;
			}
		}
	}
}

void filter()
{
	cvtColor(img, gimg, COLOR_BGR2GRAY);
	GaussianBlur(gimg, bimg, Size(3, 3), 5);
	Canny(bimg, cimg, 25, 75);
	Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(cimg, dimg, Kernel);
}

void reorder()
{
	vector<int> sum, diff;
	for (int i = 0; i < 4; ++i)
	{
		sum.push_back(biggestarea[i].x + biggestarea[i].y);
		diff.push_back(biggestarea[i].x - biggestarea[i].y);
	}
	finalpoint.push_back(Point(biggestarea[(min_element(sum.begin(), sum.end()) - sum.begin())]));
	finalpoint.push_back(Point(biggestarea[max_element(diff.begin(), diff.end()) - diff.begin()]));
	finalpoint.push_back(Point(biggestarea[min_element(diff.begin(), diff.end()) - diff.begin()]));
	finalpoint.push_back(Point(biggestarea[(max_element(sum.begin(), sum.end()) - sum.begin())]));
}

void getper()
{
	Point2f src[4] = { finalpoint[0],finalpoint[1],finalpoint[2],finalpoint[3] };
	Point2f des[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };
	Mat matrix=getPerspectiveTransform(src, des);
	warpPerspective(img, fimg, matrix, Point(w, h));
}
void fperi()
{
	for (int i = 0; i < 4; ++i)
	{
		circle(img, Point(finalpoint[i]), 5, Scalar(193, 221, 111), FILLED);
	}
}
int main()
{
	String path = "Resources/paper.jpg";
	img=imread(path);
	resize(img, img, Size(), 0.5, 0.5);
	filter();
	fcont();
	reorder();
	getper();
	fperi();
	Rect roi(5,5,h-10,w-10);
	Mat nimg = fimg(roi);
	imshow("nimg", nimg);
	imshow("img", img);
	waitKey(0);
}