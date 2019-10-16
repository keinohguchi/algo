/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include "set.h"
#include "graph.h"

static int default_same(const void *d1, const void *d2)
{
	return d1 == d2;
}

static void default_dtor(void *data) { return; }

int graph_init(struct graph *g, int (*same)(const void *d1, const void *d2),
	       void (*dtor)(void *d))
{
	g->vcount = g->ecount = 0;
	g->vertices = NULL;
	g->same = same ? same : default_same;
	g->dtor = dtor ? dtor : default_dtor;
	return 0;
}

void graph_destroy(struct graph *g)
{
	struct graph_vertex *v, *next;

	for (v = g->vertices; v; v = next) {
		next = v->next;
		set_destroy(v->adjlist);
		g->dtor(v->data);
		free(v);
	}
}


