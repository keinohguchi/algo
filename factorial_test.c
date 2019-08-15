/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>

extern int factorial(int n, int fact);

struct test {
	const char	*const name;
	int		n;
	int		want;
};

static int test(const struct test *restrict t)
{
	int got = factorial(t->n, 1);
	if (got != t->want) {
		fprintf(stderr, "%s: unexpected result:\n\t- want: %d\n\t-  got: %d\n",
			t->name, t->want, got);
		return -1;
	}
	return 0;
}

int main(void)
{
	const struct test *t, tests[] = {
		{
			.name	= "0!",
			.n	= 0,
			.want	= 1,
		},
		{
			.name	= "1!",
			.n	= 1,
			.want	= 1,
		},
		{
			.name	= "2!",
			.n	= 2,
			.want	= 2,
		},
		{
			.name	= "3!",
			.n	= 3,
			.want	= 6,
		},
		{
			.name	= "4!",
			.n	= 4,
			.want	= 24,
		},
		{
			.name	= "5!",
			.n	= 5,
			.want	= 120,
		},
		{
			.name	= "6!",
			.n	= 6,
			.want	= 720,
		},
		{
			.name	= "7!",
			.n	= 7,
			.want	= 5040,
		},
		{
			.name	= "8!",
			.n	= 8,
			.want	= 40320,
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++)
		if (test(t))
			fail++;
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
