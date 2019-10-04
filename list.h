/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_H
#define _LIST_H

struct list_node {
	struct list_node	*next;
	const void		*data;
};

struct list {
	struct list_node	*head;
	struct list_node	*tail;
	int			size;
	void			(*dtor)(void *d);
};

void list_init(struct list *l, void (*dtor)(void *data));
void list_destroy(struct list *l);
int list_ins_next(struct list *l, struct list_node *n, const void *d);
int list_rem_next(struct list *l, struct list_node *n, void **d);
static inline struct list_node *list_head(const struct list *l)
{
	return l->head;
}
static inline struct list_node *list_tail(const struct list *l)
{
	return l->tail;
}
static inline struct list_node *list_node_next(struct list_node *n)
{
	return n->next;
}
static inline void *list_node_data(struct list_node *n)
{
	return (void *)n->data;
}
static inline int list_size(const struct list *l)
{
	return l->size;
}

#endif /* _LIST_H */
