/*
 ============================================================================
 * brent.c
 *  Created on: Jan 14, 2019
 ============================================================================
*/
#include "gmp.h"

/* Method *********************************************************************/
int
min(int a, int b)
{	if (a < b)		return a;
	else			return b;
} // end min

/* Method *********************************************************************/
int
prbrent(mpz_t factor, const mpz_t n, mpz_t start)
{	/* Assign Data **********************************************************/
	long long k, r = 1, m = 43, s;
			mpz_init(factor);		// possible factor
	mpz_t tmp;	mpz_init(tmp);			// temporary storage
	mpz_t x;	mpz_init_set(x, start);		// x = tortoise = start
	mpz_t y;	mpz_init_set(y, start);		// y = hare = start
	mpz_t gcd;	mpz_init_set_ui(gcd, 1);
	mpz_t ys;	mpz_init(ys);
	mpz_t q;	mpz_init_set_ui(q, 1);

	/* Algorithm ************************************************************/
	while(mpz_cmp_ui(gcd,1) == 0)			// while (gcd == 1)
	{	mpz_set(x, y);				// x = y;
		int i;
		for (i = 1; i < r; i++)			// f(y)
		{	mpz_mul(y, y, y);		// y * y
			mpz_add_ui(y, y, 1);		// y + 1
			mpz_mod(y, y, n);		// y % n
		} // end for
		k = 0;
		while (k<r && mpz_cmp_ui(gcd,1)==0)	// while (k < r && gcd == 1)
		{	mpz_set(ys, y);			// ys = y = y saved
			s = min(m, r - k);		// smaller value
			int i;
			for (i = 0; i < s; i++)
			{	mpz_mul(y, y, y);	// y * y
				mpz_add_ui(y, y, 1);	// y + 1
				mpz_mod(y, y, n);	// y % n

				mpz_sub(tmp, x, y);	// x - y
				mpz_abs(tmp, tmp);	// absolute value
				mpz_mul(q, q, tmp);	// q * tmp
				mpz_mod(q, q, n);	// q % n
			} // end for
			mpz_gcd(gcd, q, n);		// gcd
			k += m;
		} //end while
		r *= 2;					// r = cycle size
	} // end while

	if (gcd == n)
	{	do
		{	mpz_mul(ys, ys, ys);		// ys * ys
			mpz_add_ui(ys, ys, 1);		// ys + 1
			mpz_mod(ys, ys, n);		// ys % n

			mpz_sub(tmp, x, ys);		// x - ys
			mpz_abs(tmp, tmp);		// absolute value
			mpz_gcd(gcd, tmp, n);		// gcd
		} while (mpz_cmp_ui(gcd,1) == 0);	// while gcd==1
	} // end if

	if (mpz_cmp(gcd,n)==0)				// check logic == 0
	{	mpz_clear(gcd);				// free up memory
		mpz_clear(tmp);
		mpz_clear(x);
		mpz_clear(y);
		mpz_clear(ys);
		mpz_clear(q);
		return 0;				// failed
	}else
	{	mpz_set(factor, gcd);			// save for return
		mpz_clear(gcd);				// free up memory
		mpz_clear(tmp);
		mpz_clear(x);
		mpz_clear(y);
		mpz_clear(ys);
		mpz_clear(q);
		return 1;				// success
	} // end if/else
} // end prbrent()
