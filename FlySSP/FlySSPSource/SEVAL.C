/*------------------------------------------------------------------------
  SEVAL.C

  ����   : ��ॢ�� �ணࠬ�� � c FORTRAN (���ᠩ�) �믮���� ���뭪�� �.�.
  �����  : 01.00 / 21.05.2000 /
------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"
/* ********************************************
		       B��.KY���.C��A�HA
 SEVAL=Y(I)+B(I)*(U-X(I))+C(I)*(U-X(I))**2+D(I)*(U-X(I))**3
  X(I).LT.U.LT.X(I+1)
 IF U.LT.X(1)  ,TO I=1
 IF U.GE.X(N)  ,TO I=N
       BXO�:
 N - ��C�O Y��OB N>2
 U - AP�YMEHT
 X,Y - MACC�B� A�C��CC � OP��HAT
 B,C,D - MACC�B� KO���-B
 �HTEPBA� HAX-C� �BO��H�M �O�CKOM
******************************************** */


double seval(int n, double u, double x[], double y[],
		      double b[], double c[], double d[])
{
int    j, k;
double dx, seval_v;
static int i = 1;

if( i >= n )
	i = 1;
if( u < x[i-1] )
	goto L_10;
if( u < x[i + 1-1] )
	goto L_30;
/*  �BO��H. �O�CK */
L_10:
i = 1;
j = n + 1;
L_20:
k = (i + j)/2;
if( u < x[k-1] )
	j = k;
if( u >= x[k-1] )
	i = k;
if( j > i + 1 )
	goto L_20;
/*  B���C�EH�E C��A�HA S(X) */
L_30:
dx = u - x[i-1];
seval_v = y[i-1] + dx*(b[i-1] + dx*(c[i-1] + dx*d[i-1]));
return( seval_v );
}

