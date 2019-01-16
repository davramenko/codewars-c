#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Getting along with Integer Partitions
// https://www.codewars.com/kata/55cf3b567fc0e02b0b00000b/train/c
// http://www.cyberforum.ru/cpp-beginners/thread372700.html

// ****************************************************************************************************

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

struct slst {
	int cap;
	int len;
	void** ptr;
};

typedef struct slst LIST;
typedef struct slst* PLIST;

#define plst_cnt(l) (l->len)
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

PIV iv_clone(PIV pv) {
	PIV res;

	if (pv == NULL)
		return NULL;
	if (pv->vect == NULL)
		iv_init(pv, 0);
	res = malloc(sizeof(IV));
	res->cap = pv->cap;
	res->vect = calloc(res->cap, sizeof(int));
	memcpy(res->vect, pv->vect, sizeof(int) * res->cap);
	res->len = pv->len;
	return res;
}

void iv_append(PIV pv1, PIV pv2) {
	if (pv1 == NULL || pv2 == NULL)
		return;
	if (pv1->vect == NULL)
		iv_init(pv1, 0);
	if (pv2->vect == NULL || pv2->len == 0)
		return;
	if ((pv1->len + pv2->len) >= pv1->cap) {
		int tcap = pv1->cap * 2;
		if ((pv1->len + pv2->len) >= tcap)
			tcap = (pv1->len + pv2->len) * 2;
		int* buff = calloc(tcap, sizeof(int));
		memcpy(buff, pv1->vect, sizeof(int) * pv1->cap);
		free(pv1->vect);
		pv1->vect = buff;
		pv1->cap = tcap;
	}
	memcpy(&(pv1->vect[pv1->len]), pv2->vect, sizeof(int) * pv2->len);
	pv1->len += pv2->len;
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

// ****************************************************************************************************

PLIST get_partitions(int n) {
	int i;
	PIV pv, pvc;
	PLIST res, prev;

	res = malloc(sizeof(LIST));
	lst_init(res, 64);
	if (n == 0) {
		pvc = malloc(sizeof(IV));
		iv_init(pvc, 0);
		lst_add(res, pvc);
		return res;
	}
	prev = get_partitions(n - 1);
	for (i = 0; i < plst_cnt(prev); i++) {
		pv = plst_item(prev, i);
		pvc = malloc(sizeof(IV));
		iv_init(pvc, n);
		iv_add(pvc, 1);
		iv_append(pvc, pv);
		lst_add(res, pvc);
		if (pv_cnt(pv) > 0 && (pv_cnt(pv) < 2 || pv_item(pv, 1) > pv_item(pv, 0))) {
			pvc = iv_clone(pv);
			(pvc->vect[0])++;
			lst_add(res, pvc);
		}
		iv_free(pv);
	}
	lst_free(prev, true);
	return res;
}

static int int_cmp(const void *a, const void *b)  { 
	const int *ia = (const int *)a;
	const int *ib = (const int *)b;
	return *ia  - *ib; 
} 

char* part(int n) {
	char *res;
	int range, i, j, p, lp = -1;
	double avg, median;
	PLIST parts = get_partitions(n);
	PIV pv;
	IV prods, uprods;

	iv_init(&prods, plst_cnt(parts));
	for (i = 0; i < plst_cnt(parts); i++) {
		pv = plst_item(parts, i);
		p = 1;
		//printf("{");
		for (j = 0; j < pv_cnt(pv); j++) {
			//if (j > 0) printf(", ");
			//printf("%d", pv_item(pv, j));
			p *= pv_item(pv, j);
		}
		//printf("}\n");
		iv_add(&prods, p);
		iv_free(pv);
	}
	lst_free(parts, true);

	qsort(prods.vect, prods.len, sizeof(int), int_cmp);
	iv_init(&uprods, prods.len);
	for (i = 0; i < prods.len; i++) {
		if (prods.vect[i] == lp) continue;
		lp = prods.vect[i];
		iv_add(&uprods, prods.vect[i]);
	}
	iv_free(&prods);
	range = 0;
	if (uprods.len > 1)
		range = uprods.vect[uprods.len -  1] - uprods.vect[0];
	avg = 0.0;
	//printf("[");
	for (i = 0; i < uprods.len; i++) {
		//if (i > 0) printf(",");
		//printf("%d", uprods.vect[i]);
		avg += (double)uprods.vect[i];
	}
	//printf("]\n");
	avg /= (double)uprods.len;
	i = uprods.len / 2 - 1;
	if (uprods.len == 1)
		median = uprods.vect[0];
	else if ((uprods.len % 2) == 0)
		median = (double)(uprods.vect[i] + uprods.vect[i + 1]) / 2.0;
	else
		median = uprods.vect[i + 1];
	res = malloc(256);
	snprintf(res, 256, "Range: %d Average: %.2lf Median: %.2lf", range, avg, median);
	iv_free(&uprods);
	return res;

/*
	char *res;
	struct partition p;
	int* prods;
	int range, i, j, prod_cap = 256, prod_len = 0;
	double avg, median;
	bool cont;

	p.n = (short)n;
	p.sn = p.q = 0;
	p.sset = calloc(n, sizeof(int));
	prods = calloc(prod_cap, sizeof(int));
	cont = getfirst(&p);
	while (cont) {
		int prod = 1;
		for (i = 0; i < p.sn; i++) {
			prod *= p.sset[i];
		}
		if (prod_len >= prod_cap) {
			unsigned tcap = prod_cap * 2;
			unsigned* buff = calloc(tcap, sizeof(int));
			memcpy(buff, prods, sizeof(int) * prod_cap);
			free(prods);
			prods = buff;
			prod_cap = tcap;
		}
		prods[prod_len++] = prod;
		cont = getnext(&p);
	}
	qsort(prods, prod_len, sizeof(int), int_cmp);
	for (i = 0; i < prod_len; i++) {
		while ((i + 1) < prod_len && prods[i + 1] == prods[i]) {
			for (j = i + 1; (j + 1) < prod_len; j++)
				prods[j] = prods[j + 1];
			prod_len--;
		}
	}
	range = 0;
	if (prod_len > 1)
		range = prods[prod_len -  1] - prods[0];
	avg = 0.0;
	printf("[");
	for (i = 0; i < prod_len; i++) {
		if (i > 0) printf(",");
		printf("%d", prods[i]);
		avg += (double)prods[i];
	}
	printf("]\n");
	avg /= (double)prod_len;
	i = prod_len / 2 - 1;
	if (prod_len == 1)
		median = prods[0];
	else if ((prod_len % 2) == 0)
		median = (double)(prods[i] + prods[i + 1]) / 2.0;
	else
		median = prods[i + 1];
	res = malloc(256);
	snprintf(res, 256, "Range: %d Average: %.2lf Median: %.2lf", range, avg, median);
	free(prods);
	free(p.sset);
	return res;
*/
}

int main() {
	{
		char* res = part(1);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(2);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(3);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(4);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(5);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(6);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(7);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(8);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(9);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(10);
		printf("{%s}\n", res);
		free(res);
	}
	{
		char* res = part(50);
		printf("{%s}\n", res);
		free(res);
	}
}