//2019253084 전유성 데이타구조론 hw 1번 과제
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //파일 입출력을 위해 헤더파일 stdio.h 선언
#include<string.h> // 문자열 선언을 위해 헤더파일 string.h 선언
#include<stdlib.h> 
#include<windows.h>

//삭제하는 경우를 참이면 1 거짓이면 0
#define TRUE 1
#define FALSE 0
#define Sleep_time 1000

// 구조체 데이터 타입 선언
typedef struct Listdata {
	char name[30]; //학생명
	int id; //학번
	char phone[20]; //전화번호
	char province[30]; //거주지역
} listData;

//리스트 노드 정의
typedef struct Listnode* type_listNode_pointer;
typedef struct Listnode {
	listData data;
	type_listNode_pointer link;
} listNode;


//리스트의 첫 번째 노드를 가리키는 head는 다음과 같이 정의한다.
typedef struct LinkedList {
	type_listNode_pointer head;
	int length;
} linkedList;

//함수선언
void printList(linkedList* L); //print 함수 선언
type_listNode_pointer search(linkedList* L, int x);// search 함수 선언
void insert(linkedList* L, listNode* pre, listData x);// insert 함수 선언
void delete(linkedList* L, int x);// delete 함수 선언
void reverse(linkedList* L);//reverse 함수 선언
int getLength(linkedList* L);// 리스트 노드 개수 출력 함수 선언
void insertLast(linkedList* L, listData item); //마지막 노드에 삽입하는 함수
linkedList* file_read(linkedList* L, char* txt_file); //파일 읽어오기
int menu(); //메뉴 함수 선언

int main(void) {
	linkedList LL = { NULL, 0 };
	int local;
	int exit = 0, Snum1;
	while (exit == 0) {
		switch (menu()) {
		case 1: // 파일 읽어오기
			file_read(&LL, "student_data.txt");
			printf(" 파일을 읽어 왔습니다. \n");
			continue;

		case 2: //리스트 출력
			printList(&LL);
			continue;
		case 3: //검색
			printf("ID 입력 : ");
			scanf("%d", &Snum1);
			//gets(Snum1);
			type_listNode_pointer temp = search(&LL, Snum1);
			if (temp != NULL) {
				printf("이름: %s 학번: %d 전화번호: %s 지역: %s \n ", temp->data.name, temp->data.id, temp->data.phone, temp->data.province);
			}
			else
			{
				printf("일치하는 학생이 없습니다.\n");

			}
			continue;
		case 4: //insert
			printf("학생 정보 입력(이름 학번 전화번호 지역) : ");
			listData New_data;
			scanf("%s %d %s %s", New_data.name, &New_data.id, New_data.phone, New_data.province);
			insertLast(&LL, New_data);
			printf("삽입 완료\n");
			break;
		case 5: // Delete
			printf("삭제할 학생의 학번 입력 : ");
			scanf("%d", &Snum1);
			delete(&LL, Snum1);
			printf("삭제 완료\n");
			continue;

		case 6: //reverse 함수 선언
			printf("재배치할 학생 입력 : ");
			scanf("%d", Snum1);
			reverse(&LL);
			continue;

		case 7: // 종료
			exit;
			break;

		}

		return 0;
	}
}


void printList(linkedList* L)//print 함수 정의 
{
	type_listNode_pointer p;
	printf("L = (\n");
	p = L->head;
	while (p) {
		printf("\n 이름 :% s 학번 : %d 전화번호 :%s 지역 : %s \n", p->data.name, p->data.id, p->data.phone, p->data.province);
		p = p->link;
	}
	printf("\n)\n");
}
type_listNode_pointer search(linkedList* L, int x) // search 함수 정의
{
	type_listNode_pointer temp = L->head;

	while (temp != NULL) {
		if (temp->data.id == x)
			return temp;
		else
			temp = temp->link;
	}
	return temp;
}
void insert(linkedList* L, listNode* pre, listData x) {

	listNode* newNode;
	newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data = x;

	if (L->head == NULL) {
		newNode->link = NULL;
		L->head = newNode;
	}
	else if (pre == NULL) {
		newNode->link = L->head;
		L->head = newNode;
	}
	else {
		newNode->link = pre->link;
		pre->link = newNode;
	}
	L->length++;

}

void delete(linkedList* L, int x) {// delete 함수 정의


	type_listNode_pointer pre = L->head;
	listNode* del;
	del = (listNode*)malloc(sizeof(listNode));
	del->data.id = x;

	if (L->head == NULL) {
		return FALSE;
	}
	if (x == NULL) return FALSE;
	if (L->head->data.id == x) {
		L->head = L->head->link;
		free(del);
	}
	else {
		pre = L->head;
		while (pre->link != x) {
			pre = pre->link;
			if (pre == NULL) return FALSE;
		}
		pre->link = L->head->link;
		free(del);
	}
	L->length--;

	return TRUE;
}
void reverse(linkedList* L) //reverse 함수 정의
{
	listNode* p, * q, * r;

	p = L->head;
	q = NULL;
	r = NULL;

	while (p != NULL) {
		r = q;
		q = p;
		p = p->link;
		q->link = r;
	}
	L->head = q;
}

void insertLast(linkedList* L, listData item) {
	listNode* newNode, * temp;
	newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data = item;
	newNode->link = NULL;

	if (L->head == NULL)
		L->head = newNode;
	else {
		temp = L->head;
		while (temp->link != NULL)
			temp = temp->link;
		temp->link = newNode;
	}
	L->length++;
}
int getLength(linkedList* L) // 리스트 노드 개수 출력 함수 정의
{
	return L->length;
}

linkedList* file_read(linkedList* L, char* txt_file) {
	FILE* pFile = fopen(txt_file, "r");
	if (pFile == NULL)
		printf("%s의 파일이 없어 정상적으로 뜨지 않습니다.", txt_file);
	else {
		char name[30]; //학생명
		int id = 0; //학번
		char phone[20]; //전화번호
		char province[30]; //거주지역
		while (!feof(pFile)) {

			fscanf(pFile, "%s %d %s %s \n", name, &id, phone, province);

			listNode* temp;
			listNode* newNode = (listNode*)malloc(sizeof(listNode));
			listData newData;

			strcpy(newData.name, name);
			newData.id = id;
			strcpy(newData.phone, phone);
			strcpy(newData.province, province);
			newNode->data = newData;
			newNode->link = NULL;

			insertLast(L, newData);
		}


	}
	fclose(pFile);
	return L;

}

int menu() {

	char string[5];
	printf("====================\n");
	printf("메뉴를 입력하세요.\n");
	printf("1번을 누르면 파일을 가져와 읽게 됩니다.\n");
	printf("2번을 누르면 가져온 파일 리스트를 출력합니다.\n");
	printf("3번을 누르면 검색이 됩니다.\n");
	printf("4번을 누르면 삽입 됩니다.\n");
	printf("5번을 누르면 삭제 됩니다.\n");
	printf("6번을 누르면 역순배치가 됩니다.\n");
	printf("7번을 누르면 종료가 됩니다.\n");


	scanf("%s", &string);
	//gets("%s", &string);

	int num_string = atoi(string);
	if (string == "exit")
	{
		return 7;
	}
	else if (0 < num_string && num_string < 8)
	{
		system("cls");
		return num_string;

	}
	else {
		system("cls");
		printf("%s는 잘못된 입력된 값입니다 \n", string);
		Sleep(Sleep_time);
	}
	return 0;
}
