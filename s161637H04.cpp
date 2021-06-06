#include <stdlib.h>
#include <stdio.h>
#include <math.h>
void mergeSort(double* X, unsigned* Xid, int left, int right, unsigned* TMP);
void merge(double*X,unsigned* Xid, int left, int mid, int right, unsigned* TMP);

void mergeSort(double *X, unsigned* Xid, int left, int right, unsigned* TMP) {
    int mid;
    if (left < right) { // �迭�� �� ������ �� �� �ִٸ� 
        mid = (left + right) / 2;
        mergeSort(X,Xid, left, mid,TMP);
        mergeSort(X,Xid ,mid + 1, right,TMP); 
        merge(X,Xid, left, mid, right,TMP); 
    }
}

void merge(double* X,unsigned* Xid, int left, int mid, int right, unsigned*TMP) {  //L~M�迭, M+1~R���� 2�� �迭�� �ϳ��� �迭�� �����Ͽ� �����
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
    if (R - L + 1 > THR) { // �Ӱ谪���� ũ�ٸ�
        
        dL = closestPairDC(L, M, &lp1, &lp2, X, Y, Xid, Yid, TMP, THR);
        dR = closestPairDC(M + 1, R, &rp1, &rp2, X, Y, Xid, Yid, TMP, THR);
        merge(Y, Yid, L, M, R, TMP);
        //���� ������ pt1 pt2 ����
        if (dL <= dR) {
            *pt1 = lp1;
            *pt2 = lp2;
            dLR = dL;
        }
        else {//������ ������ pt1 pt2 ����
            *pt1 = rp1;
            *pt2 = rp2;
            dLR = dR;
        }

        //���±��� ã�� �ּҰŸ�(dLR)���� ���� pair Ž��
        for (int i = L; i < R; i++) {
            for (int j = i + 1; j <= R; j++) {      
                if (Y[Yid[j]] - Y[Yid[i]] >= dLR) break; // Ž�� �����൵ �Ǵ� �κ� ����
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
    else { // �Ӱ谪���� �۴ٸ� 
        //Xid���� Yid�� ����
        for (int i = L; i <= R; i++) {
            Yid[i] = Xid[i];
        }
        //Yid �������� O(n^2)
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

        //  pair Ž��
        for (int i = L; i < R; i++) {
            for (int j = i + 1; j <= R; j++) {
                if (Y[Yid[j]] - Y[Yid[i]] >= dLR) break;   // Ž�� �����൵ �Ǵ� �κ� ����           
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

