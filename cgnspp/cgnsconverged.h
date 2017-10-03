/***************************************************************************
                          cgnsconverged.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSCONVERGED_H
#define CGNSCONVERGED_H

#include "cgnsstated.h"

/** Implementation data common for bases and zones
  */
struct CGNS::Private::Converged : CGNS::Private::Stated  {
COMMON_NODE_INTERFACE(Converged, Converged, Stated);
	child_container<CGNS::IntegralData> mIntegral;
	refcnt_ptr<ConvergenceHistory> mHistory;
	refcnt_ptr<FlowEquationSet> mEqSet;
	
	USING_READ(Stated);
	static void Read(Converged * me, refcnt_ptr<ConvergenceHistory> & cDest);

	Zone const * getZone() const;
DISTINCT_CHILDREN3(IntegralData, ConvergenceHistory, FlowEquationSet);
friend class CGNS::Converged;
};

#endif
