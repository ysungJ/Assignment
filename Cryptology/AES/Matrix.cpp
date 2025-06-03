#include <cstdint>
#include <iostream>
#include <iomanip>

// GF(2^8) 곱셈  (기약다항식 p(x) = x^8 + x^4 + x^3 + x + 1)
// 모듈러 0x11B = x^8 + x^4 + x^3 + x + 1
static uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint8_t p = 0;
    const uint8_t REDUCTION = 0x1B;   // 0b0001'1011  (x^8 = x^4+x^3+x+1)

    for (int i = 0; i < 8; ++i) {
        if (b & 1)          // XOR연산 
            p ^= a;

        bool hi = a & 0x80; // a의 x^7 계수가 1인지 확인
        a <<= 1;            // a = a·x  (왼쪽 시프트)

        if (hi)             // x^8 항이 생겼다면 x^8 = REDUCTION
            a ^= REDUCTION;

        b >>= 1;
    }
    return p;
}

// MixColumns (열 4바이트)  —  C · V  (C는 AES 고정 행렬)
static void mix_column(const uint8_t in[4], uint8_t out[4])
{
    static const uint8_t C[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    for (int r = 0; r < 4; ++r) {
        uint8_t acc = 0;
        for (int c = 0; c < 4; ++c)
            acc ^= gf_mul(C[r][c], in[c]);  // (행·열) 곱의 XOR 합
        out[r] = acc;
    }
}

int main(void){
 
    const uint8_t V[4] = { 0xD4, 0xBF, 0x5D, 0x30 }; // 열벡터 
    uint8_t R[4] = {0};

    mix_column(V, R);

    std::cout << "MixColumns 결과 (첫 번째 열): ";
    for (uint8_t b : R)
        std::cout << "0x"
        << std::hex << std::uppercase
        << std::setw(2) << std::setfill('0')
        << int(b) << ' ';
    std::cout << '\n';      //결과 

    return 0;
}