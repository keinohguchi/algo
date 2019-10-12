/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"

static void dtor_noop(void *d)
{
	return;
}

static void init_list_node(struct list_node *n, struct list_node *head, const void *data)
{
	n->next = n->prev = head;
	n->data = data;
}

int list_init(struct list *l, void (*dtor)(void *data))
{
	init_list_node(&l->list, &l->list, NULL);
	l->dtor = dtor ? dtor : dtor_noop;
	l->size = 0;
	return 0;
}

void list_destroy(struct list *l)
{
	struct list_node *node, *next;

	for (node = l->list.next; node != &l->list; node = next) {
		l->dtor((void *)node->data);
		next = node->next;
		free(node);
	}
}

int list_ins_next(struct list *l, struct list_node *n, const void *d)
{
	struct list_node *node, **next, **prev;

	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	init_list_node(node, &l->list, d);
	if (n)
		next = &n->next;
	else
		next = &l->list.next;
	prev = &(*next)->prev;
	node->next = *next;
	node->prev = *prev;
	*next = *prev = node;
	l->size++;
	return 0;
}

int list_rem_next(struct list *l, struct list_node *n, void **d)
{
	struct list_node *node;

	if (!n)
		n = &l->list;
	node = n->next;
	if (node == &l->list) {
		errno = EINVAL;
		return -1;
	}
	n->next = node->next;
	node->next->prev = n;
	*d = (void *)node->data;
	free(node);
	l->size--;
	return 0;
}

int hlist_init(struct hlist *l, void (*dtor)(void *data))
{
	l->dtor = dtor ? dtor : dtor_noop;
	l->head = l->tail = NULL;
	l->size = 0;
	return 0;
}

void hlist_destroy(struct hlist *l)
{
	struct hlist_node *n, *next;

	for (n = l->head; n != NULL; n = next) {
		l->dtor((void *)n->data);
		next = n->next;
		free(n);
	}
}

int hlist_ins_next(struct hlist *l, struct hlist_node *n, const void *d)
{
	struct hlist_node *new, **next;

	new = malloc(sizeof(*new));
	if (!new)
		return -1;
	new->data = d;
	if (n)
		next = &n->next;
	else
		next = &l->head;
	new->next = *next;
	*next = new;
	if (!new->next)
		l->tail = new;
	l->size++;
	return 0;
}

int hlist_rem_next(struct hlist *l, struct hlist_node *n, void **d)
{
	struct hlist_node *node, **next;

	if (n)
		next = &n->next;
	else
		next = &l->head;
	if (!*next) {
		errno = EINVAL;
		return -1;
	}
	node = *next;
	*next = node->next;
	if (!node->next)
		l->tail = n;
	*d = (void *)node->data;
	free(node);
	l->size--;
	return 0;
}
