/***************************************************************************
                          cgnsconverged.cpp  -  description
                             -------------------
    begin                : Mon Mar 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsconverged.h"

#include "cgnsconvergence.h"
#include "cgnsequationset.h"
#include "cgnsreferencestate.h"
#include "cgnsbroker.h"

#include "cgnszone.h"

#include <iostream>
#include <assert.h>

using CGNS::Converged;

GENERATE_D(Converged);

COMMON_NODE_IMPLEMENTATION(Converged);

INIT_IMPLEMENTATION3(Converged, mHistory, mEqSet, mIntegral);

CGNS::Private::Zone const * CGNS::Private::Converged::getZone() const {
	return P() ? static_cast<Zone const *>(this) : 0;
}
		
void CGNS::Private::Converged::Read(Converged * me,
	refcnt_ptr<ConvergenceHistory> & cDest) {
	assert(&cDest==&me->mHistory);
	// decide if this is a zone or base for stupid default name confusion
	char const * historyName=me->mParent ?
		"ZoneConvergenceHistory" : "GlobalConvergenceHistory";
	readChild(me->mNode, historyName, cDest, me);
}

OPTIONAL_CHILD_IMPLEMENTATION(ConvergenceHistory, ConvergenceHistory,
	Converged, D()->mHistory)

CGNS::ConvergenceHistory Converged::writeConvergenceHistory(int iNumIterations) {
	NEEDS(mHistory);
	char const * historyName=D()->P() ?
		"ZoneConvergenceHistory" : "GlobalConvergenceHistory";
	CGNS::Private::ConvergenceHistory * history=Private::assureChild(D()->mHistory,
		historyName, D());
	history->Modify(iNumIterations);
	return history;
}

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(FlowEquationSet, FlowEquationSet,
	Converged, D()->mEqSet)

N_CHILDS_IMPLEMENTATION_WRITE(IntegralData, IntegralData, Converged, D()->mIntegral)
