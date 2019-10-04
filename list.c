/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"

static void list_noop_dtor(void *d)
{
	return;
}

void hlist_init(struct hlist *l, void (*dtor)(void *data))
{
	l->head = l->tail = NULL;
	l->dtor = dtor;
	l->size = 0;
}

void hlist_destroy(struct hlist *l)
{
	void (*dtor)(void *data) = l->dtor != NULL ? l->dtor : list_noop_dtor;
	struct hlist_node *node, *next;

	node = hlist_head(l);
	for (node = hlist_head(l); node != NULL; node = next) {
		next = node->next;
		dtor((void *)node->data);
		free(node);
	}
}

int hlist_ins_next(struct hlist *l, struct hlist_node *n, const void *d)
{
	struct hlist_node *node, **next;

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

int hlist_rem_next(struct hlist *l, struct hlist_node *n, void **d)
{
	struct hlist_node *node, **next;

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
