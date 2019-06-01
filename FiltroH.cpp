#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

Mat imaginaryInput, complexImage, multsp;
Mat padded, filter;
Mat image, tmp;
Mat_<float> realInput, zeros;
vector<Mat> planos;

int dft_M, dft_N;

double gama_l=0.5, gama_h=1, c=0.2, d0=0.2;

int c_slider = 1;
int d_zero_slider = 1;
int gama_l_slider = 1;
int gama_h_slider = 2;


void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;


  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;


  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

void on_trackbar_homomorfico(int, void*){
  if(c_slider == 0){
    c_slider=1;
  }
  if(d_zero_slider == 0){
    d_zero_slider=1;
  }
  if(gama_l_slider == 0){
    gama_l_slider=1;
  }
  if(gama_h_slider == 0){
    gama_h_slider=1;
  }
  if(gama_l_slider >= gama_h_slider){
    gama_l_slider = gama_h_slider-1;
  }
  if(gama_h_slider <= gama_l_slider){
    gama_h_slider = gama_l_slider+1;
  }

  c = (double) c_slider/100 ;
  d0 = (double) d_zero_slider/4 ;
  gama_l = (double) gama_l_slider/100 ;
  gama_h = (double) gama_h_slider/100 ;


  planos.clear();

  realInput = Mat_<float>(padded);

  planos.push_back(realInput);
  planos.push_back(zeros);


  merge(planos, complexImage);


  dft(complexImage, complexImage);

  deslocaDFT(complexImage);


  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
        tmp.at<float> (i,j) = (gama_h-gama_l)*(1.0 - exp(-c* ((i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2))/(d0*d0) )) + gama_l;
    }
  }


  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);


  mulSpectrums(complexImage,filter,complexImage,0);


  deslocaDFT(complexImage);

  idft(complexImage, complexImage);

  planos.clear();

  split(complexImage, planos);

  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

  imshow("Filtrada", planos[0]);
}

int main(int argc, char** argv){

  char key;

  namedWindow("Original", WINDOW_NORMAL);
  namedWindow("Filtrada", WINDOW_NORMAL);


  image = imread("daft.png", CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data){
                cout << "A imagem nao pode ser aberta" << endl;
        }


  imshow("Original", image);


  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);


  copyMakeBorder(image, padded, 0,
    dft_M - image.rows, 0,
    dft_N - image.cols,
    BORDER_CONSTANT, Scalar::all(0));


  zeros = Mat_<float>::zeros(padded.size());


  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));


  filter = complexImage.clone();


  tmp = Mat(dft_M, dft_N, CV_32F);

  createTrackbar( "Gama H", "Filtrada",
  &gama_h_slider, 100, on_trackbar_homomorfico);

  createTrackbar( "Gama L", "Filtrada",
  &gama_l_slider, 100, on_trackbar_homomorfico);

  createTrackbar( "D0", "Filtrada",
  &d_zero_slider, 100, on_trackbar_homomorfico);

  createTrackbar( "C", "Filtrada",
  &c_slider, 100, on_trackbar_homomorfico);

  on_trackbar_homomorfico(100, NULL);

  while(1){

    key = (char) waitKey(10);
    if( key == 27 ) break;
  }

  return 0;
}
