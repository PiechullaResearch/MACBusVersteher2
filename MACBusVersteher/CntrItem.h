
// CntrItem.h : interface of the CMACBusVersteherCntrItem class
//

#pragma once

class CMACBusVersteherDoc;
class CMACBusVersteherView;

class CMACBusVersteherCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CMACBusVersteherCntrItem)

// Constructors
public:
	CMACBusVersteherCntrItem(REOBJECT* preo = NULL, CMACBusVersteherDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CMACBusVersteherDoc* GetDocument()
		{ return reinterpret_cast<CMACBusVersteherDoc*>(CRichEditCntrItem::GetDocument()); }
	CMACBusVersteherView* GetActiveView()
		{ return reinterpret_cast<CMACBusVersteherView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CMACBusVersteherCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

