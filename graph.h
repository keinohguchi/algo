/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _GRAPH_H
#define _GRAPH_H

#include "set.h"

struct graph_vertex {
	struct graph_vertex	*next;
	void			*data;
	struct set		*adjlist;
};

struct graph {
	int			vcount;
	int			ecount;
	struct graph_vertex	*vertices;
	int			(*same)(const void *d1, const void *d2);
	void			(*dtor)(void *data);
};

int graph_init(struct graph *g, int (*same)(const void *d1, const void *d2),
	       void (*dtor)(void *data));
void graph_destroy(struct graph *g);
int graph_ins_vertex(struct graph *g, const void *data);
int graph_rem_vertex(struct graph *g, void **data);
int graph_ins_edge(struct graph *g, const void *d1, const void *d2);
int graph_rem_edge(struct graph *g, const void *d1, void **d2);
int graph_is_adjacent(const struct graph *g, const void *d1, const void *d2);
int graph_vertex(const struct graph *g, struct graph_vertex **v);
static inline struct graph_vertex *graph_vertices(const struct graph *g)
{
	return g->vertices;
}

#endif /* _GRAPH_H */
