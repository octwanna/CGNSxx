/***************************************************************************
                          cgnsoversetholes.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSOVERSETHOLES_H
#define CGNSOVERSETHOLES_H

#include "cgnsparented.h"

#include "cgnszonegridconnectivity.h"

/** Implementation data for OversetHoles_t subnode
  */
struct CGNS::Private::OversetHoles : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(OversetHoles, ZoneGridConnectivity, Parented);
	GridLocation mLocation;
	refcnt_ptr<IndexArray> mPointList;
	child_container<IndexRange> mPointRange;
	
	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	USING_READ(Parented);
	static void Read(OversetHoles * me);
DISTINCT_CHILDREN3(GridLocation, IndexArray, IndexRange);
};

#endif
