/***************************************************************************
                          cgnsstated.cpp  -  description
                             -------------------
    begin                : Mon Mar 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsstated.h"

#include "cgnsreferencestate.h"

#include <iostream>
#include <assert.h>

using CGNS::Stated;

GENERATE_D(Stated);

COMMON_NODE_IMPLEMENTATION(Stated);

INIT_IMPLEMENTATION1(Stated, mState);

void CGNS::Private::Stated::Needs(Stated * me, refcnt_ptr<ReferenceState> & cDest) {
	assert(&cDest==&me->mState);
	if (!(me->mInitialized&implementation_traits<ReferenceState>::INIT_MASK)) {
		Dimensioned::Needs(me, cDest);
		if (cDest.get()!=0) return;
	}
	if (me->P()) Needs(me->P(), me->P()->mState);
}

bool CGNS::Private::Stated::hasState() const {
	Needs(this, mState);
	if (mState!=0) return true;
	if (!mParent) return false;
	return P()->hasState();
}

CGNS::Private::ReferenceState * CGNS::Private::Stated::getState() const {
	Needs(this, mState);
	if (mState!=0) return const_cast<ReferenceState *>(mState.get());
	if (!mParent) Emit(Exception::NO_STATE);
	return P()->getState();
}


bool Stated::hasReferenceState() const {
	NEEDS(mState);
	return D()->hasState();
}

bool Stated::hasLocalReferenceState() const {
	NEEDS(mState);
	return D()->mState!=0;
}

CGNS::ReferenceState Stated::getReferenceState() const {
	NEEDS(mState);
	return D()->getState();
}

CGNS::ReferenceState Stated::writeReferenceState() {
	NEEDS(mState);
	return Private::assureChild(D()->mState, "ReferenceState", D());
}

