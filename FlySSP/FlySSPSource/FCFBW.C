//------------------------------------------------------------------------
//  FCFBW.C
//    ����祭�� ������᪮�� �� �� �������᭮��
//  ����   : ���뭪�� �.�.
//  �����  : 01.00 / 16.09.2000 /
//------------------------------------------------------------------------
#include "ssp.h"

double fcfbw(double x[])
{
double bw;
double y[2],z[2];
   conj(x,y);      // �������᭮-ᮯ�殮���� �᫮
   cmplxsub(x,y,z);
   bw=-z[1];
return bw;
}
