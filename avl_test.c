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
	if (!node)
		return 0;
	if (enqueue(q, node->data) == -1)
		return -1;
	if (preorder(node->left, q) == -1)
		return -1;
	if (preorder(node->right, q) == -1)
		return -1;
	return 0;
}

static int inorder(const struct avl_node *node, struct queue *q)
{
	if (!node)
		return 0;
	if (inorder(node->left, q) == -1)
		return -1;
	if (enqueue(q, node->data) == -1)
		return -1;
	if (inorder(node->right, q) == -1)
		return -1;
	return 0;
}

static int postorder(const struct avl_node *node, struct queue *q)
{
	if (!node)
		return 0;
	if (postorder(node->left, q) == -1)
		return -1;
	if (postorder(node->right, q) == -1)
		return -1;
	if (enqueue(q, node->data) == -1)
		return -1;
	return 0;
}

int main(void)
{
	const struct test {
		const char	*const name;
		int		size;
		int		data[8];
		int		(*traverse)(const struct avl_node *node,
					    struct queue *q);
		int		want_size;
		int		want[8];
	} *t, tests[] = {
		{
			.name		= "ascending 2 values with preorder traversal",
			.size		= 2,
			.data		= {1, 2},
			.traverse	= preorder,
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "ascending 3 values with preorder traversal",
			.size		= 3,
			.data		= {1, 2, 3},
			.traverse	= preorder,
			.want_size	= 3,
			.want		= {2, 1, 3},
		},
		{
			.name		= "ascending 4 values with preorder traversal",
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.traverse	= preorder,
			.want_size	= 4,
			.want		= {2, 1, 3, 4},
		},
		{
			.name		= "ascending 5 values with preorder traversal",
			.size		= 5,
			.data		= {1, 2, 3, 4, 5},
			.traverse	= preorder,
			.want_size	= 5,
			.want		= {2, 1, 4, 3, 5},
		},
		{
			.name		= "ascending 6 values with preorder traversal",
			.size		= 6,
			.data		= {1, 2, 3, 4, 5, 6},
			.traverse	= preorder,
			.want_size	= 6,
			.want		= {4, 2, 1, 3, 5, 6},
		},
		{
			.name		= "ascending 7 values with preorder traversal",
			.size		= 7,
			.data		= {1, 2, 3, 4, 5, 6, 7},
			.traverse	= preorder,
			.want_size	= 7,
			.want		= {4, 2, 1, 3, 6, 5, 7},
		},
		{
			.name		= "ascending 8 values with preorder traversal",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.traverse	= preorder,
			.want_size	= 8,
			.want		= {4, 2, 1, 3, 6, 5, 7, 8},
		},
		{
			.name		= "ascending 2 values with inorder traversal",
			.size		= 2,
			.data		= {1, 2},
			.traverse	= inorder,
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "ascending 3 values with inorder traversal",
			.size		= 3,
			.data		= {1, 2, 3},
			.traverse	= inorder,
			.want_size	= 3,
			.want		= {1, 2, 3},
		},
		{
			.name		= "ascending 4 values with inorder traversal",
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.traverse	= inorder,
			.want_size	= 4,
			.want		= {1, 2, 3, 4},
		},
		{
			.name		= "ascending 5 values with inorder traversal",
			.size		= 5,
			.data		= {1, 2, 3, 4, 5},
			.traverse	= inorder,
			.want_size	= 5,
			.want		= {1, 2, 3, 4, 5},
		},
		{
			.name		= "ascending 6 values with inorder traversal",
			.size		= 6,
			.data		= {1, 2, 3, 4, 5, 6},
			.traverse	= inorder,
			.want_size	= 6,
			.want		= {1, 2, 3, 4, 5, 6},
		},
		{
			.name		= "ascending 7 values with inorder traversal",
			.size		= 7,
			.data		= {1, 2, 3, 4, 5, 6, 7},
			.traverse	= inorder,
			.want_size	= 7,
			.want		= {1, 2, 3, 4, 5, 6, 7},
		},
		{
			.name		= "ascending 8 values with inorder traversal",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "ascending 2 values with postorder traversal",
			.size		= 2,
			.data		= {1, 2},
			.traverse	= postorder,
			.want_size	= 2,
			.want		= {2, 1},
		},
		{
			.name		= "ascending 3 values with postorder traversal",
			.size		= 3,
			.data		= {1, 2, 3},
			.traverse	= postorder,
			.want_size	= 3,
			.want		= {1, 3, 2},
		},
		{
			.name		= "ascending 4 values with postorder traversal",
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.traverse	= postorder,
			.want_size	= 4,
			.want		= {1, 4, 3, 2},
		},
		{
			.name		= "ascending 5 values with postorder traversal",
			.size		= 5,
			.data		= {1, 2, 3, 4, 5},
			.traverse	= postorder,
			.want_size	= 5,
			.want		= {1, 3, 5, 4, 2},
		},
		{
			.name		= "ascending 6 values with postorder traversal",
			.size		= 6,
			.data		= {1, 2, 3, 4, 5, 6},
			.traverse	= postorder,
			.want_size	= 6,
			.want		= {1, 3, 2, 6, 5, 4},
		},
		{
			.name		= "ascending 7 values with postorder traversal",
			.size		= 7,
			.data		= {1, 2, 3, 4, 5, 6, 7},
			.traverse	= postorder,
			.want_size	= 7,
			.want		= {1, 3, 2, 5, 7, 6, 4},
		},
		{
			.name		= "ascending 8 values with postorder traversal",
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.traverse	= postorder,
			.want_size	= 8,
			.want		= {1, 3, 2, 5, 8, 7, 6, 4},
		},
		{
			.name		= "decending 2 values with preorder traversal",
			.size		= 2,
			.data		= {2, 1},
			.traverse	= preorder,
			.want_size	= 2,
			.want		= {2, 1},
		},
		{
			.name		= "decending 3 values with preorder traversal",
			.size		= 3,
			.data		= {3, 2, 1},
			.traverse	= preorder,
			.want_size	= 3,
			.want		= {2, 1, 3},
		},
		{
			.name		= "decending 4 values with preorder traversal",
			.size		= 4,
			.data		= {4, 3, 2, 1},
			.traverse	= preorder,
			.want_size	= 4,
			.want		= {3, 2, 1, 4},
		},
		{
			.name		= "decending 5 values with preorder traversal",
			.size		= 5,
			.data		= {5, 4, 3, 2, 1},
			.traverse	= preorder,
			.want_size	= 5,
			.want		= {4, 2, 1, 3, 5},
		},
		{
			.name		= "decending 6 values with preorder traversal",
			.size		= 6,
			.data		= {6, 5, 4, 3, 2, 1},
			.traverse	= preorder,
			.want_size	= 6,
			.want		= {3, 2, 1, 5, 4, 6},
		},
		{
			.name		= "decending 7 values with preorder traversal",
			.size		= 7,
			.data		= {7, 6, 5, 4, 3, 2, 1},
			.traverse	= preorder,
			.want_size	= 7,
			.want		= {4, 2, 1, 3, 6, 5, 7},
		},
		{
			.name		= "decending 8 values with preorder traversal",
			.size		= 8,
			.data		= {8, 7, 6, 5, 4, 3, 2, 1},
			.traverse	= preorder,
			.want_size	= 8,
			.want		= {5, 3, 2, 1, 4, 7, 6, 8},
		},
		{
			.name		= "decending 2 values with inorder traversal",
			.size		= 2,
			.data		= {2, 1},
			.traverse	= inorder,
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "decending 3 values with inorder traversal",
			.size		= 3,
			.data		= {3, 2, 1},
			.traverse	= inorder,
			.want_size	= 3,
			.want		= {1, 2, 3},
		},
		{
			.name		= "decending 4 values with inorder traversal",
			.size		= 4,
			.data		= {4, 3, 2, 1},
			.traverse	= inorder,
			.want_size	= 4,
			.want		= {1, 2, 3, 4},
		},
		{
			.name		= "decending 5 values with inorder traversal",
			.size		= 5,
			.data		= {5, 4, 3, 2, 1},
			.traverse	= inorder,
			.want_size	= 5,
			.want		= {1, 2, 3, 4, 5},
		},
		{
			.name		= "decending 6 values with inorder traversal",
			.size		= 6,
			.data		= {6, 5, 4, 3, 2, 1},
			.traverse	= inorder,
			.want_size	= 6,
			.want		= {1, 2, 3, 4, 5, 6},
		},
		{
			.name		= "decending 7 values with inorder traversal",
			.size		= 7,
			.data		= {7, 6, 5, 4, 3, 2, 1},
			.traverse	= inorder,
			.want_size	= 7,
			.want		= {1, 2, 3, 4, 5, 6, 7},
		},
		{
			.name		= "decending 8 values with inorder traversal",
			.size		= 8,
			.data		= {8, 7, 6, 5, 4, 3, 2, 1},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "decending 2 values with postorder traversal",
			.size		= 2,
			.data		= {2, 1},
			.traverse	= postorder,
			.want_size	= 2,
			.want		= {1, 2},
		},
		{
			.name		= "decending 3 values with postorder traversal",
			.size		= 3,
			.data		= {3, 2, 1},
			.traverse	= postorder,
			.want_size	= 3,
			.want		= {1, 3, 2},
		},
		{
			.name		= "decending 4 values with postorder traversal",
			.size		= 4,
			.data		= {4, 3, 2, 1},
			.traverse	= postorder,
			.want_size	= 4,
			.want		= {1, 2, 4, 3},
		},
		{
			.name		= "decending 5 values with postorder traversal",
			.size		= 5,
			.data		= {5, 4, 3, 2, 1},
			.traverse	= postorder,
			.want_size	= 5,
			.want		= {1, 3, 2, 5, 4},
		},
		{
			.name		= "decending 6 values with postorder traversal",
			.size		= 6,
			.data		= {6, 5, 4, 3, 2, 1},
			.traverse	= postorder,
			.want_size	= 6,
			.want		= {1, 2, 4, 6, 5, 3},
		},
		{
			.name		= "decending 7 values with postorder traversal",
			.size		= 7,
			.data		= {7, 6, 5, 4, 3, 2, 1},
			.traverse	= postorder,
			.want_size	= 7,
			.want		= {1, 3, 2, 5, 7, 6, 4},
		},
		{
			.name		= "decending 8 values with postorder traversal",
			.size		= 8,
			.data		= {8, 7, 6, 5, 4, 3, 2, 1},
			.traverse	= postorder,
			.want_size	= 8,
			.want		= {1, 2, 4, 3, 6, 8, 7, 5},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct queue want;
		struct avl tree;
		int i, ret;

		ret = avl_init(&tree, cmp, NULL);
		if (ret == -1)
			goto perr;
		ret = queue_init(&want, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size; i++) {
			ret = avl_insert(&tree, &t->data[i]);
			if (ret == -1)
				goto perr;
		}
		if (avl_size(&tree) != t->want_size) {
			fprintf(stderr, "%s: unexpected tree size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_size, avl_size(&tree));
			goto err;
		}
		ret = t->traverse(tree.root, &want);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->want_size; i++) {
			int *got;
			ret = dequeue(&want, (void **)&got);
			if (ret == -1)
				goto perr;
			if (*got != t->want[i]) {
				fprintf(stderr, "%s: unexpected value[%d]:\n\t- want: %d\n\t-  got: %d\n",
					t->name, i, t->want[i], *got);
				fail++;
			}
		}
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
