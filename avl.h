/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _AVL_H
#define _AVL_H

struct avl_node {
	struct avl_node	*left;
	struct avl_node	*right;
	const void	*data;
};

struct avl {
	struct avl_node	*root;
	int		size;
	int		(*cmp)(const void *d1, const void *d2);
	void		(*dtor)(void *data);
};

int avl_init(struct avl *tree, int (*cmp)(const void *d1, const void *d2),
	     void (*dtor)(void *data));
void avl_destroy(struct avl *tree);
int avl_insert(struct avl *tree, const void *data);
int avl_remove(struct avl *tree, void **data);
int avl_lookup(struct avl *tree, void **data);
static inline int avl_size(const struct avl *tree) { return tree->size; }

#endif /* _AVL_H */
