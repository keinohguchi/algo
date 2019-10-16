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
		int				vertices[8];
		struct edge { int from, to; } 	edges[16];
		int				start;
		int				want[8];
	} *t, tests[] = {
		{
			.name		= "two vertices, directly connected",
			.vertices	= {1, 2},
			.edges		= {{1, 2}},
			.start		= 1,
			.want		= {1},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct graph g;
		int ret;

		ret = graph_init(&g, same, NULL);
		if (ret == -1)
			goto perr;
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
