/***************************************************************************
                          cgnsconvergence.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSCONVERGENCE_H
#define CGNSCONVERGENCE_H

#include "cgnsarrayed.h"

#include "cgnsconverged.h"

/** Implementation data for ConvergenceHistory subnodes
  */
struct CGNS::Private::ConvergenceHistory : CGNS::Private::Arrayed  {
COMMON_NODE_INTERFACE(ConvergenceHistory, Converged, Arrayed);
	int mNumIterations;
	
	USING_READ(Arrayed);
	static void Read(ConvergenceHistory * me);
	
	void Modify(int iNumIterations);
DISTINCT_CHILDREN0();
};

#endif
