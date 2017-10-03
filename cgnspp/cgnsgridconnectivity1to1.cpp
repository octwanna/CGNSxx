/***************************************************************************
                          cgnsgridconnectivity1to1.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsgridconnectivity1to1.h"

#include "cgnsnodeutil.h"

#include <assert.h>

using CGNS::GridConnectivity1to1;

GENERATE_D(GridConnectivity1to1);

COMMON_NODE_IMPLEMENTATION(GridConnectivity1to1);

INIT_IMPLEMENTATION3(GridConnectivity1to1, mTransform, mPointRange, mPointRangeDonor);

void CGNS::Private::GridConnectivity1to1::Read(GridConnectivity1to1 * me) {
	readADF(me->mNode, me->mZoneDonorName);
	// plausibility check
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::GridConnectivity1to1::Read(GridConnectivity1to1 * me,
	std::auto_ptr<Transform> & cDest) {
	assert(&cDest==&me->mTransform);
	readIndex("Transform", me, me->getZone(), cDest);
	
	me->mInitialized|=implementation_traits<Transform>::INIT_MASK;
}

void CGNS::Private::GridConnectivity1to1::Read(GridConnectivity1to1 * me,
	IndexRange & cDest) {
	assert(&cDest==&me->mPointRange || &cDest==&me->mPointRangeDonor);
	readIndexRange("PointRange", me, me->mPointRange);
	readIndexRange("PointRangeDonor", me, me->mPointRangeDonor);
	me->mInitialized|=CGNS::Private::implementation_traits<CGNS::IndexRange>::INIT_MASK;
}

void CGNS::Private::GridConnectivity1to1::Modify(std::string const & iZoneDonorName) {
	writeADF(mNode, iZoneDonorName);
	mZoneDonorName=iZoneDonorName;
}

void CGNS::Private::GridConnectivity1to1::Modify(CGNS::Zone const & iZoneDonor) {
	writeADF(mNode, iZoneDonor.getName());
	mZoneDonorName=iZoneDonor.getName();
}

std::string CGNS::GridConnectivity1to1::getZoneDonorName() const {
	NEEDS_THIS();
	return D()->mZoneDonorName;
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(Transform, CGNS::Transform, GridConnectivity1to1, D()->mTransform);

DO_INDICES_ALL(Transform, CGNS::Transform, GridConnectivity1to1);

CGNS::IndexRange GridConnectivity1to1::getPointRange() const {
	NEEDS(mPointRange);
	return D()->mPointRange;
}

DO_RANGE_ALL(GridConnectivity1to1, PointRange)

CGNS::IndexRange GridConnectivity1to1::getPointRangeDonor() const {
	NEEDS(mPointRangeDonor);
	return D()->mPointRangeDonor;
}

DO_RANGE_ALL(GridConnectivity1to1, PointRangeDonor)
