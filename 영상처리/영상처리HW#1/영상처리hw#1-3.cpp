//영상처리 1-3번 문제 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTHBYTES(bits)    ( ((bits) + 31) / 32 * 4 )
#define BYTE unsigned char

int main(void) {

    FILE* infile;
    infile = fopen("1번문제RAW이미지를BMP로변환2번문제이미지.bmp", "rb");

    if (infile == NULL)
    {
        printf("영상파일이 없음 ");
        return 1;
    }

    // BMP 파일 헤더 정보 입력
    BITMAPFILEHEADER hf;
    BITMAPINFOHEADER hInfo;

    fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
    if (hf.bfType != 0x4D42) exit(1);

    fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
    if (hInfo.biBitCount != 8)
    {
        printf("Bad File fromat!");
        return 1;
    }

    // 팔레트 정보의 입력
    RGBQUAD hRGB[256];
    fread(hRGB, sizeof(RGBQUAD), 256, infile);

    // 메모리 할당
    BYTE* lpImg = new BYTE[hInfo.biSizeImage];
    fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
    fclose(infile);

    int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
    int newWidth = hInfo.biHeight; // 새로운 이미지의 너비는 원본 이미지의 높이와 같습니다.
    int newHeight = hInfo.biWidth; // 새로운 이미지의 높이는 원본 이미지의 너비와 같습니다.

    // 새로운 이미지 배열을 할당
    BYTE* rotatedImg = new BYTE[hInfo.biSizeImage];

    // 이미지를 90도 시계방향으로 회전하고 뒤집습니다.
    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            rotatedImg[i * rwsize + j] = lpImg[(newWidth - j - 1) * rwsize + i];
        }
    }

    // 회전된 이미지를 새로운 BMP 파일로 저장
    FILE* outfile = fopen("3번문제이미지.bmp", "wb");
    fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
    fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
    fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
    fwrite(rotatedImg, sizeof(char), hInfo.biSizeImage, outfile);
    fclose(outfile);
    printf("3번문제이 이미지가 정상적으로 출력되었습니다 \n");
    // 메모리 해제
    delete[] lpImg;
    delete[] rotatedImg;

    return 0;
}