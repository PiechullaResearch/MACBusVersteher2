
// MACBusVersteherDoc.cpp : implementation of the CMACBusVersteherDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MACBusVersteher.h"
#endif

#include "MACBusVersteherDoc.h"
#include "CntrItem.h"

#include <propkey.h>
#include <intsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMACBusVersteherDoc

IMPLEMENT_DYNCREATE(CMACBusVersteherDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CMACBusVersteherDoc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &CMACBusVersteherDoc::OnUpdateObjectVerbPopup)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_FILE_SAVE, &CMACBusVersteherDoc::OnFileSave)

END_MESSAGE_MAP()


// CMACBusVersteherDoc construction/destruction

CMACBusVersteherDoc::CMACBusVersteherDoc()
{
	// TODO: add one-time construction code here
	m_RawLogfileBuffer = NULL;
}

CMACBusVersteherDoc::~CMACBusVersteherDoc()
{
}

BOOL CMACBusVersteherDoc::OnNewDocument()
{
	// if (!CRichEditDoc::OnNewDocument())
	//	return FALSE;

	// WP20200610: Cannot do this because OnNewDocument() is called by the framework on startup
	// ::MessageBox(NULL, _T("This appliction does not create new documents"), _T("Information"), MB_OK | MB_ICONINFORMATION);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CMACBusVersteherDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CMACBusVersteherCntrItem(preo, const_cast<CMACBusVersteherDoc*>(this));
}

// Override OnOpenDocument to avoid that the RichText stuff tries to load a rich text file, we read and interpret a Serial Sniffer file!
BOOL CMACBusVersteherDoc::OnOpenDocument(LPCTSTR lpszFullFilename)
{
	if (m_RawLogfileBuffer)
	{
		delete[] m_RawLogfileBuffer;
		m_RawLogfileBuffer = NULL;
	}
	
	CFile* pFile = new CFile(lpszFullFilename, CFile::modeRead);
	ULONGLONG ullLength = pFile->GetLength();
	UINT uiLength = 0;
	HRESULT res = ULongLongToUInt(ullLength, &uiLength);

	if (res != S_OK)
	{
		::MessageBox(NULL, _T("Conversion from ULONGLONG to UINT did not return S_OK"), _T("Warning"), MB_OK | MB_ICONWARNING);
	}

	m_RawLogfileBuffer = new char[uiLength + 1]; // "Notice the +1" WP20200616: WTF??? +1 because we need the '\0' in the +1 slot?
	ZeroMemory(m_RawLogfileBuffer, uiLength + 1); // WP20200616: Simpler approach
	// m_RawLogfileBuffer[uiLength] = _T('\0'); // WP20200615: Why does this work?
	UINT nActual = pFile->Read(m_RawLogfileBuffer, uiLength);
	delete pFile;

	return TRUE;
}

void CMACBusVersteherDoc::OnFileSave()
{
	::MessageBox(NULL, _T("Use Save as... and give file extension .rtf"), _T("Howto save"), MB_OK | MB_ICONINFORMATION);
}


// CMACBusVersteherDoc serialization

void CMACBusVersteherDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text

	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMACBusVersteherDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMACBusVersteherDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMACBusVersteherDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMACBusVersteherDoc diagnostics

#ifdef _DEBUG
void CMACBusVersteherDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CMACBusVersteherDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CMACBusVersteherDoc commands
