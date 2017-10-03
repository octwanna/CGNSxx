/***************************************************************************
                          cgnszonebc.h  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSZONEBC_H
#define CGNSZONEBC_H

#include "cgnsstated.h"

#include "cgnszone.h"

/** Implementation data for ZoneBC_t subnodes
  */
struct CGNS::Private::ZoneBC : CGNS::Private::Stated  {
COMMON_NODE_INTERFACE(ZoneBC, Zone, Stated);
	child_container<CGNS::BC> mBC;

	Zone const * getZone() const { return P(); }
DISTINCT_CHILDREN1(BC);
};

#endif
