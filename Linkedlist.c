//데이타구조론 hw 1번 과제
#define _CRT_SECURE_NO_WARNINGS //scanf 사용하기위해서 하지마 scanf_s사용하는걸 권장함
#include<stdio.h> //파일 입출력을 위해 헤더파일 stdio.h 선언
#include<string.h> // 문자열 선언을 위해 헤더파일 string.h 선언
#include<stdlib.h> //문자열 변환을 위해 선언
#include<windows.h> // system콜을 사용하기 위해 선언1
//공백
//삭제하는 경우를 참이면 1 거짓이면 0
#define TRUE 1 //1이면 TRUE이라고 정의
#define FALSE 0 // 0이면 FALSE 정의
#define Sleep_time 1000 //지정된 시간을 대기하기위해서 정의
//공백
// 구조체 데이터 타입 선언
typedef struct Listdata {  //구조체 타입 리스트데이터 선언
	char name[30]; //학생명
	int id; //학번
	char phone[20]; //전화번호
	char province[30]; //거주지역
} listData; //리스트 타입 명칭(별칭)선언 
//공백
//리스트 노드 정의
typedef struct Listnode* type_listNode_pointer; //리스트 노드 포인터 선언 
typedef struct Listnode { //리스트노드 구조체 선언 
	listData data; // 리스트데이터 데이터 선언 
	type_listNode_pointer link; //리스트노드 포인터에 링크 선언 
} listNode; //리스트노드 별칭 선언 
//공백
//리스트의 첫 번째 노드를 가리키는 head는 다음과 같이 정의한다.
typedef struct LinkedList { //연결리스트 구조체 선언 
	type_listNode_pointer head; //연결리스트 포인터 헤더 선언 
	int length; // 구조체 길이 선언 
} linkedList; // 별칭 선언
//공백
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
listNode* find(linkedList* L, int x); //삽입에서 중복삽입을 방지하기위해 비교하는 함수 FIND선언 

int main(void) { //메인함수 
	linkedList LL = { NULL, 0 }; //연결리스트의 처음 값을 공백으로 설정한다. 
	int exit = 0, Snum1; // exit,과 입력을 위해 Snum1(id)학번을 선언 한다.
	int Length; // getLength을 출력하기 위해서 선언 
	//공백 
	while (exit == 0) { // exit가 0으로 비교연산자 선언하여 0이면 종료인지 확인 
		switch (menu()) { //메뉴함수를 넣어 작동 하는지 확인 
		case 1: // 파일 읽어오기
			file_read(&LL, "./student_data.txt");// 파일리드로 텍스트 파일 불려오기
				printf(" 파일을 읽어 왔습니다. \n"); //성공적으로 불려오면 읽었다고 알려줌
				continue;// break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
			
		case 2: //리스트 출력
			printList(&LL); //파일이 가져온 리스트을 프린트하기 
			continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
		case 3: //검색
			printf("ID 입력 : "); //학번을 입력하는 칸 출력 
			scanf("%d", &Snum1); //학번id 입력 
			//gets(Snum1);
			type_listNode_pointer temp = search(&LL, Snum1); // 검색을 하여 학번과 일치하는 학생 찾기 
			if (temp != NULL) { // NULL값이 아니면 제대로 출력된지 아닌지 확인하는 조건문 
				printf("이름: %s 학번: %d 전화번호: %s 지역: %s \n ", temp->data.name, temp->data.id, temp->data.phone, temp->data.province); // 학생 이름,학번,전화번호,지역 출력문
			}
			else //검색했을때 학생정보가 없는 경우  
			{
				printf("일치하는 학생이 없습니다.\n"); //일치하는 학생이 없다고 출력하기.

			}
			continue;//break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성

		case 4: //삽입
			printf("학생 정보 입력(이름 학번 전화번호 지역) : "); //새로운 학생을 입력하기위한 출력문 
			listData New_data; //새로운 데이터를 리스트데이터에 선언 
			scanf("%s %d %s %s", New_data.name, &New_data.id, New_data.phone, New_data.province); //새로운 학생을 새로운 데이터에 입력

			if ( find(&LL,New_data.id) != NULL) { // 중복삽입 방지를 위한 if문  
				printf("이미 똑같은 학번이 있어 등록이 안됩니다. \n");//이미 데이터가 있어 등록이 안된다는문 출력.
				continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
			}
			else { //학생이 중복된 데이터가 없어서 새로운 학생이 성공적으로 등록되는 경우  
				insertLast(&LL, New_data); //새로운 학생은 마지막 데이터필드로 삽입 
				printf("삽입 완료 \n"); // 새로운 학생이 삽입 성공했다고 알리는 출력 문
				continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
			}
		case 5: //삭제
			printf("삭제할 학생의 학번 입력 : "); //삭제 할 학생의 학번 출력문
			scanf("%d", &Snum1); //삭제할 학생 학번 입력 

			if (find(&LL,Snum1) != FALSE ) { //학생이 데이터 있어서 삭제가 되는 조건문 
				delete(&LL, Snum1); //삭제하는 delete문 선언 학생 삭제하기
				printf("삭제 완료\n"); //성공적으로 삭제했으면 성공 
				continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
			}
			else { //명단에 없어 삭제가 안되는 경우
				printf("명단에 없어 삭제가 되지 않습니다. 삭제 오류\n"); //삭제 오류라고 뜨는 출력문 작성
				continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성
			}
		case 6: //reverse 
			reverse(&LL); //순서를 바꾸는 함수 위에있는 애랑 밑에있는애랑 바낌
			continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성

		case 7:
			Length = getLength(&LL); //데이터 필드의 개수를 출력하는 변수를 선언 
			printf("%d개\n", Length); //데이터 필드의 개수 출력
			continue; //break를 사용하면 스위치가 정지됨으로 계속 실행하기위해서 continue작성

		case 8: // 종료
			exit; //종료
			break; //스위치문 정지
			
		}

		return 0; //시스템 종료 
	}
}


void printList(linkedList* L)//print 함수 정의 
{
	type_listNode_pointer p; //리스트 노드 포인터 p선언 
	printf("L = (\n"); // L = (라고 출력 
	p = L->head; // p는 L의 헤드를 대입 
	while (p) { //포인터 p가 널이 아닐 때 까지 실행하는 반복문 
		printf("\n 이름 :% s 학번 : %d 전화번호 :%s 지역 : %s \n", p->data.name, p->data.id, p->data.phone, p->data.province); //현재 노드 데이터 출력
		p = p->link; //출력한다음 포인터 p는 다음 노드로 이동한다 
	}
	printf("\n)\n"); // )라고 출력하여 데이터 닫는다 
}
type_listNode_pointer search(linkedList* L, int x) // search 함수 정의
{
	type_listNode_pointer temp = L->head; //temp에 연결리스트 헤드 저장을 한다.

	while (temp != NULL) { // temp가 널이 아닐때 까지 반복 
		if (temp->data.id == x) //temp의 데이터와 x가 같은 경우 
			return temp; //temp에 해당 노드 반환 
		else //아니면 
			temp = temp->link; //다음 노드를 가르키도록 한다.
	}
	return temp;//x와 일치하는 값이 없으면 널값으로 반환한다. 
}
void insert(linkedList* L, listNode* pre, listData x) { //연결리스트 구조체 포인터 L, 삽입 위치를 가리키는 리스트노드 포인터 pre, 삽일할 데이터 x 선언
	// 새로운 노드생성한다.
	listNode* newNode;
	newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data = x;
	FILE* pFile = fopen("student.txt", "r+"); // w 로 파일을 쓸 경우 나중에 딜리트에서 삭제가 안되어 r+로 읽기,쓰기모드 실행
	
	//리스트가 비어있는 경우 
	if (L->head == NULL) {
		newNode->link = NULL;
		L->head = newNode;
	}
	//리스트의 맨 앞에 삽입하는 경우 
	else if (pre == NULL) {
		newNode->link = L->head;
		L->head = newNode;
	}
	//리스트의 중간에 삽입하는 경우 
	else {
		newNode->link = pre->link;
		pre->link = newNode;
	}
	L->length++; //리스트이 길이 증가 
	fclose(pFile);
}

void delete(linkedList* L, int x) {// delete 함수 정의 L는 삭제할 노드를 포함한 링크드 리스트 , x는 삭제할 노드의 학번


	type_listNode_pointer pre = L->head; //pre는 L의 헤드로 초기화 
	listNode* del; 
	del = (listNode*)malloc(sizeof(listNode)); //del 포인터에 리스트노드 크기만큼 선언 
	del -> data.id = x; //del의 data 구조체에 x값 대입 

	if (L->head == NULL) { // L이 비어있는 경우 
		return FALSE; //false로 반환 
	}
	if (x == NULL) return FALSE; //x가 널값인 경우 false로 반환 
	if (L->head->data.id == x) { //L의 헤드의 데이터 id값이 x와 같으면 
		L->head = L->head->link;  //L의 헤드를 L의 헤드의 링크로 대체 
		free(del); // del 메모리 해제 
	}
	else {
		pre = L->head; //pre를 L의 head로 대입 
		while (pre->link != x) { //pre의 링크가 x가 아닐 때 까지 반복  
			pre = pre->link;  //pre를 pre의 링크로 이동 
			if (pre == NULL) return FALSE; //pre가 널이경우 false로 반환 
		}
		pre->link = L->head->link; //pre의 링크를 L의 헤드의 링크로 대체
		free(del); // del 메모리 해제
	}
	
	L->length--; //L의 길이 감소 

	return TRUE; //TRUE 반환
}


void reverse(linkedList* L) //reverse 함수 정의
{
	listNode* p, * q, * r;

	p = L->head; //첫번째 노드를 가리키는 포인터 p
	q = NULL; //새로운 리스트의 헤드를 가리키는 포인터 q 
	r = NULL; //포인터 q가 리스트를 역순으로 연결할 포인터 r
	
	//리스트이 마지막 노드까지 반복 
	while (p != NULL) {
		r = q; // r는 q를
		q = p; // q는 p를 가르킨다 
		p = p->link; //p는 다음 노드를 가리킨다.
		q->link = r; //q의 링크 r 설정하여 새로운 리스트를 역순으로 연결한다 
	}
	L->head = q; //헤드를 새로운 역순 리스트의 헤드로 변경 
}

void insertLast(linkedList* L, listData item) {
	listNode* newNode, * temp;
	newNode = (listNode*)malloc(sizeof(listNode)); //뉴노드에 메모리 할당
	newNode->data = item; //노드에 데이터 할당 
	newNode->link = NULL; //노드의 링크는 널로 초기화

	if (L->head == NULL) //리스트가 비어있는경우 
		L->head = newNode; //리스트의 헤드를 새로운  노드로 지정 
	else {
		temp = L->head; // temp가 헤드를 가르킴 
		while (temp->link != NULL) //temp의링크가 널값 일 때 까지
			temp = temp->link; //temp의 링크는 따라 이동 
		temp->link = newNode; //마지막 노드의 링크에 새로운 노드를 지정 
	}
	L->length++; //리스트 길이 증가
}
int getLength(linkedList* L) // 리스트 노드 개수 출력 함수 정의
{
	return L->length; //렝스에 값을 반환 
}

linkedList* file_read(linkedList* L, char* txt_file) {
	FILE* pFile = fopen(txt_file, "r"); //txt_file로 파일을 열고 파일 포인터 pFile에 저장 
	if (pFile == NULL) //파일 없는 경우 
		printf("%s의 파일이 없어 정상적으로 뜨지 않습니다.", txt_file); //파일 없는경우 에러 메시지 출력
	else {
		char name[30]; //학생명
		int id = 0; //학번
		char phone[20]; //전화번호
		char province[30]; //거주지역
		while (!feof(pFile)) { //파일의 끝까지 반복 

			fscanf(pFile, "%s %d %s %s \n", name, &id, phone, province); //파일에서 한 줄식 읽어서 변수에 저장 

			listNode* temp;
			listNode* newNode = (listNode*)malloc(sizeof(listNode)); //동적메모리 할당 
			listData newData; //데이터를 저장할 구조체 변수 

			strcpy(newData.name, name); //학생명을 newData.name에 복사
			newData.id = id; //학번을 뉴데이터 아이디에 복사 
			strcpy(newData.phone, phone); // 전화번호를 뉴데이터 폰에 복사 
			strcpy(newData.province, province); //거주지역을 뉴데이터에 복사 
			newNode->data = newData; //새로운 노드 데이터 저장 
			newNode->link = NULL;//새로운 노드의 링크는 널로 초기화 

			insertLast(L, newData); //마지막 노드에 새로운 노드 추가 
		}


	}
	fclose(pFile); //파일 닫기 
	return L; //데이터가 추가된 연결리스트 포인터 반환

}

int menu() { //메뉴 함수 설정하기 

	char string[5]; //string 선언 
	printf("====================\n");
	printf("메뉴를 입력하세요.\n");
	printf("1번을 누르면 파일을 가져와 읽게 됩니다.\n");
	printf("2번을 누르면 가져온 파일 리스트를 출력합니다.\n");
	printf("3번을 누르면 검색이 됩니다.\n");
	printf("4번을 누르면 삽입 됩니다.\n");
	printf("5번을 누르면 삭제 됩니다.\n");
	printf("6번을 누르면 끝과 끝이 다시 배치가 됩니다.\n");
	printf("7번을 누르면 리스트 노드 개수가 출력 됩니다.\n");
	printf("8번을 누르면 종료가 됩니다.\n");
	//메뉴판을 만들어 1번 부터 8번까지 메뉴를 알려주는 출력문 출력 

	scanf("%s", &string); //번호 입력 사용자로부터 문자열 입력 받음 
	//gets("%s", &string);

	int num_string = atoi(string); //문자열을 정수형으로 반환하여 저장 
	if (string == "exit") //exit가 입력 된 경우 
	{
		return 8; //8을 반환하여 프로그램 종료 
	}
	else if (0 < num_string && num_string < 9) //1~8사이가 입력된 경우 
	{
		system("cls"); //화면 초기화
		return num_string; //입력된 정수 반환 

	}
	else { //아니면 
		system("cls"); //화면 초기화 
		printf("%s는 잘못된 입력된 값입니다 \n", string); //잘못된 입력임을 알리는 메세지 출력 
		Sleep(Sleep_time); //sleep_time만큼 대기
	}
	return 0; //0으로 반환하여 프로그램 다시 실행
}

listNode* find(linkedList* L, int x) { //연결리스트에서 학번을 찾아 반환하는 함수 선언 
	//L는 학생정보를 알고 있는 리스트포인터, x는 학생의 학번 

	listNode* temp = L->head; //탐색을위해 헤드노드를 가리키는 포인터 변수 temp 선언
	
	while (temp != NULL) { //temp가 null이 될 떄 까지 반복
		if (temp->data.id == x) { //temp가 가리키는 노드의 data 값이 x와 같으면 
			return temp; //해당 노드 반환하고 함수 종료 
		}
		temp = temp->link; // temp를 다음 노드로 이동
	}
	return NULL; //x를 찾지 못하면 NULL 반환

}