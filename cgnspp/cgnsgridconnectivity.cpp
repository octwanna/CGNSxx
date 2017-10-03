/***************************************************************************
                          cgnsgridconnectivity.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsgridconnectivity.h"

#include "cgnsnodeutil.h"
#include "cgnsindexarray.h"
#include "cgnsdataarray.h"

#include <assert.h>

using CGNS::GridConnectivity;

GENERATE_D(GridConnectivity);

COMMON_NODE_IMPLEMENTATION(GridConnectivity);

INIT_IMPLEMENTATION5(GridConnectivity, mPointRange, mPointList,
	mPointListDonor, mCellListDonor, mInterpolants);

void CGNS::Private::GridConnectivity::Read(GridConnectivity * me) {
	readADF(me->mNode, me->mZoneDonorName);
	me->mLocation=VERTEX;
	readStringChild(me->mNode, me->mLocation);
	me->mType=CGNS::GridConnectivity::OVERSET;
	readStringChild(me->mNode, me->mType);
	// plausibility check?
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::GridConnectivity::Modify(std::string const & iZoneDonorName) {
	writeADF(mNode, iZoneDonorName);
	mZoneDonorName=iZoneDonorName;
}

void CGNS::Private::GridConnectivity::Modify(CGNS::Zone const & iZoneDonor) {
	writeADF(mNode, iZoneDonor.getName());
	mZoneDonorName=iZoneDonor.getName();
}

void CGNS::Private::GridConnectivity::Read(GridConnectivity * me,
	std::auto_ptr<IndexRange> & cDest) {
	assert(&cDest==&me->mPointRange);
	readIndexRange("PointRange", me, cDest);
}

void CGNS::Private::GridConnectivity::Read(GridConnectivity * me, refcnt_ptr<IndexArray> & cDest) {
	assert(&cDest==&me->mPointList || &cDest==&me->mPointListDonor || &cDest==&me->mCellListDonor);
	readChild(me->mNode, "PointList", me->mPointList, me);
	readChild(me->mNode, "PointListDonor", me->mPointListDonor, me);
	readChild(me->mNode, "CellListDonor", me->mCellListDonor, me);
	me->mInitialized|=implementation_traits<IndexArray>::INIT_MASK;
}

std::string CGNS::GridConnectivity::getZoneDonorName() const {
	NEEDS_THIS();
	return D()->mZoneDonorName;
}

SIMPLE_CHILD_IMPLEMENTATION_WRITE(Location, CGNS::GridLocation, GridConnectivity, D()->mLocation);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(PointRange, CGNS::IndexRange,
	GridConnectivity, D()->mPointRange);

DO_RANGE_ALL(GridConnectivity, PointRange)

OPTIONAL_CHILD_IMPLEMENTATION(PointList, IndexArray,
	GridConnectivity, D()->mPointList);

CHILD_IMPLEMENTATION_WRITE_INDEX(PointList, IndexArray,
	GridConnectivity, D()->mPointList);
	
OPTIONAL_CHILD_IMPLEMENTATION(CellListDonor, IndexArray,
	GridConnectivity, D()->mCellListDonor);

CHILD_IMPLEMENTATION_WRITE_INDEX(CellListDonor, IndexArray,
	GridConnectivity, D()->mCellListDonor);

OPTIONAL_CHILD_IMPLEMENTATION(PointListDonor, IndexArray,
	GridConnectivity, D()->mPointListDonor);

CHILD_IMPLEMENTATION_WRITE_INDEX(PointListDonor, IndexArray,
	GridConnectivity, D()->mPointListDonor);
	
OPTIONAL_CHILD_IMPLEMENTATION_WRITE(InterpolantsDonor, DataArray,
	GridConnectivity, D()->mInterpolants);

SIMPLE_CHILD_IMPLEMENTATION_WRITE(GridConnectivityType,
	GridConnectivity::GridConnectivityType, GridConnectivity, D()->mType);
