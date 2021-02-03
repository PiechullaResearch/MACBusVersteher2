// MACBusVersteherView.cpp : implementation of the CMACBusVersteherView class
//

// WP20210203: Das ist eine Pseudo-Änderung zum Testen

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MACBusVersteher.h"
#endif

#include "MACBusVersteherDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "MACBusVersteherView.h"
#include <math.h>
#include <float.h>

//#include <malloc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int HexString2Int(char *s);
static int Hex2Dec(char hex);

// CMACBusVersteherView

IMPLEMENT_DYNCREATE(CMACBusVersteherView, CRichEditView)

BEGIN_MESSAGE_MAP(CMACBusVersteherView, CRichEditView)
	ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMACBusVersteherView construction/destruction

CMACBusVersteherView::CMACBusVersteherView()
{
	// TODO: add construction code here
	m_bInsideText = false;
	m_bLastCharWasETX = false;

	m_KnownSlaves[0].strName = _T("MAC_LMS");
	m_KnownSlaves[0].iUnmaskedNumber = 30;
	m_KnownSlaves[0].strRole = _T("EADR");
	m_KnownSlaves[0].colorref = RGB(255, 137, 0);
	m_KnownSlaves[0].nOpenTransactions = -1; // Magic number for initial state, do not accept a close before an open
	m_KnownSlaves[0].dwTransactions = 0;
	
	m_KnownSlaves[1].strName = _T("MAC_PCS");
	m_KnownSlaves[1].iUnmaskedNumber = 28;
	m_KnownSlaves[1].strRole = _T("EADR");
	m_KnownSlaves[1].colorref = RGB(231, 72, 86);
	m_KnownSlaves[1].nOpenTransactions = -1;
	m_KnownSlaves[1].dwTransactions = 0;
	
	m_KnownSlaves[2].strName = _T("MAC_GdsWls1"); 
	m_KnownSlaves[2].iUnmaskedNumber = 1;
	m_KnownSlaves[2].strRole = _T("EADR");
	m_KnownSlaves[2].colorref = RGB(0, 120, 215);
	m_KnownSlaves[2].nOpenTransactions = -1;
	m_KnownSlaves[2].dwTransactions = 0;
	
	m_KnownSlaves[3].strName = _T("MAC_GdsWls2"); 
	m_KnownSlaves[3].iUnmaskedNumber = 2;
	m_KnownSlaves[3].strRole = _T("EADR");
	m_KnownSlaves[3].colorref = RGB(0, 153, 188);
	m_KnownSlaves[3].nOpenTransactions = -1;
	m_KnownSlaves[3].dwTransactions = 0;
	
	m_KnownSlaves[4].strName = _T("MAC_GdsWls3"); 
	m_KnownSlaves[4].iUnmaskedNumber = 3;
	m_KnownSlaves[4].strRole = _T("EADR");
	m_KnownSlaves[4].colorref = RGB(122, 117, 116);
	m_KnownSlaves[4].nOpenTransactions = -1;
	m_KnownSlaves[4].dwTransactions = 0;
	
	m_KnownSlaves[5].strName = _T("MAC_GdsWls4"); 
	m_KnownSlaves[5].iUnmaskedNumber = 4;
	m_KnownSlaves[5].strRole = _T("EADR");
	m_KnownSlaves[5].colorref = RGB(142, 140, 216);
	m_KnownSlaves[5].nOpenTransactions = -1;
	m_KnownSlaves[5].dwTransactions = 0;

	m_KnownSlaves[6].strName = _T("MAC_GdsWls5"); 
	m_KnownSlaves[6].iUnmaskedNumber = 5;
	m_KnownSlaves[6].strRole = _T("EADR");
	m_KnownSlaves[6].colorref = RGB(0, 135, 195);
	m_KnownSlaves[6].nOpenTransactions = -1;
	m_KnownSlaves[6].dwTransactions = 0;
	
	m_KnownSlaves[7].strName = _T("MAC_GdsWls6"); 
	m_KnownSlaves[7].iUnmaskedNumber = 6;
	m_KnownSlaves[7].strRole = _T("EADR");
	m_KnownSlaves[7].colorref = RGB(104, 118, 138);
	m_KnownSlaves[7].nOpenTransactions = -1;
	m_KnownSlaves[7].dwTransactions = 0;
	
	m_KnownSlaves[8].strName = _T("MAC_GdsWls7"); 
	m_KnownSlaves[8].iUnmaskedNumber = 7;
	m_KnownSlaves[8].strRole = _T("EADR");
	m_KnownSlaves[8].colorref = RGB(202, 80, 16);
	m_KnownSlaves[8].nOpenTransactions = -1;
	m_KnownSlaves[8].dwTransactions = 0;

	m_KnownSlaves[9].strName = _T("MAC_LMS"); // TODO: Where the fuck in the embedded code are the SADRs computed (fucking assembler?)
	m_KnownSlaves[9].iUnmaskedNumber = 62; // 30 + 32;
	m_KnownSlaves[9].strRole = _T("SADR");
	m_KnownSlaves[9].colorref = RGB(255, 137, 0);
	m_KnownSlaves[9].nOpenTransactions = -1;
	m_KnownSlaves[9].dwTransactions = 0;
	
	m_KnownSlaves[10].strName = _T("MAC_PCS");
	m_KnownSlaves[10].iUnmaskedNumber = 60; // 28 + 32;
	m_KnownSlaves[10].strRole = _T("SADR");
	m_KnownSlaves[10].colorref = RGB(231, 72, 86);
	m_KnownSlaves[10].nOpenTransactions = -1;
	m_KnownSlaves[10].dwTransactions = 0;
	
	m_KnownSlaves[11].strName = _T("MAC_GdsWls1"); 
	m_KnownSlaves[11].iUnmaskedNumber = 33; // 1 + 32;
	m_KnownSlaves[11].strRole = _T("SADR");
	m_KnownSlaves[11].colorref = RGB(0, 120, 215);
	m_KnownSlaves[11].nOpenTransactions = -1;
	m_KnownSlaves[11].dwTransactions = 0;
	
	m_KnownSlaves[12].strName = _T("MAC_GdsWls2"); 
	m_KnownSlaves[12].iUnmaskedNumber = 34; // 2 + 32;
	m_KnownSlaves[12].strRole = _T("SADR");
	m_KnownSlaves[12].colorref = RGB(0, 153, 188);
	m_KnownSlaves[12].nOpenTransactions = -1;
	m_KnownSlaves[12].dwTransactions = 0;
	
	m_KnownSlaves[13].strName = _T("MAC_GdsWls3"); 
	m_KnownSlaves[13].iUnmaskedNumber = 35; // 3 + 32;
	m_KnownSlaves[13].strRole = _T("SADR");
	m_KnownSlaves[13].colorref = RGB(122, 117, 116);
	m_KnownSlaves[13].nOpenTransactions = -1;
	m_KnownSlaves[13].dwTransactions = 0;
	
	m_KnownSlaves[14].strName = _T("MAC_GdsWls4"); 
	m_KnownSlaves[14].iUnmaskedNumber = 36; // 4 + 32;
	m_KnownSlaves[14].strRole = _T("SADR");
	m_KnownSlaves[14].colorref = RGB(142, 140, 216);
	m_KnownSlaves[14].nOpenTransactions = -1;
	m_KnownSlaves[14].dwTransactions = 0;

	m_KnownSlaves[15].strName = _T("MAC_GdsWls5"); 
	m_KnownSlaves[15].iUnmaskedNumber = 37; // 5 + 32;
	m_KnownSlaves[15].strRole = _T("SADR");
	m_KnownSlaves[15].colorref = RGB(0, 135, 195);
	m_KnownSlaves[15].nOpenTransactions = -1;
	m_KnownSlaves[15].dwTransactions = 0;
	
	m_KnownSlaves[16].strName = _T("MAC_GdsWls6"); 
	m_KnownSlaves[16].iUnmaskedNumber = 38; // 6 + 32;
	m_KnownSlaves[16].strRole = _T("SADR");
	m_KnownSlaves[16].colorref = RGB(104, 118, 138);
	m_KnownSlaves[16].nOpenTransactions = -1;
	m_KnownSlaves[16].dwTransactions = 0;
	
	m_KnownSlaves[17].strName = _T("MAC_GdsWls7"); 
	m_KnownSlaves[17].iUnmaskedNumber = 39; // 7 + 32;
	m_KnownSlaves[17].strRole = _T("SADR");
	m_KnownSlaves[17].colorref = RGB(202, 80, 16);
	m_KnownSlaves[17].nOpenTransactions = -1;
	m_KnownSlaves[17].dwTransactions = 0;

	RoleDescription rd; // WP20200818: rd is never used at the moment

	rd.ID = 0;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< LMS");
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 1;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> LMS");
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 2;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_1"); // BlockMB0002
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 3;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_1");
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 4;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_1"); // BlockMB0004
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 4;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_1"); // << BlockMB0004_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 5;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_1"); // >> BlockMB0005_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 5;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_1"); // BlockMB0005
	m_RoleDescriptions.AddTail(rd);
	
	rd.ID = 6;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_1"); // BlockMB0006
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 6;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_1"); // BlockMB0006_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 7;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_1"); // BlockMB0007_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 7;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_1"); // BlockMB0007
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 8;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_2"); // BlockMB0008
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 9;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_2"); // BlockMB0009
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 10;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_2"); // BlockMB0010
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 10;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_2"); // BlockMB0010_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 11;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_2"); // BlockMB0011_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 11;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_2"); // BlockMB0011
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 12;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_2"); // BlockMB0012
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 12;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_2"); // BlockMB0012_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 13;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_2"); // BlockMB0013
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 13;
	rd.send_direction = SEND_DIRECTION::UNDEFINED;
	rd.strDescription = _T("XX GDS_WLS_2"); // BlockMB0013_NK6 "WP20190718: Block defined by KS not used in master" [!!!]
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 14;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_3"); // BlockMB0014
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 15;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_3"); // BlockMB0015_NK6
	m_RoleDescriptions.AddTail(rd);
	
	rd.ID = 15;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_3"); // BlockMB0015
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 16;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_3"); // BlockMB0016
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 16;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_3"); // BlockMB0016_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 17;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_3"); // BlockMB0017_NK6 "WP20190718: Corrected wrongly defined block (GDS WLS 3 SND block)" [!!!]
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 17;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_3"); // BlockMB0017
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 18;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_3"); // BlockMB0018
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 18;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_3"); // BlockMB0018_NK6
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 19;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_3");
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 19;
	rd.send_direction = SEND_DIRECTION::UNDEFINED;
	rd.strDescription = _T("XX GDS_WLS_3"); // BlockMB0019_NK6 "WP20190718: Block defined by KS not used in master" [!!!]
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 20;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_4"); // BlockMB0020
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 21;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_4"); // BlockMB0021
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 22;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_4"); // BlockMB0022
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 22;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_4"); // BlockMB0022_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 23;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_4"); // BlockMB0023
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 23;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_4"); // BlockMB0023_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 24;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_4"); // BlockMB0024
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 24;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_4"); // BlockMB0024_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 25;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_4"); // BlockMB0025
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 25;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_4"); // BlockMB0025_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 26;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_5"); // BlockMB0026
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 27;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_5"); // BlockMB0027
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 28;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_5"); // BlockMB0028
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 28;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_5"); // BlockMB0028_NK6 "WP20190718: unknown to Forth MACs" 
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 29;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_5"); // BlockMB0029
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 29;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_5"); // BlockMB0029_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 30;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_5"); // BlockMB0030
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 30;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_5"); // BlockMB0030_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 31;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_5"); // BlockMB0031
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 31;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_5"); // BlockMB0031_NK6 "WP20190718: unknown to Forth MACs" 
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 32;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_6"); // BlockMB0032
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 33;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_6"); // BlockMB0033
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 34;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_6"); // BlockMB0034
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 34;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_6"); // BlockMB0034_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 35;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_6"); // BlockMB0035
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 35;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_6"); // BlockMB0035_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 36;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_6"); // BlockMB0036
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 36;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_6"); // BlockMB0036_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 37;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_6"); // BlockMB0037
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 37;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T("X> GDS_WLS_6"); // BlockMB0037_NK6 "WP20190718: unknown to Forth MACs" 
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 38;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_7"); // BlockMB0038
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 39;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_7"); // BlockMB0039
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 40;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_7"); // BlockMB0040GDS
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 40;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_7"); // BlockMB0040_NK6 "WP20190718: unknown to Forth MACs" 
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 41;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_7"); // BlockMB0041GDS
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 41;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_7"); // BlockMB0041_NK6 "WP20190718: unknown to Forth MACs" 
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 42;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_7"); // BlockMB0042GDS
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 42;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<X GDS_WLS_7"); // BlockMB0042_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 43;
	rd.send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	rd.strDescription = _T("<< GDS_WLS_7"); // BlockMB0043
	m_RoleDescriptions.AddTail(rd);

	rd.ID = 43;
	rd.send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	rd.strDescription = _T(">> GDS_WLS_7"); // BlockMB0043_NK6 "WP20190718: unknown to Forth MACs"
	m_RoleDescriptions.AddTail(rd);

	/*rd.ID = ;
	rd.send_direction = 
	rd.strDescription = _T("");
	m_RoleDescriptions.AddTail(rd);

	rd.ID = ;
	rd.send_direction = 
	rd.strDescription = _T("");
	m_RoleDescriptions.AddTail(rd);*/
}

CMACBusVersteherView::~CMACBusVersteherView()
{
}

BOOL CMACBusVersteherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CMACBusVersteherView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));

	// Set your default font here
	SendMessage(WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT));

	// Turn on the horizontal scroll bar
	m_nWordWrap = WrapNone;
	WrapChanged();

	Parse(GetDocument()->m_RawLogfileBuffer);
}


// CMACBusVersteherView printing

BOOL CMACBusVersteherView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CMACBusVersteherView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}



// CMACBusVersteherView diagnostics

#ifdef _DEBUG
void CMACBusVersteherView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CMACBusVersteherView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CMACBusVersteherDoc* CMACBusVersteherView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMACBusVersteherDoc)));
	return (CMACBusVersteherDoc*)m_pDocument;
}
#endif //_DEBUG


// CMACBusVersteherView message handlers


// Core functionality

void CMACBusVersteherView::Parse(const char* pLog)
{
	CRichEditCtrl& crec = GetRichEditCtrl();
	crec.LimitText(0xFFFFFFE);
	char oneLine[256];
	ZeroMemory(oneLine, 256);
	unsigned int linecount = 0;
	CString strCurrentTimestamp = _T("");
	CString strLastTimestamp = _T("");

	m_BlockExchangeTrace.RemoveAll();
	ASSERT(m_BlockExchangeTrace.IsEmpty()); // Paranoid :-)

	if (pLog != NULL)
	{
		bool bSuppressFullAnalysis = false;
		
		int answer = ::MessageBox(NULL, _T("Nur Handshake ausgeben"), _T("Analyseumfang"), MB_ICONQUESTION | MB_YESNO);

		if (answer == IDYES)
		{
			bSuppressFullAnalysis = true;
		}
		
		// If crec is already populated, empty it before parsing a new pLog buffer
		crec.SetSel(0, -1);
		crec.Clear();

		if (!bSuppressFullAnalysis)
		{
			AppendToLogAndScroll(crec, _T("Master data are green\n"), RGB(0, 128, 0));
			AppendToLogAndScroll(crec, _T("Slave data are red\n"), RGB(128, 0, 0));
			AppendToLogAndScroll(crec, _T("\n"), RGB(0, 0, 0));
			AppendToLogAndScroll(crec, _T("Handshake:\n"), RGB(0, 0, 0));

			for (int i = 0; i < (NUM_KNOWN_SLAVE_ADDRESSES/2); i++)
			{
				CString strMsg = _T("");
				strMsg.Format(_T(">> Sending to slave %s\n"), m_KnownSlaves[i].strName);
				AppendToLogAndScroll(crec, strMsg, m_KnownSlaves[i].colorref);
				strMsg.Format(_T("<< Receiving from slave %s\n"), m_KnownSlaves[i].strName); 
				AppendToLogAndScroll(crec, strMsg, m_KnownSlaves[i].colorref);
			}

			AppendToLogAndScroll(crec, _T("\n"), RGB(0, 0, 0));
		}
		
		char* pLogWalker = const_cast<char*>(pLog);
		
		// CString strDummy = AnalizeToken(_T("Dummy"), true, false); // Called for initialization Not very useful, output already suppressed

		ACTIVE_SPEAKER last_speaker = ACTIVE_SPEAKER::UNDEFINED;
		ACTIVE_SPEAKER active_speaker = ACTIVE_SPEAKER::UNDEFINED;
		m_activeSlave = SLAVE::UNKNOWN;
		m_nErrorsFound = 0;
		
		while (*pLogWalker != _T('\0'))
		{
			ZeroMemory(oneLine, 256);
			int pos = 0;

			while (*pLogWalker != _T('\n'))
			{
				if (*pLogWalker == _T('\0'))
				{
					goto DONE;
				}
				
				oneLine[pos] = *pLogWalker;
				pos++;
				pLogWalker++;
			}

			oneLine[pos] = *pLogWalker;
			pos++;
			pLogWalker++;
			linecount++;

			COLORREF colorref = RGB(0, 0, 0);
			CString strOneLine4Output = _T("");
			strOneLine4Output.Format(_T("%07u\t%s"), linecount, oneLine);
			CString strOneLine4Analysis = _T("");
			strOneLine4Analysis.Format(_T("%s"), oneLine);

			if (-1 != strOneLine4Output.Find(_T(",A,")))
			{
				colorref = RGB(128, 0, 0);
			}
			else if (-1 != strOneLine4Output.Find(_T(",B,")))
			{
				colorref = RGB(0, 128, 0);
			}
			
			int loc = strOneLine4Output.Find(_T('\r'));
			
			if (loc != -1 && colorref != RGB(0, 0, 0))
			{
				strOneLine4Output.Delete(loc, 2); // Removal of \r\n (if the file has newlines in this format)
			}

			// WP20200908: Just \n is also possible (msb-rs485-plus delivers this format)
			loc = strOneLine4Output.Find(_T('\n'));

			if (loc != -1 && colorref != RGB(0, 0, 0))
			{
				strOneLine4Output.Delete(loc, 1); // Removal of \r\n (if the file has newlines in this format)
			}
			
			last_speaker = active_speaker;
			
			if (-1 != strOneLine4Analysis.Find(_T(",A,")))
			{
				active_speaker = ACTIVE_SPEAKER::COMPORT_A;
				strLastTimestamp = strCurrentTimestamp;
				strCurrentTimestamp = GetTimestamp(strOneLine4Analysis);
			}
			else if (-1 != strOneLine4Analysis.Find(_T(",B,")))
			{
				active_speaker = ACTIVE_SPEAKER::COMPORT_B;
				strLastTimestamp = strCurrentTimestamp;
				strCurrentTimestamp = GetTimestamp(strOneLine4Analysis);
			}

			//if (last_speaker != active_speaker && !bSuppressFullAnalysis)
			//{
			//	// AppendToLogAndScroll(crec, _T("\n"), RGB(0, 0, 0)); wrong!
			//	AppendToInterimBuffer(_T("\n"), colorref);
			//}

			if (colorref == RGB(128, 0, 0) || colorref == RGB(0, 128, 0))
			{
				strOneLine4Output += GetMSElapsed(strLastTimestamp, strCurrentTimestamp, false);
				strOneLine4Output += _T('\n');
			}
			
			if (colorref == RGB(0, 0, 0)) // This is the headline
			{
				if (!bSuppressFullAnalysis)
				{
					AppendToLogAndScroll(crec, strOneLine4Output, colorref); // Output it directly
				}
			}
			else // Communication log line
			{
				AppendToInterimBuffer(strOneLine4Output, colorref);
			}
			
			if (active_speaker != ACTIVE_SPEAKER::UNDEFINED) // Not a header line
			{
				CString strAnalysis = _T("");
				
				// This is a very shallow kind of 'analysis' (needed for count of control characters)
				// strAnalysis = Analysis(strOneLine4Analysis); // Analysis() is the single caller for analysis calls to AnalizeToken()
				// AppendToLogAndScroll(crec, strAnalysis, RGB(0, 0, 128));
				// AppendToLogAndScroll(crec, _T("\n\n"), RGB(0, 0, 0));

				CString strVirtualPredecessorLine = _T(""); // Start of line just read - 16 ... Start of line just read -1
				CString strVirtualThisLine = _T(""); // Start of line just read ... Start of line just read + 15
				CString strVirtualSuccessorLine = _T(""); // Start of line just read + 16 ... Start of line just read + 32

				unsigned int netLinecount = linecount - 1; // Ignore header line
				
				if (!(netLinecount % 16)) // Ready to harvest next virtual line
				{
					if (netLinecount >= 32) // There is a predecessor line
					{
						strVirtualPredecessorLine = GetVirtualPredecessorLine(pLogWalker);
					}
					
					strVirtualThisLine = GetVirtualThisLine(pLogWalker); // This is the line under observation
					strVirtualSuccessorLine = GetVirtualSuccessorLine(pLogWalker);

					// This fuction also fills CList<int, int> m_handshakeLocations, it's sorted in ascending order
					strAnalysis = SemanticAnalysis(strVirtualPredecessorLine, 
						strVirtualThisLine, 
						strVirtualSuccessorLine, 
						pLog, 
						pLogWalker, 
						strCurrentTimestamp);

					if (m_handshakeLocations.IsEmpty()) // No handshakes
					{ 
						POSITION pos;
						
						pos = m_InterimBuffer.GetHeadPosition();

						while (pos)
						{
							InterimBufferItem ibi = m_InterimBuffer.GetNext(pos);
							
							if (!bSuppressFullAnalysis)
							{
								if (-1 != ibi.strLine.Find(_T("0x002"))) // Line contains STX
								{
									/*if (m_AnalyzedBlocks.IsEmpty())
									{
										::MessageBox(NULL, _T("m_AnalyzedBlocks is empty"), _T("Internal error"), MB_OK | MB_ICONERROR);
									}*/

									if (m_AnalyzedBlocks.IsEmpty())
									{
										int setYourBreakPointHere = 0;
									}
									
									if (!m_AnalyzedBlocks.IsEmpty())
									{
										CString strLatestBlockAnalysis = m_AnalyzedBlocks.GetHead();
										AppendToLogAndScroll(crec, strLatestBlockAnalysis, RGB(0, 0, 0));
									}
								}
								
								AppendToLogAndScroll(crec, ibi.strLine, ibi.colorref);
							}
						}

						m_InterimBuffer.RemoveAll();
					}
					else
					{
						int start = 0;
						POSITION pos;
						POSITION pos2;
						
						pos = m_handshakeLocations.GetHeadPosition();
						pos2 = m_Handshakes.GetHeadPosition();

						while (pos != NULL)
						{
							int end = m_handshakeLocations.GetNext(pos);

							for (int i = start; i < end; i++)
							{
								POSITION pos2 = m_InterimBuffer.FindIndex(i);

								if (pos2 == NULL)
								{
									CString strErrMsg = _T("");
									strErrMsg.Format(_T("ERROR: Invalid handshake location %d"), i);
									//::MessageBox(NULL, strErrMsg, _T("Internal error"), MB_OK | MB_ICONERROR);
									AppendToLogAndScroll(crec, strErrMsg, RGB(0, 0, 0));
									m_nErrorsFound++;
								}
								else
								{
									InterimBufferItem ibi = m_InterimBuffer.GetAt(pos2);
									
									if (!bSuppressFullAnalysis)
									{
										if (-1 != ibi.strLine.Find(_T("0x002"))) // Line contains STX
										{
											/*if (m_AnalyzedBlocks.IsEmpty())
											{
												::MessageBox(NULL, _T("m_AnalyzedBlocks is empty"), _T("Internal error"), MB_OK | MB_ICONERROR);
											}*/

											if (m_AnalyzedBlocks.IsEmpty())
											{
												int setYourBreakPointHere = 0;
											}
				
											if (!m_AnalyzedBlocks.IsEmpty())
											{
												CString strLatestBlockAnalysis = m_AnalyzedBlocks.GetHead();
												AppendToLogAndScroll(crec, strLatestBlockAnalysis, RGB(0, 0, 0));
											}
										}
			
										AppendToLogAndScroll(crec, ibi.strLine, ibi.colorref);
									}
								}
							}

							start = end;

							HandshakeItem hsi = m_Handshakes.GetNext(pos2);
							AppendToLogAndScroll(crec, hsi.strHandshake + _T('\n'), hsi.colorref);
						}

						// Output rest of interim buffer
						for (int i = start; i < 16; i++)
						{
							POSITION pos3 = m_InterimBuffer.FindIndex(i);

							if (pos3 == NULL)
							{
								CString strErrMsg = _T("");
								strErrMsg.Format(_T("Invalid rest of interim buffer location %d"), i);
								//::MessageBox(NULL, strErrMsg, _T("Internal error"), MB_OK | MB_ICONERROR);
								AppendToLogAndScroll(crec, strErrMsg, RGB(0, 0, 0));
							}
							else
							{
								InterimBufferItem ibi = m_InterimBuffer.GetAt(pos3);
								
								if (!bSuppressFullAnalysis)
								{
									if (-1 != ibi.strLine.Find(_T("0x002"))) // Line contains STX
									{
										if (m_AnalyzedBlocks.IsEmpty())
										{
											::MessageBox(NULL, _T("m_AnalyzedBlocks is empty"), _T("Internal error"), MB_OK | MB_ICONERROR);
										}
				
										CString strLatestBlockAnalysis = m_AnalyzedBlocks.GetHead();
										AppendToLogAndScroll(crec, strLatestBlockAnalysis, RGB(0, 0, 0));
									}
			
									AppendToLogAndScroll(crec, ibi.strLine, ibi.colorref);
								}
							}
						}

						m_InterimBuffer.RemoveAll();
					}
				}
			}
		}

DONE:
		AppendToLogAndScroll(crec, _T("\n\nStatistics:\n"), RGB(0, 0, 0));

		CString strErrCount = _T("");
		strErrCount.Format(_T("%d errors found in %u bytes MACBus log (Use Edit > Find... ERROR: to see details)\n\n"), m_nErrorsFound, linecount);
		AppendToLogAndScroll(crec, strErrCount, RGB(0, 0, 0));
		
		// Alert user of parse completion
		if (!bSuppressFullAnalysis)
		{
			CString strDelayExtrema = GetMSElapsed(_T(""), _T(""), true); // Fetch report
			AppendToLogAndScroll(crec, strDelayExtrema, RGB(0, 0, 0));
		}

		TransactionStatistics(crec);
		BlockExchangeStatistics(crec);
		TransactionSequence(crec);

		crec.FlashWindow(TRUE); // Does not work
		Sleep(500);
		crec.FlashWindow(FALSE);

		// Not really useful, remainded from early stages of development
		// CString strReport = AnalizeToken(_T("Dummy"), false, true); // Called for report
		// ::MessageBox(NULL, strReport, _T("Information"), MB_OK | MB_ICONINFORMATION);
	}
}

int CMACBusVersteherView::AppendToLogAndScroll(CRichEditCtrl& ctrlLog, CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = ctrlLog.GetWindowTextLength();
	ctrlLog.SetSel(nInsertionPoint, -1);

	// Set the character format
	ctrlLog.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	ctrlLog.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&ctrlLog);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&ctrlLog != ctrlLog.GetFocus())
	{
		ctrlLog.LineScroll(INT_MAX);
		ctrlLog.LineScroll(1 - nVisible);
	}

	return 0;
}

int CMACBusVersteherView::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}

CString CMACBusVersteherView::Analysis(CString strOneLine)
{
	CString strResult = _T("");
	CString strToken = _T("");
	int curPos = 0;
	int nTokensSeen = 0;

	strToken = strOneLine.Tokenize(_T(" "), curPos);
	nTokensSeen++;
	
	while (strToken != _T(""))
	{
		CString strAnalizedToken = AnalizeToken(strToken, false, false); // Called for analysis
		strResult += strAnalizedToken;
		strResult += _T(" ");
		
		strToken = strOneLine.Tokenize(_T(" \r"), curPos);
		nTokensSeen++;
	}

	return strResult;
}

CString CMACBusVersteherView::SemanticAnalysis(CString strPredecessorLine, 
	CString strLine, 
	CString strSuccessorLine, 
	const char* pLog, 
	const char* pLogWalker,
	CString strCurrentTimestamp)
{
	CString strResult = _T("");
	CString strToken = _T("");
	int curPos = 0;
	int iTokenPos = 0;

	m_handshakeLocations.RemoveAll();
	ASSERT(m_handshakeLocations.IsEmpty()); // Paranoid :-)

	m_Handshakes.RemoveAll();
	ASSERT(m_Handshakes.IsEmpty()); // Paranoid :-)

	m_AnalyzedBlocks.RemoveAll();
	ASSERT(m_AnalyzedBlocks.IsEmpty()); // Paranoid :-)

	// Just three lines are sufficient for sorts of shallow analysis, init to empty string tokens
	
	for (int i = 0; i < 16; i++)
	{
		m_aStrPredecessorTokens[i].strTimestamp = _T("");
		m_aStrPredecessorTokens[i].strValue = _T("");
		
		m_aStrTokens[i].strTimestamp = _T("");
		m_aStrTokens[i].strValue = _T("");
		
		m_aStrSuccessorTokens[i].strTimestamp = _T("");
		m_aStrSuccessorTokens[i].strValue = _T("");
	}

	// Tokenize the three lines

	strToken = strPredecessorLine.Tokenize(_T(" "), curPos);
	
	while (strToken != _T(""))
	{
		m_aStrPredecessorTokens[iTokenPos].strTimestamp = GetTimestampFromItem(strToken);
		m_aStrPredecessorTokens[iTokenPos].strValue = GetValueFromItem(strToken);

		iTokenPos++;

		if (iTokenPos >= 17)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strPredecessorLine);
			::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
			strResult = strErrMsg;
				
			return strResult;
		}

		strToken = strPredecessorLine.Tokenize(_T(" \r\n"), curPos);
	}

	curPos = 0;
	iTokenPos = 0;
		
	strToken = strLine.Tokenize(_T(" "), curPos);
	
	while (strToken != _T(""))
	{
		m_aStrTokens[iTokenPos].strTimestamp = GetTimestampFromItem(strToken);
		m_aStrTokens[iTokenPos].strValue = GetValueFromItem(strToken);

		iTokenPos++;

		if (iTokenPos >= 17)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strLine);
			::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
			strResult = strErrMsg;
				
			return strResult;
		}

		strToken = strLine.Tokenize(_T(" \r\n"), curPos);
	}

	curPos = 0;
	iTokenPos = 0;

	strToken = strSuccessorLine.Tokenize(_T(" "), curPos);
	
	if (strToken != _T("[ENDOFLOGFILE]"))
	{
		while (strToken != _T(""))
		{
			m_aStrSuccessorTokens[iTokenPos].strTimestamp = GetTimestampFromItem(strToken);
			m_aStrSuccessorTokens[iTokenPos].strValue = GetValueFromItem(strToken);

			iTokenPos++;

			if (iTokenPos >= 17)
			{
				CString strErrMsg = _T("");
				strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strSuccessorLine);
				::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
				strResult = strErrMsg;

				return strResult;
			}

			strToken = strSuccessorLine.Tokenize(_T(" \r\n"), curPos);
		}
	}

	// Tokenized three lines of log for shallow analysis 

	CString strParsed = _T("");
	int logWalkerBacktracking = 0;
	HandshakeItem hsi;
	CString strAnalysis = _T("");

	for (int i = 0; i < 16; i++)
	{
		if (!m_aStrTokens[i].strTimestamp.IsEmpty())
		{
			CONTROLCHARACTERS ctrlc = IdentifyToken(m_aStrTokens[i].strValue);

			switch (ctrlc)
			{
				case CONTROLCHARACTERS::NUL: // ASCII 00 Null
				case CONTROLCHARACTERS::SOH: // ASCII 01 Start_of_Header
				case CONTROLCHARACTERS::ACK: // ASCII 06 Acknowledgement
				case CONTROLCHARACTERS::BEL: // ASCII 07 Bell
				case CONTROLCHARACTERS::BS:  // ASCII 08 Backspace
				case CONTROLCHARACTERS::TAB: // ASCII 09 Horizontal Tab
				case CONTROLCHARACTERS::LF:  // ASCII 10 Line_Feed
				case CONTROLCHARACTERS::VT:  // ASCII 11 Vertical tab
				case CONTROLCHARACTERS::FF:  // ASCII 12 Form_Feed
				case CONTROLCHARACTERS::CR:  // ASCII 13 Carriage_Return
				case CONTROLCHARACTERS::SO:  // ASCII 14 Shift_Out
				case CONTROLCHARACTERS::SI:  // ASCII 15 Shift_In
				case CONTROLCHARACTERS::_7F: // ASCII 127 aka DEL
				case CONTROLCHARACTERS::NOTACONTROLCHARACTER:

				if (m_bLastCharWasETX)
				{
					int iValue = HexString2Int((char*)m_aStrTokens[i].strValue.GetString()); // Block Check Character (checksum)
					strParsed.Format(_T(" BccIs_%d"), iValue);
					m_bLastCharWasETX = false;
				}
				else if (m_bInsideText)
				{
					strParsed.Format(_T(" %s"), ProcessNumericToken(m_aStrTokens[i].strValue));
				}
				else
				{
					strParsed.Format(_T(" %s"), m_aStrTokens[i].strValue);
				}
				
				strResult += strParsed;
				break;

				case CONTROLCHARACTERS::STX: // ASCII 02 Start_of_Text
					if (!m_bLastCharWasETX)
					{
						// This is the start of a "text", which is a block (that needs deep analysis)
						logWalkerBacktracking = 16 - i; // Number of '\n' we have to track back
						strAnalysis = DecodeBlock(pLog, pLogWalker, logWalkerBacktracking, m_activeSlave);
						m_AnalyzedBlocks.AddTail(strAnalysis);
						strResult += strAnalysis;
						strResult += _T(" 02==STX");
						m_bInsideText = true;
					}
					else
					{
						m_bLastCharWasETX = false;
					}
					break;

				case CONTROLCHARACTERS::ETX: // ASCII 03 End_of_Text
					strResult += _T(" 03==ETX");
					m_bInsideText = false;
					m_bLastCharWasETX = true;
					break;

				case CONTROLCHARACTERS::EOT: // ASCII 04 End_of_transmission
					strResult += _T(" 04==EOT");
					break;

				case CONTROLCHARACTERS::ENQ: // ASCII 05 Enquiry
					if (!m_bLastCharWasETX)
					{
						strParsed = DecodeSlaveEnquiry(i, m_aStrTokens[i].strTimestamp);
						strResult += strParsed;
						m_handshakeLocations.AddTail(i);
						hsi.strHandshake = strParsed;
						hsi.colorref = GetSlaveColor(strParsed);
						m_Handshakes.AddTail(hsi);
					}
					else
					{
						m_bLastCharWasETX = false;
					}
					break;

				case CONTROLCHARACTERS::DLE: // ASCII 16 Data_link_escape
					if (Successor(i, false) == _T("30"))
					{
						// Anforderungsphase: Positive Rückmeldung von SADR/EADR
						strParsed = DecodePositiveRequestFeedback(i, m_aStrTokens[i].strTimestamp);
						m_activeSlave = WhoAnswered(strParsed);
						m_handshakeLocations.AddTail(i);
						hsi.strHandshake = strParsed;
						hsi.colorref = GetSlaveColor(strParsed);
						m_Handshakes.AddTail(hsi);
					}
					else if (Successor(i, false) == _T("31"))
					{
						// Übermittlungsphase: Positive Rückmeldung
						strParsed.Format(_T("10 %s means Positive_Transmission_Feedback (\"VERSTANDEN!\")"), Successor(i, false));
						m_handshakeLocations.AddTail(i);
						hsi.strHandshake = strParsed;
						hsi.colorref = RGB(0, 0, 0); // TODO: Determine the SADR/EADR color here
						m_Handshakes.AddTail(hsi);
					}

					strResult += strParsed;
					break;

				case CONTROLCHARACTERS::DC1: // ASCII 17 Device_Cotrol_1
					strResult += _T(" 11==DC1");
					break;

				case CONTROLCHARACTERS::DC2: // ASCII 18 Device_Cotrol_2
					strResult += _T(" 12==DC2");
					break;

				case CONTROLCHARACTERS::DC3: // ASCII 19 -- just in case, never seen in practice!
					strResult += _T(" 13==DC3");
					break;

				case CONTROLCHARACTERS::DC4: // ASCII 20 Device_Cotrol_4
					strResult += _T(" 14==DC4");
					break;

				case CONTROLCHARACTERS::NAK: // ASCII 21 Negative_Acknowledgement
					strResult += _T(" 15==NAK");
					break;

				case CONTROLCHARACTERS::SYN: // ASCII 22 Synchronous_Idle
					strResult += _T(" 16==SYN");
					break;

				case CONTROLCHARACTERS::ETB: // ASCII 23 End_of_Trans_Block
					strResult += _T(" 17==ETB");
					break;

				case CONTROLCHARACTERS::CAN: // ASCII 24 Cancel
					strResult += _T(" 18==CAN");
					break;

				case CONTROLCHARACTERS::EM:  // ASCII 25 End_of_Medium
					strResult += _T(" 19==EM");
					break;

				case CONTROLCHARACTERS::SUB: // ASCII 26 Substitute
					strResult += _T(" 1A==SUB");
					break;

				case CONTROLCHARACTERS::ESC: // ASCII 27 Escape
					strResult += _T(" 1B==ESC");
					break;

				case CONTROLCHARACTERS::FS:  // ASCII 28 File_Separator
					strResult += _T(" 1C==FS");
					break;

				case CONTROLCHARACTERS::GS:  // ASCII 29 Group_Separator
					strResult += _T(" 1D==GS");
					break;

				case CONTROLCHARACTERS::RS:  // ASCII 30 Record_Separator
					strResult += _T(" 1E==RS");
					break;

				case CONTROLCHARACTERS::US:  // ASCII 31 Unit_Separator
					strResult += _T(" 1F==US");
					break;

				// case CONTROLCHARACTERS::_7F:  // ASCII 127 -- Sometimes called DEL, Serial Sniffer writes 7F This was a checksum!
				// strResult += _T(" 7F");
				// break;

				default:
					strResult = _T("Error: default reached");
			}
		}
	}
	
	return strResult;
}

CString CMACBusVersteherView::Predecessor(int slot, bool toInt)
{
	CString strResult = _T("");
	
	if (slot == 0)
	{
		strResult = m_aStrPredecessorTokens[15].strValue;
	}
	else
	{
		strResult = m_aStrTokens[slot-1].strValue;
	}

	if (toInt)
	{
		int iValue = HexString2Int((char*)strResult.GetString());
		strResult.Format(_T("%d"), iValue);
	}

	return strResult;
}

CString CMACBusVersteherView::Successor(int slot, bool toInt)
{
	CString strResult = _T("");
	
	if (slot == 15)
	{
		strResult = m_aStrSuccessorTokens[0].strValue;

		if (strResult.IsEmpty())
		{
			strResult = _T("NOSUCCESSOR");
		}
	}
	else
	{
		strResult = m_aStrTokens[slot+1].strValue;
	}

	if (toInt)
	{
		int iValue = HexString2Int((char*)strResult.GetString());
		strResult.Format(_T("%d"), iValue);
	}

	return strResult;
}


CString CMACBusVersteherView::AnalizeToken(CString strToken, bool bInit, bool bGetCounts)
{
	const int NUMDECODERITEMS = 34; // Slots 0 to 32 are Serial Sniffer signatures, 33 is terminator
	
	struct DecoderItem
	{
		CString strSymbol;
		CString strClearname;
		DWORD dwCount;
	};

	static DecoderItem DecoderItems[NUMDECODERITEMS];

	if (bInit)
	{
		for (int i = 0; i < NUMDECODERITEMS; i++)
		{
			DecoderItems[i].strSymbol = _T("");
			DecoderItems[i].strClearname = _T("");
			DecoderItems[i].dwCount = 0;
		}

		DecoderItems[0].strSymbol = _T("NUL"); // ASCII 00
		DecoderItems[0].strClearname = _T("Null_Character");

		DecoderItems[1].strSymbol = _T("SOH"); // ASCII 01
		DecoderItems[1].strClearname = _T("Start_of_Header");

		DecoderItems[2].strSymbol = _T("STX"); // ASCII 02
		DecoderItems[2].strClearname = _T("Start_of_Text");

		DecoderItems[3].strSymbol = _T("ETX"); // ASCII 03
		DecoderItems[3].strClearname = _T("End_of_Text");

		DecoderItems[4].strSymbol = _T("EOT"); // ASCII 04
		DecoderItems[4].strClearname = _T("End_of_transmission");

		DecoderItems[5].strSymbol = _T("ENQ"); // ASCII 05
		DecoderItems[5].strClearname = _T("Enquiry");

		DecoderItems[6].strSymbol = _T("ACK"); // ASCII 06
		DecoderItems[6].strClearname = _T("Acknowledgement");

		DecoderItems[7].strSymbol = _T("BEL"); // ASCII 07
		DecoderItems[7].strClearname = _T("Bell");

		DecoderItems[8].strSymbol = _T("BS"); // ASCII 08
		DecoderItems[8].strClearname = _T("Backspace");

		DecoderItems[9].strSymbol = _T("TAB"); // ASCII 09 -- Sometimes called HT, Serial Sniffer writes TAB
		DecoderItems[9].strClearname = _T("Horizontal_Tab");

		DecoderItems[10].strSymbol = _T("LF"); // ASCII 10
		DecoderItems[10].strClearname = _T("Line_Feed");

		DecoderItems[11].strSymbol = _T("VT"); // ASCII 11
		DecoderItems[11].strClearname = _T("Vertical_Tab");

		DecoderItems[12].strSymbol = _T("FF"); // ASCII 12
		DecoderItems[12].strClearname = _T("Form_Feed");

		DecoderItems[13].strSymbol = _T("CR"); // ASCII 13
		DecoderItems[13].strClearname = _T("Carriage_Return");

		DecoderItems[14].strSymbol = _T("SO"); // ASCII 14
		DecoderItems[14].strClearname = _T("Shift_Out");

		DecoderItems[15].strSymbol = _T("SI"); // ASCII 15
		DecoderItems[15].strClearname = _T("Shift_In");

		DecoderItems[16].strSymbol = _T("DLE"); // ASCII 16
		DecoderItems[16].strClearname = _T("Data_link_escape");

		DecoderItems[17].strSymbol = _T("DC1"); // ASCII 17
		DecoderItems[17].strClearname = _T("Device_Cotrol_1");
		
		DecoderItems[18].strSymbol = _T("DC2"); // ASCII 18
		DecoderItems[18].strClearname = _T("Device_Cotrol_2");

		DecoderItems[19].strSymbol = _T("DC3"); // ASCII 19 -- just in case, never seen in practice!
		DecoderItems[19].strClearname = _T("Device_Cotrol_3");

		DecoderItems[20].strSymbol = _T("DC4"); // ASCII 20
		DecoderItems[20].strClearname = _T("Device_Cotrol_4");

		DecoderItems[21].strSymbol = _T("NAK"); // ASCII 21
		DecoderItems[21].strClearname = _T("Negative_Acknowledgement");

		DecoderItems[22].strSymbol = _T("SYN"); // ASCII 22
		DecoderItems[22].strClearname = _T("Synchronous_Idle");

		DecoderItems[23].strSymbol = _T("ETB"); // ASCII 23
		DecoderItems[23].strClearname = _T("End_of_Trans_Block");

		DecoderItems[24].strSymbol = _T("CAN"); // ASCII 24
		DecoderItems[24].strClearname = _T("Cancel");

		DecoderItems[25].strSymbol = _T("EM"); // ASCII 25
		DecoderItems[25].strClearname = _T("End_of_Medium");

		DecoderItems[26].strSymbol = _T("SUB"); // ASCII 26
		DecoderItems[26].strClearname = _T("Substitute");

		DecoderItems[27].strSymbol = _T("ESC"); // ASCII 27
		DecoderItems[27].strClearname = _T("Escape");

		DecoderItems[28].strSymbol = _T("FS"); // ASCII 28
		DecoderItems[28].strClearname = _T("File_Separator");

		DecoderItems[29].strSymbol = _T("GS"); // ASCII 29
		DecoderItems[29].strClearname = _T("Group_Separator");

		DecoderItems[30].strSymbol = _T("RS"); // ASCII 30
		DecoderItems[30].strClearname = _T("Record_Separator");

		DecoderItems[31].strSymbol = _T("US"); // ASCII 31
		DecoderItems[31].strClearname = _T("Unit_Separator");

		DecoderItems[32].strSymbol = _T("7F"); // ASCII 127 -- Sometimes called DEL, Serial Sniffer writes 7F
		DecoderItems[32].strClearname = _T("Delete");

		return _T("Initialized command table");
	}

	if (bGetCounts) // This is not an analysis call, but a report call
	{
		CString strReport = _T("Number of control characters\r\n");

		for (int i = 0; i < NUMDECODERITEMS; i++)
		{
			CString strPartialReport = _T("");
			
			if (DecoderItems[i].strSymbol.IsEmpty())
			{
				break; // First unoccupied slot in table reached
			}
			
			strPartialReport.Format(_T("%05lu * %s (%s)\r\n"),
				DecoderItems[i].dwCount,
				DecoderItems[i].strSymbol,
				DecoderItems[i].strClearname);

			strReport += strPartialReport;
		}

		return strReport;
	}
	
	CString strAnalyzedToken = _T("");

	// Is it a command?
	bool bTokenIsCmd = false;

	for (int i = 0; i < NUMDECODERITEMS; i++)
	{
		if (DecoderItems[i].strSymbol.IsEmpty())
		{
			break;
		}
		
		if (strToken == DecoderItems[i].strSymbol)
		{
			strAnalyzedToken = DecoderItems[i].strClearname;
			DecoderItems[i].dwCount++;
			bTokenIsCmd = true;
			break;
		}
	}

	if (!bTokenIsCmd && strToken != _T("\n"))
	{
		strAnalyzedToken = ProcessNumericToken(strToken);
	}
	
	return strAnalyzedToken;
}

CString CMACBusVersteherView::ProcessNumericToken(CString strToken)
{
	CString strProcessed = _T("");
	
	int len = strToken.GetLength();

	for (int i = 0; i < len; i += 2)
	{
		CString strByte = strToken.Mid(i, 2);
		char chr = (char)(int)strtol(strByte.GetString(), NULL, 16);
		strProcessed.Append(CString(chr));
	}
	
	return strProcessed;
}

int CMACBusVersteherView::GetNumberOfLeadingColumns(char* pLog)
{
	char oneLine[256];

	// MSB_RS485_PLUS export file has a single headerline
	
	ZeroMemory(oneLine, 256);
	int pos = 0;

	while (*pLog != _T('\n'))
	{
		if (*pLog == _T('\0'))
		{
			::MessageBox(NULL, _T("End of file reached"), _T("Error"), MB_OK | MB_ICONERROR);
		}
		
		oneLine[pos] = *pLog;
		pos++;
		pLog++;
	}

	oneLine[pos] = *pLog;
	pos++;
	pLog++;
	
	// pLog now points to the first data line
	pos = 0;

	// Get the line
	
	ZeroMemory(oneLine, 256);

	while (*pLog != _T('\n'))
	{
		if (*pLog == _T('\0'))
		{
			::MessageBox(NULL, _T("End of file reached"), _T("Error"), MB_OK | MB_ICONERROR);
		}
		
		oneLine[pos] = *pLog;
		pos++;
		pLog++;
	}

	// Count its tokens

	CString strOneLine(oneLine);
	CString strToken = _T("");
	int curPos = 0;
	int nLeadingColsSeen = 0;

	strToken = strOneLine.Tokenize(_T(",\n"), curPos);
	nLeadingColsSeen++;

	bool bDataColFound = false;

	while (strToken != _T(""))
	{
		strToken = strOneLine.Tokenize(_T(",\n"), curPos);
		nLeadingColsSeen++;

		if (-1 != strToken.Find(_T("0x")))
		{
			bDataColFound = true;
			nLeadingColsSeen--;
			break;
		}
	}

	if (!bDataColFound)
	{
		::MessageBox(NULL, _T("Data column not found"), _T("GetNumberOfLeadingColumns") _T("Error"), MB_OK | MB_ICONERROR); 
	}

	return nLeadingColsSeen;
}

// These data lines do not exist in Data Monitor export, but we have set the 
// Data Monitor window to show 16 values in a row. So it still makes sense
// to think of these lines containing 16 values each.
CString CMACBusVersteherView::GetVirtualPredecessorLine(char* pLogWalker)
{
	char* pBackup = pLogWalker;

	// Backtrack pLogWalker to start position (this is the previous "virtual line")
	int nLineBreaks = 0;

	while (true)
	{
		if (*pLogWalker == _T('\n'))
		{
			nLineBreaks++;
		}

		if (nLineBreaks == (2*16) + 1)
		{
			break;
		}

		pLogWalker--;
	}

	pLogWalker++; // Now points to the begin of the virtual predecessor line

	char readBuffer[2048]; // Must be big enough to contain at least 16 exported lines
	ZeroMemory(readBuffer, 2048);
	int pos = 0;

	// Get a chunk of data
	while (*pLogWalker != _T('\0'))
	{
		readBuffer[pos] = *pLogWalker;
		pos++;
		pLogWalker++;

		if (pos >= 2048)
		{
			break;
		}
	}

	readBuffer[2047] = _T('\0'); // terminate the data chunk
	CString strChunk(readBuffer); // More convenient to work with
	CString strResult = _T("");
	int lineStart = 0;
	int lineEnd = 0;

	for (int i = 0; i < 16; i++) // Get the next 16 timestamp/value pairs
	{
		CString strValue = _T("");
		
		// Get one line
		lineEnd = strChunk.Find(_T("\n"), lineStart); // NL looks like \r\n

		CString strLine = strChunk.Mid(lineStart, lineEnd - lineStart);

		lineStart = lineEnd + 1;

		// Get timestamp from line
		int posTimestampEnd = strLine.Find(_T("s"));
		CString strTimestamp = strLine.Left(posTimestampEnd);

		int posValue = 0;

		for (int j = 0; j < 3; j++)
		{
			posValue = strLine.Find(_T(","), posValue);
			posValue++;
		}

		posValue += 2;
		
		CString strValueFat = strLine.Mid(posValue, 3); // Get in 0xXXX format (speciality of MSB_RS485_PLUS)
		
		if (strValueFat[0] != _T('0'))
		{
			CString strMsg = _T("");
			strMsg.Format(_T("Found unexpected datum %s"), strValueFat.GetString());
			::MessageBox(NULL, strMsg, _T("Error"), MB_OK | MB_ICONERROR);
		}

		CString strValueSlim = strValueFat.Mid(1, 2);
		CString strItem = strTimestamp + _T('|') + strValueSlim;

		strResult += strItem;

		if (i != 15)
		{
			strResult += _T(' ');
		}
	}

	pLogWalker = pBackup;

	return strResult;
}

// These data lines do not exist in Data Monitor export, but we have set the 
// Data Monitor window to show 16 values in a row. So it still makes sense
// to think of these lines containing 16 values each.
CString CMACBusVersteherView::GetVirtualThisLine(char* pLogWalker)
{
	char* pBackup = pLogWalker;

	// Backtrack pLogWalker to start position of current virtual line
	int nLineBreaks = 0;

	while (true)
	{
		if (*pLogWalker == _T('\n'))
		{
			nLineBreaks++;
		}

		if (nLineBreaks == (1 * 16) + 1)
		{
			break;
		}

		pLogWalker--;
	}

	pLogWalker++; // Now points to the begin of the virtual this line

	char readBuffer[2048]; // Must be big enough to contain at least 16 exported lines
	ZeroMemory(readBuffer, 2048);
	int pos = 0;

	// Get a chunk of data
	while (*pLogWalker != _T('\0'))
	{
		readBuffer[pos] = *pLogWalker;
		pos++;
		pLogWalker++;

		if (pos >= 2048)
		{
			break;
		}
	}

	readBuffer[2047] = _T('\0'); // terminate the data chunk

	CString strChunk(readBuffer); // More convenient to work with
	CString strResult = _T("");
	int lineStart = 0;
	int lineEnd = 0;

	for (int i = 0; i < 16; i++) // Get the next 16 timestamp/value pairs
	{
		CString strValue = _T("");
		
		// Get one line
		lineEnd = strChunk.Find(_T("\n"), lineStart); // NL looks like \r\n

		CString strLine = strChunk.Mid(lineStart, lineEnd - lineStart);

		lineStart = lineEnd + 1;

		// Get timestamp from line
		int posTimestampEnd = strLine.Find(_T("s"));
		CString strTimestamp = strLine.Left(posTimestampEnd);

		int posValue = 0;

		for (int j = 0; j < 3; j++)
		{
			posValue = strLine.Find(_T(","), posValue);
			posValue++;
		}

		posValue += 2;
		
		CString strValueFat = strLine.Mid(posValue, 3); // Get in 0xXXX format (speciality of MSB_RS485_PLUS)
		
		if (strValueFat[0] != _T('0'))
		{
			CString strMsg = _T("");
			strMsg.Format(_T("Found unexpected datum %s"), strValueFat.GetString());
			::MessageBox(NULL, strMsg, _T("Error"), MB_OK | MB_ICONERROR);
		}

		CString strValueSlim = strValueFat.Mid(1, 2);
		CString strItem = strTimestamp + _T('|') + strValueSlim;

		strResult += strItem;

		if (i != 15)
		{
			strResult += _T(' ');
		}
	}

	pLogWalker = pBackup;

	return strResult;
}

// These data lines do not exist in Data Monitor export, but we have set the 
// Data Monitor window to show 16 values in a row. So it still makes sense
// to think of these lines containing 16 values each.
CString CMACBusVersteherView::GetVirtualSuccessorLine(char* pLogWalker)
{
	char* pBackup = pLogWalker;

	CString strResult = _T("");

	char readBuffer[2048]; // Must be big enough to contain at least 16 exported lines
	ZeroMemory(readBuffer, 2048);
	int pos = 0;

	// Get a chunk of data
	while (*pLogWalker != _T('\0'))
	{
		readBuffer[pos] = *pLogWalker;
		pos++;
		pLogWalker++;

		if (pos >= 2048)
		{
			break;
		}
	}

	readBuffer[2047] = _T('\0'); // terminate the data chunk
	CString strChunk(readBuffer); // More convenient to work with
	int lineStart = 0;
	int lineEnd = 0;

	for (int i = 0; i < 16; i++) // Get the next 16 timestamp/value pairs
	{
		CString strValue = _T("");
		
		// Get one line
		lineEnd = strChunk.Find(_T("\n"), lineStart); // NL looks like \r\n

		CString strLine = strChunk.Mid(lineStart, lineEnd - lineStart);

		if (strLine.IsEmpty())
		{
			break;
		}

		lineStart = lineEnd + 1;

		// Get timestamp from line
		int posTimestampEnd = strLine.Find(_T("s"));
		CString strTimestamp = strLine.Left(posTimestampEnd);

		int posValue = 0;

		for (int j = 0; j < 3; j++)
		{
			posValue = strLine.Find(_T(","), posValue);
			posValue++;
		}

		posValue += 2;
		
		CString strValueFat = strLine.Mid(posValue, 3); // Get in 0xXXX format (speciality of MSB_RS485_PLUS)
		
		if (strValueFat[0] != _T('0'))
		{
			CString strMsg = _T("");
			strMsg.Format(_T("Found unexpected datum %s"), strValueFat.GetString());
			::MessageBox(NULL, strMsg, _T("Error"), MB_OK | MB_ICONERROR);
		}

		CString strValueSlim = strValueFat.Mid(1, 2);
		CString strItem = strTimestamp + _T('|') + strValueSlim;

		strResult += strItem;

		if (i != 15)
		{
			strResult += _T(' ');
		}
	}
	
	pLogWalker = pBackup;
	
	return strResult;
}

CMACBusVersteherView::CONTROLCHARACTERS CMACBusVersteherView::IdentifyToken(CString strToken)
{
	static bool bIAmInitialized = false;
	CONTROLCHARACTERS cc = CONTROLCHARACTERS::NOTACONTROLCHARACTER;
	const int NUMDECODERITEMS = 34; // Slots 0 to 32 are Serial Sniffer signatures, 33 is terminator
	
	struct IdentifyDecoderItem
	{
		CString strASCII;
		CONTROLCHARACTERS controlCharacter;
	};
	
	static IdentifyDecoderItem identifyDecoderItems[NUMDECODERITEMS];

	if (!bIAmInitialized)
	{
		for (int i = 0; i < NUMDECODERITEMS; i++)
		{
			identifyDecoderItems[i].strASCII = _T("");
			identifyDecoderItems[i].controlCharacter = CONTROLCHARACTERS::NOTACONTROLCHARACTER;
		}

		identifyDecoderItems[0].strASCII = _T("00"); // ASCII 00
		identifyDecoderItems[0].controlCharacter = CONTROLCHARACTERS::NUL;

		identifyDecoderItems[1].strASCII = _T("01"); // ASCII 01
		identifyDecoderItems[1].controlCharacter = CONTROLCHARACTERS::SOH;

		identifyDecoderItems[2].strASCII = _T("02"); // ASCII 02
		identifyDecoderItems[2].controlCharacter = CONTROLCHARACTERS::STX;

		identifyDecoderItems[3].strASCII = _T("03"); // ASCII 03
		identifyDecoderItems[3].controlCharacter = CONTROLCHARACTERS::ETX;

		identifyDecoderItems[4].strASCII = _T("04"); // ASCII 04
		identifyDecoderItems[4].controlCharacter = CONTROLCHARACTERS::EOT;

		identifyDecoderItems[5].strASCII = _T("05"); // ASCII 05
		identifyDecoderItems[5].controlCharacter = CONTROLCHARACTERS::ENQ;

		identifyDecoderItems[6].strASCII = _T("06"); // ASCII 06
		identifyDecoderItems[6].controlCharacter = CONTROLCHARACTERS::ACK;

		identifyDecoderItems[7].strASCII = _T("07"); // ASCII 07
		identifyDecoderItems[7].controlCharacter = CONTROLCHARACTERS::BEL;

		identifyDecoderItems[8].strASCII = _T("08"); // ASCII 08
		identifyDecoderItems[8].controlCharacter = CONTROLCHARACTERS::BS;

		identifyDecoderItems[9].strASCII = _T("09"); // ASCII 09
		identifyDecoderItems[9].controlCharacter = CONTROLCHARACTERS::TAB;

		identifyDecoderItems[10].strASCII = _T("0A"); // ASCII 10
		identifyDecoderItems[10].controlCharacter = CONTROLCHARACTERS::LF;

		identifyDecoderItems[11].strASCII = _T("0B"); // ASCII 11
		identifyDecoderItems[11].controlCharacter = CONTROLCHARACTERS::VT;

		identifyDecoderItems[12].strASCII = _T("0C"); // ASCII 12
		identifyDecoderItems[12].controlCharacter = CONTROLCHARACTERS::FF;

		identifyDecoderItems[13].strASCII = _T("0D"); // ASCII 13
		identifyDecoderItems[13].controlCharacter = CONTROLCHARACTERS::CR;

		identifyDecoderItems[14].strASCII = _T("0E"); // ASCII 14
		identifyDecoderItems[14].controlCharacter = CONTROLCHARACTERS::SO;

		identifyDecoderItems[15].strASCII = _T("0F"); // ASCII 15
		identifyDecoderItems[15].controlCharacter = CONTROLCHARACTERS::SI;

		identifyDecoderItems[16].strASCII = _T("10"); // ASCII 16
		identifyDecoderItems[16].controlCharacter = CONTROLCHARACTERS::DLE;

		identifyDecoderItems[17].strASCII = _T("11"); // ASCII 17
		identifyDecoderItems[17].controlCharacter = CONTROLCHARACTERS::DC1;
		
		identifyDecoderItems[18].strASCII = _T("12"); // ASCII 18
		identifyDecoderItems[18].controlCharacter = CONTROLCHARACTERS::DC2;

		identifyDecoderItems[19].strASCII = _T("13"); // ASCII 19
		identifyDecoderItems[19].controlCharacter = CONTROLCHARACTERS::DC3;

		identifyDecoderItems[20].strASCII = _T("14"); // ASCII 20
		identifyDecoderItems[20].controlCharacter = CONTROLCHARACTERS::DC4;

		identifyDecoderItems[21].strASCII = _T("15"); // ASCII 21
		identifyDecoderItems[21].controlCharacter = CONTROLCHARACTERS::NAK;

		identifyDecoderItems[22].strASCII = _T("16"); // ASCII 22
		identifyDecoderItems[22].controlCharacter = CONTROLCHARACTERS::SYN;

		identifyDecoderItems[23].strASCII = _T("17"); // ASCII 23
		identifyDecoderItems[23].controlCharacter = CONTROLCHARACTERS::ETB;

		identifyDecoderItems[24].strASCII = _T("18"); // ASCII 24
		identifyDecoderItems[24].controlCharacter = CONTROLCHARACTERS::CAN;

		identifyDecoderItems[25].strASCII = _T("19"); // ASCII 25
		identifyDecoderItems[25].controlCharacter = CONTROLCHARACTERS::EM;

		identifyDecoderItems[26].strASCII = _T("1A"); // ASCII 26
		identifyDecoderItems[26].controlCharacter = CONTROLCHARACTERS::SUB;

		identifyDecoderItems[27].strASCII = _T("1B"); // ASCII 27
		identifyDecoderItems[27].controlCharacter = CONTROLCHARACTERS::ESC;

		identifyDecoderItems[28].strASCII = _T("1C"); // ASCII 28
		identifyDecoderItems[28].controlCharacter = CONTROLCHARACTERS::FS;

		identifyDecoderItems[29].strASCII = _T("1D"); // ASCII 29
		identifyDecoderItems[29].controlCharacter = CONTROLCHARACTERS::GS;

		identifyDecoderItems[30].strASCII = _T("1E"); // ASCII 30
		identifyDecoderItems[30].controlCharacter = CONTROLCHARACTERS::RS;

		identifyDecoderItems[31].strASCII = _T("1F"); // ASCII 31
		identifyDecoderItems[31].controlCharacter = CONTROLCHARACTERS::US;

		identifyDecoderItems[32].strASCII = _T("7F"); // ASCII 127 -- Sometimes called DEL, Serial Sniffer writes 7F
		identifyDecoderItems[32].controlCharacter = CONTROLCHARACTERS::_7F;

		bIAmInitialized = true;
	}
	
	for (int i = 0; i < NUMDECODERITEMS; i++)
	{
		if (identifyDecoderItems[i].strASCII.IsEmpty())
		{
			break;
		}
		
		if (strToken == identifyDecoderItems[i].strASCII)
		{
			cc = identifyDecoderItems[i].controlCharacter;
			break;
		}
	}

	return cc;
}

CString CMACBusVersteherView::DecodeSlaveEnquiry(int tokenSlot, CString strCurrentTimestamp)
{
	CString strResult = _T("");
	CString strHexSlaveID = Predecessor(tokenSlot, false);
	CString strIntSlaveID = Predecessor(tokenSlot, true);
	int iRawSlaveID = _ttoi(strIntSlaveID); // The format "on the wire" masked with 0100 0000
	short int iRemovalMask = 0x00000040; // binary 0000 0000 0100 0000
	int iCookedSlaveID = iRawSlaveID & ~iRemovalMask; // The format used inside the embedded software
	
	CString strSlaveClearname = _T("UNKNOWN");
	CString strRole = _T("UNKNOWN");
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (iCookedSlaveID == m_KnownSlaves[i].iUnmaskedNumber)
		{
			strSlaveClearname = m_KnownSlaves[i].strName;
			strRole = m_KnownSlaves[i].strRole;
			
			// Eine Transaktion wird geöffnet, das merken wir uns
			if (m_KnownSlaves[i].nOpenTransactions == -1) // Magic number for initial state, do not accept a close before an open
			{
				m_KnownSlaves[i].nOpenTransactions = 1; // Count starts from initial state
				
				TransactionItem ti;
				ti.dTransactionStartTime = strtod(strCurrentTimestamp, NULL);
				ti.dTransactionEndTime = 0.;
				m_KnownSlaves[i].transactions.AddTail(ti);
			}
			else
			{
				m_KnownSlaves[i].nOpenTransactions++; // Normal count

				TransactionItem ti;
				ti.dTransactionStartTime = strtod(strCurrentTimestamp, NULL);
				ti.dTransactionEndTime = 0.;
				m_KnownSlaves[i].transactions.AddTail(ti);
			}

			m_KnownSlaves[i].dwTransactions++;

			break;
		}
	}

	CString strErrMsg = _T("");
	
	if (strSlaveClearname == _T("UNKNOWN")) // iCookedSlaveID is undefined
	{
		strErrMsg.Format(_T("Attempt to send enquiry to unknown slave address %d"), iCookedSlaveID);
		// ::MessageBox(NULL, strErrMsg, _T("Error"), MB_OK | MB_ICONERROR); WP2020819: Do not halt the workflow
	}

	CString strCleartext = _T("\"") + strSlaveClearname;

	if (strRole == _T("EADR"))
	{
		strCleartext += _T(", BITTE EMPFANGEN!\"");
	}
	else
	{
		strCleartext += _T(", BITTE SENDEN!\"");
	}

	strResult.Format(_T(">> %s 05 means EnquireSlave_%d_%s (%s)"), strHexSlaveID, iCookedSlaveID, strRole, strCleartext);

	if (!strErrMsg.IsEmpty())
	{
		strResult += _T('\n');
		strResult += strErrMsg;
	}

	return strResult;
}

CString CMACBusVersteherView::DecodePositiveRequestFeedback(int tokenSlot, CString strCurrentTimestamp)
{
	CString strResult = _T("");
	CString strHexSlaveID = Predecessor(tokenSlot, false);
	CString strIntSlaveID = Predecessor(tokenSlot, true);
	int iRawSlaveID = _ttoi(strIntSlaveID); // The format "on the wire" masked with 0100 0000
	short int iRemovalMask = 0x00000040; // binary 0000 0000 0100 0000
	int iCookedSlaveID = iRawSlaveID & ~iRemovalMask; // The format used inside the embedded software
	
	CString strSlaveClearname = _T("UNKNOWN");
	CString strRole = _T("UNKNOWN");
	CString strTransactionReport = _T("");
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (iCookedSlaveID == m_KnownSlaves[i].iUnmaskedNumber)
		{
			strSlaveClearname = m_KnownSlaves[i].strName;
			strRole = m_KnownSlaves[i].strRole;

			if (m_KnownSlaves[i].nOpenTransactions < 1)
			{
				CString strErrMsg = _T("");
				strErrMsg.Format(_T("Error at timestamp %s -- No open requests for %s in role %s"), 
					strCurrentTimestamp,
					strSlaveClearname, 
					strRole);
				::MessageBox(NULL, strErrMsg + _T(" ") + m_strErrorHint, _T("Error"), MB_OK | MB_ICONERROR);

				return _T(" ") + m_strSpecialMsgDecorator + strErrMsg;
			}
			else
			{
				TransactionItem ti = m_KnownSlaves[i].transactions.GetTail();
				m_KnownSlaves[i].transactions.RemoveTail();
				ti.dTransactionEndTime = strtod(strCurrentTimestamp, NULL);
				m_KnownSlaves[i].transactions.AddTail(ti);
				m_KnownSlaves[i].nOpenTransactions--;

				strTransactionReport.Format(_T("[Enquiry was at %.9lf Response is at %.9lf This is a delay of %.9lfsec %d unresponded enquiries]"),
					ti.dTransactionStartTime,
					ti.dTransactionEndTime,
					ti.dTransactionEndTime - ti.dTransactionStartTime,
					m_KnownSlaves[i].nOpenTransactions);
			}

			break;
		}
	}
	
	CString strCleartext = _T("\"HIER ") + strSlaveClearname;

	if (strRole == _T("EADR"))
	{
		strCleartext += _T(", ICH EMPFANGE!\"");
	}
	else
	{
		strCleartext += _T(", ICH SENDE!\"");
	}

	strResult.Format(_T("<< %s 10 %s means Positive_Request_Feedback_From_%s_%d (%s)"), strHexSlaveID, Successor(tokenSlot, false), strRole, iCookedSlaveID, strCleartext);
	strResult += _T(" ");
	strResult += strTransactionReport;

	return strResult;
}

CString CMACBusVersteherView::DecodeBlock(const char* pLog, const char* pLogWalker, const int logWalkerBacktracking, SLAVE activeSlave)
{
	CString strResult = _T("Analysis of the block that follows:\n02==STX\n");
	const char* pWork = pLogWalker;

	char readBuffer[32768]; // Must be big enough to contain block plus decorating metainformation
	ZeroMemory(readBuffer, 32768);
	int pos = 0;

	// Adjust pWork
	int nCRLFSeen = 0;

	while (pWork > pLog)
	{
		pWork--;

		if (*pWork == '\n')
		{
			nCRLFSeen++;
		}

		if (nCRLFSeen == logWalkerBacktracking)
		{
			break;
		}
	}

	pWork++; // From '\n' to start of target line
	
	// Get a chunk of data
	while (*pWork != _T('\0'))
	{
		readBuffer[pos] = *pWork;
		pos++;
		pWork++;

		if (pos >= 32767)
		{
			break;
		}
	}

	readBuffer[32767] = _T('\0'); // terminate the data chunk

	CString strReadBuffer(readBuffer);

	int timestampPos = 0;
	CString strTimestamp = strReadBuffer.Tokenize(_T("s"), timestampPos);
	double dTimestamp = strtod(strTimestamp, NULL);

	CString strPort = _T("");
	int portPos = 0;
	
	for (int i = 0; i < 3; i++)
	{
		strPort = strReadBuffer.Tokenize(_T(","), portPos);
	}
	
	SEND_DIRECTION send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;

	if (strPort == _T("B"))
	{
		send_direction = SEND_DIRECTION::MASTER_TO_SLAVE;
	}
	// else if (strPort == _T("A"))
	// {
	//	SEND_DIRECTION send_direction = SEND_DIRECTION::SLAVE_TO_MASTER;
	// }

	CString strBlockID_Low = _T("");
	CString strNumberOfEntries_Low = _T("");
	CString strDataSize_Low = _T("");
	CString strBlockID_High = _T("");
	CString strNumberOfEntries_High = _T("");
	CString strDataSize_High = _T("");

	int nValuesSeen = 0;
	bool bPreceededByNL = false;
	bool bMissingETX = false;

	CString str3Form = _T("");
	pos = 0;
	
	while (pos != -1)
	{
		for (int i = 0; i < 4; i++)
		{
			str3Form = strReadBuffer.Tokenize(_T(","), pos);

			if (str3Form.IsEmpty())
			{
				bMissingETX = true;
				goto DONE;
			}
		}

		str3Form.Delete(0, 2);

		CString str2Form = _T("");

		if (str3Form[0] != _T('0'))
		{
			::MessageBox(NULL, _T("str3Form[0] != 0"), _T("Error"), MB_OK | MB_ICONERROR);
		}
		else
		{
			nValuesSeen++;
			str2Form = _T("");
			str2Form += str3Form[1];
			str2Form += str3Form[2];

			switch (nValuesSeen)
			{
				case 1: strBlockID_High = str2Form;
					break;

				case 2: strBlockID_Low = str2Form;
					break;

				case 3: strNumberOfEntries_High = str2Form;
					break;

				case 4: strNumberOfEntries_Low = str2Form;
					break;

				case 5: strDataSize_High = str2Form;
					break;

				case 6: strDataSize_Low = str2Form;
					break;

				default: ; // Do nothing
			}
			
			if (str2Form == _T("03"))
			{
				if (bPreceededByNL)
				{
					str2Form = _T("03==ETX\n");
				}
				else
				{
					str2Form = _T("\n03==ETX\n");
				}
			}
			
			strResult += str2Form;

			if (-1 != str2Form.Find(_T("03==ETX\n")))
			{
				break; // We are at end of telegram
			}

			if (!(nValuesSeen % 16))
			{
				strResult += _T('\n');
				bPreceededByNL = true;
			}
			else
			{
				strResult += _T(' ');
				bPreceededByNL = false;
			}
		}

		pos = strReadBuffer.Find(_T("\n"), pos+1);
		
		if ((pos != -1) && ((pos + 1) != _T('\0')))
		{
			pos++; // No points to begin of next line
		}
		else
		{
			break;
		}
	}

DONE:	

	if (bMissingETX)
	{
		strResult += _T("ERROR: Data ends before ETX could be found\n");
	}
	else
	{
		CString strCount = _T("");
		strCount.Format(_T("Telegram consists of %d bytes between 02==STX and 03==ETX\n"), nValuesSeen-1);
		strResult += strCount;

		CString strErrMsg = _T("");
		CString strBlockHeaderPart = _T("");
	
		BYTE BlockID = DecodeStringPair(strBlockID_High, strBlockID_Low, strErrMsg);
		strBlockHeaderPart.Format(_T("Block ID: %d\n"), BlockID);
		strResult += strBlockHeaderPart;

		if (!strErrMsg.IsEmpty())
		{
			strResult += strErrMsg;
			strResult += _T('\n');
			strErrMsg = _T("");
		}

		BYTE nEntries = DecodeStringPair(strNumberOfEntries_High, strNumberOfEntries_Low, strErrMsg);
		strBlockHeaderPart.Format(_T("Number of entries: %d\n"), nEntries);
		strResult += strBlockHeaderPart;

		if (!strErrMsg.IsEmpty())
		{
			strResult += strErrMsg;
			strResult += _T('\n');
			strErrMsg = _T("");
		}

		BYTE dataSize = DecodeStringPair(strDataSize_High, strDataSize_Low, strErrMsg);
		strBlockHeaderPart.Format(_T("Data size: %d\n"), dataSize);
		strResult += strBlockHeaderPart;

		if (!strErrMsg.IsEmpty())
		{
			strResult += strErrMsg;
			strResult += _T('\n');
		}

		BlockTraceItem bti;
		bti.ID = BlockID;
		bti.send_direction = send_direction;
		bti.dTimeSent = dTimestamp;
		bti.activeSlave = activeSlave;
		m_BlockExchangeTrace.AddTail(bti);
	}

	return strResult;
}

int CMACBusVersteherView::CountBytesInTelegram(const char* pWork)
{
	int nBytesFound = 0;
	const char* pLookAhead = pWork;
	CString strSearchArea = _T("");

	while (*(pLookAhead+2) != _T('\0'))
	{
		if (*pLookAhead == _T('E') && *(pLookAhead + 1) == _T('T') && *(pLookAhead + 2) == _T('X'))
		{
			break;
		}
		else
		{
			strSearchArea.AppendChar(*pLookAhead);
		}

		pLookAhead++;
	}

	int curPos = 0;

	CString strToken = strSearchArea.Tokenize(_T(" "), curPos);

	while (strToken != _T(""))
	{
		if (strToken.GetLength() == 2)
		{
			nBytesFound++;
		}

		strToken = strSearchArea.Tokenize(_T(" \r"), curPos);
	}

	return nBytesFound;
}

bool CMACBusVersteherView::LineContainsHandshake(CString strReportLine, COLORREF& cr, char* pLogWalker)
{
	if (-1 != strReportLine.Find("VERSTANDEN!")) // This is a DLE 31 positive transmission response
	{
		// pLogWalker now points to the beginning of the virtual line following the line
		// that contains "VERSTANDEN!" (00 DLE 31 == 0x010 0x031)

		// To begin of virtual line (composed of 16 CSV lines) where DLE 31 was found
		char* pWork = pLogWalker;
		int nLinesBacktracked = 0;

		while (true)
		{
			pWork--;

			if (*pWork == _T('\n'))
			{
				nLinesBacktracked++;
			}

			if (nLinesBacktracked == 17)
			{
				break;
			}
		}

		pWork++;

		char readBuffer[2048]; // Must be big enough to contain at least 16 exported lines
		ZeroMemory(readBuffer, 2048);
		int pos = 0;

	// Get a chunk of data
		while (*pWork != _T('\0'))
		{
			readBuffer[pos] = *pWork;
			pos++;
			pWork++;

			if (pos >= 2048)
			{
				break;
			}
		}

		readBuffer[2047] = _T('\0'); // terminate the data chunk
		CString strChunk(readBuffer); // More convenient to work with
		bool bFound = false;

		int posDLE = strChunk.Find(_T("0x010")); // DLE

		if (posDLE != -1)
		{
			int nextLineStart = strChunk.Find(_T("\n"), posDLE);
			nextLineStart++;
			int nextLineEnd = strChunk.Find(_T("\n"), nextLineStart);
			CString strNextLine = strChunk.Mid(nextLineStart, nextLineEnd - nextLineStart);
			
			int pos31 = strNextLine.Find(_T("0x031")); // "VERSTANDEN!"

			if (pos31 != -1)
			{
				bFound = true;
			}
		}

		char strSpeaker = 0;

		if (!bFound)
		{
			::MessageBox(NULL, _T("LineContainsHandshake DLE 31 nicht gefunden"), _T("Fehler"), MB_OK | MB_ICONERROR);
		}
		else
		{
			strSpeaker = strChunk[posDLE - 2];
		}

		if (strSpeaker == _T('B'))
		{
			cr = RGB(0, 255, 0); // Master
		}
		else if (strSpeaker == _T('A'))
		{
			cr = RGB(255, 0, 0); // Slave
		}

		bFound = false;

		posDLE = strChunk.Find(_T("0x010"), posDLE + 1); // DLE

		if (posDLE != -1)
		{
			int nextLineStart = strChunk.Find(_T("\n"), posDLE);
			nextLineStart++;
			int nextLineEnd = strChunk.Find(_T("\n"), nextLineStart);
			CString strNextLine = strChunk.Mid(nextLineStart, nextLineEnd - nextLineStart);
			
			int pos31 = strNextLine.Find(_T("0x031"), posDLE); // "VERSTANDEN!"

			if (pos31 != -1)
			{
				bFound = true;
			}
		}

		if (bFound)
		{
			::MessageBox(NULL, _T("LineContainsHandshake mehrere DLE 31 im Puffer"), _T("Fehler"), MB_OK | MB_ICONERROR);
		}
		
		return true; 
	}
	
	bool bFound = false;
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (-1 != strReportLine.Find(m_KnownSlaves[i].strName))
		{
			bFound = true;
			cr = m_KnownSlaves[i].colorref;
			break;
		}
	}

	return bFound;
}

CString CMACBusVersteherView::GetTimestamp(CString strLogLine)
{
	int curPos = 0;
	// Always have Datenmonitor write the timestamps to the first column!
	CString strToken = strLogLine.Tokenize(_T(","), curPos);
	int loc = strToken.Find(_T('s'));
	strToken.Delete(loc, 1); // Removal of trailing 's'

	return strToken;
}

CString CMACBusVersteherView::GetMSElapsed(CString strLastTimestamp, CString strCurrentTimestamp, bool bReport)
{
	// MSB_RS485_PLUS timestamps are just floating point numbers, representing fractions of a second
	CString strMSElapsed = _T("");
	static double dMinDelay = ULONG_MAX;
	static double dMaxDelay = 0;

	if (!bReport) // Normal call
	{
		if (strLastTimestamp.IsEmpty())
		{
			strMSElapsed = _T("\t[UNDEFINED]");
		}
		else
		{
			double dLastTimespamp = strtod(strLastTimestamp, NULL);
			double dCurrentTimespamp = strtod(strCurrentTimestamp, NULL);
			double dMSElapsed = dCurrentTimespamp - dLastTimespamp;

			strMSElapsed.Format(_T("\t[%.9lf sec]"), dMSElapsed); // MSB_RS485_PLUS export also outputs this with 9 decimal places

			// Bookkeeping
			if (dMSElapsed < dMinDelay)
			{
				dMinDelay = dMSElapsed;
			}

			if (dMSElapsed > dMaxDelay)
			{
				dMaxDelay = dMSElapsed;
			}
		}
	}
	else // Report call
	{
		strMSElapsed.Format(_T("Min delay between log lines: %lf sec\nMax delay between log lines: %lf sec"), dMinDelay, dMaxDelay);
	}

	return strMSElapsed;
}

CString CMACBusVersteherView::GetTimestampFromItem(CString oneItem)
{
	int pos = oneItem.Find(_T('|'));

	CString strTimestamp = oneItem.Left(pos);

	return strTimestamp;
}

CString CMACBusVersteherView::GetValueFromItem(CString oneItem)
{
	int pos = oneItem.Find(_T('|')) + 1;

	CString strValue = oneItem.Mid(pos);

	return strValue;
}

void CMACBusVersteherView::TransactionStatistics(CRichEditCtrl& crec)
{
	AppendToLogAndScroll(crec, _T("\n\nTransaction Delay Statistics\n"), RGB(0, 0, 0));
	
	CString strHeadline = _T("Slave\t\tRole\t\tMIN\t\tMAX\t\tAVG\t\tNumber of transactions");
	AppendToLogAndScroll(crec, strHeadline, RGB(0, 0, 0));
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		CString strTransactionsCount = _T("");
		strTransactionsCount.Format(_T("%lu"), m_KnownSlaves[i].dwTransactions);

		CString strReportline = _T("");
		
		if (m_KnownSlaves[i].dwTransactions) // Slave had transactions
		{
			CString strTransactionsCount("");
			strTransactionsCount.Format(_T("%lu"), m_KnownSlaves[i].dwTransactions);
			
			strReportline += _T("\n");
			strReportline += m_KnownSlaves[i].strName;
			strReportline += _T("\t");
			strReportline += m_KnownSlaves[i].strRole;
			strReportline += _T("\t");
			strReportline += _T("\t");
			strReportline += GetMinDelay(i);
			strReportline += _T("\t");
			strReportline += GetMaxDelay(i);
			strReportline += _T("\t");
			strReportline += GetMeanDelay(i);
			strReportline += _T("\t");
			strReportline += strTransactionsCount;
					
			strReportline += _T("\n");
		}

		AppendToLogAndScroll(crec, strReportline, RGB(0, 0, 0));
	}
}

void CMACBusVersteherView::BlockExchangeStatistics(CRichEditCtrl& crec)
{
	CList<int, int> alreadyProcessed;
	
	AppendToLogAndScroll(crec, _T("\n\nBlock Exchange Latency Statistics\n"), RGB(0, 0, 0));

	CString strHeadline = _T("Block\t\tDirection\t\tMIN\t\tMAX\t\tAVG\t\tNumber of blocks - 1\n");
	AppendToLogAndScroll(crec, strHeadline, RGB(0, 0, 0));

	POSITION pos = m_BlockExchangeTrace.GetHeadPosition();
	BYTE anchorID = 0;
	SEND_DIRECTION anchorDirection = SEND_DIRECTION::UNDEFINED;
	SLAVE anchorSlave = SLAVE::UNKNOWN;
	
	while (pos)
	{
		double dBeginTimestamp = 0.;
		double dThisLatency = 0.;
		double dMinLatency = DBL_MAX;
		double dMaxLatency = 0.;
		double dSumLatencies = 0.;
		double dMeanLatency = 0.;
		double dNInstances = 0.;
		
		BlockTraceItem bti = m_BlockExchangeTrace.GetNext(pos);
		int combinedID = bti.ID + 100 * bti.activeSlave;

		if (bti.send_direction == SEND_DIRECTION::SLAVE_TO_MASTER)
		{
			combinedID += 10000;
		}

		// combinedID now is unique for each combination of block, direction, and slave
		
		if ((alreadyProcessed.Find(combinedID) == NULL) && !anchorID) 
		{
			alreadyProcessed.AddTail(combinedID);
			anchorID = bti.ID; // First ID...
			anchorDirection = bti.send_direction; //...direction...
			anchorSlave = bti.activeSlave; // and slave combination still waiting to be processed
			dBeginTimestamp = bti.dTimeSent;
			// Look for next entry with same ID
			POSITION pos2 = pos;
			BlockTraceItem bti2;

			while (pos2)
			{
				bti2 = m_BlockExchangeTrace.GetNext(pos2);

				if (bti2.ID == anchorID && 
					bti2.send_direction == anchorDirection &&
					bti2.activeSlave == anchorSlave) // Next entry with same combi
				{
					dThisLatency = bti2.dTimeSent - dBeginTimestamp;
					dBeginTimestamp = bti2.dTimeSent;
					dSumLatencies += dThisLatency;
					dNInstances++;

					if (dThisLatency < dMinLatency)
					{
						dMinLatency = dThisLatency;
					}

					if (dThisLatency > dMaxLatency)
					{
						dMaxLatency = dThisLatency;
					}
				}
			}

			dMeanLatency = dSumLatencies / dNInstances;

			anchorID = 0;

			if (dNInstances > 0) // At least one interval present ==> indicators valid
			{
				CString strSend = _T("");

				if (bti.send_direction == SEND_DIRECTION::MASTER_TO_SLAVE)
				{
					strSend.Format(_T("M->%s"), SlaveEnumToCString(bti.activeSlave));

					if (bti.activeSlave == SLAVE::LMS || bti.activeSlave == SLAVE::PCS)
					{
						strSend += _T('\t');
					}
				}
				else if (bti.send_direction == SEND_DIRECTION::SLAVE_TO_MASTER)
				{
					strSend.Format(_T("M<-%s"), SlaveEnumToCString(bti.activeSlave));

					if (bti.activeSlave == SLAVE::LMS || bti.activeSlave == SLAVE::PCS)
					{
						strSend += _T('\t');
					}
				}

				CString strReportline = _T("");
				strReportline.Format(_T("%d\t\t%s\t%lf\t%lf\t%lf\t%d\n"),
					bti.ID,
					strSend,
					dMinLatency,
					dMaxLatency,
					dMeanLatency,
					int(dNInstances));
				AppendToLogAndScroll(crec, strReportline, RGB(0, 0, 0));
			}
		}
	}
}

void CMACBusVersteherView::TransactionSequence(CRichEditCtrl& crec)
{
	AppendToLogAndScroll(crec, _T("\n\nTransaction Sequence\n"), RGB(0, 0, 0));

	POSITION pos = m_BlockExchangeTrace.GetHeadPosition();
	
	while (pos)
	{
		BlockTraceItem bti = m_BlockExchangeTrace.GetNext(pos);

		CString strOneBlock = _T("");
		strOneBlock.Format(_T("%17gsec\t\t\t%d\n"), bti.dTimeSent, bti.ID);
		AppendToLogAndScroll(crec, strOneBlock, RGB(0, 0, 0));
	}
}

CString CMACBusVersteherView::GetMinDelay(int iSlavePos)
{
	double dMinDelay = DBL_MAX;
	POSITION pos;
	bool bMinDelayIsValid = false;

	pos = m_KnownSlaves[iSlavePos].transactions.GetHeadPosition();
	
	while (pos)
	{
		TransactionItem ti = m_KnownSlaves[iSlavePos].transactions.GetNext(pos);
		double dDelay = ti.dTransactionEndTime - ti.dTransactionStartTime;

		if (ti.dTransactionStartTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction start time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMinDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		if (ti.dTransactionEndTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction end time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMinDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		if (dDelay < dMinDelay)
		{
			dMinDelay = dDelay;
			bMinDelayIsValid = true;
		}
	}

	CString strMinDelay = _T("");

	if (bMinDelayIsValid)
	{
		strMinDelay.Format(_T("%.9lf"), dMinDelay);
	}
	else
	{
		strMinDelay = _T("INVALID");
	}

	return strMinDelay;
}

CString CMACBusVersteherView::GetMaxDelay(int iSlavePos)
{
	double dMaxDelay = 0.;
	POSITION pos;
	bool bMaxDelayIsValid = false;

	pos = m_KnownSlaves[iSlavePos].transactions.GetHeadPosition();

	while (pos)
	{
		TransactionItem ti = m_KnownSlaves[iSlavePos].transactions.GetNext(pos);
		double dDelay = ti.dTransactionEndTime - ti.dTransactionStartTime;

		if (ti.dTransactionStartTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction start time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMaxDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		if (ti.dTransactionEndTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction end time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMaxDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		if (dDelay > dMaxDelay)
		{
			dMaxDelay = dDelay;
			bMaxDelayIsValid = true;
		}
	}

	CString strMaxDelay = _T("");

	if (bMaxDelayIsValid)
	{
		strMaxDelay.Format(_T("%.9lf"), dMaxDelay);
	}
	else
	{
		strMaxDelay = _T("INVALID");
	}

	return strMaxDelay;
}

CString CMACBusVersteherView::GetMeanDelay(int iSlavePos)
{
	double dMeanDelay = 0.;
	double dDelaySum = 0.;
	double n = 0;
	POSITION pos;
	bool bMeanDelayIsValid = false;

	pos = m_KnownSlaves[iSlavePos].transactions.GetHeadPosition();

	while (pos)
	{
		TransactionItem ti = m_KnownSlaves[iSlavePos].transactions.GetNext(pos);
		double dDelay = ti.dTransactionEndTime - ti.dTransactionStartTime;

		if (ti.dTransactionStartTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction start time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMeanDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		if (ti.dTransactionEndTime == 0.)
		{
			CString strErrMsg = _T("");
			strErrMsg.Format(_T("Transaction end time is zero for %s, %s"), m_KnownSlaves[iSlavePos].strName, m_KnownSlaves[iSlavePos].strRole);
			::MessageBox(NULL, strErrMsg, _T("GetMeanDelay error"), MB_OK | MB_ICONERROR);
			continue; // because dDelay is invalid
		}

		dDelaySum += dDelay;
		n++;
		bMeanDelayIsValid = true;
	}

	dMeanDelay = dDelaySum / n;

	CString strMeanDelay = _T("");

	if (bMeanDelayIsValid)
	{
		strMeanDelay.Format(_T("%.9lf"), dMeanDelay);
	}
	else
	{
		strMeanDelay = _T("INVALID");
	}

	return strMeanDelay;
}

void CMACBusVersteherView::AppendToInterimBuffer(CString strOneLine4Output, COLORREF colorref)
{
	InterimBufferItem ibi;
	ibi.strLine = strOneLine4Output;
	ibi.colorref = colorref;

	m_InterimBuffer.AddTail(ibi);
}

COLORREF CMACBusVersteherView::GetSlaveColor(CString strContainsSlaveName)
{
	COLORREF cr = RGB(0, 0, 0);
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (-1 != strContainsSlaveName.Find(m_KnownSlaves[i].strName))
		{
			cr = m_KnownSlaves[i].colorref;
			break;
		}
	}

	return cr;
}

BYTE CMACBusVersteherView::DecodeStringPair(CString strHigh, CString strLow, CString& strErrorSummary)
{
    BYTE result = 0;
   
    BYTE ucHigh = ASCIICharFromString(strHigh);

    if (!IsValidNibble(ucHigh))
    {
        CString strErrMsg = _T("");
        strErrMsg.Format(_T("ERROR: %d is not a valid nibble "), ucHigh);
        // ::MessageBox(NULL, strErrMsg, _T("Error"), MB_OK | MB_ICONERROR); WP2020819: Do not halt the workflow
		strErrorSummary = strErrMsg;
		m_nErrorsFound++;
    }

    BYTE ucLow = ASCIICharFromString(strLow);

    if (!IsValidNibble(ucLow))
    {
        CString strErrMsg = _T("");
        strErrMsg.Format(_T("ERROR: %d is not a valid nibble"), ucLow);
        // ::MessageBox(NULL, strErrMsg, _T("Error"), MB_OK | MB_ICONERROR); WP2020819: Do not halt the workflow
		strErrorSummary += strErrMsg;
		m_nErrorsFound++;
    }

    CString strByte = _T("");
    strByte.Format(_T("%c%c"), ucHigh, ucLow);
    result = ASCIICharFromString(strByte);

    return result;
}

BYTE CMACBusVersteherView::ASCIICharFromString(CString strHexNibbleTableEntry)
{
    BYTE asciiChar = (BYTE)(int)strtol(strHexNibbleTableEntry.GetString(), NULL, 16);

    return asciiChar;
}

bool CMACBusVersteherView::IsValidNibble(BYTE bNibble)
{
    bool bAnswer = false;
   
    if ((bNibble >= 0x30 && bNibble <= 0x39) || // 0...9
        (bNibble >= 0x41 && bNibble <= 0x46))   // A...F
    {
        bAnswer = true;
    }

    return bAnswer;
}

CMACBusVersteherView::SLAVE CMACBusVersteherView::WhoAnswered(CString strToExamine)
{
	SLAVE slaveIdentified = SLAVE::UNKNOWN;

	if (-1 != strToExamine.Find(_T("MAC_LMS")))
	{
		slaveIdentified = SLAVE::LMS;
	}
	else if (-1 != strToExamine.Find(_T("MAC_PCS")))
	{
		slaveIdentified = SLAVE::PCS;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls1")))
	{
		slaveIdentified = SLAVE::GDS_WLS_1;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls2")))
	{
		slaveIdentified = SLAVE::GDS_WLS_2;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls3")))
	{
		slaveIdentified = SLAVE::GDS_WLS_3;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls4")))
	{
		slaveIdentified = SLAVE::GDS_WLS_4;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls5")))
	{
		slaveIdentified = SLAVE::GDS_WLS_5;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls6")))
	{
		slaveIdentified = SLAVE::GDS_WLS_6;
	}
	else if (-1 != strToExamine.Find(_T("MAC_GdsWls7")))
	{
		slaveIdentified = SLAVE::GDS_WLS_7;
	}

	return slaveIdentified;
}

CString CMACBusVersteherView::SlaveEnumToCString(SLAVE slave)
{
	CString strResult = _T("");
	
	switch (slave)
	{
		case UNKNOWN:
			strResult = _T("UNKNOWN");
			break;

		case GDS_WLS_1:
			strResult = _T("GDS_WLS_1");
			break;

		case GDS_WLS_2:
			strResult = _T("GDS_WLS_2");
			break;

		case GDS_WLS_3:
			strResult = _T("GDS_WLS_3");
			break;

		case GDS_WLS_4:
			strResult = _T("GDS_WLS_4");
			break;

		case GDS_WLS_5:
			strResult = _T("GDS_WLS_5");
			break;

		case GDS_WLS_6:
			strResult = _T("GDS_WLS_6");
			break;

		case GDS_WLS_7:
			strResult = _T("GDS_WLS_7");
			break;

		case LMS:
			strResult = _T("LMS");
			break;

		case PCS:
			strResult = _T("PCS");
			break;

		default:
			strResult = _T("INTERNAL_ERROR");
	}

	return strResult;
}

// https://bytes.com/topic/c/answers/215818-simple-base-converter-program

static int HexString2Int(char *s) 
{
	char *tmp = s;

	// tmp=TrimLeadingZero(s); WP20200622: Don't need this

	int i;
	int t;
	int n = 0;

	size_t l = strlen(tmp);

	for (i = l-1; i >= 0; --i) 
	{
		t = Hex2Dec(*tmp);
	
		if (t == 0) 
		{
			tmp++;
			continue; // jump out if is 0
		}

		n += t*(int)pow(16., i);
		++tmp;
	}

	return n;
}

static int Hex2Dec(char hex) 
{
	if (isupper(hex))
	{
		return hex - '7'; // upper hex chars to integers
	}
	else if (islower(hex))
	{
		return hex - 'W'; // lower hex chars to integers
	}
	else
	{
		return hex - '0'; // digit chars to their corresponding integers
	}
}

