/***************************************************************************
                          cgnsbase.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBASE_H
#define CGNSBASE_H

#include "cgnsconverged.h"

/** Implementation data for base nodes
  */
struct CGNS::Private::Base : CGNS::Private::Converged  {
COMMON_NODE_INTERFACE(Base, Base, Converged);
	child_container<CGNS::Zone> mZones;
	child_container<CGNS::Family> mFamilies;
	refcnt_ptr<BaseIterativeData> mIterativeData;
	std::auto_ptr<CGNS::Base::SimulationType> mSimulationType;
	
	int mCellDimension;
	int mPhysicalDimension;
	
	void Modify(int iCellDimension, int iPhysicalDimension);
	
	USING_READ(Converged);
	static void Read(Base * me);
DISTINCT_CHILDREN4(Zone, Family, BaseIterativeData, CGNS::Base::SimulationType);
};

#endif
