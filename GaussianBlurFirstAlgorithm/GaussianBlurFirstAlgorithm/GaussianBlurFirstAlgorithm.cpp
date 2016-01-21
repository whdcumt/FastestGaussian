// GaussianBlurFirstAlgorithm.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
 #include <cmath>
#include<opencv2/opencv.hpp>
#include<windows.h>  //������������ʱ�亯��
using namespace std;
using namespace cv;  
//ǰ������ΪOpenCVͼ��������������Ҫ�Ļ�����ͷ�ļ��������ռ�
#define M_PI 3.14159265358979323846
//ģ����������
void gaussBlur_1(IplImage *scl,IplImage *tcl, int w, int h, int r); 
int _tmain(int argc, _TCHAR* argv[])
{
	Mat InputImage=imread("test.jpg");
	imshow("ԭʼͼ��",InputImage);
	//���������ǲ��Գ����Ƿ����ú�OpenCV��򵥵ķ�ʽ��
	// @����Ĳ�����������OpenCV�������ͼ�����ͨ�����������ν��д���@
	//***********************************************************************//
	// @���Ϊ��ͨ���ģ�����ͨ������֮���ÿ��ͨ���ֱ���и�˹ģ������@
	// @���Ϊ��ͨ���ģ���ֱ�ӽ���ģ������@
	//��������ΪMat����ת��ΪIplImage���ͣ���ʵIplImage���;��ǵ�ַ��Mat���;��Ǳ�������������ת����ʱ����Ҫȡ��ַ
	//֮����ת����������ΪIplImage��ͨ������ο�����Ƚ϶�
	IplImage *InputImageConvertIp;
	InputImageConvertIp=&(IplImage(InputImage));  
	//@ͨ������
	//��������ͨ����ͼ��ͷ����������Ĵ洢
	IplImage* bImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* gImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* rImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);   
	cvSplit(InputImageConvertIp,bImg,gImg,rImg,0);
	//cvShowImage("red",rImg);
	//cvShowImage("green",gImg);
	//cvShowImage("blue",bImg);
	//@ͨ������
	IplImage* rImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* gImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* bImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	DWORD start_time=GetTickCount();
	gaussBlur_1(rImg, rImgBlur,rImgBlur->width ,rImgBlur->height,5);
	gaussBlur_1(gImg, gImgBlur,gImgBlur->width ,gImgBlur->height,5);
	gaussBlur_1(bImg, bImgBlur,bImgBlur->width ,bImgBlur->height,5);
	DWORD end_time=GetTickCount();
    cout<<"The run time is:"<<(end_time-start_time)<<"ms!"<<endl;//�������ʱ��
   // cvShowImage("reddeblur",rImgBlur);

	//ͨ���ϲ�
	 IplImage* mergeImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,3);   
	 cvMerge(bImgBlur,gImgBlur,rImgBlur,0,mergeImgBlur);
	 cvShowImage("��ͨ��ģ����ͼ��",mergeImgBlur);

	cvReleaseImage(&rImg);
	cvReleaseImage(&gImg);
	cvReleaseImage(&bImg);
	cvReleaseImage(&rImgBlur);
	cvReleaseImage(&gImgBlur);
	cvReleaseImage(&bImgBlur);
    cvReleaseImage(&mergeImgBlur);
	//**************************************************************//
	waitKey();
	return 0;
}
// source channel, target channel, width, height, radius
void gaussBlur_1(IplImage *scl,IplImage *tcl, int w, int h, int r)
{    
    CvScalar s;
	int rs = ceil(r * 2.57);          // significant radius
    for(int i=0; i<h; i++)            //ͼ��߶ȣ���Ӧ��y����
       for(int j=0; j<w; j++)         //ͼ����,��Ӧ��x����
              {
                    double val = 0.0, wsum = 0;          
                    for(int iy = i-rs; iy<i+rs+1; iy++)   //y��������ɸõ�i����������rs���룬ÿ��������iy
                      for(int ix = j-rs; ix<j+rs+1; ix++) //x��������ɸõ�j����������rs���룬ÿ��������ix,�������ǵ����Ϊ��2*rs����ƽ��
                           {
                             int x = MIN(w-1, MAX(0, ix));//������ͬʱ������ͼ��ķ�Χ�ڣ�Ϊx��������
                             int y = MIN(h-1, MAX(0, iy));//y��������
                             int dsq = (ix-j)*(ix-j)+(iy-i)*(iy-i);
                             double wght = exp(1.0*(-dsq / (2*r*r))) / (M_PI*2*r*r);  //��˹Ȩ��;
                             val += cvGet2D(scl,y,x).val[0]*wght; //����ֵ������Ӧ��Ȩ��
                             wsum += wght;    //Ȩ���ۼ�
                             }
					 s.val[0]= MIN(254, MAX(0, val/wsum));
                    cvSet2D(tcl,i,j, s); //��Ȩƽ��ֵ�õ����ս��           
               }
} 
//����Ҫ��������ѵ�scl[y*w+x]�� tcl[i*w+j]����⣬˵��ͼ�񲢲��Ǵ�ͳ�Ķ�ά����Ĵ洢��ʽ������һά�Ĵ洢��ʽ��scl[y*w+x]Ӧ�ö�Ӧ�Ŷ�ά��scl(y,x),wΪͼ��Ŀ�ȣ�Ϊ�˱�����⣺����
/*y=0,x=0��   ��Ӧ0*w+0   һά����ĵ�һ��Ԫ��
y=0,x=1,    ��Ӧ0*w+1   һά����ĵڶ���Ԫ��
........................................
y=0,x=w-1   ��Ӧ0*w+w-1 һά����ĵ�w��Ԫ��
˵������ά����ת��Ϊһά���飬�ǰ��������еĴ洢���ݵġ�
ͬ��
y=1,x=0��   ��Ӧ1*w+0   һά����ĵ�w+1��Ԫ��
y=1,x=1,    ��Ӧ1*w+1   һά����ĵ�w+2��Ԫ��
........................................
y=1,x=w-1   ��Ӧ1*w+w-1 һά����ĵ�2w��Ԫ��*/
                      
