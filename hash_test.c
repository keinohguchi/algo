/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

static int same(const void *d1, const void *d2)
{
	const int *i1 = d1, *i2 = d2;
	return *i1 == *i2;
}

static int divisionby3(const void *key)
{
	return (unsigned long)key%3;
}

static int divisionby5(const void *key)
{
	return (unsigned long)key%5;
}

static int divisionby11(const void *key)
{
	return (unsigned long)key%11;
}

static int divisionby13(const void *key)
{
	return (unsigned long)key%13;
}

static int divisionby17(const void *key)
{
	return (unsigned long)key%17;
}

static int divisionby19(const void *key)
{
	return (unsigned long)key%19;
}

static int divisionby31(const void *key)
{
	return (unsigned long)key%31;
}

static int divisionby37(const void *key)
{
	return (unsigned long)key%37;
}

static int divisionby1693(const void *key)
{
	return (unsigned long)key%1693;
}

static int divisionby1699(const void *key)
{
	return (unsigned long)key%1699;
}

static int test_hash(void)
{
	const struct test {
		const char	*const name;
		int		buckets;
		int		(*h)(const void *key);
		int		size;
		int		data[32];
	} *t, tests[] = {
		{
			.name		= "hash: 1 entry with division by 37 hash method",
			.buckets	= 1,
			.h		= divisionby37,
			.size		= 1,
			.data		= {1},
		},
		{
			.name		= "hash: 8 entries with division by 37 hash method",
			.buckets	= 4,
			.h		= divisionby37,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "hash: 16 entries with division by 37 hash method",
			.buckets	= 8,
			.h		= divisionby37,
			.size		= 16,
			.data		= {
				9, 10, 11, 12, 13, 14, 15, 16,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "hash: 32 entries with division by 37 hash method",
			.buckets	= 16,
			.h		= divisionby37,
			.size		= 32,
			.data		= {
				25, 26, 27, 28, 29, 30, 31, 32,
				9, 10, 11, 12, 13, 14, 15, 16,
				17, 18, 19, 20, 21, 22, 23, 24,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "hash: 1 entry with division by 1699 hash method",
			.buckets	= 1,
			.h		= divisionby1699,
			.size		= 1,
			.data		= {1},
		},
		{
			.name		= "hash: 8 entries with division by 1699 hash method",
			.buckets	= 4,
			.h		= divisionby1699,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "hash: 16 entries with division by 1699 hash method",
			.buckets	= 8,
			.h		= divisionby1699,
			.size		= 16,
			.data		= {
				9, 10, 11, 12, 13, 14, 15, 16,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "hash: 32 entries with division by 1699 hash method",
			.buckets	= 16,
			.h		= divisionby1699,
			.size		= 32,
			.data		= {
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
	return fail;
}

static int test_ohash(void)
{
	const struct test {
		const char	*const name;
		int		positions;
		int		(*h1)(const void *key);
		int		(*h2)(const void *key);
		int		size;
		int		data[32];
	} *t, tests[] = {
		{
			.name		= "ohash: 1 entry with division by 5/3 hash method",
			.positions	= 5,
			.h1		= divisionby5,
			.h2		= divisionby3,
			.size		= 1,
			.data		= {1},
		},
		{
			.name		= "ohash: 8 entries with division by 13/11 hash method",
			.positions	= 13,
			.h1		= divisionby13,
			.h2		= divisionby11,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "ohash: 16 entries with division by 19/17 hash method",
			.positions	= 19,
			.h1		= divisionby19,
			.h2		= divisionby17,
			.size		= 16,
			.data		= {
				9, 10, 11, 12, 13, 14, 15, 16,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "ohash: 32 entries with division by 37/31 hash method",
			.positions	= 37,
			.h1		= divisionby37,
			.h2		= divisionby31,
			.size		= 32,
			.data		= {
				25, 26, 27, 28, 29, 30, 31, 32,
				9, 10, 11, 12, 13, 14, 15, 16,
				17, 18, 19, 20, 21, 22, 23, 24,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "ohash: 1 entry with division by 1699/1693 hash method",
			.positions	= 2,
			.h1		= divisionby1699,
			.h2		= divisionby1693,
			.size		= 1,
			.data		= {1},
		},
		{
			.name		= "ohash: 8 entries with division by 1699/1693 hash method",
			.positions	= 11,
			.h1		= divisionby1699,
			.h2		= divisionby1693,
			.size		= 8,
			.data		= {1, 2, 3, 4, 5, 6, 7, 8},
		},
		{
			.name		= "ohash: 16 entries with division by 1699/1693 hash method",
			.positions	= 31,
			.h1		= divisionby1699,
			.h2		= divisionby1693,
			.size		= 16,
			.data		= {
				9, 10, 11, 12, 13, 14, 15, 16,
				1, 2, 3, 4, 5, 6, 7, 8,
			},
		},
		{
			.name		= "ohash: 32 entries with division by 1699/1693 hash method",
			.positions	= 61,
			.h1		= divisionby1699,
			.h2		= divisionby1693,
			.size		= 32,
			.data		= {
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
		struct ohash tbl;
		int i, ret;

		ret = ohash_init(&tbl, t->positions, t->h1, t->h2, same, NULL);
		if (ret == -1)
			goto perr;
		for (i = 0; i < t->size; i++) {
			ret = ohash_insert(&tbl, &t->data[i]);
			if (ret == -1)
				goto perr;
		}
		if (ohash_size(&tbl) != t->size) {
			fprintf(stderr, "%s: unexpected hash size:\n\t- want: %d\n\t-  got: %d\n",
				t->name, t->size, ohash_size(&tbl));
			goto err;
		}
		for (i = 0; i < t->size; i++) {
			int *got = (int *)&t->data[i];
			ret = ohash_lookup(&tbl, (void **)&got);
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
			ret = ohash_remove(&tbl, (void **)&got);
			if (ret == -1)
				goto perr;
			if (*got != t->data[i]) {
				fprintf(stderr, "%s: unexpected remove value:\n\t- want: %d\n\t-  got: %d\n",
					t->name, t->data[i], *got);
				goto err;
			}
		}
		if (ohash_size(&tbl)) {
			fprintf(stderr, "%s: unexpected final hash table size:\n\t- want: 0\n\t-  got: %d\n",
				t->name, ohash_size(&tbl));
			goto err;
		}
		ohash_destroy(&tbl);
		continue;
perr:
		perror(t->name);
err:
		ohash_destroy(&tbl);
		fail++;
	}
	return fail;
}

int main(void)
{
	int fail = 0;

	fail += test_hash();
	fail += test_ohash();
	if (fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
