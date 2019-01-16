#include <stdbool.h>

// Sudoku Solution Validator
// https://www.codewars.com/kata/sudoku-solution-validator/train/c

#define SZ 9
#define SQSZ 3

void ssort(char* s) {
	int i, n = SZ;
	bool fswap;
	char ch;

	do {
		fswap = false;
		for (i = 1; i < n; i++) {
			if (s[i - 1] > s[i]) {
				ch = s[i - 1];
				s[i - 1] = s[i];
				s[i] = ch;
				fswap = 1;
			}
		}
		n--;
	} while(fswap);
}

void get_row(unsigned int board[9][9], int num, char *res) {
	int i, j;

	if (!res) return;
	memset(res, '0', SZ);
	res[SZ] = 0;

	i = 0;
	for (j = 0; j < SZ; j++)
		res[i++] = (char)(board[num][j]) + '0';
	ssort(res);
}

void get_col(unsigned int board[9][9], int num, char *res) {
	int i, j;

	if (!res) return;
	memset(res, '0', SZ);
	res[SZ] = 0;

	i = 0;
	for (j = 0; j < SZ; j++)
		res[i++] = (char)(board[j][num]) + '0';
	ssort(res);
}

void get_square(unsigned int board[9][9], int num, char *res) {
	int i, j, k;
	int frow = (num / SQSZ) * SQSZ;
	int fcol = (num % SQSZ) * SQSZ;

	if (!res) return;
	memset(res, '0', SZ);
	res[SZ] = 0;

	k = 0;
	for (i = frow; i < (frow + SQSZ); i++) {
		for (j = fcol; j < (fcol + SQSZ); j++) {
			res[k++] = (char)(board[i][j]) + '0';
		}
	}
	ssort(res);
}


bool validSolution(unsigned int board[9][9]){
	char* pattern = "123456789";
	int i;
	bool res = true;

	if (!board) return false;
	for (i = 0; i < SZ; i++) {
		char s[16];

		get_row(board, i, s);
		if (strcmp(s, pattern) != 0) {
			res = false;
			break;
		}
		get_col(board, i, s);
		if (strcmp(s, pattern) != 0) {
			res = false;
			break;
		}
		get_square(board, i, s);
		if (strcmp(s, pattern) != 0) {
			res = false;
			break;
		}
	}
	return res;
}

unsigned int example1[9][9] = {{5, 3, 4, 6, 7, 8, 9, 1, 2}, 
                               {6, 7, 2, 1, 9, 5, 3, 4, 8},
                               {1, 9, 8, 3, 4, 2, 5, 6, 7},
                               {8, 5, 9, 7, 6, 1, 4, 2, 3},
                               {4, 2, 6, 8, 5, 3, 7, 9, 1},
                               {7, 1, 3, 9, 2, 4, 8, 5, 6},
                               {9, 6, 1, 5, 3, 7, 2, 8, 4},
                               {2, 8, 7, 4, 1, 9, 6, 3, 5},
                               {3, 4, 5, 2, 8, 6, 1, 7, 9}}; 
                             
unsigned int example2[9][9] = {{5, 3, 4, 6, 7, 8, 9, 1, 2}, 
                               {6, 7, 2, 1, 9, 0, 3, 4, 8},
                               {1, 0, 0, 3, 4, 2, 5, 6, 0},
                               {8, 5, 9, 7, 6, 1, 0, 2, 0},
                               {4, 2, 6, 8, 5, 3, 7, 9, 1},
                               {7, 1, 3, 9, 2, 4, 8, 5, 6},
                               {9, 0, 1, 5, 3, 7, 2, 1, 4},
                               {2, 8, 7, 4, 1, 9, 6, 3, 5},
                               {3, 0, 0, 4, 8, 1, 1, 7, 9}};
int main() {
	printf("{%s}\n", validSolution(example1) ? "true" : "false");
	printf("{%s}\n", validSolution(example2) ? "true" : "false");
}