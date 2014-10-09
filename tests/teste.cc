#include <unistd.h>
#include "Tratamento_imagem.h"
#include <iostream>



String caminho = "../img/";
String caminho_salvar = "../img/Tratada/";

String alfabeto[27] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P"
                       ,"Q","R","S","T","U","V","W","X","Y","Z"};


int * centroide_contorno(Mat image){

	int i,j,aux_esq,aux_dir,aux_sup,cent_x,cent_y;
     	int *ret = (int*)malloc(100*sizeof(int));
	aux_esq=image.cols;
	for(i=0; i<image.rows; i++){
	    for(j=0; j<image.cols; j++){
			if(aux_esq>j and image.at<uchar>(i,j)!=0){
				aux_esq=j;
			}
		}
	}
	aux_dir=0;
	for(i=0; i<image.rows; i++){
	    for(j=image.cols; j>=0 ; j--){
			if(aux_dir<j and image.at<uchar>(i,j)!=0){
				aux_dir=j;
			}
		}
	}
	aux_sup=image.rows;
	for(j=0; j<image.cols; j++){
	    for(i=0; i<image.rows and image.at<uchar>(i,j)!=0; i++){
			if(aux_sup>i and image.at<uchar>(i,j)!=0){
				aux_sup=i;
			}
		}
	}
	cout << aux_esq << endl << aux_dir << endl << aux_sup << endl;
	ret[0]=aux_esq;
	ret[1]=aux_dir;
	ret[2]=aux_sup;
	ret[3]=cent_x;
	ret[4]=cent_y;
	return ret;
	

}

int main(int argc, char** argv) {
	Mat image,image_aux;


	image=imread(argv[1],1);
	image= Tratamento_imagem::tratar_imagem(image);
	image = Tratamento_imagem::cortar_image(image,0,0,90,90);
	imwrite("./teste.jpg",image);
	centroide_contorno(image);


}
