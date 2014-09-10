#include "Tratamento_imagem.h"


/*Função que deixa a imagem em tons de cinza
 param
 	 Mat image -> Imagem que vai ser trabalhado e deixada em tons de cinza;

 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::filtro_cinza(Mat image) {
	Mat gray_image;
	cvtColor(image, gray_image, CV_RGB2GRAY);
	return gray_image;
}
/*Função que deixa a imagem em HSV
 param
 	 Mat image -> Imagem que vai ser trabalhado e deixada em tons de cinza;

 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::filtro_hsv(Mat image) {
	Mat hsv_image;
	cvtColor(image, hsv_image, CV_BGR2HSV);
	return hsv_image;
}
/*Função que faz um blur na imagem, da uma embaçada na imagem;
 param
 	 Mat image -> Imagem que vai ser trabalhado e deixada embaçada;
 	 int modo -> mode de operação, se mode de operação.
 
 1 -> Bilateral Filter
 2 -> Gaussina Bluer

 return -> retorna a imagem trabalhada;
 */
 Mat Tratamento_imagem::blur_imagem(Mat image, int modo, int MAX_KERNEL_LENGTH) {
	Mat aux_image;
	switch (modo) {
	case 1:
		for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
			bilateralFilter(image, aux_image, i, i * 2, i / 2);
		}
		return aux_image;
	case 2:
		for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
			GaussianBlur(image, aux_image, Size(i, i), 0, 0);
		}
		return aux_image;
	default:
		cout << "Modo de operação não existe" << endl;
		return aux_image;
		break;
	}

}
/*Função que satura a imagem
 param
 	 Mat image -> Imagem que vai ser saturada;

 return -> retorna a imagem trabalhada;
 *//*
Mat saturacao_imagem(Mat image) {

	Mat aux_image;
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < 3; c++) {
				aux_image.at < Vec3b > (y, x)[c] = saturate_cast < uchar
						> (alpha * (image.at < Vec3b > (y, x)[c]) + beta);
			}
		}
	}

	return aux_image;

}
*/
/*Função cria uma jenela com imagem
 param
 	 Mat image -> Imagem que vai ser visualizada;
 	 nome_janela -> Nome que irá aparecer na janela;
 */
void Tratamento_imagem::mostra_imagem(Mat image, String nome_janela) {

	namedWindow(nome_janela, CV_WINDOW_AUTOSIZE);
	imshow(nome_janela, image);

}

/*Função que aplica metodo canny para destacar contornos na imagem
 param
 	 Mat image -> Imagem que vai ser saturada;
 	 int tresh -> grau do filtro Canny

 return -> retorna a imagem trabalhada;
 */
Mat Tratamento_imagem::canny(Mat image, int thresh) {

	Mat aux_image;

	/// Detect edges using canny
	Canny(image, aux_image, thresh, thresh * 2, 3);

	return aux_image;
}



/*Função que detecta os contornos a partir de uma imagem
 param
 	 Mat image -> Imagem que vai ser identificado o contorno.

 return -> retorna classe Contorno com os dados do contorno(contours,hierarchy,size)
 
 Note

Source image is modified by this function. Also, the function does not take into account 1-pixel border of the image (it’s filled with 0’s and used for neighbor analysis in the algorithm), therefore the contours touching the image border will be clipped.
 
 */
Tratamento_imagem::Contorno Tratamento_imagem::find_contour(Mat image) {

	vector < vector<Point> > contours_aux;
	vector < Vec4i > hierarchy_aux;

	Contorno aux_contorno;
	findContours(image, contours_aux, hierarchy_aux, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	aux_contorno.contours(contours_aux);
	aux_contorno.hierarchy(hierarchy_aux);
	aux_contorno.size(image.size());
	return aux_contorno;
}
/*Função que cria uma imagem a partir dos contornos
 param
 	 Contorno -> Classe Contorno com os dados do contorno

 return -> retorna imagem desenhada a partir do contorno
 */
Mat Tratamento_imagem::draw_contour(Tratamento_imagem::Contorno contornos){
	Mat drawing = Mat::zeros(contornos.size(), CV_8UC3);
	for (int i = 0; i < contornos.contours().size(); i++) {
		Scalar color = Scalar(255, 255, 255);
		drawContours(drawing, contornos.contours(), i, color, 2, 8,
				contornos.hierarchy(), 0, Point());
	}
	return drawing;
}

/*Função que cria uma imagem a partir dos contornos
 param
 	 Mat image -> Imagem que quer achar o contorno

 return -> retorna imagem desenhada a partir do contorno
 */
Mat Tratamento_imagem::draw_contour_image(Mat image){
	Mat aux_image=image.clone();
	Tratamento_imagem::Contorno contornos=find_contour(canny(aux_image,50));
	Mat drawing = Mat::zeros(contornos.size(), CV_8UC3);
	for (int i = 0; i < contornos.contours().size(); i++) {
		Scalar color = Scalar(255, 255, 255);
		drawContours(drawing, contornos.contours(), i, color, 2, 8,
				contornos.hierarchy(), 0, Point());
	}
	return drawing;
}
/*Função que deixa a imagem em HUE
 param
 	 Mat image -> Imagem que vai ser trabalhado e deixada em HUE

 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::hue_image(Mat image) {
	Mat hue;

	Mat hsv = filtro_hsv(image);
/// Use only the Hue value
	hue.create(hsv.size(), hsv.depth());
	int ch[] = { 0, 0 };
	mixChannels(&hsv, 1, &hue, 1, ch, 1);
	return hue;
}

/*Função que aplica um alto contraste na foto
 param
 	 Mat image -> Imagem que vai ser trabalhada
 	 int bins -> grau de BackProject (quando proxima de 0 imagem fica em PB. >20 imagem fica em tons de cinza).

 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::Hist_and_Backproj(Mat image,int bins) {
	Mat aux_image=hue_image(image);
	MatND hist;
	int histSize = MAX(bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };
	calcHist( &aux_image, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
	normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
	MatND backproj;
	calcBackProject( &aux_image, 1, 0, hist, backproj, &ranges, 1, true );
	return backproj;
	
}

/*Função que aplica threshold
 param
	Mat image -> Imagem que vai ser trabalhada
	int intensity -> intensidade do operador
	int mode -> modo de operação do threshold
		0: Binary
		1: Binary Inverted
		2: Threshold Truncated
		3: Threshold to Zero
		4: Threshold to Zero Inverted
		
 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::threshold_image(Mat image, int mode,int intensity){
	Mat aux_image; 
	threshold(image,aux_image,intensity,max_BINARY_value,mode);
	return aux_image;
}

/*Função que modifica o tamanho da imagem
 param
	Mat image -> Imagem que vai ser trabalhada
	int proporcao -> razao de proporcionalidade
	int mode -> modo de operação do threshold
		0: Binary
		1: Binary Inverted
		2: Threshold Truncated
		3: Threshold to Zero
		4: Threshold to Zero Inverted
		
 return -> retorna a imagem trabalhada;

 */
Mat Tratamento_imagem::image_scale(Mat image,int mode,int proporcao)
{
	if(mode<=0 or proporcao<=0){
		cerr << "informação incorreta" << endl;
	}
	Mat aux_image;
    switch(mode){
    case 1:
    	pyrUp( image, aux_image, Size( image.cols*proporcao, image.rows*proporcao) );
    	return aux_image;
    case 2:
    	pyrDown( image, aux_image, Size( image.cols/proporcao, image.rows/proporcao ) );
    	return aux_image;
    }

}

Scalar Tratamento_imagem::getMSSIM(const Mat& i1, const Mat& i2) {
	const double C1 = 6.5025, C2 = 58.5225;
	/***************************** INITS **********************************/
	int d = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d);           // cannot calculate on one byte large values
	i2.convertTo(I2, d);

	Mat I2_2 = I2.mul(I2);        // I2^2
	Mat I1_2 = I1.mul(I1);        // I1^2
	Mat I1_I2 = I1.mul(I2);        // I1 * I2

	/***********************PRELIMINARY COMPUTING ******************************/

	Mat mu1, mu2;   //
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	///////////////////////////////// FORMULA ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);   // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

	Scalar mssim = mean(ssim_map); // mssim = average of ssim map
	return mssim;
}

double Tratamento_imagem::getPSNR(const Mat& I1, const Mat& I2) {
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else {
		double mse = sse / (double) (I1.channels() * I1.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}

// Tratamento_imagem constructor: must initialize Tratamento_imagem's attributes
Tratamento_imagem::Tratamento_imagem() {
}

// Tratamento_imagem destructor: must finalize Tratamento_imagem's attributes
Tratamento_imagem::~Tratamento_imagem() {
return;
}


