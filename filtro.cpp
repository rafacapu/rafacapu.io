#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m) {
        for (int i = 0; i<m.size().height; i++) {
                for (int j = 0; j<m.size().width; j++) {
                        cout << m.at<float>(i, j) << ",";
                }
                cout << endl;
        }
}

void menu() {
        cout << "\npressione a tecla para ativar o filtro: \n"
                "a - calcular modulo\n"
                "m - media\n"
                "g - gauss\n"
                "v - vertical\n"
                "h - horizontal\n"
                "l - laplaciano\n"
                "p - Laplaciano do Gaussiano\n"
                "esc - sair\n";
}

int main(int argvc, char** argv) {
        VideoCapture video;
        float media[] = { 1,1,1,
                1,1,1,
                1,1,1 };
        float media5[] = { 1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1 };

        float gauss[] = { 1,2,1,
                2,4,2,
                1,2,1 };
        float horizontal[] = { -1,0,1,
                -2,0,2,
                -1,0,1 };
        float vertical[] = { -1,-2,-1,
                0,0,0,
                1,2,1 };
        float laplacian[] = { 0,-1,0,
                -1,4,-1,
                0,-1,0 };

        Mat cap, frame, frame32f, frameFiltered;
        Mat mask(3, 3, CV_32F), mask1;
        Mat result, result1;
        double width, height, min, max;
        int absolut,lg=0;
        char key;

        video.open(0);
        if (!video.isOpened())
                return -1;





        width = video.get(CV_CAP_PROP_FRAME_WIDTH);
        height = video.get(CV_CAP_PROP_FRAME_HEIGHT);
        cout << "largura=" << width << "\n";;
       cout << "altura =" << height << "\n";;

        namedWindow("filtro espacial", 1);

        mask = Mat(3, 3, CV_32F, media);
        scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
        swap(mask, mask1);
        absolut = 1;

        menu();
        for (;;) {

                video >> cap;
                cvtColor(cap, frame, CV_BGR2GRAY);
                flip(frame, frame, 1);
                imshow("original", frame);
                frame.convertTo(frame32f, CV_32F);
                filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);

                if (lg == 1) {
                        mask = Mat(3, 3, CV_32F, laplacian);
                        filter2D(frameFiltered, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);
                }
                if (absolut) {
                        frameFiltered = abs(frameFiltered);
                }

                frameFiltered.convertTo(result, CV_8U);
                imshow("filtro espacial", result);
                key = (char)waitKey(10);
                if (key == 27) break;
                switch (key) {
                case 'a':
                        menu();
                        absolut = !absolut;
                        break;
                case 'm':
                        menu();
                        mask = Mat(3, 3, CV_32F, media);
                        scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
                        mask = mask1;
                        printmask(mask);
                        break;
                case 'g':
                        menu();
                        mask = Mat(3, 3, CV_32F, gauss);
                        scaleAdd(mask, 1 / 16.0, Mat::zeros(3, 3, CV_32F), mask1);
                        mask = mask1;
                        lg = 0;
                        printmask(mask);
                        break;
                case 'h':
                        menu();
                        mask = Mat(3, 3, CV_32F, horizontal);
                        printmask(mask);
                        break;
                case 'v':
                        menu();
                        mask = Mat(3, 3, CV_32F, vertical);
                        printmask(mask);
                        break;
                case 'l':
                        menu();
                        mask = Mat(3, 3, CV_32F, laplacian);
                        printmask(mask);
                        lg = 0;
                        break;
                case 'p':
                        menu();
                        mask = Mat(3, 3, CV_32F, gauss);
                        scaleAdd(mask, 1 / 16.0, Mat::zeros(3, 3, CV_32F), mask1);
                        mask = mask1;
                        lg = 1;
                        printmask(mask);
                        break;
                default:
                        break;
                }
        }
        return 0;
}
