/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
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
