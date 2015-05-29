#include<stdio.h>

void main()
{
	unsigned char *R, *N1, *N2;
	unsigned int *sr;
	unsigned int sn1=3,sn2=2;
	R=(unsigned char *)malloc(6 * sizeof(unsigned char *));
	N1=(unsigned char *)malloc(4 * sizeof(unsigned char *));
	N2=(unsigned char *)malloc(4 * sizeof(unsigned char *));
	sr= (unsigned int *)malloc(sizeof(unsigned int *)); 
	sub(R, N1, N2, sr, sn1, sn2);
}

void sub(unsigned char *R, unsigned char *N1, unsigned char *N2, unsigned int *sr, unsigned int sn1, unsigned int sn2)
{
	int i, p, carry=0;
	for(i = 0; i < sn2; i++)
	{
		if(N2[i]+carry>N1[i])
		{
			p=10+N1[i]-N2[i]-carry;
			carry=1;
		}
		else
		{
			p=N1[i] - N2[i]-carry;
			carry=0;
		}
		R[i]=p;
	}
	for(i=sn2; i<sn1; i++)
	{
		if(carry>N1[i])
		{
			p=10-carry;
			R[i]=p;
		}
		else
		{
			if(carry !=0)
			{
				R[i]=N1[i]-carry;
			}
			else
			{
				R[i]=N1[i];
			}
		}
	}
	*sr=i;
}

void add(unsigned char *R, unsigned char *N1, unsigned char *N2, unsigned int *sr, unsigned int sn1, unsigned int sn2)
{
	if(sn1 == 0 && sn2==0)
	{
		*sr=0;
	}
	else if(sn1==0)
	{
		*sr=sn2;
		memcpy(R, N2, sn2);
	}
	else if(sn2==0)
	{
		*sr=sn1;
		memcpy(R, N1, sn1);
	}
	else
	{	
		int i, p, carry=0;
		if(sn1>sn2)
		{
			for(i = 0; i < sn2; i++)
			{
				p = N1[i] + N2[i] + carry;
				R[i] = p & 255;
				carry = p >> 8;
			}
			for(i=sn2; i<sn1; i++)	
			{
				if(carry!=0)
				{
					p = N1[i] + carry;
					R[i] = p & 255;
					carry = p >> 8;
				}
				else
				{
					break;
				}
			}
			printf("%d %d ", carry, i);
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
		else if(sn1<sn2)
		{
			for(i = 0; i < sn1; i++)
			{
				p = N1[i] + N2[i] + carry;
				R[i] = p & 255;
				carry = p >> 8;
			}
			for(i=sn1; i<sn2; i++)	
			{
				if(carry!=0)
				{
					p = N2[i] + carry;
					R[i] = p & 255;
					carry = p >> 8;
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
		else
		{
			for(i = 0; i < sn1; i++)
			{
				p = N1[i] + N2[i] + carry;
				R[i] = p & 255;
				carry = p >> 8;
			}
			if(carry!=0)
			{
				R[i]=carry;
				*sr=i+1;
			}
		}
	}
}
