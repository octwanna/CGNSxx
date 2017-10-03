/***************************************************************************
                          cgnsreferencestate.h  -  description
                             -------------------
    begin                : Thu Sep 20 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSREFERENCESTATE_H
#define CGNSREFERENCESTATE_H

#include "cgnsvalued.h"

#include "cgnsstated.h"

/** Implementation data for the ReferenceState subnode
  */
struct CGNS::Private::ReferenceState : CGNS::Private::Valued  {
COMMON_NODE_INTERFACE(ReferenceState, Stated, Valued);
DISTINCT_CHILDREN0();
};

#endif
