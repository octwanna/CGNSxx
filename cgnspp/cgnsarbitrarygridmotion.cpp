/***************************************************************************
                          cgnsarbitrarygridmotion.cpp  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsarbitrarygridmotion.h"

using CGNS::ArbitraryGridMotion;

GENERATE_D(ArbitraryGridMotion);

COMMON_NODE_IMPLEMENTATION(ArbitraryGridMotion);

INIT_IMPLEMENTATION0(ArbitraryGridMotion);

void CGNS::Private::ArbitraryGridMotion::Read(ArbitraryGridMotion * me) {
	readADF(me->mNode, me->mType);
}

void CGNS::Private::ArbitraryGridMotion::Modify(
	CGNS::ArbitraryGridMotion::ArbitraryGridMotionType iType) {
	validateIndex(iType, CGNS::ArbitraryGridMotion::NULL_MOTION, CGNS::ArbitraryGridMotion::USER_MOTION);
	writeADF(mNode, iType);
	mType=iType;;
}

ArbitraryGridMotion::ArbitraryGridMotionType ArbitraryGridMotion::getType() const {
	NEEDS_THIS();
	return D()->mType;
}

