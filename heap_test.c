/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

static int ascending(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i2 - *i1;
}

static int decending(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 - *i2;
}

int main(void)
{
	const struct test {
		const char	*const name;
		int		(*cmp)(const void *d1, const void *d2);
		int		size;
		const int	data[8];
		int		want_size;
		const int	want_raw[8];
		const int	want[8];
	} *t, tests[] = {
		{
			.name		= "ascending 2 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 2,
			.data		= {1, 2},
			.want_size	= 2,
			.want_raw	= {1, 2},
			.want		= {1, 2},
		},
		{
			.name		= "ascending 2 inputs to decending order heap",
			.cmp		= decending,
			.size		= 2,
			.data		= {1, 2},
			.want_size	= 2,
			.want_raw	= {2, 1},
			.want		= {2, 1},
		},
		{
			.name		= "decending 2 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 2,
			.data		= {2, 1},
			.want_size	= 2,
			.want_raw	= {1, 2},
			.want		= {1, 2},
		},
		{
			.name		= "decending 2 inputs to decending order heap",
			.cmp		= decending,
			.size		= 2,
			.data		= {2, 1},
			.want_size	= 2,
			.want_raw	= {2, 1},
			.want		= {2, 1},
		},
		{
			.name		= "ascending 3 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 3,
			.data		= {1, 2, 3},
			.want_size	= 3,
			.want_raw	= {1, 2, 3},
			.want		= {1, 2, 3},
		},
		{
			.name		= "ascending 3 inputs to decending order heap",
			.cmp		= decending,
			.size		= 3,
			.data		= {1, 2, 3},
			.want_size	= 3,
			.want_raw	= {3, 1, 2},
			.want		= {3, 2, 1},
		},
		{
			.name		= "decending 3 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 3,
			.data		= {3, 2, 1},
			.want_size	= 3,
			.want_raw	= {1, 3, 2},
			.want		= {1, 2, 3},
		},
		{
			.name		= "decending 3 inputs to decending order heap",
			.cmp		= decending,
			.size		= 3,
			.data		= {3, 2, 1},
			.want_size	= 3,
			.want_raw	= {3, 2, 1},
			.want		= {3, 2, 1},
		},
		{
			.name		= "ascending 4 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.want_size	= 4,
			.want_raw	= {1, 2, 3, 4},
			.want		= {1, 2, 3, 4},
		},
		{
			.name		= "ascending 4 inputs to decending order heap",
			.cmp		= decending,
			.size		= 4,
			.data		= {1, 2, 3, 4},
			.want_size	= 4,
			.want_raw	= {4, 3, 2, 1},
			.want		= {4, 3, 2, 1},
		},
		{
			.name		= "decending 4 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 4,
			.data		= {4, 3, 2, 1},
			.want_size	= 4,
			.want_raw	= {1, 2, 3, 4},
			.want		= {1, 2, 3, 4},
		},
		{
			.name		= "decending 4 inputs to decending order heap",
			.cmp		= decending,
			.size		= 4,
			.data		= {4, 3, 2, 1},
			.want_size	= 4,
			.want_raw	= {4, 3, 2, 1},
			.want		= {4, 3, 2, 1},
		},
		{
			.name		= "ascending 5 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 5,
			.data		= {1, 2, 3, 4, 5},
			.want_size	= 5,
			.want_raw	= {1, 2, 3, 4, 5},
			.want		= {1, 2, 3, 4, 5},
		},
		{
			.name		= "ascending 5 inputs to decending order heap",
			.cmp		= decending,
			.size		= 5,
			.data		= {1, 2, 3, 4, 5},
			.want_size	= 5,
			.want_raw	= {5, 4, 2, 1, 3},
			.want		= {5, 4, 3, 2, 1},
		},
		{
			.name		= "decending 5 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 5,
			.data		= {5, 4, 3, 2, 1},
			.want_size	= 5,
			.want_raw	= {1, 2, 4, 5, 3},
			.want		= {1, 2, 3, 4, 5},
		},
		{
			.name		= "decending 5 inputs to decending order heap",
			.cmp		= decending,
			.size		= 5,
			.data		= {5, 4, 3, 2, 1},
			.want_size	= 5,
			.want_raw	= {5, 4, 3, 2, 1},
			.want		= {5, 4, 3, 2, 1},
		},
		{
			.name		= "ascending 6 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 6,
			.data		= {1, 2, 3, 4, 5, 6},
			.want_size	= 6,
			.want_raw	= {1, 2, 3, 4, 5, 6},
			.want		= {1, 2, 3, 4, 5, 6},
		},
		{
			.name		= "ascending 6 inputs to decending order heap",
			.cmp		= decending,
			.size		= 6,
			.data		= {1, 2, 3, 4, 5, 6},
			.want_size	= 6,
			.want_raw	= {6, 4, 5, 1, 3, 2},
			.want		= {6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "decending 6 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 6,
			.data		= {6, 5, 4, 3, 2, 1},
			.want_size	= 6,
			.want_raw	= {1, 3, 2, 6, 4, 5},
			.want		= {1, 2, 3, 4, 5, 6},
		},
		{
			.name		= "decending 6 inputs to decending order heap",
			.cmp		= decending,
			.size		= 6,
			.data		= {6, 5, 4, 3, 2, 1},
			.want_size	= 6,
			.want_raw	= {6, 5, 4, 3, 2, 1},
			.want		= {6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "ascending 7 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 7,
			.data		= {1, 2, 3, 4, 5, 6, 7},
			.want_size	= 7,
			.want_raw	= {1, 2, 3, 4, 5, 6, 7},
			.want		= {1, 2, 3, 4, 5, 6, 7},
		},
		{
			.name		= "ascending 7 inputs to decending order heap",
			.cmp		= decending,
			.size		= 7,
			.data		= {1, 2, 3, 4, 5, 6, 7},
			.want_size	= 7,
			.want_raw	= {7, 4, 6, 1, 3, 2, 5},
			.want		= {7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "decending 7 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 7,
			.data		= {7, 6, 5, 4, 3, 2, 1},
			.want_size	= 7,
			.want_raw	= {1, 4, 2, 7, 5, 6, 3},
			.want		= {1, 2, 3, 4, 5, 6, 7},
		},
		{
			.name		= "decending 7 inputs to decending order heap",
			.cmp		= decending,
			.size		= 7,
			.data		= {7, 6, 5, 4, 3, 2, 1},
			.want_size	= 7,
			.want_raw	= {7, 6, 5, 4, 3, 2, 1},
			.want		= {7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "ascending 8 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.want_size	= 8,
			.want_raw	= {1, 2, 3, 4, 5, 6, 7, 8},
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "ascending 8 inputs to decending order heap",
			.cmp		= decending,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
			.want_size	= 8,
			.want_raw	= {8, 7, 6, 4, 3, 2, 5, 1},
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{
			.name		= "decending 8 inputs to ascending order heap",
			.cmp		= ascending,
			.size		= 8,
			.data		= {8, 7, 6, 5, 4, 3, 2, 1},
			.want_size	= 8,
			.want_raw	= {1, 2, 3, 5, 6, 7, 4, 8},
			.want		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "decending 8 inputs to decending order heap",
			.cmp		= decending,
			.size		= 8,
			.data		= {8, 7, 6, 5, 4, 3, 2, 1},
			.want_size	= 8,
			.want_raw	= {8, 7, 6, 5, 4, 3, 2, 1},
			.want		= {8, 7, 6, 5, 4, 3, 2, 1},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct heap heap;
		int i, ret;

		ret = heap_init(&heap, t->cmp, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size; i++) {
			ret = heap_insert(&heap, &t->data[i]);
			if (ret == -1)
				goto perr;
		}
		if (heap_size(&heap) != t->want_size) {
			fprintf(stderr, "%s: unexpected heap size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->want_size, heap_size(&heap));
			goto err;
		}
		for (i = 0; i < t->want_size; i++) {
			int *got = heap.tree[i];
			if (*got != t->want_raw[i]) {
				fprintf(stderr, "%s: unexpected raw value[%d]:\n\t- want: %d\n\t-  got: %d\n",
					t->name, i, t->want_raw[i], *got);
				goto err;
			}
		}
		for (i = 0; i < t->want_size; i++) {
			int *got;

			ret = heap_extract(&heap, (void **)&got);
			if (ret == -1)
				goto perr;
			if (*got != t->want[i]) {
				fprintf(stderr, "%s: unexpected extracted value[%d]:\n\t- want: %d\n\t-  got: %d\n",
					t->name, i, t->want[i], *got);
				goto err;
			}
		}
		heap_destroy(&heap);
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
