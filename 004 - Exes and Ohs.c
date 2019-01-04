#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Exes and Ohs

bool xo (const char* str)
{
	int i, n = 0, m = 0, len = strlen(str);

	printf("%s\n", str);
	for (i = 0; i < len; i++) {
		if (str[i] == 'x' || str[i] == 'X')
			n++;
		if (str[i] == 'o' || str[i] == 'O')
			m++;
	}
	return (n == m);
}

int main() {
	printf("%s\n", xo("xo") ? "true" : "false");
	printf("%s\n", xo("Xo") ? "true" : "false");
	printf("%s\n", xo("xxOo") ? "true" : "false");
	printf("%s\n", xo("xxxm") ? "true" : "false");
	printf("%s\n", xo("Oo") ? "true" : "false");
	printf("%s\n", xo("ooom") ? "true" : "false");
}
