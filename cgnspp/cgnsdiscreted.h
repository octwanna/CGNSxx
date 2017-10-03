/***************************************************************************
                          cgnsdiscreted.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSDISCRETED_H
#define CGNSDISCRETED_H

#include "cgnsgridded.h"

#include "cgnszone.h"

/** Implementation data for a discreted node
  */
struct CGNS::Private::Discreted : CGNS::Private::Gridded  {
COMMON_NODE_INTERFACE(Discreted, Zone, Gridded);
	GridLocation mLocation;
	
	USING_READ(Gridded);
	static void Read(Discreted * me);

#ifdef HAVE_USING_BASE_MEMBERS	
	using Gridded::Modify;
#else
	void Modify(Rind1D iRind) { Gridded::Modify(iRind); }
	void Modify(Rind2D iRind) { Gridded::Modify(iRind); }
	void Modify(Rind3D iRind) { Gridded::Modify(iRind); }
#endif	
	void Modify(GridLocation iLocation);
	void Modify(GridLocation iLocation, Rind1D iRind);
	void Modify(GridLocation iLocation, Rind2D iRind);
	void Modify(GridLocation iLocation, Rind3D iRind);
DISTINCT_CHILDREN0();
};

#endif
