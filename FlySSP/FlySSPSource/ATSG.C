/*------------------------------------------------------------------------
  ATSG.C
	   ������� ������� �� ������� ������ ����
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 18.09.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

#include "ssp.h"

/*     ..................................................................

	����ணࠬ�� (SUBROUTINE) atsg

	�����祭�� (PURPOSE)
		 ����ணࠬ�� �� ��������� ���祭�� ��㬥�� X �㭪樨
	   F(X) � ��������  ����⮭��� ⠡��� Fi � Xi ( i=1(1)m )
	   ���祭�� �⮩ �㭪樨 � �� ��㬥�� ��� ⠡��� Fi, F'i � Xi
	   ( i=1(1)m ) ���祭�� �⮩ �㭪樨, ee �ந������� � ��㬥��
	   �ନ��� ���ᨢ ���祭��  ��㬥��  Xj  �  ���ᨢ ���祭��
	   �㭪樨 F'j (��� ���ᨢ�  ���祭��  �㭪樨 Fj � ���祭�� ��
	   �ந������� F'j) ��������� ࠧ��� n (n<=m), 㯮�冷祭�� ��
	   �⥯��� ������� ���祭�� Xj, � ��������� ���祭�� X.
		  ��ନ�㥬�  ���ᨢ�  㯮�冷稢�����  ⠪�� ��ࠧ��,
	   �⮡�  ���㫨  ࠧ���⥩  �X-Xj�  �����⠫�  �  㢥��祭���
	   ������ j.
		  (NDIM POINTS OF A GIVEN GENERAL TABLE ARE SELECTED AND
	   ORDERED SUCH THAT
		     ABS(ARG(I)-X).GE.ABS(ARG(J)-X) IF I.GT.J.)

	���饭�� � ����ணࠬ�� (USAGE)
	   atsg(X,Z,F,WORK,IROW,ICOL,ARG,VAL,NDIM);

	���ᠭ�� ��ࠬ��஢ (DESCRIPTION OF PARAMETERS):
	   X      - �������� ���祭�� ��㬥�� (SEARCH ARGUMENT),
	   Z      - ���ᨢ ����� IROW ⠡����� ���祭�� ��㬥��,
		    �ᯮ�������� � ���浪� �����⠭�� ��� �뢠���
		    ��㬥��
		    (VECTOR OF ARGUMENT VALUES (DIMENSION IROW).
		    THE ARGUMENT VALUES MUST BE STORED IN INCREASING
		    OR DECREASING SEQUENCE),
	   F      - ���ᨢ ����� IROW �������� ���祭�� �㭪樨 (� ��砥
		    �᫨ ICOL=1) ��� ���ᨢ ࠧ��୮���� IROW*2 (� ��砥
		    �᫨ ICOL=2), ���� �⮫��� ���ண� ������� ���祭��
		    䠭�樨, � ��ன - ���祭�� �ந�������.
		      (IN CASE ICOL=1, F IS THE VECTOR OF FUNCTION
		    VALUES (DIMENSION IROW).
		       IN CASE ICOL=2, F IS A DOUBLE PRECISION IROW
		    BY 2 MATRIX. THE FIRST COLUMN SPECIFIES VECTOR
		    OF FUNCTION VALUES AND THE SECOND VECTOR OF
		    DERIVATIVES).
	   WORK   - ࠡ�稩 ���ᨢ ������ IROW
		    (WORKING STORAGE (DIMENSION IROW)).
	   IROW   - ������⢮ �������� ⠡����� ���祭�� �㭪樨
		    (THE DIMENSION OF EACH COLUMN IN MATRIX F).
	   ICOL   - �᫮ ��ப � ��ᨢ� F (�� ������୮ �࠭���� ������)
		    (THE NUMBER OF COLUMNS IN F (I.E. 1 OR 2)).
	   ARG    - �ନ�㥬� ����ணࠬ��� ���ᨢ ����� NDIM
		    㯮�冷祭��� ���祭�� ��㬥��
		    (RESULTING VECTOR OF SELECTED AND ORDERED
		    ARGUMENT VALUES (DIMENSION NDIM).
	   VAL    - �ନ�㥬� ����ணࠬ��� ���ᨢ ����� NDIM (ICOL=1)
		    ���祭�� �㭪樨 ��� NDIM*2 (ICOL=2) ���祭�� �㭪樨
		    � �� �ந�������.
		      ���祭�� �㭪樨 � �� �ந������� �ᯮ��������
		    ��ନ, ������ ���祭�� �㭪樨 �����।�⢥���
		    �।����� ᮮ⢥�����饬� ���祭�� �ந�������.
		      (RESULTING VECTOR OF SELECTED FUNCTION VALUES
		    (DIMENSION NDIM) IN CASE ICOL=1.
		       IN CASE ICOL=2, VAL IS THE VECTOR OF FUNCTION
		    AND DERIVATIVE VALUES (DIMENSION 2*NDIM) WHICH ARE
		    STORED IN PAIRS (I.E. EACH FUNCTION VALUE IS FOLLOWED
		    BY ITS DERIVATIVE VALUE).
	   NDIM   - �������� �᫮ ������⮢, �롨ࠥ��� �� 㯮�冷稢����
		    �������� ���ᨢ�� Z � F.
		     (THE NUMBER OF POINTS WHICH MUST BE SELECTED OUT OF
		    THE GIVEN TABLE).

	�ਬ�砭�� (REMARKS)
	   ����� �������� IROW ����� 祬 1
	   (NO ACTION IN CASE IROW LESS THAN 1).

	   IF INPUT VALUE NDIM IS GREATER THAN IROW, THE PROGRAM
	   SELECTS ONLY A MAXIMUM TABLE OF IROW POINTS.  THEREFORE THE
	   USER OUGHT TO CHECK CORRESPONDENCE BETWEEN TABLE (ARG,VAL)
	   AND ITS DIMENSION BY COMPARISON OF NDIM AND IROW, IN ORDER
	   TO GET CORRECT RESULTS IN FURTHER WORK WITH TABLE (ARG,VAL).
	   THIS TEST MAY BE DONE BEFORE OR AFTER CALLING

	   ����ணࠬ�� (SUBROUTINE) ATSG.
		  ����ணࠬ��  ����� �ਬ������� ��� 㯮�冷祭�� ���-
	   ᨢ��, �ᯮ��㥬�� ����ணࠬ���� ALI, AHI, ASFI.
		  (SUBROUTINE ATSG ESPECIALLY CAN BE USED FOR GENERATING
	   THE TABLE (ARG,VAL) NEEDED IN SUBROUTINES ALI, AHI, AND ACFI.)

	METHOD
           SELECTION IS DONE BY GENERATING THE VECTOR WORK WITH
           COMPONENTS WORK(I)=ABS(Z(I)-X) AND AT EACH OF THE NDIM STEPS
           (OR IROW STEPS IF NDIM IS GREATER THAN IROW)
           SEARCHING FOR THE SUBSCRIPT OF THE SMALLEST COMPONENT, WHICH
           IS AFTERWARDS REPLACED BY A NUMBER GREATER THAN
           MAX(WORK(I)).

     ..................................................................
 */
void atsg(double x, double z[], double f[],
	  double work[], int irow, int icol,
	  double arg[], double val[], int ndim)
{
int    i, ii, iii, j, n;
double b, delta;

switch( for_if(irow) ){
	case -1: goto L_11;
	case  0: goto L_11;
	case  1: goto L_1;
	}
L_1:
n = ndim;
/*     IF N IS GREATER THAN IROW, N IS SET EQUAL TO IROW. */
switch( for_if(n - irow) ){
	case -1: goto L_3;
	case  0: goto L_3;
	case  1: goto L_2;
	}
L_2:
n = irow;

/*     GENERATION OF VECTOR WORK AND COMPUTATION OF ITS GREATEST ELEMENT. */
L_3:
b = 0.e0;
for( i = 1; i <= irow; i++ ){
	delta = fabs( z[i-1] - x );
	switch( for_if(delta - b) ){
		case -1: goto L_5;
		case  0: goto L_5;
		case  1: goto L_4;
		}
L_4:
	b = delta;
L_5:
	work[i-1] = delta;
	}

/*     GENERATION OF TABLE (ARG,VAL) */
b = b + 1.e0;
for( j = 1; j <= n; j++ ){
	delta = b;
	for( i = 1; i <= irow; i++ ){
		switch( for_if(work[i-1] - delta) ){
			case -1: goto L_6;
			case  0: goto L_7;
			case  1: goto L_7;
			}
L_6:
		ii = i;
		delta = work[i-1];
L_7:
		;
		}
	arg[j-1] = z[ii-1];
	switch( for_if(icol - 1) ){
		case -1: goto L_8;
		case  0: goto L_9;
		case  1: goto L_8;
		}
L_8:
	val[2*j - 1-1] = f[ii-1];
	iii = ii + irow;
	val[2*j-1] = f[iii-1];
	goto L_10;
L_9:
	val[j-1] = f[ii-1];
L_10:
	work[ii-1] = b;
	}
L_11:
return;
}

