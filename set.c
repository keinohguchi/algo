/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
#include "set.h"

static int default_same(const void *d1, const void *d2)
{
	return d1 == d2;
}

static void default_dtor(void *data) { return; }

int set_init(struct set *s, int (*same)(const void *d1, const void *d2),
	     void (*dtor)(void *data))
{
	s->head = NULL;
	s->size = 0;
	s->same = same ? same : default_same;
	s->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void set_destroy(struct set *s)
{
	struct set_node *node;

	while ((node = s->head)) {
		s->head = node->next;
		s->dtor((void *)node->data);
		free(node);
	}
}

int set_insert(struct set *s, const void *data)
{
	struct set_node *node;

	for (node = s->head; node; node = node->next)
		if (s->same(node->data, data))
			return 1;
	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->next = s->head;
	node->data = data;
	s->head = node;
	s->size++;
	return 0;
}

int set_remove(struct set *s, void **data)
{
	struct set_node *node, **pprev;

	pprev = &s->head;
	for (node = s->head; node; pprev = &node->next, node = node->next)
		if (s->same(node->data, *data))
			break;
	if (!node) {
		errno = EINVAL;
		return -1;
	}
	*data = (void *)node->data;
	*pprev = node->next;
	free(node);
	s->size--;
	return 0;
}

int set_union(struct set *su, const struct set *s1, const struct set *s2)
{
	struct set_node *node;
	int ret;

	/* data won't be freed by new set */
	ret = set_init(su, s1->same, NULL);
	if (ret == -1)
		return -1;
	/* insert s1 data into su */
	for (node = s1->head; node; node = node->next) {
		ret = set_insert(su, node->data);
		if (ret == -1)
			goto err;
	}
	/* insert s2 data into su */
	for (node = s2->head; node; node = node->next) {
		ret = set_insert(su, node->data);
		if (ret == -1)
			goto err;
	}
	return 0;
err:
	set_destroy(su);
	return ret;
}

int set_intersect(struct set *si, const struct set *s1, const struct set *s2)
{
	struct set_node *node;
	int ret;

	/* si won't free the data */
	ret = set_init(si, s1->same, NULL);
	if (ret == -1)
		return -1;
	/* insert only the data exists both in s1 and s2 */
	for (node = s1->head; node; node = node->next)
		if (set_is_member(s2, node->data)) {
			ret = set_insert(si, node->data);
			if (ret == -1)
				goto err;
		}
	return 0;
err:
	set_destroy(si);
	return ret;
}

int set_difference(struct set *sd, const struct set *s1, const struct set *s2)
{
	struct set_node *node;
	int ret;

	/* sd won't free the data */
	ret = set_init(sd, s1->same, NULL);
	if (ret == -1)
		return -1;
	for (node = s1->head; node; node = node->next)
		if (!set_is_member(s2, node->data)) {
			ret = set_insert(sd, node->data);
			if (ret == -1)
				goto err;
		}
	return 0;
err:
	set_destroy(sd);
	return ret;
}

int set_is_member(const struct set *s, const void *data)
{
	struct set_node *node;

	for (node = s->head; node; node = node->next)
		if (s->same(node->data, data))
			return 1;
	return 0;
}

int set_is_subset(const struct set *s1, const struct set *s2)
{
	struct set_node *node;

	if (set_size(s1) > set_size(s2))
		return 0;
	for (node = s1->head; node; node = node->next)
		if (!set_is_member(s2, node->data))
			return 0;
	return 1;
}

int set_is_equal(const struct set *s1, const struct set *s2)
{
	struct set_node *node;

	if (set_size(s1) != set_size(s2))
		return 0;
	for (node = s1->head; node; node = node->next)
		if (!set_is_member(s2, node->data))
			return 0;
	return 1;
}
