/* SPDX-License-Identifier: GPL-2.0 */

int factorial(int n, int fact)
{
	switch (n) {
	case 1:
		return n * fact;
	case 0:
		return fact;
	}
	fact *= n;
	n--;
	return factorial(n, fact);
}
