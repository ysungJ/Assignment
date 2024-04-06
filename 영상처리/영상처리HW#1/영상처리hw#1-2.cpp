//����ó�� 1-2�� ���� 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 512
#define HEIGHT 512

int main(void) {
    // �Է� RAW �̹��� ���� ����
    FILE* rawFile = fopen("1������RAMP�̹���.raw", "rb");
    if (rawFile == NULL) {
        printf("RAW �̹��� ������ �� �� �����ϴ�.\n");
        return 1;
    }

    // BMP ���� ��� ���� ����
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
    bih.biBitCount = 8; // 8-bit �̹���
    bih.biCompression = 0; // �����
    bih.biSizeImage = WIDTH * HEIGHT;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 256;
    bih.biClrImportant = 0;

    // BMP ���� ���� �� ��� ���� ����
    FILE* bmpFile = fopen("1������RAW�̹�����BMP�κ�ȯ2�������̹���.bmp", "wb");
    if (bmpFile == NULL) {
        printf("BMP ������ ������ �� �����ϴ�.\n");
        fclose(rawFile);
        return 1;
    }
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, bmpFile);
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, bmpFile);

    // �ȷ�Ʈ ���� ���� (�׷��̽�����)
    RGBQUAD palette[256];
    for (int i = 0; i < 256; i++) {
        palette[i].rgbBlue = i;
        palette[i].rgbGreen = i;
        palette[i].rgbRed = i;
        palette[i].rgbReserved = 0;
    }
    fwrite(palette, sizeof(RGBQUAD), 256, bmpFile);

    // raw�̹����� ����� bmp���Ϸ� ����ϱ�
    uint8_t* imageData = (uint8_t*)malloc(WIDTH * HEIGHT);
    fread(imageData, sizeof(uint8_t), WIDTH * HEIGHT, rawFile);
    for (int y = HEIGHT - 1; y >= 0; y--) {
        fwrite(&imageData[y * WIDTH], sizeof(uint8_t), WIDTH, bmpFile);
    }

    // ���� �ݱ� �� �޸� ����
    fclose(rawFile);
    fclose(bmpFile);
    free(imageData);

    printf("BMP ������ ���������� �����Ǿ����ϴ�.\n");

    return 0;
}