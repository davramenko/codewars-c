#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

// Common Denominators

//#include "vlib.h"

// ****************************************************************************************************

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

// ****************************************************************************************************

long long llpow(long long base, int nexp) {
	long long result = 1ULL;
	while (nexp) {
		if (nexp & 1)
			result *= base;
		nexp >>= 1;
		base *= base;
	}
	return result;
}

struct fract {
	long long num;
	long long denom;
};

#define pfract(p) ((struct fract*)p)

struct fract* fract_new(long long num, long long denom) {
	struct fract* pfract = malloc(sizeof(struct fract));
	pfract->num = num;
	pfract->denom = denom;
	return pfract;
}

struct fact {
	long long prime;
	int nexp;
};

#define pfact(p) ((struct fact*)p)

struct fact* fact_new(long long prime, int nexp) {
	struct fact* pfact = malloc(sizeof(struct fact));
	pfact->prime = prime;
	pfact->nexp = nexp;
	return pfact;
}

PLIST factorize(long long num) {
	PLIST res = malloc(sizeof(LIST));
	long long i, rest = num;

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
	//{
	//	STRING s;
	//	str_init(&s, 0);
	//	for (i = 0; i < plst_cnt(res); i++)
	//		str_append_format(&s, "{%lld, %d}", pfact(plst_item(res, i))->prime, pfact(plst_item(res, i))->nexp);
	//	printf("%s\n", sstr(s));
	//	str_free(&s);
	//}
	return res;
}

struct fract* fsimplify(long long num, long long denom) {
	PLIST fnum = factorize(num);
	PLIST fdenom = factorize(denom);
	long long fdiv = 1;
	int i, j;

	//printf("fsimplify(%lld, %lld)\n", num, denom);
	for (i = 0; i < plst_cnt(fnum); i++) {
		for (j = 0; j < plst_cnt(fdenom); j++) {
			//printf("frac(%lld**%d / %lld**%d)\n", pfact(plst_item(fnum, i))->prime, pfact(plst_item(fnum, i))->nexp, pfact(plst_item(fdenom, j))->prime, pfact(plst_item(fdenom, j))->nexp);
			if (pfact(plst_item(fnum, i))->prime == pfact(plst_item(fdenom, j))->prime)
				if (pfact(plst_item(fnum, i))->nexp < pfact(plst_item(fdenom, j))->nexp)
					fdiv *= llpow(pfact(plst_item(fnum, i))->prime, pfact(plst_item(fnum, i))->nexp);
				else
					fdiv *= llpow(pfact(plst_item(fdenom, j))->prime, pfact(plst_item(fdenom, j))->nexp);
		}
	}
	//printf("fdiv: %lld\n", fdiv);
	struct fract* res = fract_new(num / fdiv, denom / fdiv);
	lst_free(fnum, true);
	free(fnum);
	lst_free(fdenom, true);
	free(fdenom);
	return res;
}

PLIST simplify_lst(long long (*lst)[2], int row) {
	PLIST res = malloc(sizeof(LIST));
	int i;

	lst_init(res, 0);
	for (i = 0; i < row; i++) {
		struct fract* f = fsimplify(lst[i][0], lst[i][1]);
		lst_add(res, f);
	}
	return res;
}

// row: number of rows of lst
long long** convertFrac(long long (*lst)[2], int row) {
	int i, j;
	long long (*res)[2] = NULL;
	long long lcd;
	LIST lcm_factors;
	PLIST simplified_lst;

	//{
	//	STRING s;
	//	str_init(&s, 0);
	//	for (i = 0; i < row; i++)
	//		str_append_format(&s, "{%lld, %lld}", lst[i][0], lst[i][1]);
	//	printf("%s\n", sstr(s));
	//	str_free(&s);
	//}
	if (row <= 0)
		return NULL;
	//printf("---=== 1 ===---\n");
	simplified_lst = simplify_lst(lst, row);
	//printf("---=== 2 ===---\n");
	//{
	//	STRING s;
	//	str_init(&s, 0);
	//	for (i = 0; i < plst_cnt(simplified_lst); i++)
	//		str_append_format(&s, "{%lld, %lld}", pfract(plst_item(simplified_lst, i))->num, pfract(plst_item(simplified_lst, i))->denom);
	//	printf("%s\n", sstr(s));
	//	str_free(&s);
	//}
	lst_init(&lcm_factors, 0);
	for (i = 0; i < plst_cnt(simplified_lst); i++) {
		PLIST fdenom = factorize(pfract(plst_item(simplified_lst, i))->denom);
		for (j = 0; j < plst_cnt(fdenom); j++) {
			long long prime = pfact(plst_item(fdenom, j))->prime;
			int k, nexp = pfact(plst_item(fdenom, j))->nexp;
			bool ffound = false;
			//printf ("p: %lld**%d\n", prime, nexp);
			for (k = 0; k < slst_cnt(lcm_factors); k++) {
				if (pfact(slst_item(lcm_factors, k))->prime == prime) {
					if (nexp > pfact(slst_item(lcm_factors, k))->nexp)
						pfact(slst_item(lcm_factors, k))->nexp = nexp;
					ffound = true;
					break;
				}
			}
			if (!ffound) {
				lst_add(&lcm_factors, fract_new(pfact(plst_item(fdenom, j))->prime,  pfact(plst_item(fdenom, j))->nexp));
			}
		}
	}
	//printf("---=== 1 ===---\n");
	lcd = 1;
	for (i = 0; i < slst_cnt(lcm_factors); i++)
		lcd *= llpow(pfact(slst_item(lcm_factors, i))->prime, pfact(slst_item(lcm_factors, i))->nexp);
	res = calloc(row * 2, sizeof(long long));
	//printf("---=== 2 ===---; lcd=%lld\n", lcd);
	for (i = 0; i < plst_cnt(simplified_lst); i++) {
		res[i][0] = pfract(plst_item(simplified_lst, i))->num * lcd / pfract(plst_item(simplified_lst, i))->denom;
		//printf("---=== 2.5 ===---\n");
		res[i][1] = lcd;
	}
	//printf("---=== 3 ===---\n");
	//
	lst_free(&lcm_factors, true);
	lst_free(simplified_lst, true);
	free(simplified_lst);
	return (long long **)res;
}


// ****************************************************************************************************

struct sstr {
	int cap;
	char* str;
};

typedef struct sstr STRING;
typedef struct sstr* PSTRING;

#define sstr(s) (s.str)
#define pstr(s) (s->str)

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

void str_append(PSTRING s, char* src) {
	int len1, len2;

	if (s == NULL || src == NULL)
		return;
	if (s->str == NULL)
		str_init(s, 0);
	len1 = (int)strlen(s->str);
	len2 = (int)strlen(src);
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
	strncat(s->str, src, len2);
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

int main() {
	int i;
	{
		long long data[3][2] = { {1, 2}, {1, 3}, {1, 4} };
		long long** r = convertFrac(data, 3);
		{
			STRING s;
			str_init(&s, 0);
			str_append(&s, "result: ");
			for (i = 0; i < 3; i++)
				str_append_format(&s, "{%lld, %lld}", ((long long(*)[2])r)[i][0], ((long long(*)[2])r)[i][1]);
			printf("%s\n", sstr(s));
			str_free(&s);
		}
		if (r) free(r);
	}
	{
		long long data[3][2] = { {69, 130}, {87, 1310}, {3, 4} };
		long long** r = convertFrac(data, 3);
		{
			STRING s;
			str_init(&s, 0);
			str_append(&s, "result: ");
			for (i = 0; i < 3; i++)
				str_append_format(&s, "{%lld, %lld}", ((long long(*)[2])r)[i][0], ((long long(*)[2])r)[i][1]);
			printf("%s\n", sstr(s));
			str_free(&s);
		}
		if (r) free(r);
	}
	{
		long long data[3][2] = { {77, 130}, {84, 131}, {3, 4} };
		long long** r = convertFrac(data, 3);
		{
			STRING s;
			str_init(&s, 0);
			str_append(&s, "result: ");
			for (i = 0; i < 3; i++)
				str_append_format(&s, "{%lld, %lld}", ((long long(*)[2])r)[i][0], ((long long(*)[2])r)[i][1]);
			printf("%s\n", sstr(s));
			str_free(&s);
		}
		if (r) free(r);
	}
}
