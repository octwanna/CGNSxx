/***************************************************************************
                          cgnszone.cpp  -  description
                             -------------------
    begin                : Wed Mar 14 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnszone.h"

#include "cgnsgridcoordinates.h"
#include "cgnssolution.h"
#include "cgnsdiscretedata.h"
#include "cgnselements.h"
#include "cgnszoneiterativedata.h"
#include "cgnszonegridconnectivity.h"
#include "cgnszonebc.h"
#include "cgnsrigidgridmotion.h"
#include "cgnsarbitrarygridmotion.h"
#include "cgnsnodeutil.h"

#include <iostream>
#include <assert.h>

using CGNS::Zone;

GENERATE_D(Zone);

COMMON_NODE_IMPLEMENTATION(Zone);

INIT_IMPLEMENTATION11(Zone, mCoordinates, mIterCoordinates, mSolutions,
	mDiscrete, mElements, mFamilyName, mIterativeData, mConnectivity,
	mZoneBC, mRigidMotion, mArbitraryMotion);

static void writeZoneType(ADF::Node & iZone, std::string const & iType) {
	ADF::Node type=iZone.createChild("ZoneType", "ZoneType_t");
	type.writeData(iType);
}

void CGNS::Private::Zone::ModifyStructured(int iIndexDimension,
	int const * iNumVertices) {
	checkIndex(iIndexDimension, P()->mCellDimension+1, 1);
	for (int i=0; i<iIndexDimension; ++i)
		if (iNumVertices[i]<=0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	mNode.setDataTypeDimensions(ADF::I4, mIndexDimension=iIndexDimension, 3);
	std::vector<int> data(iIndexDimension*3);
	for (int i=0; i<iIndexDimension; ++i) {
		data[i]=mVertices[i]=iNumVertices[i];
		data[i+iIndexDimension]=mCells[i]=iNumVertices[i]-1;
		data[i+2*iIndexDimension]=0;
	}		
	mBoundaryVertices=0;
	mStructured=true;
	mNode.writeData(&data[0]);
	writeZoneType(mNode, "Structured");
}

void CGNS::Private::Zone::ModifyUnstructured(int iNumVertices,
	int iNumCells, int iBoundaryVertices) {
	if (iNumVertices<=0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	checkIndex(iBoundaryVertices, iNumVertices+1);
	mNode.setDataTypeDimensions(ADF::I4, mIndexDimension=1, 3);
	int data[3] = { iNumVertices, iNumCells, iBoundaryVertices };
	mVertices[0]=iNumVertices;
	mCells[0]=iNumCells;
	mBoundaryVertices=iBoundaryVertices;
	mStructured=false;
	mNode.writeData(data);
	writeZoneType(mNode, "Unstructured");
}

void CGNS::Private::Zone::Read(Zone * me) {
//std::cerr << "Zone::Read" << me->mNode.getName() << std::endl;
	for (int i=0; i<MAX_INDEX_DIMENSION; ++i)
		me->mVertices[i]=me->mCells[i]=0;
	checkNode(me->mNode, ADF::I4, 0, 0);
	int index=me->mNode.getDimension(0);
	if (index<1 || index>me->getCellDimension()) me->Emit(Exception::WRONG_DIMENSION);
	me->mIndexDimension=index;
	int size=me->mNode.getDimension(1);
	// in version <=1.05 vertexSizeBoundary is missing
	if (size<2 || size>3) me->Emit(Exception::WRONG_DIMENSION);
	std::vector<int> data(me->mIndexDimension*3);
	me->mNode.readData(&data[0]);
	for (int i=0; i<me->mIndexDimension; ++i) {
		me->mVertices[i]=data[i];
		me->mCells[i]=data[i+me->mIndexDimension];
	}
	
	// use boundary vertex size only for unstructured zones
	if (me->mIndexDimension==1) {
		// in version <=1.05 vertexSizeBoundary is missing
		if (size>2) me->mBoundaryVertices=data[2];
		else me->mBoundaryVertices=0;
	}
	// load ZoneType_t node
	me->mStructured=me->mIndexDimension==me->getCellDimension();
	if (me->mNode.hasChild("ZoneType")) {
		ADF::Node child=me->mNode.getChild("ZoneType");
		if (child.getLabel()=="ZoneType_t") {
			std::string structured;
			child.readData(&structured);
			if (structured=="Structured") me->mStructured=true;
			else if (structured=="Unstructured") me->mStructured=false;
		}
	}
	me->mInitialized|=INIT_MASK_SELF;
}


static void readCoordinates(CGNS::Private::Zone * me) {
	for (ADF::Node::Child_iterator_t i=me->mNode.beginChild(); i!=me->mNode.endChild(); ++i) {
		ADF::Node & child=*i;
		if (child.getLabel()==CGNS::Private::implementation_traits<
				CGNS::Private::GridCoordinates>::Label()) {
			CGNS::Private::GridCoordinates * coor=new CGNS::Private::GridCoordinates(child, me);
			coor->Init();
			if (child.getName()==CGNS::Private::implementation_traits<
					CGNS::Private::GridCoordinates>::Name())
				me->mCoordinates=coor;
			else me->mIterCoordinates.insert(coor);
		}
	}
	me->mInitialized|=CGNS::Private::implementation_traits<CGNS::GridCoordinates>::INIT_MASK;
}

void CGNS::Private::Zone::Read(Zone * me, refcnt_ptr<GridCoordinates> & cDest) {
	assert(&cDest==&me->mCoordinates);
	readCoordinates(me);
}

void CGNS::Private::Zone::Read(Zone * me, child_container<CGNS::GridCoordinates> & cDest) {
	assert(&cDest==&me->mIterCoordinates);
	readCoordinates(me);
}

void CGNS::Private::Zone::Read(Zone * me, std::auto_ptr<std::string> & cDest) {
	assert(&cDest==&me->mFamilyName);
	readFamilyName(me, cDest);	
}

int Zone::getIndexDimension() const {
	NEEDS_THIS();
	return D()->mIndexDimension;
}

bool Zone::isStructured() const {
	NEEDS_THIS();
	return D()->mStructured;
}

int Zone::getBoundaryVertices() const {
	NEEDS_THIS();
	if (isStructured()) Private::Emit(Exception::WRONG_NUM_DIMENSIONS);
	return D()->mBoundaryVertices;
}

int Zone::getTotalVertices() const {
	NEEDS_THIS();
	int vertices=D()->mVertices[0];	
	for (int i=1; i<D()->mIndexDimension; ++i)
		vertices*=D()->mVertices[i];
	return vertices;
}

int Zone::getTotalCells() const {
	NEEDS_THIS();
	int cells=D()->mCells[0];
	for (int i=1; i<D()->mIndexDimension; ++i)
		cells*=D()->mCells[i];
	return cells;
}

void Zone::getVertexDimensions(int * oVertexDimensions) const {
	NEEDS_THIS();
	Private::checkPointer(oVertexDimensions);
	for (int i=0; i<D()->mIndexDimension; ++i)
		oVertexDimensions[i]=D()->mVertices[i];
}

void Zone::getCellDimensions(int * oCellDimensions) const {
	NEEDS_THIS();
	Private::checkPointer(oCellDimensions);
	for (int i=0; i<D()->mIndexDimension; ++i)
		oCellDimensions[i]=D()->mCells[i];
}

int Zone::getVertexDimension(Index iIndex) const {
	NEEDS_THIS();
	Private::checkIndex(iIndex, D()->mIndexDimension+1, INDEX_I);
	return D()->mVertices[iIndex];
}

int Zone::getCellDimension(Index iIndex) const {
	NEEDS_THIS();
	Private::checkIndex(iIndex, D()->mIndexDimension+1, INDEX_I);
	return D()->mCells[iIndex];
}

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(GridCoordinates, GridCoordinates,
	Zone, D()->mCoordinates)

N_CHILDS_IMPLEMENTATION_WRITE(GridCoordinates, GridCoordinates, Zone, D()->mIterCoordinates)

N_CHILDS_IMPLEMENTATION_WRITE(FlowSolution, FlowSolution, Zone, D()->mSolutions)

CHILDS_IMPLEMENTATION_WRITE1(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::GridLocation)
CHILDS_IMPLEMENTATION_WRITE1(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::Rind1D)
CHILDS_IMPLEMENTATION_WRITE1(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::Rind2D)
CHILDS_IMPLEMENTATION_WRITE1(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::Rind3D)
CHILDS_IMPLEMENTATION_WRITE2(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::GridLocation, CGNS::Rind1D)
CHILDS_IMPLEMENTATION_WRITE2(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::GridLocation, CGNS::Rind2D)
CHILDS_IMPLEMENTATION_WRITE2(FlowSolution, FlowSolution, Zone, D()->mSolutions, CGNS::GridLocation, CGNS::Rind3D)

N_CHILDS_IMPLEMENTATION_WRITE(DiscreteData, DiscreteData, Zone, D()->mDiscrete)

N_CHILDS_IMPLEMENTATION(Elements, Elements, Zone, D()->mElements)

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(FamilyName, std::string, Zone, D()->mFamilyName);

std::string Zone::writeFamilyName(std::string const & iFamily) {
	NEEDS(mFamilyName);
	Private::assureChild(D()->mFamilyName, D(), iFamily, "FamilyName", "FamilyName_t").writeData(iFamily);
	return iFamily;
}

CGNS::Elements Zone::writeElements(std::string const & iElementsName,
	CGNS::Elements::ElementType iElementType, int iElementsOnBoundary) {
	NEEDS(mElements);
	Private::validateIndex(iElementType, Elements::NULL_ELEMENT, Elements::USER_ELEMENT);
	Private::Elements * ele=Private::assureChild(D()->mElements, iElementsName, D());
	ele->Modify(iElementType, iElementsOnBoundary);
	return ele;
}

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(ZoneIterativeData, ZoneIterativeData,
	Zone, D()->mIterativeData)

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(ZoneGridConnectivity, ZoneGridConnectivity,
	Zone, D()->mConnectivity)

OPTIONAL_CHILD_IMPLEMENTATION_WRITE(ZoneBC, ZoneBC,
	Zone, D()->mZoneBC)

N_CHILDS_IMPLEMENTATION_WRITE1(RigidGridMotion, RigidGridMotion, Zone,
	D()->mRigidMotion, CGNS::RigidGridMotion::RigidGridMotionType)

N_CHILDS_IMPLEMENTATION_WRITE1(ArbitraryGridMotion, ArbitraryGridMotion, Zone,
	D()->mArbitraryMotion, CGNS::ArbitraryGridMotion::ArbitraryGridMotionType)
