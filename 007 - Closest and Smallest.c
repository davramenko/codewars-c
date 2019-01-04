#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

// Closest and Smallest

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

struct cns {
	int w, i, v;
};

#define pcns(p) ((struct cns*)p)

struct cns* cns_new(int w, int i, int v) {
	struct cns* pcns = malloc(sizeof(struct cns));
	pcns->w = w;
	pcns->i = i;
	pcns->v = v;
	return pcns;
}

int get_w_diff(PLIST plsrc, int i, int j) {
	return abs(pcns(plst_item(plsrc, i))->w - pcns(plst_item(plsrc, j))->w);
}

struct pair {
	int w1, i1, v1;
	int w2, i2, v2;
	int diff;
};

#define ppair(p) ((struct pair*)p)

struct pair* pair_new(struct cns* s1, struct cns* s2) {
	if (s1 == NULL || s2 == NULL || s1->i == s2->i)
		return NULL;
	struct pair* ppair = malloc(sizeof(struct pair));
	if (s1->i < s2->i) {
		ppair->w1 = s1->w;
		ppair->i1 = s1->i;
		ppair->v1 = s1->v;
		ppair->w2 = s2->w;
		ppair->i2 = s2->i;
		ppair->v2 = s2->v;
	} else {
		ppair->w1 = s2->w;
		ppair->i1 = s2->i;
		ppair->v1 = s2->v;
		ppair->w2 = s1->w;
		ppair->i2 = s1->i;
		ppair->v2 = s1->v;
	}
	ppair->diff = abs(s1->w - s2->w);
	return ppair;
}

struct pair* mk_pair(PLIST plsrc, int i, int j) {
	return pair_new(pcns(plst_item(plsrc, i)), pcns(plst_item(plsrc, j)));
}

bool pairs_equal(struct pair* p1, struct pair* p2) {
	return (p1->i1 == p2->i1 && p1->i2 == p2->i2);
}

char* closest(char* strng) {
	int i, j, n, len, min_w_diff;
	char* def = "{{0,0,0},{0,0,0}}";
	char* res = NULL;
	char* ptr;
	STRING s;
	LIST lsrc, lp;

	len = (int)strlen(strng);
	if (len == 0) {
		len = (int)strlen(def);
		res = malloc(len + 1);
		strncpy(res, def, len);
		res[len] = 0;
		return res;
	}
	n = 1;
	for (i = 0; i < len; i++)
		if (isspace(strng[i]))
			n++;
	lst_init(&lsrc, 0);
	for (ptr = strng, i = 0; i < n; i++) {
		char* endptr;
		int v = (int)strtol(ptr, &endptr, 10);
		int w = 0;
		while (isdigit(*ptr))
			w += (*ptr++) - '0';
		lst_add(&lsrc, cns_new(w, i, v));
		if (!*endptr) break;
		ptr = endptr;
		ptr++;
	}
	//{
	//	STRING s;
	//	str_init(&s, 0);
	//	str_append(&s, "src: ");
	//	for (i = 0; i < slst_cnt(lsrc); i++)
	//		str_append_format(&s, "{%d, %d, %d}", pcns(slst_item(lsrc, i))->w, pcns(slst_item(lsrc, i))->i, pcns(slst_item(lsrc, i))->v);
	//	printf("%s\n", sstr(s));
	//	str_free(&s);
	//}
	min_w_diff = get_w_diff(&lsrc, 0, 1);
	lst_init(&lp, 0);
	lst_add(&lp, mk_pair(&lsrc, 0, 1));
	//printf("init: min_w_diff=%d\n", min_w_diff);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			int w_diff = get_w_diff(&lsrc, i, j);
			//printf("%d, %d: w_diff=%d\n", i, j, w_diff);

			if (i == j) continue;
			if (w_diff < abs(min_w_diff)) {
				min_w_diff = w_diff;
				lst_free(&lp, true);
				lst_init(&lp, 0);
				lst_add(&lp, mk_pair(&lsrc, i, j));
			} else if (w_diff == abs(min_w_diff)) {
				int k;
				bool f = false;
				struct pair *p = mk_pair(&lsrc, i, j);
				for (k = 0; k < slst_cnt(lp); k++) {
					if (pairs_equal(ppair(slst_item(lp, k)), p)) {
						f = true;
						break;
					}
				}
				if (!f)
					lst_add(&lp, p);
			}
		}
	}
	//{
	//	STRING s;
	//	str_init(&s, 0);
	//	str_append(&s, "selected: ");
	//	for (i = 0; i < slst_cnt(lp); i++)
	//		str_append_format(&s, "({%d, %d, %d}/%d/{%d, %d, %d})", ppair(slst_item(lp, i))->w1, ppair(slst_item(lp, i))->i1, ppair(slst_item(lp, i))->v1, ppair(slst_item(lp, i))->diff, ppair(slst_item(lp, i))->w2, ppair(slst_item(lp, i))->i2, ppair(slst_item(lp, i))->v2);
	//	printf("%s\n", sstr(s));
	//	str_free(&s);
	//}
	if (slst_cnt(lp) > 1) {
		int smalest_w = ppair(slst_item(lp, 0))->w1 < ppair(slst_item(lp, 0))->w2 ? ppair(slst_item(lp, 0))->w1 : ppair(slst_item(lp, 0))->w2;
		int smalest_w2 = ppair(slst_item(lp, 0))->w1 < ppair(slst_item(lp, 0))->w2 ? ppair(slst_item(lp, 0))->w2 : ppair(slst_item(lp, 0))->w1;
		for (i = 1; i < slst_cnt(lp); i++) {
			int w = ppair(slst_item(lp, i))->w1 < ppair(slst_item(lp, i))->w2 ? ppair(slst_item(lp, i))->w1 : ppair(slst_item(lp, i))->w2;
			int w2 = ppair(slst_item(lp, i))->w1 < ppair(slst_item(lp, i))->w2 ? ppair(slst_item(lp, i))->w2 : ppair(slst_item(lp, i))->w1;
			if (w < smalest_w)
				smalest_w = w;
			if (w2 < smalest_w2)
				smalest_w2 = w2;
		}
		i = 0;
		while (slst_cnt(lp) > 1 && i < slst_cnt(lp)) {
			int w = ppair(slst_item(lp, i))->w1 < ppair(slst_item(lp, i))->w2 ? ppair(slst_item(lp, i))->w1 : ppair(slst_item(lp, i))->w2;
			int w2 = ppair(slst_item(lp, i))->w1 < ppair(slst_item(lp, i))->w2 ? ppair(slst_item(lp, i))->w2 : ppair(slst_item(lp, i))->w1;
			if (w > smalest_w) {
				lst_rm_at(&lp, i);
			} else if (w2 > smalest_w2) {
				lst_rm_at(&lp, i);
			} else {
				i++;
			}
		}
		if (slst_cnt(lp) > 1) {
			int smalest_i = ppair(slst_item(lp, 0))->i1 < ppair(slst_item(lp, 0))->i2 ? ppair(slst_item(lp, 0))->i1 : ppair(slst_item(lp, 0))->i2;
			int smalest_i2 = ppair(slst_item(lp, 0))->i1 < ppair(slst_item(lp, 0))->i2 ? ppair(slst_item(lp, 0))->i2 : ppair(slst_item(lp, 0))->i1;
			for (i = 1; i < slst_cnt(lp); i++) {
				int i1 = ppair(slst_item(lp, i))->i1 < ppair(slst_item(lp, i))->i2 ? ppair(slst_item(lp, i))->i1 : ppair(slst_item(lp, i))->i2;
				int i2 = ppair(slst_item(lp, i))->i1 < ppair(slst_item(lp, i))->i2 ? ppair(slst_item(lp, i))->i2 : ppair(slst_item(lp, i))->i1;
				if (i1 < smalest_i)
					smalest_i = i;
				if (i2 < smalest_i2)
					smalest_i2 = i2;
			}
			i = 0;
			while (slst_cnt(lp) > 1 && i < slst_cnt(lp)) {
				int i1 = ppair(slst_item(lp, i))->i1 < ppair(slst_item(lp, i))->i2 ? ppair(slst_item(lp, i))->i1 : ppair(slst_item(lp, i))->i2;
				int i2 = ppair(slst_item(lp, i))->i1 < ppair(slst_item(lp, i))->i2 ? ppair(slst_item(lp, i))->i2 : ppair(slst_item(lp, i))->i1;
				if (i1 > smalest_i) {
					lst_rm_at(&lp, i);
				} else if (i2 > smalest_i2) {
					lst_rm_at(&lp, i);
				} else {
					i++;
				}
			}
		}
	}
	if (slst_cnt(lp) == 1) {
		struct pair* pp = slst_item(lp, 0);
		str_init(&s, 0);
		if (pp->w1 < pp->w2 || (pp->w1 == pp->w2 && pp->i1 < pp->i2)) {
			str_append_format(&s, "{{%d, %d, %d}, {%d, %d, %d}}", pp->w1, pp->i1, pp->v1, pp->w2, pp->i2, pp->v2);
		} else {
			str_append_format(&s, "{{%d, %d, %d}, {%d, %d, %d}}", pp->w2, pp->i2, pp->v2, pp->w1, pp->i1, pp->v1);
		}
		res = sstr(s);
	}
	lst_free(&lp, true);
	lst_free(&lsrc, true);
	return res;
}

int main() {
	char *r;

	r = closest("");
	printf("%s\n", r);
	free(r);
	r = closest("456899 50 11992 176 272293 163 389128 96 290193 85 52");
	printf("%s\n", r);
	free(r);
	r = closest("239382 162 254765 182 485944 134 468751 62 49780 108 54");
	printf("%s\n", r);
	free(r);
	r = closest("241259 154 155206 194 180502 147 300751 200 406683 37 57");
	printf("%s\n", r);
	free(r);
	r = closest("89998 187 126159 175 338292 89 39962 145 394230 167 1");
	printf("%s\n", r);
	free(r);
	r = closest("462835 148 467467 128 183193 139 220167 116 263183 41 52");
	printf("%s\n", r);
	free(r);

	r = closest("403749 18 278325 97 304194 119 58359 165 144403 128 38");
	printf("%s\n", r);
	free(r);
	r = closest("28706 196 419018 130 49183 124 421208 174 404307 60 24");
	printf("%s\n", r);
	free(r);
	r = closest("189437 110 263080 175 55764 13 257647 53 486111 27 66");
	printf("%s\n", r);
	free(r);
	r = closest("79257 160 44641 146 386224 147 313622 117 259947 155 58");
	printf("%s\n", r);
	free(r);
	r = closest("315411 165 53195 87 318638 107 416122 121 375312 193 59");
	printf("%s\n", r);
	free(r);
}
