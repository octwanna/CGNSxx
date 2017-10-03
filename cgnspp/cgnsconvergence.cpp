/***************************************************************************
                          cgnsconvergence.cpp  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsconvergence.h"

#include "cgnsconverged.h"

using CGNS::ConvergenceHistory;

GENERATE_D(ConvergenceHistory);

COMMON_NODE_IMPLEMENTATION(ConvergenceHistory);

INIT_IMPLEMENTATION0(ConvergenceHistory);

void CGNS::Private::ConvergenceHistory::Read(ConvergenceHistory * me) {
	checkNode(me->mNode, ADF::I4, 1);
	me->mNode.readData(&me->mNumIterations);
	if (me->mNumIterations<0) me->Emit(Exception::ILLEGAL_DATA);
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::ConvergenceHistory::Modify(int iNumIterations) {
	if (iNumIterations<0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	mNode.setDataTypeDimensions(ADF::I4, 1);
	mNode.writeData(&iNumIterations);
	mNumIterations=iNumIterations;
}


int CGNS::ConvergenceHistory::getIterations() const {
	NEEDS_THIS();
	return D()->mNumIterations;
}

bool CGNS::ConvergenceHistory::hasNormDefinitions() const {
	return hasDescriptor("NormDefinitions");
}

std::string CGNS::ConvergenceHistory::getNormDefinitions() const {
	return getDescriptorText("NormDefinitions");
}

std::string CGNS::ConvergenceHistory::writeNormDefinitions(std::string const & iDefinition) {
	return writeDescriptor("NormDefinitions", iDefinition);
}

void CGNS::ConvergenceHistory::deleteNormDefinitions() {
	deleteDescriptor("NormDefinitions");
}

