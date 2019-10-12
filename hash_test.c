/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

static int same(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 == *i2;
}

static int divisionby1699(const void *key)
{
	return (unsigned long)key%1699;
}

int main(void)
{
	const struct test {
		const char	*const name;
		int		buckets;
		int		(*h)(const void *key);
		int		size;
		int		data[32];
	} *t, tests[] = {
		{
			.name	= "1 entry with division hash method",
			.h	= divisionby1699,
			.data	= {1},
		},
		{
			.name	= "8 entries with division hash method",
			.h	= divisionby1699,
			.data	= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name	= "16 entries with division hash method",
			.h	= divisionby1699,
			.data	= {
				9, 10, 11, 12, 13, 14, 15, 16,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name	= "32 entries with division hash method",
			.h	= divisionby1699,
			.data	= {
				25, 26, 27, 28, 29, 30, 31, 32,
				9, 10, 11, 12, 13, 14, 15, 16,
				17, 18, 19, 20, 21, 22, 23, 24,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++) {
		struct hash tbl;
		int i, ret;

		ret = hash_init(&tbl, t->buckets, t->h, same, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size; i++) {
			ret = hash_insert(&tbl, &t->data[i]);
			if (ret == -1)
				goto perr;
		}
		if (hash_size(&tbl) != t->size) {
			fprintf(stderr, "%s: unexpected hash size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->size, hash_size(&tbl));
			goto err;
		}
		for (i = 0; i < t->size; i++) {
			int *got = (int *)&t->data[i];
			ret = hash_lookup(&tbl, (void **)&got);
			if (ret == -1)
				goto perr;
			if (*got != t->data[i]) {
				fprintf(stderr, "%s: unexpected lookup value:\n\t- want: %d\n\t-  got: %d\n",
					t->name, t->data[i], *got);
				goto err;
			}
		}
		for (i = 0; i < t->size; i++) {
			int *got = (int *)&t->data[i];
			ret = hash_remove(&tbl, (void **)&got);
			if (ret == -1)
				goto err;
			if (*got != t->data[i]) {
				fprintf(stderr, "%s: unexpected remove value:\n\t- want: %d\n\t-  got: %d\n",
					t->name, t->data[i], *got);
				goto err;
			}
		}
		if (hash_size(&tbl)) {
			fprintf(stderr, "%s: unexpected final hash table size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, hash_size(&tbl));
			goto err;
		}
		hash_destroy(&tbl);
		continue;
perr:
		perror(t->name);
err:
		hash_destroy(&tbl);
		fail++;
	}
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
