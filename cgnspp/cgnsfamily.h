/***************************************************************************
                          cgnsfamily.h  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSFAMILY_H
#define CGNSFAMILY_H

#include "cgnsnode.h"

/** Implementation data for a Family_t subnode
  */
struct CGNS::Private::Family : CGNS::Private::Node  {
COMMON_SIMPLE_NODE_INTERFACE(Family, Base, Node);
	std::auto_ptr<CGNS::BC::BCType> mBCType;
	child_container<CGNS::GeometryReference> mGeometries;

    USING_READ(Node);
	void Read(Family * me, std::auto_ptr<CGNS::BC::BCType> & cDest);

DISTINCT_CHILDREN2(CGNS::BC::BCType, GeometryReference);
};

#endif
