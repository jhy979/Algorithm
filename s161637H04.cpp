#include <stdlib.h>
#include <stdio.h>
#include <math.h>
void mergeSort(double* X, unsigned* Xid, int left, int right, unsigned* TMP);
void merge(double*X,unsigned* Xid, int left, int mid, int right, unsigned* TMP);

void mergeSort(double *X, unsigned* Xid, int left, int right, unsigned* TMP) {
    int mid;
    if (left < right) { // 배열이 더 나뉘어 질 수 있다면 
        mid = (left + right) / 2;
        mergeSort(X,Xid, left, mid,TMP);
        mergeSort(X,Xid ,mid + 1, right,TMP); 
        merge(X,Xid, left, mid, right,TMP); 
    }
}

void merge(double* X,unsigned* Xid, int left, int mid, int right, unsigned*TMP) {  //L~M배열, M+1~R까지 2개 배열을 하나의 배열로 정렬하여 만들기
    int i = left, j = mid + 1, k = left;
  
    while (i <= mid && j <= right) {
        if (X[Xid[i]] <= X[Xid[j]]) TMP[k++] = Xid[i++];
        else TMP[k++] = Xid[j++];
    }
    while (i <= mid) 
        TMP[k++] = Xid[i++];
    while (j <= right) 
        TMP[k++] = Xid[j++];

    for (int a = left; a <= right; a++)
        Xid[a] = TMP[a];
}

void sortXid(double* X, unsigned* Xid, unsigned* TMP, unsigned N) {
    mergeSort(X, Xid, 0, N-1,TMP);
}
double closestPairDC(
    unsigned L, unsigned R,	  // current leftmost and rightmost indices
    unsigned* pt1, unsigned* pt2, // closest pair points indices
    double* X, double* Y,         // (x,y) positions array(input)
    unsigned* Xid,  // point index array(sorted by x coordinates)
    unsigned* Yid,  // point index array(gradually sorted by y coordinates)
    unsigned* TMP,  // temporal index array
    unsigned THR	// threshold value
) 
{
    unsigned M = (L + R) / 2;
    double dL, dR, dLR = 100000000;
    double d;
    unsigned lp1, lp2, rp1, rp2, temp = 0;
    if (R - L + 1 > THR) { // 임계값보다 크다면
        
        dL = closestPairDC(L, M, &lp1, &lp2, X, Y, Xid, Yid, TMP, THR);
        dR = closestPairDC(M + 1, R, &rp1, &rp2, X, Y, Xid, Yid, TMP, THR);
        merge(Y, Yid, L, M, R, TMP);
        //왼쪽 값으로 pt1 pt2 갱신
        if (dL <= dR) {
            *pt1 = lp1;
            *pt2 = lp2;
            dLR = dL;
        }
        else {//오른쪽 값으로 pt1 pt2 갱신
            *pt1 = rp1;
            *pt2 = rp2;
            dLR = dR;
        }

        //여태까지 찾은 최소거리(dLR)보다 작은 pair 탐색
        for (int i = L; i < R; i++) {
            for (int j = i + 1; j <= R; j++) {      
                if (Y[Yid[j]] - Y[Yid[i]] >= dLR) break; // 탐색 안해줘도 되는 부분 제외
                d = sqrt((X[Yid[i]] - X[Yid[j]]) * (X[Yid[i]] - X[Yid[j]]) + (Y[Yid[i]] - Y[Yid[j]]) * (Y[Yid[i]] - Y[Yid[j]]));
                if (d < dLR) {
                    *pt1 = Yid[i];
                    *pt2 = Yid[j];
                    dLR = d;
                }
            }
        }
        return dLR;
    }
    else { // 임계값보다 작다면 
        //Xid값을 Yid로 복사
        for (int i = L; i <= R; i++) {
            Yid[i] = Xid[i];
        }
        //Yid 선택정렬 O(n^2)
        for (int i = L; i < R; i++) {
            int Min_idx = i;
            for (int j = i + 1; j <= R; j++) {
                if (Y[Yid[Min_idx]] > Y[Yid[j]]) {
                    Min_idx = j;
                }
            }
            temp = Yid[Min_idx];
            Yid[Min_idx] = Yid[i];
            Yid[i] = temp;
        }

        //  pair 탐색
        for (int i = L; i < R; i++) {
            for (int j = i + 1; j <= R; j++) {
                if (Y[Yid[j]] - Y[Yid[i]] >= dLR) break;   // 탐색 안해줘도 되는 부분 제외           
                d = sqrt((X[Yid[i]] - X[Yid[j]]) * (X[Yid[i]] - X[Yid[j]]) + (Y[Yid[i]] - Y[Yid[j]]) * (Y[Yid[i]] - Y[Yid[j]]));
                if (d < dLR) {
                    *pt1 = Yid[i];
                    *pt2 = Yid[j];
                    dLR = d;
                }
            }
        }
        return dLR;
    }
}

