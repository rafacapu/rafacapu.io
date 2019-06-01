#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 2
#define RAIO 5

int main(int argc, char** argv) {
        vector<int> yrange;
        vector<int> xrange;
        Mat image, frame, points, bordas;
        int width, height, gray;
        int x, y;
        //valores dos limiares
        vector<int> limiar_range ={50,100,150,200};//Se o compilador suportar C++11
        Vec3b val;

        //image = imread("spidey.png", CV_LOAD_IMAGE_GRAYSCALE);
        image = imread("daft.png", CV_LOAD_IMAGE_COLOR);
        srand(time(0));
        //srand(0);

        if (!image.data) {
                cout << "nao abriu" << "a imagem" << endl;
                exit(0);
        }

        width = image.size().width;
        height = image.size().height;
        xrange.resize(height / STEP);
        yrange.resize(width / STEP);

        iota(xrange.begin(), xrange.end(), 0);
        iota(yrange.begin(), yrange.end(), 0);

        for (uint i = 0; i<xrange.size(); i++) {
                xrange[i] = xrange[i] * STEP + STEP / 2;
        }
        for (uint i = 0; i<yrange.size(); i++) {
                yrange[i] = yrange[i] * STEP + STEP / 2;
        }

        //points = Mat(height, width, CV_8U, Scalar(255));
        //frame = Mat(height, width, CV_8U, Scalar(255));
        points = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));
        frame = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));

        random_shuffle(xrange.begin(), xrange.end());
        //Faz a pintura de pontos
        for (auto i : xrange) {
                random_shuffle(yrange.begin(), yrange.end());
                for (auto j : yrange) {
                        x = i + rand() % (2 * JITTER) - JITTER + 1;
                        y = j + rand() % (2 * JITTER) - JITTER + 1;
                        //gray = image.at<uchar>(x, y);
                        val[0] = image.at<Vec3b>(i, j)[0];
                        val[1] = image.at<Vec3b>(i, j)[1];
                        val[2] = image.at<Vec3b>(i, j)[2];
                        circle(points,
                                cv::Point(y, x),
                                RAIO,
                                CV_RGB(val[2], val[1], val[0]),
                                -1,        CV_AA);
                }
        }
        imshow("Pontilhada", points);
        imwrite("daftPont.png", points);

        //Canny e imagem FINAL xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        int s = 1,raio;

        for (auto limiar : limiar_range) {
                Canny(image, bordas, limiar, 3*limiar);

                raio = RAIO - s;

                for (int h = 1; h < height; h++) {
                        for (int w = 1; w < width; w++) {
                                if (bordas.at<uchar>(h, w) == 255) {
                                        //gray = image.at<uchar>(h, w);
                                        val[0] = image.at<Vec3b>(h, w)[0];
                                        val[1] = image.at<Vec3b>(h, w)[1];
                                        val[2] = image.at<Vec3b>(h, w)[2];

                                        circle(points,
                                                        cv::Point(w, h),
                                                        raio,
                                                        CV_RGB(val[2], val[1], val[0]),
                                                        -1, CV_AA);
                                        circle(frame,
                                                cv::Point(w, h),
                                                raio,
                                                CV_RGB(val[2], val[1], val[0]),
                                                -1, CV_AA);
                                }
                        }
                }
                s++;
        }
        imwrite("images/SpideyCP.png", points);
        imshow("FINAL", points);
        imshow("Bordas", frame);
        imwrite("images/PontosNovos_Spidey.png", frame);
        waitKey();
        return 0;
}
