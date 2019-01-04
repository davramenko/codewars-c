#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#pragma warning(disable : 4996)

struct int_vect {
	int cap;
	int len;
	int* vect;
};

typedef struct int_vect IV;
typedef struct int_vect* PIV;

struct long_vect {
	int cap;
	int len;
	long long* vect;
};

typedef struct long_vect LV;
typedef struct long_vect* PLV;

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

void lv_init(PLV v, int init_cap) {
	if (v == NULL)
		return;
	v->len = 0;
	v->cap = init_cap;
	if (v->cap <= 0)
		v->cap = 16;
	v->vect = calloc(v->cap, sizeof(long long));
}

void lv_free(PLV v) {
	if (v == NULL || v->vect == NULL)
		return;
	free(v->vect);
	v->vect = NULL;
}

void lv_add(PLV v, long long item) {
	if (v == NULL)
		return;
	if (v->vect == NULL)
		lv_init(v, 0);
	if (v->len >= v->cap) {
		int tcap = v->cap * 2;
		long long* buff = calloc(tcap, sizeof(long long));
		memcpy(buff, v->vect, sizeof(long long) * v->cap);
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

void* lst_get(PLIST l, int index) {
	if (l == NULL || index < 0)
		return NULL;
	if (l->ptr == NULL)
		lst_init(l, 0);
	if (index >= l->len)
		return NULL;
	return l->ptr[index];
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

void str_clear(PSTRING s) {
	if (s == NULL)
		return;
	if (s->str == NULL) {
		str_init(s, 0);
	} else {
		memset(s->str, 0, s->cap);
	}
}

size_t str_len(PSTRING s) {
	if (s == NULL)
		return 0;
	if (s->str == NULL) {
		str_init(s, 0);
	}
	return strlen(s->str);
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
