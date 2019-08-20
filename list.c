/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"

void list_init(struct list *l, void (*destroy)(void *data))
{
	memset(l, 0, sizeof(*l));
	l->head = l->tail = NULL;
	l->destroy = destroy;
	l->size = 0;
}

void list_destroy(struct list *l)
{
	struct node *n, *next;

	for (n = l->head; n; n = next) {
		if (l->destroy)
			(l->destroy)((void *)n->data);
		next = n->next;
		free(n);
	}
	l->head = l->tail = NULL;
	l->destroy = NULL;
	l->size = 0;
}

int list_ins_next(struct list *list, struct node *prev, const void *data)
{
	struct node **next, *node;

	node = malloc(sizeof(struct node));
	if (!node)
		return -1;
	memset(node, 0, sizeof(struct node));
	node->data = data;
	if (prev)
		next = &prev->next;
	else
		next = &list->head;
	if (*next)
		node->next = *next;
	else
		list->tail = node;
	*next = node;
	list->size += 1;
	return 0;
}

int list_rem_next(struct list *l, struct node *prev, void **data)
{
	struct node *node;

	if (prev)
		node = prev->next;
	else
		node = l->head;
	if (!node) {
		errno = EINVAL;
		return -1;
	}
	if (prev)
		prev->next = node->next;
	else
		l->head = node->next;
	if (!node->next)
		l->tail = prev;
	*data = (void *)node->data;
	free(node);
	l->size -= 1;
	return 0;
}
