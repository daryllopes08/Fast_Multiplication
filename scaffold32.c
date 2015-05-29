#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "scaffold32.h"


void Product32(void *a, void *b, void *c, unsigned int wa, unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc)
{
   	fastMultiply(a, b, c, wa, wb, wc); //Call to 5.2 algorithm  
}

void fastMultiply(void *a, void *b, void *c, unsigned int sa, unsigned int sb, unsigned int *scc)
{
	if(sa<=42 || sb<=42) //Check for word size(42 is set with trial and error) - Recursive exit condition
	{
		traditionalMultiply(a, b, c, sa, sb, scc); //Call to 1.8 algorithm
		return;
	}
	unsigned int *cint_a = (unsigned int *) a;
    	unsigned int *cint_b = (unsigned int *) b;
    	unsigned int *cint_c = (unsigned int *) c;	
	unsigned int *U1, *U2, *V1, *V2, *T1, *T2, *C, *R, *W1, *W2, *W3, *W4;
	unsigned int *st1, *st2, *sw1, *sw2, *sw3, *sw4, *sc, *sr;
	unsigned int su1, su2, sv1, sv2, baseSplit;
	*scc=0;
	if(sa==sb) //Equal size word
	{
		baseSplit=floor(sa/2);
		su1=sa-baseSplit;
		su2=baseSplit;
		sv1=sa-baseSplit;
		sv2=baseSplit;
	}
	else if(sa>sb) //Unequal size - Split the bigger word size number into half and respectively the smaller word size number 
	{
		baseSplit=floor(sa/2); 
		su1=sa-baseSplit; 
		su2=baseSplit;
		if(baseSplit>sb)
		{
			sv1=0;
			sv2=sb;
		}
		else
		{
			sv1=sb-baseSplit;
			sv2=baseSplit;
		}
	}
	else
	{
		baseSplit=floor(sb/2);
		sv1=sb-baseSplit;
		sv2=baseSplit;
		if(baseSplit>sa)
		{
			su1=0;
			su2=sa;
		}
		else
		{
			su1=sa-baseSplit;
			su2=baseSplit;
		}
	
	}
	//Allocation of memory
	int wordAlloc;
	if(sa>sb)
		wordAlloc=sa;
	else
		wordAlloc=sb;
	U1 = (unsigned int *)malloc((sa) * sizeof(unsigned int *));		
	U2= (unsigned int *)malloc((sa) * sizeof(unsigned int *));
	V1 = (unsigned int *)malloc((sb) * sizeof(unsigned int *));		
	V2 = (unsigned int *)malloc((sb) * sizeof(unsigned int *));
	T1= (unsigned int *)malloc((baseSplit) * sizeof(unsigned int *));
	T2= (unsigned int *)malloc((baseSplit) * sizeof(unsigned int *));
	W1 = (unsigned int *)malloc((wordAlloc) * sizeof(unsigned int *));		
	W2 = (unsigned int *)malloc((wordAlloc) * sizeof(unsigned int *));		
	W3 = (unsigned int *)malloc((sa+sb) * sizeof(unsigned int *));		
	W4 = (unsigned int *)malloc((wordAlloc) * sizeof(unsigned int *));
	R = (unsigned int *)malloc((baseSplit) * sizeof(unsigned int *));				
	C = (unsigned int *)malloc((baseSplit) * sizeof(unsigned int *));
	st1= (unsigned int *)malloc(sizeof(unsigned int *));
	st2= (unsigned int *)malloc(sizeof(unsigned int *));
	sw1= (unsigned int *)malloc(sizeof(unsigned int *));
	sw2= (unsigned int *)malloc(sizeof(unsigned int *));
	sw3= (unsigned int *)malloc(sizeof(unsigned int *));
	sw4= (unsigned int *)malloc(sizeof(unsigned int *));
	sr= (unsigned int *)malloc(sizeof(unsigned int *));
	sc= (unsigned int *)malloc(sizeof(unsigned int *));
	if(su2!=0)
	{
		memcpy(U2, cint_a, su2*4);
	}
	if(su1!=0)
	{
		memcpy(U1, &cint_a[su2], su1*4);
	}
	if(sv2!=0)
	{
		memcpy(V2, cint_b, sv2*4);
	}
	if(sv1!=0)
	{
		memcpy(V1, &cint_b[sv2], sv1*4);
	}
	add(T1, U1, U2, st1, su1, su2); //Call to add
	add(T2, V1, V2, st2, sv1, sv2); //Call to add
	//traditionalMultiply(T1, T2, W3, *st1, *st2, sw3);
	fastMultiply(T1, T2, W3, *st1, *st2, sw3); //Call to 5.2 algorithm
	//traditionalMultiply(U1, V1, W2, su1, sv1, sw2);
	fastMultiply(U1, V1, W2, su1, sv1, sw2); //Call to 5.2 algorithm
	//traditionalMultiply(U2, V2, W4, su2, sv2, sw4);
	fastMultiply(U2, V2, W4, su2, sv2, sw4); //Call to 5.2 algorithm
	sub(W3, W3, W2, sw3, *sw3, *sw2);  //Call to sub
	sub(W3, W3, W4, sw3, *sw3, *sw4); //Call to sub
	division(W4, *sw4, baseSplit, C, sc, W4, sw4); //Call to div
	add(W3, W3, C, sw3, *sw3, *sc);//Call to add
	division(W3, *sw3, baseSplit, C, sc, W3, sw3); //Call to div
	add(W2, W2, C, sw2, *sw2, *sc);//Call to add
	division(W2, *sw2, baseSplit, W1, sw1, W2, sw2); //Call to div
	//Integration of product
	unsigned int  *t;
	if(*sw4!=0)
	{
		memcpy(cint_c, W4, *sw4*4);
		if (*sw4 < baseSplit){
			t =  &cint_c[*sw4];
			memset(t, 0 , (baseSplit - *sw4)*4);			
		}
	}
	else
	{
		memset(cint_c, 0, baseSplit*4);
	}
	if(*sw3!=0)
	{
		memcpy(&cint_c[baseSplit], W3, *sw3*4);
		if (*sw3 < baseSplit)
		{
			t =  &cint_c[baseSplit+*sw3];
			memset(t, 0 , (baseSplit - *sw3)*4);			
		}

	}
	else
	{
		memset(&cint_c[baseSplit], 0, baseSplit*4);
	}
	
	if(*sw2!=0)
	{
		memcpy(&cint_c[baseSplit*2], W2, *sw2*4);
		if(*sw2<baseSplit && *sw1!=0)
		{
			t =  &cint_c[(baseSplit*2)+*sw2];
			memset(t, 0,  (baseSplit - *sw2)*4);
		} 
	}
	else
	{
			if(*sw1!=0)
			{
				memset(&cint_c[baseSplit*2], 0, baseSplit*4);
				//memcpy(&cint_c[baseSplit*3], W1,  *sw1*4);
			}
	}	
	if(*sw1!=0)
	{
			memcpy(&cint_c[baseSplit*3], W1,  *sw1*4);
			*scc=baseSplit*3+*sw1;
	}
	else
	{
			*scc=baseSplit*2+*sw2;
	}
 }

void traditionalMultiply(void *a, void *b, void *c, unsigned int wa, unsigned int wb, unsigned int *wc)
{
	if(wb==0 || wa==0)
	{
		*wc=0;      //Return zero if any one of the number is zero
		return;
	}
	unsigned int *cint_a = (unsigned int *) a;
    	unsigned int *cint_b = (unsigned int *) b;
    	unsigned int *cint_c = (unsigned int *) c;
	unsigned int iOuter=0, jInner=0;
	unsigned long long p, carry;
	//*wc=wa+wb;
	for(iOuter=0;iOuter<wa+wb;iOuter++) //Initialize to zero
	{
			cint_c[iOuter]=0;
	}	
	for(iOuter=0;iOuter<wa;iOuter++) //Multiplication
	{
		carry=0;
		for(jInner=0; jInner<wb; jInner++)
		{
			p=(unsigned long long)cint_a[iOuter]*cint_b[jInner]+cint_c[iOuter+jInner]+carry;
			carry = p >> 32;
			cint_c[iOuter+jInner] = p-(carry<<32);//p & 4294967295;
			
		}
		cint_c[iOuter+jInner] = carry;
	}
	if(carry!=0)
	{
		*wc=wa+wb;
	}
	else
	{
		*wc=wa+wb-1;
	}
}

void add(unsigned int *R, unsigned int  *N1, unsigned int *N2, unsigned int *sr, unsigned int sn1, unsigned int sn2)
{
	if(sn1==0 && sn2==0)
	{
		*sr=0; //Return zero if any one of the number is zero
	}
	else if(sn1==0)
	{
		*sr=sn2;
		memcpy(R, N2, sn2*4); //if number one zero just return the other number
	}
	else if(sn2==0)
	{
		*sr=sn1;
		memcpy(R, N1, sn1*4);
	}
	//Addition of Numbers
	else
	{	
		
		int i;
		long long p, carry=0;
		if(sn1>sn2) //For number 1 greater than number 2
		{
			for(i = 0; i < sn2; i++)
			{
				p = (unsigned long long) N1[i] + N2[i] + carry;
				carry = p >> 32;//p / 4294967296;
				R[i] = p-(carry<<32);//p % 4294967296;
				

			}
			for(i=sn2; i<sn1; i++)	
			{
				if(carry!=0)
				{
					p =(unsigned long long) N1[i] + carry;
					carry = p >> 32;//p / 4294967296;
					R[i] = p-(carry<<32);//p % 4294967296;
					
				}
				else
				{
					break;
				}
			}
			if(carry!=0 && i==sn1)
			{
				R[i]=carry;
				*sr=i+1;
			}
			else
			{
				for(;i<sn1;i++)
				{
					R[i] = N1[i];
				}
				*sr=i;
			}
		}
		else if(sn1<sn2) //For number 2 greater than number 1
		{
			for(i = 0; i < sn1; i++)
			{
				p = (unsigned long long) N1[i] + N2[i] + carry;
				carry = p >> 32;//p / 4294967296;
				R[i] = p-(carry<<32);//p % 4294967296;
				
			}
			for(i=sn1; i<sn2; i++)	
			{
				if(carry!=0)
				{
					p = (unsigned long long) N2[i] + carry;
					carry = p >> 32;//p / 4294967296;
					R[i] = p-(carry<<32);//p % 4294967296;
					
				}
				else
				{
					break;
				}
			}
			if(carry!=0 && i==sn2)
			{
				R[i]=carry;
				*sr=i+1;
			}
			else
			{
				for(;i<sn2;i++)
				{
					R[i] = N2[i];
				}
				*sr=i;
			}
		}
		else //equal size
		{
			for(i = 0; i < sn1; i++)
			{
				p = (unsigned long long) N1[i] + N2[i] + carry;
				carry = p >> 32;//p/4294967296;
				R[i] = p-(carry<<32);//p%4294967296;
				
			}
			if(carry!=0)
			{
				R[i]=carry;
				*sr=i+1;
			}
			else
			{
				*sr=i;
			}
		}
	}
}

void sub(unsigned int *R, unsigned int *N1, unsigned int *N2, unsigned int *sr, unsigned int sn1, unsigned int sn2)
{
	if(sn2==0) //Number 2 is zero than return number 1
	{
		*sr=sn1;
		memcpy(R, N1, sn1*4);
	}
	//subraction logic
	long long i, p, carry=0;
	for(i = 0; i < sn2; i++)
	{
		if(N2[i]+carry>N1[i])
		{
			p=(unsigned long long)4294967296+N1[i]-N2[i]-carry;
			carry=1;
		}
		else
		{
			p=(unsigned long long)N1[i] - N2[i]-carry;
			carry=0;
		}
		R[i]=p;
	}
	for(i=sn2; i<sn1; i++)
	{
		if(carry>N1[i])
		{

			p=(unsigned long long)4294967296-carry;
			R[i]=p;
			carry=1;
		}
		else
		{
			if(carry !=0)
			{
				R[i]=N1[i]-carry;
				carry=0;
			}
			else
			{
				R[i]=N1[i];
			}
		}
	}
	*sr=i;
}

void division(unsigned int *d, unsigned int sd, unsigned int sw, unsigned int *q, unsigned int *sq, unsigned int *r, unsigned int *sr)
{
	if(sd==0) //If dividend is zero
	{
		*sq=0;
		*sr=0;
		return;
	}
	if(sd<sw) // If dividend is smaller than word sw
	{
		*sq=0;
		*sr=sd;
		memcpy(r,d,sd*4);
	}
	else 
	{
			*sq = sd - sw;
			memcpy(q, &d[sw], *sq*4);
			memcpy(r, d, sw*4);
			*sr = sw;
			int i;
			for(i=*sq-1; i>=0; i--)
			{
				if(q[i]!=0)
				{
					*sq=i+1;
					break;
				}	
			}	
	}
	
} 
