#include "stdlib.h"
#include <stdio.h>
#include <math.h>
void gauss(int n,double *x,double &E,double &sigma)
{
	int i = 0;
	double sum1=0, sum2=0,xa;
	for (i = 0; i < n;i++)
	{
		sum1 = sum1 + x[i];
	}
	xa = sum1 / n;
	E = sum1 / n;
	for (i = 0; i < n;i++)
	{
		sum2 = sum2 + (x[i] - xa) * (x[i] - xa);
	}
	sigma = sum2 / (n - 1);
	sigma = pow(sigma, 0.5);
}

int main()
{
	double a[5] = {1, 2, 3, 4,5};
	double E,sigma;
	gauss(5,&a[0], E, sigma);
	printf("%lf %lf", E, sigma);
}

