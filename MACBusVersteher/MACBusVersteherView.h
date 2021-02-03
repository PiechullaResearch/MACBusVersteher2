
// MACBusVersteherView.h : interface of the CMACBusVersteherView class
//

#pragma once

class CMACBusVersteherCntrItem; // Belongs to the wizard-generated stuff

class CMACBusVersteherView : public CRichEditView
{
	enum class ACTIVE_SPEAKER
	{
		UNDEFINED,
		COMPORT_A,
		COMPORT_B
	};
	
	enum class CONTROLCHARACTERS
	{
		NOTACONTROLCHARACTER,
		NUL, // ASCII 00 Null
		SOH, // ASCII 01 Start_of_Header
		STX, // ASCII 02 Start_of_Text
		ETX, // ASCII 03 End_of_Text
		EOT, // ASCII 04 End_of_transmission
		ENQ, // ASCII 05 Enquiry
		ACK, // ASCII 06 Acknowledgement
		BEL, // ASCII 07 Bell
		BS,  // ASCII 08 Backspace
		TAB, // ASCII 09 -- Sometimes called HT, Serial Sniffer writes TAB
		LF,  // ASCII 10 Line_Feed
		VT,  // ASCII 11 Vertical_Tab
		FF,  // ASCII 12 Form_Feed
		CR,  // ASCII 13 Carriage_Return
		SO,  // ASCII 14 Shift_Out
		SI,  // ASCII 15 Shift_In
		DLE, // ASCII 16 Data_link_escape
		DC1, // ASCII 17 Device_Cotrol_1
		DC2, // ASCII 18 Device_Cotrol_2
		DC3, // ASCII 19 -- just in case, never seen in practice!
		DC4, // ASCII 20 Device_Cotrol_4
		NAK, // ASCII 21 Negative_Acknowledgement
		SYN, // ASCII 22 Synchronous_Idle
		ETB, // ASCII 23 End_of_Trans_Block
		CAN, // ASCII 24 Cancel
		EM,  // ASCII 25 End_of_Medium
		SUB, // ASCII 26 Substitute
		ESC, // ASCII 27 Escape
		FS,  // ASCII 28 File_Separator
		GS,  // ASCII 29 Group_Separator
		RS,  // ASCII 30 Record_Separator
		US,  // ASCII 31 Unit_Separator
		_7F  // ASCII 127 -- Sometimes called DEL, Serial Sniffer writes 7F
	};

	struct DecoderItem
	{
		CString strSymbol;
		CString strClearname;
		DWORD dwCount;
	};

	struct TransactionItem
	{
		double dTransactionStartTime;
		double dTransactionEndTime;
	};

	struct InterimBufferItem
	{
		CString strLine;
		COLORREF colorref;
	};

	CList<InterimBufferItem, InterimBufferItem> m_InterimBuffer;

	CList<int, int> m_handshakeLocations;

	struct HandshakeItem
	{
		CString strHandshake;
		COLORREF colorref;
	};

	CList<HandshakeItem, HandshakeItem> m_Handshakes;

	CList<CString, CString> m_AnalyzedBlocks;
	
	static const int NUM_KNOWN_SLAVE_ADDRESSES = 18; // 9 EADR and 9 SADR
	
	struct KnownSlave
	{
		CString strName;
		int iUnmaskedNumber; // Before put to the DINBus buffer, the master does iUnmaskedNumber |= 0x00000040 
		CString strRole;
		COLORREF colorref;
		CList<TransactionItem, TransactionItem> transactions;
		int nOpenTransactions;
		DWORD dwTransactions;
	};

	KnownSlave m_KnownSlaves[NUM_KNOWN_SLAVE_ADDRESSES]; // Filled with meaning by constructor

	struct DataToken
	{
		CString strTimestamp;
		CString strValue;
	};

	enum class SEND_DIRECTION
	{
		UNDEFINED,
		MASTER_TO_SLAVE,
		SLAVE_TO_MASTER
	};

	enum SLAVE
	{
		UNKNOWN,
		GDS_WLS_1,
		GDS_WLS_2,
		GDS_WLS_3,
		GDS_WLS_4,
		GDS_WLS_5,
		GDS_WLS_6,
		GDS_WLS_7,
		LMS,
		PCS
	};
	
	struct BlockTraceItem
	{
		BYTE ID;
		SEND_DIRECTION send_direction;
		double dTimeSent;
		SLAVE activeSlave;
	};

	CList<BlockTraceItem, BlockTraceItem> m_BlockExchangeTrace;

	struct RoleDescription
	{
		BYTE ID;
		SEND_DIRECTION send_direction;
		CString strDescription;
	};

	CList<RoleDescription, RoleDescription> m_RoleDescriptions;

protected: // create from serialization only
	CMACBusVersteherView();
	DECLARE_DYNCREATE(CMACBusVersteherView)

// Attributes
public:
	CMACBusVersteherDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMACBusVersteherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

// Core functionality	
	void Parse(const char* pLog);
	int AppendToLogAndScroll(CRichEditCtrl& ctrlLog, CString str, COLORREF color);
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	CString Analysis(CString strOneLine);
	CString SemanticAnalysis(CString strPredecessorLine,
		CString strLine,
		CString strSuccessorLine,
		const char* pLog,
		const char* pLogWalker,
		CString strCurrentTimestamp);
	CString Predecessor(int slot, bool toInt);
	CString Successor(int slot, bool toInt);
	CString AnalizeToken(CString strToken, bool bInit, bool bGetCounts);
	CString ProcessNumericToken(CString strToken);
	int GetNumberOfLeadingColumns(char* pLog);
	CString GetVirtualPredecessorLine(char* pLogWalker);
	CString GetVirtualThisLine(char* pLogWalker);
	CString GetVirtualSuccessorLine(char* pLogWalker);
	CONTROLCHARACTERS IdentifyToken(CString strToken);
	CString DecodeSlaveEnquiry(int tokenSlot, CString strCurrentTimestamp);
	CString DecodePositiveRequestFeedback(int tokenSlot, CString strCurrentTimestamp);
	CString DecodeBlock(const char* pLog, const char* pLogWalker, const int logWalkerBacktracking, SLAVE activeSlave);
	// CString Alias4ControlCharacters(CString strToken); Serial Sniffer
	int CountBytesInTelegram(const char* pWork);
	bool LineContainsHandshake(CString strReportLine, COLORREF& cr, char* pLogWalker);
	CString GetTimestamp(CString strLogLine);
	CString GetMSElapsed(CString strLastTimestamp, CString strCurrentTimestamp, bool bReport);
	CString GetTimestampFromItem(CString oneItem);
	CString GetValueFromItem(CString oneItem);
	void TransactionStatistics(CRichEditCtrl& crec);
	void BlockExchangeStatistics(CRichEditCtrl& crec);
	void TransactionSequence(CRichEditCtrl& crec);
	CString GetMinDelay(int iSlavePos);
	CString GetMaxDelay(int iSlavePos);
	CString GetMeanDelay(int iSlavePos);
	void AppendToInterimBuffer(CString strOneLine4Output, COLORREF colorref);
	COLORREF GetSlaveColor(CString strContainsSlaveName);
	BYTE DecodeStringPair(CString strHigh, CString strLow, CString& strErrorSummary);
	BYTE ASCIICharFromString(CString strHexNibbleTableEntry);
	bool IsValidNibble(BYTE bNibble);
	SLAVE WhoAnswered(CString strToExamine);
	CString SlaveEnumToCString(SLAVE slave);

protected:
	DataToken m_aStrPredecessorTokens[16]; // 16 data tokens
	DataToken m_aStrTokens[16]; // 16 data tokens
	DataToken m_aStrSuccessorTokens[16]; // 16 data tokens
	bool m_bInsideText;
	bool m_bLastCharWasETX;
	const CString m_strSpecialMsgDecorator = _T("!!!!! ");
	const CString m_strErrorHint = _T("Look for !!!!! in the report");
	SLAVE m_activeSlave = SLAVE::UNKNOWN;
	int m_nErrorsFound;
};

#ifndef _DEBUG  // debug version in MACBusVersteherView.cpp
inline CMACBusVersteherDoc* CMACBusVersteherView::GetDocument() const
   { return reinterpret_cast<CMACBusVersteherDoc*>(m_pDocument); }
#endif

