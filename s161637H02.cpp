#include "Graph_adj_array.h"

// 그래프 정보 읽어서 vertex, edge array 생성
void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {

    //vertex arry 초기화
    for (int i = 0; i < Vnum; i++) {
        V[i].f_hd = NONE;
        V[i].r_hd = NONE;
        V[i].name = i;
    }
    //edge array 초기화
    for (int i = 0; i < Enum; i++) {
        E[i].fp = NONE;
        E[i].rp = NONE;
    }
    //입력 받을 버퍼 선언
    int arr[3] = { -1 };

    for (int i = 0; i < Enum; i++) {
        scanf_s("%d %d %d", &arr[0], &arr[1], &arr[2]);
        //edge array 및 vertex array정보 입력
        E[i].vf = arr[0];
        E[i].vr = arr[1];
        E[i].cost = arr[2];

        //앞쪽 링크 값 채우기
        if (V[arr[0]].f_hd == NONE) {
            V[arr[0]].f_hd = i;
        }
        else {
            int front = V[arr[0]].f_hd;
            V[arr[0]].f_hd = i;
            E[i].fp = front;
        }
        //뒤쪽 링크 값 채우기
        if (V[arr[1]].r_hd == NONE) {
            V[arr[1]].r_hd = i;
        }
        else {
            int rear = V[arr[1]].r_hd;
            V[arr[1]].r_hd = i;
            E[i].rp = rear;
        }

    }

}

//DFS tree
int DFS_Tree_adj_array(
    int     src,   // source node index
    int     Vnum,  // number of vertices
    vertex* V,     // vertex structure array (starting index 0)
    int     Enum,  // number of edges
    edge* E     // edge structure array (starting index 0) 
) {
    int cost_sum = 0;
    //방문한  vertex라면 빠져나오기
    if (V[src].flag == true)
        return cost_sum;

    // vertex 방문 check
    V[src].flag = true; 

    int front = V[src].f_hd;
    int rear = V[src].r_hd;
    
    //앞쪽 링크 돌면서 방문하지 않은 vertex있다면 재귀함수 호출
    while (front != -1) {
        if (V[E[front].vr].flag == false) {
            E[front].flag = true;   //지나간 간선을 방문처리
            cost_sum += E[front].cost;
            cost_sum += DFS_Tree_adj_array(E[front].vr, Vnum, V, Enum, E);
        }
        front = E[front].fp;
    }
    //뒤쪽 링크 돌면서 방문하지 않은 vertex있다면 재귀함수 호출
    while (rear != -1) {
        if (V[E[rear].vf].flag == false) {
            E[rear].flag = true;    //지나간 간선을 방문처리
            cost_sum += E[rear].cost;
            cost_sum += DFS_Tree_adj_array(E[rear].vf, Vnum, V, Enum, E);
        }
        rear = E[rear].rp;
    }

    return cost_sum;
  
}   