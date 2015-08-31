//#include <process.h>
//#include <Windows.h>
#include <cv.h>
#include <highgui.h>
#include "ThreadHeader.h"
#include "GMMopencv.h"
#include "LoadImage.h"
#include "BackgroundImg.h"
#include "ObjectArea.h"
#include "StaticRegion.h"

using namespace cv;

#define DEBUG
#define StaticFrameThr 80

int main()
{
	//Mat image = imread("E:\\video\\PTES\\S1-T1-C\\video\\pets2006\\S1-T1-C\\4\\S1-T1-C.00001.jpeg");
	
	MyThread test;
	BackgroundImg bgImg;
	ObjectArea objectArea;
	//StaticRegion targetRegion;
	//list<DetectObject> staticTarget;

	Mat img, gray, mask, background;
	int nframe = 0;
	int OneFrameProcess = 0;
	int key;//控制视频播放
	MyLoadImage imagefile("E:\\video\\PTES\\S1-T1-C\\video\\pets2006\\S1-T1-C\\1\\S1-T1-C.");
	
#ifdef DEBUG
	clock_t start, finish;
	double sum=0.0;
#endif

	while (1)
	{

#ifdef DEBUG
		start = clock();
#endif
		nframe++;
		cout << nframe << endl;
		IplImage *frame = imagefile.GetFrameFromImg(nframe);
		img = cv::Mat(frame, true);
	    cvtColor(img, gray, CV_RGB2GRAY);
		if (nframe == 1)
		{
			mask.create(gray.size(), CV_8U);
			background.create(gray.size(), CV_8U);
			bgImg.Init(frame);  //初始化背景
		}
		mask.setTo(cv::Scalar(0));		
		background.setTo(cv::Scalar(0));
		test.DealImage(gray, mask, background);
		//objectArea.GetObjectArea(mask);
		//cv::Mat temp = objectArea.ReturnObjectArea();

		//把Mat类型转换成CvMat类型		
		 IplImage BinaryImage = mask.operator IplImage();
		 IplImage BackImage = background.operator IplImage();
		 IplImage grayImage = gray.operator IplImage();

		 objectArea.LocateObject(&BinaryImage, nframe);
		 //objectArea.ChoseRightObject(frame, bgImg);
		 objectArea.ShowObject(frame);
		 //检测到异常目标
		 //targetRegion.FindRawContourAreas(&BinaryImage, &grayImage, nframe);
		 //targetRegion.AttachTypeTag(&BackImage, &grayImage);
		// targetRegion.ExtractGoodAreas(staticTarget, &grayImage, StaticFrameThr, nframe);
		// targetRegion.ShowStaticAreas(frame, staticTarget);
		cvShowImage("src", frame);
		imshow("mask", mask);
		imshow("bg", background);
		if( ( key = cvWaitKey(OneFrameProcess ? 0 : 1)) == 27 )	
			break;
		if(key != -1)
		{
			OneFrameProcess = 1;
			if(key == 'r') OneFrameProcess = 0;
		}
		imagefile.ReleaseFrame();
#ifdef DEBUG
		finish = clock();
		double cur= (double)(finish-start)/CLOCKS_PER_SEC;
		sum += cur;
		cout << "avg:" << sum/nframe << "cur:" << cur << endl;  //s/frame:0.030(无处理）  0.078(无分块处理） 0.049（分块处理）
#endif
	}
	return 0;
}