/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct test {
	const char	*const name;
	int		size;
	int		data[8];
	int		want[8];
};

static int test(const struct test *restrict t)
{
	struct node *n;
	struct list l;
	int ret = -1;
	int i;

	list_init(&l, NULL);
	for (i = 0; i < t->size; i++)
		if (list_ins_next(&l, NULL, &t->data[i])) {
			fprintf(stderr, "%s: list_ins_next(%d) error\n",
				t->name, i);
			goto out;
		}
	if (list_size(&l) != t->size) {
		fprintf(stderr,"%s: unexpected size:\n\t- want: %d\n\t-  got: %d\n",
			t->name, t->size, list_size(&l));
		goto out;
	}
	i = 0;
	for (n = list_head(&l); n; n = list_node_next(n)) {
		const int *data = list_node_data(n);
		if (*data != t->want[i]) {
			fprintf(stderr, "%s: unexpected node value:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want[i], *data);
			goto out;
		}
		i++;
	}
	if (i != t->size) {
		fprintf(stderr, "%s: unexpected list size:\n\t- want: %d\n\t-  got: %d\n",
			t->name, t->size, i);
	}
	ret = 0;
out:
	list_destroy(&l);
	return ret;
}

int main(void)
{
	const struct test *t, tests[] = {
		{
			.name	= "single integer",
			.size	= 1,
			.data	= {1},
			.want	= {1},
		},
		{
			.name	= "sorted 2 integers",
			.size	= 2,
			.data	= {1, 2},
			.want	= {2, 1},
		},
		{
			.name	= "random 8 integers",
			.size	= 8,
			.data	= {1, 5, -1, 4, 2, 8, 9, -10},
			.want	= {-10, 9, 8, 2, 4, -1, 5, 1},
		},
		{.name = NULL },
	};
	int fail = 0;

	for (t = tests; t->name; t++)
		if (test(t))
			fail++;
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
