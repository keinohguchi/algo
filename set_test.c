/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "set.h"

static int same(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 == *i2;
}

static int test_set_insert(void)
{
	const struct test {
		const char	*const name;
		int		size;
		int		data[8];
		int		want_size;
		int		want[8];
	} *t, tests[] = {
		{
			.name		= "set_insert(): zero insert",
			.size		= 0,
			.want_size	= 0,
		},
		{
			.name		= "set_insert(): one insert",
			.size		= 1,
			.data		= {1},
			.want_size	= 1,
			.want		= {1},
		},
		{
			.name		= "set_insert(): two insert",
			.size		= 2,
			.data		= {1, 2},
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "set_insert(): four insert",
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.want_size	= 4,
			.want		= {1, 2, 4, 3},
		},
		{
			.name		= "set_insert(): eight insert",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.want_size	= 8,
			.want		= {1, 2, 4, 3, 5, 8, 7, 6},
		},
		{
			.name		= "set_insert(): two duplicate insert",
			.size		= 2,
			.data		= {1, 1},
			.want_size	= 1,
			.want		= {1},
		},
		{
			.name		= "set_insert(): three duplicate insert",
			.size		= 4,
			.data		= {1, 2, 2, 2},
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "set_insert(): four duplicate insert",
			.size		= 8,
			.data		= {1, 2, 1, 4, 1, 6, 1, 8},
			.want_size	= 5,
			.want		= {1, 2, 4, 6, 8},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct set s;
		int i, ret;

		ret = set_init(&s, same, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size; i++) {
			ret = set_insert(&s, &t->data[i]);
			if (ret == -1)
				goto perr;
		}
		if (set_size(&s) != t->want_size) {
			fprintf(stderr, "%s: unexpected set size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_size, set_size(&s));
			goto err;
		}
		for (i = 0; i < t->want_size; i++) {
			int *datap = (int *)&t->want[i];
			ret = set_remove(&s, (void **)&datap);
			if (ret == -1)
				goto perr;
		}
		if (set_size(&s)) {
			fprintf(stderr, "%s: unexpected final set size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, set_size(&s));
			goto err;
		}
		set_destroy(&s);
		continue;
perr:
		perror(t->name);
err:
		fail++;
	}
	return fail;
}

static int test_set_union(void)
{
	const struct test {
		const char	*const name;
		int		size1;
		int		size2;
		int		size_want;
		int		data1[8];
		int		data2[8];
		int		want[16];
	} *t, tests[] = {
		{
			.name		= "set_union(): zero sets union",
			.size1		= 0,
			.size2		= 0,
			.size_want	= 0,
		},
		{
			.name		= "set_union(): 1 set each union",
			.size1		= 1,
			.data1		= {1},
			.size2		= 1,
			.data2		= {2},
			.size_want	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "set_union(): 2 set each union",
			.size1		= 2,
			.data1		= {1, 2},
			.size2		= 2,
			.data2		= {3, 4},
			.size_want	= 4,
			.want		= {1, 2, 3, 4},
		},
		{
			.name		= "set_union(): 4 set each union",
			.size1		= 4,
			.data1		= {1, 2, 3, 4},
			.size2		= 4,
			.data2		= {5, 6, 7, 8},
			.size_want	= 8,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "set_union(): 8 set each union",
			.size1		= 8,
			.data1		= {1, 2, 3, 4, 5, 6, 7, 8},
			.size2		= 8,
			.data2		= {15, 16, 17, 18, 19, 20, 21, 22},
			.size_want	= 16,
			.want		= {
				8, 7, 6, 5, 4, 3, 2, 1, 22, 21, 20, 19, 18, 17,
				16, 15,
			},
		},
		{
			.name		= "set_union(): 1 set duplicate union",
			.size1		= 1,
			.data1		= {1},
			.size2		= 1,
			.data2		= {1},
			.size_want	= 1,
			.want		= {1},
		},
		{
			.name		= "set_union(): 2 set duplicate union",
			.size1		= 2,
			.data1		= {1, 2},
			.size2		= 2,
			.data2		= {2, 1},
			.size_want	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "set_union(): 4 set duplicate union",
			.size1		= 4,
			.data1		= {1, 2, 3, 4},
			.size2		= 4,
			.data2		= {3, 4, 5, 6},
			.size_want	= 6,
			.want		= {6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "set_union(): 8 set duplicate union",
			.size1		= 8,
			.data1		= {1, 2, 3, 4, 5, 6, 7, 8},
			.size2		= 8,
			.data2		= {5, 6, 7, 8, 9, 10, 11, 12},
			.size_want	= 12,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct set s1, s2, su;
		int i, ret;

		ret = set_init(&s1, same, NULL);
		if (ret == -1)
			goto perr;
		ret = set_init(&s2, same, NULL);
		if (ret == -1)
			goto perr;
		ret = set_init(&su, same, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size1; i++) {
			ret = set_insert(&s1, &t->data1[i]);
			if (ret == -1)
				goto perr;
		}
		if (set_size(&s1) != t->size1) {
			fprintf(stderr, "%s: unexpected set1 size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->size1, set_size(&s1));
			goto err;
		}
		for (i = 0; i < t->size2; i++) {
			ret = set_insert(&s2, &t->data2[i]);
			if (ret == -1)
				goto perr;
		}
		if (set_size(&s2) != t->size2) {
			fprintf(stderr, "%s: unexpected set2 size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->size2, set_size(&s2));
			goto err;
		}
		ret = set_union(&su, &s1, &s2);
		if (ret == -1)
			goto perr;
		if (set_size(&su) != t->size_want) {
			fprintf(stderr, "%s: unexpected union set size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->size_want, set_size(&su));
			goto err;
		}
		for (i = 0; i < t->size_want; i++) {
			int *gotp = (int *)&t->want[i];
			ret = set_remove(&su, (void **)&gotp);
			if (ret == -1)
				goto perr;
		}
		if (set_size(&su)) {
			fprintf(stderr, "%s: unexpected final union size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, set_size(&su));
			goto err;
		}
		set_destroy(&su);
		set_destroy(&s2);
		set_destroy(&s1);
		continue;
perr:
		perror(t->name);
err:
		set_destroy(&su);
		set_destroy(&s2);
		set_destroy(&s1);
		fail++;
	}
	return fail;
}

int main(void)
{
	int fail = 0;

	fail += test_set_insert();
	fail += test_set_union();
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
