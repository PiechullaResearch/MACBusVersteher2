
// MACBusVersteherDoc.h : interface of the CMACBusVersteherDoc class
//


#pragma once


class CMACBusVersteherDoc : public CRichEditDoc
{
protected: // create from serialization only
	CMACBusVersteherDoc();
	DECLARE_DYNCREATE(CMACBusVersteherDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;
	virtual BOOL OnOpenDocument(LPCTSTR lpszFullFilename);
	virtual void OnFileSave();

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMACBusVersteherDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	char* m_RawLogfileBuffer;
};
