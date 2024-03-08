#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<time.h>

// Ÿ�� �� �������� ����
#define Max_vertex 100 // �ִ� ���� ���� ����. �̴� �۾��� �׷����� ������ ������ ���� ū ����.
#define INFINITE_VAL 10000 // ���Ѵ� ��.(�׷����� ��� ��ũ���� ����� ���պ��� �ξ� ū ��.)
#define NUM_IMPOSSIBLE -99999 // ���� �������� ���� �� ���� ��.

typedef struct stnode* type_node_ptr; // ��������Ʈ�� ��� ����.

typedef struct stnode {
	int no; //������ȣ
	double length; // ��ũ�� ����
	type_node_ptr link; // ���� ��忡 ���� ������
} type_node;

int Num_vertices; // �۾��� �׷����� �� ���� ����.
double Cost_mat[Max_vertex][Max_vertex]; // �������. ��� ���� ���� INFINITE_VAL �� �ʱ�ȭ.
type_node_ptr Adj_list[Max_vertex]; // ��������Ʈ. ó������ NULL �� �ʱ�ȭ.
double dist[Max_vertex]; // Dijkstra �˰����� ����ϴ� �Ÿ�����.
int visited[Max_vertex]; // Ž���Լ����� ����ϴ� �湮���� ����.(1:�湮��; 0:�湮 ����)
int pred[Max_vertex]; // Ž���Լ����� ����� ������ ����. (-1 �̸� ������ ����).
int S[Max_vertex]; // Dijkstra �˰����� ����ϴ� ó���Ϸ����� ����(1:�������; 0:�ȵ������).

//--------- Prototypes of functions---------------------------------
void addq(int queue[], int* rear, int item);
void breadth_first(int start);
int choose(int number_of_vertex);
int deleteq(int queue[], int* front, int rear);
void depth_first(int current);
void dijkstra(int v0);
int empty_queue(int front, int rear);
int exist_in_queue(int queue[], int front, int rear, int v);
void print_paths_dfs_bfs(int start, int desti);
void print_paths_dijkstra(int start, int desti);
//void read_and_construct_graph(char* fileName); �ڵ��� PDF���� ���� ������ ������� �ʾƼ� �ּ�ó��.
int read_number(char buf[], int* pos);
void read_word(char buf[], int* pos, char name[]);
void read_and_construct_graph(FILE* fps);

//main �Լ�
int main(void) {
	int i, j, start_vertex, destination_vertex;
	int ib; //�� ������ buffer ���� ���� ���� ������ ��ġ�� ������.
	char buffer[100], command[30], sub[10], filename[100]; // ��ɹ� ����, ��ɾ�, �����ɾ�.
	FILE* fps; // �׷��� ������ ������ ������.
	
	//���ϴ� ���� ������ �ֱ� 
	printf("\n�׷��� ������ ���� �̸���? ");
	gets(filename);
	fps = fopen(filename, "r");
	if (!fps) {
		printf("���� ���� ����.\n");
		return;
	}

	// �ʱ�ȭ: �������, ��������Ʈ.
	for (i = 0; i < Max_vertex; i++) {
		Adj_list[i] = NULL;
		for (j = 0; j < Max_vertex; j++) {
			Cost_mat[i][j] = INFINITE_VAL; // �� ���� ��ũ�� ������ ��Ÿ��.
		}
	}
	read_and_construct_graph(fps); // �׷��� ������ ������ �о� �׷���ǥ���� �����. File *fps ����
	//��ɹ� ���� ����.
	while (1) {
		// �� ����� ���� ���� ���� visited, pred, S �� �ʱ�ȭ�Ѵ�.
		//for (i = 0; i < Num_vertices; i++) {
		for (i = 0; i < Max_vertex; i++) {
			visited[i] = 0; pred[i] = -1; S[i] = 0;
		}
		
		printf("\n��ɹ�����: dfs a 0 / dfs s 0 7 / bfs a 0 / bfs s 0 7 / dij a 0 / dij s 0 7 / exit.\n");
			printf("��ɹ��� �����ÿ�> ");

		gets(buffer); // ��ɹ� �� ��(line)�� �о� �´�. ���ڿ� buf �� �о� ��.
		ib = 0; // ib�� ���� ó���� ���ڸ� ����Ų��. ���⼭�� ù ���ڸ� ����Ű�� �ʱ�ȭ.
		
		read_word(buffer, &ib, command); // ���� ��ɴܾ �д´�.
		if (strlen(command) == 0) {
			printf("��ɿ���. \n");
			continue; // ��ɴܾ ������ �ٽ� ��� ��û.
		}
		if (strcmp(command, "dij") == 0) { // Dijkstra algorithm �� ���� ��� ������ ��� ã��
			read_word(buffer, &ib, sub);
			if (strlen(command) == 0)
				continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
			if (sub[0] == 'a') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�1
				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.

				dijkstra(start_vertex); // Dijkstra �˰��� ����.
				print_paths_dijkstra(start_vertex, -1); // ��� ���������� ��� ���.
			}
			else if (sub[0] == 's') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�2

				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				destination_vertex = read_number(buffer, &ib); // �������� �б�.
				if (destination_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.

				dijkstra(start_vertex); // Dijkstra �˰��� ����.
				printf("%d����  %d���� ��� :", start_vertex, destination_vertex); // �߰��� �κ� 1
				print_paths_dijkstra(start_vertex, destination_vertex); // �� ��� ���.
			
			}
			else {
				printf("��� ����.\n");
				continue;
			}
		} // dij
		else if (strcmp(command, "dfs") == 0) { // dfs�� ���� ��� ������ ��� ã��
			read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
			if (strlen(command) == 0)
				continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
			if (sub[0] == 'a') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�3
				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				pred[start_vertex] = -1; // ���������� ���� ������ ���� ������ ��.
				depth_first(start_vertex); // depth-first searach �˰��� ����.
				print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
			}
			else if (sub[0] == 's') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�4
				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				destination_vertex = read_number(buffer, &ib); // �������� �б�.
				if (destination_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				pred[start_vertex] = -1; // ���������� ���� ������ ����.
				depth_first(start_vertex); // depth-first searach �˰��� ����.
				printf("%d���� %d���� ���:  ", start_vertex, destination_vertex); // �߰��� �κ�2
				print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���.
			}
			else {
				printf("sub ��� ����.\n");
				continue;
			}
		} // dfs
		else if (strcmp(command, "bfs") == 0) { // bfs�� ���� ��� ������ ��� ã��
			read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
			if (strlen(command) == 0)
				continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
			if (sub[0] == 'a') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�5
				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				breadth_first(start_vertex); // breadth-first search �˰��� ����.
				print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
			}
			else if (sub[0] == 's') {
				printf("�̸� : ������, �й�: 2019253084\n");//�� ��ɾ� ������ �̸�, �й� ������ �ϱ�6
				start_vertex = read_number(buffer, &ib); // �������� �б�.
				if (start_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				destination_vertex = read_number(buffer, &ib); // �������� �б�.
				if (destination_vertex == NUM_IMPOSSIBLE) {
					printf("Wrong command.\n"); continue;
				} // ���� �ȳ����� �ٽ� ��� ��û.
				breadth_first(start_vertex); // breadth-first search �˰��� ����.
				printf("%d���� %d���� ���: ", start_vertex, destination_vertex); // �߰��� �κ� 3
				print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���
			}
			else {

				printf("sub ��� ����.\n");
				continue;
			}
		} // bfs
		else if (strcmp(command, "exit") == 0) {
			break; // ���� ����
		}
		else {
			printf("��ɿ���. \n"); continue; // �߸��� ����� �����ϰ� �ٽ� ��û.
		}
} // while(��ɹ� ���� ����)
printf("\n���α׷��� �����մϴ�.\n");

return 0; 
} // main



// �Ʒ� ť ���� �Լ����� ť�� ����ť(linear queue)�� �����Ͽ� �ۼ���.
void addq(int queue[], int* rear, int item) {
	if (*rear == Max_vertex - 1) {
		printf("adding to queue has failed since queue is full.\n");
		return;
	}
	else
		queue[++ * rear] = item;
} // addq

//��������Ʈ, ť ��� ->  queue, front, rear)�� �� �Լ��� ���������� �����Ѵ�
//��ŸƮ�� ��������, ������ ����� ������ ������ pred �� ��´�
//�ʺ�켱 Ž�� (breadth first search)BFS
void breadth_first(int start) {
	int queue[Max_vertex]; //ť���� ���� ť���ƴ� ����ť 
	int front = -1, rear = -1, u, v; //�������� ����, �۾� �߿� ť�� empty�� �ʱ�ȭ
	type_node_ptr ptr; // ������ ���� 
	visited[start] = 1;
	addq(queue, &rear, start);
	while (!empty_queue(front, rear)) { //��������� ������ �����Ѵ�. 
		u = deleteq(queue, &front, rear);
		for (ptr = Adj_list[u]; ptr; ptr = ptr->link) {
			v = ptr->no;
			if (!visited[v]) {
				printf("%d ", v);
				addq(queue, &rear, v);
				visited[v] = 1;
				pred[v] = u;
			}
		}
	}
	printf("\n");
 } //BFS


// S �� ���� �� �� dist �� �ּ��� ������ ã�´�. �� ������ȣ�� ��ȯ.
// S �� �ȵ�� �ִ� ������ �ϳ��� ������ -1 �� ��ȯ.
int choose(int number_of_vertex) { // �Է� number_of_vertex �� ������ �� ������.
	int i, j, min, minpos;
	//S�� ���� ù ������ ã��.
	for (i = 0; i < number_of_vertex; i++) {
		if (S[i] == 0)
			break;
	}
	if (i == number_of_vertex)
		return -1; // �������� ��� S �� ��� �ִ�.
	minpos = i; // S�� ���� ù ������ȣ.
	min = dist[i]; // �� ������ dist.
	for (j = i + 1; j < number_of_vertex; j++)
		if (S[j] == 0 && dist[j] < min) { // S �� ���� �����̸� distance �� min���� �� ������, ����.
			min = dist[j];
			minpos = j;
		}
	return minpos;
} // choose

//����ť���� 
int deleteq(int queue[], int* front, int rear) { 
	if (*front == rear) {
		printf("delete failed since queue is empty.\n");
		return -999;
	}
	else
		return queue[++ * front];
} //deleteq


//��������Ʈ�� ����, ����Լ� ���
//��.�Ű����� v0 �� ���������� ��Ÿ����. ������ ����� ������ ������
//pred �� ��´�(�̴� ���߿� ��θ� �˾� ���� �� ���ȴ�.)
//���̿켱 Ž���Լ� (Depth frist search) DFS
void depth_first(int current) {
	visited[current] = 1;
	type_node_ptr ptr = Adj_list[current];
	while (ptr != NULL) {
		int next = ptr->no;
		if (!visited[next]) {
			pred[next] = current;
			depth_first(next);
		}
		ptr = ptr->link;
	}
}//DFS


//������ķ� ���� 
void dijkstra(int v0) {
	int  i, u, w; //�������� ����

	//�ʱ�ȭ
	for (i = 0; i < Num_vertices; i++) {
		if (i == v0)
			continue;
		S[i] = 0;
		dist[i] = Cost_mat[v0][i];
		if (Cost_mat[v0][i] < INFINITE_VAL)
			pred[i] = v0;
		else pred[i] = -1;
	}

	S[v0] = 1; //���� ������ ó���� �Ϸ�� �������� �����.
	dist[v0] = 0; //���� �������� �ڽű����� �Ÿ� 0
	pred[v0] = -1; //���������� ������� ����
	
	for (i = 0; i < Num_vertices - 1; i ++) {	
		u = choose(Num_vertices); //S�� �ȵ���ִ� ������ DISTANCE�� �ּ��� ���� ���� 

		if (u == -1) {
			printf("Logic error, ��� S�� ����ִ� ���� �� �� �ּҸ� ã������ ��. \n");
			getchar();
		}

		if(dist[u] == INFINITE_VAL) {
			printf("��ΰ� �ִ� �������� �̹� ��� �ִ� ��θ� ã����.\n");
			break;
		}
		
		S[u] = 1;

		for (w = 0; w < Num_vertices; w++) {
			if (S[w] == 0) { //s�� ���� ���鿡 ���ؼ� 
				if (dist[u] + Cost_mat[u][w] < dist[w]) {
					dist[w] = dist[u] + Cost_mat[u][w];
					pred[w] = u;

				}//if
			}//if
			
		}//for
	}//for

}//Dijksta


int empty_queue(int front, int rear) {
	if (front == rear) return 1;
	else return 0;
} // empty_queue

// ���� v �� queue �ȿ� �̹� �����ϴ��� üũ�Ѵ�. 
int exist_in_queue(int queue[], int front, int rear, int v) {
	int i, answer = 0;
	for (i = front + 1; i <= rear; i++) {
		if (queue[i] == v)
			answer = 1;
	}
	return answer; // �����ϸ� 1, ���ϸ� 0 �� ��ȯ.
} // exist_in_queue

//dfs, bfs �˰����� ������ ����� pred ������ �̿��Ͽ� ��θ� �˾� ���� ����Ѵ�
void print_paths_dfs_bfs(int start, int desti) {
	if (start == desti) {
		printf(" %d, ", start);
	}
	else if (desti == -1) {
		for (int i = 0; i < Num_vertices; i++) {
			if (i != start) {
				printf(" %d���� %d���� ��� : ", start, i);
				// ���� �������� �ش� ���������� ��θ� ���
				print_paths_dfs_bfs(start, i);
				printf("\n");
			}
		}
	}
	else if (pred[desti] == -1) {
		printf(" %d���� %d���� ��ΰ� �����ϴ�.", start, desti);
	}
	else {
		// ���� �������� ������ ���������� ��θ� ���
		print_paths_dfs_bfs(start, pred[desti]);
		printf("%d, ", desti);
	}


}//print_paths_dfs_bfs

//: dijkstra �˰����� ������ ��, �� ����� �̿��Ͽ� �ִܰ�� �� �� �Ÿ��� �˾Ƴ��� ����Ѵ�.
void print_paths_dijkstra(int start, int desti) {
	int d, fr, to, leng, curr;
	int rpath[Max_vertex];

	if (desti == -1) { //��� ���������� ��θ� ����ؾ���
		//�Ʒ� FOR -LOOP ������ ������ ���۰� ���ᰪ�� ��Ÿ����.

		fr = 0;
		to = Num_vertices - 1;
	
	}
	else { //���� �������� desti�� ��� �ϳ��� ��� 
		fr = desti;
		to = desti; 

	}
	printf("\n");
	for (d = fr; d <= to; d++) {
		if (d == start) continue;
		curr = d;
		leng = 0;

		do {
			rpath[leng] = curr;
			leng++;
			curr = pred[curr];
		} while (curr != -1);
		printf(" %d���� %d���� �ִ� ��� : ", start, d);
		if (leng <= 1) {
			printf(" ��ΰ� ����. \n");
			continue;
		}
		for (curr = leng - 1; curr >= 0; curr--) {
			printf("%d, ", rpath[curr]);
		}
		printf("  �� ����� ���� :  %6.2f\n", dist[d]);
	}

}//print_paths_dijkstra


/* ����������� �Լ��� �ּ�ó��.
void read_and_construct_graph(char* fileName) {

} //read_and_construct_graph
*/

// �־��� buffer �� ���� ���� ���ں��� �����Ͽ� ��(number)�� �ϳ� �о� �´�.
// ���� ��� �� ������ NUM_IMPOSSIBLE �� ��ȯ�Ѵ�.
int read_number(char buf[], int* pos) {
	int n = 0, i = 0;
	char numstr[30];
	while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)
		++; // ���鹮�ڳ� �ĸ��� ��������.
	while (isdigit(buf[*pos])) {
		numstr[i] = buf[*pos]; i++; (*pos)++;
	}
	numstr[i] = '\0';
	if (i == 0) {
		printf("���� ���� ���Ͽ����. \n");
		return NUM_IMPOSSIBLE;
	}
	else {
		n = atoi(numstr);
		return n;
	}
} // read_number

// �־��� buffer ���� �ܾ �� �� �о� �´�. �� ������ ���̰� 0 �� ���ڿ��� ��ȯ��.
void read_word(char buf[], int* pos, char name[]) {
	int i = 0;
	// ���鹮�ڳ� �ĸ��� ��������.
	while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)++;
	// ������ ���鹮�ڳ� �ĸ��� �ι��ڰ� �ƴϸ� �д´�.
	while (buf[*pos] != ' ' && buf[*pos] != ',' && buf[*pos] != '\0') {
		name[i] = buf[*pos];
		i++; (*pos)++;
	}
	name[i] = '\0';
	return;
} // read_word

// ���Ͽ� �غ��� ������ �о� �������(Cost_mat), ��������Ʈ(Adj_list)�� �����.
// �� ������ Num_vertices �� ä���.
//graph�� ���� ����Ÿ ������ �о �������� Num_vertices �� ������ ���� �ִ´�.�������(Cost_mat) ��, ��������Ʈ(Adj_list) �� �����Ѵ�(�������� Cost_mat, Adj_list �� �����).
void read_and_construct_graph(FILE* fps) {
	char line[500]; // line buffer
	char str[10];
	char* ret;
	int i, k, m, leng, nr;
	int v, dv; // ������
	double co; // ��ũ�� ���
	type_node_ptr nptr, curr; // ��������Ʈ ��忡 ���� ������
	ret = fgets(line, 500, fps);
	if (!ret) {
		printf("Read a line error: no line for the number of vertices.\n");
		getchar();
	}
	// �������� �� �о� �������� Num_vertices �� �ִ´�.
	nr = sscanf(line, " %d ", &Num_vertices);
	if (nr != 1) {
		printf("Read error: number of vertices.\n");
		getchar(); // ���� ����.
	}
	int num_adj_lines = 0;
	do {
		// read a line of the list of arcs.
		ret = fgets(line, 500, fps); // read nonterminals line.
		if (!ret) { // no more line in the file
			break;
		}
		leng = strlen(line); // get the length of the line.
		if (leng == 0) break; // �ٿ� �� ���ڵ� ���ٸ� �б� ����.
		// �������� ������� �б�.
		i = 0;
		while (i < leng && !isdigit(line[i])) // move to a digit.
			i++;
		k = 0;
		while (i < leng && isdigit(line[i])) { // collect digits.
			str[k] = line[i];
			i++; k++;
		}
		str[k] = '\0'; // finish the string.
		if (k == 0) break; // �Ѱ��� ���ڵ� ����.
		v = atoi(str); // ������ �������.
		num_adj_lines++; // �������� �� �� ����.
		// ��������� ���� ���������鿡 ���� ������ ����: ����������ȣ, ��ũ���.
		do {
			while (i < leng && !isdigit(line[i])) // move to a digit.
				i++;
			if (i >= leng) { // �� �ٿ� �� �̻� ���������� ����.
				break;
			}
			// ���� ������ ����.
			k = 0;
			while (i < leng && isdigit(line[i])) { // collect digits.
				str[k] = line[i];
				i++; k++;
			}
			str[k] = '\0'; // finish the string.
			dv = atoi(str); // ���������� ��ȣ.
			// ��ũ����� ����.
			k = 0;
			while (i < leng && !isdigit(line[i])) // move to a digit.
				i++;
			while (i < leng && (isdigit(line[i]) || line[i] == '.')) { // collect digits.
				str[k] = line[i];
				i++; k++;
			}
			str[k] = '\0'; // finish the string.
			if (k == 0) {
				printf("����: �־�� �� ������ ����� �ȳ���.\n");
				getchar(); // ���� ����.
			}
			co = atof(str); // �̰��� ��ũ����̴�.
			// store the arc Cost_mat to the Cost_mat matrix.
			Cost_mat[v][dv] = co;
			// ��������Ʈ�� ��� �ϳ��� �����Ͽ� ���δ�.
			nptr = (type_node_ptr)malloc(sizeof(type_node));
			nptr->no = dv; nptr->length = co; nptr->link = NULL;
			// �ش� ��������Ʈ�� �ǵڿ� ���δ�.
			curr = Adj_list[v];
			if (curr == NULL)
				Adj_list[v] = nptr; // ����Ʈ�� ù ��尡 ��.
			else {
				while (curr->link) // �� ���� ���� ����.
					curr = curr->link;
				curr->link = nptr; // �� ��带 �� �ڿ� ���δ�.
			}
		} while (1);
	} while (1);
	if (Num_vertices != num_adj_lines) {
		printf("���������� �� ���� �� ������ �� �ٸ���. (�������� �Ͻÿ�.) \n");
		getchar(); // ��������.
	}
	fclose(fps);
} // read_and_construct_graph