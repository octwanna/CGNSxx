/***************************************************************************
                          cgnsbcdataset.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBCDATASET_H
#define CGNSBCDATASET_H

#include "cgnsstated.h"

#include "cgnsbc.h"

/** Implementation data of the BCDataSet_t subnode
  */
struct CGNS::Private::BCDataSet : CGNS::Private::Stated  {
COMMON_NODE_INTERFACE(BCDataSet, BC, Stated);
	CGNS::BC::BCType mType;
	refcnt_ptr<BCData> mDirichlet;
	refcnt_ptr<BCData> mNeumann;
	
	USING_READ(Stated);	
	static void Read(BCDataSet * me);
	static void Read(BCDataSet * me, refcnt_ptr<BCData> & cDest);

	void Modify(CGNS::BC::BCType iType);
DISTINCT_CHILDREN2(CGNS::BC::BCType, BCData);
};

#endif
