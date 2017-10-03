/***************************************************************************
                          cgnsgridconnectivity.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSGRIDCONNECTIVITY_H
#define CGNSGRIDCONNECTIVITY_H

#include "cgnsparented.h"

#include "cgnszonegridconnectivity.h"

/** Implementation data for GridConnectivity_t subnode
  */
struct CGNS::Private::GridConnectivity : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(GridConnectivity, ZoneGridConnectivity, Parented);
	std::string mZoneDonorName;
	GridLocation mLocation;
	std::auto_ptr<IndexRange> mPointRange;
	refcnt_ptr<IndexArray> mPointList;
	refcnt_ptr<IndexArray> mPointListDonor;
	refcnt_ptr<IndexArray> mCellListDonor;
	refcnt_ptr<DataArray> mInterpolants;
	CGNS::GridConnectivity::GridConnectivityType mType;

	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	USING_READ(Node);
	static void Read(GridConnectivity * me);
	static void Read(GridConnectivity * me, std::auto_ptr<IndexRange> & cDest);
	static void Read(GridConnectivity * me, refcnt_ptr<IndexArray> & cDest);
	
	void Modify(std::string const & iZoneDonorName);
	void Modify(CGNS::Zone const & iZoneDonor);
DISTINCT_CHILDREN5(GridLocation, IndexRange, IndexArray, DataArray,
	CGNS::GridConnectivity::GridConnectivityType);
};

#endif
