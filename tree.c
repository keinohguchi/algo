/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "tree.h"

static int default_cmp(const void *d1, const void *d2)
{
	return d1 - d2;
}

static void default_dtor(void *data) { return; }

int tree_init(struct tree *tree, int (*cmp)(const void *d1, const void *d2),
	      void (*dtor)(void *data))
{
	tree->root = NULL;
	tree->size = 0;
	tree->cmp = cmp ? cmp : default_cmp;
	tree->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void tree_destroy(struct tree *tree)
{
	tree_remove(tree, tree->root);
	tree->root = NULL;
}

int tree_ins_left(struct tree *tree, struct tree_node *node, const void *data)
{
	struct tree_node **next;

	if (node)
		next = &node->left;
	else
		next = &tree->root;
	if (*next) {
		/* node exists */
		if (!tree->cmp((*next)->data, data))
			return 1;
		errno = EINVAL;
		return -1;
	}
	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->left = node->right = NULL;
	node->data = data;
	*next = node;
	tree->size++;
	return 0;
}

int tree_ins_right(struct tree *tree, struct tree_node *node, const void *data)
{
	struct tree_node **next;

	if (node)
		next = &node->right;
	else
		next = &tree->root;
	if (*next) {
		/* node exits */
		if (!tree->cmp((*next)->data, data))
			return 1;
		errno = EINVAL;
		return -1;
	}
	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->left = node->right = NULL;
	node->data = data;
	*next = node;
	tree->size++;
	return 0;
}

void tree_remove(struct tree *tree, struct tree_node *node)
{
	if (!node)
		return;
	tree_remove(tree, node->left);
	tree_remove(tree, node->right);
	tree->dtor((void *)node->data);
	free(node);
	tree->size--;
}
