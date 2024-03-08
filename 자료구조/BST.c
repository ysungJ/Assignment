#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<stdbool.h>

//����ü ����
typedef struct anod* TreePointer;

typedef struct anod { //�� ����� ���� 

	int sno; //�й�
	char name[50]; //�̸� 
	TreePointer leftChild, rightChild; // ��� �ڽ� ������
} Ty_Node; // ��庰Ī

//�Լ� ���� 
bool insert(TreePointer* node, char key[50], int sno);
TreePointer modifiedSearch(TreePointer tree, char key[50]);
void inorder(TreePointer ptr);
int pre_order_ht(TreePointer curr, int lvl);
TreePointer search_BST(TreePointer curr, char key[50], TreePointer* par);
void BST_Delete(TreePointer* root, char key[50]);
int leaf_count(TreePointer tree);
void ch(TreePointer root, char key[50]);
int get_ht(TreePointer curr, char key[50], int count);

//���� �Լ� 
int main(void) {
	int ht, index, sno; //����,�迭�� �ε���, �л� �й� ����
	char* pt;
	char order[50], arr[3][50], name[50];
	bool a;
	
	FILE* fp;
	fp = fopen("sdata.txt", "r");
	
	TreePointer root = NULL, parent = NULL, tmp = NULL;

	//���� ����,�ݱ� 
	if (fp == NULL) {
		printf("������ �����ϴ�. \n");
		return 0;
	}
	else {
		//���Ͽ��� �л������� �а�, ���� Ž��Ʈ���� �߰�
		while (!feof(fp)) {
			int res = fscanf(fp, "%d %s", &sno, &name); //���� �й�, �̸� �Է��ϱ�
			if (res < 2)
						break;
			insert(&root, name, sno);
		}
	

		
	}

		//main roof
	while (1) {
		printf("������ �۾��� (in, sp, de, se, ht, ch, le, ex) ? ");
		 gets(order);
	
		index = 0;
		// �Է¹��� �˱�
		pt = strtok(order," "); // ���ڿ� �ڸ��� 
		
		while (pt != NULL)
		{
			strcpy(arr[index++], pt); //���ڿ� ����
			pt = strtok(NULL," ");

		}
		//bst�� �Է��� ������ �߰� 
		if (strcmp(arr[0],"in") == 0) { //���ڿ��� 
			a = false;
			sno = atoi(arr[1]); //string to in
			strcpy(name, arr[2]);
			a = insert(&root, name, sno); //�л� ������ ���� ����
			if (a == true)
				printf("�Է� ����! level = %d \n", get_ht(root, name, 0)); //������ ������, �Է¼����� �Բ� Ʈ�� ���� ���
		}
		//��� ������ ����
		else if (strcmp(arr[0],"sp") == 0)
		{
			system("cls");
			printf("������ ������ ������ �����ϴ� \n");
			inorder(root); //���� ��ȸ�� ��� ������ ���
	
		}
		//�Է��� ������ ����
		else if (strcmp(arr[0],"de") == 0) {
			strcpy(name, arr[1]);
			BST_Delete(&root, name); //���Ե� ������ ����
		}
	//�Է��� �����Ͱ� �ִ��� Ȯ�� 
		else if (strcmp(arr[0],"se") == 0) {
			strcpy(name, arr[1]);
			tmp = search_BST(root, name, &parent); //������ Ž�� ����
			//�����Ͱ� ������ �̸�,�й�, ����(��ġ) ����ϱ�
			printf("�̸�: %s, �й�: %d, ������ġ = %d \n", tmp->name, tmp->sno, get_ht(root, name,0)); 
		
		}
		//Ʈ�� ���� ���
		else if (strcmp(arr[0],"ht") == 0)
		{
			ht = pre_order_ht(root, 0); //preorder�� ���� ��� ����
			printf("height = %d\n", ht);
		}
		//�Է¹��� ���� ���ϵ� ���
		else if (strcmp(arr[0],"ch") == 0)
		{
			strcpy(name, arr[1]);
			ch(root, name); //�Է��� ���� �ڽ� ���
		}
		//��� ���� 
		else if (strcmp(arr[0],"le") == 0)
		{
			printf("number of leaf node = %d\n", leaf_count(root)); //���(��)�� ���� ���
		}
		//���α׷� ����
		else if (strcmp(arr[0],"ex") == 0)
		{
			printf("���α׷��� �����մϴ�!\n");
			system("pause");
			return 0;
		}

	}

	return 0;
}

//�����Լ�
bool insert(TreePointer* node, char key[50], int sno) {
	//���ο� ��� ����, �ʱ�ȭ
	TreePointer ptr, temp;
	ptr = (TreePointer)malloc(sizeof(*ptr));
	strcpy(ptr->name,key); //�� ����� �̸�
	ptr->sno = sno; //�� ����� �й� 
	
	ptr->leftChild = ptr->rightChild = NULL;  // �� ����� �ڽ� ��带 NULL�� �ʱ�ȭ

	if (!(*node)) {// �� Ʈ���� ���, �� ��带 ��Ʈ ���� ����
		*node = ptr;
	}
	else //��Ʈ���� �ƴ� ���
	{
		temp = modifiedSearch(*node, key); //�� ��带 ���� ��� Ž�� 
		
		if (!temp) { // ������ �̸��� ���� ��尡 �̹� �����ϴ� ���
			printf("���� Ű�� ��尡 �̹� �����Ͽ� ���� ���� \n");
			return false; //���� ����
		}
		else { //temp�� ����Ű�� ��忡 �� ��带 ����.
			if (strcmp(key, temp->name) < 0) { // �� ����� �̸��� �θ� ����� �̸����� ���� ���
				temp->leftChild = ptr; // ���� �ڽ� ���� ����
				return true; // ���� ����
			}

			else { // �� ����� �̸��� �θ� ����� �̸����� ū ���
				
				temp->rightChild = ptr; // ������ �ڽ� ���� ����
				return true; //���� ����
			}

		}
	}
	
}

//������ �������� Ž���Ͽ� �������� Ȯ���ϴ� �Լ�  
TreePointer modifiedSearch(TreePointer tree, char key[50]) {
	TreePointer temp = tree;  //���� ��带 ����Ű�� ������ ������ temp�� �����ϰ� �ʱ�ȭ
	while (temp) { // temp�� ���� �ƴҶ� ���� �ݺ� 
		if (strcmp(key, temp->name) == 0) { // key ���� �̸��� ���� ��쿡
			// �ߺ��� Ű ���� ã�� ��쿡 �� ������ ��ȯ 
			return NULL;
		} 
		else if (strcmp(key, temp->name) < 0) {  // key ���� ����� �̸����� ������
			if (temp->leftChild == NULL) { //���� �ڽ� ��尡 �ΰ��̸�
				return temp; //���� ���ο� ��ȯ
			}
			else { 
				temp = temp->leftChild; //�ƴϸ� ������ ��带 ���� �ڽ� ���� �̵� 
			}
		}
		else {
			if (temp->rightChild == NULL) { //������ �ڽ��� �ΰ��̸�
				return temp; //���� ��忡 ��ȯ
			}
			else {
				temp = temp->rightChild; //�ƴϸ� ������ ��带 ������ �ڽ� ���� �̵�
			}
		}
	}
	return temp; //temp ���� �ΰ��̸� ���� ������� ��ȯ
}

//inodrer(������ȸ) �Լ�  ���� �Ѹ� ������ ���
void inorder(TreePointer ptr) {
	if (ptr)
	{
		inorder(ptr->leftChild); //���� 
		printf("%s, %d\n", ptr->name, ptr->sno); // �Ѹ�
		inorder(ptr->rightChild); // ������
	}

}

//preorder(������ȸ)�� �̿��� hight (����) �Լ� 
int pre_order_ht(TreePointer curr, int lvl) {
	if (curr == NULL) // ���� ��尡 null �϶�
	{
		return lvl; // ���� ������ ��ȯ

	}
	/*���� ����Ʈ���� ���̿� ������ ����Ʈ���� ���̸� ���ϰ� 
	�� ���� ���� ���� Ʈ���� ���̷� ��ȯ*/
	int left_ht, right_ht;
	left_ht = pre_order_ht(curr->leftChild, lvl + 1);
	right_ht = pre_order_ht(curr->rightChild, lvl + 1);
	return (left_ht > right_ht ? left_ht : right_ht) ;
}

//����Ʈ�� Ž�� �ƻ����� 
TreePointer search_BST(TreePointer curr, char key[50], TreePointer* par) {
	

	*par = NULL; // �θ� ��� ������ �ʱ�ȭ
	while (curr != NULL){ // ���� ��尡 null�� �� ������ �ݺ�
		// ã���� �ϴ� Ű ���� ���� ����� Ű ���� ��
		if (strcmp(key, curr->name) == 0){  //��ġ�ϴ� ���
			return curr; //���� ��� ��ȯ 
		}
		else if (strcmp(key, curr->name) < 0) { //Ű���� ���� ��� ���� ����Ʈ�� Ž��
			*par = curr ; //���� ��带 �θ���� ����
			curr = curr->leftChild; //���� ��带 ���� �ڽ� ���� ���� 
		}
		else { //�ƴϸ� -> Ű ���� Ŭ ��� ������ ����Ʈ�� Ž��
			*par = curr; //���� ��带 �θ���� ����
			curr = curr->rightChild; //���� ��带 ������ �ڽ� ���� ���� 
		}
	}
	
	return NULL; //ã�� Ű���� ���� ��� null������ ��ȯ

}

//BST ���� �Լ� ����
void BST_Delete(TreePointer* root, char key[50]) {

	TreePointer parent = NULL;
	TreePointer curr = search_BST(*root, key, &parent); // Ű�� ���� ���� �� �θ� ��� Ž��

	if (curr == NULL) { //Ž�� ������ ��� (���� ��尡 �ΰ��� ���)
		printf("�׷� �л��� �� ã�� �� �����ϴ�. �ٽ� Ȯ���غ����� \n");
		return;
	}

 // ������ ����� ������ 0�� ���
	if (curr->leftChild == NULL && curr->rightChild == NULL) {
		if (parent->leftChild == curr) { // ������ ��尡 �θ� ����� ���� �ڽ��� ���
			parent->leftChild = NULL; // �ΰ����� ��ȯ
		}
		else { // ������ ��尡 �θ� ����� ������ �ڽ��� ���
			parent->rightChild = NULL; // ������ �ڽ��� �� ������ ��ȯ
		}
		free(curr); // ������ ��� �޸� ����
		printf("���������� �����Ǿ����ϴ�.\n");
	}
	// ������ ����� ������ 1�� ���
	else if (curr->leftChild == NULL || curr->rightChild == NULL) {
		TreePointer child = (curr->leftChild != NULL) ? curr->leftChild : curr->rightChild; // ������ ����� �ڽ� ��� Ž��

		if (parent->leftChild == curr) { // ������ ��尡 �θ� ����� ���� �ڽ��� ���
			parent->leftChild = child; // ������ ��带 �ڽ� ���� ��ü
		}
		else { // ������ ��尡 �θ� ����� ������ �ڽ��� ���
			parent->rightChild = child; // ������ ��带 �ڽ� ���� ��ü
		}

		printf("���������� �����Ǿ����ϴ�.\n");
		printf("�̵��� �л�: %s\n", child->name); // �̵��� �л��� �̸� ���
		free(curr); // ������ ��� �޸� ����
	}
	// ������ ����� ������ 2�� ���
	else {
		
		TreePointer maxNode = curr->leftChild;
		TreePointer maxNodeParent = curr; // �İ��� ����� �θ� ���
		
		//�İ��� ��� Ž���� ���� 
		//������ ����� ���� ����Ʈ������ ���� ū ��带 ã��
		while (maxNode->rightChild != NULL) {
			maxNodeParent = maxNode;
			maxNode = maxNode->rightChild;
		}

		strcpy(curr->name, maxNode->name);

		// �İ��� ����� ������ 1�� ���
		if (maxNode->leftChild != NULL) {
			if (maxNodeParent->leftChild == maxNode) { // �İ��� ��尡 �θ� ����� ���� �ڽ��� ���
				maxNodeParent->leftChild = maxNode->leftChild; // �İ��� ��带 �����ϰ� ���� �ڽ����� ��ü
			}
			else { // �İ��� ��尡 �θ� ����� ������ �ڽ��� ���
				maxNodeParent->rightChild = maxNode->leftChild; // �İ��� ��带 �����ϰ� ���� �ڽ����� ��ü
			}
		}
		else { // �İ��� ����� ������ 0�� ���
			if (maxNodeParent->leftChild == maxNode) { // �İ��� ��尡 �θ� ����� ���� �ڽ��� ���
				maxNodeParent->leftChild = NULL; // �İ��� ��带 �����ϰ� �ΰ����� ��ȯ
			}
			else { // �İ��� ��尡 �θ� ����� ������ �ڽ��� ���
				maxNodeParent->rightChild = NULL; // �İ��� ��带 �����ϰ� �ΰ����� ��ȯ
			}
		}
	
		printf("���������� �����Ǿ����ϴ�.\n");
		printf("�̵��� �л�: %s\n", maxNode->name); // �̵��� �л��� �̸� ���
		free(maxNode); // �İ��� ��� �޸� ����
	}

}

//Ʈ�� �� ���� ���ϱ� 
int leaf_count(TreePointer tree) {

	if (tree == NULL) { // �� Ʈ���� ��� 0 ��ȯ
		return 0;
	}
	else if (tree->leftChild == NULL && tree->rightChild == NULL) { // �� ����� ��� 1 ��ȯ
		return 1;
	}
	else {
		return leaf_count(tree->leftChild) + leaf_count(tree->rightChild); // ��������� �¿� ����Ʈ������ �� ��� ������ ���ؼ� ���ϱ�
	}
	}

//�Է¹��� ���� ���ϵ� ��� 
void ch(TreePointer root, char key[50]) {
	TreePointer par = NULL; // �θ� ��带 ����Ű�� ������ 
	TreePointer curr = search_BST(root,key,&par); // �Էµ� Ű���� ������ ��带 ã��
	
	if (curr == NULL) { //���� ��尡 ���� ��� 
		printf("��尡 ���� ���� �ʽ��ϴ�. \n");
		return; 

	}

	 else if (curr->leftChild  == NULL && curr->rightChild == NULL) { //���� ����� �ڽ��� �� �� ���� ��� 
		printf(" ���� �����ϳ� �ڽ��� ���� ���� �ʽ��ϴ�.\n");
	}
	else if (curr->leftChild == NULL) { //���� ���� ����, �����ʸ� �����ϴ� ��� 
		printf("������ �ڽĸ� ����, right child = %s \n", curr->rightChild->name);
	}
	else if (curr->rightChild == NULL) { //���� ���� �����ϰ� �������� ���� ���
		printf("���� �ڽĸ� ���� , left child = %s \n", curr->leftChild->name);
	}
	else { //�ڽ��� �Ѵ� �����ϴ� ���
		printf("�ڽ� �Ѵ� ���� left child = %s, right child = %s \n", curr->leftChild->name, curr->rightChild->name);
	}
}

//���� ���� �Լ� 
int get_ht(TreePointer curr, char key[50], int count) {
	
	//��尡  ���̸� ���� 0���� ��ȯ 
	if (curr == NULL) {
		return 0;
	}
	// �Էµ� key���� ������ ��带 ã���� �� �������� ���� ��ȯ
	if (strcmp(key, curr->name) == 0) {
		return count;
	}
	if (strcmp(key, curr->name) < 0) { // �Էµ� key���� ���� ����� key������ ������ ���� ����Ʈ���� �̵��Ͽ� ���� ���
 		return get_ht(curr->leftChild, key, count + 1);
	}
	else { // �Էµ� key���� ���� ����� key������ ũ�� ������ ����Ʈ���� �̵��Ͽ� ���� ���
		return get_ht(curr->rightChild, key, count + 1);
	}

}