/***************************************************************************
                          cgnszoneiterativedata.cpp  -  description
                             -------------------
    begin                : Tue Oct 23 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnszoneiterativedata.h"

#include "cgnsdataarray.h"

using CGNS::ZoneIterativeData;

GENERATE_D(ZoneIterativeData);

COMMON_NODE_IMPLEMENTATION(ZoneIterativeData);

INIT_IMPLEMENTATION0(ZoneIterativeData);

void CGNS::Private::ZoneIterativeData::Needs(ZoneIterativeData * me, refcnt_ptr<DataArray> &cDest) {
	Read(me, cDest);
}

void CGNS::Private::ZoneIterativeData::Read(ZoneIterativeData * me, refcnt_ptr<DataArray> &cDest) {
	Needs(me, me->mArrays);
#define FETCH_ARRAY(name)                          \
	if (&cDest==&me->m##name)                      \
		if (me->hasArray(#name)) {                 \
			if (!me->m##name)                      \
				me->m##name.reset(me->getArray(#name)); \
		} else me->m##name.release()
	FETCH_ARRAY(RigidGridMotionPointers);
	FETCH_ARRAY(ArbitraryGridMotionPointers);
	FETCH_ARRAY(GridCoordinatesPointers);
	FETCH_ARRAY(FlowSolutionPointers);
#undef FETCH_ARRAY	
}

int CGNS::Private::ZoneIterativeData::getSteps() const {
	Zone const * zone=P();
	if (!zone) return 0;
	Base const * base=zone->getBase();
	if (!base) return 0;
	CGNS::Base realBase(const_cast<Base *>(base));
	if (!realBase.hasBaseIterativeData()) return 0;
	return realBase.getBaseIterativeData().getNumberOfSteps();
}

static void expandArray(CGNS::Private::DataArray * iArray, int iSteps) {
	if (!iArray) return;
	int dim=iArray->mNode.getDimension(1);
	if (dim==iSteps) return;
	CGNS::Private::checkNode(iArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0);
	std::vector<char> data(std::max(iSteps, dim)*ADF::NAME_LENGTH);
	iArray->mNode.readData(&data[0]);
	for (int i=dim; i<iSteps; ++i)
		STRNCPY(&data[i*ADF::NAME_LENGTH],
			"Null                            ", ADF::NAME_LENGTH);
	iArray->mNode.setDimensions(ADF::NAME_LENGTH, iSteps);
	iArray->mNode.writeData(&data[0]);
}

void CGNS::Private::ZoneIterativeData::updateSteps() {
	int numSteps=getSteps();
	expandArray(mRigidGridMotionPointers, numSteps);
	expandArray(mArbitraryGridMotionPointers, numSteps);
	expandArray(mGridCoordinatesPointers, numSteps);
	expandArray(mFlowSolutionPointers, numSteps);
}

void CGNS::Private::ZoneIterativeData::getPointer(refcnt_ptr<DataArray> const & iArray,
	int iStep, std::string * oPointer) const {
	int numSteps=getSteps();
	Private::checkIndex(iStep, numSteps);
	Needs(this, iArray);
	checkNode(iArray->mNode, ADF::C1, ADF::NAME_LENGTH, numSteps);
	int start=iStep*ADF::NAME_LENGTH;
	ADF::Range range(start, start+ADF::NAME_LENGTH);
	char name[ADF::NAME_LENGTH+1];
	iArray->mNode.readData(range, name);
	name[ADF::NAME_LENGTH]=0;
	// strip trailing spaces
	char * cp=name+ADF::NAME_LENGTH-1;
	while (cp>name && *cp==' ') *cp--=0;
	*oPointer=name;
}

void CGNS::Private::ZoneIterativeData::writePointer(refcnt_ptr<DataArray> & cArray,
	int iStep, std::string const & iPointer, std::string const & iName) {
	int ptrSize=iPointer.size();
	if (ptrSize>ADF::NAME_LENGTH) Emit(Exception::STRING_TOO_LONG);
	updateSteps();
	int numSteps=getSteps();
	Needs(this, cArray);
	if (!cArray) {
		cArray.reset(writeArray(iName));
		cArray->mNode.setDataTypeDimensions(ADF::C1, ADF::NAME_LENGTH, numSteps);
	}
	checkNode(cArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0, numSteps);
	
	int start=iStep*ADF::NAME_LENGTH;
	ADF::Range range(start, start+ADF::NAME_LENGTH);
	char name[ADF::NAME_LENGTH+1];
	STRNCPY(name, iPointer.c_str(), ADF::NAME_LENGTH);
	// generate trailing spaces
	for (char * cp=name+ptrSize; cp<=name+ADF::NAME_LENGTH; ++cp) *cp=0;
	cArray->mNode.writeData(range, name);
}

#define GET_WRITE_POINTERS(name, type)                           \
std::string ZoneIterativeData::get##name##Name(int iStep) const {\
	std::string n;                                               \
	D()->getPointer(D()->m##name##Pointers, iStep, &n);          \
	return n;                                                    \
}                                                                \
CGNS::type ZoneIterativeData::get##name(int iStep) const {       \
	return Zone(const_cast<Private::Zone *>(D()->P()))           \
		.get##name(get##name##Name(iStep));                      \
}                                                                \
void ZoneIterativeData::write##name(int iStep,                   \
	std::string const & iName) {                                 \
	D()->writePointer(D()->m##name##Pointers,                    \
		iStep, iName, #name "Pointers");                         \
}                                                                \
	void ZoneIterativeData::write##name(int iStep, CGNS::type const & iValue) { \
	write##name(iStep, iValue.getName());                        \
}
	
OPTIONAL_CHILD_IMPLEMENTATION(RigidGridMotionPointers, DataArray,
	ZoneIterativeData, D()->mRigidGridMotionPointers)
	GET_WRITE_POINTERS(RigidGridMotion, RigidGridMotion);

OPTIONAL_CHILD_IMPLEMENTATION(ArbitraryGridMotionPointers, DataArray,
	ZoneIterativeData, D()->mArbitraryGridMotionPointers)

OPTIONAL_CHILD_IMPLEMENTATION(GridCoordinatesPointers, DataArray,
	ZoneIterativeData, D()->mGridCoordinatesPointers)

OPTIONAL_CHILD_IMPLEMENTATION(FlowSolutionPointers, DataArray,
	ZoneIterativeData, D()->mFlowSolutionPointers)

