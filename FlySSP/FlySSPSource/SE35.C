/*------------------------------------------------------------------------
  SE35.C
    ����������� �������, �������� �������� ��������
    �  �������������  ������, �  ������� ����������
    ������� �������, ������������ �� ���� ���������-
    ������� ������ ������� ���������� ���������
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"

/*     ..................................................................

	����ணࠬ�� (SUBROUTINE) se35

	�����祭�� (PURPOSE)
		 ����ணࠬ��  ������  ������⢮ Z1, Z2, ..., Zn
	   ���祭�� �㭪樨  Y(X), �������� ������⢮� Y1,Y2,...,Yn
	   �� ���祭�� � N ࠢ��������� �窠� Xi c 蠣�� Xi-Xi-1
	   (i=2,...,n).
		 �� ����� ��� ��᫥����⥫�� �窠� (Xj-2,Yj-2),
	   (Xj-1,Yj-1), (Xj,Yj)  ��� j = 5,...,n ��ந��� ��᫥����-
	   ⥫쭮���  �����童��� ���쥩 �⥯��� ����
		   Wj(X)= Aj*X**3 + Bj*X**2 + Cj*X + Dj
	   ����� � ��� �窠� �������襥 �⪫������ �� Y � ��᫥
	   ���.
		 ��� i=3, ... , n-2, � ����⢥ ᣫ�������� ���祭��
	   �㭪樨 Zi ������ ���᫥���� � �窥 Xi ���祭�� �����-
	   童��, ����஥����� �� ��� �窠� (Xi+k,Yi+k) (k=-2,...,2),
	   �.�. �����童�� Wi+2(X). � ����⢥ ᣫ������� ���祭�� Z1
	   � Z2 ������� ���᫥��� � �窠� X1 � X2 ���祭�� �����-
	   童�� W5(X), � � ����⢥ Zn-1 � Zn - ���祭�� �����童��
	   Wn(X), ���᫥��� � �窠� Xn-1 � Xn.
		 (TO COMPUTE A VECTOR OF SMOOTHED FUNCTION VALUES GIVEN
	   A VECTOR OF FUNCTION VALUES WHOSE ENTRIES CORRESPOND TO
	   EQUIDISTANTLY SPACED ARGUMENT VALUES.)

	���饭�� � ����ணࠬ�� (USAGE)
	   IER=se35(Y,Z,NDIM);

	���ᠭ�� ��ࠬ��஢ (DESCRIPTION OF PARAMETERS):
	   Y     -  ���ᨢ ����� NDIM �������� ���祭�� �㭪樨
		    (GIVEN VECTOR OF FUNCTION VALUES (DIMENSION NDIM),
	   Z     -  ���ᨢ ����� NDIM ����塞�� ᣫ������� ���祭��
		    �㭪樨
		    (RESULTING VECTOR OF SMOOTHED FUNCTION VALUES
		    (DIMENSION NDIM),
	   NDIM  -  ������⢮ �������� ���祭�� �㭪樨 � ��㬥��
		    (DIMENSION OF VECTORS Y AND Z)
	   �㭪�� �����頥� �������� �訡�� (RESULTING ERROR PARAMETER)
	IER, �ਭ����騩 ���� �� ᫥����� ���祭��:
		    IER =  0, �᫨ �訡�� ��� (NO ERROR);
		    IER = -1, �᫨ NDIM ����� 5
			     (NDIM IS LESS THAN 5).
	�ਬ�砭�� (REMARKS)
	   (1)   �᫨ IER = -1, � ���᫥��� �� �஢�������.
		 (IF IER=-1 THERE HAS BEEN NO COMPUTATION.)
	   (2)   ���ᨢ Z ����� ᮢ������ � Y. �᫨ Y �� ᮢ������
		 � Z, � �� �� �������� � ����� ���᫥���.
		 (Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.  IF Y
		 IS DISTINCT FROM Z, THEN IT IS NOT DESTROYED.)

	SUBROUTINE AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
           EXCEPT AT THE POINTS X(1),X(2),X(NDIM-1) AND X(NDIM), EACH
           SMOOTHED VALUE Z(I) IS OBTAINED BY EVALUATING AT X(I) THE
           LEAST-SQUARES POLYNOMIAL OF DEGREE 3 RELEVANT TO THE 5
	   SUCCESSIVE POINTS (X(I+K),Y(I+K)) K = -2,-1,...,2.  (SEE
	   HILDEBRAND, F.B., INTRODUCTION TO NUMERICAL ANALYSIS,
	   MC GRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP. 295-302.)

     ..................................................................
 */
int se35(double y[], double z[], int ndim)
{
int  ier,i;
double a, b, c, d;
double const c1=0.014285714285714286,
	     c2=0.057142857142857143,
	     c3=0.08571428571428571;

ier=-1;
/*        TEST OF DIMENSION */
if( (ndim - 5) < 0 )
  return ier; /*        ERROR EXIT IN CASE NDIM IS LESS THAN 5 */

/*        PREPARE LOOP */
b = y[0];      c = y[1];
/*        START LOOP */
for( i = 4; i < ndim; i++ ){
	a = b;
	b = c;
	c = y[i - 2];
	/*        GENERATE FOURTH CENTRAL DIFFERENCE */
	d = c - b - y[i - 1];
	d = d + d + c;
	d = d + d + a + y[i];
	/*        CHECK FIRST TWO COMPONENTS */
	if( i <= 4 )
	  {
	  z[0] = a - c1*d;
	  z[1] = b + c2*d;
	  }
	z[i - 2] = c - c3*d;
	}
/*        END OF LOOP
	UPDATE LAST TWO COMPONENTS */
z[ndim - 2] = y[ndim - 2] + (c2*d);
z[ndim - 1] = y[ndim - 1] - (c1*d);
ier = 0;
return ier;
}
