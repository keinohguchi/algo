/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_H
#define _LIST_H

struct node {
	struct node	*next;
	const void	*data;
};

struct list {
	struct node	*head;
	struct node	*tail;
	int		size;
	void		(*dtor)(void *d);
};

void list_init(struct list *l, void (*dtor)(void *data));
void list_destroy(struct list *l);
int list_ins_next(struct list *l, struct node *n, const void *d);
int list_rem_next(struct list *l, struct node *n, void **d);
static inline struct node *list_head(const struct list *l)
{
	return l->head;
}
static inline struct node *list_tail(const struct list *l)
{
	return l->tail;
}
static inline struct node *list_node_next(struct node *n)
{
	return n->next;
}
static inline void *list_node_data(struct node *n)
{
	return (void *)n->data;
}
static inline int list_size(const struct list *l)
{
	return l->size;
}

#endif /* _LIST_H */
