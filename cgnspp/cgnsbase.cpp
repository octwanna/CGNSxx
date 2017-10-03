/***************************************************************************
                          cgnsbase.cpp  -  description
                             -------------------
    begin                : Wed Mar 14 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsbase.h"

#include "cgnszone.h"
#include "cgnsfamily.h"
#include "cgnsbaseiterativedata.h"

#include <iostream>
#include <assert.h>

using CGNS::Base;

GENERATE_D(Base);

COMMON_NODE_IMPLEMENTATION(Base);

INIT_IMPLEMENTATION4(Base, mZones, mFamilies, mIterativeData, mSimulationType);

void CGNS::Private::Base::Read(Base * me) {
#ifdef CGNS_Log
	std::cerr << "Base::Read" << me->mNode.getName() << std::endl;
#endif
	// can't use checkNode, since for version <=1.05 there is only one data field
	// checkNode(me->mNode, ADF::I4, 2);
	if (me->mNode.getDataType()!=ADF::I4) me->Emit(Exception::WRONG_DATATYPE);
	if (me->mNode.getNumDimensions()!=1) me->Emit(Exception::WRONG_NUM_DIMENSIONS);
	int dim=me->mNode.getDimension(0);
	if (dim!=1 && dim!=2) me->Emit(Exception::WRONG_DIMENSION);
	int data[2];
	me->mNode.readData(data);
	// dim==1 means version <=1.05, and then physicaldimension==celldimension==indexdimension
	if (dim==1) data[1]=data[0];
	me->mCellDimension=data[0];
	me->mPhysicalDimension=data[1];
	// plausibility test
	if (me->mPhysicalDimension<1 || me->mPhysicalDimension>3)
		me->Emit(Exception::ILLEGAL_DATA);
	if (me->mCellDimension<1 || me->mCellDimension>me->mPhysicalDimension)
		me->Emit(Exception::ILLEGAL_DATA);
	
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::Base::Modify(int iCellDimension, int iPhysicalDimension) {
	checkIndex(iPhysicalDimension, MAX_PHYSICAL_DIMENSION+1, 1);
	checkIndex(iCellDimension, iPhysicalDimension+1, 1);
	mNode.setDataTypeDimensions(ADF::I4, 2);
	int data[2] = { iCellDimension, iPhysicalDimension };
	mNode.writeData(data);
	mCellDimension=iCellDimension;
	mPhysicalDimension=iPhysicalDimension;	
}

int Base::getCellDimension() const {
	NEEDS_THIS();
	return D()->mCellDimension;
}

int Base::getPhysicalDimension() const {
	NEEDS_THIS();
	return D()->mPhysicalDimension;
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(SimulationType, Base::SimulationType,
	Base, D()->mSimulationType);

Base::SimulationType Base::writeSimulationType(SimulationType iType) {
	NEEDS(mSimulationType);
	Private::assertIndex(iType, NULL_SIMULATION, USER_SIMULATION, Exception::NO_SIMULATIONTYPE);
/*	char const * name=Private::implementation_traits<SimulationType>::Name();
	if (!mImpl->mNode.hasChild(name))
		mImpl->mNode.createChild(name, Private::implementation_traits<SimulationType>::Label());
	if (!D()->mSimulationType.get()) D()->mSimulationType.reset(new SimulationType(iType));
	else *D()->mSimulationType=iType;
	ADF::Node n(mImpl->mNode.getChild(name));
	n.writeData(asString(iType));
*/	Private::assureChild(D()->mSimulationType, D(), iType).writeData(asString(iType));
	return iType;
	
}

OPTIONAL_CHILD_IMPLEMENTATION(BaseIterativeData, BaseIterativeData,
	Base, D()->mIterativeData);

CGNS::BaseIterativeData Base::writeBaseIterativeData(int iNumberOfSteps) {
	NEEDS(mIterativeData);
	Private::BaseIterativeData * data=Private::assureChild(D()->mIterativeData,
		"BaseIterativeData", D());
	data->Modify(iNumberOfSteps);
	return data;
}

N_CHILDS_IMPLEMENTATION_WRITE(Family, Family, Base, D()->mFamilies);

N_CHILDS_IMPLEMENTATION(Zone, Zone, Base, D()->mZones);

CGNS::Zone Base::writeStructured1DZone(std::string const & iZoneName,
	int iVertexI) {
	NEEDS(mZones);
	Private::Zone * zone=Private::assureChild(D()->mZones, iZoneName, D());
	zone->ModifyStructured(1, &iVertexI);
	return zone;
}
	
CGNS::Zone Base::writeStructured2DZone(std::string const & iZoneName,
	int iVertexI, int iVertexJ) {
	NEEDS(mZones);
	int vertices[2]={ iVertexI, iVertexJ };
	Private::Zone * zone=Private::assureChild(D()->mZones, iZoneName, D());
	zone->ModifyStructured(2, vertices);
	return zone;
}
	
CGNS::Zone Base::writeStructured3DZone(std::string const & iZoneName,
	int iVertexI, int iVertexJ, int iVertexK) {
	NEEDS(mZones);
	int vertices[3]={ iVertexI, iVertexJ, iVertexK };
	Private::Zone * zone=Private::assureChild(D()->mZones, iZoneName, D());
	zone->ModifyStructured(3, vertices);
	return zone;
}
	
CGNS::Zone Base::writeUnstructuredZone(std::string const & iZoneName,
	int iNumVertices, int iNumCells, int iNumBoundaryVertices) {
	NEEDS(mZones);
	Private::Zone * zone=Private::assureChild(D()->mZones, iZoneName, D());
	zone->ModifyUnstructured(iNumVertices, iNumCells, iNumBoundaryVertices);
	return zone;
}		
