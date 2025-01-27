/*----------------------------------------------------------------------
    MSTR.C
    ���e�e��e c�oco�a xpa�e��� �����⭮� �a�p���
  ����	 : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  ����� : 01.00 / 31.05.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	����ணࠬ��� mstr

	�����祭��
	   ���e�e��e c�oco�a xpa�e��� �����⭮� �a�p���

	���饭�� � ����ணࠬ��
		   mstr(a,r,n,msa,msr);


	�室�� � ��室�� ��ࠬ����:
	   a - ���ᨢ ��室��� ������,
	   r - ���ᨢ, ᮤ�ঠ騩 १���� ��������� ᯮᮡ� �࠭����,
	   n - �᫮ ��ப � �⮫�殢 ����� a � r
	   msa - �᫮, ��।����饥 ��� �࠭���� ������ a
		  0 - GENERAL    (��騩 n*n - ������⮢)
		  1 - SYMMETRIC  (ᨬ������ (n+1)*n/2 -������⮢)
		  2 - DIAGONAL   (���������� n-������⮢)
	   msr - �᫮, ��।����饥 ��� �࠭���� ������ r (�������筮 a)


	����ணࠬ�� �ᯮ���� �㭪�� ������⥪� ssp.lib :
		  loc

	��⮤
	   ����� � �८�ࠧ���� � ������ r
	    msa msr
	     0   0  ����� A ��������� � ������ R
	     0   1  THE UPPER TRIANGLE ELEMENTS OF A GENERAL MATRIX
		    ARE USED TO FORM A SYMMETRIC MATRIX
	     0   2  THE DIAGONAL ELEMENTS OF A GENERAL MATRIX ARE USED
		    TO FORM A DIAGONAL MATRIX
	     1   0  A SYMMETRIC MATRIX IS EXPANDED TO FORM A GENERAL
		    MATRIX
	     1   1  MATRIX A IS MOVED TO MATRIX R
	     1   2  THE DIAGONAL ELEMENTS OF A SYMMETRIC MATRIX ARE
		    USED TO FORM A DIAGONAL MATRIX
	     2   0  A DIAGONAL MATRIX IS EXPANDED BY INSERTING MISSING
		    ZERO ELEMENTS TO FORM A GENERAL MATRIX
	     2   1  A DIAGONAL MATRIX IS EXPANDED BY INSERTING MISSING
		    ZERO ELEMENTS TO FORM A SYMMETRIC MATRIX
	     2   2  MATRIX A IS MOVED TO MATRIX R
//����
#include <ssp.h>
void main(void)
{
double r[36],s[36];
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,GENERAL,SYMMETRIC);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,GENERAL,GENERAL);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,GENERAL,DIAGONAL);

for(i=0;i<21;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,SYMMETRIC,SYMMETRIC);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,SYMMETRIC,GENERAL);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,SYMMETRIC,DIAGONAL);

for(i=0;i<6;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,DIAGONAL,SYMMETRIC);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,DIAGONAL,GENERAL);
for(i=0;i<36;i++)
  {r[i]=i;s[i]=0.0;}
mstr(r,s,6,DIAGONAL,DIAGONAL);
return;
}
     ..................................................................
 */
void  mstr(double a[], double r[], int n, int msa, int msr)
{
int i, ia, ir, j;

for( i = 0; i < n; i++ ){
	for( j = 0; j < n; j++ ){
		/*        IF R IS GENERAL, FORM ELEMENT  */
		if(msr==0)
		  goto L_10;
		/* IF IN LOWER TRIANGLE OF SYMMETRIC OR DIAGONAL R, BYPASS */
		if( (i - j) > 0)  continue;
L_10:
		loc( i, j, &ir, n, n, msr );

		/* IF IN UPPER AND OFF DIAGONAL  OF DIAGONAL R, BYPASS */
		if( ir < 0)
		  continue;
		/*        OTHERWISE, FORM R(I,J) */
		r[ir] = 0.0;
		loc( i, j, &ia, n, n, msa );

		/*        IF THERE IS NO A(I,J), LEAVE R(I,J) AT 0.0 */
		if( ia < 0)  continue;
		r[ir] = a[ia];
		}
	}
return;
}
