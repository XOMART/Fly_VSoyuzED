//------------------------------------------------------------------------
//  FNW.C
//        
//  ĸâŽā   :  áë­ĒŽĸ ..
//  Ĩāá¨ī  : 01.00 / 16.09.2000 /
//------------------------------------------------------------------------
#include "ssp.h"

// áįĨâ ä §Ž-į áâŽâ­ŽŠ å ā ĒâĨā¨áâ¨Ē¨ (): Fin(w)
// w = 2*pi*f = 2*pi*n*f1 = 2*pi*n/T
double Finw(double aw,double bw)
{
double fnw;
double const rad=180/M_PI;
  fnw=-1e20;
  if(aw!=0)
    {
    fnw = bw/aw;
//    fnw = atan(fnw);
//    fnw = atan(fnw);
    fnw = atank(fnw,aw);
    fnw *= rad;
    }
return fnw;
}
