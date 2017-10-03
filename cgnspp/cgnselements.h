/***************************************************************************
                          cgnselements.h  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSELEMENTS_H
#define CGNSELEMENTS_H

#include "cgnsnode.h"

#include "cgnsindexarray.h"

/** Implementation data for Elements_t subnodes
  */
struct CGNS::Private::Elements : CGNS::Private::Node  {
COMMON_SIMPLE_NODE_INTERFACE(Elements, Zone, Node);
	CGNS::Elements::ElementType mElementType;
	int mElementSizeBoundary;
	
	// they are technically required, but we treat them as optional anyway
	std::auto_ptr<IndexRange1D> mElementRange;
	refcnt_ptr<DataArray> mElementConnectivity;
	// this is optional again
	refcnt_ptr<DataArray> mParentData;
	
	USING_READ(Node);	
	static void Read(Elements * me);
	static void Read(Elements * me, refcnt_ptr<DataArray> & cDest);
	static void Read(Elements * me, std::auto_ptr<IndexRange1D> & cDest);
	
	void Modify(CGNS::Elements::ElementType iElementType, int iElementsOnBoundary);
DISTINCT_CHILDREN2(IndexRange, DataArray);
};

#endif
