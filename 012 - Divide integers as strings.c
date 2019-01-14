#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Divide integers as strings
// https://www.codewars.com/kata/divide-integers-as-strings/train/c

bool try_sub(char* a, const char* b) {
	int lena = strlen(a);
	int lenb = strlen(b);
	int i;

	//printf("try_sub: a=\"%s\"\n", a);
	for (i = 0; i < lena; i++)
		if (a[i] < '0' || a[i] > '9')
			return false;
	for (i = 0; i < lenb; i++)
		if (b[i] < '0' || b[i] > '9')
			return false;
	while (*a == '0') {
		for (i = 0; i < lena; i++)
			a[i] = a[i + 1];
		lena--;
	}
	while (*b == '0') lenb--, b++;
	if (lenb == 0 || (lenb == 1 && b[0] == '0'))
		return true;
	if (lena < lenb) {
		//printf("try_sub: return a=\"%s\"\n", a);
		return false;
	} else if (lena == lenb) {
		int diff = strncmp(a, b, lena);
		if (diff < 0) {
			//printf("try_sub: return a=\"%s\"\n", a);
			return false;
		}
		if (diff == 0) {
			a[0] = '0';
			a[1] = 0;
			return true;
		}
	}
	for (i = lena - 1; i >= 0; i--) {
		int dig_a = a[i] - '0';
		int dig_b, j = lenb - lena + i;
		if (j < 0) break;
		dig_b = b[j] - '0';
		if (dig_a >= dig_b) {
			a[i] = (char)(dig_a - dig_b) + '0';
		} else {
			int k, m;
			for (k = i - 1; k >= 0; k--) {
				if (a[k] != '0') {
					a[k]--;
					for (m = k + 1; m < i; m++)
						a[m] = '9';
					a[i] = (char)(dig_a + 10 - dig_b) + '0';
					break;
				}
			}
			if (k < 0) {
				// Should never happen
				fprintf(stderr, "Subtraction error\n");
				exit(1);
			}
		}
	}
	return true;
}

char **divide_strings(char *a, char *b) {
	// An approach that definitely won't get past 2^64 - 1 ;)
	// Just try executing the Submit Tests - they won't get past
	// the later half of the random tests
	// TODO: Rewrite this and come up with a better approach
	// which works for arbitrarily large integers

	//char quotient[21], remainder[21];
	//sprintf(quotient, "%llu", strtoull(a, NULL, 10) / strtoull(b, NULL, 10));
	//sprintf(remainder, "%llu", strtoull(a, NULL, 10) % strtoull(b, NULL, 10));
	//*result = malloc((strlen(quotient) + 1) * sizeof(char));
	//*(result + 1) = malloc((strlen(remainder) + 1) * sizeof(char));
	//for (int i = 0; i < strlen(quotient); i++) *(*result + i) = quotient[i];
	//for (int i = 0; i < strlen(remainder); i++) *(*(result + 1) + i) = remainder[i];
	//*(*result + strlen(quotient)) = *(*(result + 1) + strlen(remainder)) = '\0';
	//return result;

	char **result = malloc(2 * sizeof(char *));
	char* quotient;
	char* remainder;
	char* divisor;
	int lena = strlen(a);
	int lenb = strlen(b);
	int lend;
	int zero_cnt = 0;
	int lenr = (lena + 1) < 4 ? 4 : lena + 1;
	int i, j;

	*result = (quotient = malloc(lenr));
	*(result + 1) = (remainder = malloc(lenr));
	memset(quotient, 0, lenr);
	memset(remainder, 0, lenr);
	//printf("---=== 1 ===---\n");
	for (i = 0; i < lena; i++)
		if (a[i] < '0' || a[i] > '9') {
			quotient[0] = '0';
			//printf("--== 1 ==--");
			return result;
		}
	//printf("---=== 2 ===---\n");
	for (i = 0; i < lenb; i++)
		if (b[i] < '0' || b[i] > '9') {
			quotient[0] = '0';
			//printf("--== 2 ==--");
			return result;
		}
	//printf("---=== 3 ===---\n");
	//printf("---=== 4 ===---\n");
	strncpy(remainder, a, lena);
	while (*remainder == '0') {
		for (i = 0; i < lena; i++)
			remainder[i] = remainder[i + 1];
		lena--;
	}
	while (*b == '0') lenb--, b++;
	if (lenb == 0 || (lenb == 1 && b[0] == '0') || lena < lenb) {
		quotient[0] = '0';
		if (*remainder == 0) {
			remainder[0] = '0';
			remainder[1] = 0;
		}
		//printf("--== 3 ==--");
		return result;
	}
	//printf("---=== 5 ===---\n");
	if (lena == lenb) {
		int diff = strncmp(a, b, lena);
		if (diff < 0) {
			quotient[0] = '0';
			if (*remainder == 0) {
				remainder[0] = '0';
				remainder[1] = 0;
			}
			//printf("--== 4 ==--");
			return result;
		}
		if (diff == 0) {
			quotient[0] = '1';
			remainder[0] = '0';
			remainder[1] = 0;
			//printf("--== 5 ==--");
			return result;
		}
	}
	//printf("---=== 6 ===---\n");
	divisor = malloc(lena + 1);
	memset(divisor, 0, lena + 1);
	strncpy(divisor, b, lenb);
	lend = lenb;
	//printf("0: divisor=\"%s\", zero_cnt=%d\n", divisor, zero_cnt);
	if (lend < lena) {
		while (lend < lena) {
			divisor[lend++] = '0';
			zero_cnt++;
			//printf("l: divisor=\"%s\", zero_cnt=%d\n", divisor, zero_cnt);
		}
		if (strncmp(remainder, divisor, lend) < 0) {
			lend--;
			zero_cnt--;
			divisor[lend] = 0;
			//printf("f: divisor=\"%s\", zero_cnt=%d\n", divisor, zero_cnt);
		}
	}
	i = 0;
	//printf("MAIN_LOOP: divisor=\"%s\", remainder=\"%s\", zero_cnt=%d\n", divisor, remainder, zero_cnt);
	while (zero_cnt >= 0) {
		int cnt = 0;
		int ln_rem = strlen(remainder);

		//printf("1: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", zero_cnt=%d\n", divisor, remainder, quotient, zero_cnt);
		while (try_sub(remainder, divisor))
			cnt++;
		//printf("2: divisor=\"%s\", remainder=\"%s\", zero_cnt=%d, cnt=%d\n", divisor, remainder, zero_cnt, cnt);
		quotient[i++] = (char)cnt + '0';
		lend--;
		zero_cnt--;
		divisor[lend] = 0;
		if (remainder[0] == '0') {
			for (j = 0; j < ln_rem; j++)
				remainder[j] = remainder[j + 1];
			ln_rem--;
		}
		if (zero_cnt < 0) break;
		//for (j = 0; j < ln_rem; j++)
		//	remainder[j] = remainder[j + 1];
		//printf("3: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", zero_cnt=%d, cnt=%d\n", divisor, remainder, quotient, zero_cnt, cnt);
		//if (*remainder == 0) {
		//	remainder[0] = '0';
		//	remainder[1] = 0;
		//}
		//printf("WWW: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", zero_cnt=%d\n", divisor, remainder, quotient, zero_cnt);
		//if (*divisor == 0) break;
		//printf("QQQ: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", zero_cnt=%d\n", divisor, remainder, quotient, zero_cnt);
		//while (remainder[0] == '0' && divisor[lend - 1] == '0' && zero_cnt >= 0) {
		//	quotient[i++] = '0';
		//	for (j = 0; j < ln_rem; j++)
		//		remainder[j] = remainder[j + 1];
		//	lend--;
		//	zero_cnt--;
		//	ln_rem--;
		//	divisor[lend] = 0;
		//	printf("XXX: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", zero_cnt=%d\n", divisor, remainder, quotient, zero_cnt);
		//}
	}
	//printf("YYY: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", rem_len=%d\n", divisor, remainder, quotient, strlen(remainder));
	while (remainder[0] == '0') {
		int len = strlen(remainder);
		for (i = 0; i < len; i++)
			remainder[i] = remainder[i + 1];
	}
	//printf("ZZZ: divisor=\"%s\", remainder=\"%s\", quotient=\"%s\", rem_len=%d\n", divisor, remainder, quotient, strlen(remainder));
	if (*remainder == 0) {
		remainder[0] = '0';
		remainder[1] = 0;
	}
	//printf("--== end ==--");
	if (divisor) free(divisor);
	return result;
}

int main() {
/*
	{
		char* a = malloc(64);
		char* b = "13000";
		strcpy(a, "13012");
		printf("%s - %s = ", a, b);
		if (try_sub(a, b))
			printf("%s\n", a);
		else
			printf("ERROR\n");
		free(a);
	}
	{
		char* a = malloc(64);
		char* b = "0000000123";
		strcpy(a, "034567");
		printf("%s - %s = ", a, b);
		if (try_sub(a, b))
			printf("%s\n", a);
		else
			printf("ERROR\n");
		free(a);
	}
	{
		char* a = malloc(64);
		char* b = "000023";
		strcpy(a, "200000");
		printf("%s - %s = ", a, b);
		if (try_sub(a, b))
			printf("%s\n", a);
		else
			printf("ERROR\n");
		free(a);
	}
*/
	{
		printf("0 / 5 = ");
		char **actual = divide_strings("0", "5");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("4 / 5 = ");
		char **actual = divide_strings("4", "5");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	// *********
	{
		printf("10 / 2 = ");
		char **actual = divide_strings("10", "2");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("20 / 3 = ");
		char **actual = divide_strings("20", "3");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("200 / 30 = ");
		char **actual = divide_strings("200", "30");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("60 / 5 = ");
		char **actual = divide_strings("60", "5");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("219 / 11 = ");
		char **actual = divide_strings("219", "11");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("729 / 9 = ");
		char **actual = divide_strings("729", "9");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("1000 / 10 = ");
		char **actual = divide_strings("1000", "10");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("600001 / 100 = ");
		char **actual = divide_strings("600001", "100");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	// *************************
	{
		printf("20000 / 23 = ");
		char **actual = divide_strings("20000", "23");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("13047 / 13 = ");
		char **actual = divide_strings("13047", "13");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("13012 / 13 = ");
		char **actual = divide_strings("13012", "13");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		printf("13128 / 13 = ");
		char **actual = divide_strings("13128", "13");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		// 23924988873 (297908388)
		printf("9048888453178660005 / 378219129 = ");
		char **actual = divide_strings("9048888453178660005", "378219129");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
	{
		// 21876 (79770)
		printf("1937483958 / 88563 = ");
		char **actual = divide_strings("1937483958", "88563");
		printf("%s (rem: %s)\n", *actual, *(actual + 1));
		free(*actual);
		free(*(actual + 1));
		free(actual);
	}
}