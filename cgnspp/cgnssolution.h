/***************************************************************************
                          cgnssolution.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSSOLUTION_H
#define CGNSSOLUTION_H

#include "cgnsdiscreted.h"

/** Implementation data for a flow solution node
  */
struct CGNS::Private::FlowSolution : CGNS::Private::Discreted  {
COMMON_NODE_INTERFACE(FlowSolution, Zone, Discreted);

	USING_READ(Discreted);
	static void Read(FlowSolution * me) { Discreted::Read(static_cast<Discreted *>(me)); }
DISTINCT_CHILDREN0();
};

#endif
