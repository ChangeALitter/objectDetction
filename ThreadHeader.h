//#pragma once
#include <cv.h>
#include <highgui.h>
#include <process.h>
#include <Windows.h>
#include "GMMopencv.h"

#ifndef THREADHEADER_H
#define THREADHEADER_H

//该类用来开辟多个线程、分块对图像进行处理，期望能对图像计算进行加速

class MyThread
{
public:
	MyThread();
	~MyThread();

	void DealImage(cv::Mat &img);  
	void DealImage(cv::Mat &frame, cv::Mat &mask, cv::Mat &bgmodle);
	
private:
	BgsMOG mog[4];   //对每一块建立一个用于计算的高斯混合模型，如果只用一个数据相关会引起重叠
	HANDLE hThread;
	static HANDLE g_hThreadEvent; //事件句柄用于主线程和从线程同步
	static unsigned int _stdcall SetImage(void *src); //线程处理函数
	//static CRITICAL_SECTION g_csThreadCode;
	void CopyDataToMat(cv::Mat &dst, cv::Mat &src, cv::Rect roi);
	
};

class MyThreadPra
{
	friend MyThread;
	BgsMOG *mog;
	cv::Mat frame;
	cv::Mat mask;
	cv::Mat bgmodle;
	MyThreadPra(BgsMOG *mymog, cv::Mat myframe, cv::Mat mymask, cv::Mat mybg)
		:mog(mymog), frame(myframe), mask(mymask), bgmodle(mybg){};
};
#endif