//---------------------------------------------------------------------------
//  SDAD.C
//           
//         
//  ĸâŽā  :  áë­ĒŽĸ ..
//  Ĩāá¨ī : 01.00 / 27.09.2000 /
//---------------------------------------------------------------------------
#include "ssp.h"
#include <math.h>

/*     ..................................................................

	SUBROUTINE SDEVP

	PURPOSE
	         
	         

	USAGE
	   SDAD(N,X,Y,SDEV,ADEV,SIG,FDATA);

	DESCRIPTION OF PARAMETERS
	   N     - į¨áĢŽ ­ ĄĢî¤Ĩ­¨Š
		   NUMBER OF OBSERVATIONS. N MUST BE > OR = TO 2.
	   X[N]  - Ŧ áá¨ĸ ¨áåŽ¤­ëå §­ įĨ­¨Š ĸāĨŦĨ­­ŽŖŽ āī¤ , [n] ¨Ģ¨ NULL
	   Y[N]  - Ŧ áá¨ĸ áŖĢ ĻĨ­­ëå §­ įĨ­¨Š ĸāĨŦĨ­­ŽŖŽ āī¤ , [n] ¨Ģ¨ NULL
	   SDEV  -  ¤āĨá ĸĨĢ¨į¨­ë Žáâ âŽį­ŽŠ ¤¨á¯Ĩāá¨¨
		   sdev=1/n * SUM (x[i]-y[i])^2, i=1,...,n
	   ADEV  -  ¤āĨá ĸĨĢ¨į¨­ë áāĨ¤­ĨŠ Žâ­Žá¨âĨĢė­ŽŠ Žč¨ĄĒ¨
		   adev=1/n * SUM ( ABS(x[i]-y[i])/x[i] ), i=1,...,n
	   SIG[N]- Ŧ áá¨ĸ ¨­¤¨ĸ¨¤ã Ģė­ëå Žâ­Žá¨âĨĢė­ëå ŽâĒĢŽ­Ĩ­¨Š ¨Ģ¨ NULL
	   FDATA(i,&xi,&yi)-äã­Ēæ¨ī įâĨ­¨ī (ā áįĨâ ) ¨áåŽ¤­ŽŖŽ ¨
		   áŖĢ ĻĨ­­ŽŖŽ ĸāĨŦĨ­­ŽŖŽ āī¤  ¨Ģ¨ NULL
	REMARKS
	   NONE

	SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
	   DATA(i,X[i],Y[i]) - THIS SUBROUTINE MUST BE PROVIDED BY THE USER.

	METHOD
	...............................................................

     INITIALIZATION
 */
//#define Sig(arg1, arg2)         ( fabs((arg1)-(arg2))/(arg1) )
//#define Sig(arg1, arg2)         ( fabs((arg1))/(arg2) )

int sdad(int n,double x[],double y[],
	   double *sdev,double *adev,
	   double sig[],
	   void (*fdata)(int i, double *xi, double *yi) )
{
int    i,io;
double xi,yi,a,sd,ad,fn;
  if(n<1) return -2;
  if(!x || !y)
    {
    if(!fdata) return -1;
      else io=3;
    }
    else
    if(!x) io=2;
      else
      if(!y)io=1;
	else    io=0;
  fn=1.0/(double)n;
  for(sd=0,ad=0,i=0;i<n;i++)
    {
    switch(io)
      {
      default:
      case 0:
	xi=x[i]; yi=y[i]; break;
      case 1:
	fdata(i,&xi,y);
	yi=y[i]; break;
      case 2:
	fdata(i,x,&yi);
	xi=x[i]; break;
      case 3:
	fdata(i,&xi,&yi);  break;
      }
      a=xi-yi;
      sd+=(a*a*fn);
      if(xi!=0)
	a=fabs(a)/xi;
	else
	a=0;
      ad+=(a*fn);
      if(sig)
	sig[i]=a;
    }
    if(sdev) *sdev=sd;
    if(adev) *adev=ad;
return 0;
}

