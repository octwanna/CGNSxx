/***************************************************************************
                          cgnszonebc.cpp  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnszonebc.h"

#include "cgnsbc.h"

using CGNS::ZoneBC;

GENERATE_D(ZoneBC);

COMMON_NODE_IMPLEMENTATION(ZoneBC);

INIT_IMPLEMENTATION1(ZoneBC, mBC);

N_CHILDS_IMPLEMENTATION_WRITE1(BC, BC, ZoneBC, D()->mBC, CGNS::BC::BCType);

