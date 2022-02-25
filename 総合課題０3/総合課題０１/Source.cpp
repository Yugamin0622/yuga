#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <process.h>

#define Y_SIZE 512	// �����ő�摜
#define X_SIZE 512

#define HIGH   255	//�@�摜�ő勭�x�l
#define LOW      0	//�@�摜�ŏ����x�l
#define LEVEL  256	//�@�摜���x���x���l

// BMP�t�@�C���̃t�H�[�}�b�g�ɏ]���ėp�ӂ����ϐ�
typedef unsigned short WORD;
typedef unsigned long DWORD;

WORD  bfType;
DWORD bfSize;
WORD  bfReserved1,
bfReserved2;
DWORD bfOffBits;
DWORD biSize,
biWidth, biHeight;
WORD  biPlanes,
biBitCount;
DWORD biCompression,
biSizeImage,
biXPelsPerMeter,
biYPelsPerMeter,
biClrUsed,
biClrImportant;

unsigned char image_in[Y_SIZE][X_SIZE][3];	// ���̓J���[�摜�z��
unsigned char image_out[Y_SIZE][X_SIZE][3];	//�o�̓J���[�摜�z��
unsigned char image_bw[Y_SIZE][X_SIZE];		//�Z�W�摜�z��
unsigned char data_rgb[Y_SIZE][X_SIZE][3];	// RGB�摜�z��


//   24Bit�r�b�g�}�b�v�t�@�C���ǂݍ���       

void readBMP(
	char* filename,							// BMP�t�@�C����
	unsigned char image[Y_SIZE][X_SIZE][3]	// 24�r�b�gRGB�摜�z�� 
)
{
	FILE* fp;
	int i, j, k;

	// �t�@�C���I�[�v�� 
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("readBmp: Open error!\n");
		exit(1);
	}
	
	
		printf("input file : %s\n", filename);
	
	

	// �w�b�_�[���ǂݍ���
	fread(&bfType, sizeof(bfType), 1, fp);
	fread(&bfSize, sizeof(bfSize), 1, fp);
	fread(&bfReserved1, sizeof(bfReserved1), 1, fp);
	fread(&bfReserved2, sizeof(bfReserved2), 1, fp);
	fread(&bfOffBits, sizeof(bfOffBits), 1, fp);
	fread(&biSize, sizeof(biSize), 1, fp);
	fread(&biWidth, sizeof(biWidth), 1, fp);
	fread(&biHeight, sizeof(biHeight), 1, fp);
	fread(&biPlanes, sizeof(biPlanes), 1, fp);
	fread(&biBitCount, sizeof(biBitCount), 1, fp);
	fread(&biCompression, sizeof(biCompression), 1, fp);
	fread(&biSizeImage, sizeof(biSizeImage), 1, fp);
	fread(&biXPelsPerMeter, sizeof(biXPelsPerMeter), 1, fp);
	fread(&biYPelsPerMeter, sizeof(biYPelsPerMeter), 1, fp);
	fread(&biClrUsed, sizeof(biClrUsed), 1, fp);
	fread(&biClrImportant, sizeof(biClrImportant), 1, fp);
	// RGB�摜�f�[�^�ǂݍ���
	for (i = 0; i < (int)biHeight; i++)
		for (j = 0; j < (int)biWidth; j++) {
			for (k = 0; k < 3; k++) {
				//fread(&image[i][j][2-k], 1, 1, fp);
				fread(&image[biHeight - i][j][2 - k], 1, 1, fp);
			}
		}
	fclose(fp);
}

//   24�r�b�g-�r�b�g�}�b�v�f�[�^��BMP�t�@�C���ɏo��    

void writeBMP(
	unsigned char image[Y_SIZE][X_SIZE][3],
	char* filename)
{
	FILE* fp;
	int i, j, k;
	// �t�@�C���I�[�v�� 
	if ((fp = fopen(filename, "wb")) == NULL) {
		printf("writeBmp: Open error!\n");
		exit(1);
	}
	printf("output file : %s\n", filename);
	// �w�b�_�[��� 
	fwrite(&bfType, sizeof(bfType), 1, fp);
	fwrite(&bfSize, sizeof(bfSize), 1, fp);
	fwrite(&bfReserved1, sizeof(bfReserved1), 1, fp);
	fwrite(&bfReserved2, sizeof(bfReserved2), 1, fp);
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, fp);

	fwrite(&biSize, sizeof(biSize), 1, fp);
	fwrite(&biWidth, sizeof(biWidth), 1, fp);
	fwrite(&biHeight, sizeof(biHeight), 1, fp);
	fwrite(&biPlanes, sizeof(biPlanes), 1, fp);
	fwrite(&biBitCount, sizeof(biBitCount), 1, fp);
	fwrite(&biCompression, sizeof(biCompression), 1, fp);
	fwrite(&biSizeImage, sizeof(biSizeImage), 1, fp);
	fwrite(&biXPelsPerMeter, sizeof(biXPelsPerMeter), 1, fp);
	fwrite(&biYPelsPerMeter, sizeof(biYPelsPerMeter), 1, fp);
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, fp);
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, fp);

	// �r�b�g�}�b�v�f�[�^ 
	for (i = 0; i < (int)biHeight; i++)
		for (j = 0; j < (int)biWidth; j++) {
			for (k = 0; k < 3; k++) {
				fwrite(&image[biHeight - i][j][2 - k], 1, 1, fp);
			}
		}

	fclose(fp);
}

// �q�f�a�J���[�摜��256�~�������Z�W�摜�֕ϊ� 

void BMPto256BW(
	unsigned char image[Y_SIZE][X_SIZE][3],
	unsigned char image_bw[Y_SIZE][X_SIZE])
{
	int y, x, a;
	for (y = 0; y < (int)biHeight; y++)
		for (x = 0; x < (int)biWidth; x++) {
			a = (int)(0.3 * image[y][x][0] + 0.59 * image[y][x][1] + 0.11 * image[y][x][2]);
			if (a < LOW) a = LOW;
			if (a > HIGH) a = HIGH;
			image_bw[y][x] = a;
		}
}


// RGB�摜�����@�摜��FIT�������@ �@�@�@

void makeFIT(unsigned char image_rgb[Y_SIZE][X_SIZE][3])
{
	int y, x;

	// F ������,�F����
	for (y = 20; y <= 40; y++)
		for (x = 20; x <= 70; x++) {
			image_rgb[y][x][0] = 255;
			image_rgb[y][x][1] = 0;
			image_rgb[y][x][2] = 0;
		}

	for (y = 70; y <= 90; y++)
		for (x = 20; x <= 70; x++) {
			image_rgb[y][x][0] = 255;
			image_rgb[y][x][1] = 0;
			image_rgb[y][x][2] = 0;
		}


	for (y = 40; y <= 140; y++)
		for (x = 20; x <= 35; x++) {
			image_rgb[y][x][0] = 255;
			image_rgb[y][x][1] = 0;
			image_rgb[y][x][2] = 0;
		}

	// �h �������A�F����
	for (y = 20; y <= 140; y++)
		for (x = 90; x <= 110; x++) {
			image_rgb[y][x][0] = 0;
			image_rgb[y][x][1] = 255;
			image_rgb[y][x][2] = 0;
		}


	// T �������A�F�����F
	for (y = 20; y <= 40; y++)
		for (x = 130; x <= 180; x++) {
			image_rgb[y][x][0] = 255;
			image_rgb[y][x][1] = 255;
			image_rgb[y][x][2] = 0;
		}

	for (y = 40; y <= 140; y++)
		for (x = 145; x <= 165; x++) {
			image_rgb[y][x][0] = 255;
			image_rgb[y][x][1] = 255;
			image_rgb[y][x][2] = 0;
		}
}


// �����摜��24bit�r�b�g�}�b�v�`���ɕϊ� 

void BWto24BMP(
	unsigned char image_bw[Y_SIZE][X_SIZE],
	unsigned char image[Y_SIZE][X_SIZE][3])
{
	int y, x, a;
	for (y = 0; y < (int)biHeight; y++)
		for (x = 0; x < (int)biWidth; x++) {
			a = image_bw[y][x];
			image[y][x][0] = a;
			image[y][x][1] = a;
			image[y][x][2] = a;
		}
}



// �����摜�̂Q�l��                      

void toBinary(
	int atai,
	unsigned char image_bw[Y_SIZE][X_SIZE])
{
	int y, x;
	for (y = 0; y < (int)biHeight; y++)
		for (x = 0; x < (int)biWidth; x++) {
			if (image_bw[y][x] >= atai)	image_bw[y][x] = HIGH;
			else image_bw[y][x] = LOW;
		}
}


// �����Z�W�摜�z��̃R�s�[              
void imageCopyBW(
	unsigned char image1[Y_SIZE][X_SIZE],
	unsigned char image2[Y_SIZE][X_SIZE])
{
	int x, y;
	for (y = 0; y < (int)biHeight; y++)
		for (x = 0; x < (int)biWidth; x++)
			image2[y][x] = image1[y][x];
}




void main(void)
{
	char input[100], output[100];
	int intensity,i;

	printf("���͉摜�t�@�C����(*.bmp)�F"); 
	
	
		scanf("%s", input);
	
	readBMP(input, image_in);	  // �摜�̓���,RGB24�r�b�g�J���[BMP�摜��z��Ɋi�[

	// �摜�����P�F�摜��̕���
	makeFIT(image_in);	// RGB�摜�����A�e�h�s������
	printf("FIT�o�͉摜�t�@�C����(*.bmp)�F"); scanf("%s", output);
	writeBMP(image_in, output);		// FIT�摜�o��

	//�摜�����Q�F�����ϊ�
	BMPto256BW(image_in, image_bw); // �q�f�a�J���[�摜�𔒍��摜�ɕϊ�
	BWto24BMP(image_bw, image_out); //�@�P�`�����l�������摜���R�`�����l���a�l�o�W���t�H�[�}�b�g�֕ϊ�
	printf("�����o�͉摜�t�@�C����(output.bmp)�F"); scanf("%s", output);
	writeBMP(image_out, output);		// �����摜�o��

	//�摜�����R�F2�l��
	printf("�Q�l���摜��臒l�i�O�`�Q�T�T�j�F"); scanf("%d", &intensity);
	toBinary(intensity, image_bw);
	BWto24BMP(image_bw, image_out);
	printf("�Q�l���o�͉摜�t�@�C����(*.bmp)�F"); scanf("%s", output);
	writeBMP(image_out, output);		// �����摜�o��

}



