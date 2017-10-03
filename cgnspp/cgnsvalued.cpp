/***************************************************************************
                          cgnsvalued.cpp  -  description
                             -------------------
    begin                : Thu Sep 20 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsvalued.h"

#include "cgnsdataarray.h"

#include <assert.h>

using CGNS::Valued;

GENERATE_D(Valued);

COMMON_NODE_IMPLEMENTATION(Valued);

INIT_IMPLEMENTATION0(Valued);

void CGNS::Private::Valued::Needs(Valued * me, child_container<double> & cDest) {
	assert(&cDest==&me->mValues);
	if (me->mValues.size()>0) return;
	Read(me, cDest);
}

static void handleValue(CGNS::DataArray const & iArray, CGNS::Private::Valued::values_t & cDest) {
	if (iArray.getNumDimensions()!=1) return;
	if (iArray.getDimension(0)!=1) return;
	CGNS::DataArray::DataType type=iArray.getDataType();
	switch (type) {
	case CGNS::DataArray::FLOAT: {
		float data; iArray.readData(&data); cDest.insert(data, iArray.getName()); break; }
	case CGNS::DataArray::DOUBLE: {
		double data; iArray.readData(&data); cDest.insert(data, iArray.getName()); break; }
	default: return;
	}
}

void CGNS::Private::Valued::Read(Valued * me, child_container<double> & cDest) {
	assert(&cDest==&me->mValues);
	me->Needs(me, me->mArrays);
	// initialize our coordinate array
	for (arrays_t::const_iterator i=me->mArrays.begin(); i!=me->mArrays.end(); ++i)
		handleValue(i->second, me->mValues);
	/// \todo: distinguish mCoordinates and mArrays!!
	me->mInitialized|=implementation_traits<double>::INIT_MASK;
}

template struct CGNS::iterator<double>;

N_CHILDS_IMPLEMENTATION_NOGET(Value, double, Valued, D()->mValues);
	
double Valued::getValue(std::string const & iQuantity) const {
	return Private::getChild(D()->mValues, iQuantity);
}

bool Valued::hasValue(CGNS::DataArray::Quantity iQuantity) const {
	return hasValue(asString(iQuantity));
}

double Valued::getValue(CGNS::DataArray::Quantity iQuantity) const {
	return getValue(asString(iQuantity));
}

double Valued::writeValue(std::string const &  iQuantity, double iValue) {
	DataArray::Quantity q;
	Private::convert(iQuantity, &q);
	return writeValue(q, iValue);
}

double Valued::writeValue(CGNS::DataArray::Quantity iQuantity, double iValue) {
	NEEDS(mValues);
	DataArray array=D()->writeArray(iQuantity);
	array.setDataTypeDimensions(DataArray::DOUBLE, 1);
	array.writeData(&iValue);
	D()->mValues.insert(iValue, asString(iQuantity));
	return iValue;
}

void Valued::deleteValue(std::string const &  iQuantity) {
	DataArray::Quantity q;
	Private::convert(iQuantity, &q);
	deleteValue(q);
}

void Valued::deleteValue(CGNS::DataArray::Quantity iQuantity) {
	NEEDS(mValues);
	D()->mValues.erase(asString(iQuantity));
	D()->deleteArray(iQuantity);
}

