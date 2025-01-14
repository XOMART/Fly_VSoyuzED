//-----------------------------------------------------------------------------
// TestAgeValidation.cpp
// ������ ��������� ������� ������ ����� ��
//
// �. ��������� 25.04.2019
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ������ ��������

//-----------------------------------------------------------------------------
int TestAgeValidation()
{
	char NameNU1975[] = "NU01_314_Sverka_4_ASK1975.txt";
	char NameNU2000[] = "NU01_314_Sverka_4_ASK2000.txt";
	char NameNU0000[] = "NU01_314_Sverka_4_ASKTE.txt";
	//char NameNU1975[] = "NU01_268_Sverka_4_0004.txt";
	//char NameNU2000[] = "NU01_268_Sverka_4_0005.txt";
	//char NameNU0000[] = "NU01_268_Sverka_4_0003.txt";

	cout << endl << endl
		<< "  ������ ������� ���������� ��������� � �������"
		<< endl
		<< "  ���������� ������������� �� � ��������� ����� � ���������"
		<< endl
		<< "  ����������� ���������� � ��������"
		<< endl << endl;


	int rc = 0;
	// �� 
	ZNU NU1, NU2, NU3;

	// �������� �� �� ��� ��01 

	// �� � ��� 2000
	cout << "  �������� �� ���������� �� ������������ ���������, ������������"
		 << endl
		 << "  ����������� ���������� ������������ �������������� ����������."
		 << endl << endl;
	cout << "  ��1:" << endl;
	rc = LoadNU01(NameNU2000, NULL, NULL, NU1, nullptr, true);
	if (rc) return rc;
	NU1.M0 = 420.0;
	NU1.MF = 50.0;
	DATETIMEDMB DT; 
	DT << NU1.t;

	// �� � ��� 1975
	cout << "  ��2:" << endl;
	rc = LoadNU01(NameNU1975, NULL, NULL, NU2, nullptr, true);
	if (rc) return rc;
	NU2.M0 = 420.0;
	NU2.MF = 50.0;
	DT << NU2.t;

	// �� � ��� ��
	cout << "  ��3:" << endl;
	rc = LoadNU01(NameNU0000, NULL, NULL, NU3, nullptr, true);
	if (rc) return rc;
	NU3.M0 = 420.0;
	NU3.MF = 50.0;
	DT << NU3.t;
	cout << endl;

	ZNU NU11;
	NU11 = NU1;
	// �������������� �� �� ��� ����� 2000 � ��� ��
	ConvertSkNu(NU11, _SK_ASKTE);
	cout << "     ��� 2000 ---> ��� ��" << endl;
	cout << "     ���������� �� ������������������ ����������� ���� �� ��,"
		 << endl
		 << "     �������� � ��� �� � ���������� �� ������������"
		 << endl
		 << "     ������������� ���������"
		 << endl << endl;
	double dX[6];
	int    i;
	for (i = 0; i < 3; ++i) {
		// ���������� ��������� � �����������
		dX[i] = (NU11.X[i] - NU3.X[i])*1e9;
		// ���������� ��������� � ��/���
		dX[i + 3] = TO_KMS(NU11.X[i + 3] - NU1.X[i + 3])*1e6;
	}
	for (i = 0; i < 6; ++i) {
		cout << "     " << (i < 3 ? "[c�]    " : "[��/���]") << dX[i] << endl;
	}
	cout << endl;
	
	ZNU NU31;
	NU31 = NU3;
	// �������������� �� �� ��� ����� �� � ��� 1975
	ConvertSkNu(NU31, _SK_ASK1975);
	cout << "     ��� TE ---> ��� 1975" << endl;
	cout << "     ���������� �� ������������������ ����������� ���� �� ��,"
		 << endl
		 << "     �������� � ��� 1975 � ���������� �� ������������" 
		 << endl
		 << "     ������������� ���������"
		 << endl << endl;
	for (i = 0; i < 3; ++i) {
		// ���������� ��������� � �����������
		dX[i] = (NU31.X[i] - NU2.X[i])*1e9;
		// ���������� ��������� � ��/���
		dX[i + 3] = TO_KMS(NU31.X[i + 3] - NU2.X[i + 3])*1e6;
	}
	for (i = 0; i < 6; ++i) {
		cout << "     " << (i < 3 ? "[c�]    " : "[��/���]") << dX[i] << endl;
	}
	cout << endl;

	NU11 = NU1;
	// �������������� �� �� ��� ����� 2000 � ��� 1975
	ConvertSkNu(NU11, _SK_ASK1975);
	cout << "     ��� 2000 ---> ��� 1975" << endl;
	cout << "     ���������� �� ������������������ ����������� ���� �� ��,"
		<< endl
		<< "     �������� � ��� 1975 � ���������� �� ������������"
		<< endl
		<< "     ������������� ���������"
		<< endl << endl;
	for (i = 0; i < 3; ++i) {
		// ���������� ��������� � �����������
		dX[i] = (NU11.X[i] - NU2.X[i])*1e9;
		// ���������� ��������� � ��/���
		dX[i + 3] = TO_KMS(NU11.X[i + 3] - NU2.X[i + 3])*1e6;
	}
	for (i = 0; i < 6; ++i) {
		cout << "     " << (i < 3 ? "[c�]    " : "[��/���]") << dX[i] << endl;
	}
	cout << endl;

	return 0;
}

//-----------------------------------------------------------------------------