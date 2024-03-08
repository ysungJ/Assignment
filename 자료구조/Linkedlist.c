//2019253084 ������ ����Ÿ������ hw 1�� ����
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //���� ������� ���� ������� stdio.h ����
#include<string.h> // ���ڿ� ������ ���� ������� string.h ����
#include<stdlib.h> 
#include<windows.h>

//�����ϴ� ��츦 ���̸� 1 �����̸� 0
#define TRUE 1
#define FALSE 0
#define Sleep_time 1000

// ����ü ������ Ÿ�� ����
typedef struct Listdata {
	char name[30]; //�л���
	int id; //�й�
	char phone[20]; //��ȭ��ȣ
	char province[30]; //��������
} listData;

//����Ʈ ��� ����
typedef struct Listnode* type_listNode_pointer;
typedef struct Listnode {
	listData data;
	type_listNode_pointer link;
} listNode;


//����Ʈ�� ù ��° ��带 ����Ű�� head�� ������ ���� �����Ѵ�.
typedef struct LinkedList {
	type_listNode_pointer head;
	int length;
} linkedList;

//�Լ�����
void printList(linkedList* L); //print �Լ� ����
type_listNode_pointer search(linkedList* L, int x);// search �Լ� ����
void insert(linkedList* L, listNode* pre, listData x);// insert �Լ� ����
void delete(linkedList* L, int x);// delete �Լ� ����
void reverse(linkedList* L);//reverse �Լ� ����
int getLength(linkedList* L);// ����Ʈ ��� ���� ��� �Լ� ����
void insertLast(linkedList* L, listData item); //������ ��忡 �����ϴ� �Լ�
linkedList* file_read(linkedList* L, char* txt_file); //���� �о����
int menu(); //�޴� �Լ� ����

int main(void) {
	linkedList LL = { NULL, 0 };
	int local;
	int exit = 0, Snum1;
	while (exit == 0) {
		switch (menu()) {
		case 1: // ���� �о����
			file_read(&LL, "student_data.txt");
			printf(" ������ �о� �Խ��ϴ�. \n");
			continue;

		case 2: //����Ʈ ���
			printList(&LL);
			continue;
		case 3: //�˻�
			printf("ID �Է� : ");
			scanf("%d", &Snum1);
			//gets(Snum1);
			type_listNode_pointer temp = search(&LL, Snum1);
			if (temp != NULL) {
				printf("�̸�: %s �й�: %d ��ȭ��ȣ: %s ����: %s \n ", temp->data.name, temp->data.id, temp->data.phone, temp->data.province);
			}
			else
			{
				printf("��ġ�ϴ� �л��� �����ϴ�.\n");

			}
			continue;
		case 4: //insert
			printf("�л� ���� �Է�(�̸� �й� ��ȭ��ȣ ����) : ");
			listData New_data;
			scanf("%s %d %s %s", New_data.name, &New_data.id, New_data.phone, New_data.province);
			insertLast(&LL, New_data);
			printf("���� �Ϸ�\n");
			break;
		case 5: // Delete
			printf("������ �л��� �й� �Է� : ");
			scanf("%d", &Snum1);
			delete(&LL, Snum1);
			printf("���� �Ϸ�\n");
			continue;

		case 6: //reverse �Լ� ����
			printf("���ġ�� �л� �Է� : ");
			scanf("%d", Snum1);
			reverse(&LL);
			continue;

		case 7: // ����
			exit;
			break;

		}

		return 0;
	}
}


void printList(linkedList* L)//print �Լ� ���� 
{
	type_listNode_pointer p;
	printf("L = (\n");
	p = L->head;
	while (p) {
		printf("\n �̸� :% s �й� : %d ��ȭ��ȣ :%s ���� : %s \n", p->data.name, p->data.id, p->data.phone, p->data.province);
		p = p->link;
	}
	printf("\n)\n");
}
type_listNode_pointer search(linkedList* L, int x) // search �Լ� ����
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

void delete(linkedList* L, int x) {// delete �Լ� ����


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
void reverse(linkedList* L) //reverse �Լ� ����
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
int getLength(linkedList* L) // ����Ʈ ��� ���� ��� �Լ� ����
{
	return L->length;
}

linkedList* file_read(linkedList* L, char* txt_file) {
	FILE* pFile = fopen(txt_file, "r");
	if (pFile == NULL)
		printf("%s�� ������ ���� ���������� ���� �ʽ��ϴ�.", txt_file);
	else {
		char name[30]; //�л���
		int id = 0; //�й�
		char phone[20]; //��ȭ��ȣ
		char province[30]; //��������
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
	printf("�޴��� �Է��ϼ���.\n");
	printf("1���� ������ ������ ������ �а� �˴ϴ�.\n");
	printf("2���� ������ ������ ���� ����Ʈ�� ����մϴ�.\n");
	printf("3���� ������ �˻��� �˴ϴ�.\n");
	printf("4���� ������ ���� �˴ϴ�.\n");
	printf("5���� ������ ���� �˴ϴ�.\n");
	printf("6���� ������ ������ġ�� �˴ϴ�.\n");
	printf("7���� ������ ���ᰡ �˴ϴ�.\n");


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
		printf("%s�� �߸��� �Էµ� ���Դϴ� \n", string);
		Sleep(Sleep_time);
	}
	return 0;
}