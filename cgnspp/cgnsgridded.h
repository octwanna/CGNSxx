/***************************************************************************
                          cgnsgridded.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSGRIDDED_H
#define CGNSGRIDDED_H

#include "cgnsarrayed.h"

#include "cgnszone.h"

/** Implementation data for nodes (possibly) with rind planes
  */
struct CGNS::Private::Gridded : CGNS::Private::Arrayed  {
COMMON_NODE_INTERFACE(Gridded, Zone, Arrayed);
	std::auto_ptr<Rind> mRind;

	Zone * getZone() { return P(); }
	Zone const * getZone() const { return P(); }
	
	int * adjustRind(int * cDims);
	
	USING_READ(Arrayed);
	static void Read(Gridded * me, std::auto_ptr<Rind> & cDest);

    void Modify() {}
	void Modify(Rind1D iRind);
	void Modify(Rind2D iRind);
	void Modify(Rind3D iRind);
DISTINCT_CHILDREN1(Rind);
};

#endif
