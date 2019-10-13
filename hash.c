/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash.h"

static int divisionby1699(const void *key)
{
	return (unsigned long)key%1699;
}

static int default_same(const void *k1, const void *k2)
{
	return k1 == k2;
}

static void default_dtor(void *data) { return; }

int hash_init(struct hash *tbl, int buckets, int (*h)(const void *key),
	      int (*same)(const void *k1, const void *k2),
	      void (*dtor)(void *data))
{
	struct hash_node **table;

	table = malloc(sizeof(struct hash_node *)*buckets);
	if (!table)
		return -1;
	memset(table, 0, sizeof(struct hash_node *)*buckets);
	tbl->table = table;
	tbl->buckets = buckets;
	tbl->size = 0;
	tbl->h = h ? h : divisionby1699;
	tbl->same = same ? same : default_same;
	tbl->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void hash_destroy(struct hash *tbl)
{
	int i;

	for (i = 0; i < tbl->buckets; i++) {
		struct hash_node *node = tbl->table[i];
		while (node) {
			struct hash_node *next = node->next;
			tbl->dtor((void *)node->data);
			free(node);
			node = next;
		}
	}
	free(tbl->table);
}

int hash_insert(struct hash *tbl, const void *data)
{
	int key = tbl->h(data) % tbl->buckets;
	struct hash_node *node;

	for (node = tbl->table[key]; node; node = node->next)
		if (tbl->same((void *)node->data, data))
			return 1;
	node = malloc(sizeof(*node));
	if (!node)
		return -1;
	node->next = tbl->table[key];
	node->data = data;
	tbl->table[key] = node;
	tbl->size++;
	return 0;
}

int hash_remove(struct hash *tbl, void **data)
{
	int key = tbl->h(*data) % tbl->buckets;
	struct hash_node *node, **pprev = &tbl->table[key];

	for (node = tbl->table[key]; node; pprev = &node->next, node = node->next)
		if (tbl->same(node->data, *data)) {
			*data = (void *)node->data;
			*pprev = node->next;
			free(node);
			tbl->size--;
			return 0;
		}
	errno = EINVAL;
	return -1;
}

int hash_lookup(const struct hash *tbl, void **data)
{
	int key = tbl->h(*data) % tbl->buckets;
	struct hash_node *node;

	for (node = tbl->table[key]; node; node = node->next)
		if (tbl->same(node->data, *data)) {
			*data = (void *)node->data;
			return 0;
		}
	errno = EINVAL;
	return -1;
}

/* vacated node placeholder for the search performance improvement */
static char ohash_vacated;

int ohash_init(struct ohash *tbl, int positions,
	       int (*h1)(const void *key), int (*h2)(const void *key),
	       int (*same)(const void *k1, const void *k2),
	       void (*dtor)(void *data))
{
	void **table = malloc(sizeof(void *)*positions);
	if (!table)
		return -1;
	memset(table, 0, sizeof(void *)*positions);
	tbl->vacated = &ohash_vacated;
	tbl->positions = positions;
	tbl->table = table;
	tbl->size = 0;
	tbl->h1 = h1 ? h1 : divisionby1699;
	tbl->h2 = h2 ? h2 : divisionby1699;
	tbl->same = same ? same : default_same;
	tbl->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void ohash_destroy(struct ohash *tbl)
{
	int i;

	for (i = 0; i < tbl->positions; i++)
		if (tbl->table[i] && tbl->table[i] != tbl->vacated)
			tbl->dtor((void *)tbl->table[i]);
	free(tbl->table);
}

static int ohash_key(const struct ohash *tbl, const void *data, int i)
{
	return (tbl->h1(data) + i * tbl->h2(data)) % tbl->positions;
}

int ohash_insert(struct ohash *tbl, const void *data)
{
	int i;

	for (i = 0; i < tbl->positions; i++) {
		int key = ohash_key(tbl, data, i);
		if (i != 0)
			fprintf(stderr,
				"%s: %d time(s) hash code(%d) collision for data(%p)\n",
				__FUNCTION__, i, key, data);
		if (tbl->table[key] && tbl->table[key] != tbl->vacated) {
			if (tbl->same(tbl->table[key], data))
				return 1;
			continue;
		}
		tbl->table[key] = (void *)data;
		tbl->size++;
		return 0;
	}
	errno = EINVAL;
	return -1;
}

int ohash_remove(struct ohash *tbl, void **data)
{
	int i;

	for (i = 0; i < tbl->positions; i++) {
		int key = ohash_key(tbl, *data, i);
		if (i != 0)
			fprintf(stderr,
				"%s: %d time(s) hash code(%d) collision for data(%p)\n",
				__FUNCTION__, i, key, *data);
		if (tbl->table[key] == NULL)
			break;
		else if (tbl->table[key] == tbl->vacated
		    || !tbl->same(tbl->table[key], *data))
			continue;
		*data = tbl->table[key];
		tbl->table[key] = tbl->vacated;
		tbl->size--;
		return 0;
	}
	errno = EINVAL;
	return -1;
}

int ohash_lookup(const struct ohash *tbl, void **data)
{
	int i;

	for (i = 0; i < tbl->positions; i++) {
		int key = ohash_key(tbl, *data, i);
		if (i != 0)
			fprintf(stderr,
				"%s: %d time(s) hash code(%d) collision for data(%p)\n",
				__FUNCTION__, i, key, *data);
		if (tbl->table[key] == NULL)
			break;
		else if (tbl->table[key] == tbl->vacated
			 || !tbl->same(tbl->table[key], *data))
			continue;
		*data = tbl->table[key];
		return 0;
	}
	errno = EINVAL;
	return -1;
}
