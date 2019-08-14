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
	size_t		size;
	void		(*destroy)(void *data);
};

void list_init(struct list *l, void (*destroy)(void *data));
void list_destroy(struct list *l);
int list_ins_next(struct list *l, struct node *n, const void *data);
int list_rem_next(struct list *l, struct node *n, void **data);

static inline int list_size(const struct list *l)
{
	return l->size;
}

static inline struct node *list_head(const struct list *l)
{
	return l->head;
}

static inline struct node *list_tail(const struct list *l)
{
	return l->tail;
}

#endif /* _LIST_H */
