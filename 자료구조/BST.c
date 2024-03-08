#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<stdbool.h>

//구조체 선언
typedef struct anod* TreePointer;

typedef struct anod { //한 노드의 구조 

	int sno; //학번
	char name[50]; //이름 
	TreePointer leftChild, rightChild; // 노드 자식 포인터
} Ty_Node; // 노드별칭

//함수 선언 
bool insert(TreePointer* node, char key[50], int sno);
TreePointer modifiedSearch(TreePointer tree, char key[50]);
void inorder(TreePointer ptr);
int pre_order_ht(TreePointer curr, int lvl);
TreePointer search_BST(TreePointer curr, char key[50], TreePointer* par);
void BST_Delete(TreePointer* root, char key[50]);
int leaf_count(TreePointer tree);
void ch(TreePointer root, char key[50]);
int get_ht(TreePointer curr, char key[50], int count);

//메인 함수 
int main(void) {
	int ht, index, sno; //높이,배열의 인덱스, 학생 학번 선언
	char* pt;
	char order[50], arr[3][50], name[50];
	bool a;
	
	FILE* fp;
	fp = fopen("sdata.txt", "r");
	
	TreePointer root = NULL, parent = NULL, tmp = NULL;

	//파일 열기,닫기 
	if (fp == NULL) {
		printf("파일이 없습니다. \n");
		return 0;
	}
	else {
		//파일에서 학생정보를 읽고, 이진 탐색트리에 추가
		while (!feof(fp)) {
			int res = fscanf(fp, "%d %s", &sno, &name); //파일 학번, 이름 입력하기
			if (res < 2)
						break;
			insert(&root, name, sno);
		}
	

		
	}

		//main roof
	while (1) {
		printf("수행할 작업은 (in, sp, de, se, ht, ch, le, ex) ? ");
		 gets(order);
	
		index = 0;
		// 입력문자 알기
		pt = strtok(order," "); // 문자열 자르기 
		
		while (pt != NULL)
		{
			strcpy(arr[index++], pt); //문자열 복사
			pt = strtok(NULL," ");

		}
		//bst에 입력한 데이터 추가 
		if (strcmp(arr[0],"in") == 0) { //문자열비교 
			a = false;
			sno = atoi(arr[1]); //string to in
			strcpy(name, arr[2]);
			a = insert(&root, name, sno); //학생 데이터 정보 삽입
			if (a == true)
				printf("입력 성공! level = %d \n", get_ht(root, name, 0)); //삽입이 성공시, 입력성공과 함께 트리 높이 출력
		}
		//모든 데이터 츨력
		else if (strcmp(arr[0],"sp") == 0)
		{
			system("cls");
			printf("파일의 내용은 다음과 같습니다 \n");
			inorder(root); //중위 순회로 모든 데이터 출력
	
		}
		//입력한 데이터 삭제
		else if (strcmp(arr[0],"de") == 0) {
			strcpy(name, arr[1]);
			BST_Delete(&root, name); //삽입된 데이터 삭제
		}
	//입력한 데이터가 있는지 확인 
		else if (strcmp(arr[0],"se") == 0) {
			strcpy(name, arr[1]);
			tmp = search_BST(root, name, &parent); //데이터 탐색 수행
			//데이터가 있으면 이름,학번, 레벨(위치) 출력하기
			printf("이름: %s, 학번: %d, 레벨위치 = %d \n", tmp->name, tmp->sno, get_ht(root, name,0)); 
		
		}
		//트리 높이 출력
		else if (strcmp(arr[0],"ht") == 0)
		{
			ht = pre_order_ht(root, 0); //preorder로 높이 출력 수행
			printf("height = %d\n", ht);
		}
		//입력받은 값의 차일드 출력
		else if (strcmp(arr[0],"ch") == 0)
		{
			strcpy(name, arr[1]);
			ch(root, name); //입력한 값의 자식 출력
		}
		//노드 갯수 
		else if (strcmp(arr[0],"le") == 0)
		{
			printf("number of leaf node = %d\n", leaf_count(root)); //노드(잎)의 개수 출력
		}
		//프로그램 종료
		else if (strcmp(arr[0],"ex") == 0)
		{
			printf("프로그램을 종료합니다!\n");
			system("pause");
			return 0;
		}

	}

	return 0;
}

//삽입함수
bool insert(TreePointer* node, char key[50], int sno) {
	//새로운 노드 생성, 초기화
	TreePointer ptr, temp;
	ptr = (TreePointer)malloc(sizeof(*ptr));
	strcpy(ptr->name,key); //새 노드의 이름
	ptr->sno = sno; //새 노드의 학번 
	
	ptr->leftChild = ptr->rightChild = NULL;  // 새 노드의 자식 노드를 NULL로 초기화

	if (!(*node)) {// 빈 트리인 경우, 새 노드를 루트 노드로 설정
		*node = ptr;
	}
	else //빈트리가 아닌 경우
	{
		temp = modifiedSearch(*node, key); //새 노드를 붙일 노드 탐색 
		
		if (!temp) { // 동일한 이름을 가진 노드가 이미 존재하는 경우
			printf("동일 키의 노드가 이미 존재하여 삽입 실패 \n");
			return false; //삽입 실패
		}
		else { //temp가 가리키는 노드에 새 노드를 붙임.
			if (strcmp(key, temp->name) < 0) { // 새 노드의 이름이 부모 노드의 이름보다 작은 경우
				temp->leftChild = ptr; // 왼쪽 자식 노드로 삽입
				return true; // 삽입 성공
			}

			else { // 새 노드의 이름이 부모 노드의 이름보다 큰 경우
				
				temp->rightChild = ptr; // 오른쪽 자식 노드로 삽입
				return true; //삽입 성공
			}

		}
	}
	
}

//삽입이 가능한지 탐색하여 가능한지 확인하는 함수  
TreePointer modifiedSearch(TreePointer tree, char key[50]) {
	TreePointer temp = tree;  //현재 노드를 가르키는 포인터 변수를 temp로 선언하고 초기화
	while (temp) { // temp가 널이 아닐때 까지 반복 
		if (strcmp(key, temp->name) == 0) { // key 값과 이름이 같을 경우에
			// 중복된 키 값을 찾은 경우에 널 값으로 반환 
			return NULL;
		} 
		else if (strcmp(key, temp->name) < 0) {  // key 값이 노드의 이름보다 작으면
			if (temp->leftChild == NULL) { //왼쪽 자식 노드가 널값이면
				return temp; //현재 노드로에 반환
			}
			else { 
				temp = temp->leftChild; //아니면 현재의 노드를 왼쪽 자식 노드로 이동 
			}
		}
		else {
			if (temp->rightChild == NULL) { //오른쪽 자식이 널값이면
				return temp; //현재 노드에 반환
			}
			else {
				temp = temp->rightChild; //아니면 현재의 노드를 오른쪽 자식 노드로 이동
			}
		}
	}
	return temp; //temp 값이 널값이면 널을 현재노드로 반환
}

//inodrer(중위순회) 함수  왼쪽 뿌리 오른쪽 출력
void inorder(TreePointer ptr) {
	if (ptr)
	{
		inorder(ptr->leftChild); //왼쪽 
		printf("%s, %d\n", ptr->name, ptr->sno); // 뿌리
		inorder(ptr->rightChild); // 오른쪽
	}

}

//preorder(전위순회)를 이용한 hight (높이) 함수 
int pre_order_ht(TreePointer curr, int lvl) {
	if (curr == NULL) // 현재 노드가 null 일때
	{
		return lvl; // 현재 레벨로 반환

	}
	/*왼쪽 서브트리의 높이와 오른쪽 서브트리의 높이를 구하고 
	더 높은 값을 현재 트리의 높이로 반환*/
	int left_ht, right_ht;
	left_ht = pre_order_ht(curr->leftChild, lvl + 1);
	right_ht = pre_order_ht(curr->rightChild, lvl + 1);
	return (left_ht > right_ht ? left_ht : right_ht) ;
}

//이진트리 탐색 탬색구현 
TreePointer search_BST(TreePointer curr, char key[50], TreePointer* par) {
	

	*par = NULL; // 부모 노드 포인터 초기화
	while (curr != NULL){ // 현재 노드가 null이 될 때까지 반복
		// 찾고자 하는 키 값을 현재 노드의 키 값과 비교
		if (strcmp(key, curr->name) == 0){  //일치하는 경우
			return curr; //현재 노드 반환 
		}
		else if (strcmp(key, curr->name) < 0) { //키값이 작을 경우 왼쪽 서브트리 탐색
			*par = curr ; //현재 노드를 부모노드로 설정
			curr = curr->leftChild; //현재 노드를 왼쪽 자식 노드로 변경 
		}
		else { //아니면 -> 키 값이 클 경우 오른쪽 서브트리 탐색
			*par = curr; //현재 노드를 부모노드로 설정
			curr = curr->rightChild; //현재 노드를 오른쪽 자식 노드로 변경 
		}
	}
	
	return NULL; //찾는 키값이 없는 경우 null값으로 반환

}

//BST 삭제 함수 구현
void BST_Delete(TreePointer* root, char key[50]) {

	TreePointer parent = NULL;
	TreePointer curr = search_BST(*root, key, &parent); // 키를 가진 노드와 그 부모 노드 탐색

	if (curr == NULL) { //탐색 실패인 경우 (현재 노드가 널값인 경우)
		printf("그런 학생을 을 찾을 수 없습니다. 다시 확인해보세요 \n");
		return;
	}

 // 삭제할 노드의 차수가 0인 경우
	if (curr->leftChild == NULL && curr->rightChild == NULL) {
		if (parent->leftChild == curr) { // 삭제할 노드가 부모 노드의 왼쪽 자식인 경우
			parent->leftChild = NULL; // 널값으로 반환
		}
		else { // 삭제할 노드가 부모 노드의 오른쪽 자식인 경우
			parent->rightChild = NULL; // 오른쪽 자식을 널 값으로 반환
		}
		free(curr); // 삭제할 노드 메모리 해제
		printf("성공적으로 삭제되었습니다.\n");
	}
	// 삭제할 노드의 차수가 1인 경우
	else if (curr->leftChild == NULL || curr->rightChild == NULL) {
		TreePointer child = (curr->leftChild != NULL) ? curr->leftChild : curr->rightChild; // 삭제할 노드의 자식 노드 탐색

		if (parent->leftChild == curr) { // 삭제할 노드가 부모 노드의 왼쪽 자식인 경우
			parent->leftChild = child; // 삭제할 노드를 자식 노드로 대체
		}
		else { // 삭제할 노드가 부모 노드의 오른쪽 자식인 경우
			parent->rightChild = child; // 삭제할 노드를 자식 노드로 대체
		}

		printf("성공적으로 삭제되었습니다.\n");
		printf("이동한 학생: %s\n", child->name); // 이동한 학생의 이름 출력
		free(curr); // 삭제할 노드 메모리 해제
	}
	// 삭제할 노드의 차수가 2인 경우
	else {
		
		TreePointer maxNode = curr->leftChild;
		TreePointer maxNodeParent = curr; // 후계자 노드의 부모 노드
		
		//후계자 노드 탐색을 위해 
		//삭제할 노드의 왼쪽 서브트리에서 가장 큰 노드를 찾음
		while (maxNode->rightChild != NULL) {
			maxNodeParent = maxNode;
			maxNode = maxNode->rightChild;
		}

		strcpy(curr->name, maxNode->name);

		// 후계자 노드의 차수가 1인 경우
		if (maxNode->leftChild != NULL) {
			if (maxNodeParent->leftChild == maxNode) { // 후계자 노드가 부모 노드의 왼쪽 자식인 경우
				maxNodeParent->leftChild = maxNode->leftChild; // 후계자 노드를 삭제하고 왼쪽 자식으로 대체
			}
			else { // 후계자 노드가 부모 노드의 오른쪽 자식인 경우
				maxNodeParent->rightChild = maxNode->leftChild; // 후계자 노드를 삭제하고 왼쪽 자식으로 대체
			}
		}
		else { // 후계자 노드의 차수가 0인 경우
			if (maxNodeParent->leftChild == maxNode) { // 후계자 노드가 부모 노드의 왼쪽 자식인 경우
				maxNodeParent->leftChild = NULL; // 후계자 노드를 삭제하고 널값으로 반환
			}
			else { // 후계자 노드가 부모 노드의 오른쪽 자식인 경우
				maxNodeParent->rightChild = NULL; // 후계자 노드를 삭제하고 널값으로 반환
			}
		}
	
		printf("성공적으로 삭제되었습니다.\n");
		printf("이동한 학생: %s\n", maxNode->name); // 이동한 학생의 이름 출력
		free(maxNode); // 후계자 노드 메모리 해제
	}

}

//트리 잎 개수 구하기 
int leaf_count(TreePointer tree) {

	if (tree == NULL) { // 빈 트리일 경우 0 반환
		return 0;
	}
	else if (tree->leftChild == NULL && tree->rightChild == NULL) { // 잎 노드일 경우 1 반환
		return 1;
	}
	else {
		return leaf_count(tree->leftChild) + leaf_count(tree->rightChild); // 재귀적으로 좌우 서브트리에서 잎 노드 개수를 구해서 더하기
	}
	}

//입력받은 값의 차일드 출력 
void ch(TreePointer root, char key[50]) {
	TreePointer par = NULL; // 부모 노드를 가리키는 포인터 
	TreePointer curr = search_BST(root,key,&par); // 입력된 키값을 가지는 노드를 찾음
	
	if (curr == NULL) { //현재 노드가 없는 경우 
		printf("노드가 존재 하지 않습니다. \n");
		return; 

	}

	 else if (curr->leftChild  == NULL && curr->rightChild == NULL) { //현재 노드의 자식이 둘 다 없는 경우 
		printf(" 노드는 존재하나 자식이 존재 하지 않습니다.\n");
	}
	else if (curr->leftChild == NULL) { //왼쪽 노드는 없고, 오른쪽만 존재하는 경우 
		printf("오른쪽 자식만 존재, right child = %s \n", curr->rightChild->name);
	}
	else if (curr->rightChild == NULL) { //왼쪽 노드는 존재하고 오른쪽은 없는 경우
		printf("왼쪽 자식만 존재 , left child = %s \n", curr->leftChild->name);
	}
	else { //자식이 둘다 존재하는 경우
		printf("자식 둘다 존재 left child = %s, right child = %s \n", curr->leftChild->name, curr->rightChild->name);
	}
}

//높이 구현 함수 
int get_ht(TreePointer curr, char key[50], int count) {
	
	//노드가  널이면 높이 0으로 반환 
	if (curr == NULL) {
		return 0;
	}
	// 입력된 key값을 가지는 노드를 찾으면 그 때까지의 높이 반환
	if (strcmp(key, curr->name) == 0) {
		return count;
	}
	if (strcmp(key, curr->name) < 0) { // 입력된 key값이 현재 노드의 key값보다 작으면 왼쪽 서브트리로 이동하여 높이 계산
 		return get_ht(curr->leftChild, key, count + 1);
	}
	else { // 입력된 key값이 현재 노드의 key값보다 크면 오른쪽 서브트리로 이동하여 높이 계산
		return get_ht(curr->rightChild, key, count + 1);
	}

}