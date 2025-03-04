# 인공지능 과제5

Reinforcement Learning

----------------------
## 과제개요
본과제는 강화학습에 대한 것이다 다룰 문제는 강의노트에서 소개한 frozen lake 문제이다 강화학습 기법 중 Q
learning 기술을 사용하도록 한다 이 기술을 사용하여 optimal policy 를 구한다 즉 q 함수를 구한다 따라서 이 문
제는 control 문제이다 프 로그램은 o ptimal policy ( 즉 optimal q value s ) 를 학습하는 단계와 그 후에 오는 test 단계
로 구성된다 test 단계에서는 구한 q value s 를 기반으로 greedy 하게 에피소드를 구해 본다 즉 start 에서 t erminal
state 까지 가는 경로를 구하여 출력한다 다음 예는 테스트 단계에서의 출력의 일부 모습이다

### 소과제 1 

table 기반의 Q learning 방법론 을 사용하여 개발한다

### 소과제 2 
function approximat ion 에 기반한 방법론을 사용한다 이를 위해 DQN(deep Q network) 을 사용하도록한다
