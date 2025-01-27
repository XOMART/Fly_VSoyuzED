//-------------------------------------------------------------------------------
// TaskTurnsPredictionSimpl.cpp : 
// �. ��������� 10.08.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������

//-------------------------------------------------------------------------------
// ������� �������� �� �� ������.
// ������������ ������������� �������� �� ������ ����� � �������� �����
//		�������� ��������������� ������ � ������: 
//		ZNU    NU			- ��������� ������� - ���������
//		ZSC_SI SI			- �������� �������� �� - ���������
//		ZMSC   KA			- ������ ��������, ����� �������� � �� ���� ����� 
//							  ������ ������������ �������� (��) - �����
//		KA.Init(...)		- ������������� ������ �������� - ����� 
//		KA.GoToVit(...)		- ������������� �������� �� �� ������ ��������� 
//							  ����� - �����
//		KA.GoAlongVit(...)	- ������� ����� ����� � �������� ������������ 
//							- ���������� ����� - �����
//		ZCPrintManager PrintMng - ��������� ���������������� - �����
//-------------------------------------------------------------------------------
int TaskTurnsPredictionSimpl()
{
	cout << "\n   Simple prediction by Turns...\n";

	int  rc = 0 ; 
	long Vit = 105 ;

	ZCPrintManager PrintMng;	// ��������� ����������������
	ZMSC		  KA;		// ������ �������� ��
	ZNU           NU;		// ��������� ������� ��������

	//----------------------------------------------------------
	// ��
	NU.Key.ka = 1 ; NU.Key.nom = 15 ; NU.Key.mod = 0 ;
	NU.Key.type = 8 ; NU.Key.bc = 1 ;

	NU.numlsf = 1 ;
	DATE dt = DATE(10, 12, 2011);
	TIME tm = TIME(12, 15, 54, 0.5432) ;
	TA_DMB( 1, &dt, &tm, &NU.t ) ;
	NU.vit  = 101 ;

	NU.sk   = _SK_OSK1975 ;
	NU.X[0] = TO_DTKM( 15000.0 ) ;
	NU.X[1] = 0.0012 ;
	NU.X[2] = TO_RAD( 69.0 ) ;
	NU.X[3] = TO_RAD( 250.0 ) ;
	NU.X[4] = TO_RAD( 125.0 ) ;
	NU.X[5] = TO_RAD( 15 ) ;
	NU.Sb   = 0 ;
	NU.Kp   = 0 ;
	NU.M0   = 1503 ;
	NU.MF   = 197 ;
	
	//---------------------------------------------------------------------------
	// ���������� ����� ���
	ZLSF LSF ;	memset(&LSF, 0, sizeof(LSF)) ;
	LSF.num     = 1 ;
	LSF.vgpz    = GPZ90 ;
	LSF.ngpz    = 4 ;
	LSF.mgpz    = 4 ;
	LSF.vatm    = ATMNULL ;
	LSF.vsvd    = 0 ;       
	LSF.varsp   = 0 ;
	LSF.vTide   = TIDENULL ;
	LSF.isDU    = 0 ;
	LSF.isSun   = 1 ;
	LSF.isMoon  = 1 ;

	// ������������� ������ �������� ��� ������� �� �������� �� (NU)
	// � �������� ���� ��� (LSF)
	rc = KA.Init(NU, &LSF) ; if (rc) return rc;

	// �������� ���������� ��������� ��� ���������������� ����������� ��������
	PrintMng.PrePrint(&KA, "�������", "_TaskTurnsPredictionSimpl") ;
	PrintMng.PrintHeaderDoc("������� �������� ��");
	PrintMng.PrintText("������� ���������� �������� �� �� �������� �����");
	// ���������������� �� � ���������� ����� ���
	PrintMng.PrintNU(NU, NULL, 3) ;
	PrintMng.PrintLSF(LSF, 1) ;

	// ������������� �������� �� ����� ����� Vit � �����������
	// ������������ ������������� ����� � ��������� �������� �
	// ����������� ������ ����� (���������� � ���������� ����)
	// �������������� ��� ������ � ����� ����� ������������ �������� 
	// ����������� �������� ������� ����� (_SK_ASKTE) � ���������� ����
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	// ���������������� ����������� ��������
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	// ���������� ��� ���������� ������
	cout << "\r	  Turn  " << Vit << "....     " ;
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit+=2 ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit++ ;

	cout << "\r	  Turn  " << Vit << "....     ";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;
	Vit+=2 ;

	cout << "\r	  Turn  " << Vit << "....     \n\n";
	rc = KA.GoAlongVit(Vit, _SK_ASKTE); if (rc) return rc;
	PrintMng.PrintMSC(&KA) ;

	// ���������� ����������������
	PrintMng.PostPrint(); 
	PrintMng.ClosePrint();
	// �������� ����������� ���������� ������������ �������
	PrintMng.ShowTextFile();
	PrintMng.ShowHtmlFile();


	return rc ;
}

//-------------------------------------------------------------------------------
