#include <cstdlib>
#include <cstring>
#define MIN(A,B)  A>B ? B : A

int min(int ins, int del, int sub) {
	int temp = (ins > del) ? del : ins;
	return (temp > sub) ? sub : temp;
}
void Edit_Distance(
	char* SS, char* TS,
	int ins_cost, int del_cost, int sub_cost,
	int** Table,
	char** SR, char** OP, char**
	TR, int* Mem_Allocated)
{
	int len_SS = strlen(SS);
	int len_TS = strlen(TS);

	//0번 행, 열 초기화
	for (int i = 0; i <= len_SS; i++) Table[i][0] = i * del_cost;
	for (int i = 0; i <= len_TS; i++) Table[0][i] = i * ins_cost;

	//테이블 생성
	for (int i = 1; i <= len_SS; i++) {
		for (int j = 1; j <= len_TS; j++) {
			int alpha = SS[i - 1] == TS[j - 1] ? 0 : sub_cost;
			Table[i][j] = MIN(MIN(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost),Table[i-1][j-1]+alpha);
				//MIN(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + alpha);
		}
	}

	int i = len_SS;
	int j = len_TS;
	int count = 0;

	//SR,OP,TR 할당을 위해 back tracing으로 사이즈 알아내기 
	while (1) {
		if (i >= 1 && j >= 1) {
			int alpha = (SS[i - 1] == TS[j - 1]) ? 0 : sub_cost;
			int Ins_Size = Table[i][j - 1] + ins_cost;
			int Sub_Size = Table[i - 1][j - 1] + alpha;
			int Del_Size = Table[i - 1][j] + del_cost;
			//insert인경우
			if (Ins_Size < Sub_Size && Ins_Size < Del_Size) {
				j--;
			}
			//delete인경우
			else if (Del_Size < Sub_Size && Del_Size < Ins_Size) {
				i--;
			}
			//substitution인경우
			else if (Sub_Size < Ins_Size && Sub_Size < Del_Size) {
				i--;
				j--;
			}
			//3개 모두 후보가 될 경우
			else if (Ins_Size == Del_Size && Del_Size == Sub_Size) {
				if (ins_cost <= del_cost && ins_cost <= alpha) {
					j--;
				}
				else if (del_cost < ins_cost && del_cost <= alpha) {
					i--;
				}
				else {
					i--;
					j--;
				}
			}
			// 2개 후보 중 ins선택
			else if ((Ins_Size == Del_Size && ins_cost <= del_cost) || (Sub_Size == Ins_Size && ins_cost <= alpha)) {
				j--;
			}
			// 2개 후보 중 del선택
			else if ((Ins_Size == Del_Size && ins_cost > del_cost) || (Sub_Size == Del_Size && del_cost <= alpha)) {
				i--;
			}
			//2개 후보 중  sub선택
			else if ((Sub_Size == Del_Size && del_cost > alpha) || (Sub_Size == Ins_Size && ins_cost > alpha)) {
				i--;
				j--;
			}
		}
		//마지막 i=0일때 끝처리 ins
		else if (i == 0) {
			j--;
		}
		//마지막 j=0일때 끝처리 del
		else {
			i--;
		}
		count++;
		if (i == 0 && j == 0)
			break;
	}
	//Mem_allocated 할당
	*Mem_Allocated = 3 * count + 3;
	// SR,OP,TR 할당
	*SR = new char[count + 1];
	*OP = new char[count + 1];
	*TR = new char[count + 1];
	i = len_SS;
	j = len_TS;
	//끝처리
	(*SR)[count] = '\0';
	(*OP)[count] = '\0';
	(*TR)[count] = '\0';

	//back tracing을 통해 연산 확인하기
	while (1) {
		if (i >= 1 && j >= 1) {
			int alpha = (SS[i - 1] == TS[j - 1]) ? 0 : sub_cost;
			int Ins_Size = Table[i][j - 1] + ins_cost;
			int Sub_Size = Table[i - 1][j - 1] + alpha;
			int Del_Size = Table[i - 1][j] + del_cost;
			//insert인경우
			if (Ins_Size < Sub_Size && Ins_Size < Del_Size) {
				(*SR)[count - 1] = '*';
				(*OP)[count - 1] = 'i';
				(*TR)[count - 1] = TS[j - 1];
				j--;
			}
			//delete인경우
			else if (Del_Size < Sub_Size && Del_Size < Ins_Size) {
				(*SR)[count - 1] = SS[i - 1];
				(*OP)[count - 1] = 'd';
				(*TR)[count - 1] = '*';
				i--;
			}
			//substitution인경우
			else if (Sub_Size < Ins_Size && Sub_Size < Del_Size) {
				(*SR)[count - 1] = SS[i - 1];
				(*OP)[count - 1] = (alpha == 0 ? '.' : 's');
				(*TR)[count - 1] = TS[j - 1];
				i--;
				j--;
			}
			//3개 모두 후보가 될 경우
			else if (Ins_Size == Del_Size && Del_Size == Sub_Size) {
				if (ins_cost <= del_cost && ins_cost <= alpha) {	//ins
					(*SR)[count - 1] = '*';
					(*OP)[count - 1] = 'i';
					(*TR)[count - 1] = TS[j - 1];
					j--;
				}
				else if (del_cost < ins_cost && del_cost <= alpha) {	//del
					(*SR)[count - 1] = SS[i - 1];
					(*OP)[count - 1] = 'd';
					(*TR)[count - 1] = '*';
					i--;
				}
				else {	//sub
					(*SR)[count - 1] = SS[i - 1];
					(*OP)[count - 1] = (alpha == 0 ? '.' : 's');
					(*TR)[count - 1] = TS[j - 1];
					i--;
					j--;
				}
			}
			// 2개 후보 중 ins선택
			else if ((Ins_Size == Del_Size && ins_cost <= del_cost) || (Sub_Size == Ins_Size && ins_cost <= alpha)) {
				(*SR)[count - 1] = '*';
				(*OP)[count - 1] = 'i';
				(*TR)[count - 1] = TS[j - 1];
				j--;
			}
			// 2개 후보 중 del선택
			else if ((Ins_Size == Del_Size && ins_cost > del_cost) || (Sub_Size == Del_Size && del_cost <= alpha)) {
				(*SR)[count - 1] = SS[i - 1];
				(*OP)[count - 1] = 'd';
				(*TR)[count - 1] = '*';
				i--;
			}
			// 2개 후보 중 sub선택
			else if ((Sub_Size == Del_Size && del_cost > alpha) || (Sub_Size == Ins_Size && ins_cost > alpha)) {
				(*SR)[count - 1] = SS[i - 1];
				(*OP)[count - 1] = (alpha == 0 ? '.' : 's');
				(*TR)[count - 1] = TS[j - 1];
				i--;
				j--;
			}
		}
		else if (i == 0) { //끝처리 ins
			(*SR)[count - 1] = '*';
			(*OP)[count - 1] = 'i';
			(*TR)[count - 1] = TS[j - 1];
			j--;
		}
		else { //끝처리 del
			(*SR)[count - 1] = SS[i - 1];
			(*OP)[count - 1] = 'd';
			(*TR)[count - 1] = '*';
			i--;
		}
		count--;
		//탈출
		if (i == 0 && j == 0)
			break;
	}

}