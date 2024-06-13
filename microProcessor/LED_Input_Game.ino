#include <Arduino.h>
#include <MsTimer2.h>

// LED 핀 번호 정의
#define LED_NO_1  0
#define LED_NO_2  1
#define LED_NO_3  2
#define LED_NO_4  3
#define LED_NO_5  4
#define LED_NO_6  5
#define LED_NO_7  6
#define LED_NO_8  7
#define LED_NO_9  14
#define LED_NO_10  15
#define LED_NO_11  16
#define LED_NO_12  17
#define LED_NO_13  18
#define LED_NO_14  19
#define LED_NO_15  20
#define LED_NO_16  21

// FND 핀 정의 (디지털 핀 사용)
#define FND1  8
#define FND2  9
#define FND3  10
#define FND4  11
#define FND5  12
#define FND6  13
#define FND7  AREF
#define FND8  GND

#define FND_A  38
#define FND_B  39
#define FND_C  40
#define FND_D  41
#define FND_E  42
#define FND_F  43
#define FND_G  44
#define FND_DP 45

#define A_BIT  0x01
#define B_BIT  0x02
#define C_BIT  0x04
#define D_BIT  0x08
#define E_BIT  0x10
#define F_BIT  0x20
#define G_BIT  0x40
#define DP_BIT 0x80

#define FND_1_SEL  0x01
#define FND_2_SEL  0x02
#define FND_3_SEL  0x04
#define FND_4_SEL  0x08
#define FND_5_SEL  0x10
#define FND_6_SEL  0x20

#define MAX_FND_NUM 6
#define MAX_FND  8

// LED 핀 번호 배열
const byte LedPinTable[16] = {
  LED_NO_1, LED_NO_2, LED_NO_3, LED_NO_4, // 1행
  LED_NO_5, LED_NO_6, LED_NO_7, LED_NO_8, // 2행
  LED_NO_9, LED_NO_10, LED_NO_11, LED_NO_12, // 3행
  LED_NO_13, LED_NO_14, LED_NO_15, LED_NO_16  // 4행
};

// 키패드 버튼 핀 번호 배열
const int buttonPins[16] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
33, 34, 35, 36, 37}; // PB1 ~ PB16
const int buzzerPin = 47; // BUZZER 핀 번호 (A0 사용)

const int FndSelectTable[6] = {
  FND1, FND2, FND3, FND4, FND5, FND6
};

const int FndPinTable[8] = {
  FND_A, FND_B, FND_C, FND_D, FND_E, FND_F, FND_G, FND_DP
};

const int FndNumberTable[10] = {
  A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT, // '0'
  B_BIT | C_BIT, // '1'
  A_BIT | B_BIT | D_BIT | E_BIT | G_BIT,  // '2'
  A_BIT | B_BIT | C_BIT | D_BIT | G_BIT,  // '3'
  B_BIT | C_BIT | F_BIT | G_BIT, // '4'
  A_BIT | C_BIT | D_BIT | F_BIT | G_BIT,  // '5'
  C_BIT | D_BIT | E_BIT | F_BIT | G_BIT,  // '6'
  A_BIT | B_BIT | C_BIT,  // '7'
  A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT | G_BIT, // '8'
  A_BIT | B_BIT | C_BIT | F_BIT | G_BIT // '9'
};

// FND 초기화
void FndInit() {
  int i;
  for (i = 0; i < MAX_FND; i++) {
    pinMode(FndSelectTable[i], OUTPUT);    // FND Sel Pin OUTPUT Set
    pinMode(FndPinTable[i], OUTPUT);       // FND Data Pin OUTPUT Set
  }
}

// FND 선택
void FndSelect(int Position) {
  int i;
  for (i = 0; i < MAX_FND_NUM; i++) {
    if (Position & (1 << i)) {
      digitalWrite(FndSelectTable[i], LOW);
    } else {
      digitalWrite(FndSelectTable[i], HIGH);
    }
  }
}

// FND 끄기
void FndOff() {
  int i;
  for (i = 0; i < MAX_FND; i++) {
    digitalWrite(FndSelectTable[i], HIGH);
  }
  delay(50);
}

// FND 데이터
void FndData(char text) {
  int i;
  if (text >= '0' && text <= '9') {
    for (i = 0; i < 8; i++) {
      if (FndNumberTable[text - '0'] & (1 << i)) {
        digitalWrite(FndPinTable[i], HIGH);
      } else {
        digitalWrite(FndPinTable[i], LOW);
      }
    }
  } else {
    // FND에 알파벳 출력
    byte data;
    switch (text) {
      case 'A': data = A_BIT | B_BIT | C_BIT | E_BIT | F_BIT | G_BIT; break;
      case 'C': data = A_BIT | D_BIT | E_BIT | F_BIT; break;
      case 'E': data = A_BIT | D_BIT | E_BIT | F_BIT | G_BIT; break;
      case 'G': data = A_BIT | C_BIT | D_BIT | E_BIT | F_BIT; break;
      case 'O': data = A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT; break;
      case 'L': data = F_BIT | E_BIT | D_BIT; break;
      case 'I': data = F_BIT | E_BIT; break;
      case 'D': data = B_BIT | G_BIT | E_BIT | D_BIT | C_BIT; break;
      case 'F': data = A_BIT | F_BIT | G_BIT | E_BIT; break;
      default: data = 0;
    }
    for (i = 0; i < 8; i++) {
      if (data & (1 << i)) {
        digitalWrite(FndPinTable[i], HIGH);
      } else {
        digitalWrite(FndPinTable[i], LOW);
      }
    }
  }
  delay(1);
}

// GOOD 텍스트 출력과 함께 소리를 내는 함수
void DrawTextFndGood(const char* text) {
  int len = strlen(text);  // 문자열의 길이를 가져옴
  int displayTime = 1000;  // 텍스트를 표시할 총 시간 (밀리초)
  int refreshInterval = 5; // 텍스트 갱신 간격 (밀리초)
  int iterations = displayTime / refreshInterval;
  int buzzerTones[5] = {329, 440, 554, 659, 880}; // 정답일 때 주파수

  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < len && i < MAX_FND_NUM; i++) {
      FndSelect(1 << (MAX_FND_NUM - len + i));  // 오른쪽에서 왼쪽으로 선택
      FndData(text[len - 1 - i]);     // 문자열의 각 문자를 반대로 출력
    }
    tone(buzzerPin, buzzerTones[(j / (iterations / 5)) % 5], 200); // 동시에 소리 출력
    delay(refreshInterval); // 갱신 간격 만큼 대기
  }
  FndOff();
}

// FAIL 텍스트 출력과 함께 소리를 내는 함수
void DrawTextFndFail(const char* text) {
  int len = strlen(text);  // 문자열의 길이를 가져옴
  int displayTime = 1000;  // 텍스트를 표시할 총 시간 (밀리초)
  int refreshInterval = 5; // 텍스트 갱신 간격 (밀리초)
  int iterations = displayTime / refreshInterval;
  int failTone = 880; // 오답일 때 주파수

  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < len && i < MAX_FND_NUM; i++) {
      FndSelect(1 << (MAX_FND_NUM - len + i));  // 오른쪽에서 왼쪽으로 선택
      FndData(text[len - 1 - i]);     // 문자열의 각 문자를 반대로 출력
    }
    tone(buzzerPin, failTone, 200); // 동시에 소리 출력
    delay(refreshInterval); // 갱신 간격 만큼 대기
  }
  FndOff();
}

// FND 숫자 출력
void DrawNumberFnd(int num) {
  FndSelect(1 << 0);  // 첫 번째 위치 선택
  FndData('0' + num);  // 숫자를 문자로 변환하여 출력
}

// 피드백을 제공하는 함수
void provideFeedback(bool correct) {
  if (correct) {
    DrawTextFndGood("GOOD");
    for (int i = 0; i < 16; i++) {
      digitalWrite(LedPinTable[i], HIGH); // 모든 LED 켜기
      delay(100);
      digitalWrite(LedPinTable[i], LOW);  // 모든 LED 끄기
    }
  } else {
    DrawTextFndFail("FAIL");
    for (int i = 0; i < 16; i++) {
      digitalWrite(LedPinTable[i], HIGH); // 모든 LED 켜기
      delay(50);
      digitalWrite(LedPinTable[i], LOW);  // 모든 LED 끄기
    }
  }
}

// LED와 키패드 매칭 정의
const int ledToButtonMap[16] = {
  3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12
};

// 키패드와 LED 매칭 정의 (문서화된 매핑)
const int buttonToLedMap[16] = {
  3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12
};

// 스테이지 1 패턴 정의
const int stage1Pattern1[] = {3, 2, 1, 0, 4, 8, 10, 11}; // LED4, LED3, LED2, LED1, LED5, LED9
const int stage1Pattern2[] = {1, 2, 3, 4, 8, 12}; // 다른 패턴1
const int stage1Pattern3[] = {0, 1, 2, 3, 5, 9}; // 다른 패턴2

const int* stage1Patterns[] = {stage1Pattern1, stage1Pattern2, stage1Pattern3}; // 패턴 배열
const int stage1Lengths[] = {8, 6, 6}; // 각 패턴의 길이

// 스테이지 2 패턴 정의
const int stage2Pattern1[] = {3, 2, 1, 0, 4, 8, 12, 13, 14, 15}; // LED4, LED3, LED2, LED1, LED5, LED9, LED13, LED14, LED15, LED16
const int stage2Pattern2[] = {2, 3, 4, 5, 9, 13, 14, 15, 11, 7}; // 다른 패턴1
const int stage2Pattern3[] = {1, 0, 4, 8, 12, 13, 14, 15, 7, 3}; // 다른 패턴2

const int* stage2Patterns[] = {stage2Pattern1, stage2Pattern2, stage2Pattern3}; // 패턴 배열
const int stage2Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 3 패턴 정의
const int stage3Pattern1[] = {3, 2, 1, 0, 5, 10, 15}; // LED4, LED3, LED2, LED1, LED6, LED11, LED16
const int stage3Pattern2[] = {2, 1, 0, 3, 6, 11, 10}; // 다른 패턴1
const int stage3Pattern3[] = {1, 0, 3, 2, 7, 8, 9, 15, 14}; // 다른 패턴2 st3 st6 수정

const int* stage3Patterns[] = {stage3Pattern1, stage3Pattern2, stage3Pattern3}; // 패턴 배열
const int stage3Lengths[] = {7, 7, 9}; // 각 패턴의 길이

// 스테이지 4 패턴 정의
const int stage4Pattern1[] = {3, 2, 1, 0, 4, 8, 12, 5, 10, 15}; // LED4, LED3, LED2, LED1, LED5, LED9, LED13, LED6, LED11, LED16
const int stage4Pattern2[] = {2, 1, 0, 3, 7, 8, 9, 11, 12, 13}; // 다른 패턴1
const int stage4Pattern3[] = {1, 0, 3, 2, 6, 5, 4, 10, 9, 8}; // 다른 패턴2

const int* stage4Patterns[] = {stage4Pattern1, stage4Pattern2, stage4Pattern3}; // 패턴 배열
const int stage4Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 5 패턴 정의
const int stage5Pattern1[] = {2, 1, 0, 4, 8, 12, 5, 10, 15, 11}; // LED3, LED2, LED1, LED5, LED9, LED13, LED6, LED11, LED16, LED12
const int stage5Pattern2[] = {1, 0, 4, 8, 12, 5, 10, 15, 14, 3}; // 다른 패턴1 
const int stage5Pattern3[] = {0, 4, 8, 12, 5, 10, 15, 14, 13, 2}; // 다른 패턴2

const int* stage5Patterns[] = {stage5Pattern1, stage5Pattern2, stage5Pattern3}; // 패턴 배열
const int stage5Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 6 패턴 정의 (타이머 8초)
const int stage6Pattern1[] = {3, 2, 0, 6, 5, 4, 11, 8, 15, 14, 12}; // LED 패턴
const int stage6Pattern2[] = {3, 2, 1, 0, 4, 10, 9, 8, 15, 14, 12}; // 다른 패턴1
const int stage6Pattern3[] = {3, 1, 0, 6, 4, 11, 8, 15, 14, 12}; // 다른 패턴2

const int* stage6Patterns[] = {stage6Pattern1, stage6Pattern2, stage6Pattern3}; // 패턴 배열
const int stage6Lengths[] = {11, 11, 10}; // 각 패턴의 길이

// 스테이지 7 패턴 정의
const int stage7Pattern1[] = {3, 2, 7, 11, 10, 15, 0, 4, 8, 12, 13}; // LED 패턴
const int stage7Pattern2[] = {2, 7, 11, 10, 15, 0, 4, 8, 12, 9, 6}; // 다른 패턴1
const int stage7Pattern3[] = {7, 11, 10, 15, 0, 4, 8, 12, 9, 5, 3}; // 다른 패턴2

const int* stage7Patterns[] = {stage7Pattern1, stage7Pattern2, stage7Pattern3}; // 패턴 배열
const int stage7Lengths[] = {11, 11, 11}; // 각 패턴의 길이

// 스테이지 8 패턴 정의 (타이머 8초)
const int stage8Pattern1[] = {3, 2, 1, 0, 4, 9, 12, 14, 15, 10, 6}; // LED 패턴
const int stage8Pattern2[] = {2, 1, 0, 4, 9, 12, 14, 15, 10, 5, 8}; // 다른 패턴1
const int stage8Pattern3[] = {1, 0, 4, 9, 12, 14, 15, 10, 6, 11, 7}; // 다른 패턴2

const int* stage8Patterns[] = {stage8Pattern1, stage8Pattern2, stage8Pattern3}; // 패턴 배열
const int stage8Lengths[] = {11, 11, 11}; // 각 패턴의 길이

// 스테이지 9 패턴 정의
const int stage9Pattern1[] = {3, 1, 6, 5, 4, 11, 9, 15, 14, 12}; // LED 패턴
const int stage9Pattern2[] = {1, 6, 5, 4, 11, 9, 15, 14, 12, 8}; // 다른 패턴1
const int stage9Pattern3[] = {6, 5, 4, 11, 9, 15, 14, 12, 8, 2}; // 다른 패턴2

const int* stage9Patterns[] = {stage9Pattern1, stage9Pattern2, stage9Pattern3}; // 패턴 배열
const int stage9Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 10 패턴 정의
const int stage10Pattern1[] = {3, 2, 1, 7, 5, 10, 15, 13, 12}; // LED 패턴
const int stage10Pattern2[] = {2, 1, 7, 5, 10, 15, 13, 12, 9}; // 다른 패턴1
const int stage10Pattern3[] = {1, 7, 5, 10, 15, 13, 12, 9, 4}; // 다른 패턴2

const int* stage10Patterns[] = {stage10Pattern1, stage10Pattern2, stage10Pattern3}; // 패턴 배열
const int stage10Lengths[] = {9, 9, 9}; // 각 패턴의 길이

// 스테이지 11 패턴 정의
const int stage11Pattern1[] = {3, 0, 6, 5, 4, 10, 9, 15, 14, 12}; // LED 패턴
const int stage11Pattern2[] = {0, 6, 3, 4, 2, 9, 15, 14, 12, 8}; // 다른 패턴1
const int stage11Pattern3[] = {2, 1, 7, 6, 4, 9, 8, 15, 13, 11}; // 다른 패턴2

const int* stage11Patterns[] = {stage11Pattern1, stage11Pattern2, stage11Pattern3}; // 패턴 배열
const int stage11Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 12 패턴 정의
const int stage12Pattern1[] = {3, 2, 0, 6, 5, 11, 10, 9, 8, 15, 12}; // LED 패턴
const int stage12Pattern2[] = {3, 1, 6, 4, 11, 9, 8, 15, 14, 12}; // 다른 패턴1
const int stage12Pattern3[] = {0, 6, 5, 11, 10, 9, 8, 15, 12, 4, 3}; // 다른 패턴2

const int* stage12Patterns[] = {stage12Pattern1, stage12Pattern2, stage12Pattern3}; // 패턴 배열
const int stage12Lengths[] = {11, 10, 11}; // 각 패턴의 길이

// 스테이지 13 패턴 정의 
const int stage13Pattern1[] = {3, 1, 0, 6, 4, 11, 9, 15, 13, 12}; // LED 패턴
const int stage13Pattern2[] = {1, 0, 6, 4, 11, 9, 15, 13, 12, 10}; // 다른 패턴1
const int stage13Pattern3[] = {0, 6, 4, 11, 9, 15, 13, 12, 10, 8}; // 다른 패턴2

const int* stage13Patterns[] = {stage13Pattern1, stage13Pattern2, stage13Pattern3}; // 패턴 배열
const int stage13Lengths[] = {10, 10, 10}; // 각 패턴의 길이

// 스테이지 14 패턴 정의
const int stage14Pattern1[] = {3, 1, 0, 6, 4, 11, 9, 14}; // LED 패턴
const int stage14Pattern2[] = {1, 0, 6, 4, 11, 9, 14, 12}; // 다른 패턴1
const int stage14Pattern3[] = {0, 6, 4, 11, 9, 14, 12, 10}; // 다른 패턴2

const int* stage14Patterns[] = {stage14Pattern1, stage14Pattern2, stage14Pattern3}; // 패턴 배열
const int stage14Lengths[] = {8, 8, 8}; // 각 패턴의 길이

// 스테이지 15 패턴 정의
const int stage15Pattern1[] = {3, 1, 7, 6, 10, 8, 15, 13}; // LED 패턴
const int stage15Pattern2[] = {1, 7, 6, 10, 8, 15, 13, 12}; // 다른 패턴1
const int stage15Pattern3[] = {7, 6, 10, 8, 15, 13, 12, 11}; // 다른 패턴2

const int* stage15Patterns[] = {stage15Pattern1, stage15Pattern2, stage15Pattern3}; // 패턴 배열
const int stage15Lengths[] = {8, 8, 8}; // 각 패턴의 길이

const int maxSequenceLength = 16; // 최대 순서 길이
int userInput[maxSequenceLength]; // 사용자 입력 배열
bool ledStatus[16] = {false}; // LED 상태 배열

bool gameStarted = false;
int currentStage = 0;

//Text LCD 정의 
const int rs = A0;
const int rw = A1;
const int e =  A2;

const int d0 = A8;
const int d1 = A9;
const int d2 = A10;
const int d3 = A11;
const int d4 = A12;
const int d5 = A13;
const int d6 = A14;
const int d7 = A15;

// LCD 초기화 명령어 및 설정
void lcdCommand(uint8_t command) {
  digitalWrite(rs, LOW);
  digitalWrite(rw, LOW);
  digitalWrite(e, HIGH);

  digitalWrite(d0, command & 0x01);
  digitalWrite(d1, command & 0x02);
  digitalWrite(d2, command & 0x04);
  digitalWrite(d3, command & 0x08);
  digitalWrite(d4, command & 0x10);
  digitalWrite(d5, command & 0x20);
  digitalWrite(d6, command & 0x40);
  digitalWrite(d7, command & 0x80);

  delayMicroseconds(1); // Enable pulse must be >450ns
  digitalWrite(e, LOW);
  delayMicroseconds(50); // Commands need > 37us to settle
}

// 문자 데이터 전송 함수
void lcdData(uint8_t data) {
  digitalWrite(rs, HIGH);
  digitalWrite(rw, LOW);
  digitalWrite(e, HIGH);

  digitalWrite(d0, data & 0x01);
  digitalWrite(d1, data & 0x02);
  digitalWrite(d2, data & 0x04);
  digitalWrite(d3, data & 0x08);
  digitalWrite(d4, data & 0x10);
  digitalWrite(d5, data & 0x20);
  digitalWrite(d6, data & 0x40);
  digitalWrite(d7, data & 0x80);

  delayMicroseconds(1); // Enable pulse must be >450ns
  digitalWrite(e, LOW);
  delayMicroseconds(50); // Data needs > 37us to settle
}

// LCD 초기화 함수
void lcdInit() {
  pinMode(rs, OUTPUT);
  pinMode(rw, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(d0, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);

  delay(50); // Wait for LCD to power up

  lcdCommand(0x30); // Function set: 8-bit, 1 line, 5x8 dots
  delayMicroseconds(4500); // Wait for more than 4.1ms
  lcdCommand(0x30); // Function set: 8-bit, 1 line, 5x8 dots
  delayMicroseconds(150); // Wait for more than 100us
  lcdCommand(0x30); // Function set: 8-bit, 1 line, 5x8 dots

  lcdCommand(0x38); // Function set: 8-bit, 2 line, 5x8 dots
  lcdCommand(0x0C); // Display ON, Cursor OFF, Blink OFF
  lcdCommand(0x06); // Entry mode: Increment cursor, No shift
  lcdCommand(0x01); // Clear display
  delayMicroseconds(2000); // Wait for more than 1.64ms
}

// LCD에 텍스트 출력
void lcdPrint(const char* str) {
  while (*str) {
    lcdData(*str++);
  }
}

// LCD에 현재 스테이지를 출력하는 함수
void displayStage(int stage) {
  lcdCommand(0x01); // Clear display
  delay(2); // Wait for the clear command to execute

  lcdPrint("Stage : ");
  if (stage < 10) {
    lcdData('0' + stage); // 한 자리 숫자
  } else {
    lcdData('0' + stage / 10); // 첫 번째 자리 숫자
    lcdData('0' + stage % 10); // 두 번째 자리 숫자
  }
}

// 초기 상태로 돌아가는 함수
void resetToInitialState() {
  // 모든 LED를 켭니다.
  for (int i = 0; i < 16; i++) {
    digitalWrite(LedPinTable[i], HIGH);
  }

  // 게임 시작 대기
  gameStarted = false;
  currentStage = 0; // 첫 번째 스테이지로 초기화
  displayStage(currentStage); // 초기화면에 Stage : 0 출력

  while (!gameStarted) {
    for (int i = 0; i < 16; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        gameStarted = true;
        break;
      }
    }
  }

  // 게임 시작 시 currentStage를 1로 설정
  currentStage = 1;

  // 카운트다운
  for (int i = 5; i > 0; i--) {
    if (i == 5) {
      displayStage(currentStage); // Stage 증가 시점 변경
    }
    DrawNumberFnd(i);
    tone(buzzerPin, 700, 200); // 1초마다 700Hz 버저 울리기
    delay(1000); // 1초 대기
  }

  // 게임 시작 시 모든 LED를 끕니다.
  turnOffAllLeds();
}

// 모든 LED 끄기
void turnOffAllLeds() {
  for (int i = 0; i < 16; i++) {
    digitalWrite(LedPinTable[i], LOW);
  }
}

// 축하 LED 이펙트 함수
void celebrationEffect() {
  int buzzerTones[] = {262, 294, 330, 349, 392, 440, 494, 523}; // 축하음 주파수 배열

  for (int i = 0; i < 3; i++) { // 3초 동안 이펙트 반복
    for (int j = 0; j < 16; j++) {
      digitalWrite(LedPinTable[j], HIGH);
      tone(buzzerPin, buzzerTones[j % 8], 100); // 각 LED에 대해 다른 톤 재생
      delay(50);
      digitalWrite(LedPinTable[j], LOW);
    }
    for (int j = 15; j >= 0; j--) {
      digitalWrite(LedPinTable[j], HIGH);
      tone(buzzerPin, buzzerTones[j % 8], 100); // 각 LED에 대해 다른 톤 재생
      delay(50);
      digitalWrite(LedPinTable[j], LOW);
    }
  }
  noTone(buzzerPin); // 모든 소리 끄기
}

void setup() {
  // LED 핀과 키패드 버튼 핀을 설정
  for (int i = 0; i < 16; i++) {
    pinMode(LedPinTable[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // 키패드 버튼 핀을 입력으로 설정
  }
  pinMode(buzzerPin, OUTPUT);
  FndInit(); // FND 초기화
  randomSeed(analogRead(0)); // 무작위 시드 설정
  
  lcdInit(); // LCD 초기화
  resetToInitialState(); // 초기 상태 설정
}

// 각 스테이지의 LED 유지 시간을 반환하는 함수
int getStageLedDuration(int stage) {
  if (stage >= 1 && stage <= 5) {
    return 700; // 1스테이지부터 5스테이지까지는 0.7초 동안 유지
  } else if (stage >= 6 && stage <= 10) {
    return 500; // 6스테이지부터 10스테이지까지는 0.5초 동안 유지
  } else if (stage >= 11 && stage <= 15) {
    return 300; // 11스테이지부터 15스테이지까지는 0.3초 동안 유지
  }
  return 1000; // 기본 유지 시간 1초
}

// LED 패턴을 표시하는 함수
void showPattern(const int* pattern, int length, int duration) {
  // 모든 LED를 끕니다.
  turnOffAllLeds();
  // 모든 LED를 한꺼번에 켭니다.
  for (int i = 0; i < length; i++) {
    digitalWrite(LedPinTable[pattern[i]], HIGH);
  }
  delay(duration); // 지정된 시간 동안 켜진 상태 유지
  // 모든 LED를 끕니다.
  turnOffAllLeds();
}

// 사용자의 입력을 받는 함수
bool getUserInputWithTimeout(int* userInput, int length, int timeout) {
  int count = 0;
  unsigned long startTime = millis();
  bool buzzerPlayed[3] = {false, false, false}; // 3, 2, 1 초에서 버저가 울렸는지 확인하기 위한 배열

  while (count < length) {
    unsigned long elapsedTime = (millis() - startTime) / 1000;
    int remainingTime = timeout - elapsedTime;

    if (remainingTime >= 0) {
      DrawNumberFnd(remainingTime); // 남은 시간 FND에 표시
    }

    if (remainingTime == 3 && !buzzerPlayed[0]) {
      tone(buzzerPin, 700, 200); // 3초에 도달하면 Buzzer 소리
      buzzerPlayed[0] = true;
    }
    if (remainingTime == 2 && !buzzerPlayed[1]) {
      tone(buzzerPin, 700, 200); // 2초에 도달하면 Buzzer 소리
      buzzerPlayed[1] = true;
    }
    if (remainingTime == 1 && !buzzerPlayed[2]) {
      tone(buzzerPin, 700, 200); // 1초에 도달하면 Buzzer 소리
      buzzerPlayed[2] = true;
    }

    if (remainingTime <= 0) { // 제한 시간 초과 확인
      noTone(buzzerPin); // Buzzer 끄기
      return false;
    }

    for (int i = 0; i < 16; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        userInput[count] = i;
        digitalWrite(LedPinTable[buttonToLedMap[i]], HIGH); // 키패드에 해당하는 LED 켜기
        ledStatus[buttonToLedMap[i]] = true;
        count++;
        delay(300); // 디바운싱을 위한 지연
        while (digitalRead(buttonPins[i]) == LOW); // 버튼이 떼어질 때까지 대기
      }
    }
  }
  noTone(buzzerPin); // Buzzer 끄기
  return true;
}

// 사용자의 입력이 정답인지 확인하는 함수
bool checkUserInput(const int* pattern, const int* userInput, int length) {
  for (int i = 0; i < length; i++) {
    if (!ledStatus[pattern[i]]) {
      return false;
    }
  }
  return true;
}

// 각 스테이지의 패턴 정보를 가져오는 함수
void getStageInfo(int stage, const int* &pattern, int &length) {
  int patternIndex = random(0, 3); // 랜덤한 패턴 선택
  switch (stage) {
    case 1:
      pattern = stage1Patterns[patternIndex];
      length = stage1Lengths[patternIndex];
      break;
    case 2:
      pattern = stage2Patterns[patternIndex];
      length = stage2Lengths[patternIndex];
      break;
    case 3:
      pattern = stage3Patterns[patternIndex];
      length = stage3Lengths[patternIndex];
      break;
    case 4:
      pattern = stage4Patterns[patternIndex];
      length = stage4Lengths[patternIndex];
      break;
    case 5:
      pattern = stage5Patterns[patternIndex];
      length = stage5Lengths[patternIndex];
      break;
    case 6:
      pattern = stage6Patterns[patternIndex];
      length = stage6Lengths[patternIndex];
      break;
    case 7:
      pattern = stage7Patterns[patternIndex];
      length = stage7Lengths[patternIndex];
      break;
    case 8:
      pattern = stage8Patterns[patternIndex];
      length = stage8Lengths[patternIndex];
      break;
    case 9:
      pattern = stage9Patterns[patternIndex];
      length = stage9Lengths[patternIndex];
      break;
    case 10:
      pattern = stage10Patterns[patternIndex];
      length = stage10Lengths[patternIndex];
      break;
    case 11:
      pattern = stage11Patterns[patternIndex];
      length = stage11Lengths[patternIndex];
      break;
    case 12:
      pattern = stage12Patterns[patternIndex];
      length = stage12Lengths[patternIndex];
      break;
    case 13:
      pattern = stage13Patterns[patternIndex];
      length = stage13Lengths[patternIndex];
      break;
    case 14:
      pattern = stage14Patterns[patternIndex];
      length = stage14Lengths[patternIndex];
      break;
    case 15:
      pattern = stage15Patterns[patternIndex];
      length = stage15Lengths[patternIndex];
      break;
    default:
      pattern = nullptr;
      length = 0;
      break;
  }
}

// 각 스테이지의 시간 제한을 반환하는 함수
int getStageTimeLimit(int stage) {
  if (stage >= 1 && stage <= 5) {
    return 9; // 1스테이지부터 5스테이지까지는 시간 제한 9초
  } else if (stage >= 6 && stage <= 10) {
    return 7; // 6스테이지부터 10스테이지까지는 시간 제한 7초
  } else if (stage >= 11 && stage <= 15) {
    return 5; // 11스테이지부터 15스테이지까지는 시간 제한 5초
  }
  return 9; // 기본 시간 제한 9초
}

// 단계별 루프를 관리하는 함수
void playStage(int stage) {
  const int* pattern;
  int length;
  
  getStageInfo(stage, pattern, length);
  if (pattern == nullptr || length == 0) {
    return; // 잘못된 스테이지일 경우 함수 종료
  }

  int timeLimit = getStageTimeLimit(stage); // 스테이지에 따른 시간 제한 설정
  int ledDuration = getStageLedDuration(stage); // 스테이지에 따른 LED 유지 시간 설정

  memset(ledStatus, false, sizeof(ledStatus)); // LED 상태 초기화

  showPattern(pattern, length, ledDuration);
  
  if (getUserInputWithTimeout(userInput, length, timeLimit)) { // 스테이지에 따른 시간 제한 적용
    if (checkUserInput(pattern, userInput, length)) {
      provideFeedback(true); // 정답일 때 긍정적 피드백
      delay(1000);  // 1초 동안 LED 켜진 상태 유지
      turnOffAllLeds();  // LED 끄기

      if (stage == 15) { // 15스테이지를 클리어했을 경우
        lcdCommand(0x01); // Clear display
        delay(2); // Wait for the clear command to execute
        lcdPrint("You've all cleared");
        lcdCommand(0xC0); // Move cursor to the second line
        lcdPrint("Congratulations");
        celebrationEffect(); // 축하 LED 이펙트
        delay(3000); // 3초 대기
        resetToInitialState(); // 초기 상태로 돌아가기
        return; // 첫 번째 스테이지로 돌아가기 위해 함수 종료
      }

      // 카운트다운
      for (int i = 5; i > 0; i--) {
        if (i == 5) {
          currentStage++; // Stage 증가 시점 변경
          displayStage(currentStage);
        }
        DrawNumberFnd(i);
        tone(buzzerPin, 700, 200); // 1초마다 700Hz 버저 울리기
        delay(1000); // 1초 대기
      }
    } else {
      provideFeedback(false); // 틀렸을 때 부정적 피드백
      delay(1000);
      resetToInitialState(); // 초기 상태로 돌아가기
      return; // 첫 번째 스테이지로 돌아가기 위해 함수 종료
    }
  } else {
    provideFeedback(false); // 시간 초과 시 부정적 피드백
    delay(1000);
    resetToInitialState(); // 초기 상태로 돌아가기
    return; // 첫 번째 스테이지로 돌아가기 위해 함수 종료
  }
}

void loop() {
  playStage(currentStage);
}
