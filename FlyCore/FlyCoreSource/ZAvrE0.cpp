//---------------------------------------------------------------------------
// ZAvrE0.cpp
// �. ��������� 03.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����
#include <FlySofa.h>	// ������� ��������������� ����������

//---------------------------------------------------------------------------
#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCalc.h>
#include <FlyCoreSource\\ZAvrE0.h>

//----------------------------------------------------------------------------
// ������ ������� �������� ������� �������� � ��������� (��������� � ���.���.)
// timeKey:
//	0 - tau ������ � ��� �� J2000 (0h 01.01.2000)
//	1 - tau ������ � ��� �� J1975 (0h 01.01.1975)
//	2 - tau ������ � ��������� ��������� �� J2000 (12h 01.01.2000)
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ������ �������� ������� ��������� � �������� �� �������� ������������� 
// ������ ��������������� ������������� ��������������� ������ � 2006 ����.
// Mean obliquity of the ecliptic, IAU 2006 precession model.
// ��� �� ������������ � ���������� IERS 2009 ����.
double ZAverageE0IERS2009(double t, TIMESCALE Scale)
{
    t = Astro2000Time(t, Scale, nullptr) ;
    return ((((-4.34e-8*t-5.76e-7)*t+0.0020034)*t-0.0001831)*t-46.836769)*t+84381.406;
}

//----------------------------------------------------------------------------
// ������ �������� ������� ��������� � �������� �� �������� ������������� 
// ������ ��������������� IAU � 2003 ����.
// ������ �������������� � ���������� IERS 2003 ����
double ZAverageE0IERS2003(double t, TIMESCALE Scale)
{
    t = Astro2000Time(t, Scale, nullptr) ;
    return ((0.001813*t-0.00059)*t-46.84024)*t+84381.448;
}

//----------------------------------------------------------------------------
// ������ �������� ������� ��������� � �������� �� �������� ������������� 
// ������ �������������� � ��-50.. ��� ������ ���������������
// ������������� ��������������� ������ � 1980 ����
// Mean obliquity of the ecliptic, IAU 1980 model.
double ZAverageE0RD50(double t, TIMESCALE Scale)
{
	t = Astro2000Time(t, Scale, nullptr) ;
	return ((0.001813*t-0.00059)*t-46.815)*t+84381.448;
}

//----------------------------------------------------------------------------
// ������ �������� ������� ��������� � �������� ��� ������ ���������� SOFA.
// SOFA ��� Open Source �����������, ����������� ��������� ���������������
// ������������� ��������������� ������ IAU, � �������� ����������������. 
// ��������� � ������� ��������.
// ������������ ��������� ������ ������� �������� ������������� ������ 
// �� ���� ���������� ���������� ����� � ������ �������. ��� ����� 
// ������������ ������������� ����� ������������� ������� TheTimes
//----------------------------------------------------------------------------
double ZAverageE0LikeSOFA(double t, TIMESCALE Scale, MODEL_AVERAGE_E0 ModAvrE0)
{
	// �������������� ������� � ������� ��������� ��������
	// ������������ �� ��������������� ����� 2000 ����
	double ta ;
	Astro2000Time(t, Scale, &ta) ;
	// "�������" �����
	// ���������� 0.5 ��� ����� ������� ������ ����� J1975 (12h)
	// � ������ ������� ������� �� 1975 ���� (0h)
	TheDblTime Dblt = ta-0.5 ; 
	// �������� � ��������� ����
	Dblt+= TheT_JD1975 ;
	// ������������� ����� 
	TheTimes TheT ; 
	TheT.fromIAT(Dblt) ;

	double e0 ;
	switch(ModAvrE0) {
		case AVRE0IAU1980: e0 = iauObl80(TheT.TT.D1(), TheT.TT.D2()); break ;
		case AVRE0IAU2006: e0 = iauObl06(TheT.TT.D1(), TheT.TT.D2()); break ;
		default: assert(0) ;
	}

	e0*= 180/k_pi*3600.0 ;
	return e0 ;
}

//----------------------------------------------------------------------------
// ������ �������� ������� ��������� � �������� 
// �� ����� �� ������� ��������� � ��������� ModAvrE0
double ZAverageE0(double t, TIMESCALE Scale, MODEL_AVERAGE_E0 ModAvrE0)
{
	// ������� ������� � ����� ��������� �������� ������������� ��
	// ��������������� ����� 2000 ����
	double ta ;	t = Astro2000Time(t, Scale, &ta) ;
	// ������ ������� � ������������ � �������� ���������� ModAvrE0
	// �������
	switch(ModAvrE0) {
		case AVRE01980:
			return ZAverageE0RD50(t, TIMEJ2000JC);
		case AVRE02003:
			return ZAverageE0IERS2003(t, TIMEJ2000JC);
		case AVRE02009:
			return ZAverageE0IERS2009(t, TIMEJ2000JC);
		case AVRE0IAU1980:
		case AVRE0IAU2006:
			return ZAverageE0LikeSOFA(ta, TIME1975ASD, ModAvrE0) ;
		default: assert(0) ;
	}
	return 0 ;
}

//----------------------------------------------------------------------------