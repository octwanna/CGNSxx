/***************************************************************************
                          cgnsdiscretedata.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSDISCRETEDATA_H
#define CGNSDISCRETEDATA_H

#include "cgnsdiscreted.h"

/** Implementation data for a discrete data node
  */
struct CGNS::Private::DiscreteData : CGNS::Private::Discreted  {
COMMON_NODE_INTERFACE(DiscreteData, Zone, Discreted);

	USING_READ(Discreted);
	static void Read(DiscreteData * me) { Discreted::Read(static_cast<Discreted *>(me)); }
DISTINCT_CHILDREN0();
};

#endif
