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
	*pathN = 0;  //패스 속 vertex 수
	int start = 0; // 시작 vertex
	int flag_count = 0; // degree가 홀수인 vertex 갯수
	int path_count = 0; // pathN count세기
	//degree 홀수인 vertex 있으면 시작점으로 지정
	for (int j = 0; j < VN; j++) {
		if (V[j].degree % 2 == 1) {
			start = j;
			break;
		}
	}
	ST.push(start); //시작점 스택에 push

	//flag 설정을 위한 홀수 degree vertex count
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
		return NULL; //none 이면 path 할당하지 않고 종료
	}
	int* path = new int[EN + 1]; // path 할당
	
	while (ST.empty() == false) { // 스택이 비어있을 때 까지 반복
		int v = ST.top();
		if (V[v].degree==0) { // 해당 vertex에서 더 이상 방문할 곳 없으면 path에 넣어주고 스택에서는 pop;
			path[path_count++] = v;
			ST.pop();		
		}
		else { // 해당 vertex에서 갈 수 있는 주변 vertex 있다면  
			DBL* top = V[v].S.top();
			if (v != E[top->d].v2) { //앞으로 갈 vertex를 Edge array에서 판별
				ST.push(E[top->d].v2); // 앞으로 갈 vertex를 스택에 삽입
				V[E[top->d].v2].S.remove(top->twin); // 거쳐간 간선 삭제
				pool.freeDBL(top->twin);
				V[E[top->d].v2].degree--;
			}
			else {	//앞으로 갈 vertex를 Edge array에서 판별
				ST.push(E[top->d].v1);
				V[E[top->d].v1].S.remove(top->twin);
				pool.freeDBL(top->twin);
				V[E[top->d].v1].degree--;
			}
			V[v].S.remove(top); // 거쳐간 간선 삭제
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
			if (V[i].S.empty() == false) { //vertex의 stack이 빌 때 까지 반복
				DBL* p = V[i].S.pop(); // 스택에서 원소들 꺼내고
				pool.freeDBL(p); // pool로 반환
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
		//스택에 삽입
		V[E[i].v1].S.push(adjE1);
		V[E[i].v2].S.push(adjE2);
		//edge 정보 삽입
		adjE1->d = i;
		adjE2->d = i;
		//서로 포인팅 
		adjE1->twin = adjE2;
		adjE2->twin = adjE1;
	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {	//새 DBL 할당

	DBL* p;
	if (DBL_pool == NULL) { //풀 비엇으면 새로 할당
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL)");
		}
		UsedMemoryForDBLs += sizeof(DBL);
		p->d = NONE;
	}
	else { //풀이 비어있지 않다면
		p = DBL_pool; //풀에서 하나 가져오기
		DBL_pool = p->rp; 
	}
	p->rp = p->lp = p->twin = NULL; // 포인터 정리
	++DBL_cnt;
	return(p);

}

void DBList::freeDBL(DBL *p) { // DBL을 풀로 다시 반환

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}
	
	p->d = NONE; 
	p->rp = DBL_pool; // 풀로 반환 받은 p를 DBL_pool 제일 앞쪽으로
	DBL_pool = p; 

	--DBL_cnt;		// DBL_count 줄이기
}

void DBList::freeDBL_pool(void) { //pool자체를 삭제
	DBL *p = DBL_pool;
	while (p != NULL) { // 풀이 빌 때 까지
		DBL_pool = p->rp;	// 풀의 앞쪽부터 하나씩 삭제
		delete p;	
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) { //풀이 다 비웠으면 DBL_cnt = 0이여야함
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) { // DBL stack에 새로운 원소 삽입

	if (ST != NULL) { 
		ST->lp = p; 
	}
	p->rp = ST;
	p->lp = NULL;
	ST = p; 

}

DBL *dblStack::pop(void) {	//DBL stack에서 가장 최근에 들어간 원소 꺼내기
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
	//p라는 원소를 DBL stack에서 제거 
	//(but, 아직 pool에서 제거된 상태는 아님, 단지 포인터만 정리하여 연결 상태만 바꿔줬을 뿐)
	
	if (ST == p) {  // p가 첫 원소라면
		ST = p->rp; 
		if (p->rp != NULL) { 
			(p->rp)->lp = NULL; 
		}
	}
	else { // p가 첫 원소가 아니면
		(p->lp)->rp = p->rp; 
		if (p->rp != NULL) { 
			(p->rp)->lp = p->lp; 
		}
	}
}

DBL *dblStack::top (void) { // DBL stack 에서 가장 위의 원소를 반환
	return ST;
}

bool dblStack::empty(void) { // DBL stack이 비어있다면 true 반환
	if (ST == NULL)
		return true;
	else
		return false;
}
