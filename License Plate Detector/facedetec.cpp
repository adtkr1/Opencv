#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	VideoCapture cap(0);
	Mat img;
	CascadeClassifier platedet;
	platedet.load("Resources/haarcascade_russian_plate_number.xml");
	/*if (facedet.empty() == 0) { cout << "no" << endl; }
	else { cout <<  "yes"; }*/
	vector<Rect> plate;
	while (true)
	{
		cap.read(img);
		platedet.detectMultiScale(img, plate, 1.3, 10);
		for (int i = 0; i < plate.size(); ++i)
		{
			Mat plateimg=img(plate[i]);
			imwrite("Licence Plate Detector/Plates/" + to_string(i) + ".png", plateimg);
			rectangle(img, plate[i].tl(), plate[i].br(), Scalar(23, 123, 12), 3);
		}
		imshow("images", img);
		waitKey(1);
	}
	return 0;
}