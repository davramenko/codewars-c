#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Strings Mix

// #include "vlib.h"

// ****************************************************************************************************

#pragma warning(disable : 4996)

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

void str_init(PSTRING s, int init_cap) {
	if (s == NULL)
		return;
	s->cap = init_cap;
	if (s->cap <= 0)
		s->cap = 64;
	s->str = malloc(s->cap);
	memset(s->str, 0, s->cap);
}

void str_append_char(PSTRING s, char ch) {
	int len;

	if (s == NULL || ch == 0)
		return;
	if (s->str == NULL)
		str_init(s, 0);
	len = (int)strlen(s->str);
	if ((len + 1) > s->cap) {
		int tcap = s->cap * 2;
		char* buff;

		buff = malloc(tcap);
		memset(buff, 0, tcap);
		strncpy(buff, s->str, len);
		free(s->str);
		s->str = buff;
		s->cap = tcap;
	}
	s->str[len] = ch;
}

void str_append_char_cnt(PSTRING s, char ch, int cnt) {
	int len, i;

	if (s == NULL || ch == 0 || cnt <= 0)
		return;
	if (s->str == NULL)
		str_init(s, 0);
	len = (int)strlen(s->str);
	if ((len + cnt + 1) > s->cap) {
		int tcap = s->cap * 2;
		char* buff;

		if ((len + cnt + 1) > tcap)
			tcap = 2 * (len + cnt + 1);
		buff = malloc(tcap);
		memset(buff, 0, tcap);
		strncpy(buff, s->str, len);
		free(s->str);
		s->str = buff;
		s->cap = tcap;
	}
	for (i = 0; i < cnt; i++)
		s->str[len++] = ch;
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

void* lst_rm_at(PLIST l, int index) {
	void* res;
	int i;

	if (l == NULL || index < 0)
		return NULL;
	if (l->ptr == NULL)
		lst_init(l, 0);
	if (index >= l->len)
		return NULL;
	res = l->ptr[index];
	for (i = index + 1; i < l->len; i++)
		l->ptr[i - 1] = l->ptr[i];
	l->len--;
	return res;
}

// ****************************************************************************************************

struct sd {
	char ch;
	int cnt;
	char pos;
	bool used;
};

#define sd_pos_hr(psd) ((psd->pos == 0) ? '=' : '0' + (char)(psd->pos & 0x3))
#define psd(p) ((struct sd*)p)

static struct sd* sd_new(char ch, int cnt) {
	struct sd* psd = malloc(sizeof(struct sd));
	psd->ch = ch;
	psd->cnt = cnt;
	psd->pos = 0;
	psd->used = false;
	return psd;
}

static struct sd* sd_clone(struct sd* oth) {
	struct sd* psd = malloc(sizeof(struct sd));
	psd->ch = oth->ch;
	psd->cnt = oth->cnt;
	psd->pos = oth->pos;
	psd->used = oth->used;
	return psd;
}

static int sd_cmp(const void *p1, const void *p2) {
	struct sd* psd1 = * (struct sd * const *) p1;
	struct sd* psd2 = * (struct sd * const *) p2;
	int res = psd2->cnt - psd1->cnt;
	if (res == 0)
		res = sd_pos_hr(psd1) - sd_pos_hr(psd2);
	return (res == 0) ? psd1->ch - psd2->ch : res;
}

static PLIST get_stats(char* s) {
	int j, i, len = (int)strlen(s);
	PLIST pl = malloc(sizeof(LIST));
	lst_init(pl, 0);
	for (i = 0; i < len; i++) {
		if (isalpha(s[i]) && islower(s[i])) {
			bool f = false;
			for (j = 0; j < plst_cnt(pl); j++)
				if (psd(plst_item(pl, j))->ch == s[i]) {
					psd(plst_item(pl, j))->cnt++;
					f = true;
					break;
				}
			if (!f)
				lst_add(pl, sd_new(s[i], 1));
		}
	}
	i = 0;
	while (i < plst_cnt(pl)) {
		if (psd(plst_item(pl, i))->cnt == 1)
			lst_rm_at(pl, i);
		else
			i++;
	}
	return pl;
}

char* mix(char* s1, char* s2) {
	PLIST pl1 = get_stats(s1);
	PLIST pl2 = get_stats(s2);
	LIST res;
	int i, j;
	STRING buff;
	char* out = NULL;

	lst_init(&res, 0);
	for (i = 0; i < plst_cnt(pl1); i++) {
		struct sd* psd1 = psd(plst_item(pl1, i));
		struct sd* psd2 = NULL;
		for (j = 0; j < plst_cnt(pl2); j++)
			if (psd(plst_item(pl2, j))->ch == psd1->ch) {
				psd2 = psd(plst_item(pl2, j));
				break;
			}
		if (!psd2) {
			psd1->pos = 1;
			psd1->used = true;
			lst_add(&res, sd_clone(psd1));
			//printf("added#1: %c: '%c'/%d\n", sd_pos_hr(psd1), psd1->ch, psd1->cnt);
		} else if (psd1->cnt > psd2->cnt) {
			psd1->pos = 1;
			psd1->used = true;
			lst_add(&res, sd_clone(psd1));
			//printf("added#2: %c: '%c'/%d\n", sd_pos_hr(psd1), psd1->ch, psd1->cnt);
			psd2->pos = 1;
			psd2->used = true;
		} else if (psd1->cnt < psd2->cnt) {
			psd2->pos = 2;
			psd2->used = true;
			lst_add(&res, sd_clone(psd2));
			//printf("added#3: %c: '%c'/%d\n", sd_pos_hr(psd2), psd2->ch, psd2->cnt);
		} else {
			psd2->pos = 0;
			psd2->used = true;
			lst_add(&res, sd_clone(psd2));
			//printf("added#4: %c: '%c'/%d\n", sd_pos_hr(psd2), psd2->ch, psd2->cnt);
		}
	}
	for (i = 0; i < plst_cnt(pl2); i++) {
		struct sd* psd2 = psd(plst_item(pl2, i));
		if (!psd2->used) {
			psd2->pos = 2;
			psd2->used = true;
			lst_add(&res, sd_clone(psd2));
			//printf("added_x: %c: '%c'/%d\n", sd_pos_hr(psd2), psd2->ch, psd2->cnt);
		}
	}
	qsort((void*)res.ptr, res.len, sizeof(void*), sd_cmp);
	str_init(&buff, 0);
	for (i = 0; i < slst_cnt(res); i++) {
		if (i > 0)
			str_append_char(&buff, '/');
		str_append_char(&buff, sd_pos_hr(psd(slst_item(res, i))));
		str_append_char(&buff, ':');
		str_append_char_cnt(&buff, psd(slst_item(res, i))->ch, psd(slst_item(res, i))->cnt);
	}
	out = sstr(buff);
	lst_free(pl1, true);
	lst_free(pl2, true);
	lst_free(&res, true);
	free(pl1);
	free(pl2);
	return out;
}

int main() {
	char* r;

	r = mix("Are they here", "yes, they are here");
	printf("[%s]\n", r);
	if (r) free(r);

	r = mix("looping is fun but dangerous", "less dangerous than coding");
	printf("[%s]\n", r);
	if (r) free(r);

	r = mix(" In many languages", " there's a pair of functions");
	printf("[%s]\n", r);
	if (r) free(r);

	r = mix("Lords of the Fallen", "gamekult");
	printf("[%s]\n", r);
	if (r) free(r);

	r = mix("codewars", "codewars");
	printf("[%s]\n", r);
	if (r) free(r);

	r = mix("A generation must confront the looming ", "codewarrs");
	printf("[%s]\n", r);
	if (r) free(r);
}
