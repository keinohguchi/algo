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

static void destroy_subtree(struct tree_node *root, void (*dtor)(void *data))
{
	if (!root)
		return;
	destroy_subtree(root->left, dtor);
	destroy_subtree(root->right, dtor);
	dtor((void *)root->data);
	free(root);
}

void tree_destroy(struct tree *tree)
{
	destroy_subtree(tree->root, tree->dtor);
	tree->root = NULL;
	tree->size = 0;
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
