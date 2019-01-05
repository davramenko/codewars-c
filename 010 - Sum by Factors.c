#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Sum by Factors

// #include "vlib.h"

// ****************************************************************************************************

#pragma warning(disable : 4996)

struct int_vect {
	int cap;
	int len;
	int* vect;
};

typedef struct int_vect IV;
typedef struct int_vect* PIV;

#define sv_cnt(v) (v.len)
#define pv_cnt(v) (v->len)
#define sv_item(v, i) (v.vect[i])
#define pv_item(v, i) (v->vect[i])

struct sstr {
	int cap;
	char* str;
};

typedef struct sstr STRING;
typedef struct sstr* PSTRING;

#define sstr(s) (s.str)
#define pstr(s) (s->str)

struct slst {
	int cap;
	int len;
	void** ptr;
};

typedef struct slst LIST;
typedef struct slst* PLIST;

#define slst_cnt(l) (l.len)
#define plst_cnt(l) (l->len)
#define slst_item(l, i) (l.ptr[i])
#define plst_item(l, i) (l->ptr[i])

void iv_init(PIV v, int init_cap) {
	if (v == NULL)
		return;
	v->len = 0;
	v->cap = init_cap;
	if (v->cap <= 0)
		v->cap = 16;
	v->vect = calloc(v->cap, sizeof(int));
}

void iv_free(PIV v) {
	if (v == NULL || v->vect == NULL)
		return;
	free(v->vect);
	v->vect = NULL;
}

void iv_add(PIV v, int item) {
	if (v == NULL)
		return;
	if (v->vect == NULL)
		iv_init(v, 0);
	if (v->len >= v->cap) {
		int tcap = v->cap * 2;
		int* buff = calloc(tcap, sizeof(int));
		memcpy(buff, v->vect, sizeof(int) * v->cap);
		free(v->vect);
		v->vect = buff;
		v->cap = tcap;
	}
	v->vect[v->len] = item;
	v->len++;
}

void lst_init(PLIST l, int init_cap) {
	if (l == NULL)
		return;
	l->len = 0;
	l->cap = init_cap;
	if (l->cap <= 0)
		l->cap = 16;
	l->ptr = calloc(l->cap, sizeof(void*));
}

void lst_free(PLIST l, bool fitems) {
	int i;

	if (l == NULL || l->ptr == NULL)
		return;
	if (fitems) {
		for (i = 0; i < l->len; i++)
			free(l->ptr[i]);
	}
	free(l->ptr);
	l->ptr = NULL;
}

void lst_add(PLIST l, void* item) {
	if (l == NULL || item == NULL)
		return;
	if (l->ptr == NULL)
		lst_init(l, 0);
	if (l->len >= l->cap) {
		int tcap = l->cap * 2;
		void** buff = calloc(tcap, sizeof(void*));
		memcpy(buff, l->ptr, sizeof(void*) * l->cap);
		free(l->ptr);
		l->ptr = buff;
		l->cap = tcap;
	}
	l->ptr[l->len] = item;
	l->len++;
}

void str_init(PSTRING s, int init_cap) {
	if (s == NULL)
		return;
	s->cap = init_cap;
	if (s->cap <= 0)
		s->cap = 64;
	s->str = malloc(s->cap);
	memset(s->str, 0, s->cap);
}

void str_free(PSTRING s) {
	if (s == NULL || s->str == NULL)
		return;
	free(s->str);
}

void str_append_format(PSTRING s, char* format, ...) {
	int len1, len2;
	va_list args;

	if (s == NULL || format == NULL)
		return;
	if (s->str == NULL)
		str_init(s, 0);
	len1 = (int)strlen(s->str);
	va_start(args, format);
#if defined(_WIN32)
	len2 = _vscprintf(format, args);
#else
	len2 = vsnprintf(0, 0, format, args);
#endif
	va_end(args);
	if ((len1 + len2 + 1) > s->cap) {
		int tcap = s->cap * 2;
		char* buff;

		if ((len1 + len2 + 1) > tcap)
			tcap = 2 * (len1 + len2 + 1);
		buff = malloc(tcap);
		memset(buff, 0, tcap);
		strncpy(buff, s->str, len1);
		free(s->str);
		s->str = buff;
		s->cap = tcap;
	}
	va_start(args, format);
	vsnprintf(&(s->str[len1]), len2 + 1, format, args);
	va_end(args);
}

// ****************************************************************************************************

struct fact {
	int prime;
	int nexp;
};

#define pfact(p) ((struct fact*)p)

struct fact* fact_new(int prime, int nexp) {
	struct fact* pfact = malloc(sizeof(struct fact));
	pfact->prime = prime;
	pfact->nexp = nexp;
	return pfact;
}

PLIST factorize(int num) {
	PLIST res = malloc(sizeof(LIST));
	int i, rest = num;

	lst_init(res, 0);
	if (num <= 1) {
		lst_add(res, fact_new(1, 1));
	} else {
		for (i = 2; i <= num; i++) {
			int n = 0;
			while (rest % i == 0) {
				n++;
				rest /= i;
			}
			if (n)
				lst_add(res, fact_new(i, n));
			if (rest <= 1)
				break;
		}
	}
	return res;
}

static int int_cmp(const void *a, const void *b)  { 
	const int *ia = (const int *)a;
	const int *ib = (const int *)b;
	return *ia  - *ib; 
} 
// In C return a char *
char* sumOfDivided(int* lst, int l) {
	int i, j, k;
	PLIST pl;
	IV v;
	STRING s;

	if (l == 0)
		return "";
	iv_init(&v, 0);
	for (i = 0; i < l; i++) {
		pl = factorize((int)abs(lst[i]));
		for (j = 0; j < plst_cnt(pl); j++) {
			bool f = false;
			for (k = 0; k < sv_cnt(v); k++) {
				if (pfact(plst_item(pl, j))->prime == sv_item(v, k)) {
					f = true;
					break;
				}
			}
			if (!f)
				iv_add(&v, pfact(plst_item(pl, j))->prime);
		}
		lst_free(pl, true);
		free(pl);
	}
	lst_free(pl, true);
	qsort(v.vect, v.len, sizeof(int), int_cmp);
	str_init(&s, 0);
	for (i = 0; i < sv_cnt(v); i++) {
		int prime = sv_item(v, i);
		int sum = 0;
		for (j = 0; j < l; j++) {
			if (lst[j] % prime == 0)
				sum += lst[j];
		}
		str_append_format(&s, "(%d %d)", prime, sum);
	}
	iv_free(&v);
	return sstr(s);
}

int main() {
	{
		int d1[2] = {12, 15};
		char* sol1 = "(2 12)(3 27)(5 15)";
		char* r = sumOfDivided(d1, 2);
		printf("%s\n%s\n\n", sol1, r);
		free(r);
	}
	{
		int d1[5] = {15,21,24,30,45};
		char* sol1  = "(2 54)(3 135)(5 90)(7 21)";
		char* r = sumOfDivided(d1, 5);
		printf("%s\n%s\n\n", sol1, r);
		free(r);
	}
	{
		int d1[10] = {107, 158, 204, 100, 118, 123, 126, 110, 116, 100};
		char* sol1 = "(2 1032)(3 453)(5 310)(7 126)(11 110)(17 204)(29 116)(41 123)(59 118)(79 158)(107 107)";
		char* r = sumOfDivided(d1, 10);
		printf("%s\n%s\n\n", sol1, r);
		free(r);
	}
}