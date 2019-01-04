#include <stdio.h>
#include <stdlib.h>

// Simple Fun #176: Reverse Letter

char *reverse_letter(const char *str) {
	size_t len = strlen(str);
	char* ptr = malloc(len + 1);
	int i, j;

	memset(ptr, 0, len + 1);
	for (j = 0, i = len - 1; i >= 0; --i) {
		if (isalpha(str[i]))
			ptr[j++] = str[i];
	}
	return ptr;
}

int main() {
	char *ptr;
    printf("%s\n", ptr = reverse_letter("krishan"));
    free(ptr);
    printf("%s\n", ptr = reverse_letter("ultr53o?n"));
    free(ptr);
    printf("%s\n", ptr = reverse_letter("ab23c"));
    free(ptr);
    printf("%s\n", ptr = reverse_letter("krish21an"));
    free(ptr);
}
