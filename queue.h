/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _QUEUE_H
#define _QUEUE_H

struct queue_node {
	struct queue_node	*next;
	const void		*data;
};

struct queue {
	struct queue_node	*head;
	struct queue_node	*tail;
	int			size;
	void			(*dtor)(void *data);
};

int queue_init(struct queue *q, void (*dtor)(void *data));
void queue_destroy(struct queue *q);
static inline int queue_size(const struct queue *q) { return q->size; }
int enqueue(struct queue *q, const void *data);
int dequeue(struct queue *q, void **data);

#endif /* _QUEUE_H */
