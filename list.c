/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"

static void list_noop_dtor(void *d)
{
	return;
}

void list_init(struct list *l, void (*dtor)(void *data))
{
	l->head = l->tail = NULL;
	l->dtor = dtor;
	l->size = 0;
}

void list_destroy(struct list *l)
{
	void (*dtor)(void *data) = l->dtor != NULL ? l->dtor : list_noop_dtor;
	struct node *node, *next;

	node = list_head(l);
	for (node = list_head(l); node != NULL; node = next) {
		next = node->next;
		dtor((void *)node->data);
		free(node);
	}
}

int list_ins_next(struct list *l, struct node *n, const void *d)
{
	struct node *node, **next;

	node = malloc(sizeof(*node));
	if (!node)
		return -ENOMEM;
	node->data = d;
	if (!n)
		next = &l->head;
	else
		next = &n->next;
	node->next = *next;
	*next = node;
	if (!node->next)
		l->tail = node;
	l->size++;
	return 0;
}

int list_rem_next(struct list *l, struct node *n, void **d)
{
	struct node *node, **next;

	if (n != NULL)
		next = &n->next;
	else
		next = &l->head;
	if (!*next)
		return -EINVAL;
	node = *next;
	*next = node->next;
	if (!*next)
		l->tail = n;
	*d = (void *)node->data;
	free(node);
	l->size--;
	return 0;
}
