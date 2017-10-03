/***************************************************************************
                          cgnszoneiterativedata.h  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSZONEITERATIVEDATA_H
#define CGNSZONEITERATIVEDATA_H

#include "cgnsarrayed.h"

#include "cgnszone.h"

/** Implementation data for BaseIterativeData subnode
  */
struct CGNS::Private::ZoneIterativeData : CGNS::Private::Arrayed {
COMMON_NODE_INTERFACE(ZoneIterativeData, Zone, Arrayed);
	refcnt_ptr<DataArray> mRigidGridMotionPointers;
	refcnt_ptr<DataArray> mArbitraryGridMotionPointers;
	refcnt_ptr<DataArray> mGridCoordinatesPointers;
	refcnt_ptr<DataArray> mFlowSolutionPointers;
	
	USING_READ(BaseClass);
	static void Read(ZoneIterativeData * me, refcnt_ptr<DataArray> &cDest);
	USING_NEEDS(BaseClass);
	static void Needs(ZoneIterativeData * me, refcnt_ptr<DataArray> &cDest);
	
    void Modify() {}
	void Modify(int iNumberOfSteps);
	
private:	
	int getSteps() const;
	void updateSteps();	
	
	void getPointer(refcnt_ptr<DataArray> const & iArray,
		int iStep, std::string * oPointer) const;
	void writePointer(refcnt_ptr<DataArray> & cArray,
		int iStep, std::string const & iPointer, std::string const & iName);
DISTINCT_CHILDREN0();
friend class CGNS::ZoneIterativeData;
};

#endif
