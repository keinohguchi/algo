/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "set.h"

struct test {
	const char	*const name;
	int		size;
	int		data[8];
	int		want_size;
	int		want[8];
};

static int same(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 == *i2;
}

static int test_set(const struct test *const t)
{
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
	return 0;
perr:
	perror(t->name);
err:
	return -1;
}

static int test_set_insert(void)
{
	const struct test *t, tests[] = {
		{
			.name		= "zero insert",
			.size		= 0,
			.want_size	= 0,
		},
		{
			.name		= "one insert",
			.size		= 1,
			.data		= {1},
			.want_size	= 1,
			.want		= {1},
		},
		{
			.name		= "two insert",
			.size		= 2,
			.data		= {1, 2},
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "four insert",
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.want_size	= 4,
			.want		= {1, 2, 4, 3},
		},
		{
			.name		= "eight insert",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.want_size	= 8,
			.want		= {1, 2, 4, 3, 5, 8, 7, 6},
		},
		{
			.name		= "two duplicate insert",
			.size		= 2,
			.data		= {1, 1},
			.want_size	= 1,
			.want		= {1},
		},
		{
			.name		= "three duplicate insert",
			.size		= 4,
			.data		= {1, 2, 2, 2},
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "four duplicate insert",
			.size		= 8,
			.data		= {1, 2, 1, 4, 1, 6, 1, 8},
			.want_size	= 5,
			.want		= {1, 2, 4, 6, 8},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		if (test_set(t))
			fail++;
	}
	return fail;
}

int main(void)
{
	int fail = 0;

	fail += test_set_insert();
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
