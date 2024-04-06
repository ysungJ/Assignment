//����ó�� 1-3�� ���� 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTHBYTES(bits)    ( ((bits) + 31) / 32 * 4 )
#define BYTE unsigned char

int main(void) {

    FILE* infile;
    infile = fopen("1������RAW�̹�����BMP�κ�ȯ2�������̹���.bmp", "rb");

    if (infile == NULL)
    {
        printf("���������� ���� ");
        return 1;
    }

    // BMP ���� ��� ���� �Է�
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

    // �ȷ�Ʈ ������ �Է�
    RGBQUAD hRGB[256];
    fread(hRGB, sizeof(RGBQUAD), 256, infile);

    // �޸� �Ҵ�
    BYTE* lpImg = new BYTE[hInfo.biSizeImage];
    fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
    fclose(infile);

    int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
    int newWidth = hInfo.biHeight; // ���ο� �̹����� �ʺ�� ���� �̹����� ���̿� �����ϴ�.
    int newHeight = hInfo.biWidth; // ���ο� �̹����� ���̴� ���� �̹����� �ʺ�� �����ϴ�.

    // ���ο� �̹��� �迭�� �Ҵ�
    BYTE* rotatedImg = new BYTE[hInfo.biSizeImage];

    // �̹����� 90�� �ð�������� ȸ���ϰ� �������ϴ�.
    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            rotatedImg[i * rwsize + j] = lpImg[(newWidth - j - 1) * rwsize + i];
        }
    }

    // ȸ���� �̹����� ���ο� BMP ���Ϸ� ����
    FILE* outfile = fopen("3�������̹���.bmp", "wb");
    fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
    fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
    fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
    fwrite(rotatedImg, sizeof(char), hInfo.biSizeImage, outfile);
    fclose(outfile);
    printf("3�������� �̹����� ���������� ��µǾ����ϴ� \n");
    // �޸� ����
    delete[] lpImg;
    delete[] rotatedImg;

    return 0;
}