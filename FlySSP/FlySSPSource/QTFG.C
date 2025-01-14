/*------------------------------------------------------------------------
	  QTFG.C
	  �������������� �������, �������� �������� � ����������������
	  ������, �� ������� ��������
  ����   : ��ॢ�� �ணࠬ�� ����� SSP �� � �믮���� ���뭪�� �.�.
  �����  : 01.00 / 26.04.1997 /
------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "ssp.h"

/*     ..................................................................

	������������ QTFG
	SUBROUTINE QTFG

	����������
	   ����ணࠬ�� ������ ������⢮ z(1),...,z(n) ���祭��
	   ��⥣ࠫ��
				x(i)
				 �
		  z(i)=z[x(i)]=  �  y(x) dx
				 �
			       x(1)
	   �� �㭪樨 y(x), �������� ������⢠�� x(1),...,x(n)
	   㯮�冷祭��� �� �����⠭�� ��� �뢠��� ��㬥�� �
	   ᮮ⢥������� ���祭�� �㭪樨.
	PURPOSE
	   TO COMPUTE THE VECTOR OF INTEGRAL VALUES FOR A GIVEN
	   GENERAL TABLE OF ARGUMENT AND FUNCTION VALUES.

	���饭�� � ����ணࠬ��
	USAGE
	   qtfg(x,y,z,ndim);

	DESCRIPTION OF PARAMETERS
	   x      - ���ᨢ ������� ndim �������� ���祭�� ��㬥��.
		    THE INPUT VECTOR OF ARGUMENT VALUES.
	   y      - ���ᨢ ������� ndim �������� ���祭�� �㭪樨.
		    THE INPUT VECTOR OF FUNCTION VALUES.
	   z      - ���ᨢ ������� ndim ���᫥���� ���祭�� ��⥣ࠫ��,
		    ����� ᮢ������ � x ��� � �.
		    THE RESULTING VECTOR OF INTEGRAL VALUES. z MAY BE
		    IDENTICAL WITH x OR y.
	   ndim   - �᫮ �祪, � ������ ������ ��⥣��㥬�� �㭪��;
		    �� ndim<1 �ந�室�� ��室 �� �ணࠬ��
		    THE DIMENSION OF VECTORS x,y,z.

	REMARKS
	   NO ACTION IN CASE NDIM LESS THAN 1.

        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
           NONE

        METHOD
           BEGINNING WITH Z(1)=0, EVALUATION OF VECTOR Z IS DONE BY
           MEANS OF TRAPEZOIDAL RULE (SECOND ORDER FORMULA).
           FOR REFERENCE, SEE
	   F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
	   MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.75.

     ..................................................................
 */
void  qtfg(double x[], double y[], double z[], int ndim)
{
int     i;
double  sum1, sum2;
sum2 = 0.0;
if(ndim<=0)
  return;
if(ndim==1)
  {
  z[0] = sum2;
  return;
  }

/*     INTEGRATION LOOP */
for( i = 1; i < ndim; i++ ){
	sum1 = sum2;
	sum2 = sum2 + 0.5*(x[i] - x[i - 1])*(y[i] + y[i - 1]);
	z[i - 1] = sum1;
	}
z[ndim-1] = sum2;
return;
}

