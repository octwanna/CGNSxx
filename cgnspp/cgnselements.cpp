/***************************************************************************
                          cgnselements.cpp  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnselements.h"

#include "cgnszone.h"
#include "cgnsdataarray.h"

#include <iostream>
#include <assert.h>

using CGNS::Elements;

GENERATE_D(Elements);

COMMON_SIMPLE_NODE_IMPLEMENTATION(Elements);

INIT_IMPLEMENTATION3(Elements, mElementRange, mElementConnectivity, mParentData);

void CGNS::Private::Elements::Read(Elements * me) {
	checkNode(me->mNode, ADF::I4, 2);
	checkRequired(me->mNode, "ElementRange", "Elements");
	checkRequired(me->mNode, "ElementConnectivity", "Elements");
	
	int data[2];
	me->mNode.readData(data);
	// plausibility check
	assertIndex(data[0], CGNS::Elements::NULL_ELEMENT,
		CGNS::Elements::NGON_n, Exception::ILLEGAL_DATA);
	assertIndex(data[1], 0, Exception::ILLEGAL_DATA);
	me->mElementType=static_cast<CGNS::Elements::ElementType>(data[0]);
	me->mElementSizeBoundary=data[1];
	
	me->mInitialized|=INIT_MASK_SELF;
}

static void convertElementConnectivity(CGNS::refcnt_ptr<CGNS::Private::DataArray> iEC) {
}

// read always both IndexArray subnodes, due to identical INIT_MASK
void CGNS::Private::Elements::Read(Elements * me, refcnt_ptr<DataArray> & cDest) {
	if (&cDest!=&me->mElementConnectivity && &cDest!=&me->mParentData)
		Node::Read(me, cDest);
	readChild(me->mNode, "ElementConnectivity", me->mElementConnectivity, me);
	convertElementConnectivity(me->mElementConnectivity);
	readChild(me->mNode, "ParentData", me->mParentData, me);
	me->mInitialized|=implementation_traits<DataArray>::INIT_MASK;
}

void CGNS::Private::Elements::Read(Elements * me, std::auto_ptr<IndexRange1D> & cDest) {
	if (&cDest!=&me->mElementRange) EMIT_INTERNAL_ERROR();
	readChild(me->mNode, "ElementRange", "IndexRange_t", me->mElementRange);
	me->mInitialized|=implementation_traits<IndexRange1D>::INIT_MASK;
}

void CGNS::Private::Elements::Modify(CGNS::Elements::ElementType iType, int iNodesOnBoundary) {
	validateIndex(iType, CGNS::Elements::NULL_ELEMENT, CGNS::Elements::USER_ELEMENT);
	if (iNodesOnBoundary<0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	mNode.setDataTypeDimensions(ADF::I4, 2);
	int data[2] = { iType, iNodesOnBoundary };
	mNode.writeData(data);
	mElementType=iType;
	mElementSizeBoundary=iNodesOnBoundary;	
	assureChild(mElementConnectivity, "ElementConnectivity", this);
	mElementConnectivity->Modify();
}

Elements::ElementType Elements::getElementType() const {
	NEEDS_THIS();
	return D()->mElementType;
}
	
int Elements::getNodesOnBoundary() const {
	NEEDS_THIS();
	return D()->mElementSizeBoundary;
}

CGNS::IndexRange1D Elements::getElementRange() const {
	NEEDS(mElementRange);
	return *D()->mElementRange.get();
}

CGNS::IndexRange1D Elements::writeElementRange(CGNS::IndexRange1D iRange) {
	NEEDS(mElementRange);
	Private::assertIndex(iRange.mMaxI, iRange.mMinI, Exception::ILLEGAL_RANGE);
	ADF::Node n(Private::assureChild(D()->mElementRange, D(), iRange,
		"ElementRange", "IndexRange_t"));
	n.setDataTypeDimensions(ADF::I4, 2);
	int data[2] = { iRange.mMinI, iRange.mMaxI };
	n.writeData(data);
	return iRange;	
}

CHILD_IMPLEMENTATION(ElementConnectivity, DataArray, Elements, D()->mElementConnectivity);
	
OPTIONAL_CHILD_IMPLEMENTATION(ParentData, DataArray, Elements, D()->mParentData);

CGNS::DataArray Elements::writeParentData() {
	NEEDS(mParentData);
	Private::assureChild(D()->mParentData, "ParentData", D());
//std::cerr << "Setting data type and dimensions of ParentData\n";
	D()->mParentData->mNode.setDataType(ADF::I4);
	return D()->mParentData.get();
}

