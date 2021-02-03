
// MACBusVersteherView.cpp : implementation of the CMACBusVersteherView class
//

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
	m_nLeadingColumns = 0;
	m_bInsideText = false;
	m_bLastCharWasETX = false;

	KnownSlaveAddresses[0].strName = _T("MAC_LMS");
	KnownSlaveAddresses[0].iUnmaskedNumber = 30;
	KnownSlaveAddresses[0].strRole = _T("EADR");
	KnownSlaveAddresses[0].colorref = RGB(255, 137, 0);
	KnownSlaveAddresses[0].nOpenTransactions = -1; // Magic number for initial state, do not accept a close before an open
	
	KnownSlaveAddresses[1].strName = _T("MAC_PCS");
	KnownSlaveAddresses[1].iUnmaskedNumber = 28;
	KnownSlaveAddresses[1].strRole = _T("EADR");
	KnownSlaveAddresses[1].colorref = RGB(231, 72, 86);
	KnownSlaveAddresses[1].nOpenTransactions = -1;
	
	KnownSlaveAddresses[2].strName = _T("MAC_GdsWls1"); 
	KnownSlaveAddresses[2].iUnmaskedNumber = 1;
	KnownSlaveAddresses[2].strRole = _T("EADR");
	KnownSlaveAddresses[2].colorref = RGB(0, 120, 215);
	KnownSlaveAddresses[2].nOpenTransactions = -1;
	
	KnownSlaveAddresses[3].strName = _T("MAC_GdsWls2"); 
	KnownSlaveAddresses[3].iUnmaskedNumber = 2;
	KnownSlaveAddresses[3].strRole = _T("EADR");
	KnownSlaveAddresses[3].colorref = RGB(0, 153, 188);
	KnownSlaveAddresses[3].nOpenTransactions = -1;
	
	KnownSlaveAddresses[4].strName = _T("MAC_GdsWls3"); 
	KnownSlaveAddresses[4].iUnmaskedNumber = 3;
	KnownSlaveAddresses[4].strRole = _T("EADR");
	KnownSlaveAddresses[4].colorref = RGB(122, 117, 116);
	KnownSlaveAddresses[4].nOpenTransactions = -1;
	
	KnownSlaveAddresses[5].strName = _T("MAC_GdsWls4"); 
	KnownSlaveAddresses[5].iUnmaskedNumber = 4;
	KnownSlaveAddresses[5].strRole = _T("EADR");
	KnownSlaveAddresses[5].colorref = RGB(142, 140, 216);
	KnownSlaveAddresses[5].nOpenTransactions = -1;

	KnownSlaveAddresses[6].strName = _T("MAC_GdsWls5"); 
	KnownSlaveAddresses[6].iUnmaskedNumber = 5;
	KnownSlaveAddresses[6].strRole = _T("EADR");
	KnownSlaveAddresses[6].colorref = RGB(0, 135, 195);
	KnownSlaveAddresses[6].nOpenTransactions = -1;
	
	KnownSlaveAddresses[7].strName = _T("MAC_GdsWls6"); 
	KnownSlaveAddresses[7].iUnmaskedNumber = 6;
	KnownSlaveAddresses[7].strRole = _T("EADR");
	KnownSlaveAddresses[7].colorref = RGB(104, 118, 138);
	KnownSlaveAddresses[7].nOpenTransactions = -1;
	
	KnownSlaveAddresses[8].strName = _T("MAC_GdsWls7"); 
	KnownSlaveAddresses[8].iUnmaskedNumber = 7;
	KnownSlaveAddresses[8].strRole = _T("EADR");
	KnownSlaveAddresses[8].colorref = RGB(202, 80, 16);
	KnownSlaveAddresses[8].nOpenTransactions = -1;

	KnownSlaveAddresses[9].strName = _T("MAC_LMS"); // TODO: Where the fuck in the embedded code are the SADRs computed (fucking assembler?)
	KnownSlaveAddresses[9].iUnmaskedNumber = 62; // 30 + 32;
	KnownSlaveAddresses[9].strRole = _T("SADR");
	KnownSlaveAddresses[9].colorref = RGB(255, 137, 0);
	KnownSlaveAddresses[9].nOpenTransactions = -1;
	
	KnownSlaveAddresses[10].strName = _T("MAC_PCS");
	KnownSlaveAddresses[10].iUnmaskedNumber = 60; // 28 + 32;
	KnownSlaveAddresses[10].strRole = _T("SADR");
	KnownSlaveAddresses[10].colorref = RGB(231, 72, 86);
	KnownSlaveAddresses[10].nOpenTransactions = -1;
	
	KnownSlaveAddresses[11].strName = _T("MAC_GdsWls1"); 
	KnownSlaveAddresses[11].iUnmaskedNumber = 33; // 1 + 32;
	KnownSlaveAddresses[11].strRole = _T("SADR");
	KnownSlaveAddresses[11].colorref = RGB(0, 120, 215);
	KnownSlaveAddresses[11].nOpenTransactions = -1;
	
	KnownSlaveAddresses[12].strName = _T("MAC_GdsWls2"); 
	KnownSlaveAddresses[12].iUnmaskedNumber = 34; // 2 + 32;
	KnownSlaveAddresses[12].strRole = _T("SADR");
	KnownSlaveAddresses[12].colorref = RGB(0, 153, 188);
	KnownSlaveAddresses[12].nOpenTransactions = -1;
	
	KnownSlaveAddresses[13].strName = _T("MAC_GdsWls3"); 
	KnownSlaveAddresses[13].iUnmaskedNumber = 35; // 3 + 32;
	KnownSlaveAddresses[13].strRole = _T("SADR");
	KnownSlaveAddresses[13].colorref = RGB(122, 117, 116);
	KnownSlaveAddresses[13].nOpenTransactions = -1;
	
	KnownSlaveAddresses[14].strName = _T("MAC_GdsWls4"); 
	KnownSlaveAddresses[14].iUnmaskedNumber = 36; // 4 + 32;
	KnownSlaveAddresses[14].strRole = _T("SADR");
	KnownSlaveAddresses[14].colorref = RGB(142, 140, 216);
	KnownSlaveAddresses[14].nOpenTransactions = -1;

	KnownSlaveAddresses[15].strName = _T("MAC_GdsWls5"); 
	KnownSlaveAddresses[15].iUnmaskedNumber = 37; // 5 + 32;
	KnownSlaveAddresses[15].strRole = _T("SADR");
	KnownSlaveAddresses[15].colorref = RGB(0, 135, 195);
	KnownSlaveAddresses[15].nOpenTransactions = -1;
	
	KnownSlaveAddresses[16].strName = _T("MAC_GdsWls6"); 
	KnownSlaveAddresses[16].iUnmaskedNumber = 38; // 6 + 32;
	KnownSlaveAddresses[16].strRole = _T("SADR");
	KnownSlaveAddresses[16].colorref = RGB(104, 118, 138);
	KnownSlaveAddresses[16].nOpenTransactions = -1;
	
	KnownSlaveAddresses[17].strName = _T("MAC_GdsWls7"); 
	KnownSlaveAddresses[17].iUnmaskedNumber = 39; // 7 + 32;
	KnownSlaveAddresses[17].strRole = _T("SADR");
	KnownSlaveAddresses[17].colorref = RGB(202, 80, 16);
	KnownSlaveAddresses[17].nOpenTransactions = -1;
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

// Version for Serial Sniffer
//void CMACBusVersteherView::Parse(const char* pLog)
//{
//	CRichEditCtrl& crec = GetRichEditCtrl();
//	char oneLine[256];
//	ZeroMemory(oneLine, 256);
//	unsigned int linecount = 0;
//	CString strCurrentTimestamp = _T("");
//	CString strLastTimestamp = _T("");
//
//	if (pLog != NULL)
//	{
//		bool bSuppressFullAnalysis = false;
//		
//		int answer = ::MessageBox(NULL, _T("Nur Handshake ausgeben"), _T("Analyseumfang"), MB_ICONQUESTION | MB_YESNO);
//
//		if (answer == IDYES)
//		{
//			bSuppressFullAnalysis = true;
//		}
//		
//		// If crec is already populated, empty it before parsing a new pLog buffer
//		crec.SetSel(0, -1);
//		crec.Clear();
//		
//		char* pLogWalker = const_cast<char*>(pLog);
//
//		m_nLeadingColumns = GetNumberOfLeadingColumns(pLogWalker);
//		
//		CString strDummy = AnalizeToken(_T("Dummy"), true, false); // Called for initialization
//
//		ACTIVE_SPEAKER last_speaker = ACTIVE_SPEAKER::UNDEFINED;
//		ACTIVE_SPEAKER active_speaker = ACTIVE_SPEAKER::UNDEFINED;
//		
//		while (*pLogWalker != _T('\0'))
//		{
//			ZeroMemory(oneLine, 256);
//			int pos = 0;
//
//			while (*pLogWalker != _T('\n'))
//			{
//				oneLine[pos] = *pLogWalker;
//				pos++;
//				pLogWalker++;
//			}
//
//			oneLine[pos] = *pLogWalker;
//			pos++;
//			pLogWalker++;
//			linecount++;
//
//			COLORREF colorref = RGB(0, 0, 0);
//			CString strOneLine4Output = _T("");
//			strOneLine4Output.Format(_T("%07u\t%s"), linecount, oneLine);
//			CString strOneLine4Analysis = _T("");
//			strOneLine4Analysis.Format(_T("%s"), oneLine);
//
//			if (linecount == 1)
//			{
//				if (-1 == strOneLine4Output.Find(_T("Logfile SerialSniffer Version 6")))
//				{
//					CString strWarning = _T("");
//					strWarning.Format(_T("File does not begin with \"%s\""), _T("Logfile SerialSniffer Version 6"));
//					::MessageBox(NULL, strWarning, _T("Warning"), MB_OK | MB_ICONWARNING);
//				}
//			}
//
//			if (-1 != strOneLine4Output.Find(_T("Comport A")))
//			{
//				colorref = RGB(128, 0, 0);
//			}
//			else if (-1 != strOneLine4Output.Find(_T("Comport B")))
//			{
//				colorref = RGB(0, 128, 0);
//			}
//			
//			int loc = strOneLine4Output.Find(_T('\r'));
//			
//			if (loc != -1 && colorref != RGB(0, 0, 0))
//			{
//				strOneLine4Output.Delete(loc, 2); // Removal of \r\n
//			}
//			
//			last_speaker = active_speaker;
//			
//			if (-1 != strOneLine4Analysis.Find(_T("Comport A")))
//			{
//				active_speaker = ACTIVE_SPEAKER::COMPORT_A;
//				strLastTimestamp = strCurrentTimestamp;
//				strCurrentTimestamp = GetTimestamp(strOneLine4Analysis);
//
//			}
//			else if (-1 != strOneLine4Analysis.Find(_T("Comport B")))
//			{
//				active_speaker = ACTIVE_SPEAKER::COMPORT_B;
//				strLastTimestamp = strCurrentTimestamp;
//				strCurrentTimestamp = GetTimestamp(strOneLine4Analysis);
//
//			}
//
//			if (last_speaker != active_speaker && !bSuppressFullAnalysis)
//			{
//				AppendToLogAndScroll(crec, _T("\n"), RGB(0, 0, 0));
//			}
//
//			if (colorref == RGB(128, 0, 0) || colorref == RGB(0, 128, 0))
//			{
//				strOneLine4Output += GetMSElapsed(strLastTimestamp, strCurrentTimestamp, false);
//			}
//			
//			if (!bSuppressFullAnalysis)
//			{
//				AppendToLogAndScroll(crec, strOneLine4Output, colorref);
//			}
//			
//			if (active_speaker != ACTIVE_SPEAKER::UNDEFINED) // Not a header line
//			{
//				CString strAnalysis = _T("");
//				
//				// This is a very shallow kind of 'analysis' (needed for count of control characters)
//				strAnalysis = Analysis(strOneLine4Analysis); // Analysis() is the single caller for analysis calls to AnalizeToken()
//				// AppendToLogAndScroll(crec, strAnalysis, RGB(0, 0, 128));
//				// AppendToLogAndScroll(crec, _T("\n\n"), RGB(0, 0, 0));
//
//				CString strPredecessorLine = _T("");
//				CString strSuccessorLine = _T("");
//
//				if (linecount >= 7) // Predecessor line available
//				{
//					strPredecessorLine = GetPredecessorLine(pLog, pLogWalker);
//				}
//
//				if (*pLogWalker != _T('\0')) // Successor line available
//				{
//					strSuccessorLine = GetSuccessorLine(pLogWalker);
//				}
//				
//				strAnalysis = SemanticAnalysis(strPredecessorLine, strOneLine4Analysis, strSuccessorLine, pLog, pLogWalker, strCurrentTimestamp);
//				strAnalysis.TrimLeft();
//				int logicEnd = strAnalysis.Find(_T("EOT"));
//
//				while (logicEnd != -1)
//				{
//					logicEnd += 3;
//					
//					if (logicEnd < strAnalysis.GetLength())
//					{
//						strAnalysis.SetAt(logicEnd, _T('\n'));
//					}
//					else
//					{
//						//strAnalysis.Append(_T("\n"));
//					}
//					
//					logicEnd = strAnalysis.Find(_T("EOT"), logicEnd);
//				}
//
//				CString strNLLineNumber = _T("");
//				strNLLineNumber.Format(_T("\n%07u\t"), linecount);
//
//				strAnalysis.Replace(_T("\n"), strNLLineNumber);
//
//				CString strAnalysisDone = strNLLineNumber + strAnalysis;
//
//				int curPos = 0;
//				CString strToken = strAnalysisDone.Tokenize(_T("\n"), curPos);
//
//				while (strToken != _T(""))
//				{
//					COLORREF colorref = RGB(0, 0, 0);
//					
//					if (LineContainsHandshake(strToken, colorref))
//					{
//						CString strFinalLine = _T("\n") + strToken;
//						AppendToLogAndScroll(crec, strFinalLine, colorref);
//					}
//					else
//					{
//						CString strFinalLine = _T("\n") + strToken;
//						
//						if (!bSuppressFullAnalysis)
//						{
//							AppendToLogAndScroll(crec, strFinalLine, RGB(0, 0, 0));
//						}
//					}
//
//					strToken = strAnalysisDone.Tokenize(_T("\n"), curPos);
//				}
//				
//				if (!bSuppressFullAnalysis)
//				{
//					AppendToLogAndScroll(crec, _T("\n\n"), RGB(0, 0, 0));
//				}
//			}
//		}
//
//		// Alert user of parse completion
//		if (!bSuppressFullAnalysis)
//		{
//			CString strDelayExtrema = GetMSElapsed(_T(""), _T(""), true); // Fetch report
//			AppendToLogAndScroll(crec, strDelayExtrema, RGB(0, 0, 0));
//		}
//
//		// Color test:
//		/*CString strSampleText = _T("\nThe quick brown fox jumps over the lazy dog");
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[0].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[1].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[2].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[3].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[4].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[5].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[6].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[7].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[8].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[9].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[10].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[11].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[12].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[13].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[14].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[15].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[16].colorref);
//		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[17].colorref);*/
//
//		crec.FlashWindow(TRUE); // Does not work
//		Sleep(500);
//		crec.FlashWindow(FALSE);
//
//		CString strReport = AnalizeToken(_T("Dummy"), false, true); // Called for report
//		::MessageBox(NULL, strReport, _T("Information"), MB_OK | MB_ICONINFORMATION);
//	}
//}

void CMACBusVersteherView::Parse(const char* pLog)
{
	CRichEditCtrl& crec = GetRichEditCtrl();
	char oneLine[256];
	ZeroMemory(oneLine, 256);
	unsigned int linecount = 0;
	CString strCurrentTimestamp = _T("");
	CString strLastTimestamp = _T("");

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
		
		char* pLogWalker = const_cast<char*>(pLog);

		m_nLeadingColumns = GetNumberOfLeadingColumns(pLogWalker);
		
		// CString strDummy = AnalizeToken(_T("Dummy"), true, false); // Called for initialization Not very useful, output already suppressed

		ACTIVE_SPEAKER last_speaker = ACTIVE_SPEAKER::UNDEFINED;
		ACTIVE_SPEAKER active_speaker = ACTIVE_SPEAKER::UNDEFINED;
		
		while (*pLogWalker != _T('\0'))
		{
			ZeroMemory(oneLine, 256);
			int pos = 0;

			while (*pLogWalker != _T('\n'))
			{
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
			
			int loc = strOneLine4Output.Find(_T('\n'));
			
			if (loc != -1 && colorref != RGB(0, 0, 0))
			{
				strOneLine4Output.Delete(loc, 2); // Removal of \n
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

			if (last_speaker != active_speaker && !bSuppressFullAnalysis)
			{
				AppendToLogAndScroll(crec, _T("\n"), RGB(0, 0, 0));
			}

			if (colorref == RGB(128, 0, 0) || colorref == RGB(0, 128, 0))
			{
				strOneLine4Output += GetMSElapsed(strLastTimestamp, strCurrentTimestamp, false);
			}
			
			if (!bSuppressFullAnalysis)
			{
				AppendToLogAndScroll(crec, strOneLine4Output, colorref);
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

				if (linecount >= 3) // Predecessor line available
				{
					strVirtualPredecessorLine = GetPredecessorLine(pLog, pLogWalker);
				}

				if (*pLogWalker != _T('\0')) // Successor line available
				{
					strVirtualSuccessorLine = GetSuccessorLine(pLogWalker);
				}
				
				strAnalysis = SemanticAnalysis(strVirtualPredecessorLine, strVirtualThisLine, strVirtualSuccessorLine, pLog, pLogWalker, strCurrentTimestamp);
				strAnalysis.TrimLeft();
				int logicEnd = strAnalysis.Find(_T("EOT"));

				while (logicEnd != -1)
				{
					logicEnd += 3;
					
					if (logicEnd < strAnalysis.GetLength())
					{
						strAnalysis.SetAt(logicEnd, _T('\n'));
					}
					else
					{
						//strAnalysis.Append(_T("\n"));
					}
					
					logicEnd = strAnalysis.Find(_T("EOT"), logicEnd);
				}

				CString strNLLineNumber = _T("");
				strNLLineNumber.Format(_T("\n%07u\t"), linecount);

				strAnalysis.Replace(_T("\n"), strNLLineNumber);

				CString strAnalysisDone = strNLLineNumber + strAnalysis;

				int curPos = 0;
				CString strToken = strAnalysisDone.Tokenize(_T("\n"), curPos);

				while (strToken != _T(""))
				{
					COLORREF colorref = RGB(0, 0, 0);
					
					if (LineContainsHandshake(strToken, colorref))
					{
						CString strFinalLine = _T("\n") + strToken;
						AppendToLogAndScroll(crec, strFinalLine, colorref);
					}
					else
					{
						CString strFinalLine = _T("\n") + strToken;
						
						if (!bSuppressFullAnalysis)
						{
							AppendToLogAndScroll(crec, strFinalLine, RGB(0, 0, 0));
						}
					}

					strToken = strAnalysisDone.Tokenize(_T("\n"), curPos);
				}
				
				if (!bSuppressFullAnalysis)
				{
					AppendToLogAndScroll(crec, _T("\n\n"), RGB(0, 0, 0));
				}
			}
		}

		// Alert user of parse completion
		if (!bSuppressFullAnalysis)
		{
			CString strDelayExtrema = GetMSElapsed(_T(""), _T(""), true); // Fetch report
			AppendToLogAndScroll(crec, strDelayExtrema, RGB(0, 0, 0));
		}

		// Color test:
		/*CString strSampleText = _T("\nThe quick brown fox jumps over the lazy dog");
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[0].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[1].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[2].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[3].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[4].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[5].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[6].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[7].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[8].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[9].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[10].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[11].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[12].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[13].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[14].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[15].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[16].colorref);
		AppendToLogAndScroll(crec, strSampleText, KnownSlaveAddresses[17].colorref);*/

		crec.FlashWindow(TRUE); // Does not work
		Sleep(500);
		crec.FlashWindow(FALSE);

		CString strReport = AnalizeToken(_T("Dummy"), false, true); // Called for report
		::MessageBox(NULL, strReport, _T("Information"), MB_OK | MB_ICONINFORMATION);
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
		if (nTokensSeen > m_nLeadingColumns) // There is a varying number of leading columns
		{
			CString strAnalizedToken = AnalizeToken(strToken, false, false); // Called for analysis
			strResult += strAnalizedToken;
			strResult += _T(" ");
		}
		
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
	int nTokensSeen = 0;

	// Just three lines are sufficient for sorts of shallow analysis, init to empty string tokens
	
	for (int i = 0; i < 16; i++)
	{
		m_aStrPredecessorTokens[i] = _T("");
		m_aStrTokens[i] = _T("");
		m_aStrSuccessorTokens[i] = _T("");
	}

	// Tokenize the three lines

	strToken = strPredecessorLine.Tokenize(_T(" "), curPos);
	nTokensSeen++;
	
	while (strToken != _T(""))
	{
		if (nTokensSeen > m_nLeadingColumns) // There is a varying number of leading columns
		{
			m_aStrPredecessorTokens[iTokenPos] = strToken;
			iTokenPos++;

			if (iTokenPos >= 17)
			{
				CString strErrMsg = _T("");
				strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strPredecessorLine);
				::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
				strResult = strErrMsg;
				
				return strResult;
			}
		}

		strToken = strPredecessorLine.Tokenize(_T(" \r\n"), curPos);
		nTokensSeen++;
	}

	curPos = 0;
	iTokenPos = 0;
	nTokensSeen = 0;
		
	strToken = strLine.Tokenize(_T(" "), curPos);
	nTokensSeen++;
	
	while (strToken != _T(""))
	{
		if (nTokensSeen > m_nLeadingColumns) // There is a varying number of leading columns
		{
			m_aStrTokens[iTokenPos] = strToken;
			iTokenPos++;

			if (iTokenPos >= 17)
			{
				CString strErrMsg = _T("");
				strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strLine);
				::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
				strResult = strErrMsg;
				
				return strResult;
			}
		}

		strToken = strLine.Tokenize(_T(" \r\n"), curPos);
		nTokensSeen++;
	}

	curPos = 0;
	iTokenPos = 0;
	nTokensSeen = 0;

	strToken = strSuccessorLine.Tokenize(_T(" "), curPos);
	nTokensSeen++;
	
	while (strToken != _T(""))
	{
		if (nTokensSeen > m_nLeadingColumns) // There is a varying number of leading columns
		{
			m_aStrSuccessorTokens[iTokenPos] = strToken;
			iTokenPos++;

			if (iTokenPos >= 17)
			{
				CString strErrMsg = _T("");
				strErrMsg.Format(_T("Error: More than 16 MAC tokens in %s"), strSuccessorLine);
				::MessageBox(NULL, strErrMsg, _T("Bad line format"), MB_OK | MB_ICONERROR);
				strResult = strErrMsg;
				
				return strResult;
			}
		}

		strToken = strSuccessorLine.Tokenize(_T(" \r\n"), curPos);
		nTokensSeen++;
	}

	// Tokenized three lines of log for shallow analysis 

	CString strParsed = _T("");

	for (int i = 0; i < 16; i++)
	{
		if (!m_aStrTokens[i].IsEmpty())
		{
			CONTROLCHARACTERS ctrlc = IdentifyToken(m_aStrTokens[i]);

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

					if (m_bLastCharWasETX) // Then this is the Block Check Character and the tokens are checksums, not control characters
					{
						if (m_aStrTokens[i] == _T("NUL"))
						{
							m_aStrTokens[i] = _T("0");
						}

						if (m_aStrTokens[i] == _T("SOH"))
						{
							m_aStrTokens[i] = _T("1");
						}

						if (m_aStrTokens[i] == _T("ACK"))
						{
							m_aStrTokens[i] = _T("6");
						}

						if (m_aStrTokens[i] == _T("BEL"))
						{
							m_aStrTokens[i] = _T("7");
						}
						
						if (m_aStrTokens[i] == _T("BS"))
						{
							m_aStrTokens[i] = _T("8");
						}
						
						if (m_aStrTokens[i] == _T("TAB"))
						{
							m_aStrTokens[i] = _T("9");
						}

						if (m_aStrTokens[i] == _T("LF"))
						{
							m_aStrTokens[i] = _T("A");
						}

						if (m_aStrTokens[i] == _T("VT"))
						{
							m_aStrTokens[i] = _T("B");
						}

						if (m_aStrTokens[i] == _T("FF"))
						{
							m_aStrTokens[i] = _T("C");
						}

						if (m_aStrTokens[i] == _T("CR"))
						{
							m_aStrTokens[i] = _T("D");
						}

						if (m_aStrTokens[i] == _T("SO"))
						{
							m_aStrTokens[i] = _T("E");
						}

						if (m_aStrTokens[i] == _T("SI"))
						{
							m_aStrTokens[i] = _T("F");
						}

						if (m_aStrTokens[i] == _T("_7F"))
						{
							m_aStrTokens[i] = _T("7F");
						}

						int iValue = HexString2Int((char*)m_aStrTokens[i].GetString()); // Block Check Character (checksum)
						strParsed.Format(_T(" BccIs_%d"), iValue);
						m_bLastCharWasETX = false;
					}
					else if (m_bInsideText)
					{
						strParsed.Format(_T(" %s"), ProcessNumericToken(m_aStrTokens[i]));
					}
					else
					{
						strParsed.Format(_T(" %s"), m_aStrTokens[i]);
					}
				
					strResult += strParsed;
					break;

				// case CONTROLCHARACTERS::NUL: // ASCII 00 Null This was a checksum!
				// strResult += _T(" NUL");
				// break;

				// case CONTROLCHARACTERS::SOH: // ASCII 01 Start_of_Header This was a checksum!
				// strResult += _T(" SOH");
				// break;

				case CONTROLCHARACTERS::STX: // ASCII 02 Start_of_Text
					// This is the start of a "text", which is a block (that needs deep analysis)
					strResult += DecodeBlock(pLog, pLogWalker);
					strResult += _T(" STX");
					m_bInsideText = true;
					break;

				case CONTROLCHARACTERS::ETX: // ASCII 03 End_of_Text
					strResult += _T(" ETX");
					m_bInsideText = false;
					m_bLastCharWasETX = true;
					break;

				case CONTROLCHARACTERS::EOT: // ASCII 04 End_of_transmission
					strResult += _T(" EOT");
					break;

				case CONTROLCHARACTERS::ENQ: // ASCII 05 Enquiry
					strParsed = DecodeSlaveEnquiry(i, strCurrentTimestamp);
					strResult += strParsed;
					break;

				// case CONTROLCHARACTERS::ACK: // ASCII 06 Acknowledgement This was a checksum!
				// strResult += _T(" ACK");
				// break;

				// case CONTROLCHARACTERS::BEL: // ASCII 07 Bell This was a checksum!
				// strResult += _T(" BEL");
				// break;

				// case CONTROLCHARACTERS::BS:  // ASCII 08 Backspace This was a checksum!
				// strResult += _T(" BS");
				// break;

				// case CONTROLCHARACTERS::TAB: // ASCII 09 -- Sometimes called HT, Serial Sniffer writes TAB This was a checksum!
				// strResult += _T(" TAB");
				// break;

				// case CONTROLCHARACTERS::LF:  // ASCII 10 Line_Feed This was a checksum!
				// strResult += _T(" LF");
				// break;

				// case CONTROLCHARACTERS::VT:  // ASCII 11 Vertical_Tab This was a checksum!
				// 	strResult += _T(" VT");
				// 	break;

				// case CONTROLCHARACTERS::FF:  // ASCII 12 Form_Feed This was a checksum!
				// strResult += _T(" FF");
				// break;

				// case CONTROLCHARACTERS::CR:  // ASCII 13 Carriage_Return This was a checksum!
				// strResult += _T(" CR");
				// break;

				// case CONTROLCHARACTERS::SO:  // ASCII 14 Shift_Out This was a checksum!
				// strResult += _T(" SO");
				// break;

				// case CONTROLCHARACTERS::SI:  // ASCII 15 Shift_In This was a checksum!
				// strResult += _T(" SI");
				// break;

				case CONTROLCHARACTERS::DLE: // ASCII 16 Data_link_escape
					if (Successor(i, false) == _T("30"))
					{
						// Anforderungsphase: Positive Rückmeldung von SADR/EADR
						strParsed = DecodePositiveRequestFeedback(i);
					}
					else if (Successor(i, false) == _T("31"))
					{
						// Übermittlungsphase: Positive Rückmeldung
						strParsed.Format(_T(" (DLE %s)==Positive_Transmission_Feedback(\"VERSTANDEN!\")"), Successor(i, false));
					}

					strResult += strParsed;
					break;

				case CONTROLCHARACTERS::DC1: // ASCII 17 Device_Cotrol_1
					strResult += _T(" DC1");
					break;

				case CONTROLCHARACTERS::DC2: // ASCII 18 Device_Cotrol_2
					strResult += _T(" DC2");
					break;

				case CONTROLCHARACTERS::DC3: // ASCII 19 -- just in case, never seen in practice!
					strResult += _T(" DC3");
					break;

				case CONTROLCHARACTERS::DC4: // ASCII 20 Device_Cotrol_4
					strResult += _T(" DC4");
					break;

				case CONTROLCHARACTERS::NAK: // ASCII 21 Negative_Acknowledgement
					strResult += _T(" NAK");
					break;

				case CONTROLCHARACTERS::SYN: // ASCII 22 Synchronous_Idle
					strResult += _T(" SYN");
					break;

				case CONTROLCHARACTERS::ETB: // ASCII 23 End_of_Trans_Block
					strResult += _T(" ETB");
					break;

				case CONTROLCHARACTERS::CAN: // ASCII 24 Cancel
					strResult += _T(" CAN");
					break;

				case CONTROLCHARACTERS::EM:  // ASCII 25 End_of_Medium
					strResult += _T(" EM");
					break;

				case CONTROLCHARACTERS::SUB: // ASCII 26 Substitute
					strResult += _T(" SUB");
					break;

				case CONTROLCHARACTERS::ESC: // ASCII 27 Escape
					strResult += _T(" ESC");
					break;

				case CONTROLCHARACTERS::FS:  // ASCII 28 File_Separator
					strResult += _T(" FS");
					break;

				case CONTROLCHARACTERS::GS:  // ASCII 29 Group_Separator
					strResult += _T(" GS");
					break;

				case CONTROLCHARACTERS::RS:  // ASCII 30 Record_Separator
					strResult += _T(" RS");
					break;

				case CONTROLCHARACTERS::US:  // ASCII 31 Unit_Separator
					strResult += _T(" US");
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
		strResult = Alias4ControlCharacters(m_aStrPredecessorTokens[15]);
	}
	else
	{
		strResult = Alias4ControlCharacters(m_aStrTokens[slot-1]);
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
		strResult = m_aStrSuccessorTokens[0];
	}
	else
	{
		strResult = m_aStrTokens[slot+1];
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

// Serial Sniffer version
//int CMACBusVersteherView::GetNumberOfLeadingColumns(char* pLog)
//{
//	char oneLine[256];
//
//	while (*pLog != _T('\0'))
//	{
//		ZeroMemory(oneLine, 256);
//		int pos = 0;
//
//		while (*pLog != _T('\n'))
//		{
//			oneLine[pos] = *pLog;
//			pos++;
//			pLog++;
//		}
//
//		if (oneLine[0] == _T('\r')) // This line terminates the header
//		{
//			break;
//		}
//		
//		oneLine[pos] = *pLog;
//		pos++;
//		pLog++;
//	}
//
//	// \r is followed by \n, skip it
//	pLog++;
//
//	// pLog now points to the first data line
//	int pos = 0;
//
//	// Get the line
//	
//	ZeroMemory(oneLine, 256);
//
//	while (*pLog != _T('\n'))
//	{
//		oneLine[pos] = *pLog;
//		pos++;
//		pLog++;
//	}
//
//	// Count its tokens
//
//	CString strOneLine(oneLine);
//	CString strToken = _T("");
//	int curPos = 0;
//	int nLeadingColsSeen = 0;
//
//	strToken = strOneLine.Tokenize(_T(" "), curPos);
//	nLeadingColsSeen++;
//
//	bool bIs2014Format = false; // Is the lofile an old example (DIBBus directory on my D:)
//	
//	while (strToken != _T(""))
//	{
//		strToken = strOneLine.Tokenize(_T(" \r"), curPos);
//		nLeadingColsSeen++;
//
//		if (strToken == _T("('Slave')") || strToken == _T("('Master')")) // This is the final leading 'decorator' column
//		{
//			break;
//		}
//
//		if (strToken == _T("('COM5:')") || strToken == _T("('COM6:')")) // This is the final leading 'decorator' column
//		{
//			bIs2014Format = true;
//			break;
//		}
//	}
//
//	if (bIs2014Format)
//	{
//		nLeadingColsSeen++; // Because here the additional char-format column follows
//	}
//
//	return nLeadingColsSeen;
//}

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

CString CMACBusVersteherView::GetPredecessorLine(const char* pLog, char* pLogWalker)
{
	char* pBackup = pLogWalker;

	while (*pLogWalker != _T('\r'))
	{
		pLogWalker--; // Back to this line's \r
	}

	pLogWalker--;

	while (*pLogWalker != _T('\r'))
	{
		pLogWalker--; // Back to last line's \r
	}

	while (*pLogWalker != _T('\n'))
	{
		pLogWalker--;
	}

	pLogWalker++;

	// pLogWalker now points to the start of the last line, get the line

	char oneLine[256];
	int pos = 0;
	
	ZeroMemory(oneLine, 256);

	while (*pLogWalker != _T('\n'))
	{
		oneLine[pos] = *pLogWalker;
		pos++;
		pLogWalker++;
	}

	CString strResult(oneLine);

	pLogWalker = pBackup;

	return strResult;
}
	
// Serial Sniffer version
//CString CMACBusVersteherView::GetSuccessorLine(char* pLogWalker)
//{
//	char* pBackup = pLogWalker;
//
//	char oneLine[256];
//	int pos = 0;
//
//	ZeroMemory(oneLine, 256);
//
//	while (*pLogWalker != _T('\r') && *pLogWalker != _T('\0'))
//	{
//		oneLine[pos] = *pLogWalker;
//		pos++;
//		pLogWalker++;
//	}
//	
//	CString strResult(oneLine);
//	
//	pLogWalker = pBackup;
//	
//	return strResult;
//}

// These data lines do not exist in Data Monitor export, but we have set the 
// Data Monitor window to show 16 values in a row. So it still makes sense
// to think of these lines containing 16 values each.
CString CMACBusVersteherView::GetSuccessorLine(char* pLogWalker)
{
	char* pBackup = pLogWalker;

	// Advance pLogWalker to start position (this is the next "virtual line")
	int nLineBreaks = 0;

	while (*pLogWalker != _T('\0'))
	{
		if (*pLogWalker == _T('\n'))
		{
			nLineBreaks++;
		}

		if (nLineBreaks == 15)
		{
			break;
		}

		pLogWalker++;
	}

	pLogWalker++;

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
	pos = 0;

	for (int i = 0; i < 16; i++) // Get the next 16 data items
	{
		pos = strChunk.Find(_T("0x"), pos);

		if (pos == -1) // less than 16 available
		{
			// ::MessageBox(NULL, _T("GetSuccessorLine cannot find 0x"), _T("Error"), MB_OK | MB_ICONERROR);
			break;
		}

		CString strIntermediate = strChunk.Mid(pos + 2, 3); // Get in 0xXXX format (speciality of MSB_RS485_PLUS)
		pos++;

		if (strIntermediate[0] != _T('0'))
		{
			CString strMsg = _T("");
			strMsg.Format(_T("Found unexpected datum %s"), strIntermediate.GetString());
			::MessageBox(NULL, strMsg, _T("Error"), MB_OK | MB_ICONERROR);
		}
		
		strResult += strIntermediate.Mid(1, 2);
		
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
		CString strSymbol;
		CONTROLCHARACTERS controlCharacter;
	};
	
	static IdentifyDecoderItem identifyDecoderItems[NUMDECODERITEMS];

	if (!bIAmInitialized)
	{
		for (int i = 0; i < NUMDECODERITEMS; i++)
		{
			identifyDecoderItems[i].strSymbol = _T("");
			identifyDecoderItems[i].controlCharacter = CONTROLCHARACTERS::NOTACONTROLCHARACTER;
		}

		identifyDecoderItems[0].strSymbol = _T("NUL"); // ASCII 00
		identifyDecoderItems[0].controlCharacter = CONTROLCHARACTERS::NUL;

		identifyDecoderItems[1].strSymbol = _T("SOH"); // ASCII 01
		identifyDecoderItems[1].controlCharacter = CONTROLCHARACTERS::SOH;

		identifyDecoderItems[2].strSymbol = _T("STX"); // ASCII 02
		identifyDecoderItems[2].controlCharacter = CONTROLCHARACTERS::STX;

		identifyDecoderItems[3].strSymbol = _T("ETX"); // ASCII 03
		identifyDecoderItems[3].controlCharacter = CONTROLCHARACTERS::ETX;

		identifyDecoderItems[4].strSymbol = _T("EOT"); // ASCII 04
		identifyDecoderItems[4].controlCharacter = CONTROLCHARACTERS::EOT;

		identifyDecoderItems[5].strSymbol = _T("ENQ"); // ASCII 05
		identifyDecoderItems[5].controlCharacter = CONTROLCHARACTERS::ENQ;

		identifyDecoderItems[6].strSymbol = _T("ACK"); // ASCII 06
		identifyDecoderItems[6].controlCharacter = CONTROLCHARACTERS::ACK;

		identifyDecoderItems[7].strSymbol = _T("BEL"); // ASCII 07
		identifyDecoderItems[7].controlCharacter = CONTROLCHARACTERS::BEL;

		identifyDecoderItems[8].strSymbol = _T("BS"); // ASCII 08
		identifyDecoderItems[8].controlCharacter = CONTROLCHARACTERS::BS;

		identifyDecoderItems[9].strSymbol = _T("TAB"); // ASCII 09 -- Sometimes called HT, Serial Sniffer writes TAB
		identifyDecoderItems[9].controlCharacter = CONTROLCHARACTERS::TAB;

		identifyDecoderItems[10].strSymbol = _T("LF"); // ASCII 10
		identifyDecoderItems[10].controlCharacter = CONTROLCHARACTERS::LF;

		identifyDecoderItems[11].strSymbol = _T("VT"); // ASCII 11
		identifyDecoderItems[11].controlCharacter = CONTROLCHARACTERS::VT;

		identifyDecoderItems[12].strSymbol = _T("FF"); // ASCII 12
		identifyDecoderItems[12].controlCharacter = CONTROLCHARACTERS::FF;

		identifyDecoderItems[13].strSymbol = _T("CR"); // ASCII 13
		identifyDecoderItems[13].controlCharacter = CONTROLCHARACTERS::CR;

		identifyDecoderItems[14].strSymbol = _T("SO"); // ASCII 14
		identifyDecoderItems[14].controlCharacter = CONTROLCHARACTERS::SO;

		identifyDecoderItems[15].strSymbol = _T("SI"); // ASCII 15
		identifyDecoderItems[15].controlCharacter = CONTROLCHARACTERS::SI;

		identifyDecoderItems[16].strSymbol = _T("DLE"); // ASCII 16
		identifyDecoderItems[16].controlCharacter = CONTROLCHARACTERS::DLE;

		identifyDecoderItems[17].strSymbol = _T("DC1"); // ASCII 17
		identifyDecoderItems[17].controlCharacter = CONTROLCHARACTERS::DC1;
		
		identifyDecoderItems[18].strSymbol = _T("DC2"); // ASCII 18
		identifyDecoderItems[18].controlCharacter = CONTROLCHARACTERS::DC2;

		identifyDecoderItems[19].strSymbol = _T("DC3"); // ASCII 19 -- just in case, never seen in practice!
		identifyDecoderItems[19].controlCharacter = CONTROLCHARACTERS::DC3;

		identifyDecoderItems[20].strSymbol = _T("DC4"); // ASCII 20
		identifyDecoderItems[20].controlCharacter = CONTROLCHARACTERS::DC4;

		identifyDecoderItems[21].strSymbol = _T("NAK"); // ASCII 21
		identifyDecoderItems[21].controlCharacter = CONTROLCHARACTERS::NAK;

		identifyDecoderItems[22].strSymbol = _T("SYN"); // ASCII 22
		identifyDecoderItems[22].controlCharacter = CONTROLCHARACTERS::SYN;

		identifyDecoderItems[23].strSymbol = _T("ETB"); // ASCII 23
		identifyDecoderItems[23].controlCharacter = CONTROLCHARACTERS::ETB;

		identifyDecoderItems[24].strSymbol = _T("CAN"); // ASCII 24
		identifyDecoderItems[24].controlCharacter = CONTROLCHARACTERS::CAN;

		identifyDecoderItems[25].strSymbol = _T("EM"); // ASCII 25
		identifyDecoderItems[25].controlCharacter = CONTROLCHARACTERS::EM;

		identifyDecoderItems[26].strSymbol = _T("SUB"); // ASCII 26
		identifyDecoderItems[26].controlCharacter = CONTROLCHARACTERS::SUB;

		identifyDecoderItems[27].strSymbol = _T("ESC"); // ASCII 27
		identifyDecoderItems[27].controlCharacter = CONTROLCHARACTERS::ESC;

		identifyDecoderItems[28].strSymbol = _T("FS"); // ASCII 28
		identifyDecoderItems[28].controlCharacter = CONTROLCHARACTERS::FS;

		identifyDecoderItems[29].strSymbol = _T("GS"); // ASCII 29
		identifyDecoderItems[29].controlCharacter = CONTROLCHARACTERS::GS;

		identifyDecoderItems[30].strSymbol = _T("RS"); // ASCII 30
		identifyDecoderItems[30].controlCharacter = CONTROLCHARACTERS::RS;

		identifyDecoderItems[31].strSymbol = _T("US"); // ASCII 31
		identifyDecoderItems[31].controlCharacter = CONTROLCHARACTERS::US;

		identifyDecoderItems[32].strSymbol = _T("7F"); // ASCII 127 -- Sometimes called DEL, Serial Sniffer writes 7F
		identifyDecoderItems[32].controlCharacter = CONTROLCHARACTERS::_7F;

		bIAmInitialized = true;
	}
	
	for (int i = 0; i < NUMDECODERITEMS; i++)
	{
		if (identifyDecoderItems[i].strSymbol.IsEmpty())
		{
			break;
		}
		
		if (strToken == identifyDecoderItems[i].strSymbol)
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
	
	// Was (in a rawer form):
	// strParsed.Format(_T(" (%s ENQ)==EnquireSlave_%s"), Predecessor(i, false), Predecessor(i, true));

	CString strSlaveClearname = _T("UNKNOWN");
	CString strRole = _T("UNKNOWN");
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (iCookedSlaveID == KnownSlaveAddresses[i].iUnmaskedNumber)
		{
			strSlaveClearname = KnownSlaveAddresses[i].strName;
			strRole = KnownSlaveAddresses[i].strRole;
			// xyz TODO: Hier die "Transaktionen" einfügen!!!

			break;
		}
	}

	CString strCleartext = strSlaveClearname;

	if (strRole == _T("EADR"))
	{
		strCleartext += _T(", EMPFANGE!\"");
	}
	else
	{
		strCleartext += _T(", SENDE!\"");
	}

	strResult.Format(_T(" (>> %s ENQ)==EnquireSlave_%d_%s(%s)"), strHexSlaveID, iCookedSlaveID, strRole, strCleartext);

	return strResult;
}

CString CMACBusVersteherView::DecodePositiveRequestFeedback(int tokenSlot)
{
	CString strResult = _T("");
	CString strHexSlaveID = Predecessor(tokenSlot, false);
	CString strIntSlaveID = Predecessor(tokenSlot, true);
	int iRawSlaveID = _ttoi(strIntSlaveID); // The format "on the wire" masked with 0100 0000
	short int iRemovalMask = 0x00000040; // binary 0000 0000 0100 0000
	int iCookedSlaveID = iRawSlaveID & ~iRemovalMask; // The format used inside the embedded software

	// Was (in a rawer form):
	// strParsed.Format(_T(" (%s DLE %s)==Positive_Request_Feedback_From_%s"), Predecessor(i, false), Successor(i, false), Predecessor(i, true));

	CString strSlaveClearname = _T("UNKNOWN");
	CString strRole = _T("UNKNOWN");
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (iCookedSlaveID == KnownSlaveAddresses[i].iUnmaskedNumber)
		{
			strSlaveClearname = KnownSlaveAddresses[i].strName;
			strRole = KnownSlaveAddresses[i].strRole;
			break;
		}
	}
	
	CString strCleartext = strSlaveClearname;

	if (strRole == _T("EADR"))
	{
		strCleartext += _T(", ICH EMPFANGE!\"");
	}
	else
	{
		strCleartext += _T(", ICH SENDE!\"");
	}

	strResult.Format(_T(" (<< %s DLE %s)==Positive_Request_Feedback_From_%s_%d(%s)"), strHexSlaveID, Successor(tokenSlot, false), strRole, iCookedSlaveID, strCleartext);

	return strResult;
}

CString CMACBusVersteherView::DecodeBlock(const char* pLog, const char* pLogWalker)
{
	CString strResult = _T("\nAnalysis of the block that follows:\n");
	const char* pWork = pLogWalker;

	// Backtrack to the begin of a block (first data item after STX)

	while (pWork-2 >= pLog && *pWork != _T('X'))
	{
		pWork--;
	}

	if (*(pWork - 1) == _T('T') && *(pWork - 2) == _T('S')) // Found last STX
	{
		pWork++; // To blank after X
		pWork++; // To start of first nibble of first byte
	}
	else
	{
		strResult += _T("\nFatal error: Cannot find STX");
	}
	
	int iNDataBytesInTelegram = CountBytesInTelegram(pWork);
	CString strCount = _T("");
	strCount.Format(_T("Telegram consists of %d bytes\n"), iNDataBytesInTelegram);
	strResult += strCount;
	
	
	/*char acByte[3];

	ZeroMemory(acByte, 3);
*/
	// First byte is address. Nö, kann nicht sein!
	//acByte[0] = *pWork;
	//pWork++;
	//acByte[1] = *pWork;

	//CString strAddress(acByte); // Intestingly, the MAC adress is encoded in BCD
	//CString strAddressLine = _T("\nAddress if BCD == ") + strAddress +  _T("\n");
	//strResult += strAddressLine;

	//int iAddress = HexString2Int(acByte);
	//strAddressLine.Format(_T("\nAddress if DEC == %d\n"), iAddress);
	//strResult += strAddressLine;

	//short int iRemovalMask = 0x00000040; // binary 0000 0000 0100 0000
	//int iAddress2 = iAddress & ~iRemovalMask;
	//strAddressLine.Format(_T("\nAddress if REM == %d\n"), iAddress2);
	//strResult += strAddressLine;

	pWork++;
	pWork++;

	// Second and third bytes are block ID
	
	
	
	//WORD wBlockID 


	return strResult;
}

CString CMACBusVersteherView::Alias4ControlCharacters(CString strToken)
{
	CString strResult = _T("");
	
	if (strToken == _T("NUL"))
	{
		strResult = _T("0"); // unneeded
	}

	if (strToken == _T("SOH"))
	{
		strResult = _T("1");
	}

	if (strToken == _T("STX"))
	{
		strResult = _T("2");
	}

	if (strToken == _T("ETX"))
	{
		strResult = _T("3");
	}

	if (strToken == _T("EOT"))
	{
		strResult = _T("4");
	}

	if (strToken == _T("ENQ"))
	{
		strResult = _T("5");
	}

	if (strToken == _T("ACK"))
	{
		strResult = _T("6");
	}

	if (strToken == _T("BEL"))
	{
		strResult = _T("7");
	}

	if (strToken == _T("BS"))
	{
		strResult = _T("8"); // unneeded
	}

	if (strToken == _T("TAB"))
	{
		strResult = _T("9"); // unneeded
	}

	if (strResult.IsEmpty())
	{
		strResult = strToken;
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

bool CMACBusVersteherView::LineContainsHandshake(CString strReportLine, COLORREF& cr)
{
	if (-1 != strReportLine.Find("VERSTANDEN!")) // This is a DLE 31 positive transmission response
	{
		cr = RGB(0, 0, 128);
		return true; 
	}
	
	bool bFound = false;
	
	for (int i = 0; i < NUM_KNOWN_SLAVE_ADDRESSES; i++)
	{
		if (-1 != strReportLine.Find(KnownSlaveAddresses[i].strName))
		{
			bFound = true;
			cr = KnownSlaveAddresses[i].colorref;
			break;
		}
	}

	return bFound;
}

// Serial Sniffer version
//CString CMACBusVersteherView::GetTimestamp(CString strLogLine)
//{
//	CString strToken = _T("");
//	int curPos = 0;
//
//	strToken = strLogLine.Tokenize(_T(" "), curPos);
//
//	while (strToken != _T(""))
//	{
//		if (strToken[2] == _T(':') && strToken[5] == _T(':') && strToken[8] == _T(':')) // This is a timestamp
//		{
//			break;
//		}
//
//		strToken = strLogLine.Tokenize(_T(" "), curPos);
//	}
//
//	return strToken;
//}

CString CMACBusVersteherView::GetTimestamp(CString strLogLine)
{
	int curPos = 0;
	// Always have Datenmonitor write the timestamps to the first column!
	CString strToken = strLogLine.Tokenize(_T(","), curPos);
	int loc = strToken.Find(_T('s'));
	strToken.Delete(loc, 1); // Removal of trailing 's'

	return strToken;
}

// SerialSniffer version
//CString CMACBusVersteherView::GetMSElapsed(CString strLastTimestamp, CString strCurrentTimestamp, bool bReport)
//{
//	CString strMSElapsed = _T("");
//	
//	CString strHours = _T("");
//	CString strMinutes = _T("");
//	CString strSeconds = _T("");
//	CString strMilliseconds = _T("");
//	DWORD dwHours = 0;
//	DWORD dwMinutes = 0;
//	DWORD dwSeconds = 0;
//	DWORD dwMilliseconds = 0;
//	DWORD dwStartInMs = 0;
//	DWORD dwEndInMs = 0;
//
//	static DWORD dwMinDelay = ULONG_MAX;
//	static DWORD dwMaxDelay = 0;
//
//	int curPos = 0;
//	int nTokensSeen = 0;
//
//	if (!bReport) // Normal call
//	{
//		if (strLastTimestamp.IsEmpty())
//		{
//			strMSElapsed = _T("\t[UNDEFINED]");
//		}
//		else
//		{
//			CString strToken = strLastTimestamp.Tokenize(_T(":"), curPos);
//			nTokensSeen++;
//
//			while (strToken != _T(""))
//			{
//				if (nTokensSeen == 1)
//				{
//					strHours = strToken;
//				}
//				else if (nTokensSeen == 2)
//				{
//					strMinutes = strToken;
//				}
//				else if (nTokensSeen == 3)
//				{
//					strSeconds = strToken;
//				}
//				else if (nTokensSeen == 4)
//				{
//					strMilliseconds = strToken;
//				}
//
//				strToken = strLastTimestamp.Tokenize(_T(":"), curPos);
//				nTokensSeen++;
//			}
//
//			dwHours = strtoul(strHours, NULL, 10);
//			dwMinutes = strtoul(strMinutes, NULL, 10);
//			dwSeconds = strtoul(strSeconds, NULL, 10);
//			dwMilliseconds = strtoul(strMilliseconds, NULL, 10);
//
//			dwStartInMs = (3600000 * dwHours) + (60000 * dwMinutes) + (1000 * dwSeconds) + dwMilliseconds;
//
//			curPos = 0;
//			nTokensSeen = 0;
//
//			strToken = strCurrentTimestamp.Tokenize(_T(":"), curPos);
//			nTokensSeen++;
//
//			while (strToken != _T(""))
//			{
//				if (nTokensSeen == 1)
//				{
//					strHours = strToken;
//				}
//				else if (nTokensSeen == 2)
//				{
//					strMinutes = strToken;
//				}
//				else if (nTokensSeen == 3)
//				{
//					strSeconds = strToken;
//				}
//				else if (nTokensSeen == 4)
//				{
//					strMilliseconds = strToken;
//				}
//
//				strToken = strCurrentTimestamp.Tokenize(_T(":"), curPos);
//				nTokensSeen++;
//			}
//
//			dwHours = strtoul(strHours, NULL, 10);
//			dwMinutes = strtoul(strMinutes, NULL, 10);
//			dwSeconds = strtoul(strSeconds, NULL, 10);
//			dwMilliseconds = strtoul(strMilliseconds, NULL, 10);
//
//			dwEndInMs = (3600000 * dwHours) + (60000 * dwMinutes) + (1000 * dwSeconds) + dwMilliseconds;
//
//			DWORD dwMSElapsed = dwEndInMs - dwStartInMs;
//			strMSElapsed.Format(_T("\t[%lu msec]"), dwMSElapsed);
//
//			if (dwMSElapsed < dwMinDelay)
//			{
//				dwMinDelay = dwMSElapsed;
//			}
//
//			if (dwMSElapsed > dwMaxDelay)
//			{
//				dwMaxDelay = dwMSElapsed;
//			}
//		}
//	}
//	else // Report call
//	{
//		strMSElapsed.Format(_T("Min delay between log lines: %lu msec\nMaxdelay between log lines: %lu msec"), dwMinDelay, dwMaxDelay);
//	}
//
//	return strMSElapsed;
//}

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
		strMSElapsed.Format(_T("Min delay between log lines: %lf msec\nMaxdelay between log lines: %lf msec"), dMinDelay, dMaxDelay);
	}

	return strMSElapsed;
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

