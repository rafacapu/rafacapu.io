#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>



using namespace std;
using namespace cv;
void detecta(Mat nova, Mat velha, int diferenca){
  for(int i=0;i<velha.rows;i++){
    for(int j=0;j<velha.cols;j++){
      if(abs(nova.at<uchar>(i,j) - velha.at<uchar>(i,j) > diferenca)){
        cout << "Movimento detectado no pixel (" << i << ", " << j << ")" << endl;

      }
    }
  }
}
int main()
{  Mat image,imggrayscale, imgequalize;

  int width, height;


  vector<Mat> planes;
  Mat hist, histnew;
  int nbins = 256;// número de "bins" no histograma, ou o número de colunas
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
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


        imshow("normal",image);
        waitKey(5);


        width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);// função padrão do openCV,declara que a altura é a altura captada pela função durante o processo
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_8UC1, Scalar(0,0,0));

  Mat histImgNova(histh, histw, CV_8UC1, Scalar(0,0,0));


  while(1){



    cvtColor(image, imggrayscale, CV_BGR2GRAY);
    equalizeHist(imggrayscale,imgequalize);

    calcHist(&imggrayscale, 1, 0, Mat(), hist, 1,
             &nbins, &histrange,
             uniform, acummulate);

    calcHist(&imgequalize, 1, 0, Mat(), histnew, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(histnew, histnew, 0, histImgNova.rows, NORM_MINMAX, -1, Mat());

    histImg.setTo(Scalar(0));
    histImgNova.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImg,
           Point(i, histh),
           Point(i, histh-cvRound(hist.at<float>(i))),
           Scalar(120, 120, 255), 1, 8, 0);
      line(histImgNova,
           Point(i, histh),
           Point(i, histh-cvRound(histnew.at<float>(i))),
           Scalar(120, 255, 255), 1, 8, 0);
    }
    histImg.copyTo(imgequalize(Rect(0, 0       ,nbins, histh)));
    histImgNova.copyTo(imggrayscale(Rect(0, 0       ,nbins, histh)));



    imshow("normal em cinza", imggrayscale);

    imshow("equalizada", imgequalize);

    if(waitKey(30) >= 0) break;

  }
   }

  return 0;

}


