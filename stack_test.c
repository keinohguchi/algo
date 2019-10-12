/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct test {
	const char	*const name;
	int		size;
	int		data[8];
	int		want[8];
};

static int test_stack(const struct test *const t)
{
	struct stack s;
	int i, ret;

	ret = stack_init(&s, NULL);
	if (ret == -1)
		goto perr;
	for (i = 0; i < t->size; i++) {
		ret = push(&s, &t->data[i]);
		if (ret == -1)
			goto perr;
	}
	if (stack_size(&s) != t->size) {
		fprintf(stderr, "%s: unexpected stack size:\n\t- want: %d\n\t-  got: %d\n",
			t->name, t->size, stack_size(&s));
		goto err;
	}
	for (i = 0; i < t->size; i++) {
		int *got;
		ret = pop(&s, (void **)&got);
		if (ret == -1)
			goto perr;
		if (*got != t->want[i]) {
			fprintf(stderr, "%s: unexpected %dth value:\n\t- want: %d\n\t-  got: %d\n",
				t->name, i+1, t->want[i], *got);
			goto err;
		}
	}
	if (stack_size(&s)) {
		fprintf(stderr, "%s: unexpected final stack size:\n\t- want: 0\n\t-  got: %d\n",
			t->name, stack_size(&s));
		goto err;
	}
	stack_destroy(&s);
	return 0;
perr:
	perror(t->name);
err:
	return ret;
}

int main(void)
{
	const struct test *t, tests[] = {
		{
			.name	= "no data push",
			.size	= 0,
		},
		{
			.name	= "single push",
			.size	= 1,
			.data	= {1},
			.want	= {1},
		},
		{
			.name	= "four integer push",
			.size	= 4,
			.data	= {1, 2, 3, 4},
			.want	= {4, 3, 2, 1},
		},
		{
			.name	= "eight integer push",
			.size	= 8,
			.data	= {1, 2, 3, 4, 5, 6, 7, 8},
			.want	= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{.name	 = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		if (test_stack(t))
			fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
