#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(){

Mat image;
Mat imageTR;
Mat q1,q2,q3,q4;





image =imread("daft.png",CV_LOAD_IMAGE_GRAYSCALE);


    int larg = image.size().width;
	 int alt= image.size().height;


    q1 = image( Rect( 0 ,  0 , larg/2, alt/2 )).clone();
	q2 = image( Rect(larg/2,  0 , larg/2, alt/2 )).clone();
	q3 = image( Rect( 0 , alt/2, larg/2, alt/2 )).clone();
	q4 = image( Rect(larg/2, alt/2, larg/2 ,alt/2 )).clone();


    image.copyTo(imageTR);
	q1.copyTo(imageTR(Rect(larg/2, alt/2, larg/2 , alt/2 )));
	q2.copyTo(imageTR(Rect( 0 , alt/2, larg/2 , alt/2 )));
	q3.copyTo(imageTR(Rect(larg/2,  0 , larg/2 , alt/2 )));
	q4.copyTo(imageTR(Rect( 0 ,  0 , larg/2 , alt/2 )));


	namedWindow("imageTR", WINDOW_AUTOSIZE);
	imshow("imageTR",imageTR);


	waitKey();
	return 0;
}












