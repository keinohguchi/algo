/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SET_H
#define _SET_H

struct set_node {
	struct set_node	*next;
	const void	*data;
};

struct set {
	int		size;
	struct set_node	*head;
	int		(*same)(const void *d1, const void *d2);
	void		(*dtor)(void *data);
};

int set_init(struct set *s, int (*same)(const void *d1, const void *d2),
	     void (*dtor)(void *data));
void set_destroy(struct set *s);
static inline int set_size(const struct set *s) { return s->size; }
int set_insert(struct set *s, const void *data);
int set_remove(struct set *s, void **data);
int set_union(struct set *su, const struct set *s1, const struct set *s2);
int set_intersect(struct set *si, const struct set *s1, const struct set *s2);
int set_difference(struct set *sd, const struct set *s1, const struct set *s2);
int set_is_member(const struct set *s, const void *data);
int set_is_subset(const struct set *s1, const struct set *s2);
int set_is_equal(const struct set *s1, const struct set *s2);

#endif /* _SET_H */
