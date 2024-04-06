//영상처리 1-2번 문제 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 512
#define HEIGHT 512

int main(void) {
    // 입력 RAW 이미지 파일 열기
    FILE* rawFile = fopen("1번문제RAMP이미지.raw", "rb");
    if (rawFile == NULL) {
        printf("RAW 이미지 파일을 열 수 없습니다.\n");
        return 1;
    }

    // BMP 파일 헤더 정보 설정
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    bfh.bfType = 0x4D42; // "BM"
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + WIDTH * HEIGHT;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = WIDTH;
    bih.biHeight = HEIGHT;
    bih.biPlanes = 1;
    bih.biBitCount = 8; // 8-bit 이미지
    bih.biCompression = 0; // 비압축
    bih.biSizeImage = WIDTH * HEIGHT;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 256;
    bih.biClrImportant = 0;

    // BMP 파일 생성 및 헤더 정보 쓰기
    FILE* bmpFile = fopen("1번문제RAW이미지를BMP로변환2번문제이미지.bmp", "wb");
    if (bmpFile == NULL) {
        printf("BMP 파일을 생성할 수 없습니다.\n");
        fclose(rawFile);
        return 1;
    }
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, bmpFile);
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, bmpFile);

    // 팔레트 정보 설정 (그레이스케일)
    RGBQUAD palette[256];
    for (int i = 0; i < 256; i++) {
        palette[i].rgbBlue = i;
        palette[i].rgbGreen = i;
        palette[i].rgbRed = i;
        palette[i].rgbReserved = 0;
    }
    fwrite(palette, sizeof(RGBQUAD), 256, bmpFile);

    // raw이미지를 뒤집어서 bmp파일로 출력하기
    uint8_t* imageData = (uint8_t*)malloc(WIDTH * HEIGHT);
    fread(imageData, sizeof(uint8_t), WIDTH * HEIGHT, rawFile);
    for (int y = HEIGHT - 1; y >= 0; y--) {
        fwrite(&imageData[y * WIDTH], sizeof(uint8_t), WIDTH, bmpFile);
    }

    // 파일 닫기 및 메모리 해제
    fclose(rawFile);
    fclose(bmpFile);
    free(imageData);

    printf("BMP 파일이 성공적으로 생성되었습니다.\n");

    return 0;
}