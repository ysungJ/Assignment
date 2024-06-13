// 영상처리 과제 3번 
#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <windows.h>

#define MAX 512
#define SNR 9.0 // 3-1번 문제의 SNR
#define WHITE 255
#define BLACK 0
#define THRESHOLD 150
#define SNR2 8.0 //3-2번 문제는 SNR이 8.0
using namespace std;

//구조체 선언 
typedef struct headers {
    BITMAPFILEHEADER hFile;
    BITMAPINFOHEADER hInfo;
    RGBQUAD hRGB[256];
} BITMAPHEADERS;

//함수 선언
//공통으로 사용할 함수  
void generate_headers(BITMAPHEADERS& bh); //비트맵 파일 헤더와 정보를 생성하는 함수 
void reverse_raw(BYTE* image); //raw파일을 뒤집히는 함수 
void bmp1(BYTE* output_image, string output_name); // Bmp파일을 생성 및 저장 하는 함수, 1번 문제 이미지를 저장하는 경로
void bmp2(BYTE* output_image, string output_name); //2번문제를위한 저장경로, bmp1함수랑 같음 
double calculate_variance(BYTE* image); //이미지 분산을 계산하는 함수 
double gaussian(double sigma); //  시그마를 갖는 가우시안 잡음을 생성하는 함수
void add_gaussian_noise(BYTE* image, BYTE* noise, double sigma); // 이미지에 가우시안 잡음을 추가하는 함수
//3-1번문제에 필요한 함수 
void masking(BYTE* masked, BYTE* image, int mask[][9]); // 마스크를 사용하여 이미지에 마스킹을 수행하는 함수
void masking_stochastic(BYTE* masked_image, BYTE* image, double mask[][25]); //stochastic 마스크를 사용하여 이미지에 stocahstic 마스킹을 수행하는 함수
double calculate_error_rate(BYTE* masked_original, BYTE* masked_noise); // 원본 이미지와 노이즈 이미지의 에러 비율을 계산하는 함수
void edge_detection(BYTE* image, BYTE* noise, int mask[][9], string output_name); //마스크를 사용하여 엣지 검출을 수행하는 함수 
void edge_detection_stochastic(BYTE* image, BYTE* noise, double mask[][25], string output_name); //Stochastic을 사용하여 Stochastic 엣지 검출 수행하는 함수 
//3-2번문제에 필요한 함수
int calculate_MSE(BYTE* masked_image, BYTE* masked_noise); //MSE를 계산 해주는 함수
void lowpass_masking(BYTE* masked_image, BYTE* image, double mask[9]); // Lowpass 필터링을 위해 마스크를 하는 함수
void lowpass_filter(BYTE* image, BYTE* noise, string output_name); // Lowpass_fiter를 수행하는 함수
void median_masking(BYTE* masked, BYTE* image); // Median 필터링을 수행하기 위해 이미지 생성하는 함수 
void median_filter(BYTE* image, BYTE* noise, string output_name); //Median_Filter를 수행하는 함수 

//메인함수 
int main(void) {

    //난수 초기화
    srand((unsigned int)time(NULL));

    //엣지 검출에 사용할 마스크 행렬 
    //G_Y ≡ G_X^T이 성립 
    //로버츠 
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
    //G_Y ≡ G_X^T이 성립 
    //프리윗
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
    //G_Y ≡ G_X^T이 성립 
    //소벨
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

    //G_Y ≡ G_X^T이 성립 
    //스토캐스틱
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

    //1번 문제 이미지 처리를 위하여 레나 사진을 읽어온다. 
    FILE* input_file1 = fopen("lena_raw_512x512.raw", "rb");
    if (input_file1 == NULL) {
        printf("FILE1 is ERROR\n"); //파일 없으면 프로그램 종료
        return 1;
    }

    BYTE* image1 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);

    size_t read_bytes1 = fread(image1, sizeof(BYTE), MAX * MAX, input_file1);
    if (read_bytes1 != MAX * MAX) {
        printf("Failed to read RAW image1 file \n");
        fclose(input_file1); // 파일을 닫고 프로그램을 종료
        return 1;
    }

    fclose(input_file1);

    // 1번 레나 이미지에 가우시안 잡음 추가
    BYTE* noise1 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    double variance = calculate_variance(image1);
    double stddev_noise = sqrt(variance / pow(10.0, ((double)SNR / 10)));
    add_gaussian_noise(image1, noise1, stddev_noise);
    
    //1번 문제 대한 결과를 저장한다. 
    bmp1(image1, "0-1. Lena_Original");
    reverse_raw(image1);
    // 가우시안 이미지가 추가된 이미지를 저장 
    bmp1(noise1, "0-2. Lena_GaussianNoise");
    reverse_raw(noise1);

    //엣지 검출 함수 작동 
    edge_detection(image1, noise1, Roberts, "1-1. Roberts");
    edge_detection(image1, noise1, Prewitt, "1-2. Prewitt");
    edge_detection(image1, noise1, Sobel, "1-3. Sobel");
    edge_detection_stochastic(image1, noise1, Stochastic, "1-4. Stochastic");

    //2번 문제 이미지 처리를 위하여 보트 사진을 읽어온다. 
    FILE* input_file2 = fopen("BOAT512.raw.raw", "rb");
    if (input_file2 == NULL) {
        printf("FILE2 is ERROR\n");
        return 1;
    }
    //1번 문제를 읽어오는 것과 형식이 같음 
    BYTE* image2 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    size_t read_bytes2 = fread(image2, sizeof(BYTE), MAX * MAX, input_file2);

    if (read_bytes2 != MAX * MAX) {
        printf("Failed to read RAW image2 file \n");
        fclose(input_file2);
        return 1;
    }
    fclose(input_file2);

    //  보트 이미지에 대한 가우시안 노이즈 추가 등의 처리
    BYTE* noise2 = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    double variance2 = calculate_variance(image2);
    double stddev_noise3 = sqrt(variance2 / pow(10.0, ((double)SNR2 / 10)));
    add_gaussian_noise(image2, noise2, stddev_noise3);
    
    //2번문제 로우패스 필터링과 메디안 필터링을 수행 
    lowpass_filter(image2, noise2, "2-1. Lowpass");
    median_filter(image2, noise2, "2-2. Median");

    // 메모리 해제
    free(image1);
    free(noise1);
    free(image2);
    free(noise2);

    return 0;
}

//비트맵 파일 헤더와 정보를 생성하는 함수 
void generate_headers(BITMAPHEADERS& bh) {
    // 비트맵 파일 헤더 설정
    bh.hFile.bfType = 0x4D42;
    bh.hFile.bfReserved1 = 0;
    bh.hFile.bfReserved2 = 0;
    bh.hFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD) * 256;
    bh.hFile.bfSize = bh.hFile.bfOffBits + MAX * MAX;
    
    // 비트맵 정보 헤더 설정
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
   
    // 헤더에 해당하는 RGBQUAD 배열 설정, 그레이스케일
    for (int i = 0; i < 256; i++) {
        bh.hRGB[i].rgbBlue = i;
        bh.hRGB[i].rgbGreen = i;
        bh.hRGB[i].rgbRed = i;
        bh.hRGB[i].rgbReserved = 0;
    }
}

//raw파일을 뒤집히는 함수 
void reverse_raw(BYTE* image) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (i < MAX / 2) {
                swap(image[i * MAX + j], image[(MAX - i - 1) * MAX + j]);
            }
        }
    }
}

// Bmp파일을 생성 및 저장 하는 함수, 1번 문제 이미지를 저장하는 경로 
void bmp1(BYTE* output_image, string output_name) {
    //이미지를 역으로 반환 
    reverse_raw(output_image);

    //비트맵 헤더 생성 
    BITMAPHEADERS bh;
    generate_headers(bh);

    //저장 경로 
    string PATH = "Output_Image/" + output_name + "_Edge_Detection(" + to_string(THRESHOLD) + ").bmp";
    
    //파일 열기 
    FILE* output_file = fopen(PATH.c_str(), "wb");
    
    // 헤더 및 이미지 데이터 쓰기
    fwrite(&bh.hFile, sizeof(BITMAPFILEHEADER), 1, output_file);
    fwrite(&bh.hInfo, sizeof(BITMAPINFOHEADER), 1, output_file);
    fwrite(bh.hRGB, sizeof(RGBQUAD), 256, output_file);
    fwrite(output_image, sizeof(BYTE), MAX * MAX, output_file);
    
    //파일 닫기 
    fclose(output_file);

}

//2번문제를위한 저장경로, bmp1함수랑 같음 
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

//이미지 분산을 계산하는 함수 
double calculate_variance(BYTE* image) {
    double avg_1 = 0, avg_2 = 0;

    // 이미지의 각 픽셀에 대해 계산
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            int temp = image[i * MAX + j];
            avg_1 += temp; // 픽셀 값을 합한다
            avg_2 += temp * temp; // 픽셀의 제곱의 합을 계산 
        }
    }
    // 픽셀 값 평균 계산 및 제곱의 평균을 계산 
    avg_1 /= (double)(MAX * MAX);
    avg_2 /= (double)(MAX * MAX);
   
    //계산한 값을 반환 시킨다. 
    return avg_2 - (avg_1 * avg_1);
}

//  시그마를 갖는 가우시안 잡음을 생성하는 함수
double gaussian(double sigma) {
    static int ready = 0;
    static double gstore;
    double v1, v2, r, fac, gaus;
    int r1, r2;

    // 가우시안 분포 생성 알고리즘
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

// 이미지에 가우시안 잡음을 추가하는 함수
void add_gaussian_noise(BYTE* image, BYTE* noise, double sigma) {
    int pixel;

    // 이미지의 각 픽셀에 대해 가우시안 잡음을 생성하고 추가
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            // 가우시안 잡음 생성 후 현재 픽셀 값에 더한다.
            pixel = (int)image[i * MAX + j] + (gaussian(sigma) + 0.5);
            // 픽셀 값이 255 초과일 경우 255로 설정
            if (pixel > 255) {
                pixel = 255;
            }
            // 픽셀 값이 0 미만일 경우 0으로 설정
            if (pixel < 0) {
                pixel = 0;
            }
            // 가우시안 잡음이 추가된 값으로 노이즈 이미지로 갱신 
            noise[i * MAX + j] = (BYTE)pixel;
        }
    }
}
// 마스크를 사용하여 이미지에 마스킹을 수행하는 함수
void masking(BYTE* masked, BYTE* image, int mask[][9]) {
    // 마스크의 이웃 픽셀 상대 좌표
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // 이미지 전체를 순회하면서 마스킹 수행
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int avg_1 = 0;
            int avg_2 = 0;

            // 마스크를 이용하여 주변 픽셀을 가중합 계산
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // 경계 픽셀 처리
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
                // 픽셀 값에 마스크를 적용하여 가중합 계산
                avg_1 += image[newy * MAX + newx] * mask[0][i];
                avg_2 += image[newy * MAX + newx] * mask[1][i];
            }

            // 가중합 절대값을 구하여 임계값과 비교하여 마스킹 수행
            int val = abs(avg_1) + abs(avg_2);
            if (THRESHOLD <= val) {
                masked[y * MAX + x] = (BYTE)WHITE; //화이트로 설정 
            }
            else {
                masked[y * MAX + x] = (BYTE)BLACK; //블랙으로 설정 
            }
        }
    }
}
//stochastic 마스크를 사용하여 이미지에 stocahstic 마스킹을 수행하는 함수
void masking_stochastic(BYTE* masked_image, BYTE* image, double mask[][25]) {
    // 마스크의 이웃 픽셀 상대 좌표
    int dy[25] = { -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0,
                  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2, 2 };

    int dx[25] = { -2, -1, 0,  1,  2, -2, -1, 0,  1,  2, -2, -1, 0,
                  1,  2,  -2, -1, 0, 1,  2,  -2, -1, 0, 1,  2 };
    // 이미지 전체를 순회하면서 마스킹 수행
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            double avg_1 = 0.0;
            double avg_2 = 0.0;
            
            //stochastic을 이용해서 주변 픽셀을 가중합 계산
            for (int i = 0; i < 25; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // 경계 픽셀 처리
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
                // 픽셀 값에 stochastic을 적용하여 가중합 계산
                avg_1 += (double)image[newy * MAX + newx] * mask[0][i];
                avg_2 += (double)image[newy * MAX + newx] * mask[1][i];
            }
            // 가중합 절대값을 구하여 임계값과 비교하여 마스킹 수행
            double val = abs(avg_1) + abs(avg_2);
            if (THRESHOLD <= val) {
                masked_image[y * MAX + x] = (BYTE)WHITE; //화이트
            }
            else {
                masked_image[y * MAX + x] = (BYTE)BLACK; //블랙
            }
        }
    }
}

// 원본 이미지와 노이즈 이미지의 에러 비율을 계산하는 함수
double calculate_error_rate(BYTE* masked_original, BYTE* masked_noise) {
    double n0 = 0;
    double n1 = 0;
    // 이미지 전체를 순회하면서 에러 픽셀 개수를 계산
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (masked_original[i * MAX + j] == WHITE) {
                n0++; // 오리지널 이미지의 흰색 픽셀 개수 증가
            }

            if (masked_original[i * MAX + j] != masked_noise[i * MAX + j]) {
                n1++; // 오리지널 이미지와 노이즈 이미지의 픽셀이 다른 경우 에러 픽셀 개수 증가
            }
        }
    }
    // 에러 비율을 계산하여 반환
    return n1 / n0;
}

//마스크를 사용하여 엣지 검출을 수행하는 함수 
void edge_detection(BYTE* image, BYTE* noise, int mask[][9], string output_name) {

    // 오리지널 이미지 엣지 검출
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking(masked_original, image, mask);

    // 잡음 이미지 엣지 검출
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking(masked_noise, noise, mask);

    // 에러 비율 출력
    printf("%s Edge Detection Error Rate : %lf\n", output_name.c_str(), calculate_error_rate(masked_original, masked_noise));
    
    // BMP 파일로 저장
    bmp1(masked_original, output_name + "_Original");
    bmp1(masked_noise, output_name);
    // 메모리 해제
    free(masked_noise);
    free(masked_original);
}

//Stochastic을 사용하여 Stochastic 엣지 검출 수행하는 함수 
void edge_detection_stochastic(BYTE* image, BYTE* noise, double mask[][25], string output_name) {

    // 오리지널 이미지 엣지 검출
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking_stochastic(masked_original, image, mask);

    // 잡음 이미지 엣지 검출
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    masking_stochastic(masked_noise, noise, mask);

    // 에러 비율 출력
    printf("%s Edge Detection Error Rate : %lf\n", output_name.c_str(), calculate_error_rate(masked_original, masked_noise));
    // BMP 파일로 저장
    bmp1(masked_original, output_name + "_Original");
    bmp1(masked_noise, output_name);

    // 메모리 해제
    free(masked_noise);
    free(masked_original);

}

//MSE를 계산 해주는 함수, 두 이미지에서 평균 제곱 오차를 계산
int calculate_MSE(BYTE* masked_image, BYTE* masked_noise) {
    long long int mse = 0;
    // 이미지 전체를 순회하면서 MSE 계산
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            mse += pow((masked_image[i * MAX + j] - masked_noise[i * MAX + j]), 2);
        }
    }
    // 이미지 픽셀 수로 나누어 MSE 값을 반환
    return (int)(mse / (MAX * MAX));
}

// Lowpass 필터링을 위해 마스크를 하는 함수
void lowpass_masking(BYTE* masked_image, BYTE* image, double mask[9]) {
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // 이미지 전체를 순회하면서 Lowpass 적용
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int avg = 0;
            // 마스크를 사용하여 주변 픽셀의 가중 평균 계산
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];

                // 경계 처리
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
            // 결과 값을 마스킹된 이미지에 저장
            masked_image[y * MAX + x] = (BYTE)avg;
        }
    }
}

// Lowpass_fiter를 수행하는 함수
void lowpass_filter(BYTE* image, BYTE* noise, string output_name) {
    double Lowpass[9] = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
                         1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };

    // 오리지널 이미지에 Lowpass 적용
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    lowpass_masking(masked_original, image, Lowpass);

    // 노이즈 이미지에 Lowpass 적용
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    lowpass_masking(masked_noise, noise, Lowpass);

    //계산한 MSE를 출력한다
    printf("%s MSE : %d\n", output_name.c_str(), calculate_MSE(masked_original, masked_noise));
    
    //BMP 파일 저장 
    bmp2(masked_original, output_name + "_Original");
    bmp2(masked_noise, output_name);
    
    //메모리 해제
    free(masked_noise);
    free(masked_original);
}

// Median 필터링을 수행하기위에 이미지 생성하는 함수 
void median_masking(BYTE* masked, BYTE* image) {
    int dy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    // 이미지 전체를 순회하면서 중앙값 필터링 적용
    for (int y = 0; y < MAX; y++) {
        for (int x = 0; x < MAX; x++) {
            int arr[9] = {};

            // 주변 픽셀 값을 배열에 저장
            for (int i = 0; i < 9; i++) {
                int newy = y + dy[i];
                int newx = x + dx[i];
                // 경계 처리
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
            // 배열을 정렬하여 Median 값을 마스킹된 이미지에 저장
            sort(arr, arr + 9);
            masked[y * MAX + x] = (BYTE)arr[4];
        }
    }
}

//Median_Filter를 수행하는 함수 
void median_filter(BYTE* image, BYTE* noise, string output_name) {
    
    //오리지널 이미지에 Median 값 적용 
    BYTE* masked_original = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    median_masking(masked_original, image);

    //노이즈 이미지에 Median 값 적용 
    BYTE* masked_noise = (BYTE*)malloc(sizeof(BYTE) * MAX * MAX);
    median_masking(masked_noise, noise);
    
    //mse 출력
    printf("%s MSE : %d\n", output_name.c_str(), calculate_MSE(masked_original, masked_noise));
    
    //bmp 저장 
    bmp2(masked_original, output_name + "_Original");
    bmp2(masked_noise, output_name);
    
    //메모리 해제 
    free(masked_noise);
    free(masked_original);
}