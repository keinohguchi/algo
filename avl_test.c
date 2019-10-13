/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include "queue.h"

static int cmp(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 - *i2;
}

static int preorder(const struct avl_node *node, struct queue *q)
{
	if (enqueue(q, node->data) == -1)
		return -1;
	if (preorder(node->left, q) == -1)
		return -1;
	if (preorder(node->right, q) == -1)
		return -1;
	return 0;
}

int main(void)
{
	const struct test {
		const char	*const name;
		int		size;
		int		data[8];
		int		(*traversal)(const struct avl_node *node,
					     struct queue *q);
		int		want_size;
		int		want[8];
	} *t, tests[] = {
		{
			.name		= "sorted eight value with preorder traversal",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.traversal	= preorder,
			.want_size	= 8,
			.want		= {4, 2, 1, 3, 7, 6, 5, 8},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct queue want;
		struct avl tree;
		int ret;

		ret = avl_init(&tree, cmp, NULL);
		if (ret == -1)
			goto perr;
		ret = queue_init(&want, NULL);
		if (ret == -1)
			goto perr;
		avl_destroy(&tree);
		if (avl_size(&tree)) {
			fprintf(stderr, "%s: unexpected final tree size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, avl_size(&tree));
			goto err;
		}
		continue;
perr:
		perror(t->name);
err:
		fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
