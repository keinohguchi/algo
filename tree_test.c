/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

static int compare(const void *d1, const void *d2)
{
	return d1 - d2;
}

static int test_tree()
{
	const struct test {
		const char	*const name;
	} *t, tests[] = {
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct tree tree;
		int ret;

		ret = tree_init(&tree, compare, NULL);
		if (ret == -1)
			goto perr;
		tree_destroy(&tree);
		continue;
perr:
		perror(t->name);
		fail++;
	}
	return fail;
}

int main(void)
{
	int fail = 0;
	fail += test_tree();
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
