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

typedef struct St_element { //���ÿ� ������ ������ 
	short int row;
	short int col;
	short int dir;

}ty_element;
ty_element Stack[MAX_STACK_SIZE];

typedef struct ListNode* listPointer;
typedef struct ListNode { //���� ���� ���

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

int maze[m + 2][p + 2]; //������ ��� �������� ����
int mark[m + 2][p + 2];// ��ũ ��� 

//�Լ����� 
int path(int sy, int sx, int dy, int dx); //�̷� ��� Ž�� �Լ� ����
void gotoxy(int x, int y); //xy�����̴� �Լ�
void push(listPointer* top, ty_element item); //���ÿ� ������ �ֱ�
ty_element pop(listPointer* top); //���ؿ� ������ ��������
void revere(listPointer* Ptrtop); //[][]��ǥ �������


//�����Լ�
int main(void) {

	int starty, startx, endy, endx;
	int res;
	int i = 0, j = 0;
	FILE* pFile = NULL;
	pFile = fopen("maze_14_2.txt", "r"); //���� �о����
	
//���� �б� ���� 
	if (!pFile) {
		printf("���� ���� �Դϴ�. �о�ü� �����ϴ� \n");
		getchar();
	}
	//���Ͽ��� �̷θ� �д´�.
	for (i = 0; i < m + 2; i++) {
		for ( j = 0; j < p + 2; j++) {
			res = fscanf(pFile, "%d", &maze[i][j]);
			if (res != 1) {
				printf("������ �о� �� �� �����ϴ�.");
				getchar();
			}
		}
	}
	//�ݺ� ���� �����Ҳ��� ctrl + DŰ �����ֱ� 
	while (1) {
		printf("�����, ��ǥ����?(4���� ����; ����� ��Ʈ��+�� ����): ");
		res = scanf("%d %d %d %d", &starty, &startx, &endy, &endx);
		if (res != 4) {
			printf("���α׷��� �����մϴ�. exit \n");
			break;
		}
		//���������� �Է��ߴٸ� ��� Ž�� ����.
		path(starty, startx, endy, endx);
	}

	return 0; 
}


/*�̷� ��� Ž�� �Լ�
 [sy,sx]���� ����, [dy,dx]�� �����ϴ� ��� ã�� */
int path(int sy, int sx, int dy, int dx) {

	listPointer top = NULL; //���� ž ���� �ʱⰪ ��
	int i, j, row, col;
	int nextRow, nextCol;
	int dir, baseX, baseY;
	int found = 0, num_bktrack =0, path_length = 0, ocount = 0;
	int EXIT_ROW = dy, EXIT_COL = dx; //��ǥ��
	ty_element position;
	
	if (maze[sy][sx] == 1 || maze[dy][dx] == 1) { //������̳� ��ǥ���� 0�̾ƴ� 1�� ��� -> ���� ��� 
		printf("�Է� ���� ������̳� ��ǥ���� ���� �� �Դϴ�. \n");
		return 0;
	}

	//�̷θ� ȭ�鿡 �׷� �������� ã�� 
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	baseX = presentCur.dwCursorPosition.X; //���̽���ǥ x
	baseY = presentCur.dwCursorPosition.Y;//���̽� ��ǥ y
	
	
	for (i = 0; i < m + 2; i++) {
		for (j = 0; j < p + 2; j++) {
			gotoxy(j + baseX, i + baseY); //���� ��ǥ�� j + baseX, i + baseY���̵�
				printf("%1d", maze[i][j]); //maze�迭 ���
			
		}
	}
	//�� ���ÿ� ������� �ִ´� -> �ʱ�ȭ �ϱ�
	position.row = sy; 
	position.col = sx;
	position.dir = 0;
	push(&top, position);

	//mark���� �ʱ�ȭ �ٿ���� ������ ���� �� ��� 0���� �ʱ�ȭ
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[1 + i][1 + j] = 0;
		}
		printf("\n");
	}
	
	while (top && !found) { //���ÿ� path�������ְ� �ⱸ�� ��ã�°��
		position = pop(&top);
		row = position.row;
		col = position.col;
		dir = position.dir;

		
		while (dir < 8 && !found) // �ֺ� ���� ���������� ã�̺���
		{
			nextRow = row + Move[dir].vert;
			nextCol = col + Move[dir].horiz;

			if ((nextRow == EXIT_ROW) && (nextCol == EXIT_COL)) { //���� ��ġ�� �ⱸ���
				found = TRUE; //Ż���Դϴ�.
			//��
				gotoxy(col + baseX, row + baseY);
				printf("*");
			
				push(&top, position); //���ÿ� ������ġ ����
				gotoxy(nextCol + baseX, nextRow + baseY);
				printf("<"); // ������ Ż�ⱸ�� <ǥ���ϱ� 
			


			}
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) { //������ġ�� 0�̸鼭 �湮���� ���� �� �̸�
				
				
				if (row == sy && col == sx  ) { // �������� >�� ���
					gotoxy(col + baseX, row + baseY);
					printf(">"); //�����ϴ� ���� > ǥ���ϱ� 
				}

				else {
					gotoxy(col + baseX, row + baseY );
					printf("*"); //������ �ƴ� ���� * ��� 
				}
				
			
				mark[row][col] = TRUE;
				position.row = row; 
				position.col = col; 
				position.dir = ++dir;
				push(&top, position); //���ÿ� ������ġ ����
				row = nextRow;
				col = nextCol;
				dir = 0;
				
			}
			else {
				++dir; // eixt�� �ƴϸ鼭 �� �� ���� ���� �� 
			}
			
			if (dir == 8) {
				//��Ʈ��ŷ�� �Ͼ�� $ǥ���ϱ�
				gotoxy(col + baseX, row + baseY);
				printf("$");
				
				num_bktrack += 1; //��Ʈ��ŷ ������ 1���� 
				
				Sleep(Timestep);
			}

			Sleep(Timestep);
		}

	}
	
	if (found) { //�ⱸ�� ã���� �� 
		gotoxy(baseX, baseY + m + 2);

		printf("\nã�� ���(Row, Col):"); //�湮�� �� ���

		revere(&top);	//�������� ��� ������ ��θ� �������� ����ϱ� 

		while (top) { //while ����Ͽ� ����� ���� ���ϱ� �� ��� ������ 
			
			position = pop(&top);

			if (path_length % 6 == 0)
			{
				printf("\n");
			}
			
			printf("[%2d, %2d]", position.row, position.col);
			
			++path_length; //�н��� ����

			
		}
		printf("[%2d,%2d]",dy,dx); // ���� ���
		++path_length; //���� ���� �߰� 


		printf("\nPath�� ���� : %d \n", path_length);
		printf("��Ʈ��ŷ : %d \n", num_bktrack);

	}
	else { // Ż�ⱸ�� ã���� ���� ��� 
		gotoxy(baseX, baseY+m+2); 
		printf("\n���� ã���� �����ϴ� �̷�Ż�� x \n");
	
	}

}

void gotoxy(int x, int y) { //x y�� ���� ������ gotoxy�Լ� 
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}

void push(listPointer* top, ty_element item){//���ÿ� ������ �Է�
	listPointer temp = (listPointer)malloc(sizeof(listNode)); //�޸� �Ҵ�
	temp->data.row = item.row; //���ο� ��忡 ������ �Է� 
	temp->data.col = item.col;
	temp->data.dir = item.dir;
	temp->link = *top;//���ο� ����� ��ũ�� �������� ����
	*top = temp; //top�� ���ο� ��带 ����Ű���� ���� 
}


ty_element pop(listPointer* top) { //������ ��������
	listPointer temp = *top; //������ּ� ����
	
	ty_element item = temp->data; //item�� temp�� �����͸� ����ũ�� ��� ����
	*top = temp->link; //top�����Ͱ� temp�����Ͱ� ����Ű�� ����� �������� ����Ű���� ����
	free(temp); //temp �޸� ����
	return item; //item���� ��ȯ 
}

void revere(listPointer* Ptrtop) { //����� ���� ������ ���� ��� ex) [12, 12] [10 10] -> [1 1] [2 2]
	listPointer temp, curr, next; //��������� 3�� ����

	temp = NULL; 
	curr = *Ptrtop; //������ �ʱ�ȭ 
	do {//do while�� curr�� �� ���� �� �� ���� �ݺ� 
		next = curr->link; //���� ���� �ʱ�ȭ
		curr->link = temp; //temp�� �Ҵ�
		temp = curr; //temp�� curr�� �Ҵ�
		curr = next; //next ���� ���� �̵� 
	} while (curr);

	*Ptrtop = temp; // temp�� ������Ʈ 

}
