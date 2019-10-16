/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

static int same(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 == *i2;
}

int main(void)
{
	const struct test {
		const char			*const name;
		int				vertices_size;
		int				vertices[8];
		int				edges_size;
		struct edge { int from, to; } 	edges[16];
		int				start;
		int				want_vcount;
		int				want_ecount;
		int				want_path_size;
		int				want_path[16];
	} *t, tests[] = {
		{
			.name		= "two vertices, directly connected",
			.vertices_size	= 2,
			.vertices	= {1, 2},
			.edges_size	= 1,
			.edges		= {{1, 2}},
			.start		= 1,
			.want_vcount	= 2,
			.want_ecount	= 1,
			.want_path_size	= 2,
			.want_path	= {1, 2},
		},
		{
			.name		= "two vertices, not connected",
			.vertices_size	= 2,
			.vertices	= {1, 2},
			.edges_size	= 1,
			.edges		= {{1, 2}},
			.start		= 2,
			.want_vcount	= 2,
			.want_ecount	= 1,
			.want_path_size	= 0,
			.want_path	= {},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct graph g;
		int i, ret;

		ret = graph_init(&g, same, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->vertices_size; i++) {
			ret = graph_ins_vertex(&g, &t->vertices[i]);
			if (ret == -1)
				goto perr;
		}
		for (i = 0; i < t->edges_size; i++) {
			ret = graph_ins_edge(&g, &t->edges[i].from,
					     &t->edges[i].to);
			if (ret == -1)
				goto perr;
		}
		if (graph_vcount(&g) != t->want_vcount) {
			fprintf(stderr, "%s: unexpected vertex count:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_vcount, graph_vcount(&g));
			goto err;
		}
		if (graph_ecount(&g) != t->want_ecount) {
			fprintf(stderr, "%s: unexpected edge count:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_ecount, graph_ecount(&g));
			goto err;
		}
		for (i = 0; i < t->edges_size; i++) {
			int *got = (int *)&t->edges[i].to;
			ret = graph_rem_edge(&g, &t->edges[i].from, (void **)&got);
			if (ret == -1)
				goto perr;
		}
		for (i = 0; i < t->vertices_size; i++) {
			int *got = (int *)&t->vertices[i];
			ret = graph_rem_vertex(&g, (void **)&got);
			if (ret == -1)
				goto perr;
		}
		if (graph_vcount(&g)) {
			fprintf(stderr, "%s: unexpected final vertex count:\n\t- want: 0\n\t-  got: %d\n",
				t->name, graph_vcount(&g));
			goto err;
		}
		if (graph_ecount(&g)) {
			fprintf(stderr, "%s: unexpected final edge count:\n\t- want: 0\n\t-  got: %d\n",
				t->name, graph_ecount(&g));
			goto err;
		}
		graph_destroy(&g);
		continue;
perr:
		perror(t->name);
err:
		fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
