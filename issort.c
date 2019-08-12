/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>

int issort(void *data, size_t esize, size_t n,
	   int (*cmp)(const void *a, const void *b))
{
	char *a = data;
	void *key;
	int i, j;

	key = malloc(esize);
	if (!key)
		return -1;
	for (j = 1; j < n; j++) {
		memcpy(key, &a[j*esize], esize);
		for (i = j-1; i >= 0; i--) {
			/* find a spot for the key */
			if (cmp(key, &a[i*esize]) > 0)
				break;
			/* make a space for the key */
			memcpy(&a[(i+1)*esize], &a[i*esize], esize);
		}
		memcpy(&a[(i+1)*esize], key, esize);
	}
	free(key);
	return 0;
}
