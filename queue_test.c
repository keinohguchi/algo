/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct test {
	const char	*const name;
	int		size;
	int		data[8];
	int		want[8];
};

static int test_queue(const struct test *const t)
{
	struct queue q;
	int i, ret;

	ret = queue_init(&q, NULL);
	if (ret == -1)
		goto perr;
	for (i = 0; i < t->size; i++) {
		ret = enqueue(&q, &t->data[i]);
		if (ret == -1)
			goto perr;
	}
	if (queue_size(&q) != t->size) {
		fprintf(stderr, "%s: unexpected queue size:\n\t- want: %d\n\t-  got: %d\n",
			t->name, t->size, queue_size(&q));
		goto err;
	}
	for (i = 0; i < t->size; i++) {
		int *got;
		ret = dequeue(&q, (void **)&got);
		if (ret == -1)
			goto perr;
		if (*got != t->want[i]) {
			fprintf(stderr, "%s: unexpected %dth value:\n\t- want: %d\n\t-  got: %d\n",
				t->name, i+1, t->want[i], *got);
			goto err;
		}
	}
	if (queue_size(&q)) {
		fprintf(stderr, "%s: unexpected final queue size:\n\t- want: 0\n\t-  got: %d\n",
			t->name, queue_size(&q));
	}
	queue_destroy(&q);
	return 0;
perr:
	perror(t->name);
err:
	return -1;
}

int main(void)
{
	const struct test *t, tests[] = {
		{
			.name	= "zero enqueue",
			.size	= 0,
		},
		{
			.name	= "single enqueue",
			.size	= 1,
			.data	= {1},
			.want	= {1},
		},
		{
			.name	= "double enqueues",
			.size	= 2,
			.data	= {1, 2},
			.want	= {1, 2},
		},
		{
			.name	= "quad enqueues",
			.size	= 4,
			.data	= {1, 2, 3, 4},
			.want	= {1, 2, 3, 4},
		},
		{
			.name	= "eight enqueues",
			.size	= 8,
			.data	= {1, 2, 3, 4, 5, 6, 7, 8},
			.want	= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		if (test_queue(t))
			fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
