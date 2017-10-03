/***************************************************************************
                          cgnsoversetholes.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsoversetholes.h"

#include "cgnsindexarray.h"

#include "cgnsnodeutil.h"

using CGNS::OversetHoles;

GENERATE_D(OversetHoles);

COMMON_NODE_IMPLEMENTATION(OversetHoles);

INIT_IMPLEMENTATION0(OversetHoles);

void CGNS::Private::OversetHoles::Read(OversetHoles * me) {
	me->mLocation=VERTEX;
	readStringChild(me->mNode, me->mLocation);
	me->mInitialized|=INIT_MASK_SELF;
}


SIMPLE_CHILD_IMPLEMENTATION_WRITE(Location, CGNS::GridLocation,
	OversetHoles, D()->mLocation);

OPTIONAL_CHILD_IMPLEMENTATION(PointList, IndexArray,
	OversetHoles, D()->mPointList);

CHILD_IMPLEMENTATION_WRITE_INDEX(PointList, IndexArray,
	OversetHoles, D()->mPointList);

N_CHILDS_IMPLEMENTATION(PointRange, IndexRange, OversetHoles, D()->mPointRange)

CGNS::IndexRange OversetHoles::writePointRange(std::string const & iName, CGNS::IndexRange iRange) {
	NEEDSM(D()->mPointRange);
	ADF::Node range=Private::assureChild(D()->mPointRange, D(), iRange, iName);
	int index=D()->getZone() ? D()->getZone()->getIndexDimension() : 3;
	switch (index) {
		case 1: Private::writeADF(range, iRange.m1D); break;
		case 2: Private::writeADF(range, iRange.m2D); break;
		case 3: Private::writeADF(range, iRange.m3D); break;
	}
	return iRange;
}

