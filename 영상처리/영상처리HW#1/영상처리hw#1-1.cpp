// 영상처리 과제 1-1번
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

//사진크기를 정의한다 
// 512*512사진이기때문에 미리 정의해준다. 
#define WIDTH 512
#define HEIGHT 512

//이미지에 ramp값을 채워주는 함수 선언 
void Ramp(uint8_t** image);
    
//메인 함수 
int main(void) {
    //메모리에 이미지를 할당한다.
    uint8_t** image = (uint8_t**)malloc(HEIGHT * sizeof(uint8_t*));
    if (image == NULL) { //만약에 배열 "자체"를 할당 못 하는 경우
        printf("배열이 메모리에 적재하지 못하여 프로그램이 실패하였습니다.\n");
        return 1;
    }
    for (int i = 0; i < HEIGHT; i++) {
        image[i] = (uint8_t*)malloc(WIDTH * sizeof(uint8_t));
        if (image[i] == NULL) { //포인터가 할당된 메모리를 가르키지 못하는 경우
            printf("포인터가 할당된 메모리를 가르키지 못하여 실패하였습니다.\n");
            return 1;
        }
    }
    //램프함수를 불러와서 이미지를 만든다.
    Ramp(image);

    // 만들어진 ramp파일을 저장한다.
    FILE* file = fopen("1번문제RAMP이미지.raw", "wb");
    if (file != NULL) {
        for (int y = 0; y < HEIGHT; y++) {
            fwrite(image[y], sizeof(uint8_t), WIDTH, file);
        }
        fclose(file);
        printf("RAW 이미지가 성공적으로 만들어졌습니다.\n");
    }
    else {
        printf("이미지 파일을 만들지 못 했습니다.\n 시스템을 종료합니다.\n");
    }

    // 할당된 메모리를 빼준다 
    for (int i = 0; i < HEIGHT; i++) {
        free(image[i]);
    }
    free(image);

    return 0;
}

//Ramp 함수의 내용
void Ramp(uint8_t** image) {
    // 문제나온 그래프처럼 각 구간에 맞게 ramp값을 할당 해준다.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // x값 즉 n의 값이 100일 때, y값 r(n)의 값은 120으로 유지되어야한다. 
            if (x <= 100) {
                image[y][x] = 120;
            }
            else if (x > 100 && x <= 200) { // x값 즉 n의 값이 100~200사이 일 때, y값 r(n)의 값은 135까지 증가되어야한다. 
                image[y][x] = 120 + (uint8_t)((x - 100) * (135 - 120) / 100);
            }
            else if ( x > 200 && x <= 280) { // x값 즉 n의 값이 200 ~ 280사이 일 때, y값 r(n)의 값은 225까지 증가되어야한다.
                image[y][x] = 135 + (uint8_t)((x - 200) * (225 - 135) / 80);
            }
            else if (x > 280 && x <= 300) { // x값 즉 n의 값이 280~300이 일 때, y값 r(n)의 값은 240까지 증가되어야한다.
                image[y][x] = 225 + (uint8_t)((x - 280) * (240 - 225) / 20);
            }
            else { // x값 즉 n의 값이 300~512사이 일 때, y값 r(n)의 값은 240으로 유지되어야한다.
                image[y][x] = 240;
            }
        }
    }
}