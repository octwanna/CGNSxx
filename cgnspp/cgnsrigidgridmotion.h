/***************************************************************************
                          cgnsrigidgridmotion.h  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSRIGIDGRIDMOTION_H
#define CGNSRIGIDGRIDMOTION_H

#include "cgnsarrayed.h"

#include "cgnszone.h"

/** Implementation data for RigidGridMotion subnode
  */
struct CGNS::Private::RigidGridMotion : CGNS::Private::Arrayed  {
COMMON_NODE_INTERFACE(RigidGridMotion, Zone, Arrayed);
	CGNS::RigidGridMotion::RigidGridMotionType mType;
	// must be kept together, without padding in between
	Point mOriginLocationBefore, mOriginLocationAfter;
	
	std::auto_ptr<Point> mRigidRotationAngle;
	std::auto_ptr<Point> mRigidVelocity;
	std::auto_ptr<Point> mRigidRotationRate;
	
	int getPhysicalDimension() const;
	
	USING_READ(BaseClass);
	static void Read(RigidGridMotion * me);
	static void Read(RigidGridMotion * me, std::auto_ptr<Point> & cDest);
	
	USING_NEEDS(BaseClass);
	static void Needs(RigidGridMotion * me, std::auto_ptr<Point> & cDest);
	// dummy for DO_COORS_ALL
	static void Needs(RigidGridMotion * me, Point &) {}
	
	void Modify(CGNS::RigidGridMotion::RigidGridMotionType iType);
DISTINCT_CHILDREN0();
};

#endif
