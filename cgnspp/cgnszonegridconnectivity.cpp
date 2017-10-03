/***************************************************************************
                          cgnszonegridconnectivity.cpp  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnszonegridconnectivity.h"

#include "cgnsgridconnectivity1to1.h"
#include "cgnsgridconnectivity.h"
#include "cgnsoversetholes.h"

using CGNS::ZoneGridConnectivity;

GENERATE_D(ZoneGridConnectivity);

COMMON_NODE_IMPLEMENTATION(ZoneGridConnectivity);

INIT_IMPLEMENTATION3(ZoneGridConnectivity, mConnectivity1to1, mConnectivity, mHoles);

N_CHILDS_IMPLEMENTATION_WRITE1(GridConnectivity1to1, GridConnectivity1to1,
	ZoneGridConnectivity, D()->mConnectivity1to1, std::string const &)
CHILDS_IMPLEMENTATION_WRITE1(GridConnectivity1to1, GridConnectivity1to1,
	ZoneGridConnectivity, D()->mConnectivity1to1, CGNS::Zone const &)


N_CHILDS_IMPLEMENTATION_WRITE1(GridConnectivity, GridConnectivity,
	ZoneGridConnectivity, D()->mConnectivity, std::string const &)
CHILDS_IMPLEMENTATION_WRITE1(GridConnectivity, GridConnectivity,
	ZoneGridConnectivity, D()->mConnectivity, CGNS::Zone const &)

N_CHILDS_IMPLEMENTATION_WRITE(OversetHoles, OversetHoles, ZoneGridConnectivity, D()->mHoles)
