//---------------------------------------------------------
//	NDTR.C
//      ������� ����������� �������������
//	����	: ���뭪�� �.�.
//	����� 	: 01.00 / 25.09.1996 /
//---------------------------------------------------------
#include <math.h>
#include "ssp.h"

/*.......................................................................

	������������ NDTR
	PURPOSE
	   COMPUTES Y = P(X) = PROBABILITY THAT THE RANDOM VARIABLE  U,
           DISTRIBUTED NORMALLY(0,1), IS LESS THAN OR EQUAL TO X.
           F(X), THE ORDINATE OF THE NORMAL DENSITY AT X, IS ALSO
	   COMPUTED.

	USAGE
           CALL NDTR(X,P,D)

        DESCRIPTION OF PARAMETERS
	   X--INPUT SCALAR FOR WHICH P(X) IS COMPUTED.
           P--OUTPUT PROBABILITY.
           D--OUTPUT DENSITY.

        REMARKS
           MAXIMUM ERROR IS 0.0000007.

	SUBROUTINES AND SUBPROGRAMS REQUIRED
	   NONE

	METHOD
           BASED ON APPROXIMATIONS IN C. HASTINGS, APPROXIMATIONS FOR
           DIGITAL COMPUTERS, PRINCETON UNIV. PRESS, PRINCETON, N.J.,
           1955.  SEE EQUATION 26.2.17, HANDBOOK OF MATHEMATICAL
           FUNCTIONS, ABRAMOWITZ AND STEGUN, DOVER PUBLICATIONS, INC.,
	   NEW YORK.

.......................................................................
*/
/*
     ����� �㭪樨 ��ଠ�쭮�� ��।������ �u(U) [- ���., U].
���� � ��⥣ࠫ�� ����⭮�⥩ �(U) [0,U] � �㭪樥� �訡�� erf(z) [0,z]:

		U
	  1     �   -t�/2      1      U           1
P=�(U)= ������� �  e      dt = � erf(��) = �u(U)- �
	�(2*pi) �              2     �2           2
		0
����
	 U   �(U)       �u(U)
	 0.0  0.0        0.5
	 0.5  0.19146    0.69146
	 2.9  0.49813    0.99813
�.� १����
     p=ndtr(U,&d)-0.5;
ࠢ�� १�����
     p=pnormn(U);
*/
float  ndtr(float x, float *d)
{
float p, ax, t;
ax = (float)fabs(x);
t = 1.0f/(1.0f + .2316419f*ax);
(*d) = (float)(0.3989423f*exp( -x*x/2.0f ));
p = 1.0f - (*d)*t*((((1.330274f*t - 1.821256f)*t + 1.781478f)*t - 0.3565638f)*
 t + 0.3193815f);
if(x<0)
  p = 1.0f - p;
return p;
}

