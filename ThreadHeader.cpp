#include "ThreadHeader.h"

const int NUM_THREADS = 4;

HANDLE MyThread::g_hThreadEvent = NULL;
//CRITICAL_SECTION MyThread::g_csThreadCode;

MyThread::MyThread()
{
	g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//InitializeCriticalSection(&g_csThreadCode);
}

MyThread::~MyThread()
{
	CloseHandle(g_hThreadEvent);
	//DeleteCriticalSection(&g_csThreadCode);
}



void MyThread::DealImage(cv::Mat &frame, cv::Mat &mask, cv::Mat &bgmodle) //DealImage作为BgsMOG的友元类才能访问私有成员
{

	HANDLE handle[NUM_THREADS];
	MyThreadPra *threadPra[NUM_THREADS];

	int patch_h = frame.rows/2;
	int patch_w = frame.cols/2;
	int x = 0, y = 0 ;
	int height = 0, width = 0;
	for (int i=0; i < NUM_THREADS; i++)
	{
		switch(i)
		{
		case 0:
			x = 0;
			y= 0;
			height = patch_h;
			width = patch_w;
			break;
		case 1:
			x = patch_w;
			y = 0;
			height = patch_h;
			width = frame.cols-patch_w;
			break;
		case 2:
			x = 0;
			y = patch_h;
			height = frame.rows-patch_h;
			width = patch_w;
			break;
		case 3:
			x = patch_w;
			y = patch_h;
			height = frame.rows-patch_h;
			width = frame.cols-patch_w;
			break;
		}
		
		cv::Rect imgRoi(x,y,width,height);
		cv::Mat img(frame, imgRoi);
		cv::Mat msk(mask, imgRoi);
		cv::Mat bg(bgmodle, imgRoi);
		threadPra[i] = new MyThreadPra(&mog[i], img, msk, bg);
	}
	for (int i=0; i < NUM_THREADS; i++)
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, SetImage, threadPra[i], 0, NULL);	 
		WaitForSingleObject(g_hThreadEvent, INFINITE);
    }
	WaitForMultipleObjects(NUM_THREADS, handle, TRUE, INFINITE);
	//delete[] threadPra;
}

void MyThread::CopyDataToMat(cv::Mat &dst, cv::Mat &src, cv::Rect roi)
{
	for (int x = roi.x , i = 0; i < src.cols; x++, i++)
		for (int y = roi.y, j = 0; j < src.rows; y++, j++)
			dst.at<uchar>(y,x) = src.at<uchar>(j,i);
}

void MyThread::DealImage(cv::Mat &image)
{
	HANDLE handle[NUM_THREADS];	
	int patch_h = image.rows/2;
	int patch_w = image.cols/2;
	int x = 0, y = 0 ;
	int height = 0, width = 0;
	for (int i=0; i < NUM_THREADS; i++)
	{
		switch(i)
		{
		case 0:
			x = 0;
			y= 0;
			height = patch_h;
			width = patch_w;
			break;
		case 1:
			x = patch_w;
			y = 0;
			height = patch_h;
			width = image.cols-patch_w;
			break;
		case 2:
			x = 0;
			y = patch_h;
			height = image.rows-patch_h;
			width = patch_w;
			break;
		case 3:
			x = patch_w;
			y = patch_h;
			height = image.rows-patch_h;
			width = image.cols-patch_w;
			break;
		}
		cv::Rect imgRoi(x,y,patch_w,patch_h);
		cv::Mat img=image(imgRoi);
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, SetImage, &img, 0, NULL);//img 是否可以传递类
		WaitForSingleObject(g_hThreadEvent, INFINITE);
	}
	WaitForMultipleObjects(NUM_THREADS, handle, TRUE, INFINITE);
}

unsigned int _stdcall MyThread::SetImage(void *val)
{
	
	MyThreadPra pra = *(MyThreadPra *)val;	
	SetEvent(g_hThreadEvent);
	//EnterCriticalSection(&g_csThreadCode);
	pra.mog->operatorGMM(pra.frame, pra.mask, pra.bgmodle, 0.005);
	//LeaveCriticalSection(&g_csThreadCode);	
	return 0;
}

