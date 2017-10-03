/***************************************************************************
                          cgnszone.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSZONE_H
#define CGNSZONE_H

#include "cgnsconverged.h"

#include "cgnsbase.h"

/** Implementation data for zone nodes
  */
struct CGNS::Private::Zone : CGNS::Private::Converged  {
COMMON_NODE_INTERFACE(Zone, Base, Converged);
	refcnt_ptr<GridCoordinates> mCoordinates;
	child_container<CGNS::GridCoordinates> mIterCoordinates;
	child_container<CGNS::FlowSolution> mSolutions;
	child_container<CGNS::DiscreteData> mDiscrete;
	std::auto_ptr<std::string> mFamilyName;
	child_container<CGNS::Elements> mElements;
	refcnt_ptr<ZoneIterativeData> mIterativeData;
	refcnt_ptr<ZoneGridConnectivity> mConnectivity;
	refcnt_ptr<ZoneBC> mZoneBC;
	child_container<CGNS::RigidGridMotion> mRigidMotion;
	child_container<CGNS::ArbitraryGridMotion> mArbitraryMotion;
	int mIndexDimension;
	int mVertices[MAX_INDEX_DIMENSION];
	int mCells[MAX_INDEX_DIMENSION];
	int mBoundaryVertices;
	bool mStructured;
	
	Base const * getBase() const { return P(); }
	int getCellDimension() const { getBase()->Needs(getBase()); return getBase()->mCellDimension; }
	int getPhysicalDimension() const { getBase()->Needs(getBase()); return getBase()->mPhysicalDimension; }
	int getIndexDimension() const { return mIndexDimension; }
		
	USING_READ(Converged);
	static void Read(Zone * me);
	static void Read(Zone * me, std::auto_ptr<std::string> & cDest);
	static void Read(Zone * me, refcnt_ptr<GridCoordinates> & cDest);
	static void Read(Zone * me, child_container<CGNS::GridCoordinates> & cDest);

	void ModifyStructured(int iIndexDimension, int const * iNumVertices);
	void ModifyUnstructured(int iNumVertices, int iNumCells,
		int iBoundaryVertices);
DISTINCT_CHILDREN10(GridCoordinates, FlowSolution, DiscreteData, std::string,
	Elements, ZoneIterativeData, ZoneGridConnectivity, ZoneBC, RigidGridMotion,
	ArbitraryGridMotion);
};

#endif
