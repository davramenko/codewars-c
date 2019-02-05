#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Magnet particules in boxes
// https://www.codewars.com/kata/magnet-particules-in-boxes/train/c
// https://github.com/gideo/CodeWars/blob/master/4kyu_MagnetParticulesInBoxes.js

double doubles(int maxk, int maxn) {
	double s = 0;
	int k = 1, n;

	while(k <= maxk) {
		for(n = 1; n <= maxn; n++) 
			s += 1.0 / (((double)k) * pow(n + 1, 2 * k));
		k++;
	}
	return s;
}

int main() {
    printf("[%lf]\n", doubles(1, 10)); //, 0.5580321939764581);
    printf("[%lf]\n", doubles(10, 1000)); //, 0.6921486500921933);
    printf("[%lf]\n", doubles(10, 10000)); //, 0.6930471674194457);
}