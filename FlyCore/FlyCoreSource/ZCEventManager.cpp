//-------------------------------------------------------------------------------
// ZCEventManager.cpp
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZCEventManager.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZHBL.h>
#include <FlyCoreSource\\ZCalc.h>
#include <FlyCoreSource\\ZNIP.h>

//------------------------------------------------------------------------------- 
// �������� ���� ������� 
//------------------------------------------------------------------------------- 
// �������� �� ������� �������� ���� �������������� (������� ����������� ����� ���)
bool IsZRVEvent (ModEventName e){ return (e>=ModEvent_ZRVBEG ? true:false) ; } 
// �������� �� ������� �������� ����� (������� ����������� ����� �����)
bool IsVITEvent (ModEventName e){ return ((e>=ModEvent_VITVU && e<=ModEvent_SHADOWEND) ? true:false) ; } 
// �������� �� ������� �������� ������������
bool IsGOEvent  (ModEventName e){ return ((e>=ModEvent_ONBREAKTIME && e<=ModEvent_ONARG) ? true:false) ; } 
// �������� �� ������� �������� ������������� ������ ������������ ���������
bool IsDUEvent  (ModEventName e){ return ((e>=ModEvent_DUON && e<=ModEvent_DUOFF) ? true:false) ; } 
bool IsUserEvent(ModEventName e){ return (e>=ID_USEREVENT_FIRST ? true:false) ; } 
bool IsModEvent (ModEventName e){ return ((e>=ModEvent_BEG && e<=ModEvent_END) ? true:false) ; } 

//-------------------------------------------------------------------------------
// ������������� ������� ��� (������, ����� ��� ��������)
int IDZRVEvent(ModEventName e)
{
	int k = (int)(e-ModEvent_ZRVBEG) ;
	// ����������� �������: ������, ����� ��� �������� ����
	ModEventName EN ;
	if ((k/3)*3 == k) EN = ModEvent_ZRVBEG ;
	else EN =((k+2)/3)*3==(k+2) ? ModEvent_ZRVEND : ModEvent_ZRVPARAM ; 
	return EN ;
}

//-------------------------------------------------------------------------------
// ������ ���� �� ������� ���
int IndNipZRVEvent(ModEventName e) 
{
	int k = (int)(e-ModEvent_ZRVBEG) ;
	return ((int)(k/3)) ;
}

//-------------------------------------------------------------------------------
// ��������� �������
//-------------------------------------------------------------------------------
ZCEventManager::ZCEventManager() : ZCEventCase()
{
}

//-------------------------------------------------------------------------------
ZCEventManager::~ZCEventManager()
{
}

//-------------------------------------------------------------------------------
int ZCEventManager::CalcParams(ZMSC* KA, ZSC_SI* SI, int indPar, double* P, double* D)
{
	if (!SI) SI = &KA->FSC ; 
	timet = SI->ta ;

	// �������� ������������ ��������� � ����������� �������� ��
	if (!SI || SI->E.ID==ModEvent_EMPTY) {
		assert(0) ;
	}

	// ��� ������������� ������������ ��������� ������� �����
	// ������ �����, ���������� ����, ����������� � ������������ ������ � ������
	if (isNeedTestVit) {
		// ����������� ���� THBLFR ��������� ZSC_SI � ������� � ��� ����� ��������
		// ���������� � ���������� ��������������� ������� �����: ������, ������, 
		// ���������� �����
		if (indPar<=0 || indPar==ModEvent_VITNU) {
			IP(ModEvent_VITNU) = true ; 
			// ���������� ����� ����� � �������� ������ ��� �������� �������
			// ����������� ����������� ���� ������
			PutVITParam(indPar, 0,-SI->THBLFR[8], ModEvent_VITNU, P, D) ;
		}

		if (indPar<=0 || (indPar>=ModEvent_HMIN && indPar<=ModEvent_RMAX)) {
			IP(ModEvent_HMIN) = true ; 
			IP(ModEvent_HMAX) = true ; 
			IP(ModEvent_RMIN) = true ; 
			IP(ModEvent_RMAX) = true ; 
			PutVITParam(indPar, SI->THBLFR[1], SI->THBLFR[6], ModEvent_HMIN, P, D) ;
			PutVITParam(indPar, SI->THBLFR[1],-SI->THBLFR[6], ModEvent_HMAX, P, D) ;
			PutVITParam(indPar, SI->THBLFR[5], SI->THBLFR[7], ModEvent_RMIN, P, D) ;
			PutVITParam(indPar, SI->THBLFR[5],-SI->THBLFR[7], ModEvent_RMAX, P, D) ;
	}	}

	// ��� ������������� ������������ ��������� ���
	// �������� ���� � ���, ����� � ������� ���������
	if (isNeedTestZRV) {
		bool isZRVEvent = IsZRVEvent((ModEventName)indPar) ;
		if ((indPar<0 && IsNeedTrackingZRV()) || isZRVEvent) {
			double Params[7] ;
			int    i ; 
			int    i0 = isZRVEvent ? IndNipZRVEvent((ModEventName)indPar) : 0 ;
			int    N  = isZRVEvent ? i0+1 : CountNips() ;
			// ������� �� �����
			for (i=i0; i<N; i++) {
				IP(ModEvent_ZRVBEG+i*3) = true ; 
				IP(ModEvent_ZRVEND+i*3) = true ; 
				IP(ModEvent_ZRVPARAM+i*3) = true ; 
				// ������ ���� � ������� �����
				int indNip = INips(i) ;
				// ������ ���������� ��������� ��������� ���-��
				CalcZRVParams(&ALLNIPS[indNip], SI->GSK, Params) ;
				// ��� ����� - ����������� �������� - ���� �����
				// �������������������� ���� �����
				double Gmin = ALLNIPS[indNip].MinGam ;
				CT(ModEvent_ZRVBEG+i*3) = Params[0]-Gmin ;
				PT(ModEvent_ZRVBEG+i*3) = Params[1] ;
				// ��� ������ - ����������� �������� - 
				// ���� ����� �� ������ �����
				// !!!!! ���� ����� ����� ��� ��� ������� ���������������� 
				// �����, ����� ����������� �������� ��������� �� 
				// ������������� � ������������� ���� !!!!!!!!
				CT(ModEvent_ZRVEND+i*3) =-(Params[0]-Gmin) ;
				PT(ModEvent_ZRVEND+i*3) =-Params[1] ;
				// ��� ��������� - ����������� �������� - 
				// �������� ��������� ����� ����� �� ������ �����
				CT(ModEvent_ZRVPARAM+i*3) =-Params[1] ;
				PT(ModEvent_ZRVPARAM+i*3) = 0 ;
				//
				if (isZRVEvent && D) {
					int ID = IDZRVEvent((ModEventName)indPar) ;
					if (ID==ModEvent_ZRVPARAM) *D =-Params[1] ;
					else {
						if (ID==ModEvent_ZRVEND) *D =-(Params[0]-Gmin) ;
						else *D =(Params[0]-Gmin) ;
					}
	}	}	}	}
	
	return 0 ;
}

//-------------------------------------------------------------------------------
void ZCEventManager::PutVITParam(int indI, double Pc, double Dc, int indC, double* P, double* D) 
{
	if (indI>0 && indI!=indC) return ;
	if (!D || !P) {	PT(indC) = Pc ;	CT(indC) = Dc ; }
	else { *P = Pc ; *D = Dc ; }
}

//-------------------------------------------------------------------------------
// ��� ��������� ������������� �������� �� ����� ����� ����� �������� �����
// �� ����� ���� �������������� ����������� ��� ��� �������: ���� ����� � ��������.
// ��� ���� �� ������ ���� � �� ����� ���� �������� ���� ����� ����� ������ 
// ������������ ��������, ��� ���������, �������� ���������� �� ����� ����� 
// ��������������� ��������� �� ���������. ������ ��� ������ ������ � ����� ������ 
// �������.
// ��������� �������� ������ ������� ��������� � ����� ����� ������ ������������,
// �� ���� ����� ������, �������� ������������ ��� ����������, � ������������ 
// �������� ���� ����� �� ������ � �� ����� ���� �������������� ������ ������������
// ��������, �� ���� ���� ���������.
// ������� ������������� ���������� ������������� ��������� ������ � ����� ��� � 
// �������� ������� OnEvent � OnFullZRV.
//
// ���� �������� ���� ����� � ������ ��������� ������ ������������ ��������, ��
// ��� �������� ��� ��� ���.
int ZCEventManager::TestZRV(ZMSC* KA, ZSC_SI& EVSI, int& i, int& j)
{
	int rc = 0 ;

	// ���� ������� �� �������� �������� ���, �� ������ �����
	if (!IsZRVEvent(ie[i])) return 0 ;
	// ��� ������� ��� (����, �����, ��������)
	ModEventName en = (ModEventName)IDZRVEvent(ie[i]) ;	

	// ������ ������ ����
	int indNum = IndNipZRVEvent(ie[i]) ;
	// ������ ���� � ������� ������������� � ��������� ���
	int indNip = INips(indNum) ;
	// ����� ����
	int NumNip = ALLNIPS[indNip].Num ;
	// ����������� ���������� ���� �����
	double Gmin= ALLNIPS[indNip].MinGam ;
	ZZRV*  Z = &ZRV[indNum] ; 

	// ���������� ������������� ��� �������������� ���
	// �����, ������ � ��������� ���
	switch(en) {
		case ModEvent_ZRVBEG:
			// ���������� �������������� ����� � ���
			Z->Vit= EVSI.VitTE ;
			Z->G1 = EVSI.E.Value ;
			Z->A1 = EVSI.E.Value2 ;
			Z->t1 = EVSI.E.ta ;
			Z->Nip= NumNip ;
			Z->KA = KA->NU.Key.ka ;
			Z->isFound|= 1 ;
			// ���������� ������� �������������� ��������� ��������
			memcpy(Z->X, EVSI.ASK1975, 6*sizeof(double)) ;
			Z->X[6] = EVSI.m ;
			//
			PostFindZRV(KA, EVSI, Z) ;
			break ;
		case ModEvent_ZRVPARAM:
			if (EVSI.E.Value<Gmin) {
				// ���� ���� ����� �� ��������� ������ ����������
				// �����������, �� ����� ��� ������������
				return-1 ;
			} else {
				if (!(Z->isFound&1 || Z->isFound&2) && CP(ie[i]-2)<Gmin && CT(ie[i]-1)<Gmin) {
					// ���������� "����������� ����"
					// � ���� ������ �� ���� �� ����� �� ��� �� ���� ���� 
					// �� ������������.
					// ��� �� ���������� ��� ������������ ������� �� ��� �����
					//	1 - �� ������ ���� �� ���������
					//  2 - �� ��������� �� ����� ����
					// � ������ �� ���� ������ ����� �������� ���������� 
					// �������������� ���� � ����� �� ���

					//-----------------------------------------------------------
					// ����� �� ��������� �� ����� ����
					double Gmax = EVSI.E.Value ;
					double tp   = EVSI.E.ta ;
					double dh   = timet-tp ; 

					// timet     - ����� �� ������� ���� �������������� �� ������
					//             �� ������ ���������� ������� 
					//             ZCEventCase ==> ZCEventManager.
					// EVSI.E.ta - ����� ������������� � ����������� ������� (���������).
					//             EVSI - ��������� � ���������� �������� �� �� ������ �������
					//             EVSI.E - ��������� � ���������������� �������
					//             ������ ����� EVSI.E.ta ����������� � ��������������� ���
					//             ��� ����� ����������� ��������� Z->tp.

					//-----------------------------------------------------------
					// ������� ������� ����� � ���

					// ��������� �������� ���� ����� � ����� ����
					// ie[i]   - ������������� ������� - ��������
					// ie[i-1] - ������������� ������� - ����� ��� ���
					// ie[i-2] - ������������� ������� - ���� � ���
					// �� ������ ����������� ����������� ��� 
					// ie[i-1] = ie[i-2] = ModEvent_EMPTY, �� ���� ���� � ����� �� 
					// ���������� - � ���� � ���� ���� ����������� ���
					double OldCT = CT(ie[i]-2) ;
					double oldtt = timet ;
					// ���������� ������� ������������� ������� - ���� � ���
					//
					// ���������� ������������ ��������� ��� ������ ��������� �� ��� �����
					// � ������ ��� ������� - ������ � ����� ��� - �� ������� ������� �� ����
					// �������������� ������ �� ������� �������� ��������� ����� ���������������
					// ��������� �� �������.
					// �� ������ ���� ���� ����� ���� ���������, �� ��������� - ����, �������������
					// ����� ����� ������� ������� ������ ���
					// ���������� ��� ����� ���...
					CT(ie[i]-2) = Gmax-Gmin ;			// �������� ��������������� ���������
					timet = tp ;						// �����
					ie[j] = (ModEventName)(ie[i]-2) ;	// ����� "���������" ������� - ������ ��� 
					// �������� ������ ����� � ���
					ZSC_SI SI ;
					rc = ClarifyEvent(KA, ie[j], &SI, &dh) ; RRC ;
					// �������� ������������� ������� ������������� ������� - ���� � ���
					CT(ie[i]-2) = OldCT ;		// ��������������� �������� ���� ����� �� ����� ����
					timet = oldtt ;				// ��������������� ������� �����
					ie[j] = ModEvent_EMPTY ;	// �������� ������������� "����������" �������
					// ���� �� "��������" ������������� "����������" �������, �� ��� ���������� �������
					// � ������ �� �� � ����� ��������� ������������ �������, ���� � 
					// ��� ����� ���������� ��������, ��� ������� � ������!!!!
					//
					// ���������� �������������� ����� � ���
					Z->Vit= SI.VitTE ;
					Z->G1 = SI.E.Value ;
					Z->A1 = SI.E.Value2 ;
					Z->t1 = SI.E.ta ;
					Z->Nip= NumNip ;
					Z->KA = KA->NU.Key.ka ;
					Z->isFound|= 1 ;
					// ���������� ������� �������������� ��������� ��������
					memcpy(Z->X, SI.ASK1975, 6*sizeof(double)) ;
					Z->X[6] = SI.m ;
					// ����� ������� ��� ������� �� ���� � ���
					KA->OnEvent(SI.E, &SI) ;
					j++ ; 

					//-----------------------------------------------------------
					// ����������� ���� ����������� ���������
					// ���������� �������������� ����������� ��������� ���
					Z->Gmax = EVSI.E.Value ;
					Z->Ap   = EVSI.E.Value2 ;
					Z->tp   = EVSI.E.ta ;
					Z->isFound|= 2 ;
					// ����� ������� ��� ������� �� ���������� ���������
					KA->OnEvent(EVSI.E, &EVSI) ;

					//-----------------------------------------------------------
					// ������� ������� ������ �� ���

					// ��������� �������� ���� ����� �� ������ ����
					double OldCP = CP(ie[i]-1) ;
					double oldtp = timep ;
					// ���������� ������� ������������� ������� - ����� �� ���
					CP(ie[i]-1) =-(Gmax-Gmin) ;
					timep = tp ;
					ie[j] = (ModEventName)(ie[i]-1) ; 
					// �������� ������ ������ �� ���
					rc = ClarifyEvent(KA, ie[j], &SI, nullptr) ; RRC ;
					// �������� ������������� ������� ������������� ������� - ����� �� ���
					CP(ie[i]-1) = OldCP ;	  // ��������������� �������� ���� ����� � ����� �� ������ ����
					timep = oldtp ;			  // ��������������� ����� �� ������ ����
					ie[j] = ModEvent_EMPTY ;  // �������� ������������� "����������" �������
					// ���� �� "��������" ������������� "����������" �������, �� ��� ���������� �������
					// � ������ �� �� � ����� ��������� ������������ �������, ����� �� 
					// ��� ����� ���������� ��������, ��� ������� � ������!!!!
					//
					// ���������� �������������� ������ �� ���
					Z->G2 = SI.E.Value ;		// ���� ����� �� ���������, ����	
					Z->A2 = SI.E.Value2 ;		// ������ �� ���������, ����
					Z->t2 = SI.E.ta ;			// ����� ����������� ���������, ���
					Z->isFound|= 4 ;			// ������� ���������� ������� "��������"
					// 
					PostFindZRV(KA, SI, Z) ;
					j++ ;
					return-1 ;
				} else {
					// ����������� ���� ����������� ���������
					// ���������� �������������� ����������� ��������� ���
					Z->Gmax = EVSI.E.Value ;
					Z->Ap   = EVSI.E.Value2 ;
					Z->tp   = EVSI.E.ta ;
					Z->isFound|= 2 ;
					//
					PostFindZRV(KA, EVSI, Z) ;
				}
			}
			break ;
		case ModEvent_ZRVEND:
			// ���������� �������������� ������ �� ���
			Z->G2 = EVSI.E.Value ;
			Z->A2 = EVSI.E.Value2 ;
			Z->t2 = EVSI.E.ta ;
			Z->isFound|= 4 ;
			//
			PostFindZRV(KA, EVSI, Z) ;
			break ;
		default: assert(0) ;
	}

	// �������� ������������ ������� ����������� ��������� 	
	if (en==ModEvent_ZRVPARAM) {
		// ������� ���������� �� ��� "����������" � ������
		// ����������� ���������
		if (EVSI.E.Value<Gmin) {
			// ���� ���� ����� �� ��������� ������ ����������
			// �����������, �� ����� ��� ������������
			return-1 ;
	}	}

	return 0 ;
}

//-------------------------------------------------------------------------------
// ��������� ������������� ��� � ������ ZF ������-���
// ��� ������ OnEvent ��� �������������� ��� ��� ������������
// � �������������� � ����������. � ���� ��� ����� �������
// ����� OnFullZRV, ���� ������� �������� ��� ����������� 
// �������������� ���.
void ZCEventManager::PostFindZRV(ZMSC* KA, ZSC_SI& SI, ZZRV* Z)
{
	ZZRV ZF =*Z ;
	// ���������� ����������� ������� �� ��������� �������
	// (����� ������� ��� ������� �� ��������� ������� ���)
	KA->OnEvent(SI.E, &SI) ;
	// ������� �� ������ ��������� ���...
	if (ZF.isFound==7) {
		// ������� �� ������ ��������� ���
		KA->OnFullZRV(&ZF, &SI) ;
		// ��������� ������
		memset(Z, 0, sizeof(ZZRV)) ;
		Z->Nip =-1 ;
	}
}

//-------------------------------------------------------------------------------
int ZCEventManager::TestEvents(ZMSC* KA, ZSC_SI* SI)
{
	// ���� ��� ������������� �������� ������� �� ������ �������� �� �������
	if (!isNeedTestZRV && !isNeedTestVit) return 0 ;
	// ������� �������� �������� ��
	if (!SI) SI = &KA->FSC ;

	// ��������� ����������� ����� ��������� �������� ����
	PreTest() ;
	// ����������� ��� �������������� ��������� ��� �������� ����
	// � ��� ����� ��������� �������������� ������� �����  
	// (���� � ��� ������ � ������, ������ ����� � ���������� ����), 
	// � ��������� �������������� ������� ��� (����, �����, ��������)
	int rc = CalcParams(KA, SI) ; RRC ;

	//---------------------------------------------------------------------------
	// �������� ����������� �������������� ������� �����
	// (�������� � ������������ ������ � ������, ���������� � ����������
	// ���� ������)
	int i, j=0 ;
	if (isNeedTestVit) {
		// �������� �������������� ����������, � ������ ������� ����� 
		// ������ �� ��� ������ ���� � ������ �� ����
		for(i=ModEvent_BEG ; i<=ModEvent_END ; i++) {
			if (IP(i) && CP(i)<=0 && CT(i)>=0) {
				ie[j] = (ModEventName)i ; 
				j++ ; 
	}	}	}

	//---------------------------------------------------------------------------
	// �������� ���������� ���
	if (isNeedTestZRV) {
		for(i=ModEvent_ZRVBEG ; i<ModEvent_ZRVBEG+3*CountNips() ; i++) {
			if (IP(i) && CP(i)<=0 && CT(i)>=0) {
				ie[j] = (ModEventName)i ; 
				j++ ; 
	}	}	}

	//---------------------------------------------------------------------------
	// ��������� ���� ��������� �������������� ����������, 
	// �������� � �������� ����������� ���� ���� ���� � ������ �� ����
	ZSC_SI EVSI ;
	for(i=0; i<COUNT_ModEvents; i++) {
		// ���� ��������� ������������� ������� �������, ������ �� ����
		// ��� ������ �� ������ �������
		if (!ie[i]) break ;
		// ��������� ��������� ���������� �������� �� � ������ �����������
		// ����������� �������
		memset(&EVSI, 0, sizeof(EVSI));

		//-----------------------------------------------------------------------
		// ��������� ������� ����������� �������, ����� 
		// �������������� �������� ��������� �� ������������� 
		// ������� �������� � �������������.
		rc = ClarifyEvent(KA, ie[i], &EVSI) ; RRC ;
		//-----------------------------------------------------------------------
		// �������� ������� ��������� ��� ���� "���������"
		// �������� "�����������" ���
		// �������� ������� �� ���� ���� ��� ����� ������� ���...
		rc = TestZRV(KA, EVSI, i, j) ;
		if (rc) {
			// ���� ������� �������...
			if (rc==-1) { 
				// ���� ������� ��� ���������� � �������� (rc=-1), 
				// �� � ������ �-�� TestZRV ��������������� ������� 
				// ������� �� ������� ��� �������
				rc = 0 ; 
				continue ; 
			}
			return rc ;
		}

		//-----------------------------------------------------------------------
		// ���������� ����������� ������� �� ��������� �������
		KA->OnEvent(EVSI.E, &EVSI) ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZCEventManager::ClarifyEvent(ZMSC* KA, ModEventName indEvent, ZSC_SI* SI, double* ddh)
{
	int    rc=0 ;
	int    MaxCountIter=100 ;
	double eFSCZ=1e-6/1e4 ;			// �������� �� ����������
	double eFSCT=0.001/86400.0 ;	// �������� �� �������

	// ��������� ��������� ������ ��������
	// (������ ������������� ������)
	KA->FastStory() ;

	double Zt=CT(indEvent) ; 
	double Zp=CP(indEvent) ;
	double tt=timet ;
	double tp=timep ;

	// �������� ������ ������ �����
	int CountIter=0 ; 
	double t, dht, ht=0, dZ=Zt, dZP ;
	//
	if (ddh) ht =-(*ddh) ;
	// ������������ ������� ��������� ������� �������
	while (1) {
		// �������� �� ������� ������ �����
		dht=-dZ/(Zt-Zp)*(tt-tp) ;
		// �������� ����� ������ �����
		ht+= dht ;
		// ������������ �� ����������� ������� ����������� �������� ��������
		// �������������� ����������
		rc = KA->BackStep(SI, ht, indEvent) ; if (rc) goto end ;
		// ������ ��������������� ���������
		CalcParams(KA, SI, indEvent, &dZP, &dZ) ;
		t  = SI->ta ;
		// ��������� ��������� - �������� ����� ��� ������ ������� 
		if (dZ*Zt>= 0) { Zt = dZ ; tt = t ; }
		else { Zp = dZ ; tp = t ; }
		// �������� ������� ���������� 
		if (fabs(dZ)<eFSCZ || fabs(dht)<eFSCT) {
			// �������������� �������� ������ ���� ������� ������ ����
			if (dZ<0) {
				// ���� �� ����������� �� ����������� ������� �����
				dht= dZ/(Zp-Zt)*(tt-tp) ;
				ht+= 2*dht ;
				// ��������������� ��������� ������
				KA->FastReStory() ;
				// ���������� �����
				rc = KA->BackStep(SI, ht, indEvent) ; if (rc) goto end ;
				// ������ ��������������� ���������
				rc = CalcParams(KA, SI, indEvent, &dZP, &dZ) ; if (rc) goto end ;
			}
			break ;
		} 
		// ����������� ������� ����� ��������
		CountIter++ ;
		// �������� ����������� ��������
		if (CountIter>MaxCountIter) { rc =-2 ; goto end ; }
		// ��������������� ��������� �� �� ������ ���������� (�� ���������) ����
		// (������ ������������� ������)
		KA->FastReStory() ;
	}

end: ;
	// ��������������� ��������� ������
	KA->FastReStory() ;
	// ��������������� �������� ��������������� ���������
	// �� ������ ���������� ����
	CalcParams(KA, &KA->FSC, indEvent, nullptr, nullptr) ;

	// ��� �������� ���������� ��������� ������� ������ ������� � �����
	// ��� ��������� �������� �� ����� ������ ������� ���������� � ��������� 
	// ZSC_SI* SI
	// ��������� ������ ������������� ��������� �� ������ ���������

	//�������� ������������ ������������ ������� (������ �����)
	if (indEvent==ModEvent_VITVU) {
		if (SI->ASKTE[2]<0) {
			SI->VitTE++ ;
			SI->ASKTE[2] = 0.0 ;
		}
		SI->E.Value = (double)SI->VitTE ;
	}

	return rc ;
}

//-------------------------------------------------------------------------------
