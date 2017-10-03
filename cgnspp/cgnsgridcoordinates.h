/***************************************************************************
                          cgnsgridcoordinates.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSGRIDCOORDINATES_H
#define CGNSGRIDCOORDINATES_H

#include "cgnsgridded.h"

/** Implementation data for grid coordinate subnodes
  */
struct CGNS::Private::GridCoordinates : CGNS::Private::Gridded  {
COMMON_NODE_INTERFACE(GridCoordinates, Zone, Gridded);
	typedef child_container<CGNS::DataArray> coordinates_t;
	coordinates_t mCoordinates;
	
    USING_READ(Gridded);
    static void Read(GridCoordinates * me, child_container<CGNS::DataArray> & cDest);
    USING_NEEDS(Gridded);
    static void Needs(GridCoordinates * me, child_container<CGNS::DataArray> & cDest);
DISTINCT_CHILDREN0(); // no DataArray, this is already in ../../Arrayed
};

#endif
