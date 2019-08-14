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

int list_ins_next(struct list *l, struct node *n, const void *data)
{
	struct node **nextp, *newp;

	if (!n) {
		nextp = &l->head;
		goto insert;
	}
	for (nextp = &l->head; *nextp; nextp = &(*nextp)->next) {
		if (*nextp == n) {
			nextp = &(*nextp)->next;
			goto insert;
		}
	}
	errno = EINVAL;
	return -1;
insert:
	newp = malloc(sizeof(struct node));
	if (!newp)
		return -1;
	memset(newp, 0, sizeof(struct node));
	newp->data = data;
	if (*nextp)
		newp->next = (*nextp)->next;
	if (!newp->next)
		l->tail = newp;
	*nextp = newp;
	l->size += 1;
	return 0;
}

int list_rem_next(struct list *l, struct node *n, void **data)
{
	struct node *node, *prev = NULL;

	if (!n) {
		node = l->head;
		if (node == NULL) {
			errno = EINVAL;
			return -1;
		}
		goto remove;
	}
	for (node = l->head; node; node = node->next) {
		if (node == n)
			goto remove;
		prev = node;
	}
	errno = EINVAL;
	return -1;
remove:
	l->size -= 1;
	*data = (void *)node->data;
	if (prev)
		prev->next = node->next;
	else
		l->head = node->next;
	if (!node->next)
		l->tail = prev;
	free(node);
	return 0;
}
