//---------------------------------------------------------------------------
// ����������� ������ ���������������� ����������
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\Z.h>

//---------------------------------------------------------------------------
#define NEED_ANAL_AE
//#define AE_2003      // ������ 2003 ����, ����� - 1996 ���� 
// -----------------------------------------------------------------------
#ifdef AE_2003
   // ������� ����� - 2003 ����, ����� - 1996 ����
    #define STARTIME_2003
   // �������        - 2003 ����, ����� - 1996 ����
    #define NUTATION_2003
   // ���������      - 2003 ����, ����� - 2000 ����
    #define PRECESSION_2003
   // ������ � ��������� - 2009 ����, ����� - 1996 ����
    #define E0_2003
   // ������������� �������� ��� - 2009 ����, ����� - 1996 ����
    #define VAR_PVZ_OCEAN_2003
#else
   // ������������� �������� ��� - 2009 ����, ����� - 1996 ����
   // #define VAR_PVZ_OCEAN_2003
#endif AE_2003
//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

//---------------------------------------------------------------------------
#include "ModType.h"
#include "ModAstro.h"
//---------------------------------------------------------------------------
#define SIZE_OPER_MAS_TABL_AE 30
double  Tae ;

//---------------------------------------------------------------------------
// ���������� ��������� �� ������ ������ � ������������
static CModAstro *pAstroData = nullptr ;
//---------------------------------------------------------------------------
#ifdef _CML_ASTRODATA_SS
CML_AstroData *pAstroDataSS;
#endif _CML_ASTRODATA_SS
//---------------------------------------------------------------------------
// ���� ������������� ������ � ������������
static bool  fAstroData = 0 ;
//---------------------------------------------------------------------------
// ������ ������ �� 9-�� ���������� ������ � ��������
void Arg_Planet(double t, double *u);
//---------------------------------------------------------------------------
// ������� �������� ����������� ������� ������� ��������������� ������
//---------------------------------------------------------------------------
HRESULT Init_AEJG( double dTA_Begin )
{
	if( !pAstroData ) pAstroData = new CModAstro(BTS_EARTH);
#ifdef _CML_ASTRODATA_SS
	//if(! pAstroDataSS ) pAstroDataSS = new CModAstro(BTS_SS);
#endif _CML_ASTRODATA_SS 
	fAstroData = true ;
	return 0 ;
}
//---------------------------------------------------------------------------
// ������� �������� ����������� ������� ������� ��������������� ������
//---------------------------------------------------------------------------
void End_AEJG()
{
	if ( pAstroData  ) { delete pAstroData ; pAstroData = nullptr ; }
#ifdef _CML_ASTRODATA_SS
	if( pAstroDataSS ) { delete pAstroDataSS ; pAstroDataSS = nullptr ; }
#endif _CML_ASTRODATA_SS
}

//---------------------------------------------------------------------------
// ������ ��������������� ������
//---------------------------------------------------------------------------
short AEJG(double t,short N, short mode)
{
	short ko=0;
	static short Flag_TAE, Flag_NotPVZ;
    //-------------------------------------------------------------------------
    // ������������� ��������������� ������
    //-------------------------------------------------------------------------
    if( !fAstroData ) Init_AEJG( t ) ; 
    Tae = t ;
    if(fabs(Tae-Wae->t[N]) < Wae->dop[ N ]) return 0 ;
    //-------------------------------------------------------------------------
    // ������ ��������������� ������
    //-------------------------------------------------------------------------
    if(mode==BTS_EARTH)
    {
      if(pAstroData)
      {
        double *pDblPar ;
        AstroObject  Obj;

        switch(N)
        {
          default:
          //---------------------------------------------------------------------
          case 0: // ������
          pDblPar = Wae->qso;
          Obj = Sun;
          break;
          //---------------------------------------------------------------------
          case 1: // ����
          pDblPar = Wae->qlu;
          Obj = Moon;
          break;
          //---------------------------------------------------------------------
          case 8: // ��������
          pDblPar = Wae->qme;
          Obj = Mercury;
          break;
          //---------------------------------------------------------------------
          case 5: // ������
          pDblPar = Wae->qwe;
          Obj = Venus;
          break;
          //---------------------------------------------------------------------
          case 6: // ����
          pDblPar = Wae->qma;
          Obj = Mars;
          break;
          //---------------------------------------------------------------------
          case 2: // ������
          pDblPar = Wae->qup;
          Obj = Jove;
          break;
          //---------------------------------------------------------------------
          case 3: // ������
          pDblPar = Wae->qsa;
          Obj = Saturn;
          break;
          //---------------------------------------------------------------------
          case 4: // ����
          pDblPar = Wae->qur;
          Obj = Uranus;
          break;
          //---------------------------------------------------------------------
          case 7: // ������
          pDblPar = Wae->qne;
          Obj = Neptune;
          break;
          //---------------------------------------------------------------------
          case 9: // ������
          pDblPar = Wae->qpl;
          Obj = Pluto;
          break;
          //---------------------------------------------------------------------
          case 12: // �������
#ifdef NUTATION_2003
          return pAstroData->Calc_AstroData_nutation(Tae, Wae->qnu);
#else
          pDblPar = Wae->qnu;
          Obj = Nutation;
          break;
#endif
          //---------------------------------------------------------------------
          case 10: // ���������
          return ( ( short )pAstroData->Calc_AstroData_precess(Tae, Wae->qpr)) ;
          //---------------------------------------------------------------------
          case 11: // ���
          //if(!Flag_NotPVZ) ko=TABL_AE(t,N);
          if(ko==15)
          {
            Flag_NotPVZ=true;
            ko=0;
          }

          return ko;

          //---------------------------------------------------------------------
        }

        //������ ��������������� ������ �� ������
        ko = ( short )pAstroData->Calc_AstroData(Tae, Obj, 0, pDblPar);
        if(ko)
          Flag_NotPVZ = ko;

        return ko;
      }
      else
      {
        if(N==10) { 
			// ��������� �� ����� 2000 ���� (�� ������ 12 ����� 01.01.2000 ��������� �������� ����� ����)
			ZPrecessJ2000( Tae, Wae->qpr, TIME1975ASD ) ;
			for (int i = 3; --i >= 0; Wae->qpr[i] /= M_SEKRAD);
			Wae->t[10]=Tae ;
			Wae->metod[ 10 ]=1 ;			
			//PRECES(Tae); 
			return 0; 
		}

        //if(!Flag_TAE) Flag_TAE=TABL_AE(t,N);
        if(!Flag_TAE) return 0;
        ko=Flag_TAE;

#ifdef NEED_ANAL_AE
        if( N<10 && (fabs(Tae-Wae->t[10])>Wae->dop[10])) {
			// ��������� �� ����� 2000 ���� (�� ������ 12 ����� 01.01.2000 ��������� �������� ����� ����)
			ZPrecessJ2000( Tae, Wae->qpr, TIME1975ASD ) ;
			for (int i = 3; --i >= 0; Wae->qpr[i] /= M_SEKRAD);
			Wae->t[10]=Tae ;
			Wae->metod[ 10 ]=1 ;			
			//PRECES(Tae);
		}
        if((N>=2 && N<10) && (fabs(Tae-Wae->t[0])>Wae->dop[0]))
        {
          if((ko=ANAL_AE(Tae,0))!=0) return ko;
        }

        ko=ANAL_AE(Tae,N);
#endif
       }
    }

#ifdef _CML_ASTRODATA_SS
    if(mode==BTS_SS)
    {
      if(pAstroDataSS)
      {
        double *pDblPar, Temp_Dbl[3];
        AstroObject  Obj;

        switch(N)
        {
          default:
          //---------------------------------------------------------------------
          case 0: // ������
          pDblPar = Wae->qso;
          Obj = Sun;
          break;
          //---------------------------------------------------------------------
          case 1: // ����
          pDblPar = Wae->qlu;
          Obj = Moon;
          break;
          //---------------------------------------------------------------------
          case 8: // ��������
          pDblPar = Wae->qme;
          Obj = Mercury;
          break;
          //---------------------------------------------------------------------
          case 5: // ������
          pDblPar = Wae->qwe;
          Obj = Venus;
          break;
          //---------------------------------------------------------------------
          case 6: // ����
          pDblPar = Wae->qma;
          Obj = Mars;
          break;
          //---------------------------------------------------------------------
          case 2: // ������
          pDblPar = Wae->qup;
          Obj = Jove;
          break;
          //---------------------------------------------------------------------
          case 3: // ������
          pDblPar = Wae->qsa;
          Obj = Saturn;
          break;
          //---------------------------------------------------------------------
          case 4: // ����
          pDblPar = Wae->qur;
          Obj = Uranus;
          break;
          //---------------------------------------------------------------------
          case 7: // ������
          pDblPar = Wae->qne;
          Obj = Neptune;
          break;
          //---------------------------------------------------------------------
          case 9: // ������
          pDblPar = Wae->qpl;
          Obj = Pluto;
          break;
          //---------------------------------------------------------------------
          case 12:// �������
#ifdef NUTATION_2003
          return pAstroData->Calc_AstroData_nutation(Tae, Wae->qnu);
#else
          pDblPar = Wae->qnu;
          Obj = Nutation;
          break;
#endif
          //---------------------------------------------------------------------
          case 10:// ���������
          return pAstroDataSS->Calc_AstroData_precess(Tae, Wae->qpr);
          //---------------------------------------------------------------------
          case 11:// ���
          if(!Flag_NotPVZ)
            ko=TABL_AE(t,N);
          if(ko==15)
          {
            Flag_NotPVZ=true; ko=0;
          }

          return ko;

          //---------------------------------------------------------------------
          case 13: // ����� (��� �� ��)
          pDblPar = Wae->qze;
          Obj = Earth;
          break;
        }

        //������ ��������������� ������ �� ������
        pAstroDataSS->AstroRazmSet(0,0,0,0);  //����� J2000 ������������� ��� ������ BTS_SS

        ko=pAstroDataSS->Calc_AstroData(Tae, Obj, 0, pDblPar);

        if(ko)
          Flag_NotPVZ = ko;

//        double dResult[6];
//        memset(dResult, 0, sizeof(dResult));
//        ko=pAstroDataSS->Calc_Precesion(Tae, Obj, dResult);

        return ko;
      }
      else
      {
        if(N==10) { PRECES(Tae); return 0; }

        if(!Flag_TAE)
          Flag_TAE=TABL_AE(t,N);
        if(!Flag_TAE) return 0;
        ko=Flag_TAE;

#ifdef NEED_ANAL_AE
        if(N<10 && (fabs(Tae-Wae->t[10])>Wae->dop[10])) PRECES(Tae);
        if((N>=2 && N<10) && (fabs(Tae-Wae->t[0])>Wae->dop[0]))
        {
          if((ko=ANAL_AE(Tae,0))!=0) return ko;
        }
        ko=ANAL_AE(Tae,N);
#endif
      }
    }
#endif _CML_ASTRODATA_SS

    return ko;
}

//----------------------------------------------------------------------------
// ������� ������� ��������� ������� ����� B1975 � J2000
//----------------------------------------------------------------------------
double Calc_Delta_J2000_B1975()
{
	double jd2000[ 2 ] = { 2451545., 0.0 } ;
	double jd1975[ 2 ] = { 2442413., 0.5 }, bd1975[ 2 ] ;
	double Delta_T ;
	// ���������� ������ ��������� 1975 ���� � ��������� ����
	JD_Bessel( jd1975, bd1975 ) ;
	// ���������� ������� ����� B1975 � J2000 � ��������� ����
	Delta_T = ( bd1975[ 0 ] - jd2000[ 0 ] ) + ( bd1975[ 1 ] - jd2000[ 1 ] ) ;
	// ���������� ������� ����� B1975 � J2000 � ��������� ���������
	Delta_T /= 36525. ;
	return Delta_T ;
}
//----------------------------------------------------------------------------
// ������� ������� ��������� �� ����� �� 0 ��� 01.01.1975 �� ��������� �������
//----------------------------------------------------------------------------
/*
void Calc_Prec_Par_B1975(double Tae, double *Preces)
{
	double Cs;
	Cs = tau2000( t_TDT( *W_DELTA_2000 + Tae ) ) - J_ask0 ;
	//  preces(J_ask0,Cs,Preces);
	preces_IERS( J_ask0, Cs, Preces ) ;
}
*/
//----------------------------------------------------------------------------
// ������� ������� ��������� �� ����� �� 0 ��� 01.01.1975 �� ��������� �������
//----------------------------------------------------------------------------
/*
void Calc_Prec_Par(double dIAT_E1, double dIAT_E2, double *Preces)
{
	double Tep, Tau, Tmp_Preces[ 3 ] ;
	short i;
	//������ ��������� �� ����� �� �1 �� �2
	//����� ����� E1 ������������ J2000 � �����.���������
	double TDT = t_TDT(*W_DELTA_2000+dIAT_E1) ;
	// ����� � ��������� ��������� �� 0h 01.01.2000 �� 12h 01.01.2000 - 32 ��� (�����?)
	Tep = tau2000(TDT);
	// ����� ����� E2 ������������ ����� E1 � �����.���������
	Tau = (dIAT_E2-dIAT_E1)/36525.;
	preces_IERS(J_ask0,-J_ask0, Tmp_Preces);
	preces_IERS(Tep,Tau,Preces);
	// ������� ��������� � �������
	for(i=0; i<3; i++) Preces[i] /= k_sekrad;
}
*/
//----------------------------------------------------------------------------
// ������� ������� ���������
//----------------------------------------------------------------------------
/*
void PRECES(double Tae)
{
	double Cs;
	short i;
	Cs = tau2000( t_TDT( *W_DELTA_2000 + Tae ) ) - J_ask0 ;
	preces_IERS( J_ask0, Cs, Wae->qpr ) ;
	for( i = 3 ; --i >= 0 ; Wae->qpr[ i ] /= k_sekrad ) ;
	Wae->t[ 10 ] = Tae ;
	Wae->metod[ 10 ] = 1 ;
}
*/
/*
#ifndef PRECESSION_2003
//----------------------------------------------------------------------------
// ������ ���������� ��������� �� �au �� �au+tau (� ������� ��������)
//----------------------------------------------------------------------------
static double Mas_Kft_Prec[15] = {
	+2306.2181, //  0 - �������� ����� ���_�, ���_�
	+1.39656,   //  1 -
	-0.139e-3,  //  2 -
	+0.30188,   //  3 - �������. ����� ���_�
	-0.344e-3,  //  4 -
	+0.017998,  //  5 - �������. ����  ���_�
	+1.09468,   //  6 - �������. ����� ���_�
	+0.66e-4,   //  7 -
	+0.018203,  //  8 - �������. ����  ���_�
	+2004.3109, //  9 - �������� ����� ���_�
	-0.8533,    // 10 -
	-0.217e-3,  // 11 -
	-0.42665,   // 12 - �������. ����� ���_�
	-0.217e-3,  // 13 -
	-0.041833   // 14 - �������. ����  ���_�
};
//----------------------------------------------------------------------------
// ������ ���������� ��������� �� �au �� �au+tau (� ������� ��������)
//----------------------------------------------------------------------------
void preces(double Tau, double tau, double *Precess)
{
	double dTemp;
	// ������ ���������� ���������
	dTemp = Mas_Kft_Prec[ 0]+(Mas_Kft_Prec[ 1]+Mas_Kft_Prec[ 2]*Tau)*Tau;
	Precess[0] = (tau*(tau*(Mas_Kft_Prec[ 5]*tau+Mas_Kft_Prec[ 4]*Tau+Mas_Kft_Prec[ 3])+dTemp));
	Precess[1] = (tau*(tau*(Mas_Kft_Prec[ 8]*tau+Mas_Kft_Prec[ 7]*Tau+Mas_Kft_Prec[ 6])+dTemp));
	dTemp = Mas_Kft_Prec[ 9]+(Mas_Kft_Prec[10]+Mas_Kft_Prec[11]*Tau)*Tau;
	Precess[2] = (tau*(tau*(Mas_Kft_Prec[14]*tau+Mas_Kft_Prec[13]*Tau+Mas_Kft_Prec[12])+dTemp));
}
#endif
*/
//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� tau �� ����� J2000
// dIAT_D1975 - ������� ����� � ��� �� 0 ����� ������� �����
//----------------------------------------------------------------------------
/*
HRESULT Precess_J2000(double dIAT_D1975, double *Precess)
{
	double Tau = 0., tau;

	// ���������� �������� � ����� J2000 (�� 12 ����� 1 ������)
	tau = tau2000(t_TDT(*W_DELTA_2000+dIAT_D1975)); 
	// ������ ���������� ���������
	preces_IERS(Tau, tau, Precess);
	// ���������� ���������� ��������� � �������
	for(int i=0; i<3; i++) Precess[i]/= k_sekrad;
	return S_OK;
}
*/
//----------------------------------------------------------------------------
/*
static double Mas_Kft_Prec_IERS2003[18] = {
	+2.650545,     //  0 - ���_�: ���������
	+2306.083227,  //  1 -        �������� ������������
	+0.2988499,    //  2 -        **2
	+0.01801828,   //  3 -        **3
	-5.791e-6,     //  4 -        **4
	-3.173e-7,     //  5 -        **5
	-2.650545,     //  6 - ���_�: ���������
	+2306.077181,  //  7 -        �������� ������������
	+1.0927348,    //  8 -        **2
	+0.01826837,   //  9 -        **3
	-2.8596e-5,    // 10 -        **4
	-2.904e-7,     // 11 -        **5
	+0.0,          // 12 - ���_�: ���������
	+2004.191903,  // 13 -        �������� ������������
	-0.4294934,    // 14 -        **2
	-0.04182264,   // 15 -        **3
	-7.089e-6,     // 16 -        **4
	-1.274e-7,     // 17 -        **5
} ;
//----------------------------------------------------------------------------
// ������ ���������� ��������� �� �au �� �au+tau (� ������� ��������)
//----------------------------------------------------------------------------
#ifndef PRECESSION_2003
void preces(double Tau, double tau, double *Precess);
#endif
HRESULT Precess_J2000_IERS(double Tau, double *Precess);
void preces_IERS(double Tau, double tau, double *Precess)
{
#ifdef PRECESSION_2003
 static bool   Flag_PrecessFix75 = false;
 static double PrecessFix75[3];
        double PrecessFix[3];
        UINT   i;

// ������ ���������� ��������� �� ������� �����
 Precess_J2000_IERS( Tau+tau, Precess );
// ������ ���������� ��������� �� ������������� �����
 if( Tau == J_ask0 )
   { if( !Flag_PrecessFix75  )
       { Precess_J2000_IERS( Tau, PrecessFix75 ); Flag_PrecessFix75 = true; }
     for( i=0; i<3; i++ ) PrecessFix[i] = PrecessFix75[i];
   }
 else
   { Precess_J2000_IERS( Tau, PrecessFix ); }
// ������ ����� ���������
 for( i=0; i<3; i++ ) Precess[i] -= PrecessFix[i];
#else
  preces( Tau, tau, Precess );
#endif
}
//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� tau �� ����� J2000
// Tau - ������� ����� � ��� �� 0 ����� ������� �����
//----------------------------------------------------------------------------
HRESULT Precess_J2000_IERS(double tau, double *Precess)
{
	for( UINT i=0; i<3; i++ )
		Precess[i] = Mas_Kft_Prec_IERS2003[i*6]+
			(Mas_Kft_Prec_IERS2003[i*6+1]+
            (Mas_Kft_Prec_IERS2003[i*6+2]+
            (Mas_Kft_Prec_IERS2003[i*6+3]+
            (Mas_Kft_Prec_IERS2003[i*6+4]+
            Mas_Kft_Prec_IERS2003[i*6+5]*tau)*tau)*tau)*tau)*tau;
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// �� ��������� FlagTDT = true (����� TDT �� ���), ����� ����� TAI
//----------------------------------------------------------------------------
double SR_E0( double Tae, bool FlagTDT )
{
	double Cs ;
	Cs = DELTA_2000 + Tae ;
	if( !FlagTDT ) Cs = t_TDT( Cs ) ;
	Cs=tau2000( Cs );
	return(sr_E0_IERS(Cs) / M_SEKRAD);
}

//----------------------------------------------------------------------------
// ������ �������� ������� ������� �������� � ��������� (��������� � ���.���.)
//----------------------------------------------------------------------------
double sr_E0_IERS(double tau2000)
{
#ifdef E0_2003
	// AE2009
	return ((((-4.34e-8*tau2000-5.76e-7)*tau2000+0.0020034)*tau2000-0.0001831)*tau2000-46.836769)*tau2000+84381.406;
#else
	//return ((0.001813*tau2000-0.00059)*tau2000-46.81500)*tau2000+84381.448;
	// Convention_2003
	return ((0.001813*tau2000-0.00059)*tau2000-46.84024)*tau2000+84381.448;
#endif
}
//----------------------------------------------------------------------------
// t - ����� TDT � ��������� �� J2000.0
// u - ���������-������ �� 5-�� ���������� l,l',F,D,Omega � ��������
// FlagCalc - ���� ������� (0 - 5 ����������(�� ���������),1 - 5,GMST+PI, 2 - 5,Planet) - ����� 09.02.09
//----------------------------------------------------------------------------
void fargTIME( double t, double *u, char FlagCalc )
{
	double x, y ;
	short  i, j ;

	// ������� �������� �� ������� ��� ����������� �������
	const static double a[5][6] = {
	// Convention_2003 - ?
	{ 1325.,-24.470e-5, 5.1635e-2, 31.8792, 715923.2178, 485868.249036 },    // ������� �������� ����
	{   99.,-11.490e-5,-0.0136e-2, -0.5532,1292581.0481,1287104.793048 },    // ������� �������� ������
	{ 1342.,  0.417e-5,-0.1037e-2,-12.7512, 295262.8478, 335779.526232 },    // L-Omega
	{ 1236., -3.169e-5, 0.6593e-2, -6.3706,1105601.2090,1072260.703692 },    // ������� ��������� (�������� ������� ������) ���� � ������
	{   -6., -5.939e-5, 0.7702e-2,  7.4722, 813109.4569, 450160.398036 } } ; // ������� ������� ����������� ���� ������ ���� �� ���������                              };
	// ������ �������� ��������������� ����������
	for( i = 0 ; i < 5 ; i++ ) {
		x=a[i][0]*t;
		x-=floor(x);
		for( j=1,y=0.; j<6; y=y*t+a[i][j++] );
		x+=y/1296000.;
		u[i]=(x-floor(x))*k_dwapi;
	}
	// ������ �������������� ����������
	if( FlagCalc == 1 ) { 
		// ������ GMST+PI
		u[5] = ZAverageSiderealTime(t*36525-del_TDT/k_cbc)*k_dwapi+k_pi;
		u[5] = fmod(u[5],k_dwapi); // ������ ����� �����
	}
	if( FlagCalc == 2 ) {
		// ������ ��������� ������
		Arg_Planet( t, u+5 ); 
	}
	return ;
}
//----------------------------------------------------------------------------
// t - ����� TDT � ��������� �� J2000.0
// u - ���������-������ �� 9-�� ���������� ������ � ��������
//----------------------------------------------------------------------------
void Arg_Planet(double t, double *u)
{
	//lMe
	u[0] = 4.402608842 + 2608.7903141574 * t;
	//lVe
	u[1] = 3.176146697 + 1021.3285546211 * t;
	//lE
	u[2] = 1.753470314 + 628.3075849991 * t;
	//lMa			
	u[3] = 6.203480913 + 334.0612426700 * t; 			
	//lJu
	u[4] = 0.599546497 + 52.9690962641 * t;
	//lSa
	u[5] = 0.874016757 + 21.3299104960 * t;
	//lUr
	u[6] = 5.481293872 + 7.4781598567 * t;
	//lNe
	u[7] = 5.311886287 + 3.8133035638 * t;
	//pa
	u[8] = (0.024381750 + 0.00000538691 * t) * t;
}
//---------------------------------------------------------------------------
// ���ר� ������� �������� �� B1975 � J2000
// ��������� - double Matr_Precess[9]
//---------------------------------------------------------------------------
void Calc_Matr_Preces_B1975_J2000( double *Matr_B1975_J2000 )
{
	static bool FlagInit_mJ2000_B1975 = false;
	double      Preces[ 3 ];
	UINT        i;

	if( !FlagInit_mJ2000_B1975 ) { // ������ �������   
		memset( mJ2000_B1975, 0, 9*sizeof(double) ) ;
		// ������ ���������� ���������
		#ifdef PRECESSION_2003
			//Precess_J2000_IERS( J_ask0, Preces );
		#else
			//preces( 0, J_ask0, Preces );
			ZPrecessJ2000( 0.5, Preces, TIME2000ASD) ;
		#endif
		// ������� � �������
			for (i = 0; i<3; i++) Preces[i] /= M_SEKRAD;
		// ������ ������� ���������
		mJ2000_B1975[0] = -sin(Preces[0])*sin(Preces[1]) + cos(Preces[0])*cos(Preces[1])*cos(Preces[2]);
		mJ2000_B1975[1] = -cos(Preces[0])*sin(Preces[1]) - sin(Preces[0])*cos(Preces[1])*cos(Preces[2]);
		mJ2000_B1975[2] = -cos(Preces[1])*sin(Preces[2]);
		mJ2000_B1975[3] =  sin(Preces[0])*cos(Preces[1]) + cos(Preces[0])*sin(Preces[1])*cos(Preces[2]);
		mJ2000_B1975[4] =  cos(Preces[0])*cos(Preces[1]) - sin(Preces[0])*sin(Preces[1])*cos(Preces[2]);
		mJ2000_B1975[5] = -sin(Preces[1])*sin(Preces[2]);
		mJ2000_B1975[6] =  cos(Preces[0])*sin(Preces[2]);
		mJ2000_B1975[7] = -sin(Preces[0])*sin(Preces[2]);
		mJ2000_B1975[8] =  cos(Preces[2]);
		FlagInit_mJ2000_B1975 = true;
	}
	// ���������� ������� � �������� ������
	memcpy( Matr_B1975_J2000, mJ2000_B1975, 9 * sizeof( double ) ) ;
}

//----------------------------------------------------------------------------
