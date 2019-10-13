/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
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
