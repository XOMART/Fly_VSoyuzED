//------------------------------------------------------------------------
// ALI.C
// ���������������� ������� � ������� ����������������� ��������
// �������-��������
// �����   : ������� ��������� ������ SSP �� � �������� �������� �.�.
// ������  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include <math.h>
#include "ssp.h"

double INF_SUP(const double X1, const double X2, const double X3);

//------------------------------------------------------------------------
// ������������ (SUBROUTINE) ALI
// 
// ����������(PURPOSE)
// ��������� �������� Y ����������� ������� Y=Y(X) ��� ����������
// �������� X �� ��������� �� ������� Xi, Yi (i=1,2,...,N)
// ��������������� �������� ��������� � �������.
// TO INTERPOLATE FUNCTION VALUE Y FOR A GIVEN ARGUMENT VALUE
// X USING A GIVEN TABLE (ARG,VAL) OF ARGUMENT AND FUNCTION
// VALUES.
// 
// ��������� � ������������ (USAGE)
// IER=ali(X,ARG,VAL,Y,NDIM,EPS);
// 
// ������� � �������� ��������� (DESCRIPTION OF PARAMETERS)
// X      - �������� �������� ���������, ��� �������� �����������
// ����������������� �������� �������,
// DOUBLE PRECISION ARGUMENT VALUE SPECIFIED BY INPUT.
// ARG    - ������ ������ NDIM �������� ��������� ��������
// ���������. � �������� ���������� ���� ������ ��
// ��������.
// DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
// ARGUMENT VALUES OF THE TABLE (NOT DESTROYED).
// VAL    - ������ ������ NDIM �������� ��������� ��������
// �������. � �������� ���������� ���� ������ ��������.
// DOUBLE PRECISION INPUT VECTOR (DIMENSION NDIM) OF
// FUNCTION VALUES OF THE TABLE (DESTROYED).
// Y      - ����������� ����������������� �������� �������.
// RESULTING INTERPOLATED DOUBLE PRECISION FUNCTION
// VALUE.
// NDIM   - ����� �������� �������� �������� ��������� � �������;
// ���� NDIM<1, �� �������������� ����� �� ������������
// ��� ����������.
// AN INPUT VALUE WHICH SPECIFIES THE NUMBER OF
// POINTS IN TABLE (ARG,VAL).
// EPS    - �������� ������� ������� ���������� �����������
// ������������.
// SINGLE PRECISION INPUT CONSTANT WHICH IS USED AS
// UPPER BOUND FOR THE ABSOLUTE ERROR.
// FOR THE ABSOLUTE ERROR.
// ������������ ���������� ��������� ������ IER
// (A RESULTING ERROR PARAMETER)
// ���������� (REMARKS)
// (1) TABLE (ARG,VAL) SHOULD REPRESENT A SINGLE-VALUED
// FUNCTION AND SHOULD BE STORED IN SUCH A WAY, THAT THE
// DISTANCES ABS(ARG(I)-X) INCREASE WITH INCREASING
// SUBSCRIPT I. TO GENERATE THIS ORDER IN TABLE (ARG,VAL),
// SUBROUTINES DATSG, DATSM OR DATSE COULD BE USED IN A
// PREVIOUS STAGE.
// (2) NO ACTION BESIDES ERROR MESSAGE IN CASE NDIM LESS
// THAN 1.
// (3) INTERPOLATION IS TERMINATED EITHER IF THE DIFFERENCE
// BETWEEN TWO SUCCESSIVE INTERPOLATED VALUES IS
// ABSOLUTELY LESS THAN TOLERANCE EPS, OR IF THE ABSOLUTE
// VALUE OF THIS DIFFERENCE STOPS DIMINISHING, OR AFTER
// (NDIM-1) STEPS. FURTHER IT IS TERMINATED IF THE
// PROCEDURE DISCOVERS TWO ARGUMENT VALUES IN VECTOR ARG
// WHICH ARE IDENTICAL. DEPENDENT ON THESE FOUR CASES,
// ERROR PARAMETER IER IS CODED IN THE FOLLOWING FORM
// IER=0 - ����������� �������� ��������, ������ ���.
// IT WAS POSSIBLE TO REACH THE REQUIRED
// ACCURACY (NO ERROR).
// IER=1 - �������� �������� �� ����������� ��-��
// ������ ����������.
// IT WAS IMPOSSIBLE TO REACH THE REQUIRED
// ACCURACY BECAUSE OF ROUNDING ERRORS.
// IER=2 - �������� �������� �� ����������� ��� �����������
// �������� �� ����� ���� ���������, ��� ��� ����
// �������� NDIM.
// IT WAS IMPOSSIBLE TO CHECK ACCURACY BECAUSE
// NDIM IS LESS THAN 3, OR THE REQUIRED ACCURACY
// COULD NOT BE REACHED BY MEANS OF THE GIVEN
// TABLE. NDIM SHOULD BE INCREASED.
// IER=3 - ������������ ���������� �� ���������� ��������
// �������� ��-�� ����������� ���������� ��������
// � ������� ���������.
// THE PROCEDURE DISCOVERED TWO ARGUMENT VALUES
// IN VECTOR ARG WHICH ARE IDENTICAL.
// 
// SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
// NONE
// 
// METHOD
// INTERPOLATION IS DONE BY MEANS OF AITKENS SCHEME OF
// LAGRANGE INTERPOLATION. ON RETURN Y CONTAINS AN INTERPOLATED
// FUNCTION VALUE AT POINT X, WHICH IS IN THE SENSE OF REMARK
// (3) OPTIMAL WITH RESPECT TO GIVEN TABLE. FOR REFERENCE, SEE
// F.B.HILDEBRAND, INTRODUCTION TO NUMERICAL ANALYSIS,
// MCGRAW-HILL, NEW YORK/TORONTO/LONDON, 1956, PP.49-50.
//------------------------------------------------------------------------
// x-�������� �������� ��������� ��� ������� ����������� �����������������
// �������� �������;
// arg-������ ������ ndim �������� ��������� �������� ���������; � ��������
// ���������� ���� ������ �� ��������;
// val-������ ������ ndim �������� ��������� �������� �������; � ��������
// ���������� ���� ������ ��������;
// y-����������� ����������������� �������� �������;
// ndim-����� �������� �������� �������� �������;���� ndim<1, ��������������
// ����� �� ������������ ��� ���������� ����������;
// eps-�������� ������� ������� ���������� ����������� ������������.
// ������������ ���������� ���� �� ��������� ��������
// 0-���� ����������� �������� ��� ��������� �������� ������������,
// ������ ���;
// 1-�������� �������� �� ����������� ��-�� ������ ����������;
// 2-�������� �������� �� ����������� ��� ����������� �������� �� �����
// ���� ���������, ��� ��� ���� �������� ndim;
// 3-������������ ���������� �� ���������� �������� �������� ��-�� ����-
// ������� ���� ���������� �������� � ������� arg.
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//����
int aliTest(void)
{
	double arg[] ={1.3  ,1.4   ,1.5   ,1.6},
		   val[]={0.934,0.9523,0.9661,0.9763},
           val1[]={0.934,0.9523,0.9661,0.9763},
           x=1.43,y;
	int i,ier,n=4;
	//�筮� ���祭��          0.9569
	//�।��������� १���� 0.95687155
	//��⮢� १����  ALI  0.95687155  eps=[1e-16,1e-5]  ier=2
	//��⮢� १����  ALI  0.95687155  eps=1e-4          ier=0
	ier=ali(x,arg,val,&y,n,1e-8f,0);for(i=0;i<4;val[i]=val1[i],i++);
	ier=ali(x,arg,val,&y,n,1e-4f,0);
	return ier;
}

//------------------------------------------------------------------------
int ali(double x, double arg[], double val[], double *y,
		int ndim, float eps,int n8)
{
	int    ier, i, iend, j, k;
	double delt1, delt2,a,b,c;
	double h;

	if(n8<1)  n8=(int)INF_SUP(2,n8,8);

	ier = 2;
	delt2 = 0.0;
	if( (ndim - 1) < 0) goto L_9;
	else
	if( (ndim - 1) == 0) goto L_7;

	//     START OF AITKEN-LOOP 
	for( j = 2 ; j <= ndim; j++ ){
		delt1 = delt2;
	    iend = j - 1;
		k=j-1;
	    for( i = 0 ; i < iend; i++ ){
			h = arg[i] - arg[j-1];
			if( h == 0.0) goto L_13;
			//a=(val[i]*(x - arg[j-1]) - val[j-1]*(x -arg[i]))/h;
			b=val[i]*(x - arg[k]);
			c=val[k]*(x - arg[i]);
			a=(b - c)/h;
			val[k] =a;
       }
		// delt2 = fabs( val[j-1] - val[iend-1] );
		delt2 = fabs( val[k] - val[iend-1] );
		if ((j - 2) <= 0) continue;
		if ((delt2-(double)eps)<=0.0) goto L_10;
		if (j >= n8) {
			if ((delt2 - delt1) < 0.0) continue;
			else goto L_11;
		}
    }
	//     END OF AITKEN-LOOP
L_7:
	j = ndim;
L_8:
	(*y) = val[j-1];
L_9:
	return ier;
	//     THERE IS SUFFICIENT ACCURACY WITHIN NDIM-1 ITERATION STEPS
L_10:
	ier = 0;
	goto L_8;
	//    TEST VALUE DELT2 STARTS OSCILLATING 
L_11:
	ier = 1;
L_12:
	j = iend;
	goto L_8;
	//    THERE ARE TWO IDENTICAL ARGUMENT VALUES IN VECTOR ARG 
L_13:
	ier = 3;
	goto L_12;
}

