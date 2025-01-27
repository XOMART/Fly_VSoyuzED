/*------------------------------------------------------------------------
	  FORIT.C
	  ���������� ������������� ����� ������������� �������,
	  �������� �������� ��������
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	������������ (SUBROUTINE) forit

	����������
	   ����ணࠬ�� ������ �����樥��� A(0),A(k),B(k) (k=1,2,...,M)
	   �鸞 ����:
	       F(X)=A(0)+SUM(A(K)COS KX+B(K)SIN KX)
	   �������� �� ���ࢠ�� [0,2*M_PI] �� ࠢ�������饩 ��⥬�
	   2N+1 㧫��
		    2*M_PI
	       Xt= --------*t, t=0,1,...,2N.
		    2*N+1
	PURPOSE
	   FOURIER ANALYSIS OF A PERIODICALLY TABULATED FUNCTION.
	   COMPUTES THE COEFFICIENTS OF THE DESIRED NUMBER OF TERMS
	   IN THE FOURIER SERIES F(X)=A(0)+SUM(A(K)COS KX+B(K)SIN KX)
	   WHERE K=1,2,...,M TO APPROXIMATE A GIVEN SET OF
	   PERIODICALLY TABULATED VALUES OF A FUNCTION.

	���饭�� � ����ணࠬ�� (USAGE)
	   ier=forif(fnt,n,m,a,b);

	DESCRIPTION OF PARAMETERS
	   FNT-���ᨢ ������ 2N+1 �������� ���祭�� �㭪樨
	       VECTOR OF TABULATED FUNCTION VALUES OF LENGTH 2N+1
	   n  -�᫮, ��।����饥 ������ࢠ� ⠪�� ��ࠧ��, �� 2N+1
	       �窠 ᮤ�ন��� �� ���ࢠ�� [0,2*M_PI];
	       DEFINES THE INTERVAL SUCH THAT 2N+1 POINTS ARE TAKEN
	       OVER THE INTERVAL (0,2PI). THE SPACING IS THUS 2PI/2N+1
	   m  -���ᨬ���� ���冷� ���பᨬ������ ��ମ���;
	       MAXIMUM ORDER OF HARMONICS TO BE FITTED
	   a  -���ᨢ ������ m+1 ���᫥���� �����樥�⮢ ���� ��
	       ��ᨭ��� a[0],a[1],...,a[m]
	       RESULTANT VECTOR OF FOURIER COSINE COEFFICIENTS OF
	       LENGTH M+1
	       A SUB 0, A SUB 1,..., A SUB M
	   b  -���ᨢ ������ m+1 ���᫥���� �����樥�⮢ ���� ��
	       ᨭ��� b[0]=0,b[1],...,b[m]
	       RESULTANT VECTOR OF FOURIER SINE COEFFICIENTS OF
	       LENGTH M+1
	       B SUB 0, B SUB 1,..., B SUB M
	   ����ணࠬ�� �����頥�
	   ier-�������� �訡��, �ਭ����騩 �� �믮������ �ணࠬ��
	       ᫥���騥 ���祭��:
	       0 - �訡�� ���, 
	       1 - (1-n) <= m,
	       2 - (2-m) < 0,
	       RESULTANT ERROR CODE WHERE
	       IER=0  NO ERROR
	       IER=1  N NOT GREATER OR EQUAL TO M
	       IER=2  M LESS THAN 0

        REMARKS
           M MUST BE GREATER THAN OR EQUAL TO ZERO
           N MUST BE GREATER THAN OR EQUAL TO M
           THE FIRST ELEMENT OF VECTOR B IS ZERO IN ALL CASES

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           USES RECURSIVE TECHNIQUE DESCRIBED IN A. RALSTON, H. WILF,
	   'MATHEMATICAL METHODS FOR DIGITAL COMPUTERS', JOHN WILEY
           AND SONS, NEW YORK, 1960, CHAPTER 24. THE METHOD OF INDEXING
           THROUGH THE PROCEDURE HAS BEEN MODIFIED TO SIMPLIFY THE
           COMPUTATION.

     ..................................................................
 */
int forit(double fnt[], int n, int m, double a[], double b[])
{
int    i, j, ier;
double an, c, c1, coef, const_, fntz, q, s, s1, u0, u1, u2;


/*        CHECK FOR PARAMETER ERRORS  */
ier = 0;
// L_20:
if(m<0)
  goto L_30;
  else goto L_40;
L_30:
ier = 2;
return ier;
L_40:
if( (m-n)<=0)
  goto L_60;
  else goto L_50;
L_50:
ier = 1;
return ier;

/*        COMPUTE AND PRESET CONSTANTS  */
L_60:
an = n;
coef = 2.0/(2.0*an + 1.0);
const_ = M_PI*coef;
s1 = sin( const_ );
c1 = cos( const_ );
c = 1.0;
s = 0.0;
j = 1;
fntz = fnt[0];
L_70:
u2 = 0.0;
u1 = 0.0;
i = 2*n + 1;

/*        FORM FOURIER COEFFICIENTS RECURSIVELY  */
L_75:
u0 = fnt[i-1] + 2.0*c*u1 - u2;
u2 = u1;
u1 = u0;
i = i - 1;
if( (i-1)<=0)
  goto L_80;
  else goto L_75;
L_80:
a[j-1] = coef*(fntz + c*u1 - u2);
b[j-1] = coef*s*u1;
if( (j - (m + 1)) < 0)
  goto L_90;
  else goto L_100;
L_90:
q = c1*c - s1*s;
s = c1*s + s1*c;
c = q;
j = j + 1;
goto L_70;
L_100:
a[0] = a[0]*0.5;
return ier;
}


