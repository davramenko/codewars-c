#include <stdio.h>
#include <stdlib.h>

// Tortoise racing

int* race(int v1, int v2, int g) {
	int* res = NULL;

	res = calloc(3, sizeof(int));
	if (res) {
		if (v1 >= v2) {
			res[0] = res[1] = res[2] = -1;
			return res;
		}
		int t2 = 3600 * g / (v2 - v1);
		res[2] = t2 % 60;
		t2 /= 60;
		res[1] = t2 % 60;
		res[0] = t2 / 60;
	}
	return res;
}

int main() {
	int* a;

	a = race(720, 850, 70);
	if (a) {
		printf("%d:%02d:%02d\n", a[0], a[1], a[2]);
		free(a);
	} else {
		printf("[NULL]\n");
	}
	a = race(820, 81, 550);
	if (a) {
		printf("%d:%02d:%02d\n", a[0], a[1], a[2]);
		free(a);
	} else {
		printf("[NULL]\n");
	}
	a = race(80, 91, 37);
	if (a) {
		printf("%d:%02d:%02d\n", a[0], a[1], a[2]);
		free(a);
	} else {
		printf("[NULL]\n");
	}
}
