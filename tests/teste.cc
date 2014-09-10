#include <unistd.h>
#include <time.h>
#include "Tratamento_imagem.h"

String caminho = "./";
time_t timer;
struct tm * timeinfo;

void salvar_imagem(Mat image){
	time (&timer);
	timeinfo = localtime (&timer);
	imwrite( caminho+asctime(timeinfo)+".jpg", image );
	cout << "Funcionando" << endl;

}

int main(int argc, char** argv) {
	Mat image_old = imread(argv[1], 1);
	double aux;
	Mat image;
	image=imread(argv[1],1);		
	image = Tratamento_imagem::Hist_and_Backproj(image,atoi(argv[2]));
	salvar_imagem( image );			

	while(true)
			{
				imshow("C em Libras",image);
				if(waitKey(30)>=0)
					break;
			}
	
}

