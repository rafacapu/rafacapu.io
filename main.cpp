#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(){

Mat image;
Vec3b val;
int x1,x2,y1,y2;




image = imread("daft.png",CV_LOAD_IMAGE_GRAYSCALE);



namedWindow("janela",WINDOW_AUTOSIZE);	// Nomeando e identificando janelas

    cout << "por favor digite as primeiras cordenadas" << endl;
    cin >> x1 >> y1;
    cout << endl;
    cout << "por favor digite as segundas coordenadas" << endl;
    cin >> x2 >> y2;
    cout << endl;

  for(int i=x1;i<x2;i++){
    for(int j=y1;j<y2;j++){
      image.at<uchar>(i,j)=255-image.at<uchar>(i,j);			// Acessando elementos unsigned.. gravando numero 0
    }
  }

  imshow("janela", image);  			// Abrindo janela para imagem
  imwrite("regions.png", image);
  waitKey();
  return 0;
}











