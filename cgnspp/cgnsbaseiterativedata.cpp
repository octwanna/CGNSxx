/***************************************************************************
                          baseiterativedata.cpp  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsbaseiterativedata.h"

#include "cgnsbase.h"
#include "cgnsdataarray.h"

using CGNS::BaseIterativeData;

GENERATE_D(BaseIterativeData);

COMMON_NODE_IMPLEMENTATION(BaseIterativeData);

INIT_IMPLEMENTATION0(BaseIterativeData);

void CGNS::Private::BaseIterativeData::Read(BaseIterativeData * me) {
	checkNode(me->mNode, ADF::I4, 1);
	me->mNode.readData(&me->mNumberOfSteps);
	if (me->mNumberOfSteps<0) me->Emit(Exception::ILLEGAL_DATA);
	/// \todo plausibility and consistency check for mNumberOfSteps and data arrays or values
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::BaseIterativeData::Read(BaseIterativeData * me, refcnt_ptr<DataArray> &cDest) {
	Needs(me, me->mArrays);
#define FETCH_ARRAY(name)                          \
	if (&cDest==&me->m##name)                      \
		if (me->hasArray(#name))                   \
			me->m##name.reset(me->getArray(#name))
	FETCH_ARRAY(TimeValues);
	FETCH_ARRAY(IterationValues);
	FETCH_ARRAY(NumberOfZones);
	FETCH_ARRAY(ZonePointers);
	FETCH_ARRAY(NumberOfFamilies);
	FETCH_ARRAY(FamilyPointers);
#undef FETCH_ARRAY	
}

void CGNS::Private::BaseIterativeData::Modify(int iNumberOfSteps) {
	if (iNumberOfSteps<0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	mNode.setDataTypeDimensions(ADF::I4, 1);
	mNode.writeData(&iNumberOfSteps);
	mNumberOfSteps=iNumberOfSteps;
}

template<typename T>
void CGNS::Private::BaseIterativeData::getValue(refcnt_ptr<DataArray> const & iArray,
	int iStep, T * oValue) const {
	Needs(this);
	Needs(this, iArray);
	checkNode(iArray->mNode, adfType(*oValue), mNumberOfSteps);
	Private::checkIndex(iStep, iArray->mNode.getDimension(0));
	ADF::Range range(iStep, iStep+1);
	iArray->mNode.readData(range, oValue);
}

template<typename T>
int CGNS::Private::BaseIterativeData::appendValue(refcnt_ptr<DataArray> & cArray,
	T iValue, std::string const & iName) {
	Needs(this);
	Needs(this, cArray);
	if (!cArray) {
		cArray.reset(writeArray(iName));
		// unfortunately can't ADF handle zero dimensions
		cArray->mNode.setDataTypeDimensions(adfType(iValue), mNumberOfSteps ? mNumberOfSteps : 1);
	}
	else {
		if (!mNumberOfSteps) Private::Emit(Exception::ILLEGAL_DATA);
		checkNode(cArray->mNode, adfType(iValue), mNumberOfSteps);
	}
	if (mNumberOfSteps) mNumberOfSteps=appendStep();
	else mNumberOfSteps=1;
	writeValue(cArray, mNumberOfSteps-1, iValue, iName);
	return mNumberOfSteps;
}


template<typename T>
T CGNS::Private::BaseIterativeData::writeValue(refcnt_ptr<DataArray> & cArray,
	int iStep, T iValue, std::string const & iName) {
	Needs(this);
	checkIndex(iStep, mNumberOfSteps);
	Needs(this, cArray);
	if (!cArray) {
		cArray.reset(writeArray(iName));
		cArray->mNode.setDataTypeDimensions(adfType(iValue), mNumberOfSteps);
	}
	else {
		checkNode(cArray->mNode, adfType(iValue), mNumberOfSteps);
	}
	ADF::Range range(iStep, iStep+1);
	cArray->mNode.writeData(range, &iValue);
	return iValue;
}

int CGNS::Private::BaseIterativeData::getNumPointer(refcnt_ptr<DataArray> const & iArray, int iStep) const {
	Needs(this);
	Private::checkIndex(iStep, mNumberOfSteps);
	Needs(this, iArray);
	if (!iArray) return 0;
	checkNode(iArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0, mNumberOfSteps);
	int maxNum=iArray->mNode.getDimension(1);
	int len=maxNum*ADF::NAME_LENGTH;
	std::vector<char> data(len);
	ADF::Range range(iStep*len, (iStep+1)*len);
	iArray->mNode.readData(range, &data[0]);
	for (int i=0; i<maxNum; ++i) {
		if (STRNCMP(&data[i*ADF::NAME_LENGTH], "Null", 4)==0) return i;
	}
	return maxNum;
}

void CGNS::Private::BaseIterativeData::getPointer(refcnt_ptr<DataArray> const & iArray,
	int iStep, int iNum, std::string * oPointer) const {
	Needs(this);
	Private::checkIndex(iStep, mNumberOfSteps);
	Needs(this, iArray);
	checkNode(iArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0, mNumberOfSteps);
	Private::checkIndex(iStep, mNumberOfSteps);
	int maxNum=iArray->mNode.getDimension(1);
	Private::checkIndex(iNum, maxNum);
	int start=(iStep*maxNum+iNum)*ADF::NAME_LENGTH;
	ADF::Range range(start, start+ADF::NAME_LENGTH);
	char name[ADF::NAME_LENGTH+1];
	iArray->mNode.readData(range, name);
	name[ADF::NAME_LENGTH]=0;
	// strip trailing spaces
	char * cp=name+ADF::NAME_LENGTH-1;
	while (cp>name && *cp==' ') *cp--=0;
	*oPointer=name;
}

int CGNS::Private::BaseIterativeData::appendPointer(refcnt_ptr<DataArray> & cArray,
	refcnt_ptr<DataArray> & cNumArray, int iStep, std::string const & iPointer,
	std::string const & iName, std::string const & iNumName) {
	int ptrSize=iPointer.size();
	if (ptrSize>ADF::NAME_LENGTH) Emit(Exception::STRING_TOO_LONG);
	Needs(this);
	Private::checkIndex(iStep, mNumberOfSteps);
	Needs(this, cNumArray);
	int num=0;
	if (cNumArray.get()!=0) getValue(cNumArray, iStep, &num);
	else writeValue(cNumArray, iStep, num, iNumName);
	Needs(this, cArray);
	if (!cArray) {
		cArray.reset(writeArray(iName));
		cArray->mNode.setDataType(ADF::C1);
	}
	else checkNode(cArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0, mNumberOfSteps);
	int maxNum=cArray->mNode.getDimension(1);
	if (num>=maxNum) {
		// we have to expand, which involves major reshuffling
		std::vector<char> data(ADF::NAME_LENGTH*(maxNum+1)*mNumberOfSteps);
		cArray->mNode.readData(&data[0]);
		for (int j=mNumberOfSteps-1; j>0; --j) {
			int src=ADF::NAME_LENGTH*maxNum*j;
			int dst=ADF::NAME_LENGTH*(maxNum+1)*j;
			MEMMOVE(&data[dst], &data[src], ADF::NAME_LENGTH*maxNum);
			// fill the new slot
			STRNCPY(&data[dst-ADF::NAME_LENGTH],
				"Null                            ", ADF::NAME_LENGTH);
		}
	}
	++num;
	writeValue(cArray, iStep, num, iPointer);
	return num;
}

void CGNS::Private::BaseIterativeData::writePointer(refcnt_ptr<DataArray> & cArray,
	int iStep, int iNum, std::string const & iPointer) {
	int ptrSize=iPointer.size();
	if (ptrSize>ADF::NAME_LENGTH) Emit(Exception::STRING_TOO_LONG);
	Needs(this);
	checkIndex(iStep, mNumberOfSteps);
	Needs(this, cArray);
	checkNode(cArray->mNode, ADF::C1, ADF::NAME_LENGTH, 0, mNumberOfSteps);
	int maxNum=cArray->mNode.getDimension(1);
	checkIndex(iNum, maxNum);
	
	int start=(iStep*maxNum+iNum)*ADF::NAME_LENGTH;
	ADF::Range range(start, start+ADF::NAME_LENGTH);
	char name[ADF::NAME_LENGTH+1];
	STRNCPY(name, iPointer.c_str(), ADF::NAME_LENGTH);
	// generate trailing spaces
	for (char * cp=name+ptrSize; cp<=name+ADF::NAME_LENGTH; ++cp) *cp=' ';
	cArray->mNode.writeData(range, name);
}

int CGNS::Private::BaseIterativeData::appendStep() {
	int step=++mNumberOfSteps;
	if (step!=appendStep(mTimeValues)) Emit(Exception::WRONG_DIMENSION);
	if (step!=appendStep(mIterationValues)) Emit(Exception::WRONG_DIMENSION);
	if (step!=appendStep(mNumberOfZones)) Emit(Exception::WRONG_DIMENSION);
	if (step!=appendStep(mNumberOfFamilies)) Emit(Exception::WRONG_DIMENSION);
	if (step!=appendStep(mZonePointers)) Emit(Exception::WRONG_DIMENSION);
	if (step!=appendStep(mFamilyPointers)) Emit(Exception::WRONG_DIMENSION);
	mNode.writeData(&mNumberOfSteps);
	return step-1;
}

int CGNS::Private::BaseIterativeData::appendStep(DataArray * iArray) {
	if (!iArray) return mNumberOfSteps;
	ADF::Node node=iArray->mNode;
	int numDim=node.getNumDimensions();
	int dims[ADF::MAX_DIMENSIONS];
	node.getDimensions(dims);
	int numSteps=dims[numDim-1];
	int size=node.getDataSize();
	int newSize=(size/numSteps)*(numSteps+1);
	char defaultValue=(node.getDataType()==ADF::C1) ? ' ' : 0;
	std::vector<char> data(newSize, defaultValue);
	node.readData(static_cast<void *>(&data[0]));
	dims[numDim-1]=++numSteps;
	node.setDimensions(numDim, dims);
	node.writeData(static_cast<void *>(&data[0]));
	return numSteps;
}

int CGNS::BaseIterativeData::getNumberOfSteps() const {
	NEEDS_THIS();
	return D()->mNumberOfSteps;
}
	
#define GET_WRITE_ARRAY(name, type)                           \
type BaseIterativeData::get##name(int iStep) const {          \
	type value=0;                                             \
	D()->getValue(D()->m##name, iStep, &value);               \
	return value;                                             \
}                                                             \
type BaseIterativeData::write##name(int iStep, type iValue) { \
	NEEDS_THIS();                                             \
	return D()->writeValue(D()->mTimeValues,                  \
		iStep, iValue, #name);                                \
}
	
#define GET_APPEND_WRITE_ARRAY(name, type)                \
GET_WRITE_ARRAY(name, type)                               \
int BaseIterativeData::append##name(type iValue) {        \
	NEEDS_THIS();                                         \
	return D()->appendValue(D()->m##name, iValue, #name); \
}
	
#define GET_APPEND_WRITE_POINTER_ARRAY(type, name, numName)                 \
int BaseIterativeData::getMaxNum##type() const {                            \
	NEEDS(m##name);                                                         \
	if (!D()->m##name) return 0;                                            \
	return D()->m##name->mNode.getDimension(1);                             \
}                                                                           \
int BaseIterativeData::getNum##type(int iStep) const {                      \
	return D()->getNumPointer(D()->m##name, iStep);                         \
}                                                                           \
std::string BaseIterativeData::get##type##Name(int iStep, int iNum) const { \
	std::string n;                                                          \
	D()->getPointer(D()->m##name, iStep, iNum, &n);                         \
	return n;                                                               \
}                                                                           \
CGNS::type BaseIterativeData::get##type(int iStep, int iNum) const {        \
	std::string n;                                                          \
	D()->getPointer(D()->m##name, iStep, iNum, &n);                         \
	Base parent(const_cast<Private::Base *>(D()->P()));                     \
	return parent.get##type(n);                                             \
}                                                                           \
int BaseIterativeData::append##type(int iStep, std::string const & iName) { \
	return D()->appendPointer(D()->m##name,                                 \
		D()->m##numName, iStep, iName, #name, #numName);                    \
}                                                                           \
int BaseIterativeData::append##type(int iStep, CGNS::type const & iType) {        \
	return append##type(iStep, iType.getName());                            \
}                                                                           \
void BaseIterativeData::write##type(int iStep, std::string const & iType,   \
	int iNum) {                                                             \
	D()->writePointer(D()->m##name, iStep, iNum, iType);                    \
}                                                                           \
void BaseIterativeData::write##type(int iStep, CGNS::type const & iType, int iNum) { \
	write##type(iStep, iType.getName(), iNum);                              \
}

OPTIONAL_CHILD_IMPLEMENTATION(TimeValues, DataArray, BaseIterativeData, D()->mTimeValues);
GET_APPEND_WRITE_ARRAY(TimeValues, double);

OPTIONAL_CHILD_IMPLEMENTATION(IterationValues, DataArray, BaseIterativeData, D()->mIterationValues);
GET_APPEND_WRITE_ARRAY(IterationValues, int);

void BaseIterativeData::appendTimeIterationValues(double iTime, int iIteration) {
	int steps=appendTimeValues(iTime);
	if (!D()->mIterationValues) {
		D()->mIterationValues.reset(D()->writeArray("IterationValues"));
		D()->mIterationValues->mNode.setDataTypeDimensions(ADF::I4, steps);
	}
	writeIterationValues(steps-1, iIteration);
}
	
OPTIONAL_CHILD_IMPLEMENTATION(NumberOfZones, DataArray, BaseIterativeData, D()->mNumberOfZones);
GET_WRITE_ARRAY(NumberOfZones, int);

OPTIONAL_CHILD_IMPLEMENTATION(ZonePointers, DataArray, BaseIterativeData, D()->mZonePointers);
GET_APPEND_WRITE_POINTER_ARRAY(Zone, ZonePointers, NumberOfZones);

OPTIONAL_CHILD_IMPLEMENTATION(NumberOfFamilies, DataArray, BaseIterativeData, D()->mNumberOfFamilies);
GET_WRITE_ARRAY(NumberOfFamilies, int);			

OPTIONAL_CHILD_IMPLEMENTATION(FamilyPointers, DataArray, BaseIterativeData, D()->mFamilyPointers);
GET_APPEND_WRITE_POINTER_ARRAY(Family, FamilyPointers, NumberOfFamilies);

