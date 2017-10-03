/***************************************************************************
                          cgnsutil.cpp  -  description
                             -------------------
    begin                : Fri Mar 16 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsutil.h"

#include <iostream>
#include <assert.h>


template<typename T>
std::string CGNS::asString(T iT) {
	std::string text;
	Private::convert(iT, &text);
	return text;
}

void CGNS::Private::checkPointer(char const * iPointer) {
	if (!iPointer) Emit(Exception::NULL_STRING_POINTER);
}

void CGNS::Private::checkIndex(int iIndex, int iMax, int iMin) {
	if (iIndex<iMin) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	if (iIndex>=iMax) Emit(Exception::NUMBER_GREATER_THAN_MAXIMUM);
}

void CGNS::Private::checkString(char const * iString, int iMaxLength) {
	checkPointer(iString);
	if (!*iString) Emit(Exception::EMPTY_STRING);
	if (iMaxLength && static_cast<int>(strlen(iString))>iMaxLength)
		Emit(Exception::STRING_TOO_LONG);
}

#ifdef HAVE_NAMESPACE_INSTANTIATION
#define INSTANTIATE_AS_STRING(type) \
template std::string CGNS::asString<CGNS::type>(CGNS::type)
#else
#define INSTANTIATE_AS_STRING(type) \
namespace CGNS { \
template std::string asString<CGNS::type>(CGNS::type); \
}
#endif

#define DO_CONVERT(type, names, last, error)                          \
bool CGNS::Private::convertN(std::string const & iString, CGNS::type * oData) {\
	return findName(iString, names, last, oData);                     \
}                                                                     \
void CGNS::Private::convert(std::string const & iString, CGNS::type * oData) { \
	if (!findName(iString, names, last, oData)) {                     \
		Emit(Exception::error); }                                     \
}                                                                     \
void CGNS::Private::convert(CGNS::type iData, std::string * oString) {         \
	if (iData<0 || iData>last) {                                      \
		Emit(Exception::error); }                                     \
	*oString=names[iData];                                            \
}                                                                     \
void CGNS::Private::writeADF(ADF::Node & iNode, CGNS::type iData) {   \
	iNode.writeData(asString(iData));                                 \
}                                                                     \
void CGNS::Private::readADF(ADF::Node const & iNode, CGNS::type & iData) {  \
	std::string str;                                                  \
	iNode.readData(&str);                                             \
	if (!convertN(str, &iData)) iData=last;                           \
}                                                                     \
INSTANTIATE_AS_STRING(type)


static char const * const stdLocations[CGNS::USER_LOCATION+1] = {	
	"Null", "Vertex", "CellCenter", "FaceCenter",
	"IFaceCenter", "JFaceCenter", "KFaceCenter",
	"EdgeCenter", "UserDefined" };
DO_CONVERT(GridLocation, stdLocations, USER_LOCATION, NO_LOCATION);

static char const * const stdElements[CGNS::Elements::NGON_n+1] = {	
	"Null", "UserDefined",
	"NODE",
	"BAR_2", "BAR_3",
	"TRI_3", "TRI_6", "QUAD_4", "QUAD_8", "QUAD_9",
	"TETRA_4", "TETRA_10", "PYRA_5", "PYRA_13", "PENTA_6", "PENTA_15", "PENTA_18",
	"HEXA_8", "HEXA_20", "HEXA_27",
	"MIXED", "NGON_n" };
DO_CONVERT(Elements::ElementType, stdElements, CGNS::Elements::NGON_n, NO_ELEMENT);

static char const * const stdSimulationTypes[CGNS::Base::USER_SIMULATION+1] = {
	"Null", "TimeAccurate", "NonTimeAccurate", "UserDefined" };
DO_CONVERT(Base::SimulationType, stdSimulationTypes,
	CGNS::Base::USER_SIMULATION, NO_SIMULATIONTYPE);

static char const * const stdConnectivityTypes[CGNS::GridConnectivity::USER_CONNECTIVITY+1] = {
	"Null", "Overset", "Abutting", "Abutting1to1", "UserDefined" };
DO_CONVERT(GridConnectivity::GridConnectivityType, stdConnectivityTypes,
	CGNS::GridConnectivity::USER_CONNECTIVITY, NO_CONNECTIVITY);

static char const * stdClasses[CGNS::USER_CLASS+1] = { "Null",
	"Dimensional", "NormalizedByDimensional", "NormalizedByUnknownDimensional",
	"NondimensionalParameter", "DimensionlessConstant", "UserDefined" };
DO_CONVERT(DataClass, stdClasses, USER_CLASS, NO_DATACLASS);

static char const * stdMasses[CGNS::USER_MASS+1] = { "Null",
	"Kilogram", "Gram", "Slug", "PoundMass", "UserDefined" };
static char const * stdLengths[CGNS::USER_LENGTH+1] = { "Null",
	"Meter", "Centimeter", "Millimeter", "Foot", "Inch", "UserDefined" };
static char const * stdTimes[CGNS::USER_TIME+1] = { "Null", "Second", "UserDefined" };
static char const * stdTemperatures[CGNS::USER_TEMPERATURE+1] = { "Null",
	"Kelvin", "Celsius", "Rankine", "Fahrenheit", "UserDefined" };
static char const * stdAngles[CGNS::USER_ANGLE+1] = { "Null",
	"Degree", "Radian", "UserDefined" };
DO_CONVERT(MassUnit, stdMasses, USER_MASS, NO_UNITS);
DO_CONVERT(LengthUnit, stdLengths, USER_LENGTH, NO_UNITS);
DO_CONVERT(TimeUnit, stdTimes, USER_TIME, NO_UNITS);
DO_CONVERT(TemperatureUnit, stdTemperatures, USER_TEMPERATURE, NO_UNITS);
DO_CONVERT(AngleUnit, stdAngles, USER_ANGLE, NO_UNITS);

static char const * stdQuantities[CGNS::DataArray::USER_DATA+1] = {
	"Null",	"CoordinateX", "CoordinateY", "CoordinateZ",
	"CoordinateR", "CoordinateTheta", "CoordinatePhi",
	"CoordinateNormal", "CoordinateTangential",
	"CoordinateXi", "CoordinateEta", "CoordinateZeta",
	"CoordinateTransform", "InterpolantsDonor", "ElementConnectivity", "ParentData",
	"Potential", "Streamfunction", "Density", "Pressure", "Temperature",
	"EnergyInternal", "Enthalpy", "Entropy", "EntropyApprox",
	"DensityStagnation", "PressureStagnation",
	"TemperatureStagnation", "EnergyStagnation",
	"EnthalpyStagnation", "EnergyStagnationDensity",
	"VelocityX", "VelocityY", "VelocityZ", "VelocityR", "VelocityTheta", "VelocityPhi",
	"VelocityMagnitude", "VelocityNormal", "VelocityTangential",
	"VelocitySound", "VelocitySoundStagnation",
	"MomentumX", "MomentumY", "MomentumZ", "MomentumMagnitude",
	"EnergyKinetic", "PressureDynamic", "VorticityX", "VorticityY", "VorticityZ",
	"VorticityMagnitude", "SkinFrictionX", "SkinFrictionY", "SkinFrictionZ",
	"SkinFrictionMagnitude", "VelocityAngleX", "VelocityAngleY", "VelocityAngleZ",
	"VelocityUnitVectorX", "VelocityUnitVectorY", "VelocityUnitVectorZ",
	"MassFlow",	"ViscosityKinematic", "ViscosityMolecular", "ViscosityEddyKinematic",
	"ViscosityEddy", "ThermalConductivity",	
	"ReynoldsStressXX", "ReynoldsStressXY", "ReynoldsStressXZ",
	"ReynoldsStressYY", "ReynoldsStressYZ", "ReynoldsStressZZ",
	"PowerLawExponent", "SutherlandLawConstant", "TemperatureReference",
	"ViscosityMolecularReference", "ThermalConductivityReference",
	"IdealGasConstant", "SpecificHeatPressure", "SpecificHeatVolume",
	"LengthReference",
	"TurbulentDistance", "TurbulentEnergyKinetic", "TurbulentDissipation",
	"TurbulentDissipationRate", "TurbulentBBReynolds", "TurbulentSANuTilde",
	"Mach", "Mach_Velocity", "Mach_VelocitySound",
	"Reynolds", "Reynolds_Velocity", "Reynolds_Length", "Reynolds_ViscosityKinematic",
	"Prandtl", "Prandtl_ThermalConductivity", "Prandtl_ViscosityMolecular",
	"Prandtl_SpecificHeatPressure", "PrandtlTurbulent",
	"SpecificHeatRatio", "SpecificHeatRatio_Pressure", "SpecificHeatRatio_Volume",
	"CoefPressure", "CoefSkinFrictionX", "CoefSkinFrictionY", "CoefSkinFrictionZ",
	"Coef_PressureDynamic", "Coef_PressureReference",
	"RiemannInvarianPlus", "RiemannInvariantMinus",
	"CharacteristicEntropy", "CharacteristicVorticity1", "CharacteristicVorticity2",
	"CharacteristicAcousticPlus", "CharacteristicAcousticMinus",
	"ForceX", "ForceY", "ForceZ", "ForceR", "ForceTheta", "ForcePhi",
	"Lift", "Drag", "MomentX", "MomentY", "MomentZ",
	"MomentR", "MomentTheta", "MomentPhi", "MomentXi", "MomentEta", "MomentZeta",
	"Moment_CenterX", "Moment_CenterY", "Moment_CenterZ",
	"CoefLift", "CoefDrag", "CoefMomentX", "CoefMomentY", "CoefMomentZ",
	"CoefMomentR", "CoefMomentTheta", "CoefMomentPhi",
	"CoefMomentXi", "CoefMomentEta", "CoefMomentZeta",
	"Coef_Area", "Coef_Length",
	"TimeValues", "IterationValues", "NumberOfZones", "NumberOfFamilies",
	"ZonePointers", "FamilyPointers",
	"RigidGridMotionPointers", "ArbitraryGridMotionPointers",
	"GridCoordinatesPointers", "FlowSolutionPointers",
	"OriginLocation", "RigidRotationAngle", "RigidVelocity", "RigidRotationRate",
	"GridVelocityX", "GridVelocityY", "GridVelocityZ",
	"GridVelocityR", "GridVelocityTheta", "GridVelocityPhi",
	"GridVelocityXi", "GridVelocityEta", "GridVelocityZeta",
	// from version 2.1 on
	"FuelAirRatio", "ReferenceTemperatureHOF", "EnthalpyEnergyRatio",
	"CompressibilityFactor", "VibrationalElectronEnergy", "HeatOfFormation",
	"VibrationalElectronTemperature",
	"UserDefined"
};
DO_CONVERT(DataArray::Quantity, stdQuantities, CGNS::DataArray::USER_DATA, NO_QUANTITY);

static char const * stdBCTypes[CGNS::BC::USER_BC+1] = {
	"Null",
	// BCTypeSimple_t
	"BCGeneral", "BCDirichlet", "BCNeumann", "BCExtrapolate",
	"BCWallInviscid", "BCWallViscousHeatFlux", "BCWallViscousIsothermal",
	"BCWallViscous", "BCWall",
	"BCInflowSubsonic", "BCInflowSupersonic",
	"BCOutflowSubsonic", "BCOutflowSupersonic",
	"BCTunnelInflow", "BCTunnelOutflow",
	"BCDegenerateLine", "BCDegeneratePoint",
	"BCSymmetryPlane", "BCSymmetryPolar", "BCAxisymmetricWedge",
	// BCTypeCompound_t
	"BCInflow", "BCOutflow", "BCFarfield",
	"UserDefined"
};
DO_CONVERT(BC::BCType, stdBCTypes, CGNS::BC::USER_BC, NO_BCTYPE);

static char const * stdGETypes[CGNS::GoverningEquations::USER_EQUATION+1] = {
	"Null", "FullPotential", "Euler", "NSLaminar", "NSTurbulent",
	"NSLaminarIncompressible", "NSTurbulentIncompressible", "UserDefined" };
DO_CONVERT(GoverningEquations::GoverningEquationsType, stdGETypes,
	CGNS::GoverningEquations::USER_EQUATION, NO_EQUATIONS);


static char const * stdGMTypes[CGNS::GasModel::USER_GAS+1] = {
	"Null", "Ideal", "VanderWaals",
    // from version 2.1 on
    "CaloricallyPerfect", "ThermallyPerfect", "ConstantDensity", "RedlichKwong",
	"UserDefined" };
DO_CONVERT(GasModel::GasModelType, stdGMTypes, CGNS::GasModel::USER_GAS, NO_GASMODEL);

static char const * stdVMTypes[CGNS::ViscosityModel::USER_VISCOSITY+1] = {
	"Null", "Constant", "PowerLaw", "SutherlandLaw", "UserDefined" };
DO_CONVERT(ViscosityModel::ViscosityModelType, stdVMTypes,
	CGNS::ViscosityModel::USER_VISCOSITY, NO_VISCOSITYMODEL);

static char const * stdTCMTypes[CGNS::ThermalConductivityModel::USER_TC+1] = {
	"Null", "ConstantPrandtl", "PowerLaw", "SutherlandLaw", "UserDefined" };
DO_CONVERT(ThermalConductivityModel::ThermalConductivityModelType, stdTCMTypes,
	CGNS::ThermalConductivityModel::USER_TC, NO_THERMALCONDUCTIVITYMODEL);

static char const * stdTCTypes[CGNS::TurbulenceClosure::USER_CLOSURE+1] = {
	"Null", "EddyViscosity", "ReynoldsStress", "ReynoldsStressAlgebraic", "UserDefined" };
DO_CONVERT(TurbulenceClosure::TurbulenceClosureType, stdTCTypes,
	CGNS::TurbulenceClosure::USER_CLOSURE, NO_TURBULENCECLOSURE);

static char const * stdTMTypes[CGNS::TurbulenceModel::USER_TURBULENCE+1] = {
	"Null", "Algebraic_BaldwinLomax", "Algebraic_CebeciSmith",
	"HalfEquation_JohnsonKing", "OneEquation_BaldwinBarth", "OneEquation_SpalartAllmaras",
	"TwoEquation_JonesLaunder", "TwoEquation_MenterSST", "TwoEquation_Wilcox", "UserDefined" };
DO_CONVERT(TurbulenceModel::TurbulenceModelType, stdTMTypes,
	CGNS::TurbulenceModel::USER_TURBULENCE, NO_TURBULENCEMODEL);

static char const * stdGFTypes[CGNS::GeometryReference::USER_FORMAT+1] = {
	"Null", "NASA-IGES", "SDRC", "Unigraphics", "ProEngineer", "ICEM-CFD", "UserDefined" };
DO_CONVERT(GeometryReference::GeometryFormat, stdGFTypes,
	CGNS::GeometryReference::USER_FORMAT, NO_GEOMETRYFORMAT);

static char const * stdRGMTypes[CGNS::RigidGridMotion::USER_MOTION+1] = {
	"Null", "ConstantRate", "VariableRate", "UserDefined" };
DO_CONVERT(RigidGridMotion::RigidGridMotionType, stdRGMTypes,
	CGNS::RigidGridMotion::USER_MOTION, NO_RIGIDGRIDMOTION);

static char const * stdAGMTypes[CGNS::ArbitraryGridMotion::USER_MOTION+1] = {
	"Null", "NonDeformingGrid", "DeformingGrid", "UserDefined" };
DO_CONVERT(ArbitraryGridMotion::ArbitraryGridMotionType, stdAGMTypes,
	CGNS::ArbitraryGridMotion::USER_MOTION, NO_ARBITRARYGRIDMOTION);

static char const * stdTRMTypes[CGNS::ThermalRelaxationModel::USER_RELAXATION+1] = {
    "Null", "Frozen", "ThermalEquilib", "ThermalNonequilib", "UserDefined" };
DO_CONVERT(ThermalRelaxationModel::ThermalRelaxationModelType, stdTRMTypes,
	CGNS::ThermalRelaxationModel::USER_RELAXATION, NO_THERMALRELAXATIONMODEL);

static char const * stdCKMTypes[CGNS::ChemicalKineticsModel::USER_KINETICS+1] = {
    "Null", "Frozen", "ChemicalEquilibCurveFit", "ChemicalEquilibMinimization",
    "ChemicalNonequilib", "UserDefined" };
DO_CONVERT(ChemicalKineticsModel::ChemicalKineticsModelType, stdCKMTypes,
	CGNS::ChemicalKineticsModel::USER_KINETICS, NO_CHEMICALKINETICSMODEL);

#undef DO_CONVERT

#define MAP_CONVERT(class, type, null, user)                  \
bool CGNS::Private::convertN(std::string const & iString,     \
	CGNS::class::type * oData){                               \
	CGNS::DataArray::Quantity d;                              \
	if (!convertN(iString, &d)) return false;                 \
	if (d==0) { *oData=CGNS::class::null; return true; }      \
	if (d==CGNS::DataArray::USER_DATA) {                      \
		*oData=CGNS::class::user; return true; }              \
	if (d<static_cast<int>(CGNS::class::null)                 \
		|| d>static_cast<int>(CGNS::class::user))             \
		  return false;                                       \
	*oData=static_cast<CGNS::class::type>(d+0);               \
	return true;                                              \
}                                                             \
void CGNS::Private::convert(std::string const & iString,      \
	CGNS::class::type * oData) {                              \
	if (!convertN(iString, oData)) {                          \
		Emit(Exception::NO_QUANTITY); }                       \
}                                                             \
void CGNS::Private::convert(CGNS::class::type iData,          \
	std::string * oString) {                                  \
	if (iData<CGNS::class::null || iData>CGNS::class::user) { \
		Emit(Exception::NO_QUANTITY); }                       \
	convert(static_cast<CGNS::DataArray::Quantity>(iData+0),  \
		oString);                                             \
}                                                             \
INSTANTIATE_AS_STRING(class::type)

MAP_CONVERT(GridCoordinates, CoordinateType, NULL_COORDINATE, USER_COORDINATE);
MAP_CONVERT(Discreted, FieldType, NULL_FIELD, USER_FIELD);

#undef MAP_CONVERT

void CGNS::Private::checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1) {
	if (iNode.getDataType()!=iType) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=1) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1, int iDim2) {
	if (iNode.getDataType()!=iType) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=2) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
	if (iDim2>0 && iNode.getDimension(1)!=iDim2) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1, int iDim2, int iDim3) {
	if (iNode.getDataType()!=iType) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=3) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
	if (iDim2>0 && iNode.getDimension(1)!=iDim2) Emit(Exception::WRONG_DIMENSION);
	if (iDim3>0 && iNode.getDimension(2)!=iDim3) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNodeReal(ADF::Node const & iNode, int iDim1) {
	ADF::DataType type=iNode.getDataType();
	if (type!=ADF::R4 && type!=ADF::R8) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=1) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNodeReal(ADF::Node const & iNode, int iDim1, int iDim2) {
	ADF::DataType type=iNode.getDataType();
	if (type!=ADF::R4 && type!=ADF::R8) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=2) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
	if (iDim2>0 && iNode.getDimension(1)!=iDim2) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNodeReal(ADF::Node const & iNode, int iDim1, int iDim2, int iDim3) {
	ADF::DataType type=iNode.getDataType();
	if (type!=ADF::R4 && type!=ADF::R8) Emit(Exception::WRONG_DATATYPE);
	if (iNode.getNumDimensions()!=3) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (iDim1>0 && iNode.getDimension(0)!=iDim1) Emit(Exception::WRONG_DIMENSION);
	if (iDim2>0 && iNode.getDimension(1)!=iDim2) Emit(Exception::WRONG_DIMENSION);
	if (iDim3>0 && iNode.getDimension(2)!=iDim3) Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::checkNodeAll(ADF::Node const & iNode, ADF::DataType iType,
	int iAllDim, int iMaxNumDims) {
	if (iNode.getDataType()!=iType) Emit(Exception::WRONG_DATATYPE);
	int numDims=iNode.getNumDimensions();
	if (iMaxNumDims && numDims>iMaxNumDims) Emit(Exception::WRONG_NUM_DIMENSIONS);
	if (!iAllDim) return;
	int dims=1;
	for (int d=0; d<numDims; ++d)
		dims*=iNode.getDimension(d);
	if (dims!=iAllDim) Emit(Exception::WRONG_DIMENSION);

}

void CGNS::Private::deleteLabeledChildren(ADF::Node & iNode, std::string const & iLabel) {
	for (ADF::Node::Child_iterator_t i=iNode.beginChild(); i!=iNode.endChild(); ++i) {
		if (i->getLabel()==iLabel)
			iNode.deleteChild(*i);
	}
}

void CGNS::Private::warnRequired(char const * iElementName, char const * iParentName) {
	std::cerr << "Missing required element: " << iElementName << " in node "
		<< iParentName << std::endl;
}

void CGNS::Private::checkRequired(ADF::Node const & iNode,
	char const * iElementName, char const * iParentName) {
	if (!iNode.hasChild(iElementName)) warnRequired(iElementName, iParentName);
}

void CGNS::Private::readADF(ADF::Node const & iNode, int & cDest) {
	checkNode(iNode, ADF::I4, 1);
	iNode.readData(&cDest);
}

void CGNS::Private::readADF(ADF::Node const & iNode, std::string & cDest) {
	checkNode(iNode, ADF::C1, 0);
	iNode.readData(&cDest);
}

void CGNS::Private::readADF(ADF::Node const & iNode, _Range1D & cDest) {
	checkNodeAll(iNode, ADF::I4, 2, 2);
/*	
	// have to do it manually, since it may be simply 2 or 1*2
	if (iNode.getDataType()!=ADF::I4) Emit(Exception::WRONG_DATATYPE);
	int numDim=iNode.getNumDimensions();
	if (numDim!=1 && numDim!=2) Emit(Exception::WRONG_NUM_DIMENSIONS);
	int dim=iNode.getDimension(0);
	if (numDim==2) dim*=iNode.getDimension(1);
	if (dim!=2) Emit(Exception::WRONG_DIMENSION);
*/	
#if 0	
	int data[2];
	iNode.readData(data);
	cDest.mMinI=data[0];
	cDest.mMaxI=data[1];
#else
	iNode.readData(&cDest.mMinI);
#endif	
}

void CGNS::Private::readADF(ADF::Node const & iNode, _Range2D & cDest) {
	checkNodeAll(iNode, ADF::I4, 4, 2);
#if 0	
	int data[4];
	iNode.readData(data);
	cDest.mMinI=data[0];
	cDest.mMaxI=data[2];
	cDest.mMinJ=data[1];
	cDest.mMaxJ=data[3];
#else
	iNode.readData(&cDest.mMinI);
#endif	
}

void CGNS::Private::readADF(ADF::Node const & iNode, _Range3D & cDest) {
	checkNodeAll(iNode, ADF::I4, 6, 2);
#if 0	
	int data[6];
	iNode.readData(data);
	cDest.mMinI=data[0];
	cDest.mMaxI=data[3];
	cDest.mMinJ=data[1];
	cDest.mMaxJ=data[4];
	cDest.mMinK=data[2];
	cDest.mMaxK=data[5];
#else
	iNode.readData(&cDest.mMinI);
#endif	
}

void CGNS::Private::readADF(ADF::Node const & iNode, Point & cDest) {
	checkNodeReal(iNode, 0);
	int dims=iNode.getDimension(0);
	switch (dims) {
		case 1: iNode.readData(&cDest.m1D.mX); return;
		case 2: iNode.readData(&cDest.m2D.mX); return;
		case 3: iNode.readData(&cDest.m3D.mX); return;
	}
	Emit(Exception::WRONG_DIMENSION);
}

void CGNS::Private::writeADF(ADF::Node & iNode, int iValue) {
	iNode.setDataTypeDimensions(ADF::I4, 1);
	iNode.writeData(&iValue);
}

void CGNS::Private::writeADF(ADF::Node & iNode, std::string const & iValue) {
	iNode.writeData(iValue);
}

void CGNS::Private::writeADF(ADF::Node & iNode, _Range1D const & cDest) {
	iNode.setDataTypeDimensions(ADF::I4, 1, 2);
	iNode.writeData(&cDest.mMinI);
}

void CGNS::Private::writeADF(ADF::Node & iNode, _Range2D const & cDest) {
	iNode.setDataTypeDimensions(ADF::I4, 2, 2);
	iNode.writeData(&cDest.mMinI);
}

void CGNS::Private::writeADF(ADF::Node & iNode, _Range3D const & cDest) {
	iNode.setDataTypeDimensions(ADF::I4, 3, 2);
	iNode.writeData(&cDest.mMinI);
}

CGNS::PreReadStrategy CGNS::Private::SStrategy=CGNS::FULL_PREREAD;

CGNS::PreReadStrategy CGNS::SetStrategy(PreReadStrategy iStrategy) {
	Private::checkIndex(iStrategy, FULL_PREREAD, ON_DEMAND_PREREAD+1);
	PreReadStrategy old=Private::SStrategy;
	Private::SStrategy=iStrategy;
	return old;
}

CGNS::PreReadStrategy CGNS::GetStrategy() { return Private::SStrategy; }
