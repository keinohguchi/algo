/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "stack.h"

static void default_dtor(void *data) { return; }

int stack_init(struct stack *s, void (*dtor)(void *data))
{
	s->dtor = dtor ? dtor : default_dtor;
	s->head = NULL;
	s->size = 0;
	return 0;
}

void stack_destroy(struct stack *s)
{
	struct stack_node *node;

	while ((node = s->head)) {
		s->head = node->next;
		s->dtor((void *)node->data);
		free(node);
	}
}

int push(struct stack *s, const void *data)
{
	struct stack_node *node;
	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->data = data;
	node->next = s->head;
	s->head = node;
	s->size++;
	return 0;
}

int pop(struct stack *s, void **data)
{
	struct stack_node *node;

	if (!(node = s->head)) {
		errno = EINVAL;
		return -1;
	}
	*data = (void *)node->data;
	s->head = node->next;
	s->size--;
	free(node);
	return 0;
}
