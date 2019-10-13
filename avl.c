/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "avl.h"

static int default_cmp(const void *d1, const void *d2)
{
	return d1 - d2;
}

static void default_dtor(void *data) { return; }

int avl_init(struct avl *tree, int (*cmp)(const void *d1, const void *d2),
	     void (*dtor)(void *data))
{
	tree->root = NULL;
	tree->size = 0;
	tree->cmp = cmp ? cmp : default_cmp;
	tree->dtor = dtor ? dtor : default_dtor;
	return 0;
}

static void remove_tree(struct avl *tree, struct avl_node **node)
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

void avl_destroy(struct avl *tree)
{
	remove_tree(tree, &tree->root);
}

static int insert_node(struct avl *tree, struct avl_node **root,
		       const void *data)
{
	struct avl_node *node;

	if (!*root) {
		node = malloc(sizeof(*node));
		if (!node)
			return -1;
		node->left = node->right = NULL;
		node->factor = avl_balanced;
		node->hidden = 0;
		node->data = data;
		*root = node;
		return 0;
	}
	errno = EINVAL;
	return -1;
}

int avl_insert(struct avl *tree, const void *data)
{
	return insert_node(tree, &tree->root, data);
}
