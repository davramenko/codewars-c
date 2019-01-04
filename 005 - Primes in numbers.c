#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Primes in numbers

/*
int* get_primes(int max_num, size_t* plen) {
	int i, maxi;
	char* candidates = malloc(max_num);
	size_t pcap = 65536;
	int* primes = NULL;

	for (i = 0; i < max_num; i++)
		candidates[i] = 1;
	maxi = (int)sqrt((double)max_num);
	for (i = 2; i <= maxi; i++) {
		if (candidates[i]) {
			int j, k, sqi = i * i;
			for (k = 0; ; k++) {
				j = sqi + i * k;
				if (j > max_num) break;
				candidates[j] = 0;
			}
		}
	}
	*plen = 0;
	for (i = 2; i < max_num; i++) {
		if (candidates[i]) {
			//printf("prime: %d\n", i);
			if (!primes) {
				primes = calloc(pcap, sizeof(int));
			} else if (*plen >= pcap) {
				int j;
				int* ptr = calloc((pcap *= 2), sizeof(int));
				for (j = 0; j < *plen; j++)
					ptr[j] = primes[j];
				free(primes);
				primes = ptr;
			}
			primes[*plen] = i;
			*plen += 1;
		}
	}
	return primes;
}

char* factors(int lst) {
	size_t plen;
	int i, rest = lst;
	int* primes;
	char* res = NULL;

	//primes = get_primes(lst <= 100000 ? lst + 1 : (int)sqrt(lst), &plen);
	//primes = get_primes(lst + 1, &plen);
	primes = get_primes(lst <= 20000000 ? lst + 1 : 20000000, &plen);
	for (i = 0; i < plen; i++) {
		int n = 0;
		while (rest % primes[i] == 0) {
			n++;
			rest /= primes[i];
		}
		if (n) {
			char buff[64];
			size_t res_sz = 0, buff_sz;
			char* ptr;

			if (n > 1)
				buff_sz = snprintf(buff, sizeof(buff), "(%d**%d)", primes[i], n);
			else
				buff_sz = snprintf(buff, sizeof(buff), "(%d)", primes[i]);
			if (res)
				res_sz = strlen(res);
			ptr = malloc(res_sz + buff_sz + 1);
			memset(ptr, 0, res_sz + buff_sz + 1);
			if (res) {
				strncpy(ptr, res, res_sz);
				free(res);
			}
			strncat(ptr, buff, buff_sz);
			res = ptr;
		}
		if (rest <= 1)
			break;
	}
	return res;
}
*/

char* factors(int lst) {
	int i, rest = lst;
	char* res = NULL;

	for (i = 2; i <= lst; i++) {
		int n = 0;
		while (rest % i == 0) {
			n++;
			rest /= i;
		}
		if (n) {
			char buff[64];
			size_t res_sz = 0, buff_sz;
			char* ptr;

			if (n > 1)
				buff_sz = snprintf(buff, sizeof(buff), "(%d**%d)", i, n);
			else
				buff_sz = snprintf(buff, sizeof(buff), "(%d)", i);
			if (res)
				res_sz = strlen(res);
			ptr = malloc(res_sz + buff_sz + 1);
			memset(ptr, 0, res_sz + buff_sz + 1);
			if (res) {
				strncpy(ptr, res, res_sz);
				free(res);
			}
			strncat(ptr, buff, buff_sz);
			res = ptr;
		}
		if (rest <= 1)
			break;
	}
	return res;
}

int main() {
	printf("%s\n", factors(7775460));
	printf("%s\n", factors(7919));
	printf("%s\n", factors(17*17*93*677));
	printf("%s\n", factors(7537*123863));
	printf("%s\n", factors(7*5113051));
	printf("%s\n", factors(2*18211931));
}