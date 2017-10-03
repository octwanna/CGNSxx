/***************************************************************************
                          cgnsbc.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsbc.h"

#include "cgnsbcdataset.h"
#include "cgnsindexarray.h"
#include "cgnsnodeutil.h"
#include "cgnsdataarray.h"

#include <assert.h>

using CGNS::BC;

GENERATE_D(BC);

COMMON_NODE_IMPLEMENTATION(BC);

INIT_IMPLEMENTATION6(BC, mPointRange, mPointList, mFamilyName,
	mInwardNormalIndex, mNormalList, mDataSets);

N_CHILDS_IMPLEMENTATION_WRITE1(BCDataSet, BCDataSet, BC, D()->mDataSets, BC::BCType);

void CGNS::Private::BC::Read(BC * me) {
	me->mType=CGNS::BC::USER_BC;
	readADF(me->mNode, me->mType);
	me->mLocation=VERTEX;
	readStringChild(me->mNode, me->mLocation);
	
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::BC::Read(BC * me, std::auto_ptr<std::string> & cDest) {
	assert(&cDest==&me->mFamilyName);
	readFamilyName(me, cDest);	
}

void CGNS::Private::BC::Read(BC * me, std::auto_ptr<IndexRange> & cDest) {
	assert(&cDest==&me->mPointRange);
	readIndexRange("PointRange", me, cDest);
}

void CGNS::Private::BC::Read(BC * me, std::auto_ptr<Normal> & cDest) {
	assert(&cDest==&me->mInwardNormalIndex);
	readIndex("InwardNormalIndex", me, me->getZone(), cDest);
}

void CGNS::Private::BC::Read(BC * me, refcnt_ptr<IndexArray> & cDest) {
	assert(&cDest==&me->mNormalList || &cDest==&me->mPointList);
	readChild(me->mNode, "NormalList", me->mNormalList, me);
	readChild(me->mNode, "PointList", me->mPointList, me);
	me->mInitialized|=implementation_traits<IndexArray>::INIT_MASK;
}

void CGNS::Private::BC::Modify(CGNS::BC::BCType iType) {
	validateIndex(iType, CGNS::BC::NULL_BC, CGNS::BC::USER_BC);
	writeADF(mNode, mType=iType);
}

SIMPLE_CHILD_IMPLEMENTATION_WRITE(Location, CGNS::GridLocation, BC, D()->mLocation);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(PointRange, CGNS::IndexRange,
	BC, D()->mPointRange);

DO_RANGE_ALL(BC, PointRange)

OPTIONAL_CHILD_IMPLEMENTATION(PointList, IndexArray,
	BC, D()->mPointList);

CHILD_IMPLEMENTATION_WRITE_INDEX(PointList, IndexArray,
	BC, D()->mPointList);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(FamilyName, std::string, BC, D()->mFamilyName);

std::string BC::writeFamilyName(std::string const & iFamily) {
	NEEDS(mFamilyName);
	Private::assureChild(D()->mFamilyName, D(), iFamily,
		"FamilyName", "FamilyName_t").writeData(iFamily);
	return iFamily;
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(InwardNormalIndex, CGNS::Normal, BC, D()->mInwardNormalIndex);

DO_INDICES_ALL(InwardNormalIndex, CGNS::Normal, BC);

OPTIONAL_CHILD_IMPLEMENTATION(InwardNormalList, IndexArray,
	BC, D()->mNormalList);

CHILD_IMPLEMENTATION_WRITE_INDEX(InwardNormalList, IndexArray,
	BC, D()->mNormalList);

BC::BCType BC::getBCType() const {
	NEEDS_THIS();
	return D()->mType;
}

