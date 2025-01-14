//-----------------------------------------------------------------------
//   NORMX11.C
//   ������������ ���������� ��������� ���������������� �������
//   � ��������� [-1,1]
//   �����  : �������� �.�.
//   ������  : 01.00 / 10.08.1997 /
//------------------------------------------------------------------------
#include "ssp.h"

void  normx11(double top[], int n, double *x0, double *xd)
{
	// ������������ ���������� ��������� ���������������� �������
	// � ��������� [-1,1]
	int  i;
	double xa, xm;
	xa = top[0]; (*x0) = xa; //xe = 0.e0;
	for (i=0; i<n; i++){
		xm = top[i];
		if ((xa-xm)>0.0) xa = xm;
		if (((*x0)-xm)<0.0) (*x0) = xm;
	}
	(*xd) = (*x0)-xa;
	if ((*xd)<=0.0) return; //��

	(*x0) =-((*x0)+xa)/(*xd);
	(*xd) = 2.e0/(*xd);
	//����������
	for (i=0; i<n; i++)	top[i] = top[i]*(*xd)+(*x0);
	return;
}
