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
	if (!tree_size(tree))
		return;
	tree_rem_left(tree, NULL);
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

static void remove_tree(struct tree *tree, struct tree_node **node)
{
	if (!*node)
		return;
	remove_tree(tree, &(*node)->left);
	remove_tree(tree, &(*node)->right);
	tree->dtor((void *)(*node)->data);
	free(*node);
	*node = NULL;
	tree->size--;
}

int tree_rem_left(struct tree *tree, struct tree_node *node)
{
	struct tree_node **sub;

	if (node)
		sub = &node->left;
	else
		sub = &tree->root;
	if (!*sub) {
		/* no left subtree */
		errno = EINVAL;
		return -1;
	}
	remove_tree(tree, sub);
	return 0;
}

int tree_rem_right(struct tree *tree, struct tree_node *node)
{
	struct tree_node **sub;

	if (node)
		sub = &node->right;
	else
		sub = &tree->root;
	if (!*sub) {
		/* no right subtree */
		errno = EINVAL;
		return -1;
	}
	remove_tree(tree, sub);
	return 0;
}
