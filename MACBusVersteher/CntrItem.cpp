
// CntrItem.cpp : implementation of the CMACBusVersteherCntrItem class
//

#include "stdafx.h"
#include "MACBusVersteher.h"

#include "MACBusVersteherDoc.h"
#include "MACBusVersteherView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMACBusVersteherCntrItem implementation

IMPLEMENT_SERIAL(CMACBusVersteherCntrItem, CRichEditCntrItem, 0)

CMACBusVersteherCntrItem::CMACBusVersteherCntrItem(REOBJECT* preo, CMACBusVersteherDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CMACBusVersteherCntrItem::~CMACBusVersteherCntrItem()
{
	// TODO: add cleanup code here
}


// CMACBusVersteherCntrItem diagnostics

#ifdef _DEBUG
void CMACBusVersteherCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CMACBusVersteherCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

