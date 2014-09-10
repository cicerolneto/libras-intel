#include <unistd.h>
#include <time.h>
#include "Tratamento_imagem.h"

String caminho = "/tmp/";
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

	while(true){
		image=imread(argv[1],1);		
			aux=Tratamento_imagem::getPSNR(image , image_old);			
			if(aux < 20 && aux!=0){
			salvar_imagem( image );			
		}
		sleep(atoi(argv[2]));
		image_old = image;

	}

}

