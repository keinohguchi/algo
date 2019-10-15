/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _HEAP_H
#define _HEAP_H

struct heap {
	int	size;
	int	alloc;
	void	**tree;
	int	(*cmp)(const void *d1, const void *d2);
	void	(*dtor)(void *data);
};

int heap_init(struct heap *heap, int (*cmp)(const void *d1, const void *d2),
	      void (*dtor)(void *data));
void heap_destroy(struct heap *heap);
int heap_insert(struct heap *heap, const void *data);
int heap_extract(struct heap *heap, void **data);
static inline int heap_size(const struct heap *heap) { return heap->size; };

#endif /* _HEAP_H */
