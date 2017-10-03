/***************************************************************************
                          cgnsgridconnectivity1to1.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSGRIDCONNECTIVITY1TO1_H
#define CGNSGRIDCONNECTIVITY1TO1_H

#include "cgnsparented.h"

#include "cgnszonegridconnectivity.h"

/** Implementation data for GridConnectivity1to1_t subnode
  */
struct CGNS::Private::GridConnectivity1to1 : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(GridConnectivity1to1, ZoneGridConnectivity, Parented);
	std::string mZoneDonorName;
	std::auto_ptr<Transform> mTransform;
	IndexRange mPointRange;
	IndexRange mPointRangeDonor;
	
	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	void Modify(std::string const & iZoneDonorName);
	void Modify(CGNS::Zone const & iZoneDonor);
	
	USING_READ(Node);	
	static void Read(GridConnectivity1to1 * me);
	static void Read(GridConnectivity1to1 * me, std::auto_ptr<Transform> & cDest);
	static void Read(GridConnectivity1to1 * me, IndexRange & cDest);
DISTINCT_CHILDREN2(Transform, IndexRange);
};

#endif
