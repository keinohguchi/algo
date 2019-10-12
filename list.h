/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_H
#define _LIST_H

struct list_node {
	struct list_node	*next;
	struct list_node	*prev;
	const void		*data;
};

struct list {
	struct list_node	list;
	int			size;
	void			(*dtor)(void *);
};

int list_init(struct list *l, void (*dtor)(void *data));
void list_destroy(struct list *l);
int list_ins_next(struct list *l, struct list_node *n, const void *d);
int list_rem_next(struct list *l, struct list_node *n, void **d);
static inline struct list_node *list_head(const struct list *l)
{
	return l->list.next;
}
static inline struct list_node *list_tail(const struct list *l)
{
	return l->list.prev;
}
static inline int list_size(const struct list *l)
{
	return l->size;
}
static inline struct list_node *list_next(const struct list_node *n)
{
	return n->next;
}
#define list_for_each(l, n)	for (n = list_head(l); n != &(l)->list; n = list_next(n))

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

int hlist_init(struct hlist *l, void (*dtor)(void *data));
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
static inline int hlist_size(const struct hlist *l)
{
	return l->size;
}
static inline struct hlist_node *hlist_next(const struct hlist_node *n)
{
	return n->next;
}

#endif /* _LIST_H */
