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
		int				size;
		int				vertices[8];
		int				edge_size;
		struct edge { int from, to; } 	edges[16];
		int				start;
		int				want_vcount;
		int				want_ecount;
		int				want_path_size;
		int				want_path[16];
	} *t, tests[] = {
		{
			.name		= "two vertices, directly connected",
			.size		= 2,
			.vertices	= {1, 2},
			.edge_size	= 1,
			.edges		= {{1, 2}},
			.start		= 1,
			.want_vcount	= 2,
			.want_ecount	= 1,
			.want_path_size	= 2,
			.want_path	= {1, 2},
		},
		{
			.name		= "two vertices, not connected",
			.size		= 2,
			.vertices	= {1, 2},
			.edge_size	= 1,
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
		for (i = 0; i < t->size; i++) {
			ret = graph_ins_vertex(&g, &t->vertices[i]);
			if (ret == -1)
				goto perr;
		}
		graph_destroy(&g);
		continue;
perr:
		perror(t->name);
		fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
