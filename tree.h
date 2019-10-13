/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TREE_H
#define _TREE_H

struct tree_node {
	struct tree_node	*left;
	struct tree_node	*right;
	const void		*data;
};

struct tree {
	struct tree_node	*root;
	int			size;
	int			(*cmp)(const void *d1, const void *d2);
	void			(*dtor)(void *data);
};

int tree_init(struct tree *tree, int (*cmp)(const void *d1, const void *d2),
	      void (*dtor)(void *data));
void tree_destroy(struct tree *tree);
int tree_ins_left(struct tree *tree, struct tree_node *node, const void *data);
int tree_ins_right(struct tree *tree, struct tree_node *node, const void *data);
int tree_rem_left(struct tree *tree, struct tree_node *node);
int tree_rem_right(struct tree *tree, struct tree_node *node);
int tree_merge(struct tree *merge, struct tree *t1, struct tree *t2);
static inline int tree_size(const struct tree *tree) { return tree->size; }

#endif /* _TREE_H */
