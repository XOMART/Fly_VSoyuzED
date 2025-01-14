//-----------------------------------------------------------------------------
// SamplPredictByTurnForSys2.cpp
// �. ��������� 14.06.2019
//
// ������������ 8-� �� ��� �� � 2-� ����������
//		W1 = 339 ����
//		W2 = 39  ����
// � �������� ����� ���������� ���� ���
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������
#include <FlyCoreLink.h>	// ����������� ���� ���������

int TaskTurnsPredictionSys2();

void main()
{
	// ����� �������� � ���������� ���������� 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// ������������� ���� Fly, ���������� ��������� ����� � ����������� 
	// �������������� ��������, ������������� ���������� ����������,
	// � ����������� �������� ������������� ���������� � ��...
	int rc = FlyGlobalInit(); if (rc) return;

	// ������ ��������������� �������� �� �� ������
	TaskTurnsPredictionSys2();
}

//-----------------------------------------------------------------------------
int RunUpSysEstimateNU(ZLSF& LSF, ZNU& NU0, ZNU& NUK, double uz, double e1z, double e2z, double az, double havrz = 0);

//-----------------------------------------------------------------------------
ZMSC_EXHV_BEG(ZModExtern2)
	double GSK[6];
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0);	RRC;
	double HBL[3];
	ZGRtoHBL(GSK, HBL, 0, 0);
	Yexe[0] = 1e4*HBL[0];
	return 0;
ZMSC_EXTFORCE(ZModExtern2)
	// ���� �������������� ����������� ��������
	Y[0] += 0.0;
	Y[1] += 0.0;
	Y[2] += 0.0;
	Y[N - 1] += 0.0;
ZMSC_END

#define PZModExtern2 ZTSelfRemovPointer<ZModExtern2>

//-----------------------------------------------------------------------------
int TaskTurnsPredictionSys2()
{
	int  j, rc = 0;
	long Vit;
	long Vitn = 100;	// ��������� ����� ������������� 
	long Vitk = 117;	// �������� ����� �������������

	PZCPrintManager PrintMng;
	PZModExtern2	KA[8];	// ������ ��������
	ZNU			    NU[8];	// ��������� �������

	//-------------------------------------------------------------------------
	// ����� ���
	ZLSF LSF; memset(&LSF, 0, sizeof(LSF));
	LSF.num = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 16;
	LSF.mgpz = 16;
	LSF.vatm = ATMSMA81; // ATMNULL ;
	LSF.vsvd = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDENULL;
	LSF.isDU = 0;
	LSF.isSun = 1;
	LSF.isMoon = 1;

	//-------------------------------------------------------------------------
	// ����� ���������� ������ - ���
	// ��������� ������������� ������
	double Tz, iz, havrz, az, pz, ez, e1z, e2z;
	// ������ �� ������� ������
	double Hs = 270;
	char   Msg[512];
	rc = MBuildSSOMIV_hs(Hs, Tz, iz, pz, e1z, e2z, Msg); RRC;
	ez = sqrt(e1z*e1z + e2z*e2z);
	az = pz / (1 - ez*ez);
	havrz = 270.0;

	printf("   Build Selection of 8 IC\n");
	//-------------------------------------------------------------------------
	// ������������ ������ �� 8 �� ���������� � ��������� �� 45 ��������
	for (j = 0; j<8; j++) {
		NU[j].Key.ka = j + 1; NU[j].Key.nom = 15; NU[j].Key.mod = 0;
		NU[j].Key.type = 8; NU[j].Key.bc = 1;
		NU[j].numlsf = 1;
		DATE dt = DATE(1, 4, 2018);
		TIME tm = TIME(9, 0, 0, 0);
		TA_DMB(1, &dt, &tm, &NU[j].t);
		NU[j].vit = 100;
		// ����������� �� - ��������� ��������� �� ����������� 
		// ����������� �������� ��� ���
		NU[j].sk = _SK_OSKTE;
		NU[j].X[0] = TO_DTKM(az);
		NU[j].X[1] = ez;
		NU[j].X[2] = TO_RAD(iz);
		if ((j / 2) * 2 == j)
			NU[j].X[3] = TO_RAD(339.555);
		else
			NU[j].X[3] = TO_RAD(39.551);
		NU[j].X[4] = TO_RAD(0);
		NU[j].X[5] = TO_RAD(1e-15 + 45.0*((double)j));
		NU[j].Sb = 0.001;
		NU[j].Kp = 0;
		NU[j].M0 = 900;
		NU[j].MF = 50;

		// �������� ��������� ���� �� � ����� �������� ��� ���� ��
		// ���������� ���������� (������� �������, �������������� � �������) 
		// ��� "�����" �� ������ ����� 
		{
			ZNU NUK;
			printf("\r");
			printf("      IC - %d", j + 1);
			rc = RunUpSysEstimateNU(LSF, NU[j], NUK, 0, e1z, e2z, 0, havrz); RRC;
			NU[j] = NUK;
		}

		// ����� �������������� �� �������� ��� ����� ��������������� ���� 
		// �������������� ���������������� ���������
		// !!!! �������� ����� ������ �� ������ !!!!
		KA[j]->SetExternalEquationsCount(1);
		// ������������� ������� ���������������� ��
		// ������� ���� 8 - �� ����� ��
		rc = KA[j]->Init(NU[j], &LSF); RRC;
		// � �������� ������������� �������� �������������� �������������� 
		// ��������� ����� ��� "��������" ��� � "���������"
		// � ������ ������ ��������� �� �� ��� ��� ������ ���-���� ����������.
		KA[j]->ExternalEquationsOn();
		KA[j]->ExternalEquationsOff();
	}
	printf("\n");

	// ������������ ������ ��������������� ����������
	PrintMng->MPK.AflatOn();
	PrintMng->MPK.FromVitOn();
	PrintMng->MPK << MODPR_KA;
	PrintMng->MPK << MODPR_VIT;
	PrintMng->MPK << MODPR_V_Hmin;
	PrintMng->MPK << MODPR_V_Hmax;
	PrintMng->MPK << MODPR_V_VUdate;
	PrintMng->MPK << MODPR_V_VUtime;
	PrintMng->MPK << MODPR_V_VUmssv;
	PrintMng->MPK << MODPR_V_NUmssv;
	PrintMng->MPK << MODPR_V_VUL;
	PrintMng->MPK << MODPR_V_NUtime;
	PrintMng->MPK << MODPR_V_NUL;
	PrintMng->MPK << MODPR_V_Tdr;
	PrintMng->MPK << MODPR_V_Hsr;
	PrintMng->MPK << MODPR_V_Hmin;
	PrintMng->MPK << MODPR_V_HminU;
	PrintMng->MPK << MODPR_V_Hmax;
	PrintMng->MPK << MODPR_V_HmaxU;
	PrintMng->MPK << MODPR_Aosk;
	PrintMng->MPK << MODPR_eosk;
	PrintMng->MPK << MODPR_T_wTE;
	PrintMng->MPK << MODPR_T_iTE;
	PrintMng->MPK << MODPR_T_RTE;
	PrintMng->MPK << MODPR_T_VTE;

	// ������������� ���������������� � �������� ���������. 
	PrintMng->PrePrint(&KA[0], "�������������� �����", "_TaskTurnsPredictionSys2");
	PrintMng->PrintHeaderDoc("\n   �������������� ����� � ������ ������ �� \"�\"\n\n");
	// ���������������� �� � ���
	for (j = 0; j<8; j++) PrintMng->PrintNU(NU[j], NULL, 3);
	PrintMng->PrintLSF(LSF, 1);
	// ������������ ���� �� �� ������ ���������� �����
	for (j = 7; j >= 0; j--) { rc = KA[j]->GoToVit(Vitn); RRC; }
	Vit = Vitn;

	// ��������������� �������� �� ������ � �������� ���� ������������� �����
	double t00 = -999;
	double Xexe;

	printf("\n\n");
	printf("   First turn........%d\n", Vitn);
	printf("   Last turn.........%d\n", Vitk);

	while (1) {
		for (j = 7; j >= 0; j--) {
			printf("\r");
			printf("   Turn..............%d  SC...%d", Vit, j + 1);
			// �������� ������� ���������������� ���������
			KA[j]->ExternalEquationsOn();
			// ������������� �������� ����� ��������� ����� 
			// � �������� ���� �������������
			// �������� �� �����
			rc = KA[j]->GoAlongVit(Vit); if (rc) break;
			// ��������� ������� ���������������� ���������
			KA[j]->ExternalEquationsOff(&Xexe);
			KA[j]->FV.HAvr = Xexe / (KA[j]->FV.Td / k_cbc);
			//
			PrintMng->PrintMSC(&KA[j]);
		}

		PrintMng->PrintLineMSC();
		Vit++;
		if (Vit>Vitk) break;
	}

	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// �������� ����������� ���������� ������������ �������
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	printf("\n\n");
	return rc;
}

//-----------------------------------------------------------------------------

