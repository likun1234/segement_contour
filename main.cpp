#include <iostream>  
#include <opencv2/opencv.hpp>
#include<time.h>      
using namespace std;   
using namespace cv;  

      
int main()
{

	Mat imageSource=imread("1_2.png",0); 
	if(imageSource.empty()){
		printf("Can not load image\n");
		return -1;
	}
	clock_t start, finish; 
	start = clock(); 
	resize(imageSource,imageSource,Size(imageSource.cols/2,imageSource.rows/2),0,0,INTER_LINEAR);  
	imshow("Source Image",imageSource); 

	Mat image;  
	GaussianBlur(imageSource,image,Size(5,5),0);  
	double low_thresh = 100.0;  
	double high_thresh = 250.0;
	Canny(image, image, low_thresh, high_thresh, 3); 

	vector<vector<Point> > contours;  
	vector<Vec4i> hierarchy;  
	findContours(image,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());  

	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);  
	Mat Contours=Mat::zeros(image.size(),CV_8UC1); 

	for(unsigned int i=0;i<contours.size();i++) {  
		for(unsigned int j=0;j<contours[i].size();j++) {  
			Point P=Point(contours[i][j].x,contours[i][j].y);
			Contours.at<uchar>(P)=255; 
		} 
		drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);
	}
	vector<Point2f> vp2f; 
	int count = 0; 
	for(unsigned int i=0; i<contours.size(); i++) {
		if(contours[i].size()>500) {  
			RotatedRect rectPoint = minAreaRect(contours[i]);
			Point2f fourPoint2f[4];

			rectPoint.points(fourPoint2f); 

			float width = rectPoint.size.width;
			float height = rectPoint.size.height;
			float area = rectPoint.size.area();
			float ratio;
			if(width>height){
				ratio = (float)height / (float)width;
			} else {
				ratio = (float)width / (float)height;
			}

			if(ratio >= 0.5 || ratio <= 0.25){  
				continue;
			}else{
				for( int i=0; i<3; i++){
					line(imageSource, fourPoint2f[i], fourPoint2f[i+1], Scalar(255,255,255,3));
				}
				line(imageSource, fourPoint2f[0], fourPoint2f[3], Scalar(255,255,255,3));
				count++;
				for(int i=0; i<4; i++){
					vp2f.push_back(fourPoint2f[i]);
				}
			}
		}
	}
	finish = clock(); 
	double duration = (double)(finish - start) / CLOCKS_PER_SEC; 

	cout<<"Run time is:"<<duration<<endl;
	for(unsigned int i=0; i<vp2f.size(); i++){
		cout<<i<<' '<<vp2f[i].x<<' '<<vp2f[i].y<<endl;
	}


	imshow("Contours Image",imageContours);  
	imshow("result",imageSource);  
	waitKey(0); 
	return 0;  
}  

