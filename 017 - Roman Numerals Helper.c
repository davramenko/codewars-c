#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Roman Numerals Helper
// https://www.codewars.com/kata/roman-numerals-helper/train/c
// https://code.sololearn.com/co4RsqM8ryVm/#py

char* aroman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I", NULL};
int adecimal[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1, 0};

#if !defined(_WIN32)
#define strnicmp strncasecmp
#endif

int find_roman(char* p, int len) {
	int i, idx = -1;
	if (len > 0 && len <= 2) {
		for (i = 0; aroman[i] != NULL; i++) {
			if (strlen(aroman[i]) == len && strnicmp(aroman[i], p, len) == 0) {
				idx = i;
				break;
			}
		}
	}
	return idx;
}

int from_roman(char* roman) {
	int l, i, num = 0;
	char *p = roman;
	while ((l = strlen(p)) > 0) {
		if (l >= 2 && (i = find_roman(p, 2)) >= 0) {
			p += 2;
		} else if ((i = find_roman(p, 1)) >= 0) {
			p++;
		} else {
			num = 0;
			break;
		}
		num += adecimal[i];
	}
	return num;
}

void to_roman(int number, char* destination) {
	int i;
	destination[0] = 0;
	for (i = 0; adecimal[i] > 0; i++) {
		while (number >= adecimal[i]) {
			number -= adecimal[i];
			strcat(destination, aroman[i]);
		}
	}
}

int main()  {
	int res;
	char test[100] = {0};

	if ((res = from_roman("XXI")) != 21)
		printf("\"XXI\" should be: 21 but %d returned\n", res);
	if ((res = from_roman("I")) != 1)
		printf("\"I\" should be: 1 but %d returned\n", res);
	if ((res = from_roman("XXX")) != 30)
		printf("\"XXX\" should be: 30 but %d returned\n", res);

	if ((res = from_roman("MCMXC")) != 1990)
		printf("\"MCMXC\" should be: 1990 but %d returned\n", res);
	if ((res = from_roman("MMVIII")) != 2008)
		printf("\"MMVIII\" should be: 2008 but %d returned\n", res);
	if ((res = from_roman("MDCLXVI")) != 1666)
		printf("\"MDCLXVI\" should be: 1666 but %d returned\n", res);


	memset(test, 0, sizeof(test));
	to_roman(21, test);
	if (strcmp(test, "XXI") != 0)
		printf("21 should be: XXI but \"%s\" returned\n", test);

	memset(test, 0, sizeof(test));
	to_roman(1, test);
	if (strcmp(test, "I") != 0)
		printf("1 should be: I but \"%s\" returned\n", test);

	memset(test, 0, sizeof(test));
	to_roman(30, test);
	if (strcmp(test, "XXX") != 0)
		printf("30 should be: XXX but \"%s\" returned\n", test);


	memset(test, 0, sizeof(test));
	to_roman(1990, test);
	if (strcmp(test, "MCMXC") != 0)
		printf("1990 should be: MCMXC but \"%s\" returned\n", test);

	memset(test, 0, sizeof(test));
	to_roman(2008, test);
	if (strcmp(test, "MMVIII") != 0)
		printf("2008 should be: MMVIII but \"%s\" returned\n", test);

	memset(test, 0, sizeof(test));
	to_roman(1666, test);
	if (strcmp(test, "MDCLXVI") != 0)
		printf("1666 should be: MDCLXVI but \"%s\" returned\n", test);

}
