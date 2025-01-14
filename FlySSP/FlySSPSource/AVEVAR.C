//---------------------------------------------------------------------------
//  AVEVAR.C
//    ������ ���������� �������� � ��������� (��ᬥ饭��� �業��)
//  ����  : ���뭪�� �.�.
//  ����� : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
//---------------------------------------------------------------------------
//	SUBROUTINE AVEVAR
//
//	PURPOSE
//	   COMPUTE MEANS, STANDARD DEVIATIONS.
//
//	USAGE
//	   void avevar(double data[], int n, double *ave, double *svar);
//
//	DESCRIPTION OF PARAMETERS
//	   data[n] - ����� �������, ࠧ��୮�� n
//	   n       - �᫮ �������
//	   *ave    - ���� १���� - �롮�筮� �।���
//	   *svar   - ���� १���� - �롮�筠� ��ᯥ���
//
//	REMARKS
//	   �᫨ ave=NULL ��� svar=NULL, � ᮮ⢥����騩 १����
//	   �� ��������
//---------------------------------------------------------------------------

void avevar(double data[], int n, double *ave, double *svar)
{
int    j;
double s,a,b;
	b=a=0;
	if(ave) (*ave)=0.0;
	if(svar) (*svar)=0.0;
	if(ave || svar)
	  {
	  for (j=0;j<n;j++) a += data[j];
	    a /= n;
	  if(ave) (*ave)=a;
	  }
	if(svar)
	  {
	  for (j=0;j<n;j++)
	    {
	    s=data[j]-a;
	    b += s*s;
	    }
	  if(n>1)
	    b /= (n-1);
	  (*svar)=b;
	  }
return;
}
