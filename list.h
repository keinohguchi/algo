/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_H
#define _LIST_H

struct hlist_node {
	struct hlist_node	*next;
	const void		*data;
};

struct hlist {
	struct hlist_node	*head;
	struct hlist_node	*tail;
	int			size;
	void			(*dtor)(void *d);
};

void hlist_init(struct hlist *l, void (*dtor)(void *data));
void hlist_destroy(struct hlist *l);
int hlist_ins_next(struct hlist *l, struct hlist_node *n, const void *d);
int hlist_rem_next(struct hlist *l, struct hlist_node *n, void **d);
static inline struct hlist_node *hlist_head(const struct hlist *l)
{
	return l->head;
}
static inline struct hlist_node *hlist_tail(const struct hlist *l)
{
	return l->tail;
}
static inline struct hlist_node *hlist_node_next(struct hlist_node *n)
{
	return n->next;
}
static inline void *hlist_node_data(struct hlist_node *n)
{
	return (void *)n->data;
}
static inline int hlist_size(const struct hlist *l)
{
	return l->size;
}

#endif /* _LIST_H */
