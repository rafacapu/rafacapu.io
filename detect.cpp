#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>




using namespace cv;
using namespace std;

int main( ){
	Mat image;
	int width, height;

	vector<Mat> planes;
	int nbins = 64;
	float range[] = {0,256};
	const float *histange = { range };
	bool uniform = true, acummulate = false;
	double compare;

	Mat hist, histA;

	 VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "nenhuma câmera detectada" << endl;
        return -1;
    }

    while(true){
        cap >> image;
        if(image.empty()){
            cerr << "frame inválido" << endl;
            continue;
        }

  	}

  	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  	cout << "Largura = " << width << endl;
  	cout << "Altura = " << height << endl;

  	while(1){

  	 VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "nenhuma câmera detectada" << endl;
        return -1;
    }

    while(true){
        cap >> image;
        if(image.empty()){
            cerr << "frame inválido" << endl;
            continue;
        }

  	}




  		cap >> image;
   		split (image,planes);

  		calcHist(&planes[0], 1, 0, Mat(), hist, 1, &nbins, &histange, uniform, acummulate);
    	normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());

    	if(!histA.data){ hist.copyTo(histA); continue;}

    	compare = compareHist(hist, histA, CV_COMP_CORREL);


    	if (compare < 0.99){

    		hist.copyTo(histA);
    		  putText(image, "movimento detectado !", Point(width/2 - 200, height/2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 2);
    	}

    	imshow("image", image);

    	if(waitKey(3000) >= 0) break;

	}
	return 0;
}
