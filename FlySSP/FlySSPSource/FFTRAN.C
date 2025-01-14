//---------------------------------------------------------
// FFTRAN.C
//   ������� �������������� ����� (������ � ��������)
// ����  : ���뭪�� �.�.
// ����� : 01.00 / 01.10.2000 /
//---------------------------------------------------------
#include "ssp.h"
#include <math.h>

/*     ..................................................................

	������������ fftran

	����������
	   ����ணࠬ�� ������ �����樥��� X(real,imag)[k]
		     k=1,2,...,M
	   ����ண� �८�ࠧ������ ����:
//	       F(X)=A(0)+SUM(A(K)COS KX+B(K)SIN KX)
//	   �������� �� ���ࢠ�� [0,2*M_PI] �� ࠢ�������饩 ��⥬�
	���饭�� � ����ணࠬ��
	   int fftran(sign, double dt, double *x, int unsigned npow);
	   ier=fftran(sign, dt, x, npow);

	�室�� � ��室�� ��ࠬ����
	   sign   - <0(-1) ��אַ� ���,
		    >0(+1) ���⭮� ���,
	   dt     - 蠣 ���祭�� ��㬥��,
	   x[2*n] - ���ᨢ ������ 2*n �������� ���祭�� �㭪樨;
		    n=1<<npow; (ࠢ�� �⥯��� 2)
		    � ��砥 ��אַ�� ��� ��� ���祭�� ࠢ�� 0:
		    x[0]=x1, x[1]=0, ..., x[n-2]=xn, x[n-1]=0,
	   npow   - �᫮ �祪 �६������ �鸞;

	   ����ணࠬ�� �����頥�
	   ier-�������� �訡��, �ਭ����騩 �� �믮������ �ணࠬ��
	       ᫥���騥 ���祭��:
	       0 - �訡�� ���,
	       1 - npow > 13.
	   ����ணࠬ�� ��뢠�� �㭪樨 ��ࠡ�⪨ ���������� �ᥫ:
	       void cmplxadd(double a1[],double a2[],double a[]);
	       void cmplxsub(double a1[],double a2[],double a[]);
	       void cmplxmpy(double a1[],double a2[],double a[]);
	       void cmplxdev(double a1[],double a2[],double a[]);
	       void cmplxeq(double alpha[],double a[]);
     .................................................................. */

//======�㭪�� ��� � ����
int fftran(int sign, double t, double *x, int unsigned npow)
{
double  cxcs[2],hold[2],xa[2],de[2];
double *cs;
int     msk[13];
int     i,ii,ij,j,nn,nmax,nw,nw1,layer,ll,loc,mm,jj,kk;
double  zz,delta,w;
  if(npow > 13 ) return -1;
  nmax = 1 << npow;
  zz = 2 * M_PI  * sign / (double) nmax;
  delta=t;
  cs=(double *)&cxcs[0];
  if( sign > 0 )
    delta=1.0/(delta * (double)nmax );
  msk[0]=nmax/2;
  for(i=1;i< (int)npow;i++)
    msk[i]=msk[i-1]/2;
  nn=nmax;
  mm=2;
  /*���譨� 横� ᫮�� npow*/
  for(layer=0;layer<(int)npow;layer++)
    {//45
    nn/=2;nw=0;
    for(i=1;i<=mm;i++,i++)
      { //40
      ii=nn*i;
      /*cxcs= cexp(2*PI*sign/nmax)*/
      w=(double)nw * zz;
      cs[0]=cos(w);
      cs[1]=sin(w);
      for(j=1;j<=nn;j++)
	{ //20
	ii+=1;
	ij=ii-nn;
	jj=(ii-1)*2;
	kk=(ij-1)*2;
	cmplxmpy(cxcs,&x[jj],xa);
	cmplxsub(&x[kk],xa,&x[jj]);
	cmplxadd(&x[kk],xa,de);
	cmplxeq(de,&x[kk]);
	} //20
      for(loc=2;loc<=(int)npow;loc++)
	{  //25
	ll=nw-msk[loc-1];
	if( ll <= 0 ) break;
	nw=ll;
	}  //25
      if( ll == 0 ) nw=msk[loc];
	 else nw+=msk[loc-1];
      } //40
      mm *= 2;
    }
    nw=0;
    de[0]=delta;de[1]=0;
    for(i=1;i<=nmax/*pow*/;i++)
      {
      nw1=nw+1;
      jj=(nw1-1)*2;
      kk=(i-1)*2;
      cmplxeq(&x[jj],hold);
      if( (nw1-i) >= 0 )
	{
	if( (nw1-i) > 0)
		{
		cmplxmpy(&x[kk],de,&x[jj]);
		}
	cmplxmpy(hold,de,&x[kk]);
	}
      for(loc=1;loc<=(int)npow;loc++)
	{
	ll=nw-msk[loc-1];
	if(ll <= 0) break;
	nw=ll;
	}
	if( ll == 0) nw=msk[loc];
	  else nw += msk[loc-1];
      }
return 0;
}
