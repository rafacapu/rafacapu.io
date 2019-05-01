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
      if(abs(nova.at<uchar>(i,j) - velha.at<uchar>(i,j) > diferenca)){ //Aprimorar a contagem
        cout << "Movimento detectado no pixel (" << i << ", " << j << ")" << endl;

      }
    }
  }
}

int main(int argc, char** argv){
  Mat image;
  int width, height;

  vector<Mat> planes;
  Mat histR, histRnovo;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
float dif;
Mat nova,velha;
 VideoCapture cap(0);
    if(!cap.isOpened()){
        cerr << "nenhuma câmera detectada" << endl;
        return -1;
    }

    while(true){
        cap >> image;
        if(image.empty()){
            cerr << "Frame invalid and skipped!" << endl;
            continue;
        }
        imshow("normal",image);
        waitKey(5);
width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "Largura = " << width << endl;
  cout << "Altura  = " << height << endl;

  cout << "Digite um limite para o detector de movimento: ";
  cin >> dif;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;

    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh),
           Point(i, cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh),
           Point(i, cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));


    imshow("image", image);

    cvtColor(image, nova, CV_BGR2GRAY); //Converte a imagem para somente tons de cinza

    detecta(nova, velha, dif);
    nova.copyTo(velha);


    if(waitKey(30) >= 0) break;
  }
  return 0;
}
}
