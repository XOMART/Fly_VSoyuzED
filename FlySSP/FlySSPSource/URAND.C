/*---------------------------------------------------------------------------
  URAND.C
	  ��������� ��������������� ����� �� ������������ ������
	  (��.���ᠩ�)
  ����	 : ��ॢ�� �� � �믮���� ���뭪�� �.�.
  ����� : 01.00 / 15.04.1994 /
---------------------------------------------------------------------------*/
#include <math.h>
#include "ssp.h"

double urand(long *iy)
{
static long 		ia,ic,itwo=2,m2=0,m,mic;
static double 		s;
       double 		halfm;

  if(m2==0) /*���� �室*/
  {
    m=1;
    do
    {
      m2 =  m;
      m  =  itwo * m2;
    } while (m>m2);
    halfm=m2;//����� 楫��᫥����� ��設���� ᫮��
    /*
      ���᫨�� �����⥫� � ���饭�� ���������
      ��������⭮�� ��⮤�
    */
    ia = (long)(8.0*floor(halfm * atan(1.0)/8.0)+5);
    ic = (long)(2.0*floor(halfm * (0.5 - sqrt(3.0)/6.0))+1.0);
    mic = (m2-ic) + m2;
    s=0.5/halfm;/*����⠡����騩 �����⥫� ��� �८�ࠧ������ � �᫮
		  � ��. �窮�*/
  }
  /*���᫨�� ᫥���饥 ��砩��� �᫮*/
  *iy*=ia;
  /*᫥���騩 ������ - ��� ��設 , ����� ��
    ����᪠�� ��९������� 楫�� �ᥫ
    �� ᫮����� */
  if(*iy > mic)
	*iy =(*iy-m2) - m2;
  *iy += ic;
  /*᫥���騩 ������ - ��� ��設 � ������
    ����� ��設���� ᫮�� ��� ᫮����� ����� ,
    祬 ��� 㬭������*/
  if(*iy/2 > m2)
	*iy= (*iy - m2 ) - m2;
  /*᫥���騩 ������ - ��� ��設 � ������
    ��९������� 楫��� �᫠ ����� �� ������� ࠧ��
    */
  if(*iy < 0)
     *iy=(*iy+m2) +m2;
  return((double) *iy * s);

}