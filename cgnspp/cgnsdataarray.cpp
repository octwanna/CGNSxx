/***************************************************************************
                          cgnsdataarray.cpp  -  description
                             -------------------
    begin                : Mon Mar 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsdataarray.h"

#include "cgnsarrayed.h"

#include <iostream>
#include <assert.h>

using CGNS::DataArray;

GENERATE_D(DataArray);

COMMON_NODE_IMPLEMENTATION(DataArray);

INIT_IMPLEMENTATION2(DataArray, mConversion, mExponents);

static double const idealGamma=1.4;

static DataArray::DataConversion stdConversion = {1,0};

static DataArray::DimensionalExponents none = {0,0,0,0,0};
//static DataArray::DimensionalExponents mass = {1,0,0,0,0}; // currently unused
static DataArray::DimensionalExponents length = {0,1,0,0,0};
static DataArray::DimensionalExponents duration = {0,0,1,0,0};
static DataArray::DimensionalExponents temperature = {0,0,0,1,0};
static DataArray::DimensionalExponents angle = {0,0,0,0,1};
static DataArray::DimensionalExponents potential = {0,2,-1,0,0};
static DataArray::DimensionalExponents density = {1,-3,0,0,0};
static DataArray::DimensionalExponents pressure = {1,-1,-2,0,0};
static DataArray::DimensionalExponents energyDensity = {0,2,-2,0,0};
static DataArray::DimensionalExponents entropy = {1,2,-2,-1,0};
static DataArray::DimensionalExponents entropyApprox = {1-idealGamma,3*idealGamma-1,-2,0,0};
static DataArray::DimensionalExponents velocity = {0,1,-1,0,0};
static DataArray::DimensionalExponents momentum = {1,-2,-1,0,0};
static DataArray::DimensionalExponents vorticity = {0,0,-1,0,0};
static DataArray::DimensionalExponents viscosity = {1,-1,-1,0,0};
static DataArray::DimensionalExponents thermalConductivity = {1,1,-3,-1,0};
static DataArray::DimensionalExponents specificHeat = {0,2,-2,-1,0};
static DataArray::DimensionalExponents dissipation = {0,2,-3,0,0};
static DataArray::DimensionalExponents force = {1,1,-2,0,0};
static DataArray::DimensionalExponents moment = {1,2,-2,0,0};
static DataArray::DimensionalExponents area = {0,2,0,0,0};
static DataArray::DimensionalExponents rotationRate = {0,0,-1,0,1};
//static DataArray::DimensionalExponents  = {0,0,0,0,0};

static DataArray::DimensionalExponents * stdExponents[DataArray::USER_DATA+1] = {
	// NONE,
	&none,
// Coordinates
	// COORDINATE_X, COORDINATE_Y, COORDINATE_Z,
	&length, &length, &length,
	// COORDINATE_R, COORDINATE_THETA, COORDINATE_PHI,
	&length, &angle, &angle,
	// COORDINATE_NORMAL, COORDINATE_TANGENTIAL,
	&length, &length,
	// COORDINATE_XI, COORDINATE_ETA, COORDINATE_ZETA,
	&length, &length, &length,
// Coordinate related
	// COORDINATE_TRANSFORM, INTERPOLANTS_DONOR, ELEMENT_CONNECTIVITY, PARENT_DATA,
	&none, &none, &none, &none,
// Solution quantities
	// POTENTIAL, STREAM_FUNCTION,
	&potential, &potential,
	// DENSITY, PRESSURE, TEMPERATURE,
	&density, &pressure, &temperature,
	// ENERGY_INTERNAL, ENTHALPY, ENTROPY, ENTROPY_APPROX,
	&energyDensity, &energyDensity, &entropy, &entropyApprox,
	// DENSITY_STAGNATION, PRESSURE_STAGNATION,
	&density, &pressure,
	// TEMPERATURE_STAGNATION, ENERGY_STAGNATION,
	&temperature, &energyDensity,
	// ENTHALPY_STAGNATION, ENERGY_STAGNATION_DENSITY,
	&energyDensity, &pressure,
	// VELOCITY_X, VELOCITY_Y, VELOCITY_Z,
	&velocity, &velocity, &velocity,
	// VELOCITY_R, VELOCITY_THETA, VELOCITY_PHI,
	&velocity, &velocity, &velocity,
	// VELOCITY_MAGNITUDE,
	&velocity,
	// VELOCITY_NORMAL, VELOCITY_TANGENTIAL,
	&velocity, &velocity,
	// VELOCITY_SOUND, VELOCITY_SOUND_STAGNATION,
	&velocity, &velocity,
	// MOMENTUM_X, MOMENTUM_Y, MOMENTUM_Z,
	&momentum, &momentum, &momentum,
	// MOMENTUM_MAGNITUDE,
	&momentum,
	// ENERGY_KINETIC, PRESSURE_DYNAMIC,
	&energyDensity, &pressure,
	// VORTICITY_X, VORTICITY_Y, VORTICITY_Z,
	&vorticity, &vorticity, &vorticity,
	// VORTICITY_MAGNITUDE,
	&vorticity,
	// SKIN_FRICTION_X, SKIN_FRICTION_Y, SKIN_FRICTION_Z,
	&pressure, &pressure, &pressure,
	// SKIN_FRICTION_MAGNITUDE,
	&pressure,
	// VELOCITY_ANGLE_X, VELOCITY_ANGLE_Y, VELOCITY_ANGLE_Z,
	&angle, &angle, &angle,
	// VELOCITY_UNIT_VECTOR_X, VELOCITY_UNIT_VECTOR_Y, VELOCITY_UNIT_VECTOR_Z,
	&none, &none, &none,
	// MASS_FLOW,
	&momentum,
	// VISCOSITY_KINEMATIC, VISCOSITY_MOLECULAR, VISCOSITY_EDDY_KINEMATIC, VISCOSITY_EDDY,
	&potential, &viscosity, &potential, &viscosity,
	// THERMAL_CONDUCTIVITY,
	&thermalConductivity,
	// REYNOLDS_STRESS_XX, REYNOLDS_STRESS_XY, REYNOLDS_STRESS_XZ,
	&pressure, &pressure, &pressure,
	// REYNOLDS_STRESS_YY, REYNOLDS_STRESS_YZ, REYNOLDS_STRESS_ZZ,
	&pressure, &pressure, &pressure,	

// Solution related
	// POWER_LAW_EXPONENT, SUTHERLAND_LAW_CONSTANT, TEMPERATURE_REFERENCE,
	&none, &temperature, &temperature,
	// VISCOSITY_MOLECULAR_REFERENCE, THERMAL_CONDUCTIVITY_REFERENCE,
	&viscosity, &thermalConductivity,
	// IDEAL_GAS_CONSTANT, SPECIFIC_HEAT_PRESSURE, SPECIFIC_HEAT_VOLUME,
	&specificHeat, &specificHeat, &specificHeat,
	// LENGTH_REFERENCE,
	&length,
// Turbulence model quantities
	// TURBULENT_DISTANCE, TURBULENT_ENERGY_KINETIC, TURBULENT_DISSIPATION,
	&length, &energyDensity, &dissipation,
	// TURBULENT_DISSIPATION_RATE, TURBULENT_B_B_REYNOLDS, TURBULENT_S_A_NU_TILDE,
	&vorticity, &none, &potential,
// Nondimensional parameters
	// MACH, MACH_VELOCITY, MACH_VELOCITY_SOUND,
	&none, &velocity, &velocity,
	// REYNOLDS, REYNOLDS_VELOCITY, REYNOLDS_LENGTH, REYNOLDS_VISCOSITY_KINEMATIC,
	&none, &velocity, &length, &potential,
	// PRANDTL, PRANDTL_THERMAL_CONDUCTIVITY, PRANDTL_VISCOSITY_MOLECULAR,
	&none, &thermalConductivity, &viscosity,
	// PRANDTL_SPECIFIC_HEAT_PRESSURE, PRANDTL_TURBULENT,
	&specificHeat, &none,
	// SPECIFIC_HEAT_RATIO, SPECIFIC_HEAT_RATIO_PRESSURE, SPECIFIC_HEAT_RATIO_VOLUME,
	&none, &specificHeat, &specificHeat,
	// COEF_PRESSURE,
	&none,
	// COEF_SKIN_FRICTION_X, COEF_SKIN_FRICTION_Y, COEF_SKIN_FRICTION_Z,
	&none, &none, &none,
	// COEF_PRESSURE_DYNAMIC, COEF_PRESSURE_REFERENCE,
	&pressure, &pressure,
// Characteristics and Riemann invariants
	// RIEMANN_INVARIANT_PLUS, RIEMANN_INVARIANT_MINUS,
	&velocity, &velocity,
	// CHARACTERISTIC_ENTROPY, CHARACTERISTIC_VORTICITY_1, CHARACTERISTIC_VORTICITY_2,
	&pressure, &velocity, &velocity,
	// CHARACTERISTIC_ACOUSTIC_PLUS, CHARACTERISTIC_ACOUSTIC_MINUS,
	&pressure, &pressure,
// Forces and moments
	// FORCE_X, FORCE_Y, FORCE_Z,
	&force, &force, &force,
	// FORCE_R, FORCE_THETA, FORCE_PHI,
	&force, &force, &force,
	// LIFT, DRAG,
	&force, &force,
	// MOMENT_X, MOMENT_Y, MOMENT_Z,
	&moment, &moment, &moment,
	// MOMENT_R, MOMENT_THETA, MOMENT_PHI,
	&moment, &moment, &moment,
	// MOMENT_XI, MOMENT_ETA, MOMENT_ZETA,
	&moment, &moment, &moment,
	// MOMENT_CENTER_X, MOMENT_CENTER_Y, MOMENT_CENTER_Z,
	&length, &length, &length,
	// COEF_LIFT, COEF_DRAG,
	&none, &none,
	// COEF_MOMENT_X, COEF_MOMENT_Y, COEF_MOMENT_Z,
	&none, &none, &none,
	// COEF_MOMENT_R, COEF_MOMENT_THETA, COEF_MOMENT_PHI,
	&none, &none, &none,
	// COEF_MOMENT_XI, COEF_MOMENT_ETA, COEF_MOMENT_ZETA,
	&none, &none, &none,
	// COEF_PRESSURE_DYNAMIC,
//	&pressure, // already under nondimensional parameters
	// COEF_AREA, COEF_LENGTH,
	&area, &length,
// Time-dependent flow
	// TIME_VALUES, ITERATION_VALUES,
	&duration, &none,
	// NUMBER_OF_ZONES, NUMBER_OF_FAMILIES,
	&none, &none,
	// ZONE_POINTERS, FAMILY_POINTERS,
	&none, &none,
	// RIGID_GRID_MOTION_POINTERS, ARBITRARY_GRID_MOTION_POINTERS,
	&none, &none,
	// GRID_COORDINATES_POINTERS, FLOW_SOLUTIONS_POINTERS,
	&none, &none,	
	// ORIGIN_LOCATION, RIGID_ROTATION_ANGLE, RIGID_VELOCITY, RIGID_ROTATION_RATE,
	&length, &angle, &velocity, &rotationRate,
	// GRID_VELOCITY_X, GRID_VELOCITY_Y, GRID_VELOCITY_Z,
	&velocity, &velocity, &velocity,
	// GRID_VELOCITY_R, GRID_VELOCITY_THETA, GRID_VELOCITY_PHI,
	&velocity, &rotationRate, &rotationRate,
	// GRID_VELOCITY_XI, GRID_VELOCITY_ETA, GRID_VELOCITY_ZETA,
	&velocity, &velocity, &velocity,
// from version 2.1 on		
	// FUEL_AIR_RATIO, REFERENCE_TEMPERATURE_HOF, ENTHALPY_ENERGY_RATIO,
	&none, &temperature, &none,
	// COMPRESSIBILITY_FACTOR, VIBRATIONAL_ELECTRON_ENERGY,
	&none, &energyDensity,
	// HEAT_OF_FORMATION, VIBRATIONAL_ELECTRON_TEMPERATURE,
	&energyDensity, &temperature,
// Done with all predefined quantities
	// USER
	&none
};


static DataArray::DimensionalExponents * createExponents(ADF::Node const & iNode) {
    CGNS::Private::checkNodeReal(iNode, 5);
	double data[5];
	iNode.readData(data);
	std::auto_ptr<DataArray::DimensionalExponents> exponents(new DataArray::DimensionalExponents);
	exponents->mMass=data[0];
	exponents->mLength=data[1];
	exponents->mTime=data[2];
	exponents->mTemperature=data[3];
	exponents->mAngle=data[4];
	return exponents.release();
}

static DataArray::DataConversion * createConversion(ADF::Node const & iNode) {
    CGNS::Private::checkNodeReal(iNode, 2);
	double data[2];
	iNode.readData(data);
	std::auto_ptr<DataArray::DataConversion> conversion(new DataArray::DataConversion);
	conversion->mScale=data[0];
	conversion->mOffset=data[1];
	return conversion.release();
}


void CGNS::Private::DataArray::Read(DataArray * me) {
	me->mQuantity=CGNS::DataArray::USER_DATA;
	convertN(me->mNode.getName(), &me->mQuantity);
	me->mInitialized|=INIT_MASK_SELF;
}

void CGNS::Private::DataArray::Read(DataArray * me,
	std::auto_ptr<CGNS::DataArray::DataConversion> & cDest) {
//std::cerr << "ReadDataConversion (" << me->mNode.getName() << "\n";	
	assert(&cDest==&me->mConversion);
	char const * name=implementation_traits<CGNS::DataArray::DataConversion>::Name();
	if (me->mNode.hasChild(name)) {
//std::cerr << "ReadDataConversion got Name\n";	
		ADF::Node child=me->mNode.getChild(name);
		if (child.getLabel()==implementation_traits<CGNS::DataArray::DataConversion>::Label()) {
//std::cerr << "ReadDataConversion got Label\n";	
			cDest.reset(createConversion(child));
		}
	}
	me->mInitialized|=implementation_traits<CGNS::DataArray::DataConversion>::INIT_MASK;
}

void CGNS::Private::DataArray::Read(DataArray * me,
	std::auto_ptr<CGNS::DataArray::DimensionalExponents> & cDest) {
//std::cerr << "ReadDimensionalExponents (" << me->mNode.getName() << "\n";	
	assert(&cDest==&me->mExponents);
	char const * name=implementation_traits<CGNS::DataArray::DimensionalExponents>::Name();
	if (me->mNode.hasChild(name)) {
//std::cerr << "ReadDimensionalExponents got Name\n";	
		ADF::Node child=me->mNode.getChild(name);
		if (child.getLabel()==implementation_traits<CGNS::DataArray::DimensionalExponents>::Label()) {
//std::cerr << "ReadDimensionalExponents got Label\n";	
			cDest.reset(createExponents(child));
		}
	}
	me->mInitialized|=implementation_traits<CGNS::DataArray::DataConversion>::INIT_MASK;
}

void CGNS::Private::DataArray::Modify(CGNS::DataArray::Quantity iQuantity) {
	mQuantity=iQuantity;
	// make sure there are dimensions
	mNode.setDataTypeDimensions(ADF::R8, 1);
}

bool DataArray::hasDataConversion() const {
	NEEDS(mConversion);
	return D()->mConversion.get()!=0;
}

DataArray::DataConversion DataArray::getDataConversion() const {
	NEEDS(mConversion);
	if (D()->mConversion.get()!=0) return *D()->mConversion;
	else return stdConversion;
}

bool DataArray::hasDimensionalExponents() const {
	NEEDS(mExponents);
	return D()->mExponents.get()!=0;
}

DataArray::DimensionalExponents DataArray::getDimensionalExponents() const {
	NEEDS(mExponents);
	Private::DataArray const * impl=D();
	if (impl->mExponents.get()!=0) return *impl->mExponents;
	DataClass dataClass=getDataClass();
	if (dataClass==NONDIMENSIONAL_PARAMETER || dataClass==DIMENSIONLESS_CONSTANT)
		return none;
	/// \todo try and return standard exponents for the standard values
	if (impl->mQuantity<=NULL_DATA) Private::Emit(Exception::NO_QUANTITY);
	if (impl->mQuantity>=USER_DATA) Private::Emit(Exception::NO_EXPONENTS);
	return *stdExponents[impl->mQuantity];
}
	
DataArray::Quantity DataArray::getQuantity() const {
	NEEDS_THIS();
	return D()->mQuantity;
}

int DataArray::getNumDimensions() const {
	return D()->mNode.getNumDimensions();
}

int DataArray::getDimension(int iNth) const {
	return D()->mNode.getDimension(iNth);
}

static ADF::DataType toADFType(DataArray::DataType iType) {
	switch (iType) {
		case DataArray::CHAR: return ADF::C1;
		case DataArray::INT: return ADF::I4;
		case DataArray::FLOAT: return ADF::R4;
		case DataArray::DOUBLE: return ADF::R8;
		default: EMIT_INTERNAL_ERROR();
	}
}

static DataArray::DataType toCGNSType(ADF::DataType iType) {
	switch (iType) {
//		case ADF::MT: return DataArray::;
		case ADF::I4: return DataArray::INT;
		case ADF::U4: return DataArray::INT;
		case ADF::I8: return DataArray::INT;
		case ADF::U8: return DataArray::INT;
		case ADF::R4: return DataArray::FLOAT;
		case ADF::R8: return DataArray::DOUBLE;
//		case ADF::X4: return DataArray::;
//		case ADF::X8: return DataArray::;
		case ADF::C1: return DataArray::CHAR;
		case ADF::B1: return DataArray::CHAR;
//		case ADF::STRUCT: return DataArray::;
//		case ADF::LK: return DataArray::;
		default: EMIT_INTERNAL_ERROR();
	}
}
	
DataArray::DataType DataArray::getDataType() const {
	return toCGNSType(D()->mNode.getDataType());
}

void DataArray::setDimensions(int iNumDimensions, int const * iDimensions) {
	D()->mNode.setDimensions(iNumDimensions, iDimensions);
}

void DataArray::setDimensions(int iDimension) {
	D()->mNode.setDimensions(iDimension);
}

void DataArray::setDimensions(int iDim1, int iDim2) {
	D()->mNode.setDimensions(iDim1, iDim2);
}

void DataArray::setDimensions(int iDim1, int iDim2, int iDim3) {
	D()->mNode.setDimensions(iDim1, iDim2, iDim3);
}

void DataArray::setDataTypeDimensions(DataType iType,
	int iNumDimensions, int const * iDimensions) {
	D()->mNode.setDataTypeDimensions(toADFType(iType), iNumDimensions, iDimensions);
}

void DataArray::setDataTypeDimensions(DataType iType, int iDimension) {
	D()->mNode.setDataTypeDimensions(toADFType(iType), iDimension);
}

void DataArray::setDataTypeDimensions(DataType iType, int iDim1, int iDim2) {
	D()->mNode.setDataTypeDimensions(toADFType(iType), iDim1, iDim2);
}

void DataArray::setDataTypeDimensions(DataType iType, int iDim1, int iDim2, int iDim3) {
	D()->mNode.setDataTypeDimensions(toADFType(iType), iDim1, iDim2, iDim3);
}

void DataArray::readData(int * oData) const {
	D()->mNode.readData(oData);
}

void DataArray::readData(float * oData) const {
	D()->mNode.readData(oData);
}

void DataArray::readData(double * oData) const {
	D()->mNode.readData(oData);
}
	
void DataArray::writeData(int const * iData) {
	D()->mNode.writeData(iData);
}

void DataArray::writeData(float const * iData) {
	D()->mNode.writeData(iData);
}

void DataArray::writeData(double const * iData) {
	D()->mNode.writeData(iData);
}
