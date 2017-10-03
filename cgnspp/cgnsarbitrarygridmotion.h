/***************************************************************************
                          cgnsarbitrarygridmotion.h  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSARBITRARYGRIDMOTION_H
#define CGNSARBITRARYGRIDMOTION_H

#include "cgnsdiscreted.h"

#include "cgnszone.h"

/** Implementation data for an arbitrary grid motion
  */
struct CGNS::Private::ArbitraryGridMotion : CGNS::Private::Discreted  {
COMMON_NODE_INTERFACE(ArbitraryGridMotion, Zone, Discreted);
	CGNS::ArbitraryGridMotion::ArbitraryGridMotionType mType;
	
	USING_READ(BaseClass);
	static void Read(ArbitraryGridMotion * me);
	
	void Modify(CGNS::ArbitraryGridMotion::ArbitraryGridMotionType iType);
DISTINCT_CHILDREN0();
};

#endif
