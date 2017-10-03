/***************************************************************************
                          cgnszonegridconnectivity.h  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSZONEGRIDCONNECTIVITY_H
#define CGNSZONEGRIDCONNECTIVITY_H

#include "cgnsparented.h"

#include "cgnszone.h"

/** Implementation data for zone connectivity nodes
  */
struct CGNS::Private::ZoneGridConnectivity : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(ZoneGridConnectivity, Zone, Parented);
	child_container<CGNS::GridConnectivity1to1> mConnectivity1to1;
	child_container<CGNS::GridConnectivity> mConnectivity;
	child_container<CGNS::OversetHoles> mHoles;

	Zone const * getZone() const { return P(); }
DISTINCT_CHILDREN3(GridConnectivity1to1, GridConnectivity, OversetHoles);
};

#endif
