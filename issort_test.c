/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern int issort(void *data, size_t esize, size_t n,
		  int (*cmp)(const void *a, const void *b));

static int intcmp(const void *a, const void *b)
{
	const int *ap = a, *bp = b;
	return *ap - *bp;
}

struct test {
	const char	*const name;
	int		data[16];
	size_t		n;
	int		want[16];
};

static void test(const struct test *restrict t)
{
	int ret, i;

	ret = issort((void *)t->data, sizeof(int), t->n, intcmp);
	if (ret) {
		fprintf(stderr, "%s: issort() exit with %d\n",
			t->name, ret);
		goto err;
	}
	for (i = 0; i < t->n; i++)
		if (t->data[i] != t->want[i]) {
			fprintf(stderr, "%s: %d(data[%d]) != %d(want[%d])\n",
				t->name, t->data[i], i, t->want[i], i);
			goto err;
		}
	exit(EXIT_SUCCESS);
err:
	exit(EXIT_FAILURE);
}

int main(void)
{
	const struct test *t, tests[] = {
		{
			.name	= "zero entry",
			.data	= {},
			.n	= 0,
			.want	= {},
		},
		{
			.name	= "single entry",
			.data	= {1},
			.n	= 1,
			.want	= {1},
		},
		{
			.name	= "sorted two entries",
			.data	= {1, 2},
			.n	= 2,
			.want	= {1, 2},
		},
		{
			.name	= "unsorted two entries",
			.data	= {2, 1},
			.n	= 2,
			.want	= {1, 2},
		},
		{
			.name	= "sorted four entries",
			.data	= {1, 2, 3, 4},
			.n	= 4,
			.want	= {1, 2, 3, 4},
		},
		{
			.name	= "unsorted four entries",
			.data	= {-1, -2, -3, -4},
			.n	= 4,
			.want	= {-4, -3, -2, -1},
		},
		{
			.name	= "sorted sixteen entries",
			.data	= {
				1, 2, 3, 4, 5, 6, 7, 8,
				9, 10, 11, 12, 13, 14, 15, 16,
			},
			.n	= 16,
			.want	= {
				1, 2, 3, 4, 5, 6, 7, 8,
				9, 10, 11, 12, 13, 14, 15, 16,
			},
		},
		{
			.name	= "unsorted sixteen entries",
			.data	= {
				-1, -2, -3, -4, -5, -6, -7, -8,
				-9, -10, -11, -12, -13, -14, -15, -16,
			},
			.n	= 16,
			.want	= {
				-16, -15, -14, -13, -12, -11, -10, -9,
				-8, -7, -6, -5, -4, -3, -2, -1,
			},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		int ret, status;
		pid_t pid;

		pid = fork();
		if (pid == -1)
			goto perr;
		else if (pid == 0)
			test(t);

		ret = waitpid(pid, &status, 0);
		if (ret == -1)
			goto perr;
		if (WIFSIGNALED(status)) {
			fprintf(stderr, "%s: signaled by %s\n",
				t->name, strsignal(WTERMSIG(status)));
			goto err;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "%s: does not exit\n",
				t->name);
			goto err;
		}
		if (WEXITSTATUS(status))
			goto err;
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
