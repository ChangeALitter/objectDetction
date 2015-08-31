//#pragma once
#include <cv.h>
#include <highgui.h>
#include <process.h>
#include <Windows.h>
#include "GMMopencv.h"

#ifndef THREADHEADER_H
#define THREADHEADER_H

//�����������ٶ���̡߳��ֿ��ͼ����д��������ܶ�ͼ�������м���

class MyThread
{
public:
	MyThread();
	~MyThread();

	void DealImage(cv::Mat &img);  
	void DealImage(cv::Mat &frame, cv::Mat &mask, cv::Mat &bgmodle);
	
private:
	BgsMOG mog[4];   //��ÿһ�齨��һ�����ڼ���ĸ�˹���ģ�ͣ����ֻ��һ��������ػ������ص�
	HANDLE hThread;
	static HANDLE g_hThreadEvent; //�¼�����������̺߳ʹ��߳�ͬ��
	static unsigned int _stdcall SetImage(void *src); //�̴߳�����
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