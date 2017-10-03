/***************************************************************************
                          cgnsarrayed.cpp  -  description
                             -------------------
    begin                : Tue Mar 27 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsarrayed.h"

#include "cgnsdataarray.h"
#include "cgnsbroker.h"

#include <iostream>
#include <assert.h>

using CGNS::Arrayed;

GENERATE_D(Arrayed);

COMMON_NODE_IMPLEMENTATION(Arrayed);

INIT_IMPLEMENTATION1(Arrayed, mArrays);

bool CGNS::Private::Arrayed::hasArray(std::string const & iName) const {
	Needs(this, mArrays);
	return mArrays.has(iName);
}

bool CGNS::Private::Arrayed::hasArray(CGNS::DataArray::Quantity iQuantity) const {
	return hasArray(asString(iQuantity));
}

CGNS::Private::DataArray * CGNS::Private::Arrayed::getArray(std::string const & iName) const {
	Needs(this, mArrays);
	return static_cast<DataArray *>(const_cast<Node *>(
		Broker::GetImplementation(mArrays.find(iName))));
}

CGNS::DataArray CGNS::Private::Arrayed::getArray(CGNS::DataArray::Quantity iQuantity) const {
	return getArray(asString(iQuantity));
}

CGNS::Private::DataArray * CGNS::Private::Arrayed::writeArray(std::string const & iName) {
	Needs(this, mArrays);
	return assureChild(mArrays, iName, this);
}

CGNS::DataArray CGNS::Private::Arrayed::writeArray(CGNS::DataArray::Quantity iQuantity) {
	Needs(this, mArrays);
	DataArray * impl=assureChild(mArrays, asString(iQuantity), this);
	impl->Modify(iQuantity);
	return impl;
}

void CGNS::Private::Arrayed::deleteArray(std::string const & iName) {
	Needs(this, mArrays);
	if (!hasArray(iName)) return;
	mArrays.erase(iName);
	mNode.deleteChild(iName);
}

void CGNS::Private::Arrayed::deleteArray(CGNS::DataArray::Quantity iQuantity) {
	deleteArray(asString(iQuantity));
}

N_CHILDS_IMPLEMENTATION_WRITE(DataArray, DataArray, Arrayed, D()->mArrays);
	
bool Arrayed::hasDataArray(CGNS::DataArray::Quantity iQuantity) const {
	return D()->hasArray(iQuantity);
}

CGNS::DataArray Arrayed::getDataArray(CGNS::DataArray::Quantity iQuantity) const {
	return D()->getArray(iQuantity);
}

CGNS::DataArray Arrayed::writeDataArray(CGNS::DataArray::Quantity iQuantity) {
	return D()->writeArray(iQuantity);
}

void Arrayed::deleteDataArray(CGNS::DataArray::Quantity iQuantity) {
	D()->deleteArray(iQuantity);
}

void Arrayed::deleteDataArray(std::string const & iQuantity) {
	DataArray::Quantity q;
	Private::convert(iQuantity, &q);
	D()->deleteArray(q);
}
