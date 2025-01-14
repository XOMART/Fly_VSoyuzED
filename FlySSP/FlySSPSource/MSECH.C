/*------------------------------------------------------------------------
  MSECH.C
	  ���������� ���� ������� f(x) �� ��������� [a,b]
	  ������� �������-����
  ����   : ���뭪�� �.�.
  �����  : 01.00 / 1.02.1997 /
------------------------------------------------------------------------*/
#include <math.h>


double msechord(const double a,
		const double b,
		short *iter,
		float tol,
		double (*fuser)(const double ,const void *),
		const void *userdata)
{
// ���� ���� ����������� �ࠢ����� f(x) �� ���ࢠ�� [a,b]
// ��⮤�� ᥪ���-�� (� �᭮�� ���樮���� ��㫠 ���⮭�).
// ����: ��� f(x)= x**3 - 2*x - 5, ��७� x=2.0945514815 �� ���ࢠ��
//       x=[2,3]
//                         �室�� ��ࠬ����:
//  a    - ����� �࠭�� ���᪠ ����,
//  b    - �ࠢ�� �࠭�� ���᪠ ����,
//  iter - ���ᨬ��쭮� �᫮ ���権 (�᫨ iter<=0, � �� 㬮�砭�� 10),
//  tol  - ����譮��� ���᫥��� ���� � ������ ���祭�� �㭪樨,
//  fuser(x,userdata)  - ����ணࠬ�� ���짮��⥫�, ��������� ���祭��
//                       �㭪樨 f(x) ��� ���祭�� ��㬥�� x �
//                       �ᯮ�짮������ �ᯮ����⥫��� ������ userdata,
//  userdata - ���� �ᯮ����⥫��� ������, �ᯮ��㥬�� �� ���᫥����
//             ����, ���� NULL.
//                        �室�� ��ࠬ����:
//  �����頥��� ���祭�� ����ணࠬ�� - ��७�
//  iter >  0 - 䠪��᪮� �᫮ ���権 ��� ���⨦���� ��������
//              �筮�� tol ���᪠ ���� f(x);
//       = -1 - �訡�� ��室��� ������: ����� �窠�� � � b �㭪�� ��
//              ���ᥪ��� ��� x;
//       =  0 - �������� �筮��� tol �� ���⨣��� ��-�� ��࠭�祭�� ��
//              ��� ���権 iter;


short maxiter=*iter,i;
double df,f1,f2,x1,x2,x,step;//eps,
if(maxiter<=0)
   maxiter=10;
//eps=macheps();
x1=a;
x2=b;
f1=fuser(x1,userdata);
f2=fuser(x2,userdata);
if( f1*f2 > 0 )
   {
   *iter=-1;//�訡�� � ��: �ਢ�� �� ���ᥪ��� ���
   x=x1;
   return x;
   }
i=0;
x=x1;
do
  {
  df=f2-f1;
  if( df)
    x=x2-(x2-x1)*f2/(f2-f1);
    else break;
  if(fabs(f1)>fabs(f2))   {step=f1;x1=x;f1=fuser(x,userdata);}
     else {step=f2;x2=x;f2=fuser(x,userdata);}
  }while( ( fabs(step)) > tol  &&  ++i <= maxiter);
if(fabs(step) < tol)
  *iter=0;
  else *iter=i;

if(fabs(f1)<fabs(f2))   x=x1;
     else x=x2;
return x;
}
/*
//  ���
double ftest(const double x,const void *data)
{
double f;
if(data==NULL)
  f=0;
f=x* (x*x-2.0) -5;
return f;
}

void main(void)
{
double x,f,a=2.0,b=3.0;
short iter=10;
float tol=1e-15;
x=msechord(a,b,&iter,tol,ftest,NULL);
f=x;
a=f;
return;
}
*/