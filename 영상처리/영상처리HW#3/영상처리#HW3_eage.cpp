// ����ó�� ���� 3�� 
#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <windows.h>

#define MAX 512
#define SNR 9.0 // 3-1�� ������ SNR
#define WHITE 255
#define BLACK 0
#define THRESHOLD 150
#define SNR2 8.0 //3-2�� ������ SNR�� 8.0
using namespace std;

//����ü ���� 
typedef struct headers {
    BITMAPFILEHEADER hFile;
    BITMAPINFOHEADER hInfo;
    RGBQUAD hRGB[256];
} BITMAPHEADERS;

//�Լ� ����
//�������� ����� �Լ�  
void generate_headers(BITMAPHEADERS& bh); //��Ʈ�� ���� ����� ������ �����ϴ� �Լ� 
void reverse_raw(BYTE* image); //raw������ �������� �Լ� 
void bmp1(BYTE* output_image, string output_name); // Bmp������ ���� �� ���� �ϴ� �Լ�, 1�� ���� �̹����� �����ϴ� ���
void bmp2(BYTE* output_image, string output_name); //2������������ ������, bmp1�Լ��� ���� 
double calculate_variance(BYTE* image); //�̹��� �л��� ����ϴ� �Լ� 
double gaussian(double sigma); //  �ñ׸��� ���� ����þ� ������ �����ϴ� �Լ�
void add_gaussian_noise(BYTE* image, BYTE* noise, double sigma); // �̹����� ����þ� ������ �߰��ϴ� �Լ�
//3-1�������� �ʿ��� �Լ� 
void masking(BYTE* masked, BYTE* image, int mask[][9]); // ����ũ�� ����Ͽ� �̹����� ����ŷ�� �����ϴ� �Լ�
void masking_stochastic(BYTE* masked_image, BYTE* image, double mask[][25]); //stochastic ����ũ�� ����Ͽ� �̹����� stocahstic ����ŷ�� �����ϴ� �Լ�
double calculate_error_rate(BYTE* masked_original, BYTE* masked_noise); // ���� �̹����� ������ �̹����� ���� ������ ����ϴ� �Լ�
void edge_detection(BYTE* image, BYTE* noise, int mask[][9], string output_name); //����ũ�� ����Ͽ� ���� ������ �����ϴ� �Լ� 
void edge_detection_stochastic(BYTE* image, BYTE* noise, double mask[][25], string output_name); //Stochastic�� ����Ͽ� Stochastic ���� ���� �����ϴ� �Լ� 
//3-2�������� �ʿ��� �Լ�
int calculate_MSE(BYTE* masked_image, BYTE* masked_noise); //MSE�� ��� ���ִ� �Լ�
void lowpass_masking(BYTE* masked_image, BYTE* image, double mask[9]); // Lowpass ���͸��� ���� ����ũ�� �ϴ� �Լ�
void lowpass_filter(BYTE* image, BYTE* noise, string output_name); // Lowpass_fiter�� �����ϴ� �Լ�
void median_masking(BYTE* masked, BYTE* image); // Median ���͸��� �����ϱ� ���� �̹��� �����ϴ� �Լ� 
void median_filter(BYTE* image, BYTE* noise, string output_name); //Median_Filter�� �����ϴ� �Լ� 

//�����Լ� 
int main(void) {

    //���� �ʱ�ȭ
    srand((unsigned int)time(NULL));

    //���� ���⿡ ����� ����ũ ��� 
    //G_Y �� G_X^T�� ���� 
    //�ι��� 
    int Roberts[2][9] = { 
        //G_X
        {0, 0, -1, 
         0, 1, 0, 
         0, 0, 0},
         //G_Y
        {-1, 0, 0, 
         0, 1, 0, 
         0, 0, 0} 
    };
    //G_Y �� G_X^T�� ���� 
    //������
    int Prewitt[2][9] = { 
        //G_X
        {1, 0, -1, 
         1, 0, -1, 
         1, 0, -1},
        //G_Y
        {-1, -1, -1, 
         0, 0, 0, 
         1, 1, 1} 
    };
    //G_Y �� G_X^T�� ���� 
    //�Һ�
    int Sobel[2][9] = { 
        //G_X
        {1, 0, -1, 
        2, 0, -2, 
        1, 0, -1},
        //G_Y
        {-1, -2, -1, 
        0, 0, 0, 
        1, 2, 1} 
    };

    //G_Y �� G_X^T�� ���� 
    //����ĳ��ƽ
    double Stochastic[2][25] = {
        //G_X
        {0.267, 0.364, 0, -0.364, -0.267, 
         0.373, 0.562, 0, -0.562, -0.373, 
         0.463, 1.000, 0, -1.000, -0.463, 
         0.373, 0.562, 0, -0.562, -0.373, 
         0.267, 0.364, 0, -0.364, -0.267},
         //G_Y
        {-0.267, -0.373, -0.463, -0.373, -0.267, 
         -0.364, -0.562, -1.000, -0.562, -0.364, 
          0, 0, 0, 0, 0,      
          0.364,  0.562,  1.000, 0.562,  0.364,  
          0.267,  0.373,  0.463,  0.373,  0.267} 
    };

    //1�� ���� �̹��� ó���� ���Ͽ� ���� ������ �о�´�. 
    FILE* input_file1 = fopen("lena_raw_512x512.raw", "rb");
    if (input_file1 == NULL) {
        printf("FILE1 is ERROR\n"); //���� ������ ���α׷� ����
        return 1;
    }

    BYTE* image1 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);

    size_t read_bytes1 = fread(image1, sizeof(BYTE), MAX * MAX, input_file1);
    if (read_bytes1 != MAX * MAX) {
        printf("Failed to read RAW image1 file \n");
        fclose(input_file1); // ������ �ݰ� ���α׷��� ����
        return 1;
    }

    fclose(input_file1);

    // 1�� ���� �̹����� ����þ� ���� �߰�
    BYTE* noise1 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    double variance = calculate_variance(image1);
    double stddev_noise = sqrt(variance / pow(10.0, ((double)SNR / 10)));
    add_gaussian_noise(image1, noise1, stddev_noise);
    
    //1�� ���� ���� ����� �����Ѵ�. 
    bmp1(image1, "0-1. Lena_Original");
    reverse_raw(image1);
    // ����þ� �̹����� �߰��� �̹����� ���� 
    bmp1(noise1, "0-2. Lena_GaussianNoise");
    reverse_raw(noise1);

    //���� ���� �Լ� �۵� 
    edge_detection(image1, noise1, Roberts, "1-1. Roberts");
    edge_detection(image1, noise1, Prewitt, "1-2. Prewitt");
    edge_detection(image1, noise1, Sobel, "1-3. Sobel");
    edge_detection_stochastic(image1, noise1, Stochastic, "1-4. Stochastic");

    //2�� ���� �̹��� ó���� ���Ͽ� ��Ʈ ������ �о�´�. 
    FILE* input_file2 = fopen("BOAT512.raw.raw", "rb");
    if (input_file2 == NULL) {
        printf("FILE2 is ERROR\n");
        return 1;
    }
    //1�� ������ �о���� �Ͱ� ������ ���� 
    BYTE* image2 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    size_t read_bytes2 = fread(image2, sizeof(BYTE), MAX * MAX, input_file2);

    if (read_bytes2 != MAX * MAX) {
        printf("Failed to read RAW image2 file \n");
        fclose(input_file2);
        return 1;
    }
    fclose(input_file2);

    //  ��Ʈ �̹����� ���� ����þ� ������ �߰� ���� ó��
    BYTE* noise2 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    double variance2 = calculate_variance(image2);
    double stddev_noise3 = sqrt(variance2 / pow(10.0, ((double)SNR2 / 10)));
    add_gaussian_noise(image2, noise2, stddev_noise3);
    
    //2������ �ο��н� ���͸��� �޵�� ���͸��� ���� 
    lowpass_filter(image2, noise2, "2-1. Lowpass");
    median_filter(image2, noise2, "2-2. Median");

    // �޸� ����
    free(image1);
    free(noise1);
    free(image2);
    free(noise2);

    return 0;
}

//��Ʈ�� ���� ����� ������ �����ϴ� �Լ� 
void generate_headers(BITMAPHEADERS& bh) {
    // ��Ʈ�� ���� ��� ����
    bh.hFile.bfType = 0x4D42;
    bh.hFile.bfReserved1 = 0;
    bh.hFile.bfReserved2 = 0;
    bh.hFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD) * 256;
    bh.hFile.bfSize = bh.hFile.bfOffBits + MAX * MAX;
    
    // ��Ʈ�� ���� ��� ����
    bh.hInfo.biSize = 40;
    bh.hInfo.biWidth = MAX;
    bh.hInfo.biHeight = MAX;
    bh.hInfo.biPlanes = 1;
    bh.hInfo.biBitCount = 8;
    bh.hInfo.biCompression = 0;
    bh.hInfo.biSizeImage = MAX * MAX;
    bh.hInfo.biXPelsPerMeter = 0;
    bh.hInfo.biYPelsPerMeter = 0;
    bh.hInfo.biClrUsed = 0;
    bh.hInfo.biClrImportant = 0;
   
    // ����� �ش��ϴ� RGBQUAD �迭 ����, �׷��̽�����
    for (int i = 0; i < 256; i++) {
        bh.hRGB[i].rgbBlue = i;
        bh.hRGB[i].rgbGreen = i;
        bh.hRGB[i].rgbRed = i;
        bh.hRGB[i].rgbReserved = 0;
    }
}

//raw������ �������� �Լ� 
void reverse_raw(BYTE* image) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (i < MAX / 2) {
                swap(image[i * MAX + j], image[(MAX - i - 1) * MAX + j]);
            }
        }
    }
}

// Bmp������ ���� �� ���� �ϴ� �Լ�, 1�� ���� �̹����� �����ϴ� ��� 
void bmp1(BYTE* output_image, string output_name) {
    //�̹����� ������ ��ȯ 
    reverse_raw(output_image);

    //��Ʈ�� ��� ���� 
    BITMAPHEADERS bh;
    generate_headers(bh);

    //���� ��� 
    string PATH = "Output_Image/" + output_name + "_Edge_Detection(" + to_string(THRESHOLD) + ").bmp";
    
    //���� ���� 
    FILE* output_file = fopen(PATH.c_str(), "wb");
    
    // ��� �� �̹��� ������ ����
    fwrite(&bh.hFile, sizeof(BITMAPFILEHEADER), 1, output_file);
    fwrite(&bh.hInfo, sizeof(BITMAPINFOHEADER), 1, output_file);
    fwrite(bh.hRGB, sizeof(RGBQUAD), 256, output_file);
    fwrite(output_image, sizeof(BYTE), MAX * MAX, output_file);
    
    //���� �ݱ� 
    fclose(output_file);

}

//2������������ ������, bmp1�Լ��� ���� 
void bmp2(BYTE* output_image, string output_name) {
    reverse_raw(output_image);

    BITMAPHEADERS bh;
    generate_headers(bh);
    string PATH = "Output_Image/" + output_name + ".bmp";

    FILE* output_file = fopen(PATH.c_str(), "wb");

    fwrite(&bh.hFile, sizeof(BITMAPFILEHEADER), 1, output_file);
    fwrite(&bh.hInfo, sizeof(BITMAPINFOHEADER), 1, output_file);
    fwrite(bh.hRGB, sizeof(RGBQUAD), 256, output_file);

    fwrite(output_image, sizeof(BYTE), MAX * MAX, output_file);
    fclose(output_file);

}

//�̹��� �л��� ����ϴ� �Լ� 
double calculate_variance(BYTE* image) {
    double avg_1 = 0, avg_2 = 0;

    // �̹����� �� �ȼ��� ���� ���
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            int temp = image[i * MAX + j];
            avg_1 += temp; // �ȼ� ���� ���Ѵ�
            avg_2 += temp * temp; // �ȼ��� ������ ���� ��� 
        }
    }
    // �ȼ� �� ��� ��� �� ������ ����� ��� 
    avg_1 /= (double)(MAX * MAX);
    avg_2 /= (double)(MAX * MAX);
   
    //����� ���� ��ȯ ��Ų��. 
    return avg_2 - (avg_1 * avg_1);
}

//  �ñ׸��� ���� ����þ� ������ �����ϴ� �Լ�
double gaussian(double sigma) {
    static int ready = 0;
    static double gstore;
    double v1, v2, r, fac, gaus;
    int r1, r2;

    // ����þ� ���� ���� �˰���
    if (ready == 0) {
        do {
            r1 = rand();
            r2 = rand();
            v1 = 2. * ((double)r1 / (double)RAND_MAX - 0.5);
            v2 = 2. * ((double)r2 / (double)RAND_MAX - 0.5);
            r = v1 * v1 + v2 * v2;
        } while (r > 1.0);
        fac = (double)sqrt((double)(-2 * log(r) / r));
        gstore = v1 * fac;
        gaus = v2 * fac;
        ready = 1;
    }
    else {
        ready = 0;
        gaus = gstore;
    }
    return gaus * sigma;
}

// �̹����� ����þ� ������ �߰��ϴ� �Լ�
void add_gaussian_noise(BYTE* image, BYTE* noise, double sigma) {
    int pixel;

    // �̹����� �� �ȼ��� ���� ����þ� ������ �����ϰ� �߰�
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            // ����þ� ���� ���� �� ���� �ȼ� ���� ���Ѵ�.
            pixel = (int)image[i * MAX + j] + (gaussian(sigma) + 0.5);
            // �ȼ� ���� 255 �ʰ��� ��� 255�� ����
            if (pixel > 255) {
                pixel = 255;
            }
            // �ȼ� ���� 0 �̸��� ��� 0���� ����
            if (pixel < 0) {
                pixel = 0;
            }
            // ����þ� ������ �߰��� ������ ������ �̹����� ���� 
            noise[i * MAX + j] = (BYTE)pixel;
        }
    }
}
// ����ũ�� ����Ͽ� �̹����� ����ŷ�� �����ϴ� �Լ�
void masking(BYTE* masked, BYTE* image, int mask[][9]) {
    // ����ũ�� �̿� �ȼ� ��� ��ǥ
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // �̹��� ��ü�� ��ȸ�ϸ鼭 ����ŷ ����
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int avg_1 = 0;
            int avg_2 = 0;

            // ����ũ�� �̿��Ͽ� �ֺ� �ȼ��� ������ ���
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // ��� �ȼ� ó��
                if (newy < 0) {
                    newy = 0;
                }
                else if (newy >= MAX) {
                    newy = MAX - 1;
                }

                if (newx < 0) {
                    newx = 0;
                }
                else if (newx >= MAX) {
                    newx = MAX - 1;
                }
                // �ȼ� ���� ����ũ�� �����Ͽ� ������ ���
                avg_1 += image[newy * MAX + newx] * mask[0][i];
                avg_2 += image[newy * MAX + newx] * mask[1][i];
            }

            // ������ ���밪�� ���Ͽ� �Ӱ谪�� ���Ͽ� ����ŷ ����
            int val = abs(avg_1) + abs(avg_2);
            if (THRESHOLD <= val) {
                masked[y * MAX + x] = (BYTE)WHITE; //ȭ��Ʈ�� ���� 
            }
            else {
                masked[y * MAX + x] = (BYTE)BLACK; //������ ���� 
            }
        }
    }
}
//stochastic ����ũ�� ����Ͽ� �̹����� stocahstic ����ŷ�� �����ϴ� �Լ�
void masking_stochastic(BYTE* masked_image, BYTE* image, double mask[][25]) {
    // ����ũ�� �̿� �ȼ� ��� ��ǥ
    int dy[25] = { -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0,
                  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2, 2 };

    int dx[25] = { -2, -1, 0,  1,  2, -2, -1, 0,  1,  2, -2, -1, 0,
                  1,  2,  -2, -1, 0, 1,  2,  -2, -1, 0, 1,  2 };
    // �̹��� ��ü�� ��ȸ�ϸ鼭 ����ŷ ����
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            double avg_1 = 0.0;
            double avg_2 = 0.0;
            
            //stochastic�� �̿��ؼ� �ֺ� �ȼ��� ������ ���
            for (int i = 0; i < 25; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // ��� �ȼ� ó��
                if (newy < 0) {
                    newy = 0;
                }
                else if (newy >= MAX) {
                    newy = MAX - 1;
                }

                if (newx < 0) {
                    newx = 0;
                }
                else if (newx >= MAX) {
                    newx = MAX - 1;
                }
                // �ȼ� ���� stochastic�� �����Ͽ� ������ ���
                avg_1 += (double)image[newy * MAX + newx] * mask[0][i];
                avg_2 += (double)image[newy * MAX + newx] * mask[1][i];
            }
            // ������ ���밪�� ���Ͽ� �Ӱ谪�� ���Ͽ� ����ŷ ����
            double val = abs(avg_1) + abs(avg_2);
            if (THRESHOLD <= val) {
                masked_image[y * MAX + x] = (BYTE)WHITE; //ȭ��Ʈ
            }
            else {
                masked_image[y * MAX + x] = (BYTE)BLACK; //��
            }
        }
    }
}

// ���� �̹����� ������ �̹����� ���� ������ ����ϴ� �Լ�
double calculate_error_rate(BYTE* masked_original, BYTE* masked_noise) {
    double n0 = 0;
    double n1 = 0;
    // �̹��� ��ü�� ��ȸ�ϸ鼭 ���� �ȼ� ������ ���
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (masked_original[i * MAX + j] == WHITE) {
                n0++; // �������� �̹����� ��� �ȼ� ���� ����
            }

            if (masked_original[i * MAX + j] != masked_noise[i * MAX + j]) {
                n1++; // �������� �̹����� ������ �̹����� �ȼ��� �ٸ� ��� ���� �ȼ� ���� ����
            }
        }
    }
    // ���� ������ ����Ͽ� ��ȯ
    return n1 / n0;
}

//����ũ�� ����Ͽ� ���� ������ �����ϴ� �Լ� 
void edge_detection(BYTE* image, BYTE* noise, int mask[][9], string output_name) {

    // �������� �̹��� ���� ����
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking(masked_original, image, mask);

    // ���� �̹��� ���� ����
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking(masked_noise, noise, mask);

    // ���� ���� ���
    printf("%s Edge Detection Error Rate : %lf\n", output_name.c_str(), calculate_error_rate(masked_original, masked_noise));
    
    // BMP ���Ϸ� ����
    bmp1(masked_original, output_name + "_Original");
    bmp1(masked_noise, output_name);
    // �޸� ����
    free(masked_noise);
    free(masked_original);
}

//Stochastic�� ����Ͽ� Stochastic ���� ���� �����ϴ� �Լ� 
void edge_detection_stochastic(BYTE* image, BYTE* noise, double mask[][25], string output_name) {

    // �������� �̹��� ���� ����
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking_stochastic(masked_original, image, mask);

    // ���� �̹��� ���� ����
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking_stochastic(masked_noise, noise, mask);

    // ���� ���� ���
    printf("%s Edge Detection Error Rate : %lf\n", output_name.c_str(), calculate_error_rate(masked_original, masked_noise));
    // BMP ���Ϸ� ����
    bmp1(masked_original, output_name + "_Original");
    bmp1(masked_noise, output_name);

    // �޸� ����
    free(masked_noise);
    free(masked_original);

}

//MSE�� ��� ���ִ� �Լ�, �� �̹������� ��� ���� ������ ���
int calculate_MSE(BYTE* masked_image, BYTE* masked_noise) {
    long long int mse = 0;
    // �̹��� ��ü�� ��ȸ�ϸ鼭 MSE ���
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            mse += pow((masked_image[i * MAX + j] - masked_noise[i * MAX + j]), 2);
        }
    }
    // �̹��� �ȼ� ���� ������ MSE ���� ��ȯ
    return (int)(mse / (MAX * MAX));
}

// Lowpass ���͸��� ���� ����ũ�� �ϴ� �Լ�
void lowpass_masking(BYTE* masked_image, BYTE* image, double mask[9]) {
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // �̹��� ��ü�� ��ȸ�ϸ鼭 Lowpass ����
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int avg = 0;
            // ����ũ�� ����Ͽ� �ֺ� �ȼ��� ���� ��� ���
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // ��� ó��
                if (newy < 0) {
                    newy = 0;
                }
                else if (newy >= MAX) {
                    newy = MAX - 1;
                }

                if (newx < 0) {
                    newx = 0;
                }
                else if (newx >= MAX) {
                    newx = MAX - 1;
                }
                avg += image[newy * MAX + newx] * mask[i];
            }
            // ��� ���� ����ŷ�� �̹����� ����
            masked_image[y * MAX + x] = (BYTE)avg;
        }
    }
}

// Lowpass_fiter�� �����ϴ� �Լ�
void lowpass_filter(BYTE* image, BYTE* noise, string output_name) {
    double Lowpass[9] = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
                         1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };

    // �������� �̹����� Lowpass ����
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    lowpass_masking(masked_original, image, Lowpass);

    // ������ �̹����� Lowpass ����
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    lowpass_masking(masked_noise, noise, Lowpass);

    //����� MSE�� ����Ѵ�
    printf("%s MSE : %d\n", output_name.c_str(), calculate_MSE(masked_original, masked_noise));
    
    //BMP ���� ���� 
    bmp2(masked_original, output_name + "_Original");
    bmp2(masked_noise, output_name);
    
    //�޸� ����
    free(masked_noise);
    free(masked_original);
}

// Median ���͸��� �����ϱ����� �̹��� �����ϴ� �Լ� 
void median_masking(BYTE* masked, BYTE* image) {
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // �̹��� ��ü�� ��ȸ�ϸ鼭 �߾Ӱ� ���͸� ����
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int arr[9] = {};

            // �ֺ� �ȼ� ���� �迭�� ����
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];
                // ��� ó��
                if (newy < 0) {
                    newy = 0;
                }
                else if (newy >= MAX) {
                    newy = MAX - 1;
                }

                if (newx < 0) {
                    newx = 0;
                }
                else if (newx >= MAX) {
                    newx = MAX - 1;
                }
                arr[i] = image[newy * MAX + newx];
            }
            // �迭�� �����Ͽ� Median ���� ����ŷ�� �̹����� ����
            sort(arr, arr + 9);
            masked[y * MAX + x] = (BYTE)arr[4];
        }
    }
}

//Median_Filter�� �����ϴ� �Լ� 
void median_filter(BYTE* image, BYTE* noise, string output_name) {
    
    //�������� �̹����� Median �� ���� 
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    median_masking(masked_original, image);

    //������ �̹����� Median �� ���� 
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    median_masking(masked_noise, noise);
    
    //mse ���
    printf("%s MSE : %d\n", output_name.c_str(), calculate_MSE(masked_original, masked_noise));
    
    //bmp ���� 
    bmp2(masked_original, output_name + "_Original");
    bmp2(masked_noise, output_name);
    
    //�޸� ���� 
    free(masked_noise);
    free(masked_original);
}