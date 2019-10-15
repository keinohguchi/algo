/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "heap.h"

static int default_cmp(const void *d1, const void *d2)
{
	return d1 - d2;
}

static void default_dtor(void *data) { return; }

int heap_init(struct heap *heap, int (*cmp)(const void *d1, const void *d2),
	      void (*dtor)(void *data))
{
	heap->size = heap->alloc = 0;
	heap->tree = NULL;
	heap->cmp = cmp ? cmp : default_cmp;
	heap->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void heap_destroy(struct heap *heap)
{
	if (!heap->size)
		return;
	if (heap->dtor != default_dtor) {
		int i;
		for (i = 0; i < heap->size; i++)
			heap->dtor(heap->tree[i]);
	}
	free(heap->tree);
	heap->tree = NULL;
	heap->alloc = 0;
	heap->size = 0;
}

static inline int parent(int i) { return (i-1)/2; }
static inline int left(int i) { return 2*i+1; }
static inline int right(int i) { return 2*i+2; }

static inline void swap(void **tree, int i, int j)
{
	void *tmp = tree[i];
	tree[i] = tree[j];
	tree[j] = tmp;
}

static int candidate(struct heap *heap, int i)
{
	if (right(i) == heap->size
	    || heap->cmp(heap->tree[left(i)], heap->tree[right(i)]) >= 0)
		return left(i);
	else
		return right(i);
}

static void bottomup(struct heap *heap)
{
	int i;

	for (i = heap->size-1; i > 0; i--) {
		/* heapified */
		if (heap->cmp(heap->tree[parent(i)], heap->tree[i]) >= 0)
			return;
		swap(heap->tree, i, parent(i));
	}
}

static void topdown(struct heap *heap)
{
	int i, j;

	for (i = 0; left(i) < heap->size; i = j) {
		j = candidate(heap, i);
		heap->tree[i] = heap->tree[j];
	}
}

int heap_insert(struct heap *heap, const void *data)
{
	if (heap->alloc <= heap->size) {
		int alloc = (heap->size+1)*2;
		heap->tree = realloc(heap->tree, sizeof(void *)*alloc);
		if (!heap->tree)
			return -1;
		heap->alloc = alloc;
	}
	heap->tree[heap->size++] = (void *)data;
	bottomup(heap);
	return 0;
}

int heap_extract(struct heap *heap, void **data)
{
	if (!heap->size) {
		errno = EINVAL;
		return -1;
	}
	*data = heap->tree[0];
	topdown(heap);
	heap->size--;
	return 0;
}
