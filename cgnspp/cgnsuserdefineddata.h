/***************************************************************************
                          cgnsuserdefineddata.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSUSERDEFINEDDATA_H
#define CGNSUSERDEFINEDDATA_H

#include "cgnsarrayed.h"

/** Implementation data for UserDefinedData_t subnodes
  */
struct CGNS::Private::UserDefinedData : CGNS::Private::Arrayed  {
	// HACK for non-parented nodes
	UserDefinedData(ADF::Node const & mNode, Node * iParent)
		: Arrayed(mNode, dynamic_cast<Dimensioned *>(iParent)) {}
COMMON_SIMPLE_NODE_INTERFACE(UserDefinedData, Dimensioned, Arrayed);
DISTINCT_CHILDREN0();
};

#endif
