#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <time.h>
#include <stack>
#include "DBL.h"

//#define NO_PATH_OUT   // comment out this line for path output
void Error_Exit(const char *s);

typedef struct _Vertex {
	dblStack S;		// adj list contains edge index
	int degree;
} Vertex;

typedef struct _Edge {
	int v1, v2;
} Edge;

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN);
void adjListGenerate(Vertex *V, Edge *E, int VN, int EN);
void deallocGraph(Vertex *V, Edge *E, int VN);
int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN);

DBList pool;	// DBL storage pool

int main() {
	int T, VN, EN;
	Vertex *V;
	Edge   *E;
	int *path;	// Euler cycle or path
	int pathN;  // path length
	int  flag;	// 0: cycle, 1: path, 2: none
	clock_t start_time, finish_time;

	scanf("%d", &T);	// read # of tests
	for (int t = 1; t <= T; t++) {	// for each test
		graphGeneration(&V, &E, &VN, &EN);

		start_time = clock(); // set the start time

		path = Find_Euler(V, E, VN, EN, &flag, &pathN); // find an Euler path or cycle

		finish_time = clock(); // set finish time
		
		double cmpt = (((double)(finish_time - start_time)) / CLK_TCK)*1000; // compute the time passed
		printf("Test= %d flag= %d VnumInCycle/Path)= %d ", t, flag, pathN);

		if (flag == 0)
			printf("Euler_cycle(exec_time= %.2f msec)\n",cmpt);
		else if (flag == 1)
			printf("Euler_path(exec_time= %.2f msec)\n", cmpt);
		else
			printf("not_Eulerian(exec_time= %.2f msec)\n", cmpt);

#ifndef NO_PATH_OUT
		if (flag != 2) {
			for (int i = 0; i < EN + 1; i++) {
				printf("%d\n", path[i]);
			}
		}
#endif
		if (flag != 2)
			delete[] path;
		deallocGraph(V, E, VN);
	}
	pool.freeDBL_pool();	// clear all the DBL elements

	return 0;
}

int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN) {

	stack<int> ST;		
	*pathN = 0;  //�н� �� vertex ��
	int start = 0; // ���� vertex
	int flag_count = 0; // degree�� Ȧ���� vertex ����
	int path_count = 0; // pathN count����
	//degree Ȧ���� vertex ������ ���������� ����
	for (int j = 0; j < VN; j++) {
		if (V[j].degree % 2 == 1) {
			start = j;
			break;
		}
	}
	ST.push(start); //������ ���ÿ� push

	//flag ������ ���� Ȧ�� degree vertex count
	for (int j = 0; j < VN; j++) {
		if (V[j].degree % 2 == 1) {
			flag_count++;
		}
	}

	if (flag_count == 0)
		*flag = 0; // cycle
	else if (flag_count == 2)
		*flag = 1; // path
	else {
		*flag = 2;
		return NULL; //none �̸� path �Ҵ����� �ʰ� ����
	}
	int* path = new int[EN + 1]; // path �Ҵ�
	
	while (ST.empty() == false) { // ������ ������� �� ���� �ݺ�
		int v = ST.top();
		if (V[v].degree==0) { // �ش� vertex���� �� �̻� �湮�� �� ������ path�� �־��ְ� ���ÿ����� pop;
			path[path_count++] = v;
			ST.pop();		
		}
		else { // �ش� vertex���� �� �� �ִ� �ֺ� vertex �ִٸ�  
			DBL* top = V[v].S.top();
			if (v != E[top->d].v2) { //������ �� vertex�� Edge array���� �Ǻ�
				ST.push(E[top->d].v2); // ������ �� vertex�� ���ÿ� ����
				V[E[top->d].v2].S.remove(top->twin); // ���İ� ���� ����
				pool.freeDBL(top->twin);
				V[E[top->d].v2].degree--;
			}
			else {	//������ �� vertex�� Edge array���� �Ǻ�
				ST.push(E[top->d].v1);
				V[E[top->d].v1].S.remove(top->twin);
				pool.freeDBL(top->twin);
				V[E[top->d].v1].degree--;
			}
			V[v].S.remove(top); // ���İ� ���� ����
			pool.freeDBL(top);
			V[v].degree--;
		}
	}
	*pathN = path_count;
	return path;
}

void deallocGraph(Vertex *V, Edge *E, int VN) {

	for (int i = 0; i < VN; i++) {
		while (1) {
			if (V[i].S.empty() == false) { //vertex�� stack�� �� �� ���� �ݺ�
				DBL* p = V[i].S.pop(); // ���ÿ��� ���ҵ� ������
				pool.freeDBL(p); // pool�� ��ȯ
			}
			else 
				break;
		}
	}
	delete[] V;
	delete[] E;
}

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN) {
	scanf("%d %d", VN, EN);	// read # of vertices and edges
	//allocVandEarray(V, E, *VN, *EN);	// vertex and edge array allocation

	*V = new Vertex[*VN];
	*E = new Edge[*EN];
	if (*V == NULL || *E == NULL) {
		Error_Exit("Memory Allocation Error!");
	}

	for (int v = 0; v < *VN; v++) {
		(*V)[v].degree = 0;
	}
	for (int e = 0; e < *EN; e++) {
		scanf("%d %d", &((*E)[e].v1), &((*E)[e].v2));	// read edge information
		++((*V)[(*E)[e].v1].degree);
		++((*V)[(*E)[e].v2].degree);
	}
	adjListGenerate(*V, *E, *VN, *EN);	// create adj list in G=(V,E)
}

void adjListGenerate(Vertex *V, Edge *E, int VN, int EN) {
	
	for (int i = 0; i < EN; i++) {
		DBL* adjE1 = pool.allocDBL();
		DBL* adjE2 = pool.allocDBL();
		//���ÿ� ����
		V[E[i].v1].S.push(adjE1);
		V[E[i].v2].S.push(adjE2);
		//edge ���� ����
		adjE1->d = i;
		adjE2->d = i;
		//���� ������ 
		adjE1->twin = adjE2;
		adjE2->twin = adjE1;
	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {	//�� DBL �Ҵ�

	DBL* p;
	if (DBL_pool == NULL) { //Ǯ ������� ���� �Ҵ�
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL)");
		}
		UsedMemoryForDBLs += sizeof(DBL);
		p->d = NONE;
	}
	else { //Ǯ�� ������� �ʴٸ�
		p = DBL_pool; //Ǯ���� �ϳ� ��������
		DBL_pool = p->rp; 
	}
	p->rp = p->lp = p->twin = NULL; // ������ ����
	++DBL_cnt;
	return(p);

}

void DBList::freeDBL(DBL *p) { // DBL�� Ǯ�� �ٽ� ��ȯ

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}
	
	p->d = NONE; 
	p->rp = DBL_pool; // Ǯ�� ��ȯ ���� p�� DBL_pool ���� ��������
	DBL_pool = p; 

	--DBL_cnt;		// DBL_count ���̱�
}

void DBList::freeDBL_pool(void) { //pool��ü�� ����
	DBL *p = DBL_pool;
	while (p != NULL) { // Ǯ�� �� �� ����
		DBL_pool = p->rp;	// Ǯ�� ���ʺ��� �ϳ��� ����
		delete p;	
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) { //Ǯ�� �� ������� DBL_cnt = 0�̿�����
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) { // DBL stack�� ���ο� ���� ����

	if (ST != NULL) { 
		ST->lp = p; 
	}
	p->rp = ST;
	p->lp = NULL;
	ST = p; 

}

DBL *dblStack::pop(void) {	//DBL stack���� ���� �ֱٿ� �� ���� ������
	DBL *p;
	p = ST;
	if (ST->rp == NULL)
		ST = NULL;
	else {
		ST = ST->rp;
		ST->lp = NULL;
	}

	return p;
}

void dblStack::remove(DBL* p) {	
	//p��� ���Ҹ� DBL stack���� ���� 
	//(but, ���� pool���� ���ŵ� ���´� �ƴ�, ���� �����͸� �����Ͽ� ���� ���¸� �ٲ����� ��)
	
	if (ST == p) {  // p�� ù ���Ҷ��
		ST = p->rp; 
		if (p->rp != NULL) { 
			(p->rp)->lp = NULL; 
		}
	}
	else { // p�� ù ���Ұ� �ƴϸ�
		(p->lp)->rp = p->rp; 
		if (p->rp != NULL) { 
			(p->rp)->lp = p->lp; 
		}
	}
}

DBL *dblStack::top (void) { // DBL stack ���� ���� ���� ���Ҹ� ��ȯ
	return ST;
}

bool dblStack::empty(void) { // DBL stack�� ����ִٸ� true ��ȯ
	if (ST == NULL)
		return true;
	else
		return false;
}
