/***************************************************************************
                          cgnsstated.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSSTATED_H
#define CGNSSTATED_H

#include "cgnsdimensioned.h"

/** Implementation data for nodes with reference state
  */
struct CGNS::Private::Stated : CGNS::Private::Dimensioned  {
COMMON_NODE_INTERFACE(Stated, Stated, Dimensioned);
	refcnt_ptr<ReferenceState> mState;

	bool hasState() const;
	ReferenceState * getState() const;
	
	// specialisations
	USING_NEEDS(Dimensioned);
	static void Needs(Stated * me, refcnt_ptr<ReferenceState> & cDest);
DISTINCT_CHILDREN1(ReferenceState);
};

#endif
