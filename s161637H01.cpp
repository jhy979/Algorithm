#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);
SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	int arr[3] = { -1 }; // 입력 받을 버퍼 선언

	//vertex, edge array 정보 입력
	for (int i = 0; i < Enum; i++) {
		scanf_s("%d %d %d", &arr[0], &arr[1], &arr[2]);
		E[i].vf = arr[0];
		E[i].vr = arr[1];
		E[i].cost = arr[2];
		E[i].name = i;
		E[i].flag = false;

		//SLLPool에서 가져와서 할당하기
		SLL* p = pool.allocSLL();
		p->i = i; 
		V[arr[0]].S.push(p);
		
		SLL* q = pool.allocSLL();
		q->i = i;
		V[arr[1]].S.push(q);
		
		V[arr[0]].name = arr[1];
		V[arr[1]].name = arr[1];
		V[arr[1]].flag = false;

	}

}
void Free_Graph_adj_list(int Vnum, vertex *V) {

	if (V == NULL)
		return;
	else {
		for (int i = 0; i < Vnum; i++) {
			// V 배열의 adjacency list의 원소를 pop을 통해 돌려준다.
			while (!V[i].S.empty()) {
				pool.freeSLL(V[i].S.pop());
			}
		}
	}

}
int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
	) 
{
	int cost_sum = 0;

	//bfs를 수행하기 위한 큐 선언
	std::queue <int> q;

	//시작 vertex를 넣고 방문처리
	q.push(src);
	V[src].flag = true;
	
	while (!q.empty()) {
		//큐의 가장 앞에 있는 원소를 현재 체크할 노드로 선정
		int checking_node = q.front();
		q.pop();
		SLL* p = V[checking_node].S.top();
		while (p!= NULL) {
			//앞쪽 링크를 확인하면서 방문하지 않은 노드가 있다면, 방문처리 후 큐에 넣기
			if (V[E[p->i].vf].flag == false) {
				q.push(E[p->i].vf);
				V[E[p->i].vf].flag = true;
				E[p->i].flag = true;
				cost_sum += E[p->i].cost;
			}
			// 뒤쪽 링크도 확인
			if (V[E[p->i].vr].flag == false) {
				q.push(E[p->i].vr);
				V[E[p->i].vr].flag = true;
				E[p->i].flag = true;
				cost_sum += E[p->i].cost;
			}
				
				p = p->p;
		}
	}

	return cost_sum;
}



// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST
	SLL* p = ST;
	if (ST->p == NULL) {
		ST = NULL;
	}
	else {
		ST = ST->p;
	}
	return p;
}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {		// allocate an SLL element
	SLL *p;
	if (SLL_pool == NULL) {
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL) {
			errorExit("Memory allocation error(allocSLL)");
		}
		UsedMemoryForSLLs += sizeof(SLL);
		p->i = NONE;
	}
	else {
		p = SLL_pool;
		SLL_pool = p->p;
	}
	p->p = NULL;
	++SLL_cnt;
	return(p);
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}
	p->i = NONE;
	p->p = SLL_pool;
	SLL_pool = p;
	--SLL_cnt;
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;
	while (p != NULL) {
		SLL_pool = p->p;
		free(p);
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}
	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}