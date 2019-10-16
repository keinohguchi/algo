/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <errno.h>
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
		set_destroy(&v->adjlist);
		g->dtor((void *)v->data);
		free(v);
	}
}

int graph_ins_vertex(struct graph *g, const void *data)
{
	struct graph_vertex *v;
	int ret;

	for (v = g->vertices; v; v = v->next)
		if (g->same(v->data, data))
			return 1;
	v = malloc(sizeof(*v));
	if (!v)
		return -1;
	ret = set_init(&v->adjlist, g->same, NULL);
	if (ret == -1) {
		free(v);
		return -1;
	}
	v->data = data;
	v->next = g->vertices;
	g->vertices = v;
	g->vcount++;
	return 0;
}

int graph_rem_vertex(struct graph *g, void **data)
{
	struct graph_vertex *v, **prev = &g->vertices;

	for (v = g->vertices; v; prev = &v, v = v->next)
		if (g->same(v->data, *data)) {
			set_destroy(&v->adjlist);
			*data = (void *)v->data;
			*prev = v->next;
			free(v);
			g->vcount--;
			return 0;
		}
	return -1;
}

int graph_ins_edge(struct graph *g, const void *d1, const void *d2)
{
	struct graph_vertex *v;

	for (v = g->vertices; v; v = v->next)
		if (g->same(v->data, d1)) {
			int ret = set_insert(&v->adjlist, d2);
			if (ret == 0)
				g->ecount++;
			return ret;
		}
	/* no vertex for d1 yet */
	errno = EINVAL;
	return -1;
}

int graph_rem_edge(struct graph *g, const void *d1, void **d2)
{
	struct graph_vertex *v;

	for (v = g->vertices; v; v = v->next)
		if (g->same(v->data, d1)) {
			int ret = set_remove(&v->adjlist, d2);
			if (ret == 0)
				g->ecount--;
			return ret;
		}
	/* no vertex for d1 yet */
	errno = EINVAL;
	return -1;
}
