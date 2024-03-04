//����Ÿ������ hw 1�� ����
#define _CRT_SECURE_NO_WARNINGS //scanf ����ϱ����ؼ� ������ scanf_s����ϴ°� ������
#include<stdio.h> //���� ������� ���� ������� stdio.h ����
#include<string.h> // ���ڿ� ������ ���� ������� string.h ����
#include<stdlib.h> //���ڿ� ��ȯ�� ���� ����
#include<windows.h> // system���� ����ϱ� ���� ����1
//����
//�����ϴ� ��츦 ���̸� 1 �����̸� 0
#define TRUE 1 //1�̸� TRUE�̶�� ����
#define FALSE 0 // 0�̸� FALSE ����
#define Sleep_time 1000 //������ �ð��� ����ϱ����ؼ� ����
//����
// ����ü ������ Ÿ�� ����
typedef struct Listdata {  //����ü Ÿ�� ����Ʈ������ ����
	char name[30]; //�л���
	int id; //�й�
	char phone[20]; //��ȭ��ȣ
	char province[30]; //��������
} listData; //����Ʈ Ÿ�� ��Ī(��Ī)���� 
//����
//����Ʈ ��� ����
typedef struct Listnode* type_listNode_pointer; //����Ʈ ��� ������ ���� 
typedef struct Listnode { //����Ʈ��� ����ü ���� 
	listData data; // ����Ʈ������ ������ ���� 
	type_listNode_pointer link; //����Ʈ��� �����Ϳ� ��ũ ���� 
} listNode; //����Ʈ��� ��Ī ���� 
//����
//����Ʈ�� ù ��° ��带 ����Ű�� head�� ������ ���� �����Ѵ�.
typedef struct LinkedList { //���Ḯ��Ʈ ����ü ���� 
	type_listNode_pointer head; //���Ḯ��Ʈ ������ ��� ���� 
	int length; // ����ü ���� ���� 
} linkedList; // ��Ī ����
//����
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
listNode* find(linkedList* L, int x); //���Կ��� �ߺ������� �����ϱ����� ���ϴ� �Լ� FIND���� 

int main(void) { //�����Լ� 
	linkedList LL = { NULL, 0 }; //���Ḯ��Ʈ�� ó�� ���� �������� �����Ѵ�. 
	int exit = 0, Snum1; // exit,�� �Է��� ���� Snum1(id)�й��� ���� �Ѵ�.
	int Length; // getLength�� ����ϱ� ���ؼ� ���� 
	//���� 
	while (exit == 0) { // exit�� 0���� �񱳿����� �����Ͽ� 0�̸� �������� Ȯ�� 
		switch (menu()) { //�޴��Լ��� �־� �۵� �ϴ��� Ȯ�� 
		case 1: // ���� �о����
			file_read(&LL, "./student_data.txt");// ���ϸ���� �ؽ�Ʈ ���� �ҷ�����
				printf(" ������ �о� �Խ��ϴ�. \n"); //���������� �ҷ����� �о��ٰ� �˷���
				continue;// break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
			
		case 2: //����Ʈ ���
			printList(&LL); //������ ������ ����Ʈ�� ����Ʈ�ϱ� 
			continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
		case 3: //�˻�
			printf("ID �Է� : "); //�й��� �Է��ϴ� ĭ ��� 
			scanf("%d", &Snum1); //�й�id �Է� 
			//gets(Snum1);
			type_listNode_pointer temp = search(&LL, Snum1); // �˻��� �Ͽ� �й��� ��ġ�ϴ� �л� ã�� 
			if (temp != NULL) { // NULL���� �ƴϸ� ����� ��µ��� �ƴ��� Ȯ���ϴ� ���ǹ� 
				printf("�̸�: %s �й�: %d ��ȭ��ȣ: %s ����: %s \n ", temp->data.name, temp->data.id, temp->data.phone, temp->data.province); // �л� �̸�,�й�,��ȭ��ȣ,���� ��¹�
			}
			else //�˻������� �л������� ���� ���  
			{
				printf("��ġ�ϴ� �л��� �����ϴ�.\n"); //��ġ�ϴ� �л��� ���ٰ� ����ϱ�.

			}
			continue;//break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�

		case 4: //����
			printf("�л� ���� �Է�(�̸� �й� ��ȭ��ȣ ����) : "); //���ο� �л��� �Է��ϱ����� ��¹� 
			listData New_data; //���ο� �����͸� ����Ʈ�����Ϳ� ���� 
			scanf("%s %d %s %s", New_data.name, &New_data.id, New_data.phone, New_data.province); //���ο� �л��� ���ο� �����Ϳ� �Է�

			if ( find(&LL,New_data.id) != NULL) { // �ߺ����� ������ ���� if��  
				printf("�̹� �Ȱ��� �й��� �־� ����� �ȵ˴ϴ�. \n");//�̹� �����Ͱ� �־� ����� �ȵȴٴ¹� ���.
				continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
			}
			else { //�л��� �ߺ��� �����Ͱ� ��� ���ο� �л��� ���������� ��ϵǴ� ���  
				insertLast(&LL, New_data); //���ο� �л��� ������ �������ʵ�� ���� 
				printf("���� �Ϸ� \n"); // ���ο� �л��� ���� �����ߴٰ� �˸��� ��� ��
				continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
			}
		case 5: //����
			printf("������ �л��� �й� �Է� : "); //���� �� �л��� �й� ��¹�
			scanf("%d", &Snum1); //������ �л� �й� �Է� 

			if (find(&LL,Snum1) != FALSE ) { //�л��� ������ �־ ������ �Ǵ� ���ǹ� 
				delete(&LL, Snum1); //�����ϴ� delete�� ���� �л� �����ϱ�
				printf("���� �Ϸ�\n"); //���������� ���������� ���� 
				continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
			}
			else { //��ܿ� ���� ������ �ȵǴ� ���
				printf("��ܿ� ���� ������ ���� �ʽ��ϴ�. ���� ����\n"); //���� ������� �ߴ� ��¹� �ۼ�
				continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�
			}
		case 6: //reverse 
			reverse(&LL); //������ �ٲٴ� �Լ� �����ִ� �ֶ� �ؿ��ִ¾ֶ� �ٳ�
			continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�

		case 7:
			Length = getLength(&LL); //������ �ʵ��� ������ ����ϴ� ������ ���� 
			printf("%d��\n", Length); //������ �ʵ��� ���� ���
			continue; //break�� ����ϸ� ����ġ�� ���������� ��� �����ϱ����ؼ� continue�ۼ�

		case 8: // ����
			exit; //����
			break; //����ġ�� ����
			
		}

		return 0; //�ý��� ���� 
	}
}


void printList(linkedList* L)//print �Լ� ���� 
{
	type_listNode_pointer p; //����Ʈ ��� ������ p���� 
	printf("L = (\n"); // L = (��� ��� 
	p = L->head; // p�� L�� ��带 ���� 
	while (p) { //������ p�� ���� �ƴ� �� ���� �����ϴ� �ݺ��� 
		printf("\n �̸� :% s �й� : %d ��ȭ��ȣ :%s ���� : %s \n", p->data.name, p->data.id, p->data.phone, p->data.province); //���� ��� ������ ���
		p = p->link; //����Ѵ��� ������ p�� ���� ���� �̵��Ѵ� 
	}
	printf("\n)\n"); // )��� ����Ͽ� ������ �ݴ´� 
}
type_listNode_pointer search(linkedList* L, int x) // search �Լ� ����
{
	type_listNode_pointer temp = L->head; //temp�� ���Ḯ��Ʈ ��� ������ �Ѵ�.

	while (temp != NULL) { // temp�� ���� �ƴҶ� ���� �ݺ� 
		if (temp->data.id == x) //temp�� �����Ϳ� x�� ���� ��� 
			return temp; //temp�� �ش� ��� ��ȯ 
		else //�ƴϸ� 
			temp = temp->link; //���� ��带 ����Ű���� �Ѵ�.
	}
	return temp;//x�� ��ġ�ϴ� ���� ������ �ΰ����� ��ȯ�Ѵ�. 
}
void insert(linkedList* L, listNode* pre, listData x) { //���Ḯ��Ʈ ����ü ������ L, ���� ��ġ�� ����Ű�� ����Ʈ��� ������ pre, ������ ������ x ����
	// ���ο� �������Ѵ�.
	listNode* newNode;
	newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data = x;
	FILE* pFile = fopen("student.txt", "r+"); // w �� ������ �� ��� ���߿� ����Ʈ���� ������ �ȵǾ� r+�� �б�,������ ����
	
	//����Ʈ�� ����ִ� ��� 
	if (L->head == NULL) {
		newNode->link = NULL;
		L->head = newNode;
	}
	//����Ʈ�� �� �տ� �����ϴ� ��� 
	else if (pre == NULL) {
		newNode->link = L->head;
		L->head = newNode;
	}
	//����Ʈ�� �߰��� �����ϴ� ��� 
	else {
		newNode->link = pre->link;
		pre->link = newNode;
	}
	L->length++; //����Ʈ�� ���� ���� 
	fclose(pFile);
}

void delete(linkedList* L, int x) {// delete �Լ� ���� L�� ������ ��带 ������ ��ũ�� ����Ʈ , x�� ������ ����� �й�


	type_listNode_pointer pre = L->head; //pre�� L�� ���� �ʱ�ȭ 
	listNode* del; 
	del = (listNode*)malloc(sizeof(listNode)); //del �����Ϳ� ����Ʈ��� ũ�⸸ŭ ���� 
	del -> data.id = x; //del�� data ����ü�� x�� ���� 

	if (L->head == NULL) { // L�� ����ִ� ��� 
		return FALSE; //false�� ��ȯ 
	}
	if (x == NULL) return FALSE; //x�� �ΰ��� ��� false�� ��ȯ 
	if (L->head->data.id == x) { //L�� ����� ������ id���� x�� ������ 
		L->head = L->head->link;  //L�� ��带 L�� ����� ��ũ�� ��ü 
		free(del); // del �޸� ���� 
	}
	else {
		pre = L->head; //pre�� L�� head�� ���� 
		while (pre->link != x) { //pre�� ��ũ�� x�� �ƴ� �� ���� �ݺ�  
			pre = pre->link;  //pre�� pre�� ��ũ�� �̵� 
			if (pre == NULL) return FALSE; //pre�� ���̰�� false�� ��ȯ 
		}
		pre->link = L->head->link; //pre�� ��ũ�� L�� ����� ��ũ�� ��ü
		free(del); // del �޸� ����
	}
	
	L->length--; //L�� ���� ���� 

	return TRUE; //TRUE ��ȯ
}


void reverse(linkedList* L) //reverse �Լ� ����
{
	listNode* p, * q, * r;

	p = L->head; //ù��° ��带 ����Ű�� ������ p
	q = NULL; //���ο� ����Ʈ�� ��带 ����Ű�� ������ q 
	r = NULL; //������ q�� ����Ʈ�� �������� ������ ������ r
	
	//����Ʈ�� ������ ������ �ݺ� 
	while (p != NULL) {
		r = q; // r�� q��
		q = p; // q�� p�� ����Ų�� 
		p = p->link; //p�� ���� ��带 ����Ų��.
		q->link = r; //q�� ��ũ r �����Ͽ� ���ο� ����Ʈ�� �������� �����Ѵ� 
	}
	L->head = q; //��带 ���ο� ���� ����Ʈ�� ���� ���� 
}

void insertLast(linkedList* L, listData item) {
	listNode* newNode, * temp;
	newNode = (listNode*)malloc(sizeof(listNode)); //����忡 �޸� �Ҵ�
	newNode->data = item; //��忡 ������ �Ҵ� 
	newNode->link = NULL; //����� ��ũ�� �η� �ʱ�ȭ

	if (L->head == NULL) //����Ʈ�� ����ִ°�� 
		L->head = newNode; //����Ʈ�� ��带 ���ο�  ���� ���� 
	else {
		temp = L->head; // temp�� ��带 ����Ŵ 
		while (temp->link != NULL) //temp�Ǹ�ũ�� �ΰ� �� �� ����
			temp = temp->link; //temp�� ��ũ�� ���� �̵� 
		temp->link = newNode; //������ ����� ��ũ�� ���ο� ��带 ���� 
	}
	L->length++; //����Ʈ ���� ����
}
int getLength(linkedList* L) // ����Ʈ ��� ���� ��� �Լ� ����
{
	return L->length; //������ ���� ��ȯ 
}

linkedList* file_read(linkedList* L, char* txt_file) {
	FILE* pFile = fopen(txt_file, "r"); //txt_file�� ������ ���� ���� ������ pFile�� ���� 
	if (pFile == NULL) //���� ���� ��� 
		printf("%s�� ������ ���� ���������� ���� �ʽ��ϴ�.", txt_file); //���� ���°�� ���� �޽��� ���
	else {
		char name[30]; //�л���
		int id = 0; //�й�
		char phone[20]; //��ȭ��ȣ
		char province[30]; //��������
		while (!feof(pFile)) { //������ ������ �ݺ� 

			fscanf(pFile, "%s %d %s %s \n", name, &id, phone, province); //���Ͽ��� �� �ٽ� �о ������ ���� 

			listNode* temp;
			listNode* newNode = (listNode*)malloc(sizeof(listNode)); //�����޸� �Ҵ� 
			listData newData; //�����͸� ������ ����ü ���� 

			strcpy(newData.name, name); //�л����� newData.name�� ����
			newData.id = id; //�й��� �������� ���̵� ���� 
			strcpy(newData.phone, phone); // ��ȭ��ȣ�� �������� ���� ���� 
			strcpy(newData.province, province); //���������� �������Ϳ� ���� 
			newNode->data = newData; //���ο� ��� ������ ���� 
			newNode->link = NULL;//���ο� ����� ��ũ�� �η� �ʱ�ȭ 

			insertLast(L, newData); //������ ��忡 ���ο� ��� �߰� 
		}


	}
	fclose(pFile); //���� �ݱ� 
	return L; //�����Ͱ� �߰��� ���Ḯ��Ʈ ������ ��ȯ

}

int menu() { //�޴� �Լ� �����ϱ� 

	char string[5]; //string ���� 
	printf("====================\n");
	printf("�޴��� �Է��ϼ���.\n");
	printf("1���� ������ ������ ������ �а� �˴ϴ�.\n");
	printf("2���� ������ ������ ���� ����Ʈ�� ����մϴ�.\n");
	printf("3���� ������ �˻��� �˴ϴ�.\n");
	printf("4���� ������ ���� �˴ϴ�.\n");
	printf("5���� ������ ���� �˴ϴ�.\n");
	printf("6���� ������ ���� ���� �ٽ� ��ġ�� �˴ϴ�.\n");
	printf("7���� ������ ����Ʈ ��� ������ ��� �˴ϴ�.\n");
	printf("8���� ������ ���ᰡ �˴ϴ�.\n");
	//�޴����� ����� 1�� ���� 8������ �޴��� �˷��ִ� ��¹� ��� 

	scanf("%s", &string); //��ȣ �Է� ����ڷκ��� ���ڿ� �Է� ���� 
	//gets("%s", &string);

	int num_string = atoi(string); //���ڿ��� ���������� ��ȯ�Ͽ� ���� 
	if (string == "exit") //exit�� �Է� �� ��� 
	{
		return 8; //8�� ��ȯ�Ͽ� ���α׷� ���� 
	}
	else if (0 < num_string && num_string < 9) //1~8���̰� �Էµ� ��� 
	{
		system("cls"); //ȭ�� �ʱ�ȭ
		return num_string; //�Էµ� ���� ��ȯ 

	}
	else { //�ƴϸ� 
		system("cls"); //ȭ�� �ʱ�ȭ 
		printf("%s�� �߸��� �Էµ� ���Դϴ� \n", string); //�߸��� �Է����� �˸��� �޼��� ��� 
		Sleep(Sleep_time); //sleep_time��ŭ ���
	}
	return 0; //0���� ��ȯ�Ͽ� ���α׷� �ٽ� ����
}

listNode* find(linkedList* L, int x) { //���Ḯ��Ʈ���� �й��� ã�� ��ȯ�ϴ� �Լ� ���� 
	//L�� �л������� �˰� �ִ� ����Ʈ������, x�� �л��� �й� 

	listNode* temp = L->head; //Ž�������� ����带 ����Ű�� ������ ���� temp ����
	
	while (temp != NULL) { //temp�� null�� �� �� ���� �ݺ�
		if (temp->data.id == x) { //temp�� ����Ű�� ����� data ���� x�� ������ 
			return temp; //�ش� ��� ��ȯ�ϰ� �Լ� ���� 
		}
		temp = temp->link; // temp�� ���� ���� �̵�
	}
	return NULL; //x�� ã�� ���ϸ� NULL ��ȯ

}