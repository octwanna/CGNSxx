/***************************************************************************
                          cgnsgridcoordinates.cpp  -  description
                             -------------------
    begin                : Wed Mar 14 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsgridcoordinates.h"

#include "cgnsdataarray.h"

#include <iostream>
#include <assert.h>

using CGNS::GridCoordinates;

GENERATE_D(GridCoordinates);

COMMON_NODE_IMPLEMENTATION(GridCoordinates);

INIT_IMPLEMENTATION1(GridCoordinates, mCoordinates);

static bool isCoordinate(CGNS::DataArray::Quantity iQuantity) {
	if (iQuantity<CGNS::DataArray::COORDINATE_X) return false;
	if (iQuantity>CGNS::DataArray::COORDINATE_ZETA) return false;
	return true;
}

void CGNS::Private::GridCoordinates::Needs(GridCoordinates * me,
	child_container<CGNS::DataArray> & cDest) {
	if (&cDest!=&me->mCoordinates) {
		Gridded::Needs(me, cDest);
		return;
	}
	if (me->mCoordinates.size()>0) return;
	Read(me, cDest);
}

void CGNS::Private::GridCoordinates::Read(GridCoordinates * me,
	child_container<CGNS::DataArray> & cDest) {
	if (&cDest!=&me->mCoordinates)
		Gridded::Read(me, cDest);
	me->Needs(me, me->mArrays);
	// initialize our coordinate array
	for (arrays_t::const_iterator i=me->mArrays.begin(); i!=me->mArrays.end(); ++i)
		if (isCoordinate(i->second.getQuantity())) me->mCoordinates.insert(i->second);
	/// \todo: distinguish mCoordinates and mArrays!!
	me->mInitialized|=implementation_traits<DataArray>::INIT_MASK;
}

N_CHILDS_IMPLEMENTATION_NOITER(Coordinate, DataArray, GridCoordinates, D()->mCoordinates);

bool GridCoordinates::hasCoordinate(CoordinateType iCoordinate) const {
	NEEDS(mCoordinates);
	return D()->hasArray(static_cast<DataArray::Quantity>(iCoordinate+0));
}

CGNS::DataArray GridCoordinates::getCoordinate(CoordinateType iCoordinate) const {
	NEEDS(mCoordinates);
	return D()->getArray(static_cast<DataArray::Quantity>(iCoordinate+0));
}

void GridCoordinates::readData(CoordinateType iCoordinate, double * oCoordinates) const {
	DataArray coord=getCoordinate(iCoordinate);
	coord.readData(oCoordinates);
}

void GridCoordinates::readData(CoordinateType iCoordinate, float * oCoordinates) const {
	DataArray coord=getCoordinate(iCoordinate);
	coord.readData(oCoordinates);
}

/*
CGNS::DataArray GridCoordinates::writeCoordinate(CoordinateType iCoordinate) {
	NEEDS(mCoordinates);
	DataArray coordinate=D()->writeArray(static_cast<DataArray::Quantity>(iCoordinate+0));
	Private::Zone * zone=D()->getZone();
	int dims[MAX_INDEX_DIMENSION];
	int index=zone->mIndexDimension;
	for (int i=0; i<index; ++i) dims[i]=zone->mVertices[i];
	coordinate.setDimensions(index, D()->adjustRind(dims));
	return coordinate;
}
*/
CGNS::DataArray GridCoordinates::writeCoordinate(CoordinateType iCoordinate,
	CGNS::DataArray::DataType iType) {
	NEEDS(mCoordinates);
	DataArray coordinate=D()->writeArray(static_cast<DataArray::Quantity>(iCoordinate+0));
	Private::Zone * zone=D()->getZone();
	int dims[MAX_INDEX_DIMENSION];
	int index=zone->mIndexDimension;
	for (int i=0; i<index; ++i) dims[i]=zone->mVertices[i];
	coordinate.setDataTypeDimensions(iType, index, D()->adjustRind(dims));
	return coordinate;
}

void GridCoordinates::writeData(CoordinateType iCoordinate, double const * iCoordinates) {
	DataArray coord=writeCoordinate(iCoordinate, DataArray::DOUBLE);
	coord.writeData(iCoordinates);
}

void GridCoordinates::writeData(CoordinateType iCoordinate, float const * iCoordinates) {
	DataArray coord=writeCoordinate(iCoordinate, DataArray::FLOAT);
	coord.writeData(iCoordinates);
}
