#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Multiplying numbers as strings
// https://www.geeksforgeeks.org/multiply-large-numbers-represented-as-strings/
// https://www.geeksforgeeks.org/divide-large-number-represented-string/

char *multiply(char *a, char *b) {
	int len1;
	int len2;
	int i_n1 = 0;
	int i_n2 = 0;
	char* result = NULL;
	int i, j, res_sz = 0;

	while (*a == '0' || *a == ' ') a++;
	while (*b == '0' || *b == ' ') b++;
	len1 = strlen(a);
	len2 = strlen(b);
	if (len1 == 0 || len2 == 0)
		return "0";

	result = malloc(len1 + len2 + 1);
	memset(result, 0, len1 + len2 + 1);
	for (i = len1 - 1; i >= 0; i--) {
		int carry = 0;
		int n1 = a[i] - '0';
   
		i_n2 = 0;
		for (j = len2 - 1; j >= 0; j--) {
			int n2 = b[j] - '0';
			int sum = n1 * n2 + result[i_n1 + i_n2] + carry;

			if (res_sz < (i_n1 + i_n2 + 1))
				res_sz = i_n1 + i_n2 + 1;
			carry = sum / 10;
			result[i_n1 + i_n2] = sum % 10;
			i_n2++;
		}
		if (carry > 0) {
			result[i_n1 + i_n2] += carry;
			if (res_sz < (i_n1 + i_n2 + 1))
				res_sz = i_n1 + i_n2 + 1;
		}
		i_n1++; 
	} 
	i = res_sz - 1;
	while (i >= 0 && result[i] == 0)
		i--;
	if (i < 0)
		return "0";
    for (i = 0; i < res_sz / 2; i++) {
    	int tmp = result[i];
    	result[i] = result[res_sz - i - 1];
    	result[res_sz - i - 1] = tmp;
    }
    for (i = 0; i < res_sz; i++)
    	result[i] += '0';
	return result;
}

int main() {
	char* r;
	printf("[%s]\n{%s}\n\n", r = multiply("2", "3"), "6");
	if (r) free(r);
	printf("[%s]\n{%s}\n\n", r = multiply("30", "69"), "2070");
	if (r) free(r);
	printf("[%s]\n{%s}\n\n", r = multiply("11", "85"), "935");
	if (r) free(r);
}