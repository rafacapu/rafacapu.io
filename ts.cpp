#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

double alfa,beta;

int desfoque_slider_max = 200;

int top_slider = 0;
int top_slider_max = 100;

Mat image1, image2, blended;
Mat imageTop, imageBlur;

char TrackbarName[50];


int altura_focada=20, Y_max, X_max, inicio=0, desfoque=10;
Mat frame32f, frameFiltered;
int n1, n2, limite, d;

void on_trackbar_blend(int, void*) {

        blended = image1.clone();
        n1 = inicio;
        n2 = inicio + limite;
        d = (desfoque / 20)*100;

        for (int i = 0; i<blended.rows; i++) {
                alfa= 0.5*(tanh((i-n1)/(d+ 1)) - tanh((i-n2)/(d+ 1)));
                beta = 1 - alfa;
                addWeighted(imageTop.row(i), alfa, imageBlur.row(i), beta, 0.0, blended.row(i));
        }
        imshow("addweighted", blended);
}


void on_trackbar_altura(int, void*) {
        imageBlur.copyTo(imageTop);
        limite = altura_focada *Y_max / 100;

        if (inicio >= Y_max - limite) {
                inicio = Y_max - limite - 1;
                cout << "não pode ir além. \n";
        }
        if (limite > 0) {
                Mat tmp = image2(Rect(0, inicio, X_max, limite));
                tmp.copyTo(imageTop(Rect(0, inicio, X_max, limite)));

        }
        on_trackbar_blend(desfoque, 0);
}

void on_trackbar_desfoque(int, void*) {

        image1.convertTo(frame32f, CV_32F);

        for (int i = 1; i < desfoque; i = i + 2){
                blur(image1, imageBlur, Size(i, i), Point(-1, -1));
        }

        on_trackbar_altura(altura_focada, 0);
}

int main(int argvc, char** argv) {
        image1 = imread("bavaria.jpeg");
        image2 = imread("bavaria.jpeg");
        image2.copyTo(imageTop);
        image1.copyTo(imageBlur);
        namedWindow("addweighted", 1);


        X_max = image1.size().width;
        Y_max = image2.size().height;

        on_trackbar_desfoque(desfoque, 0);
        on_trackbar_blend(desfoque, 0);


        sprintf(TrackbarName, "Altura x %d", Y_max);
        createTrackbar(TrackbarName, "addweighted",
                &altura_focada,
                100,
                on_trackbar_altura);
        on_trackbar_altura(altura_focada, 0);


        sprintf(TrackbarName, "Deslocamento x %d", Y_max);
        createTrackbar(TrackbarName, "addweighted",
                &inicio,
                Y_max,
                on_trackbar_altura);

        sprintf(TrackbarName, "Desfoque x %d", desfoque_slider_max);
        createTrackbar(TrackbarName, "addweighted",
                &desfoque,
                desfoque_slider_max,
                on_trackbar_desfoque);

        waitKey(0);
        return 0;
}

