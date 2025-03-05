#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

#define m 12
#define p 12
#define Max_dir 8
#define Timestep 10
#define TURE 1
#define FALSE 0
#define MAX_STACK_SIZE 100

typedef struct Aoff {
	short int vert;
	short int horiz;
}offsets;

typedef struct St_element { //스택에 저장할 데이터 
	short int row;
	short int col;
	short int dir;

}ty_element;
ty_element Stack[MAX_STACK_SIZE];

typedef struct ListNode* listPointer;
typedef struct ListNode { //연결 스택 노드

	ty_element data;
	listPointer link;

}listNode;
 
/*N NE 
 E SE
 S SW
 W NW */
offsets Move[Max_dir] = { {-1,0}, {-1,1},  
	{0,1},{1,1}, 
	{1,0},{1,-1}, 
	{0,-1},{-1,-1} };

int maze[m + 2][p + 2]; //메이즈 행렬 전역변수 선언
int mark[m + 2][p + 2];// 마크 행렬 

//함수선언 
int path(int sy, int sx, int dy, int dx); //미로 경로 탐색 함수 선언
void gotoxy(int x, int y); //xy움직이는 함수
void push(listPointer* top, ty_element item); //스택에 데이터 넣기
ty_element pop(listPointer* top); //스텍에 데이터 꺼내오기
void revere(listPointer* Ptrtop); //[][]좌표 역순출력


//메인함수
int main(void) {

	int starty, startx, endy, endx;
	int res;
	int i = 0, j = 0;
	FILE* pFile = NULL;
	pFile = fopen("maze_14_2.txt", "r"); //파일 읽어오기
	
//파일 읽기 실패 
	if (!pFile) {
		printf("파일 오류 입니다. 읽어올수 없습니다 \n");
		getchar();
	}
	//파일에서 미로를 읽는다.
	for (i = 0; i < m + 2; i++) {
		for ( j = 0; j < p + 2; j++) {
			res = fscanf(pFile, "%d", &maze[i][j]);
			if (res != 1) {
				printf("파일을 읽어 올 수 없습니다.");
				getchar();
			}
		}
	}
	//반복 수행 종료할꺼면 ctrl + D키 눌러주기 
	while (1) {
		printf("출발점, 목표점은?(4개의 정수; 종료는 컨트롤+디 엔터): ");
		res = scanf("%d %d %d %d", &starty, &startx, &endy, &endx);
		if (res != 4) {
			printf("프로그램을 종료합니다. exit \n");
			break;
		}
		//정상적으로 입력했다면 경로 탐색 시작.
		path(starty, startx, endy, endx);
	}

	return 0; 
}


/*미로 경로 탐색 함수
 [sy,sx]에서 시작, [dy,dx]에 도달하는 경로 찾기 */
int path(int sy, int sx, int dy, int dx) {

	listPointer top = NULL; //스택 탑 변수 초기값 널
	int i, j, row, col;
	int nextRow, nextCol;
	int dir, baseX, baseY;
	int found = 0, num_bktrack =0, path_length = 0, ocount = 0;
	int EXIT_ROW = dy, EXIT_COL = dx; //목표점
	ty_element position;
	
	if (maze[sy][sx] == 1 || maze[dy][dx] == 1) { //출발점이나 목표점이 0이아닌 1인 경우 -> 막힌 경우 
		printf("입력 오류 출발점이나 목표점이 막힌 셀 입니다. \n");
		return 0;
	}

	//미로를 화면에 그려 기준점을 찾기 
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	baseX = presentCur.dwCursorPosition.X; //베이스좌표 x
	baseY = presentCur.dwCursorPosition.Y;//베이스 좌표 y
	
	
	for (i = 0; i < m + 2; i++) {
		for (j = 0; j < p + 2; j++) {
			gotoxy(j + baseX, i + baseY); //현재 좌표를 j + baseX, i + baseY로이동
				printf("%1d", maze[i][j]); //maze배열 출력
			
		}
	}
	//빈 스택에 출발점을 넣는다 -> 초기화 하기
	position.row = sy; 
	position.col = sx;
	position.dir = 0;
	push(&top, position);

	//mark정보 초기화 바운더리 제외한 내부 셀 모두 0으로 초기화
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[1 + i][1 + j] = 0;
		}
		printf("\n");
	}
	
	while (top && !found) { //스택에 path가남아있고 출구를 못찾는경우
		position = pop(&top);
		row = position.row;
		col = position.col;
		dir = position.dir;

		
		while (dir < 8 && !found) // 주변 방향 여덟방향을 찾이보기
		{
			nextRow = row + Move[dir].vert;
			nextCol = col + Move[dir].horiz;

			if ((nextRow == EXIT_ROW) && (nextCol == EXIT_COL)) { //다음 위치가 출구라면
				found = TRUE; //탈출입니다.
			//끝
				gotoxy(col + baseX, row + baseY);
				printf("*");
			
				push(&top, position); //스택에 현재위치 저장
				gotoxy(nextCol + baseX, nextRow + baseY);
				printf("<"); // 마지막 탈출구에 <표시하기 
			


			}
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) { //다음위치가 0이면서 방문한적 없는 곳 이면
				
				
				if (row == sy && col == sx  ) { // 시작점을 >로 출력
					gotoxy(col + baseX, row + baseY);
					printf(">"); //시작하는 점에 > 표시하기 
				}

				else {
					gotoxy(col + baseX, row + baseY );
					printf("*"); //시작점 아닌 곳에 * 찍기 
				}
				
			
				mark[row][col] = TRUE;
				position.row = row; 
				position.col = col; 
				position.dir = ++dir;
				push(&top, position); //스택에 현재위치 저장
				row = nextRow;
				col = nextCol;
				dir = 0;
				
			}
			else {
				++dir; // eixt가 아니면서 갈 수 없는 곳일 때 
			}
			
			if (dir == 8) {
				//백트레킹이 일어나면 $표시하기
				gotoxy(col + baseX, row + baseY);
				printf("$");
				
				num_bktrack += 1; //백트레킹 개수에 1증가 
				
				Sleep(Timestep);
			}

			Sleep(Timestep);
		}

	}
	
	if (found) { //출구를 찾았을 때 
		gotoxy(baseX, baseY + m + 2);

		printf("\n찾은 경로(Row, Col):"); //방문한 길 출력

		revere(&top);	//리버스로 경로 지나간 경로를 역순으로 출력하기 

		while (top) { //while 사용하여 경로의 길이 더하기 및 출력 나누기 
			
			position = pop(&top);

			if (path_length % 6 == 0)
			{
				printf("\n");
			}
			
			printf("[%2d, %2d]", position.row, position.col);
			
			++path_length; //패스의 길이

			
		}
		printf("[%2d,%2d]",dy,dx); // 끝점 출력
		++path_length; //끝점 길이 추가 


		printf("\nPath의 길이 : %d \n", path_length);
		printf("백트레킹 : %d \n", num_bktrack);

	}
	else { // 탈출구를 찾을수 없는 경우 
		gotoxy(baseX, baseY+m+2); 
		printf("\n길을 찾을수 없습니다 미로탈출 x \n");
	
	}

}

void gotoxy(int x, int y) { //x y과 어디로 가는지 gotoxy함수 
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}

void push(listPointer* top, ty_element item){//스택에 데이터 입력
	listPointer temp = (listPointer)malloc(sizeof(listNode)); //메모리 할당
	temp->data.row = item.row; //새로운 노드에 데이터 입력 
	temp->data.col = item.col;
	temp->data.dir = item.dir;
	temp->link = *top;//새로운 노드의 링크를 이전노드로 연결
	*top = temp; //top이 새로운 노드를 가리키도록 연결 
}


ty_element pop(listPointer* top) { //데이터 꺼내오기
	listPointer temp = *top; //노드의주소 저장
	
	ty_element item = temp->data; //item에 temp가 데이터를 가리크는 노드 저장
	*top = temp->link; //top포인터가 temp포인터가 가르키는 노드의 다음노드로 가르키도록 저함
	free(temp); //temp 메모리 해제
	return item; //item으로 반환 
}

void revere(listPointer* Ptrtop) { //경로의 길이 리버스 시켜 출력 ex) [12, 12] [10 10] -> [1 1] [2 2]
	listPointer temp, curr, next; //노드포인터 3개 선언

	temp = NULL; 
	curr = *Ptrtop; //헤더노드 초기화 
	do {//do while문 curr이 널 값이 될 때 까지 반복 
		next = curr->link; //다음 노드로 초기화
		curr->link = temp; //temp로 할당
		temp = curr; //temp에 curr를 할당
		curr = next; //next 다음 노드로 이동 
	} while (curr);

	*Ptrtop = temp; // temp로 업데이트 

}
