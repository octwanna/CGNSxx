/***************************************************************************
                          cgnsmodel.cpp  -  description
                             -------------------
    begin                : Mon May 27 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsmodel.h"

#include "cgnszone.h"

#include "cgnsnodeutil.h"

#include <assert.h>

#define IMPLEMENT_MODEL(model)            \
using CGNS::model;                        \
GENERATE_D(model);                        \
COMMON_SIMPLE_NODE_IMPLEMENTATION(model); \
INIT_IMPLEMENTATION0(model);              \
CGNS::model::model##Type CGNS::model::getType() const { \
	return D()->mType; }                  \
void CGNS::Private::model::Modify(CGNS::model::model##Type iType) { \
	writeADF(mNode, iType);               \
	mType=iType;                          \
}                                         \
void CGNS::Private::model::Read(model * me) { \
	readADF(me->mNode, me->mType);        \
	me->mInitialized|=INIT_MASK_SELF;     \
}                                         \

static bool getDiffusionTerm(CGNS::Private::Node const * me, int iIndexDim,
	std::auto_ptr<CGNS::Private::DiffusionModel_t> const & cDest, CGNS::DiffusionIndex iIndex) {
	CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::KI_INDEX);
	switch (iIndexDim) {
	case 1: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::II_INDEX); break;
	case 2: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::IJ_INDEX);
		if (iIndex==CGNS::KK_INDEX)
			CGNS::Private::Emit(CGNS::Exception::INDEX_TOO_HIGH);
	}
	return cDest->mActive[iIndex];
}		

static void writeDiffusionTerm(CGNS::Private::Node * me, int iIndexDim,
	std::auto_ptr<CGNS::Private::DiffusionModel_t> & cDest,
	CGNS::DiffusionIndex iIndex, bool iIsModeled) {
	CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::KI_INDEX);
	int index=3;
	switch (iIndexDim) {
	case 0: break;
	case 1: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::II_INDEX); break;
	case 2: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::IJ_INDEX);
		if (iIndex==CGNS::KK_INDEX) CGNS::Private::Emit(CGNS::Exception::INDEX_TOO_HIGH);
		if (iIndex==CGNS::IJ_INDEX) iIndex=CGNS::KK_INDEX;
		break;
	case 3: break;
	default: EMIT_INTERNAL_ERROR(); index=3;
	}
	ADF::Node diff(CGNS::Private::assureChild(cDest, me, CGNS::Private::DiffusionModel_t(iIndex)));
	cDest->mActive[iIndex]=iIsModeled;
	diff.setDataTypeDimensions(ADF::I4, index*(index+1)/2);
	diff.writeData(&cDest->mActive[0]);
}

static void writeThinLayerDiffusion(CGNS::Private::Node * me, int iIndexDim,
	std::auto_ptr<CGNS::Private::DiffusionModel_t> & cDest, CGNS::DiffusionIndex iIndex) {
	switch (iIndexDim) {
	case 0: break;
	case 1: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::II_INDEX); break;
	case 2: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::JJ_INDEX); break;
	case 3: CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::KK_INDEX); break;
	default: EMIT_INTERNAL_ERROR();
		CGNS::Private::checkIndex(iIndex, CGNS::II_INDEX, CGNS::KK_INDEX);
	}
	if (cDest.get()) {
		*cDest=CGNS::Private::DiffusionModel_t(iIndex);
	}
	writeDiffusionTerm(me, iIndexDim, cDest, iIndex, true);
}

static void writeFullDiffusion(CGNS::Private::Node * me, int iIndexDim,
	std::auto_ptr<CGNS::Private::DiffusionModel_t> & cDest) {
	int index=iIndexDim ? iIndexDim : 3;
	if (index<1 || index>MAX_INDEX_DIMENSION) EMIT_INTERNAL_ERROR();
	ADF::Node diff(CGNS::Private::assureChild(cDest, me,
		CGNS::Private::DiffusionModel_t(CGNS::II_INDEX)));
	for (int i=0; i<index*(index+1)/2; ++i)
		cDest->mActive[i]=1;
	diff.setDataTypeDimensions(ADF::I4, index*(index+1)/2);
	diff.writeData(&cDest->mActive[0]);
}
	

#define IMPLEMENT_DIFFUSION(model)                    \
bool model::hasDiffusionModel() const {               \
	NEEDS(mDiffusion);                                \
	return D()->mDiffusion.get()!=0;                  \
}                                                     \
bool model::getDiffusionTerm(DiffusionIndex iIndex) const { \
	NEEDS(mDiffusion);                                \
	return ::getDiffusionTerm(D(),                    \
		D()->getZone() ? D()->getZone()->getIndexDimension() : 0, \
		D()->mDiffusion, iIndex);                     \
}                                                     \
void model::writeDiffusionTerm(DiffusionIndex iIndex, bool iIsModeled) { \
	NEEDS(mDiffusion);                                \
	::writeDiffusionTerm(D(),                         \
		D()->getZone() ? D()->getZone()->getIndexDimension() : 0, \
		D()->mDiffusion, iIndex, iIsModeled);         \
}                                                     \
void model::writeThinLayerDiffusion(DiffusionIndex iIndex) { \
	NEEDS(mDiffusion);                                \
	::writeThinLayerDiffusion(D(),                    \
		D()->getZone() ? D()->getZone()->getIndexDimension() : 0, \
		D()->mDiffusion, iIndex);                     \
}                                                     \
void model::writeFullDiffusion() {                    \
	NEEDS(mDiffusion);                                \
	::writeFullDiffusion(D(),                         \
		D()->getZone() ? D()->getZone()->getIndexDimension() : 0, \
		D()->mDiffusion);                             \
}                                                     \
void model::deleteDiffusionModel() {                  \
	NEEDS(mDiffusion);                                \
	D()->mNode.deleteChild("DiffusionModel");         \
	D()->mDiffusion.release();                        \
}
	
IMPLEMENT_MODEL(GoverningEquations);
IMPLEMENT_DIFFUSION(CGNS::GoverningEquations);
void CGNS::Private::GoverningEquations::Read(GoverningEquations * me,
	std::auto_ptr<DiffusionModel_t> & cDest) {
	assert(&cDest==&me->mDiffusion);
	readDiffusionModel(me, me->getZone(), cDest);
}


IMPLEMENT_MODEL(GasModel);

IMPLEMENT_MODEL(ViscosityModel);

IMPLEMENT_MODEL(ThermalConductivityModel);

IMPLEMENT_MODEL(TurbulenceClosure);

IMPLEMENT_MODEL(TurbulenceModel);
IMPLEMENT_DIFFUSION(CGNS::TurbulenceModel);
void CGNS::Private::TurbulenceModel::Read(TurbulenceModel * me,
	std::auto_ptr<DiffusionModel_t> & cDest) {
	assert(&cDest==&me->mDiffusion);
	readDiffusionModel(me, me->getZone(), cDest);
}

IMPLEMENT_MODEL(ThermalRelaxationModel);

IMPLEMENT_MODEL(ChemicalKineticsModel);

