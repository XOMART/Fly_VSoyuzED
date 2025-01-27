//---------------------------------------------------------------------------
//  MOMENT.C
//    ������ ���������� ������������� Mx,M[Mx],Sx,Dx,Ac,Ek
//  ����  : ���뭪�� �.�.
//  ����� : 01.00 / 25.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

int  moment(double data[],int n,
	    double *ave,  // Mx - �।��� ��䬥��᪮�
	    double *adev, //    - �।��� �⪫������ �� Mx
	    double *sdev, // Sx - �।��� �������᪮�
	    double *svar, // Dx - ��ᯥ���
	    double *skew, // Ac - �ᨬ�����
	    double *curt) // Ek - ������
{
int    j,err;
double s,p;
//	void nrerror();
err=-1;
	if (n <= 1)
	  return err;
err=0;
	  //nrerror("n must be at least 2 in MOMENT");
	s=0.0;
	for (j=0;j<n;j++) s += data[j];
	*ave=s/n;
	*adev=(*svar)=(*skew)=(*curt)=0.0;
	for (j=0;j<n;j++) {
		*adev += fabs(s=data[j]-(*ave));
		*svar += (p=s*s);
		*skew += (p *= s);
		*curt += (p *= s);
	}
	*adev /= n;
	*svar /= (n-1);
	*sdev=sqrt(*svar);
	if (*svar) {
		*skew /= (n*(*svar)*(*sdev));
		*curt=(*curt)/(n*(*svar)*(*svar))-3.0;
	}
	else
	err=-2;
//	nrerror("No skew/kurtosis when variance = 0 (in MOMENT)");
return err;
}
