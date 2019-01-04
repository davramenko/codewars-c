#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Which are in?

static int my_compare (const void * a, const void * b)  { 
	return strcmp (*(const char **) a, *(const char **) b); 
} 

// sz1: size of array1, sz2: size of array2, lg: size of the returned array
char** inArray(char* array1[], int sz1, char* array2[], int sz2, int* lg) {
	int i, j, k, n = 0;
	char** buff = NULL;
	char** ptr;

	*lg = 0;
	for (i = 0; i < sz1; i++) {
		for (j = 0; j < sz2; j++) {
			printf("Looking \"%s\" in \"%s\"\n", array1[i], array2[j]);
			if (strstr(array2[j], array1[i]) != NULL) {
				printf("FOUND\n");
				ptr = (char**)calloc(n + 1, sizeof(char*));
				if (ptr == NULL) {
					fprintf(stderr, "Cannot allocate memory\n");
					exit(1);
				}
				if (n > 0) {
					for (k = 0; k < n; k++) {
						ptr[k] = buff[k];
					}
				}
				if (buff) free(buff);
				buff = ptr;
				buff[n] = array1[i];
				n++;
				break;
			}
		}
	}
	if (n > 0)
		qsort (buff, n, sizeof (const char *), my_compare);
	*lg = n;
	return buff;
}

int main() {
	int i, lg;
	char **res;

	{
		char* arr1[3] = { "arp", "live", "strong" };
		char* arr2[5] = { "lively", "alive", "harp", "sharp", "armstrong" };
		res = inArray(arr1, 3, arr2, 5, &lg);
		for (i = 0; i < lg; i++) {
			printf("[%s]\n", res[i]);
		}
		if (res) free(res);
	}
	printf("===========================================\n");
	{
		char* arr1[7] = { "cod", "code", "wars", "ewar", "pillow", "bed", "phht" };
		char* arr2[8] = { "lively", "alive", "harp", "sharp", "armstrong", "codewars", "cod", "code" };
		res = inArray(arr1, 3, arr2, 5, &lg);
		for (i = 0; i < lg; i++) {
			printf("[%s]\n", res[i]);
		}
		if (res) free(res);
	}
	printf("===========================================\n");
}
