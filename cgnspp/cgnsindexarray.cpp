/***************************************************************************
                          cgnsindexarray.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsindexarray.h"

#include "cgnsutil.h"

#include "cgnsparented.h"

using CGNS::IndexArray;

GENERATE_D(IndexArray);

COMMON_SIMPLE_NODE_IMPLEMENTATION(IndexArray);

INIT_IMPLEMENTATION0(IndexArray);

void CGNS::Private::IndexArray::Read(IndexArray * me) {
//	checkNode(me->mNode, ADF::I4,0,0);
    // HACK! better handle it in CHILD_IMPLEMENTATION_WRITE or something similar
	if (me->mNode.getDataType()==ADF::MT)
		me->mNode.setDataTypeDimensions(ADF::I4, 1);
	else if (me->mNode.getDataType()!=ADF::I4) me->Emit(Exception::WRONG_DATATYPE);
    int numDim=me->mNode.getNumDimensions();
	int dim=me->mNode.getDimension(0);
//std::cerr << "IndexArray: " << numDim << ": " << dim << std::endl;
	if (numDim==1) me->mLength=dim;
	else {
		if (dim<1 || dim>3) me->Emit(Exception::WRONG_DIMENSION);
		me->mLength=me->mNode.getDimension(1);
	}
}

void CGNS::Private::IndexArray::Modify(int iIndexDimension) {
    if (iIndexDimension) {
		mNode.setDataTypeDimensions(ADF::I4, iIndexDimension, 1);
	}
	else {
		mNode.setDataTypeDimensions(ADF::I4, 1);
	}
	int data=0;
	mNode.writeData(&data);
}

int IndexArray::getLength() const {
	NEEDS_THIS();
	return D()->mLength;
}

void IndexArray::setLength(int iLength) {
	if (iLength<=0) Private::Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	NEEDS_THIS();
	D()->mNode.setDataTypeDimensions(ADF::I4,
		D()->mNode.getDimension(0), D()->mLength=iLength);
}
	
void IndexArray::readData(CGNS::Index1D * oData) const {
	NEEDS_THIS();
	Private::checkNode(D()->mNode, ADF::I4, 1, D()->mLength);
	D()->mNode.readData(&oData->mI);
}

void IndexArray::readData(CGNS::Index2D * oData) const {
	NEEDS_THIS();
	Private::checkNode(D()->mNode, ADF::I4, 2, D()->mLength);
	D()->mNode.readData(&oData->mI);
}

void IndexArray::readData(CGNS::Index3D * oData) const {
	NEEDS_THIS();
	Private::checkNode(D()->mNode, ADF::I4, 3, D()->mLength);
	D()->mNode.readData(&oData->mI);
}
	
void IndexArray::readData(int * oData) const {
	NEEDS_THIS();
	Private::checkNode(D()->mNode, ADF::I4, D()->mLength);
	D()->mNode.readData(oData);
}

void IndexArray::writeData(CGNS::Index1D const * iData) {
	NEEDS_THIS();
	D()->mNode.setDataTypeDimensions(ADF::I4, 1, D()->mLength);
	D()->mNode.writeData(&iData->mI);
}

void IndexArray::writeData(CGNS::Index2D const * iData) {
	NEEDS_THIS();
	D()->mNode.setDataTypeDimensions(ADF::I4, 2, D()->mLength);
	D()->mNode.writeData(&iData->mI);
}

void IndexArray::writeData(CGNS::Index3D const * iData) {
	NEEDS_THIS();
	D()->mNode.setDataTypeDimensions(ADF::I4, 3, D()->mLength);
	D()->mNode.writeData(&iData->mI);
}

void IndexArray::writeData(int const * iData) {
	NEEDS_THIS();
	D()->mNode.setDataTypeDimensions(ADF::I4, D()->mLength);
	D()->mNode.writeData(iData);
}
