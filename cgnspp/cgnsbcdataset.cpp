/***************************************************************************
                          cgnsbcdataset.cpp  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsbcdataset.h"

#include "cgnsbcdata.h"

#include <assert.h>

using CGNS::BCDataSet;

GENERATE_D(BCDataSet);

COMMON_NODE_IMPLEMENTATION(BCDataSet);

INIT_IMPLEMENTATION2(BCDataSet, mDirichlet, mNeumann);

void CGNS::Private::BCDataSet::Read(BCDataSet * me) {
	me->mType=CGNS::BC::USER_BC;
	readADF(me->mNode, me->mType);
	
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::BCDataSet::Read(BCDataSet * me, refcnt_ptr<BCData> & cDest) {
	assert(&cDest==&me->mDirichlet || &cDest==&me->mNeumann);
	readChild(me->mNode, "DirichletData", me->mDirichlet, me);
	readChild(me->mNode, "NeumannData", me->mNeumann, me);
	me->mInitialized|=implementation_traits<BCData>::INIT_MASK;
}

void CGNS::Private::BCDataSet::Modify(CGNS::BC::BCType iType) {
	validateIndex(iType, CGNS::BC::NULL_BC, CGNS::BC::USER_BC);
	writeADF(mNode, mType=iType);
}

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(DirichletData, BCData, BCDataSet, D()->mDirichlet);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(NeumannData, BCData, BCDataSet, D()->mNeumann);

CGNS::BC::BCType BCDataSet::getBCType() const {
	NEEDS_THIS();
	return D()->mType;
}
