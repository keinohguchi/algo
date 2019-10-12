/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _HASH_H
#define _HASH_H

struct hash_node {
	struct hash_node	*next;
	const void		*data;
};

/* chained hash table */
struct hash {
	int			size;
	int			buckets;
	struct hash_node	**table;
	int			(*h)(const void *key);
	int			(*same)(const void *k1, const void *k2);
	void			(*dtor)(void *data);
};

int hash_init(struct hash *tbl, int buckets, int (*h)(const void *key),
	      int (*same)(const void *k1, const void *k2),
	      void (*dtor)(void *data));
void hash_destroy(struct hash *tbl);
int hash_insert(struct hash *tbl, const void *data);
int hash_remove(struct hash *tbl, void **data);
int hash_lookup(struct hash *tbl, void **data);
static inline int hash_size(const struct hash *tbl) { return tbl->size; }

#endif /* _HASH_H */
