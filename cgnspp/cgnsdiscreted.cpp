/***************************************************************************
                          cgnsdiscreted.cpp  -  description
                             -------------------
    begin                : Wed Mar 14 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsdiscreted.h"

#include "cgnsdataarray.h"

#include "cgnsnodeutil.h"

#include <iostream>
#include <assert.h>

using CGNS::Discreted;

GENERATE_D(Discreted);

COMMON_NODE_IMPLEMENTATION(Discreted);

INIT_IMPLEMENTATION0(Discreted);

void CGNS::Private::Discreted::Read(Discreted * me) {
	me->mLocation=VERTEX;
	readStringChild(me->mNode, me->mLocation);
	me->mInitialized|=INIT_MASK_SELF;
}


void CGNS::Private::Discreted::Modify(GridLocation iLocation) {
	checkIndex(iLocation, USER_LOCATION+1, NULL_LOCATION);
	ADF::Node loc=mNode.createChild(implementation_traits<GridLocation>::Name(),
		implementation_traits<GridLocation>::Label());
	loc.writeData(asString(iLocation));
	mLocation=iLocation;
}

void CGNS::Private::Discreted::Modify(GridLocation iLoc, Rind1D iRind) {
	Modify(iLoc);
	Modify(iRind);
}

void CGNS::Private::Discreted::Modify(GridLocation iLoc, Rind2D iRind) {
	Modify(iLoc);
	Modify(iRind);
}

void CGNS::Private::Discreted::Modify(GridLocation iLoc, Rind3D iRind) {
	Modify(iLoc);
	Modify(iRind);
}

SIMPLE_CHILD_IMPLEMENTATION_WRITE(Location, CGNS::GridLocation,
	Discreted, D()->mLocation);

int Discreted::getNumFields() const {
	int num=0;
	for (int i=POTENTIAL; i<=REYNOLDS_STRESS_ZZ; ++i)
		if (D()->hasArray(static_cast<DataArray::Quantity>(i))) ++num;
	return num;
}

Discreted::Field_iterator_t Discreted::beginField() const {
	NEEDS(mArrays);
	child_container<DataArray>::const_iterator  iter(D()->mArrays.begin());
	return Field_iterator_t(*reinterpret_cast<void **>(&iter));
}

Discreted::Field_iterator_t Discreted::endField() const {
	NEEDS(mArrays);
	child_container<DataArray>::const_iterator  iter(D()->mArrays.end());
	return Field_iterator_t(*reinterpret_cast<void **>(&iter));
}
	
void Discreted::deleteField(FieldType iField) {
	D()->deleteArray(static_cast<DataArray::Quantity>(iField+0));
}

void Discreted::deleteField(std::string const & iName) {
	DataArray::Quantity q;
	Private::convert(iName, &q);
	if (D()->hasArray(q)) D()->deleteArray(q);
}

/*
Discreted::FieldType Discreted::GetFieldType(int iNth) const {
	if (iNth<0) Emit(Exception::INDEX_TOO_LOW);
	int num=0;
	for (int i=POTENTIAL; i<=REYNOLDS_STRESS_ZZ; ++i)
		if (D()->hasArray(static_cast<DataArray::Quantity>(i))) {
			if (num==iNth) return static_cast<FieldType>(i);
			++num;
		}
	Emit(Exception::INDEX_TOO_HIGH);
}
*/

bool Discreted::hasField(FieldType iField) const {
	return D()->hasArray(static_cast<DataArray::Quantity>(iField+0));
}

CGNS::DataArray Discreted::getField(FieldType iField) const {
	return D()->getArray(static_cast<DataArray::Quantity>(iField+0));
}

void Discreted::readData(FieldType iField, double * oField) const {
	DataArray field=getField(iField);
	field.readData(oField);
}

void Discreted::readData(FieldType iField, float * oField) const {
	DataArray field=getField(iField);
	field.readData(oField);
}

CGNS::DataArray Discreted::writeField(FieldType iQuantity, CGNS::DataArray::DataType iType) {
	NEEDS(mArrays);
	DataArray field=D()->writeArray(static_cast<DataArray::Quantity>(iQuantity+0));
	Private::Zone * zone=D()->getZone();
	int dims[MAX_INDEX_DIMENSION];
	int index=zone->mIndexDimension;
	for (int i=0; i<index; ++i) dims[i]=zone->mVertices[i];
	field.setDataTypeDimensions(iType, index, D()->adjustRind(dims));
	return field;
}

void Discreted::writeData(FieldType iField, double const * iFields) {
	DataArray field=writeField(iField);
	field.writeData(iFields);
}

void Discreted::writeData(FieldType iField, float const * iFields) {
	DataArray field=writeField(iField);
	field.writeData(iFields);
}
