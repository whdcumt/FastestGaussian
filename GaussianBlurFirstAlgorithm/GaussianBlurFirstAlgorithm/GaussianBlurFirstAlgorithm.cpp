// GaussianBlurFirstAlgorithm.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
 #include <cmath>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;  
//前面三行为OpenCV图像输入和输出所需要的基本的头文件和命名空间
#define M_PI 3.14159265358979323846
//模糊函数声明
void gaussBlur_1(IplImage *scl,IplImage *tcl, int w, int h, int r); 
int _tmain(int argc, _TCHAR* argv[])
{
	Mat InputImage=imread("test.jpg");
	imshow("原始图像",InputImage);
	//以上两句是测试程序是否配置好OpenCV最简单的方式。
	// @下面的操作就是利用OpenCV将输入的图像根据通道的数量依次进行处理，@
	//***********************************************************************//
	// @如果为三通道的，进行通道分离之后对每个通道分别进行高斯模糊处理，@
	// @如果为单通道的，则直接进行模糊处理。@
	//下面两句为Mat类型转换为IplImage类型，其实IplImage类型就是地址，Mat类型就是变量，所以类型转换的时候需要取地址
	//之所以转换类型是因为IplImage的通道分离参考程序比较多
	IplImage *InputImageConvertIp;
	InputImageConvertIp=&(IplImage(InputImage));  
	//@通道分离
	//定义三个通道的图像头，方便分离后的存储
	IplImage* bImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* gImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* rImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);   
	cvSplit(InputImageConvertIp,bImg,gImg,rImg,0);
	cvShowImage("red",rImg);
	cvShowImage("green",gImg);
	cvShowImage("blue",bImg);
	//@通道分离
	IplImage* rImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	gaussBlur_1(rImg, rImgBlur,rImgBlur->width ,rImgBlur->height,2 );
    cvShowImage("reddeblur",rImgBlur);


	
	cvReleaseImage(&rImg);
	cvReleaseImage(&gImg);
	cvReleaseImage(&bImg);
	cvReleaseImage(&rImgBlur);
	
	//**************************************************************//
	waitKey();
	return 0;
}
// source channel, target channel, width, height, radius
void gaussBlur_1(IplImage *scl,IplImage *tcl, int w, int h, int r)
{    
    CvScalar s;
	int rs = ceil(r * 2.57);          // significant radius
    for(int i=0; i<h; i++)            //图像高度，对应着y坐标
       for(int j=0; j<w; j++)         //图像宽度,对应着x坐标
              {
                    double val = 0.0, wsum = 0;          
                    for(int iy = i-rs; iy<i+rs+1; iy++)   //y点的坐标由该点i向上下延伸rs距离，每次延伸至iy
                      for(int ix = j-rs; ix<j+rs+1; ix++) //x点的坐标由该点j向左右延伸rs距离，每次延伸至ix,这样覆盖的面积为（2*rs）的平方
                           {
                             int x = MIN(w-1, MAX(0, ix));//覆盖面同时限制在图像的范围内，为x坐标限制
                             int y = MIN(h-1, MAX(0, iy));//y坐标限制
                             int dsq = (ix-j)*(ix-j)+(iy-i)*(iy-i);
                             double wght = exp(1.0*(-dsq / (2*r*r))) / (M_PI*2*r*r);  //高斯权重;
                             val += cvGet2D(scl,y,x).val[0]*wght; //像素值乘以相应的权重
                             wsum += wght;    //权重累加
                             }
					 s.val[0]=val/wsum;
					 s.val[0]= MIN(254, MAX(1, s.val[0]));
                    cvSet2D(tcl,i,j, s); //加权平均值得到最终结果           
               }
} 
//最重要的理解点和难点scl[y*w+x]和 tcl[i*w+j]的理解，说明图像并不是传统的二维矩阵的存储方式，而是一维的存储方式，scl[y*w+x]应该对应着二维的scl(y,x),w为图像的宽度，为了便于理解：假设
/*y=0,x=0，   对应0*w+0   一维数组的第一个元素
y=0,x=1,    对应0*w+1   一维数组的第二个元素
........................................
y=0,x=w-1   对应0*w+w-1 一维数组的第w个元素
说明将二维数组转换为一维数组，是按照是逐行的存储数据的。
同理：
y=1,x=0，   对应1*w+0   一维数组的第w+1个元素
y=1,x=1,    对应1*w+1   一维数组的第w+2个元素
........................................
y=1,x=w-1   对应1*w+w-1 一维数组的第2w个元素*/
                      
