#include "field.h"
#include "list.h"

LIST_HEAD(fields);

struct field* get_field(int m) {
    struct field* f;
    list_for_each_entry(f, fields, list) {
        if (f->m == m)
            return f;
    }
    return NULL;
}

int register_field(struct field* f) {
	// if (f->m > 256) {
	// 	return -1;}

	//fields[f-> m] = f;
	list_add(f->list, fields)

	return 0;
}

int sum(int m, int a, int b) {
	struct field* f;

	f = get_field(m);
	if (!f) {
		f = get_field(0);
		f->m = m;
	}

	return f->sum(a, b);
}