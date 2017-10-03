/***************************************************************************
                          cgnsbc.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBC_H
#define CGNSBC_H

#include "cgnsstated.h"

#include "cgnszonebc.h"

/** Implementation of the BC_t subnode
  */
struct CGNS::Private::BC : CGNS::Private::Stated  {
COMMON_NODE_INTERFACE(BC, ZoneBC, Stated);
	CGNS::BC::BCType mType;
	GridLocation mLocation;
	std::auto_ptr<IndexRange> mPointRange;
	refcnt_ptr<IndexArray> mPointList;
	std::auto_ptr<std::string> mFamilyName;
	std::auto_ptr<Normal> mInwardNormalIndex;
	refcnt_ptr<IndexArray> mNormalList;
	
	child_container<CGNS::BCDataSet> mDataSets;
	
	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	USING_READ(Stated);	
	static void Read(BC * me);
	static void Read(BC * me, std::auto_ptr<IndexRange> & cDest);
	static void Read(BC * me, std::auto_ptr<Normal> & cDest);
	static void Read(BC * me, refcnt_ptr<IndexArray> & cDest);
	static void Read(BC * me, std::auto_ptr<std::string> & cDest);

	void Modify(CGNS::BC::BCType iType);
DISTINCT_CHILDREN6(CGNS::BC::BCType, IndexRange, IndexArray, std::string, Normal, BCDataSet);
};

#endif
