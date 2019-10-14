/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "queue.h"

static int cmp(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 - *i2;
}

static int preorder(const struct tree_node *node, struct queue *q)
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

static int inorder(const struct tree_node *node, struct queue *q)
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

static int postorder(const struct tree_node *node, struct queue *q)
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

static int test_tree()
{
	enum direction { left, right };
	struct data {
		enum direction	direction;
		int		data;
	};
	const struct test {
		const char	*const name;
		int		size;
		struct data	data[8];
		int		(*traverse)(const struct tree_node *node,
					     struct queue *q);
		int		want_size;
		int		want[8];
	} *t, tests[] = {
		{
			.name		= "eight left inserts with preorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{left, 2},{left, 3},{left, 4},
				{left, 5},{left, 6},{left, 7},{left, 8},
			},
			.traverse	= preorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "eight right inserts with preorder traversal",
			.size		= 8,
			.data		= {
				{right, 1},{right, 2},{right, 3},{right, 4},
				{right, 5},{right, 6},{right, 7},{right, 8},
			},
			.traverse	= preorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "eight left/right inserts with preorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{right, 2},{left, 3},{right, 4},
				{left, 5},{right, 6},{left, 7},{right, 8},
			},
			.traverse	= preorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "eight left inserts with inorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{left, 2},{left, 3},{left, 4},
				{left, 5},{left, 6},{left, 7},{left, 8},
			},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "eight right inserts with inorder traversal",
			.size		= 8,
			.data		= {
				{right, 1},{right, 2},{right, 3},{right, 4},
				{right, 5},{right, 6},{right, 7},{right, 8},
			},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "eight left/right inserts with inorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{right, 2},{left, 3},{right, 4},
				{left, 5},{right, 6},{left, 7},{right, 8},
			},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {1, 3, 5, 7, 8, 6, 4, 2},
		},
		{
			.name		= "eight right/left inserts with inorder traversal",
			.size		= 8,
			.data		= {
				{right, 1},{left, 2},{right, 3},{left, 4},
				{right, 5},{left, 6},{right, 7},{left, 8},
			},
			.traverse	= inorder,
			.want_size	= 8,
			.want		= {2, 4, 6, 8, 7, 5, 3, 1},
		},
		{
			.name		= "eight left inserts with postorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{left, 2},{left, 3},{left, 4},
				{left, 5},{left, 6},{left, 7},{left, 8},
			},
			.traverse	= postorder,
			.want_size	= 8,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "eight right inserts with postorder traversal",
			.size		= 8,
			.data		= {
				{right, 1},{right, 2},{right, 3},{right, 4},
				{right, 5},{right, 6},{right, 7},{right, 8},
			},
			.traverse	= postorder,
			.want_size	= 8,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "eight left/right inserts with postorder traversal",
			.size		= 8,
			.data		= {
				{left, 1},{right, 2},{left, 3},{right, 4},
				{left, 5},{right, 6},{left, 7},{right, 8},
			},
			.traverse	= postorder,
			.want_size	= 8,
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct tree_node *node;
		struct tree tree;
		struct queue want;
		int i, ret;

		ret = tree_init(&tree, cmp, NULL);
		if (ret == -1)
			goto perr;
		node = tree.root;
		for (i = 0; i < t->size; i++) {
			switch (t->data[i].direction) {
			case left:
				ret = tree_ins_left(&tree, node, &t->data[i].data);
				if (ret == -1)
					goto perr;
				node = node ? node->left : tree.root;
				break;
			case right:
				ret = tree_ins_right(&tree, node, &t->data[i].data);
				if (ret == -1)
					goto perr;
				node = node ? node->right : tree.root;
				break;
			}
		}
		if (tree_size(&tree) != t->want_size) {
			fprintf(stderr, "%s: unexpected tree size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_size, tree_size(&tree));
			goto err;
		}
		ret = queue_init(&want, NULL);
		if (ret == -1)
			goto perr;
		ret = t->traverse(tree.root, &want);
		if (ret == -1)
			goto perr;
		if (queue_size(&want) != t->want_size) {
			fprintf(stderr, "%s: unexpected result queue size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_size, queue_size(&want));
			goto err;
		}
		for (i = 0; i < t->want_size; i++) {
			int *got;
			ret = dequeue(&want, (void **)&got);
			if (!got) {
				fprintf(stderr, "%s: unexpected NULL result\n",
					t->name);
				goto err;
			}
			if (*got != t->want[i]) {
				fprintf(stderr, "%s: unexpected value[%d]:\n\t- want: %d\n\t-  got: %d\n",
					t->name, i, t->want[i], *got);
				goto err;
			}
		}
		queue_destroy(&want);
		tree_destroy(&tree);
		if (tree_size(&tree)) {
			fprintf(stderr, "%s: unexpected final tree size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, tree_size(&tree));
			goto err;
		}
		continue;
perr:
		perror(t->name);
err:
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
