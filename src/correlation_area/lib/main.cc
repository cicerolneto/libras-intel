#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Correlacao.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){


    Correlacao *im1 = new Correlacao();
    Correlacao *im2 = new Correlacao();
    Mat image1 = imread(argv[1]);
    Mat image2 = imread(argv[2]);

    image1.convertTo(image1,CV_8UC1);
	
    image2.convertTo(image2,CV_8UC1);

   
    im1->mount_vector(image1);

    cout << "Feature vector one is ready -> img1" << endl;

    im2->mount_vector(image2);

    cout << "Feature vector two is ready -> img2" << endl;

    for(int i = 0; i < im1->__size; i++){
		cout << "Imagem 1: " << im1->_feature.at(i) << "  Image 2: " <<  im2->_feature.at(i) << endl;
    }

    double result = im2->pearson_correlation(im1->_feature);

    cout << "==================" << endl;
    cout << "Correlação :  " << result << endl;
    cout << "==================" << endl;

    return 0;

}

