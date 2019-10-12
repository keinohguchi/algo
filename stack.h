/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _STACK_H
#define _STACK_H

struct stack_node {
	struct stack_node	*next;
	const void		*data;
};

struct stack {
	struct stack_node	*head;
	int			size;
	void			(*dtor)(void *data);
};

int stack_init(struct stack *s, void (*dtor)(void *data));
void stack_destroy(struct stack *s);
static inline int stack_size(const struct stack *s) { return s->size; }
int push(struct stack *s, const void *data);
int pop(struct stack *s, void **data);

#endif /* _STACK_H */
