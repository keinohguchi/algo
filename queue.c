/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "queue.h"

static void default_dtor(void *data) { return; }

int queue_init(struct queue *q, void (*dtor)(void *data))
{
	q->dtor = dtor ? dtor : default_dtor;
	q->head = q->tail = NULL;
	q->size = 0;
	return 0;
}

void queue_destroy(struct queue *q)
{
	struct queue_node *node;

	while ((node = q->head)) {
		q->head = node->next;
		q->dtor((void *)node->data);
		free(node);
	}
}

int enqueue(struct queue *q, const void *data)
{
	struct queue_node *node;

	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->data = data;
	node->next = NULL;
	if (q->tail)
		q->tail->next = node;
	else
		q->head = node;
	q->tail = node;
	q->size++;
	return 0;
}

int dequeue(struct queue *q, void **data)
{
	struct queue_node *node;

	node = q->head;
	if (!node) {
		errno = EINVAL;
		return -1;
	}
	*data = (void *)node->data;
	q->head = node->next;
	if (!q->head)
		q->tail = NULL;
	q->size--;
	free(node);
	return 0;
}
