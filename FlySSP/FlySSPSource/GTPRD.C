/*------------------------------------------------------------------------
  GTPRD.C
    ��������� ������������������ ������� �� ������ ������� ������ ����
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 10.08.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	����ணࠬ�� gtprd

	�����祭��
	   PREMULTIPLY A GENERAL MATRIX BY THE TRANSPOSE OF ANOTHER
	   GENERAL MATRIX

	USAGE
	   gtprd(A,B,R,N,M,L)

	DESCRIPTION OF PARAMETERS
	   A - NAME OF FIRST INPUT MATRIX
	   B - NAME OF SECOND INPUT MATRIX
	   R - NAME OF OUTPUT MATRIX
	   N - NUMBER OF ROWS IN A AND B
	   M - NUMBER OF COLUMNS IN A AND ROWS IN R
	   L - NUMBER OF COLUMNS IN B AND R

	REMARKS
	   MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX A
	   MATRIX R CANNOT BE IN THE SAME LOCATION AS MATRIX B
	   ALL MATRICES MUST BE STORED AS GENERAL MATRICES

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   MATRIX TRANSPOSE OF A IS NOT ACTUALLY CALCULATED. INSTEAD,
	   ELEMENTS OF MATRIX A ARE TAKEN COLUMNWISE RATHER THAN
	   ROWWISE FOR POSTMULTIPLICATION BY MATRIX B.

     ..................................................................
 */
void  gtprd(double a[], double b[], double r[], int n, int m, int l)
{
int i,j,k;
/*                                        �
     �ந�������� ����� ��饣� ���� R = A * B
     ��� - a[m*n] ����� ��饣� ���� ,
	 - b[m*l] ����� ��饣� ���� ,
	 - r[n*l] ����� ��饣� ���� */
if(!a || !b || !r)
  return ;
if(l<=0 || m<=0 || n<=0 )
  return ;
for ( i=0;i<n;i++)
  for ( k=0;k<l;k++)
     {
     for (*(r+(i*l)+k)=0, j=0;j<m;j++)
       *(r+(i*l)+k) +=( *(a+(j*n)+i) * *(b+(j*l)+k) );
     }
return;
}
/*
int i, ib, ij, ik, ir, j, k;


ir = 0;
ik = -n;
for( k = 1; k <= l; k++ ){
	ij = 0;
	ik = ik + n;
	for( j = 1; j <= m; j++ ){
		ib = ik;
		ir = ir + 1;
		r[ir-1] = 0;
		for( i = 1; i <= n; i++ ){
			ij = ij + 1;
			ib = ib + 1;
			r[ir-1] = r[ir-1] + a[ij-1]*b[ib-1];
			}
		}
	}
*/
