/***************************************************************************
                          cgnsindexarray.h  -  description
                             -------------------
    begin                : Fri Feb 8 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSINDEXARRAY_H
#define CGNSINDEXARRAY_H

#include "cgnsnode.h"

/**Implementation data for nodes containing index data arrays
  */
struct CGNS::Private::IndexArray : CGNS::Private::Node  {
COMMON_SIMPLE_NODE_INTERFACE(IndexArray, Node, Node);
	int mLength;
	
	USING_READ(Node);
	static void Read(IndexArray * me);

	void Modify(int iIndexDimension);
			
DISTINCT_CHILDREN0();
};

#endif
