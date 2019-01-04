#include <time.h>
#include "vlib.h"

struct fract {
	long long num;
	long long denom;
};

struct fract* create_fract(long long num, long long denom) {
	struct fract* pfract = malloc(sizeof(struct fract));
	pfract->num = num;
	pfract->denom = denom;
	return pfract;
}

int main() {
	int i;
	STRING s;
	LV v;
	LIST l;

	srand((unsigned)time(NULL));
	lv_init(&v, 0);
	for (i = 0; i < 5; i++)
		lv_add(&v, rand() * rand());

	str_init(&s, 16);
	str_append(&s, "qwe123//");
	str_append(&s, "[asdf]");
	str_append_format(&s, "{%lld, %llu}", 123456789000000000LL, 9876543210000000000UL);
	str_append(&s, "(");
	for (i = 0; i < v.len; i++)
		if (i == 0)
			str_append_format(&s, "%d", v.vect[i]);
		else
			str_append_format(&s, ", %d", v.vect[i]);
	str_append(&s, ")");
	printf("%d:%s\n", s.cap, s.str);

	lst_init(&l, 0);
	lst_add(&l, create_fract(123, 456));
	lst_add(&l, create_fract(456, 789));
	lst_add(&l, create_fract(321, 654));
	str_clear(&s);
	for (i = 0; i < l.len; i++)
		str_append_format(&s, "{%lld, %lld}", ((struct fract*)lst_get(&l, i))->num, ((struct fract*)lst_get(&l, i))->denom);
	printf("%d:%s\n", s.cap, sstr(s));

	lst_free(&l, true);
	str_free(&s);
	lv_free(&v);
}
