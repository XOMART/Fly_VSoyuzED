/*------------------------------------------------------------------------
  SG13.C
    ����������� �������, �������� �������� ��������
    � ��������������� ������,  � ������� ����������
    ������ �������, ������������ �� ���� ���������-
    ������� ������ ������� ���������� ���������
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include "ssp.h"
/*     ..................................................................

	����ணࠬ�� (SUBROUTINE) sg13

	�����祭�� (PURPOSE)
		 ����ணࠬ��  ������  ������⢮ Z1, Z2, ..., Zn
	   ���祭�� �㭪樨 Y(X), �������� ������⢠�� X1,X2,...,Xn
	   ���祭��  ��㬥��  �  Y1, Y2, ..., Yn  ᮮ⢥�������
	   ���祭�� �㭪樨.
		 �� ����� �६ ��᫥����⥫�� �窠� (Xj-2,Yj-2),
	   (Xj-1,Yj-1), (Xj,Yj) ��� j = 3,...,n ��ந��� ��᫥����-
	   ⥫쭮��� �����童��� ��ࢮ� �⥯��� ����
				 Wj(X)= Aj*X +Bj
	   ����� � ��� �窠� �������襥 �⪫������ �� Y � ��᫥
	   ���.
		 ��� i = 2, ... , n-1, � ���� � ������ �窥 Xi ��
	   �᪫�祭��� �������  �祪 X1 � Xn, � ����⢥ ᣫ����-
	   ���� ���祭�� �㭪樨  Zi ������ ���᫥���� � �窥 Xi
	   ���祭�� �����童�� Wi+1(X), � ����⢥ Z1 - ���᫥����
	   � �窥 X1 ���祭�� �����童�� W3(X), �  � ����⢥ Zn -
	   ���祭�� �����童�� Wn(X), ���᫥���� �� X=Xn.
	   (TO COMPUTE A VECTOR OF SMOOTHED FUNCTION VALUES GIVEN
	   VECTORS  OF ARGUMENT VALUES AND CORRESPONDING FUNCTION
	   VALUES.)

	���饭�� � ����ணࠬ�� (USAGE)
	   IER=sg13(X,Y,Z,NDIM);

	���ᠭ�� ��ࠬ��஢ (DESCRIPTION OF PARAMETERS):
	   X     -  ���ᨢ ����� NDIM �������� ���祭�� ��㬥��
		    (GIVEN VECTOR OF ARGUMENT VALUES (DIMENSION NDIM),
	   Y     -  ���ᨢ ����� NDIM �������� ���祭�� ��㬥��
		    (GIVEN VECTOR OF FUNCTION VALUES CORRESPONDING TO X
		    (DIMENSION NDIM),
	   Z     -  ���ᨢ ����� NDIM ����塞�� ᣫ������� ���祭��
		    �㭪樨
		    (RESULTING VECTOR OF SMOOTHED FUNCTION VALUES
		    (DIMENSION NDIM),
	   NDIM  -  ������⢮ �������� ���祭�� �㭪樨 � ��㬥��
		    (DIMENSION OF VECTORS X, Y AND Z).
	   �㭪�� �����頥� �������� �訡�� (RESULTING ERROR PARAMETER)
	IER, �ਭ����騩 ���� �� ᫥����� ���祭��:
		    IER =  0, �᫨ �訡�� ��� (NO ERROR);
		    IER = -1, �᫨ NDIM ����� 3
			     (NDIM IS LESS THAN 3).

	�ਬ�砭�� (REMARKS)
	   (1)   �᫨ IER = -1, � ���᫥��� �� �஢�������.
		 (IF IER=-1 THERE HAS BEEN NO COMPUTATION.)
	   (2)   ���ᨢ Z ����� ᮢ������ � X ��� Y. �᫨ X
		 ��� Y �� ᮢ������ � Z, � ��� �� ��������
		 � ����� ���᫥���.
		 (Z CAN HAVE THE SAME STORAGE ALLOCATION AS Y.
		 IF Y IS DISTINCT FROM Z, THEN IT IS NOT
		 DESTROYED.).

	SUBROUTINES AND SUBPROGRAMS REQUIRED
	   NONE

	METHOD
	   IF X IS THE (SUPPRESSED) VECTOR OF ARGUMENT VALUES, THEN
	   EXCEPT AT THE ENDPOINTS X(1) AND X(NDIM), EACH SMOOTHED
	   VALUE Z(I) IS OBTAINED BY EVALUATING AT X(I) THE LEAST-
	   SQUARES POLYNOMIAL OF DEGREE 1 RELEVANT TO THE 3 SUCCESSIVE
	   POINTS (X(I+K),Y(I+K)) K = -1,0,1.  (SEE HILDEBRAND, F.B.,
	   INTRODUCTION TO NUMERICAL ANALYSIS, MC GRAW-HILL, NEW YORK/
	   TORONTO/LONDON, 1956, PP. 295-302.)

     ..................................................................
 */
int sg13(double x[], double y[], double z[], int ndim)
{
int     i, ier;
double  h, t1, t2, t3, xm, ym;
double const ci3=1.0/3.0;/* 0.3333333 */

/*        TEST OF DIMENSION */
ier = -1;
if( (ndim - 3) < 0 )
  return ier; /*        ERROR EXIT IN CASE NDIM IS LESS THAN 3 */

/*        START LOOP */
for( i = 2; i < ndim; i++ ){
	xm = ci3*(x[i - 2] + x[i - 1] + x[i]);
	ym = ci3*(y[i - 2] + y[i - 1] + y[i]);
	t1 = x[i - 2] - xm;
	t2 = x[i - 1] - xm;
	t3 = x[i] - xm;
	xm = t1*t1 + t2*t2 + t3*t3;
	if( xm > 0.0)
	   xm = (t1*(y[i - 2] - ym)
	      +  t2*(y[i - 1] - ym)
	      +  t3*(y[i]     - ym))/xm;
	/*        CHECK FIRST POINT */
	if( i <= 2 )
	   h = xm*t1 + ym;
	z[i - 2] = h;
	h = xm*t2 + ym;
	}
/*        END OF LOOP
	UPDATE LAST TWO COMPONENTS */
z[ndim - 2] = h;
z[ndim - 1] = xm*t3 + ym;
ier = 0;
return ier;
}

