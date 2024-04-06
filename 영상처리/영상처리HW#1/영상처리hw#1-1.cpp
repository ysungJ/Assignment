// ����ó�� ���� 1-1��
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

//����ũ�⸦ �����Ѵ� 
// 512*512�����̱⶧���� �̸� �������ش�. 
#define WIDTH 512
#define HEIGHT 512

//�̹����� ramp���� ä���ִ� �Լ� ���� 
void Ramp(uint8_t** image);
    
//���� �Լ� 
int main(void) {
    //�޸𸮿� �̹����� �Ҵ��Ѵ�.
    uint8_t** image = (uint8_t**)malloc(HEIGHT * sizeof(uint8_t*));
    if (image == NULL) { //���࿡ �迭 "��ü"�� �Ҵ� �� �ϴ� ���
        printf("�迭�� �޸𸮿� �������� ���Ͽ� ���α׷��� �����Ͽ����ϴ�.\n");
        return 1;
    }
    for (int i = 0; i < HEIGHT; i++) {
        image[i] = (uint8_t*)malloc(WIDTH * sizeof(uint8_t));
        if (image[i] == NULL) { //�����Ͱ� �Ҵ�� �޸𸮸� ����Ű�� ���ϴ� ���
            printf("�����Ͱ� �Ҵ�� �޸𸮸� ����Ű�� ���Ͽ� �����Ͽ����ϴ�.\n");
            return 1;
        }
    }
    //�����Լ��� �ҷ��ͼ� �̹����� �����.
    Ramp(image);

    // ������� ramp������ �����Ѵ�.
    FILE* file = fopen("1������RAMP�̹���.raw", "wb");
    if (file != NULL) {
        for (int y = 0; y < HEIGHT; y++) {
            fwrite(image[y], sizeof(uint8_t), WIDTH, file);
        }
        fclose(file);
        printf("RAW �̹����� ���������� ����������ϴ�.\n");
    }
    else {
        printf("�̹��� ������ ������ �� �߽��ϴ�.\n �ý����� �����մϴ�.\n");
    }

    // �Ҵ�� �޸𸮸� ���ش� 
    for (int i = 0; i < HEIGHT; i++) {
        free(image[i]);
    }
    free(image);

    return 0;
}

//Ramp �Լ��� ����
void Ramp(uint8_t** image) {
    // �������� �׷���ó�� �� ������ �°� ramp���� �Ҵ� ���ش�.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // x�� �� n�� ���� 100�� ��, y�� r(n)�� ���� 120���� �����Ǿ���Ѵ�. 
            if (x <= 100) {
                image[y][x] = 120;
            }
            else if (x > 100 && x <= 200) { // x�� �� n�� ���� 100~200���� �� ��, y�� r(n)�� ���� 135���� �����Ǿ���Ѵ�. 
                image[y][x] = 120 + (uint8_t)((x - 100) * (135 - 120) / 100);
            }
            else if ( x > 200 && x <= 280) { // x�� �� n�� ���� 200 ~ 280���� �� ��, y�� r(n)�� ���� 225���� �����Ǿ���Ѵ�.
                image[y][x] = 135 + (uint8_t)((x - 200) * (225 - 135) / 80);
            }
            else if (x > 280 && x <= 300) { // x�� �� n�� ���� 280~300�� �� ��, y�� r(n)�� ���� 240���� �����Ǿ���Ѵ�.
                image[y][x] = 225 + (uint8_t)((x - 280) * (240 - 225) / 20);
            }
            else { // x�� �� n�� ���� 300~512���� �� ��, y�� r(n)�� ���� 240���� �����Ǿ���Ѵ�.
                image[y][x] = 240;
            }
        }
    }
}