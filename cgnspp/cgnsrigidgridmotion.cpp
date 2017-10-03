/***************************************************************************
                          cgnsrigidgridmotion.cpp  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsrigidgridmotion.h"

#include "cgnsdataarray.h"
#include "cgnsbase.h"
#include "cgnsnodeutil.h"

using CGNS::RigidGridMotion;

GENERATE_D(RigidGridMotion);

COMMON_NODE_IMPLEMENTATION(RigidGridMotion);

INIT_IMPLEMENTATION0(RigidGridMotion);

int CGNS::Private::RigidGridMotion::getPhysicalDimension() const {
	Zone const * zone=P();
	if (!zone) return 0;
	return zone->getPhysicalDimension();
}

void CGNS::Private::RigidGridMotion::Read(RigidGridMotion * me) {
	readADF(me->mNode, me->mType);
	int dim=me->getPhysicalDimension();
	checkRequired(me->mNode, "OriginLocation", "RigidGridMotion");
	ADF::Node orgLoc(me->mNode.getChild("OriginLocation"));
	checkNodeReal(orgLoc, dim, 2);
	orgLoc.readData(&me->mOriginLocationBefore.m3D.mX);
}

static CGNS::Point readPoint(ADF::Node const & iNode, int iDim) {
	CGNS::Private::checkNodeReal(iNode, iDim);
	CGNS::Point p;
	switch (iDim) {
	case 1: iNode.readData(&p.m1D.mX); return p;
	case 2: iNode.readData(&p.m2D.mX); return p;
	case 3: iNode.readData(&p.m3D.mX); return p;
	default: EMIT_INTERNAL_ERROR();
	}
}

void CGNS::Private::RigidGridMotion::Read(RigidGridMotion * me,
	std::auto_ptr<Point> & cDest) {
	int dim=me->getPhysicalDimension();
#define FETCH_POINT(name)                              \
	if (&cDest==&me->m##name)                          \
		if (me->hasArray(#name)) {                     \
			if (me->m##name.get()==0)                  \
				me->m##name.reset(new Point(readPoint( \
					me->mNode.getChild(#name), dim))); \
		} else me->m##name.release()
	FETCH_POINT(RigidRotationAngle);
	FETCH_POINT(RigidVelocity);
	FETCH_POINT(RigidRotationRate);
#undef FETCH_POINT
}	

void CGNS::Private::RigidGridMotion::Needs(RigidGridMotion * me,
	std::auto_ptr<Point> & cDest) {
	me->Read(me, cDest);
}

void CGNS::Private::RigidGridMotion::Modify(CGNS::RigidGridMotion::RigidGridMotionType iType) {
	validateIndex(iType, CGNS::RigidGridMotion::NULL_MOTION, CGNS::RigidGridMotion::USER_MOTION);
	writeADF(mNode, iType);
	mType=iType;;
}

SIMPLE_CHILD_IMPLEMENTATION(OriginLocationBefore, CGNS::Point,
	RigidGridMotion, D()->mOriginLocationBefore);
DO_COORS_ALL(OriginLocationBefore, CGNS::Point, RigidGridMotion);
SIMPLE_CHILD_IMPLEMENTATION(OriginLocationAfter, CGNS::Point,
	RigidGridMotion, D()->mOriginLocationAfter);
DO_COORS_ALL(OriginLocationAfter, CGNS::Point, RigidGridMotion);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(RigidRotationAngle, CGNS::Point,
	RigidGridMotion, D()->mRigidRotationAngle);
DO_COORS_ALL(RigidRotationAngle, CGNS::Point, RigidGridMotion);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(RigidVelocity, CGNS::Point,
	RigidGridMotion, D()->mRigidVelocity);
DO_COORS_ALL(RigidVelocity, CGNS::Point, RigidGridMotion);

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(RigidRotationRate, CGNS::Point,
	RigidGridMotion, D()->mRigidRotationRate);
DO_COORS_ALL(RigidRotationRate, CGNS::Point, RigidGridMotion);

