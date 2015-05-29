#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "scaffold16.h"

/* You are suppose to change the routine Product16 here to your own routine 
 * The mpz calls in the scaffolded Product16 below are the normal GMP function
 * calls and should be neglected. By casting the void pointers as normal unsigned
 * integers, you should be able to access the data values as normal 4 bytes words.
 */

void Product16(void *a, void *b, void *c, unsigned int sa, unsigned int bitsa, unsigned int sb, unsigned int bitsb, unsigned int *sc, unsigned int *bitsc)
{
    	unsigned short int *cint_a = (unsigned short int *) a;
    	unsigned short int *cint_b = (unsigned short int *) b;
    	unsigned short int *cint_c = (unsigned short int *) c;
    
	unsigned int iOuter=0,jInner=0,p=0,base=65536, carry;
	*sc=sa+sb;
	if(sa>sb)
	{
		for(iOuter=0;iOuter<sa;iOuter++)
		{
			cint_c[iOuter]=0;
		}
	}	
	else
	{
		for(iOuter=0;iOuter<sb;iOuter++)
		{
			cint_c[iOuter]=0;
		}
	}		
	for(iOuter=0;iOuter<sa;iOuter++)
	{
		carry=0;
		for(jInner=0;jInner<sb;jInner++)
		{
			p=cint_a[iOuter]*cint_b[jInner]+cint_c[iOuter+jInner]+carry;
			cint_c[iOuter+jInner] = p & 65535;
			carry = p >> 16;
		}
		cint_c[iOuter+jInner] = carry;
	}	
}

