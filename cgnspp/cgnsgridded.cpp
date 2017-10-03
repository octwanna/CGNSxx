/***************************************************************************
                          cgnsgridded.cpp  -  description
                             -------------------
    begin                : Wed Mar 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsgridded.h"

#include <iostream>
#include <assert.h>

using CGNS::Gridded;

GENERATE_D(Gridded);

COMMON_NODE_IMPLEMENTATION(Gridded);

INIT_IMPLEMENTATION1(Gridded, mRind);

void CGNS::Private::Gridded::Read(Gridded * me, std::auto_ptr<Rind> & cDest) {
	Needs(me->getZone());
	char const * name=implementation_traits<Rind>::Name();
	if (me->mNode.hasChild(name)) {
		ADF::Node rind=me->mNode.getChild(name);
		if (rind.getLabel()==implementation_traits<Rind>::Label()) {
			int index=me->getZone()->getIndexDimension();
			assert(index>0 && index<=MAX_INDEX_DIMENSION);
			if (cDest.get()==0) cDest.reset(new Rind);
/*			checkNode(rind, ADF::I4, 2*index);
			int data[2*MAX_INDEX_DIMENSION];
			rind.readData(data);
			*cDest.get()=*reinterpret_cast<Rind *>(data);
*/			readRangeADF(rind, *cDest);
		}
	}
	me->mInitialized|=implementation_traits<Rind>::INIT_MASK;
}

#define MODIFY_RIND_IMPLEMENTATION(dim)                          \
void CGNS::Private::Gridded::Modify(Rind##dim##D iRind) {        \
	if (getZone()->getCellDimension()!=dim)                      \
		Emit(Exception::WRONG_NUM_DIMENSIONS);                   \
	ADF::Node rind=mNode.createChild(                            \
		implementation_traits<Rind>::Name(),                     \
		implementation_traits<Rind>::Label());                   \
	rind.setDataTypeDimensions(ADF::I4, dim*2);                  \
	rind.writeData(&iRind.mMinI);                                \
	if (!mRind.get()) mRind.reset(new Rind);                     \
	(*mRind).m##dim##D=iRind;                                    \
}

MODIFY_RIND_IMPLEMENTATION(1);
MODIFY_RIND_IMPLEMENTATION(2);
MODIFY_RIND_IMPLEMENTATION(3);

int * CGNS::Private::Gridded::adjustRind(int * cDims) {
	Needs(this, mRind);
	if (!mRind.get()) return cDims;
	int index=getZone()->mIndexDimension;
	switch (index) {
		case 3: cDims[2]-=mRind->m3D.mMaxK+mRind->m3D.mMinK;
		case 2: cDims[1]-=mRind->m2D.mMaxJ+mRind->m2D.mMinJ;
		case 1: cDims[0]-=mRind->m1D.mMaxI+mRind->m1D.mMinI;
			break;
		default: EMIT_INTERNAL_ERROR();
	}
	return cDims;
}

bool Gridded::hasRind() const {
	NEEDS(mRind);
	return D()->mRind.get()!=0;
}

int Gridded::getRind(Index iIndex) const {
	NEEDS(mRind);
	switch (D()->getZone()->mIndexDimension) {
		case 1: Private::checkIndex(iIndex, I_MIN, I_MAX); break;
		case 2: Private::checkIndex(iIndex, I_MIN, J_MAX); break;
		case 3: Private::checkIndex(iIndex, I_MIN, K_MAX); break;
		default: /*std::cerr << "Gridded::getRind\n" << std::flush; */EMIT_INTERNAL_ERROR();
	}
	if (!hasRind()) return 0;
	return (&(*D()->mRind).m3D.mMinI)[iIndex]; // really bad hack!!!
}
