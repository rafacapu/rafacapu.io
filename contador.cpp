#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;




using namespace cv;
using namespace std;

int main(){
	Mat image, ImagemLimpa;
	int larg, alt;
	int nob, nobM, noobjectsB, nobSemBuracos, noburacos;

	CvPoint p;
	image = imread("bolha.png",CV_LOAD_IMAGE_GRAYSCALE);


	larg = image.size().width; // funções para conseguir a altura e largura da imagem
	alt = image.size().height;

	p.x = 0;
	p.y = 0; // iniciando no pixel 0;




	image.copyTo(ImagemLimpa); // esta função remove as bolhas incompreensíveis para o programa, utiliza o flood fill e o ponto CV,para varrer cada coluna e cada linha da borda

	for (int i = 0; i<alt; i++ ){
		if (image.at<uchar>(i,0) == 255 ){
			p.x = 0;
			p.y = i;
			floodFill(ImagemLimpa,p,0);
		}else if(image.at<uchar>(i,larg-1) == 255 ){
			p.x = larg-1;
			p.y = i;
			floodFill(ImagemLimpa,p,0);  // para cada pixel branco encontrado nas colunas, utiliza o floodfill para pintar de preto
		}
	}
	for (int j = 0; j<larg; j++ ){
		if (image.at<uchar>(0,j) == 255 ){
			p.x = j;
			p.y = 0;
			floodFill(ImagemLimpa,p,0);
		}else if(image.at<uchar>(alt-1,j) == 255 ){
			p.x = j;
			p.y = alt-1;
			floodFill(ImagemLimpa,p,0); //mesma ideia,só que com linhas
		}
	}
	imshow("ImagemLimpa",ImagemLimpa);
	imwrite("imagem limpa sem bolhas.png", ImagemLimpa);



	nob=0;
	nobM=0;
	for(int i=0; i<alt; i++){
		for(int j=0; j<larg; j++){
			if(ImagemLimpa.at<uchar>(i,j) == 255){   //com  imagem sem as bolhas nas bordas, hora de começar a contar os objetos, para fazer isso, faz o programa  varrer a image

				nob++;
				p.x = j;
				p.y = i;
				floodFill(ImagemLimpa,p,nob);   // Para cada pixel branco encontrado, o contador é incrementado,e então usamos o floodfill para pintar todos de cinza

				if(nob == 255){
					nobM = nobM + nob;
					nob = 0;

				}
				floodFill(ImagemLimpa,p,50);
			}
		}
	}
	nob =  nob + nobM;
	imshow("sem bolhas borda rotulada",ImagemLimpa);
	imwrite("sem bolhas borda rotulada.png",ImagemLimpa);




	noburacos = 0;

	noobjectsB = 0;


	p.x = 0;


	p.y = 0;
	floodFill(ImagemLimpa,p,128);


	for(int i = 0; i< alt; i++){
		for(int j = 0; j<larg; j++){
			if(ImagemLimpa.at<uchar>(i,j) == 0){
				noburacos++;
				noobjectsB++;
				p.x = j;
				p.y = i;
				floodFill(ImagemLimpa,p,noburacos);
			}
		}
	}
	imshow("buracos rotulados",ImagemLimpa);
	imwrite("buracos rotulados.png",ImagemLimpa);

	nobSemBuracos =  nob - noobjectsB;

	cout << "Numero de objetos fora da borda: " << nob << endl;
	cout << "Numero de objetos sem buracos: " << nobSemBuracos << endl;
	cout << "Numero de objetos com buracos: " << noobjectsB << endl;
	cout << "Numero de buracos: " << noburacos << endl;


	waitKey();
	return 0;

}
