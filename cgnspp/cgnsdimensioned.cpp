/***************************************************************************
                          cgnsdimensioned.cpp  -  description
                             -------------------
    begin                : Mon Mar 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsdimensioned.h"

#include "cgnsutil.h"

#include <iostream>
#include <assert.h>

using CGNS::Dimensioned;

GENERATE_D(Dimensioned)

COMMON_NODE_IMPLEMENTATION(Dimensioned);

INIT_IMPLEMENTATION2(Dimensioned, mClass, mUnits)

static CGNS::DataClass * createClass(ADF::Node const & iNode) {
    std::string text;
    iNode.readData(&text);
	std::auto_ptr<CGNS::DataClass> c(new CGNS::DataClass(CGNS::NULL_CLASS));
	CGNS::Private::convert(text, &*c);
	return c.release();
}

static inline std::string strip(char const * iString) {
	std::string s(iString, ADF::NAME_LENGTH);
	s.erase(s.find(' '));
	return s;
}

static CGNS::DimensionalUnits * createUnits(ADF::Node const & iNode) {
    CGNS::Private::checkNode(iNode, ADF::C1, ADF::NAME_LENGTH, 5);
    char data[5][ADF::NAME_LENGTH];
    iNode.readData(&data[0][0]);
	std::auto_ptr<CGNS::DimensionalUnits> units(new CGNS::DimensionalUnits);
	CGNS::Private::convert(strip(data[0]), &units->mMass);
	CGNS::Private::convert(strip(data[1]), &units->mLength);
	CGNS::Private::convert(strip(data[2]), &units->mTime);
	CGNS::Private::convert(strip(data[3]), &units->mTemperature);
	CGNS::Private::convert(strip(data[4]), &units->mAngle);
	return units.release();
}

void CGNS::Private::Dimensioned::Read(Dimensioned * me, std::auto_ptr<DataClass> & cDest) {
	assert(&cDest==&me->mClass);
	char const * name=implementation_traits<DataClass>::Name();
	if (me->mNode.hasChild(name)) {
		ADF::Node n(me->mNode.getChild(name));
		if (n.getLabel()==implementation_traits<DataClass>::Label())
			cDest.reset(createClass(n));
	}
	me->mInitialized|=implementation_traits<DataClass>::INIT_MASK;
}

void CGNS::Private::Dimensioned::Read(Dimensioned * me, std::auto_ptr<DimensionalUnits> & cDest) {
	assert(&cDest==&me->mUnits);
	char const * name=implementation_traits<DimensionalUnits>::Name();
	if (me->mNode.hasChild(name)) {
		ADF::Node n(me->mNode.getChild(name));
		if (n.getLabel()==implementation_traits<DimensionalUnits>::Label())
			cDest.reset(createUnits(n));
	}
	me->mInitialized|=implementation_traits<DimensionalUnits>::INIT_MASK;
}

void CGNS::Private::Dimensioned::Needs(Dimensioned * me,
	std::auto_ptr<DataClass> & cDest) {
	assert(&cDest==&me->mClass);
	if (!(me->mInitialized&implementation_traits<DataClass>::INIT_MASK)) {
		Node::Needs(me, cDest);
		if (cDest.get()!=0) return;
	}
	if (me->P()) Needs(me->P(), me->P()->mClass);
}

void CGNS::Private::Dimensioned::Needs(Dimensioned * me,
	std::auto_ptr<DimensionalUnits> & cDest) {
	assert(&cDest==&me->mUnits);
	if (!(me->mInitialized&implementation_traits<DimensionalUnits>::INIT_MASK)) {
		Node::Needs(me, cDest);
		if (cDest.get()!=0) return;
	}
	if (me->P()) Needs(me->P(), me->P()->mUnits);
}

bool CGNS::Private::Dimensioned::hasDataClass() const {
	Needs(this, mClass);
	if (mClass.get()) return true;
	if (!P()) return false;
	return P()->hasDataClass();
}

bool CGNS::Private::Dimensioned::hasDimensionalUnits() const {
	Needs(this, mUnits);
	if (mUnits.get()) return true;
	if (!P()) return false;
	return P()->hasDimensionalUnits();
}

CGNS::DataClass CGNS::Private::Dimensioned::getDataClass() const {
	Needs(this, mClass);
	if (mClass.get()) return *mClass;
	if (!P()) Emit(Exception::NO_DATACLASS);
	return P()->getDataClass();
}

CGNS::DimensionalUnits const & CGNS::Private::Dimensioned::getDimensionalUnits() const {
	Needs(this, mUnits);
	if (mUnits.get()) return *mUnits;
	if (!P()) Emit(Exception::NO_UNITS);
	return P()->getDimensionalUnits();
}

bool Dimensioned::hasLocalDataClass() const {
	NEEDS(mClass);
	return D()->mClass.get()!=0;
}

bool Dimensioned::hasLocalDimensionalUnits() const {
	NEEDS(mUnits);
	return D()->mUnits.get()!=0;
}

bool Dimensioned::hasDataClass() const {
	return D()->hasDataClass();
}

bool Dimensioned::hasDimensionalUnits() const {
	return D()->hasDimensionalUnits();
}

CGNS::DataClass Dimensioned::getDataClass() const {
	return D()->getDataClass();
}

CGNS::DimensionalUnits const & Dimensioned::getDimensionalUnits() const {
	return D()->getDimensionalUnits();
}

CGNS::MassUnit Dimensioned::getMassUnit() const {
	return D()->getDimensionalUnits().mMass;
}

CGNS::LengthUnit Dimensioned::getLengthUnit() const {
	return D()->getDimensionalUnits().mLength;
}

CGNS::TimeUnit Dimensioned::getTimeUnit() const {
	return D()->getDimensionalUnits().mTime;
}

CGNS::TemperatureUnit Dimensioned::getTemperatureUnit() const {
	return D()->getDimensionalUnits().mTemperature;
}

CGNS::AngleUnit Dimensioned::getAngleUnit() const {
	return D()->getDimensionalUnits().mAngle;
}

CGNS::DataClass Dimensioned::writeDataClass(CGNS::DataClass iClass) {
	NEEDS(mClass);
	iClass=Private::validateIndex(iClass, NULL_CLASS, USER_CLASS);
	std::string dc(asString(iClass));
	// fill with spaces
	while (dc.size()<ADF::NAME_LENGTH) dc+=' ';
	Private::assureChild(D()->mClass, D(), iClass).writeData(dc);
/*	
	char const * name=Private::implementation_traits<DataClass>::Name();
	if (!D()->mNode.hasChild(name))
		 D()->mNode.createChild(name,
		 	Private::implementation_traits<DataClass>::Label());
	
	ADF::Node n(D()->mNode.getChild(name));
	n.writeData(dc);
	if (!D()->mClass.get()) D()->mClass.reset(new DataClass(iClass));
	else *D()->mClass=iClass;
*/	
	return iClass;
}

CGNS::DimensionalUnits const & Dimensioned::writeDimensionalUnits(CGNS::DimensionalUnits const & iUnits) {
	NEEDS(mUnits);
	DimensionalUnits u;
	u.mMass=Private::validateIndex(iUnits.mMass, NULL_MASS, USER_MASS);
	u.mLength=Private::validateIndex(iUnits.mLength, NULL_LENGTH, USER_LENGTH);
	u.mTime=Private::validateIndex(iUnits.mTime, NULL_TIME, USER_TIME);
	u.mTemperature=Private::validateIndex(iUnits.mTemperature, NULL_TEMPERATURE, USER_TEMPERATURE);
	u.mAngle=Private::validateIndex(iUnits.mAngle, NULL_ANGLE, USER_ANGLE);
	// build up name data
	std::string units(asString(u.mMass));
	// fill with spaces
	units.append(' ', 1*ADF::NAME_LENGTH-units.size());
	units+=asString(u.mLength);
	units.append(' ', 2*ADF::NAME_LENGTH-units.size());
	units+=asString(u.mTime);
	units.append(' ', 3*ADF::NAME_LENGTH-units.size());
	units+=asString(u.mTemperature);
	units.append(' ', 4*ADF::NAME_LENGTH-units.size());
	units+=asString(u.mAngle);
	units.append(' ', 5*ADF::NAME_LENGTH-units.size());
	
/*	if (!D()->mUnits.get()) D()->mUnits.reset(new DimensionalUnits(u));
	else *D()->mUnits=u;

	char const * name=Private::implementation_traits<DimensionalUnits>::Name();
	if (!D()->mNode.hasChild(name))
		 D()->mNode.createChild(name,
		 	Private::implementation_traits<DimensionalUnits>::Label());
	
	ADF::Node n(D()->mNode.getChild(name));
*/	ADF::Node n(Private::assureChild(D()->mUnits, D(), u));
	n.setDataTypeDimensions(ADF::C1, 5, ADF::NAME_LENGTH);
	n.writeData(units.data());
	return *D()->mUnits;
}

static CGNS::DimensionalUnits stdUnits[] = {
	{ CGNS::KILOGRAM, CGNS::METER, CGNS::SECOND, CGNS::KELVIN, CGNS::RADIAN },
	{ CGNS::GRAM, CGNS::CENTIMETER, CGNS::SECOND, CGNS::KELVIN, CGNS::RADIAN },
};

CGNS::DimensionalUnits const & Dimensioned::writeDimensionalUnits(StandardUnits iUnits) {
	Private::assertIndex(iUnits, SI_UNITS, CGS_UNITS, Exception::NO_UNITS);
	return writeDimensionalUnits(stdUnits[iUnits]);
}
