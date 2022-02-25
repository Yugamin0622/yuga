#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <process.h>

#define Y_SIZE 512	// 処理最大画像
#define X_SIZE 512

#define HIGH   255	//　画像最大強度値
#define LOW      0	//　画像最小強度値
#define LEVEL  256	//　画像強度レベル値

// BMPファイルのフォーマットに従って用意した変数
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

unsigned char image_in[Y_SIZE][X_SIZE][3];	// 入力カラー画像配列
unsigned char image_out[Y_SIZE][X_SIZE][3];	//出力カラー画像配列
unsigned char image_bw[Y_SIZE][X_SIZE];		//濃淡画像配列
unsigned char data_rgb[Y_SIZE][X_SIZE][3];	// RGB画像配列


//   24Bitビットマップファイル読み込み       

void readBMP(
	char* filename,							// BMPファイル名
	unsigned char image[Y_SIZE][X_SIZE][3]	// 24ビットRGB画像配列 
)
{
	FILE* fp;
	int i, j, k;

	// ファイルオープン 
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("readBmp: Open error!\n");
		exit(1);
	}
	
	
		printf("input file : %s\n", filename);
	
	

	// ヘッダー情報読み込む
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
	// RGB画像データ読み込む
	for (i = 0; i < (int)biHeight; i++)
		for (j = 0; j < (int)biWidth; j++) {
			for (k = 0; k < 3; k++) {
				//fread(&image[i][j][2-k], 1, 1, fp);
				fread(&image[biHeight - i][j][2 - k], 1, 1, fp);
			}
		}
	fclose(fp);
}

//   24ビット-ビットマップデータをBMPファイルに出力    

void writeBMP(
	unsigned char image[Y_SIZE][X_SIZE][3],
	char* filename)
{
	FILE* fp;
	int i, j, k;
	// ファイルオープン 
	if ((fp = fopen(filename, "wb")) == NULL) {
		printf("writeBmp: Open error!\n");
		exit(1);
	}
	printf("output file : %s\n", filename);
	// ヘッダー情報 
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

	// ビットマップデータ 
	for (i = 0; i < (int)biHeight; i++)
		for (j = 0; j < (int)biWidth; j++) {
			for (k = 0; k < 3; k++) {
				fwrite(&image[biHeight - i][j][2 - k], 1, 1, fp);
			}
		}

	fclose(fp);
}

// ＲＧＢカラー画像を256諧調白黒濃淡画像へ変換 

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


// RGB画像処理　画像にFITを書く　 　　　

void makeFIT(unsigned char image_rgb[Y_SIZE][X_SIZE][3])
{
	int y, x;

	// F を書く,色が赤
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

	// Ｉ を書く、色が緑
	for (y = 20; y <= 140; y++)
		for (x = 90; x <= 110; x++) {
			image_rgb[y][x][0] = 0;
			image_rgb[y][x][1] = 255;
			image_rgb[y][x][2] = 0;
		}


	// T を書く、色が黄色
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


// 白黒画像を24bitビットマップ形式に変換 

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



// 白黒画像の２値化                      

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


// 白黒濃淡画像配列のコピー              
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

	printf("入力画像ファイル名(*.bmp)："); 
	
	
		scanf("%s", input);
	
	readBMP(input, image_in);	  // 画像の入力,RGB24ビットカラーBMP画像を配列に格納

	// 画像処理１：画像上の文字
	makeFIT(image_in);	// RGB画像処理、ＦＩＴを書く
	printf("FIT出力画像ファイル名(*.bmp)："); scanf("%s", output);
	writeBMP(image_in, output);		// FIT画像出力

	//画像処理２：白黒変換
	BMPto256BW(image_in, image_bw); // ＲＧＢカラー画像を白黒画像に変換
	BWto24BMP(image_bw, image_out); //　単チャンネル白黒画像を３チャンネルＢＭＰ標準フォーマットへ変換
	printf("白黒出力画像ファイル名(output.bmp)："); scanf("%s", output);
	writeBMP(image_out, output);		// 白黒画像出力

	//画像処理３：2値化
	printf("２値化画像の閾値（０～２５５）："); scanf("%d", &intensity);
	toBinary(intensity, image_bw);
	BWto24BMP(image_bw, image_out);
	printf("２値化出力画像ファイル名(*.bmp)："); scanf("%s", output);
	writeBMP(image_out, output);		// 白黒画像出力

}



