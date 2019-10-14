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

static int rotate_left(struct avl_node **node)
{
	struct avl_node *child = (*node)->left;
	struct avl_node *right = child->right;

	switch (child->factor) {
	case avl_left_heavy:
		/* LL rotation */
		child->factor = (*node)->factor = avl_balanced;
		(*node)->left = right;
		child->right = *node;
		*node = child;
		return 0;
	case avl_right_heavy:
		/* LR rotation */
		switch (right->factor) {
		case avl_left_heavy:
			(*node)->factor = avl_right_heavy;
			child->factor = avl_balanced;
			break;
		case avl_right_heavy:
			(*node)->factor = avl_balanced;
			child->factor = avl_left_heavy;
			break;
		case avl_balanced:
			(*node)->factor = child->factor = avl_balanced;
			break;
		}
		right->factor = avl_balanced;
		(*node)->left = right->right;
		right->right = *node;
		child->right = right->left;
		right->left = child;
		*node = right;
		return 0;
	default:
		errno = EINVAL;
		return -1;
	}
}

static int rotate_right(struct avl_node **node)
{
	struct avl_node *child = (*node)->right;
	struct avl_node *left = child->left;

	switch (child->factor) {
	case avl_right_heavy:
		/* RR rotation */
		child->factor = (*node)->factor = avl_balanced;
		(*node)->right = left;
		child->left = *node;
		*node = child;
		return 0;
	case avl_left_heavy:
		/* RL rotation */
		switch (left->factor) {
		case avl_left_heavy:
			(*node)->factor = avl_balanced;
			child->factor = avl_right_heavy;
			break;
		case avl_right_heavy:
			(*node)->factor = avl_left_heavy;
			child->factor = avl_balanced;
			break;
		case avl_balanced:
			(*node)->factor = child->factor = avl_balanced;
			break;
		}
		left->factor = avl_balanced;
		child->factor = (*node)->factor = avl_balanced;
		(*node)->right = left->left;
		left->left = *node;
		child->left = left->right;
		left->right = child;
		*node = left;
		return 0;
	default:
		errno = EINVAL;
		return -1;
	}
}

static int insert_node(struct avl *tree, struct avl_node **node,
		       const void *data, int *balanced);

static int insert_left(struct avl *tree, struct avl_node **node,
		       const void *data, int *balanced)
{
	struct avl_node **child = &(*node)->left;
	int ret;

	ret = insert_node(tree, child, data, balanced);
	if (ret == -1)
		return -1;
	if (*balanced)
		return 0;
	switch ((*node)->factor) {
	case avl_left_heavy:
		if (rotate_left(node) == -1)
			return -1;
		*balanced = 1;
		break;
	case avl_balanced:
		(*node)->factor = avl_left_heavy;
		break;
	case avl_right_heavy:
		(*node)->factor = avl_balanced;
		*balanced = 1;
		break;
	default:
		errno = EINVAL;
		return -1;
	}
	return 0;
}

static int insert_right(struct avl *tree, struct avl_node **node,
			const void *data, int *balanced)
{
	struct avl_node **child = &(*node)->right;
	int ret;

	ret = insert_node(tree, child, data, balanced);
	if (ret == -1)
		return -1;
	if (*balanced)
		return 0;
	switch ((*node)->factor) {
	case avl_left_heavy:
		(*node)->factor = avl_balanced;
		*balanced = 1;
		break;
	case avl_balanced:
		(*node)->factor = avl_right_heavy;
		break;
	case avl_right_heavy:
		if (rotate_right(node) == -1)
			return -1;
		*balanced = 1;
		break;
	default:
		errno = EINVAL;
		return -1;
	}
	return 0;
}

static struct avl_node *alloc_node(const void *data)
{
	struct avl_node *node;

	node = malloc(sizeof(*node));
	if (!node)
		return NULL;
	node->left = node->right = NULL;
	node->factor = avl_balanced;
	node->hidden = 0;
	node->data = data;
	return node;
}

static int insert_node(struct avl *tree, struct avl_node **node,
		       const void *data, int *balanced)
{
	int ret;

	if (!*node) {
		if (!(*node = alloc_node(data)))
			return -1;
		tree->size++;
		*balanced = 0;
		return 0;
	}
	ret = tree->cmp(data, (*node)->data);
	if (ret < 0)
		return insert_left(tree, node, data, balanced);
	else if (ret > 0)
		return insert_right(tree, node, data, balanced);
	else
		/* no duplicate nodes */
		return 0;
}

int avl_insert(struct avl *tree, const void *data)
{
	int balanced = 0;
	return insert_node(tree, &tree->root, data, &balanced);
}
