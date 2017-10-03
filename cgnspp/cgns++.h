/***************************************************************************
                          cgns++.h  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSPP_H
#define CGNSPP_H

#include "adf++.h"

#include <exception>
#include <iterator>
#include <string>


namespace CGNS {

// using declarations
using ADF::refcnt_ptr;
using ADF::iterator;

// forward declarations
class Exception;
class File;
class Node;

// the different SIDS nodes
class Base;
class BaseIterativeData;
class Zone;
class ZoneIterativeData;
class GridCoordinates;
class FlowSolution;
class DiscreteData;
class ZoneGridConnectivity;
class RigidGridMotion;
class ArbitraryGridMotion;

class GridConnectivity;
class GridConnectivity1to1;
class OversetHoles;

class ZoneBC;
class BC;
class BCDataSet;

class Family;
class GeometryReference;
class Elements;

class ReferenceState;
class BCData;

class ConvergenceHistory;
class FlowEquationSet;
class GoverningEquations;

class GasModel;
class ViscosityModel;
class ThermalConductivityModel;
class TurbulenceModel;
class TurbulenceClosure;

class DataArray;
class IndexArray;

class UserDefinedData;
class ThermalRelaxationModel;
class ChemicalKineticsModel;

// the internally used base classes
//class Parented;
class Pointed;
class Dimensioned;
class Stated;
class Arrayed;
class Converged;
class Gridded;
class Valued;
class Discreted;

//typedef Valued IntegralData;
class IntegralData;

// all the implementation classes
namespace Private {
class File;
class Node;
class Base;
class Zone;
class GridCoordinates;
class FlowSolution;
class DiscreteData;
class ZoneGridConnectivity;
class GridConnectivity;
class GridConnectivity1to1;
class OversetHoles;
class Family;
class GeometryReference;
class Elements;
class ZoneBC;
class BC;
class BCDataSet;
class ReferenceState;
class BCData;
class ConvergenceHistory;
class FlowEquationSet;
class DataArray;
class Pointed;
class Dimensioned;
class Stated;
class Arrayed;
class Converged;
class BaseIterativeData;
class ZoneIterativeData;
class RigidGridMotion;
class ArbitraryGridMotion;
class GoverningEquations;
class GasModel;
class ViscosityModel;
class ThermalConductivityModel;
class TurbulenceModel;
class TurbulenceClosure;

class Parented;
class Gridded;
class Valued;
class Discreted;
class DiffusionModel_t;
class IterativeData;

class IndexArray;
class UserDefinedData;
class ThermalRelaxationModel;
class ChemicalKineticsModel;
// the versatile information broker and node creator class
class Broker;
} // namespace Private

/** Possible values for the position of field data */
enum GridLocation {
	NULL_LOCATION,		///< Unspecified position
	VERTEX,				///< Located at vertices
	CELL_CENTER,		///< Located at cell centers
	FACE_CENTER,		///< Located at face centers
	I_FACE_CENTER,		///< Located at centers of I faces
	J_FACE_CENTER,		///< Located at centers of J faces
	K_FACE_CENTER,		///< Located at centers of K faces
	EDGE_CENTER,		///< Located at edge centers
	USER_LOCATION		///< Userdefined position
};

/** Converts various enum values into a string representation */
template<typename T>
std::string asString(T iT);

enum DataClass { NULL_CLASS,
	DIMENSIONAL, NORMALIZED_BY_DIMENSIONAL, NORMALIZED_BY_UNKNOWN_DIMENSIONAL,
	NONDIMENSIONAL_PARAMETER, DIMENSIONLESS_CONSTANT, USER_CLASS };
enum MassUnit { NULL_MASS,
	KILOGRAM, GRAM, SLUG, POUND_MASS, POUND=POUND_MASS, USER_MASS };
enum LengthUnit { NULL_LENGTH,
	METER, CENTIMETER, MILLIMETER, FOOT, INCH, USER_LENGTH };
enum TimeUnit { NULL_TIME, SECOND, USER_TIME };
enum TemperatureUnit { NULL_TEMPERATURE,
	KELVIN, CELSIUS, RANKINE, FAHRENHEIT, USER_TEMPERATURE };
enum AngleUnit { NULL_ANGLE, DEGREE, RADIAN, USER_ANGLE };
enum DiffusionIndex { II_INDEX, JJ_INDEX, KK_INDEX, IJ_INDEX, JK_INDEX, KI_INDEX,
	JI_INDEX=IJ_INDEX, IK_INDEX=KI_INDEX, KJ_INDEX=JK_INDEX };
struct DimensionalUnits {
	MassUnit mMass;
	LengthUnit mLength;
	TimeUnit mTime;
	TemperatureUnit mTemperature;
	AngleUnit mAngle;
};
	
	
// this layout corresponds to the one in the ADF node, so keep it!
struct _Range1D {
	int mMinI, mMaxI;
};
struct _Range2D {
	int mMinI, mMinJ;
	int mMaxI, mMaxJ;
};
struct _Range3D {
	int mMinI, mMinJ, mMinK;
	int mMaxI, mMaxJ, mMaxK;
};

struct _Index1D {
	int mI;
};
struct _Index2D {
	int mI, mJ;
};
struct _Index3D {
	int mI, mJ, mK;
};

struct _Coor1D {
	double mX;
};
struct _Coor2D {
	double mX, mY;
};
struct _Coor3D {
	double mX, mY, mZ;
};


#define UNIONISE_STRUCTS(name) \
union name { name##1D m1D; name##2D m2D; name##3D m3D; \
	name() {} \
	name(name##1D i1D) : m1D(i1D) {} \
	name(name##2D i2D) : m2D(i2D) {} \
	name(name##3D i3D) : m3D(i3D) {} \
}

#define DEFINE_UNIQUE_STRUCTS(name, base) \
struct name##1D : base##1D {}; \
struct name##2D : base##2D {}; \
struct name##3D : base##3D {}; \
UNIONISE_STRUCTS(name)

DEFINE_UNIQUE_STRUCTS(IndexRange, _Range);
DEFINE_UNIQUE_STRUCTS(Rind, _Range);
DEFINE_UNIQUE_STRUCTS(Transform, _Index);
DEFINE_UNIQUE_STRUCTS(Index, _Index);
DEFINE_UNIQUE_STRUCTS(Normal, _Index);
DEFINE_UNIQUE_STRUCTS(Strided, _Index);
DEFINE_UNIQUE_STRUCTS(Point, _Coor);

struct StridedRange1D : IndexRange1D, Strided1D {};
struct StridedRange2D : IndexRange2D, Strided2D {};
struct StridedRange3D : IndexRange3D, Strided3D {};

UNIONISE_STRUCTS(StridedRange);

struct Descriptor {
	Descriptor() {}
	Descriptor(std::string const & iName, std::string const & iContent)
		: mName(iName), mContent(iContent) {}
	std::string const & getName() const;
	std::string const & getContent() const;
	void readData(std::string * oData) const;
	void readData(char * oData) const;
	void readData(signed char * oData) const;
	void readData(unsigned char * oData) const;
	void readData(void * oData) const;
	std::string const mName, mContent;
};

/** The strategy to adhere to regarding preread and caching */
enum PreReadStrategy {
	FULL_PREREAD,		///< Preread all the structure on file open
	INTERNAL_PREREAD,	///< Preread only the immediate node data on first node access
	ON_DEMAND_PREREAD	///< Do not preread, but cache on first access
//	NO_PREREAD			///< Do not cache at all, access everything directly
};
PreReadStrategy SetStrategy(PreReadStrategy iStrategy);
PreReadStrategy GetStrategy();


#define CHILD_INTERFACE_WRITE(name, type) \
type write##name()

#define CHILD_INTERFACE_WRITE1(name, type, p1) \
type write##name(p1)

#define CHILD_INTERFACE_WRITE2(name, type, p1, p2) \
type write##name(p1, p2)

#define CHILD_INTERFACE_WRITE3(name, type, p1, p2, p3)         \
type write##name(p1, p2, p3)

#define CHILD_INTERFACE(name, type) \
type get##name() const

#define CHILD_INTERFACE_D(name, type) \
void get##name(type *) const

#define CHILD_INTERFACE_ALL_D(name, type) \
CHILD_INTERFACE_D(name, type##1D); \
CHILD_INTERFACE_D(name, type##2D); \
CHILD_INTERFACE_D(name, type##3D) \

#define CHILD_INTERFACE_WRITE_ALL_D(name, type) \
CHILD_INTERFACE_WRITE1(name, type##1D, type##1D); \
CHILD_INTERFACE_WRITE1(name, type##2D, type##2D); \
CHILD_INTERFACE_WRITE1(name, type##3D, type##3D) \

#define OPTIONAL_CHILD(name) \
bool has##name() const;      \
void delete##name()

#define OPTIONAL_CHILD_INTERFACE(name, type) \
CHILD_INTERFACE(name, type);                 \
OPTIONAL_CHILD(name)

#define CHILD_INTERFACE_WRITE_D(name, type) \
CHILD_INTERFACE_ALL_D(name, type);          \
CHILD_INTERFACE_WRITE_ALL_D(name, type)

#define OPTIONAL_CHILD_INTERFACE_WRITE_D(name, type) \
CHILD_INTERFACE_WRITE_D(name, type);                 \
OPTIONAL_CHILD(name)

#define OPTIONAL_CHILD_INTERFACE_WRITE(name, type) \
OPTIONAL_CHILD_INTERFACE(name, type);              \
CHILD_INTERFACE_WRITE(name, type)

#define OPTIONAL_CHILD_INTERFACE_WRITE1(name, type, p1) \
OPTIONAL_CHILD_INTERFACE(name, type);                   \
CHILD_INTERFACE_WRITE1(name, type, p1)

#define OPTIONAL_CHILD_INTERFACE_WRITE2(name, type, p1, p2) \
OPTIONAL_CHILD_INTERFACE(name, type);                       \
CHILD_INTERFACE_WRITE2(name, type, p1, p2)

#define OPTIONAL_CHILD_INTERFACE_WRITE3(name, type, p1, p2, p3) \
OPTIONAL_CHILD_INTERFACE(name, type);                           \
CHILD_INTERFACE_WRITE3(name, type, p1, p2, p3)

#define CHILDS_INTERFACE_WRITE(name, type) \
CHILD_INTERFACE_WRITE1(name, type, std::string const &)

#define CHILDS_INTERFACE_WRITE1(name, type, p1) \
CHILD_INTERFACE_WRITE2(name, type, std::string const &, p1)

#define CHILDS_INTERFACE_WRITE2(name, type, p1, p2) \
CHILD_INTERFACE_WRITE3(name, type, std::string const &, p1, p2)

#define CHILDS_INTERFACE_WRITE_ALL_D(name, type) \
CHILDS_INTERFACE_WRITE1(name, type##1D, type##1D); \
CHILDS_INTERFACE_WRITE1(name, type##2D, type##2D); \
CHILDS_INTERFACE_WRITE1(name, type##3D, type##3D) \

#define N_EMPTY_CHILDS_INTERFACE(name, type)     \
int getNum##name() const;                        \
typedef iterator<type> name##_iterator_t;        \
name##_iterator_t begin##name() const;           \
name##_iterator_t end##name() const;             \
void delete##name(std::string const & iName)

#define N_CHILDS_INTERFACE(name, type)           \
N_EMPTY_CHILDS_INTERFACE(name, type);            \
bool has##name(std::string const & iName) const; \
type get##name(std::string const & iName) const

#define N_CHILDS_INTERFACE_WRITE(name, type) \
N_CHILDS_INTERFACE(name, type);                \
CHILDS_INTERFACE_WRITE(name, type)

#define N_CHILDS_INTERFACE_WRITE1(name, type, p1) \
N_CHILDS_INTERFACE(name, type);                     \
CHILDS_INTERFACE_WRITE1(name, type, p1)

#define N_CHILDS_INTERFACE_WRITE2(name, type, p1, p2) \
N_CHILDS_INTERFACE(name, type);                         \
CHILDS_INTERFACE_WRITE2(name, type, p1, p2)


/** This exception is thrown when an error occurs calling a CGNS function.
  */
class Exception : public std::exception  {
public:
	enum { NO_ERROR=0, ADFLIB_ERROR,
		CGNSPP_FIRST_ERROR=256,
		NULL_POINTER, NULL_STRING_POINTER, EMPTY_STRING,
		STRING_TOO_LONG,
		NUMBER_LESS_THAN_MINIMUM, NUMBER_GREATER_THAN_MAXIMUM,
		INDEX_TOO_LOW, INDEX_TOO_HIGH,
		NO_CGNS_NODE, NO_CGNS_PARENT,
		ILLEGAL_OPEN_MODE, MULTIPLE_LIBRARY_VERSION_NODES, UNKNOWN_VERSION,
		NO_NAMED_CHILD, NO_LABELED_CHILD,
		UNKNOWN_DATA_ARRAY, UNKNOWN_QUANTITY, INVALID_LOCATION,
		NO_QUANTITY, NO_EXPONENTS, NO_UNITS, NO_LOCATION, NO_ELEMENT,
		NO_SIMULATIONTYPE, NO_CONNECTIVITY, NO_BCTYPE,
		NO_EQUATIONS, NO_GASMODEL, NO_VISCOSITYMODEL,
		NO_THERMALCONDUCTIVITYMODEL, NO_TURBULENCECLOSURE, NO_TURBULENCEMODEL,
		NO_THERMALRELAXATIONMODEL, NO_CHEMICALKINETICSMODEL,
		NO_GEOMETRYFORMAT, NO_RIGIDGRIDMOTION, NO_ARBITRARYGRIDMOTION,
		NO_DATACLASS, NO_STATE, NO_PARENT_NODE,
		WRONG_DATATYPE, WRONG_NUM_DIMENSIONS, WRONG_DIMENSION, ILLEGAL_DATA,
		ILLEGAL_RANGE,
		TODO=510, INTERNAL_ERROR=511
	};
	explicit Exception(int iErrorCode) throw();
	virtual ~Exception() throw();
	
	int getErrorCode() const throw();
	char const * getErrorMessage() const throw();
	virtual char const * what() const throw();
	
private:
	int mErrorCode;
};

/** Represents an ADF file according to the CGNS specifications
  */
class File {
public:
	static double getLibraryVersion();
	
	/** Possible open modes */
	enum OpenMode {
		WRITE,	///< Write only mode.
		READ,	///< Read only mode.
		MODIFY	///< Reading and/or writing is allowed.
	};
	/** How many levels deep to search for base nodes */
	enum SearchBaseDown {
		STRICT,
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_ALL=-1
	};
	/** Opens a CGNS database with specified open mode. */
	explicit File(std::string const & iFileName,
		OpenMode iMode=READ, SearchBaseDown iSearch=STRICT);
	/** Destroys this CGNS database handle and close the file. */
	~File();

	/** Gets the CGNS library version this file was created with. */
	double getFileVersion() const;
	
	N_CHILDS_INTERFACE(Base, Base);

	/// \todo Perhaps enum for cell and physical dimensions?
	/** Writes a new CGNSBase_t node and makes it available. */
	Base writeBase(std::string const & iBaseName,
		int iCellDimension, int iPhysicalDimension);
		
private:
	refcnt_ptr<Private::File> mImpl;

}; // CGNS::File


/** Represents an arbitrary SIDS compatible node in a CGNS file
  */
class Node {
public:
	/** Gets the name of this node. */
	std::string getName() const;
	/** Gets the label of this node. */
	std::string getLabel() const;

    /** Gets the number of descriptors. */
    int getNumDescriptors() const;
    /** Asks if there is a named descriptor. */
    bool hasDescriptor(std::string const & iDescriptorName) const;
    /** Gets the text of the named descriptor. */
    std::string getDescriptorText(std::string const & iDescriptorName) const;
    /** Writes a descriptor with name and text. If the name already exists,
      * the text is modified. */
    std::string const & writeDescriptor(std::string const & iDescriptorName,
    	std::string const & iDescriptorText);
    /** Deletes the named descriptor. */
    void deleteDescriptor(std::string const & iDescriptorName);

    typedef iterator<Descriptor> Descriptor_iterator_t;
    Descriptor_iterator_t beginDescriptor() const;
    Descriptor_iterator_t endDescriptor() const;

    OPTIONAL_CHILD_INTERFACE_WRITE1(Ordinal, int, int);

	N_CHILDS_INTERFACE_WRITE(UserDefinedData, UserDefinedData);
	
	/** Flushes this node's data to disk. */
	void flush();

    /** Gets the ADF id to tamper directly with the ADF file. */
    double getDatabaseId() const;
	
protected:
	/** Creates this basic node from implementation data. */
	Node(Private::Node * iImpl);
	/** Copies the node. */
	Node(Node const & iRhs);
	/** Deletes this node. */
	~Node();
	
	/** Implementation data */
	refcnt_ptr<Private::Node> mImpl;
private:	
	Node const & operator=(Node const & iRhs);
friend class Private::Broker;	
}; // CGNS::Node

/** Represents a SIDS node holding a Elements_t.
  */
class Elements : public Node {
public:
	enum ElementType { NULL_ELEMENT, USER_ELEMENT,
		NODE,
		BAR_2, BAR_3,
		TRI_3, TRI_6, QUAD_4, QUAD_8, QUAD_9,
		TETRA_4, TETRA_10, PYRA_5, PYRA_13, PENTA_6, PENTA_15, PENTA_18,
		HEXA_8, HEXA_20, HEXA_27,
		MIXED, NGON_n, 
	};

	/** Gets the element type of this node. */
	ElementType getElementType() const;
	
	/** Gets the number of nodes on the boundary. Zero may mean unknown. */
	int getNodesOnBoundary() const;
	
	CHILD_INTERFACE(ElementRange, IndexRange1D);
	CHILD_INTERFACE_WRITE1(ElementRange, IndexRange1D, IndexRange1D);
	
	CHILD_INTERFACE(ElementConnectivity, DataArray);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(ParentData, DataArray);
	
	/** Creates these elements from implementation data. */
	Elements(Private::Elements * iImpl);
}; // CGNS::Elements


/** Represents a SIDS node holding a ZoneGridConnectivity_t.
  */
class ZoneGridConnectivity : public Node {
public:
	N_CHILDS_INTERFACE_WRITE1(GridConnectivity1to1, GridConnectivity1to1, std::string const & iDonorZoneName);
	CHILDS_INTERFACE_WRITE1(GridConnectivity1to1, GridConnectivity1to1, Zone const & iDonorZone);
	
	N_CHILDS_INTERFACE_WRITE1(GridConnectivity, GridConnectivity, std::string const & iDonorZoneName);
	CHILDS_INTERFACE_WRITE1(GridConnectivity, GridConnectivity, Zone const & iDonorZone);
	
	N_CHILDS_INTERFACE_WRITE(OversetHoles, OversetHoles);
	
	/** Creates this connectivity from implementation data. */
	ZoneGridConnectivity(Private::ZoneGridConnectivity * iImpl);
}; // CGNS::ZoneGridConnectivity


/** Represents a SIDS node holding a GridConnectivity1to1_t.
  */
class GridConnectivity1to1 : public Node {
public:
	/** Gets the name of the donor zone. */
	std::string getZoneDonorName() const;

	CHILD_INTERFACE(Transform, Transform);
	OPTIONAL_CHILD_INTERFACE_WRITE_D(Transform, Transform);
	
	CHILD_INTERFACE(PointRange, IndexRange);
	CHILD_INTERFACE_WRITE_D(PointRange, IndexRange);
	
	CHILD_INTERFACE(PointRangeDonor, IndexRange);
	CHILD_INTERFACE_WRITE_D(PointRangeDonor, IndexRange);

	/** Creates this connectivity from implementation data. */
	GridConnectivity1to1(Private::GridConnectivity1to1 * iImpl);
}; // CGNS::GridConnectivity1to1


/** Represents a SIDS node holding a GridConnectivity_t.
  */
class GridConnectivity : public Node {
public:
	/** Gets the name of the donor zone. */
	std::string getZoneDonorName() const;

	/** Gets the location of the solution data. */
	CHILD_INTERFACE(Location, GridLocation);
	CHILD_INTERFACE_WRITE1(Location, GridLocation, GridLocation);
	
	OPTIONAL_CHILD_INTERFACE(PointRange, IndexRange);
	CHILD_INTERFACE_WRITE_D(PointRange, IndexRange);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(PointList, IndexArray);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(CellListDonor, IndexArray);
	OPTIONAL_CHILD_INTERFACE_WRITE(PointListDonor, IndexArray);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(InterpolantsDonor, DataArray);
	
	enum GridConnectivityType { NULL_CONNECTIVITY,
		OVERSET, ABUTTING, ABUTTING1TO1, USER_CONNECTIVITY };
	
	CHILD_INTERFACE(GridConnectivityType, GridConnectivityType);
	CHILD_INTERFACE_WRITE1(GridConnectivityType, GridConnectivityType, GridConnectivityType);

	/** Creates this connectivity from implementation data. */
	GridConnectivity(Private::GridConnectivity * iImpl);
}; // CGNS::GridConnectivity


/** Represents a SIDS node holding a OversetHoles_t.
  */
class OversetHoles : public Node {
public:
	/** Gets the location of the solution data. */
	CHILD_INTERFACE(Location, GridLocation);
	CHILD_INTERFACE_WRITE1(Location, GridLocation, GridLocation);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(PointList, IndexArray);
	
	N_CHILDS_INTERFACE_WRITE1(PointRange, IndexRange, IndexRange);
	CHILDS_INTERFACE_WRITE_ALL_D(PointRange, IndexRange);
		
	/** Creates this hole from implementation data. */
	OversetHoles(Private::OversetHoles * iImpl);
}; // CGNS::OversetHoles


/** Represents a SIDS node possibly with DataClass_t and DimensionUnits_t subnodes.
  */
class Dimensioned : public Node {
public:
	enum StandardUnits { SI_UNITS, CGS_UNITS };
	
	/** Asks whether there is a local DataClass_t subnode. */
	bool hasLocalDataClass() const;
	/** Asks whether there is a local DimensionalUnits_t subnode. */
	bool hasLocalDimensionalUnits() const;
	/** Gets the mass units applicable to this node. */
	MassUnit getMassUnit() const;
	/** Gets the length units applicable to this node. */
	LengthUnit getLengthUnit() const;
	/** Gets the time units applicable to this node. */
	TimeUnit getTimeUnit() const;
	/** Gets the temperature units applicable to this node. */
	TemperatureUnit getTemperatureUnit() const;
	/** Gets the angle units applicable to this node. */
	AngleUnit getAngleUnit() const;

    OPTIONAL_CHILD_INTERFACE_WRITE1(DataClass, DataClass, DataClass);
    OPTIONAL_CHILD_INTERFACE_WRITE1(DimensionalUnits,
    	DimensionalUnits const &, DimensionalUnits const &);

	DimensionalUnits const & writeDimensionalUnits(StandardUnits iUnits);
protected:
	/** Creates this node from implementation data. */
	Dimensioned(Private::Dimensioned * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Dimensioned() {}
}; // CGNS::Dimensioned



/** Represents a SIDS node possibly with ReferenceState_t subnodes.
  */
class Stated : public Dimensioned {
public:
	/** Asks whether there is a local ReferenceState_t subnode. */
	bool hasLocalReferenceState() const;
	
	OPTIONAL_CHILD_INTERFACE_WRITE(ReferenceState, ReferenceState);
	
protected:
	/** Creates this node from implementation data. */
	Stated(Private::Stated * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Stated() {}
}; // CGNS::Stated


/** Represents a SIDS base or zone node.
  */
class Converged : public Stated {
public:
	OPTIONAL_CHILD_INTERFACE_WRITE1(ConvergenceHistory, ConvergenceHistory, int);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(FlowEquationSet, FlowEquationSet);
	
	N_CHILDS_INTERFACE_WRITE(IntegralData, IntegralData);

protected:	
	/** Creates this node from implementation data. */
	Converged(Private::Converged * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Converged() {}
}; // CGNS::Converged


/** Represents a CGNSBase_t structure.
  */
class Base : public Converged {
public:
//	~Base();
	enum SimulationType { NULL_SIMULATION, TIME_ACCURATE,
		NON_TIME_ACCURATE, USER_SIMULATION };
		
	OPTIONAL_CHILD_INTERFACE_WRITE1(SimulationType, SimulationType, SimulationType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(BaseIterativeData, BaseIterativeData, int);
	
	/** Gets the cell dimension, i.e. segments, faces or cells. */
	int getCellDimension() const;
	/** Gets the physical dimension the cells live in. */
	int getPhysicalDimension() const;

	N_CHILDS_INTERFACE(Zone, Zone);

	/** Writes a new structured 1D zone in this base and make it available. */
	Zone writeStructured1DZone(std::string const & iZoneName, int iVertexI);
	/** Writes a new structured 2D zone in this base and make it available. */
	Zone writeStructured2DZone(std::string const & iZoneName,
		int iVertexI, int iVertexJ);
	/** Writes a new structured 3D zone in this base and make it available. */
	Zone writeStructured3DZone(std::string const & iZoneName,
		int iVertexI, int iVertexJ, int iVertexK);
	/** Writes a new unstructured zone in this base and make it available.
	  * 0 for iNumBoundaryVertices means unsorted vertices, otherwise
	  * the first iNumBoundaryVertices vertices are assumed to lie on
	  * the boundary of the geometry. */
	Zone writeUnstructuredZone(std::string const & iZoneName,
		int iNumVertices, int iNumCells, int iNumBoundaryVertices=0);

	N_CHILDS_INTERFACE_WRITE(Family, Family);
	
	/** Creates this base from implementation data. */
	Base(Private::Base * iImpl);
}; // Base

/** Represents a SIDS node representing physical data.
  */
class DataArray : public Dimensioned {
public:
	enum Quantity { NULL_DATA,
		// Coordinates
		COORDINATE_X, COORDINATE_Y, COORDINATE_Z,
		COORDINATE_R, COORDINATE_THETA, COORDINATE_PHI,
		COORDINATE_NORMAL, COORDINATE_TANGENTIAL,
		COORDINATE_XI, COORDINATE_ETA, COORDINATE_ZETA,
		// Coordinate related
		COORDINATE_TRANSFORM,
		INTERPOLANTS_DONOR,
		ELEMENT_CONNECTIVITY,
		PARENT_DATA,
		// Solution quantities
		POTENTIAL, STREAM_FUNCTION,
		DENSITY, PRESSURE, TEMPERATURE,
		ENERGY_INTERNAL, ENTHALPY, ENTROPY, ENTROPY_APPROX,
		DENSITY_STAGNATION, PRESSURE_STAGNATION,
		TEMPERATURE_STAGNATION, ENERGY_STAGNATION,
		ENTHALPY_STAGNATION, ENERGY_STAGNATION_DENSITY,
		VELOCITY_X, VELOCITY_Y, VELOCITY_Z,
		VELOCITY_R, VELOCITY_THETA, VELOCITY_PHI,
		VELOCITY_MAGNITUDE,
		VELOCITY_NORMAL, VELOCITY_TANGENTIAL,
		VELOCITY_SOUND, VELOCITY_SOUND_STAGNATION,
		MOMENTUM_X, MOMENTUM_Y, MOMENTUM_Z,
		MOMENTUM_MAGNITUDE,
		ENERGY_KINETIC, PRESSURE_DYNAMIC,
		VORTICITY_X, VORTICITY_Y, VORTICITY_Z,
		VORTICITY_MAGNITUDE,
		SKIN_FRICTION_X, SKIN_FRICTION_Y, SKIN_FRICTION_Z,
		SKIN_FRICTION_MAGNITUDE,
		VELOCITY_ANGLE_X, VELOCITY_ANGLE_Y, VELOCITY_ANGLE_Z,
		VELOCITY_UNIT_VECTOR_X, VELOCITY_UNIT_VECTOR_Y, VELOCITY_UNIT_VECTOR_Z,
		MASS_FLOW,
		VISCOSITY_KINEMATIC, VISCOSITY_MOLECULAR, VISCOSITY_EDDY_KINEMATIC, VISCOSITY_EDDY,
		THERMAL_CONDUCTIVITY,
		REYNOLDS_STRESS_XX, REYNOLDS_STRESS_XY, REYNOLDS_STRESS_XZ,
		REYNOLDS_STRESS_YY, REYNOLDS_STRESS_YZ, REYNOLDS_STRESS_ZZ,
		// Solution related
		POWER_LAW_EXPONENT, SUTHERLAND_LAW_CONSTANT, TEMPERATURE_REFERENCE,
		VISCOSITY_MOLECULAR_REFERENCE, THERMAL_CONDUCTIVITY_REFERENCE,
		IDEAL_GAS_CONSTANT, SPECIFIC_HEAT_PRESSURE, SPECIFIC_HEAT_VOLUME,
		LENGTH_REFERENCE,
		// Turbulence model quantities
		TURBULENT_DISTANCE, TURBULENT_ENERGY_KINETIC, TURBULENT_DISSIPATION,
		TURBULENT_DISSIPATION_RATE, TURBULENT_B_B_REYNOLDS, TURBULENT_S_A_NU_TILDE,
		// Nondimensional parameters
		MACH, MACH_VELOCITY, MACH_VELOCITY_SOUND,
		REYNOLDS, REYNOLDS_VELOCITY, REYNOLDS_LENGTH, REYNOLDS_VISCOSITY_KINEMATIC,
		PRANDTL, PRANDTL_THERMAL_CONDUCTIVITY, PRANDTL_VISCOSITY_MOLECULAR,
		PRANDTL_SPECIFIC_HEAT_PRESSURE, PRANDTL_TURBULENT,
		SPECIFIC_HEAT_RATIO, SPECIFIC_HEAT_RATIO_PRESSURE, SPECIFIC_HEAT_RATIO_VOLUME,
		COEF_PRESSURE,
		COEF_SKIN_FRICTION_X, COEF_SKIN_FRICTION_Y, COEF_SKIN_FRICTION_Z,
		COEF_PRESSURE_DYNAMIC, COEF_PRESSURE_REFERENCE,
		// Characteristics and Riemann invariants
		RIEMANN_INVARIANT_PLUS, RIEMANN_INVARIANT_MINUS,
		CHARACTERISTIC_ENTROPY, CHARACTERISTIC_VORTICITY_1, CHARACTERISTIC_VORTICITY_2,
		CHARACTERISTIC_ACOUSTIC_PLUS, CHARACTERISTIC_ACOUSTIC_MINUS,
		
		// Forces and moments
		FORCE_X, FORCE_Y, FORCE_Z,
		FORCE_R, FORCE_THETA, FORCE_PHI,
		LIFT, DRAG,
		MOMENT_X, MOMENT_Y, MOMENT_Z,
		MOMENT_R, MOMENT_THETA, MOMENT_PHI,
		MOMENT_XI, MOMENT_ETA, MOMENT_ZETA,
		MOMENT_CENTER_X, MOMENT_CENTER_Y, MOMENT_CENTER_Z,
		
		COEF_LIFT, COEF_DRAG,
		COEF_MOMENT_X, COEF_MOMENT_Y, COEF_MOMENT_Z,
		COEF_MOMENT_R, COEF_MOMENT_THETA, COEF_MOMENT_PHI,
		COEF_MOMENT_XI, COEF_MOMENT_ETA, COEF_MOMENT_ZETA,
		//COEF_PRESSURE_DYNAMIC, // already under nondimensional parameters
		COEF_AREA, COEF_LENGTH,
		// Time-dependent flow
		TIME_VALUES, ITERATION_VALUES,
		NUMBER_OF_ZONES, NUMBER_OF_FAMILIES,
		ZONE_POINTERS, FAMILY_POINTERS,
		RIGID_GRID_MOTION_POINTERS, ARBITRARY_GRID_MOTION_POINTERS,
		GRID_COORDINATES_POINTERS, FLOW_SOLUTIONS_POINTERS,
		ORIGIN_LOCATION, RIGID_ROTATION_ANGLE, RIGID_VELOCITY, RIGID_ROTATION_RATE,
		GRID_VELOCITY_X, GRID_VELOCITY_Y, GRID_VELOCITY_Z,
		GRID_VELOCITY_R, GRID_VELOCITY_THETA, GRID_VELOCITY_PHI,
		GRID_VELOCITY_XI, GRID_VELOCITY_ETA, GRID_VELOCITY_ZETA,
		// from version 2.1 on
		FUEL_AIR_RATIO, REFERENCE_TEMPERATURE_HOF, ENTHALPY_ENERGY_RATIO,
		COMPRESSIBILITY_FACTOR, VIBRATIONAL_ELECTRON_ENERGY,
		HEAT_OF_FORMATION, VIBRATIONAL_ELECTRON_TEMPERATURE,
		// Done with all predefined quantities
		USER_DATA
	};
	
	/** The different data types a DataArray node may contain. */
	enum DataType { CHAR, INT, FLOAT, REAL4=FLOAT, DOUBLE, REAL8=DOUBLE };
	typedef char CHAR_t;
	typedef int INT_t;
	typedef float FLOAT_t;
	typedef FLOAT_t REAL4_t;
	typedef double DOUBLE_t;
	typedef DOUBLE_t REAL8_t;
	
	/** Represents a linear rescaling of data. */
	struct DataConversion {
		double mScale;
		double mOffset;
		double operator()(double iX) const { return iX*mScale+mOffset; }
		double backScale(double iY) const { return (iY-mOffset)/mScale; }
	};
	/** Represents the physical meaning of dimensional data. */
	struct DimensionalExponents {
		double mMass;
		double mLength;
		double mTime;
		double mTemperature;
		double mAngle;
	};

	/** Asks if there is explicit conversion information for this data. */
	bool hasDataConversion() const;
	/** Gets the conversion factors applicable to this data. */
	DataConversion getDataConversion() const;
	/** Gets the conversion scale applicable to this data. */
	double getConversionScale() const;
	/** Gets the conversion scale applicable to this data. */
	double getConversionOffset() const;
	/** Asks if there is explicit exponent information for this data. */
	bool hasDimensionalExponents() const;
	/** Gets the dimensional exponents applicable to this data. */
	DimensionalExponents getDimensionalExponents() const;
	/** Gets the mass exponent applicable to this data. */
	double getMassExponent() const;
	/** Gets the length exponent applicable to this data. */
	double getLengthExponent() const;
	/** Gets the time exponent applicable to this data. */
	double getTimeExponent() const;
	/** Gets the temperature exponent applicable to this data. */
	double getTemperatureExponent() const;
	/** Gets the angle exponent applicable to this data. */
	double getAngleExponent() const;
	
	/** Gets the quantity type of this data. */
	Quantity getQuantity() const;
	
	/** Gets the number of dimensions. */
	int getNumDimensions() const;
	/** Gets the size of the nth dimension. */
	int getDimension(int iNth) const;
	/** Sets all dimensions at once. */
	void setDimensions(int iNumDimensions, int const * iDimensions);
	/** Sets a single one. */
	void setDimensions(int iDimension);
	/** Sets two dimensions. */
	void setDimensions(int iDim1, int iDim2);
	/** Sets three dimensions. */
	void setDimensions(int iDim1, int iDim2, int iDim3);
	
	/** Gets the data type used in this DataArray. */
	DataType getDataType() const;
	/** Sets data type and all dimensions at once. */
	void setDataTypeDimensions(DataType iType, int iNumDimensions, int const * iDimensions);
	/** Sets data type and a single one dimension at once. */
	void setDataTypeDimensions(DataType iType, int iDimension);
	/** Sets data type and two dimensions at once. */
	void setDataTypeDimensions(DataType iType, int iDim1, int iDim2);
	/** Sets data type and three dimensions at once. */
	void setDataTypeDimensions(DataType iType, int iDim1, int iDim2, int iDim3);
	
	/** Reads the data from disk, possibly with conversion. */
	void readData(INT_t * oData) const;
	void readData(FLOAT_t * oData) const;
	void readData(DOUBLE_t * oData) const;
	
	/** Reads the data from disk, with strides and possibly conversion. */
	void readData(StridedRange1D iDiskStride, INT_t * oData, StridedRange1D iMemoryStride) const;
	void readData(StridedRange1D iDiskStride, FLOAT_t * oData, StridedRange1D iMemoryStride) const;
	void readData(StridedRange1D iDiskStride, DOUBLE_t * oData, StridedRange1D iMemoryStride) const;
	void readData(StridedRange2D iDiskStride, INT_t * oData, StridedRange2D iMemoryStride) const;
	void readData(StridedRange2D iDiskStride, FLOAT_t * oData, StridedRange2D iMemoryStride) const;
	void readData(StridedRange2D iDiskStride, DOUBLE_t * oData, StridedRange2D iMemoryStride) const;
	void readData(StridedRange3D iDiskStride, INT_t * oData, StridedRange3D iMemoryStride) const;
	void readData(StridedRange3D iDiskStride, FLOAT_t * oData, StridedRange3D iMemoryStride) const;
	void readData(StridedRange3D iDiskStride, DOUBLE_t * oData, StridedRange3D iMemoryStride) const;
	
	/** Writes the data to disk. */
	void writeData(INT_t const * iData);
	void writeData(FLOAT_t const * iData);
	void writeData(DOUBLE_t const * iData);
	
	/** Creates this data array from implementation data. */
	DataArray(Private::DataArray * iImpl);
}; // CGNS::DataArray


/** Represents a SIDS node possibly with DataArray_t subnodes.
  */
class Arrayed : public Dimensioned {
public:	
	N_CHILDS_INTERFACE_WRITE(DataArray, DataArray);
	
	/** Asks if there is a given quantity. */
	bool hasDataArray(DataArray::Quantity iQuantity) const;
	/** Gets the given quantity. Throws if not available. */
	DataArray getDataArray(DataArray::Quantity iQuantity) const;
	
	/** Creates or changes the given quantity. */
	DataArray writeDataArray(DataArray::Quantity iQuantity);
	/** Deletes the given quantity. */
	void deleteDataArray(DataArray::Quantity iQuantity);
	
protected:
	/** Creates this node from implementation data. */
	Arrayed(Private::Arrayed * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Arrayed() {}
}; // CGNS::Arrayed


/** Represents a SIDS node possibly with Rind_t subnodes.
  */
class Gridded : public Arrayed {
public:
	enum Index { I_MIN, I_MAX, J_MIN, J_MAX, K_MIN, K_MAX };
	/** Asks if there is any Rind_t subnode. */
	bool hasRind() const;
	/** Gets the number of rind planes beyond the specified index boundary. */
	int getRind(Index iIndex) const;
	
protected:
	/** Creates this node from implementation data. */
	Gridded(Private::Gridded * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Gridded() {}
}; // CGNS::Gridded


/** Represents a SIDS node holding either a FlowSolution_t or DiscreteData_t.
  */
class Discreted : public Gridded {
public:
	enum FieldType { NULL_FIELD,
		POTENTIAL=DataArray::POTENTIAL, STREAMFUNCTION,
		DENSITY, PRESSURE, TEMPERATURE, ENERGY_INTERNAL, ENTHALPY, ENTROPY, ENTROPY_APPROX,
		DENSITY_STAGNATION, PRESSURE_STAGNATION, TEMPERATURE_STAGNATION, ENERGY_STAGNATION,
		ENTHALPY_STAGNATION, ENERGY_STAGNATION_DENSITY,
		VELOCITY_X, VELOCITY_Y, VELOCITY_Z,
		VELOCITY_R, VELOCITY_THETA, VELOCITY_PHI,
		VELOCITY_MAGNITUDE,
		VELOCITY_NORMAL, VELOCITY_TANGENTIAL,
		VELOCITY_SOUND, VELOCITY_SOUND_STAGNATION,
		MOMENTUM_X, MOMENTUM_Y, MOMENTUM_Z,
		MOMENTUM_MAGNITUDE,
		ENERGY_KINETIC, PRESSURE_DYNAMIC,
		VORTICITY_X, VORTICITY_Y, VORTICITY_Z,
		VORTICITY_MAGNITUDE,
		SKIN_FRICTION_X, SKIN_FRICTION_Y, SKIN_FRICTION_Z,
		SKIN_FRICTION_MAGNITUDE,
		VELOCITY_ANGLE_X, VELOCITY_ANGLE_Y, VELOCITY_ANGLE_Z,
		VELOCITY_UNIT_VECTOR_X, VELOCITY_UNIT_VECTOR_Y, VELOCITY_UNIT_VECTOR_Z,
		MASS_FLOW,
		VISCOSITY_KINEMATIC, VISCOSITY_MOLECULAR, VISCOSITY_EDDY_KINEMATIC, VISCOSITY_EDDY,
		THERMAL_CONDUCTIVITY,
		REYNOLDS_STRESS_XX, REYNOLDS_STRESS_XY, REYNOLDS_STRESS_XZ,
		REYNOLDS_STRESS_YY, REYNOLDS_STRESS_YZ, REYNOLDS_STRESS_ZZ,
		USER_FIELD
	};
	
	/** Gets the location of the solution data. */
	CHILD_INTERFACE(Location, GridLocation);
	CHILD_INTERFACE_WRITE1(Location, GridLocation, GridLocation);
	
	/** Gets the number of fields, e.g. Pressure, VelocityX etc. */
	int getNumFields() const;
	/** Gets the quantity type of the nth field. */
	FieldType getFieldType(int iNth) const;
	
	/** Asks if there is a subnode for the given QuantityType. */
	bool hasField(FieldType iField) const;
	
	/** Gets the corresponding DataArray_t subnode. */
	DataArray getField(FieldType iField) const;
	/** Gets the named DataArray_t subnode. */
	DataArray getField(std::string const & iFieldName) const;
	
	/** Does the actual read of the quantity entry. */
	void readData(FieldType iQuantity, double * oField) const;
	void readData(FieldType iQuantity, float * oField) const;

	/** Does the actual write of the quantity entry. */
	void writeData(FieldType iQuantity, double const * iField);
	void writeData(FieldType iQuantity, float const * iField);

    /** Adds the named quantity entry to this node and returns it. */
    DataArray writeField(FieldType iField,
    	DataArray::DataType iType=DataArray::DOUBLE);
	
	typedef iterator<DataArray> Field_iterator_t;
	Field_iterator_t beginField() const;
	Field_iterator_t endField() const;
	
	void deleteField(FieldType iField);
	void deleteField(std::string const & iName);
	
protected:	
	/** Creates this node from implementation data. */
	Discreted(Private::Discreted * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Discreted() {}
}; // CGNS::Discreted

/** Represents a SIDS node holding a RigidGridMotion_t.
  */
class RigidGridMotion : public Arrayed {
public:
	enum RigidGridMotionType { NULL_MOTION,
		CONSTANT_RATE, VARIABLE_RATE, USER_MOTION };
	
	RigidGridMotionType getType() const;
	
	CHILD_INTERFACE(OriginLocationBefore, Point);
	CHILD_INTERFACE_WRITE_D(OriginLocationBefore, Point);
	
	CHILD_INTERFACE(OriginLocationAfter, Point);
	CHILD_INTERFACE_WRITE_D(OriginLocationAfter, Point);
	
#define POINT_INTERFACE(name)          \
OPTIONAL_CHILD_INTERFACE(name, Point); \
CHILD_INTERFACE_WRITE_D(name, Point)
	POINT_INTERFACE(RigidRotationAngle);
	POINT_INTERFACE(RigidVelocity);
	POINT_INTERFACE(RigidRotationRate);
#undef POINT_INTERFACE

	/** Creates this node from implementation data. */
	RigidGridMotion(Private::RigidGridMotion * iImpl);
}; // CGNS::RigidGridMotion


/** Represents a SIDS node holding a ArbitraryGridMotion_t.
  */
class ArbitraryGridMotion : public Discreted {
public:
	enum ArbitraryGridMotionType { NULL_MOTION,
		NON_DEFORMING_GRID, DEFORMING_GRID, USER_MOTION };

	ArbitraryGridMotionType getType() const;
			
	/** Creates this grid motion data from implementation data. */
	ArbitraryGridMotion(Private::ArbitraryGridMotion * iImpl);
}; // CGNS::ArbitraryGridMotion

/** Represents a Zone_t structure.
  */
class Zone : public Converged  {
public:
	/** Gets the index dimension, i.e. 1/2/3 for structured in 1D/2D/3D,
	    1 for unstructured. */
	int getIndexDimension() const;
	/** Asks whether this is a structured or unstructured zone. */
	bool isStructured() const;
	
    /** Gets the number of boundary vertices.
     * Zero if no info is available. Throws if structured. */
	int getBoundaryVertices() const;

    /** Gets the total number of vertices. */
    int getTotalVertices() const;
    /** Gets the total number of cells. */
    int getTotalCells() const;

	/** Gets the number of vertices in each index direction. */
    void getVertexDimensions(int * oVertexDimensions) const;
	/** Gets the number of vertices in each index direction. */
    void getCellDimensions(int * oCellDimensions) const;

    /** Enumeration for the different index dimensions. */
    enum Index { INDEX_I, INDEX_J, INDEX_K };
	/** Gets the number of vertices in the given index direction. */
    int getVertexDimension(Index iIndex) const;
	/** Gets the number of vertices in each index direction. */
    int getCellDimension(Index iIndex) const;

    			
    /* The interface for the ordinary single GridCoordinates node.
       This is separate from the possibly many iterative GridCoordinates nodes. */
	OPTIONAL_CHILD_INTERFACE_WRITE(GridCoordinates, GridCoordinates);
	
    /* The interface for the possibly many iterative GridCoordinates nodes.
       They are separate from the single GridCoordinates node. */
	N_CHILDS_INTERFACE_WRITE(GridCoordinates, GridCoordinates);
	
	N_CHILDS_INTERFACE_WRITE(FlowSolution, FlowSolution);
	CHILDS_INTERFACE_WRITE1(FlowSolution, FlowSolution, GridLocation);
	CHILDS_INTERFACE_WRITE1(FlowSolution, FlowSolution, Rind1D);
	CHILDS_INTERFACE_WRITE1(FlowSolution, FlowSolution, Rind2D);
	CHILDS_INTERFACE_WRITE1(FlowSolution, FlowSolution, Rind3D);
	CHILDS_INTERFACE_WRITE2(FlowSolution, FlowSolution, GridLocation, Rind1D);
	CHILDS_INTERFACE_WRITE2(FlowSolution, FlowSolution, GridLocation, Rind2D);
	CHILDS_INTERFACE_WRITE2(FlowSolution, FlowSolution, GridLocation, Rind3D);
	
	N_CHILDS_INTERFACE_WRITE(DiscreteData, DiscreteData);
	
	N_CHILDS_INTERFACE_WRITE2(Elements, Elements,
		Elements::ElementType iElementType, int iElementsOnBoundary=0);
		
	OPTIONAL_CHILD_INTERFACE_WRITE1(FamilyName, std::string, std::string const &);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(ZoneIterativeData, ZoneIterativeData);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(ZoneGridConnectivity, ZoneGridConnectivity);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(ZoneBC, ZoneBC);
	
	N_CHILDS_INTERFACE_WRITE1(RigidGridMotion, RigidGridMotion,
		RigidGridMotion::RigidGridMotionType);
	
	N_CHILDS_INTERFACE_WRITE1(ArbitraryGridMotion, ArbitraryGridMotion,
		ArbitraryGridMotion::ArbitraryGridMotionType);
	
	/** Creates this zone from implementation data. */
	Zone(Private::Zone * iImpl);
};

/** Represents a SIDS node representing index data.
  */
class IndexArray : public Node {
public:
	/** Gets the number of entries. */
	int getLength() const;
	/** Sets all dimensions at once. */
	void setLength(int iLength);
	
	/** Reads the data from disk. */
	void readData(Index1D * oData) const;
	void readData(Index2D * oData) const;
	void readData(Index3D * oData) const;
	/** Identical with Index1D */
	void readData(int * oData) const;
	
	/** Writes the data to disk. */
	void writeData(Index1D const * iData);
	void writeData(Index2D const * iData);
	void writeData(Index3D const * iData);
	/** Identical with Index1D */
	void writeData(int const * iData);
	
	/** Creates this data array from implementation data. */
	IndexArray(Private::IndexArray * iImpl);
}; // CGNS::IndexArray

/** Represents a SIDS node possibly with scalar only DataArray_t subnodes.
  */
class Valued : public Arrayed {
public:
	N_CHILDS_INTERFACE_WRITE1(Value, double, double iValue);
	
	/** Asks if there is a given quantity. */
	bool hasValue(DataArray::Quantity iQuantity) const;
	/** Gets the given quantity. Throws if not available. */
	double getValue(DataArray::Quantity iQuantity) const;
	
	/** Creates or changes the given quantity. */
	double writeValue(DataArray::Quantity iQuantity, double iValue);
	
	/** Deletes the given quantity. */
	void deleteValue(DataArray::Quantity iQuantity);
	
protected:	
	/** Creates this node from implementation data. */
	Valued(Private::Valued * iImpl);
	/** Deletes this node and disables access via base pointers. */
	~Valued() {}
}; // CGNS::Valued

#define DERIVE_VALUED(name)                          \
class name : public Valued {                         \
public:                                              \
	name(Private::Valued * iImpl) : Valued(iImpl) {} \
}

DERIVE_VALUED(IntegralData);

/** Represents a SIDS BC_t node
  */
class BC : public Stated {
public:
	enum BCType { NULL_BC,
		// BCTypeSimple_t
		GENERAL, DIRICHLET, NEUMANN, EXTRAPOLATE,
		WALL_INVISCID, WALL_VISCOUS_HEAT_FLUX, WALL_VISCOUS_ISOTHERMAL,
		WALL_VISCOUS, WALL,
		INFLOW_SUBSONIC, INFLOW_SUPERSONIC,
		OUTFLOW_SUBSONIC, OUTFLOW_SUPERSONIC,
		TUNNEL_INFLOW, TUNNEL_OUTFLOW,
		DEGENERATE_LINE, DEGENERATE_POINT,
		SYMMETRY_PLANE, SYMMETRY_POLAR, AXISYMMETRIC_WEDGE,
		// BCTypeCompound_t
		INFLOW, OUTFLOW, FARFIELD,
		USER_BC
	};
	
	/** Gets the type of this boundary patch. */
	BCType getBCType() const;
	
	/** Gets the location of the BC data. */
	CHILD_INTERFACE(Location, GridLocation);
	CHILD_INTERFACE_WRITE1(Location, GridLocation, GridLocation);
	
	N_CHILDS_INTERFACE_WRITE1(BCDataSet, BCDataSet, BCType);

	OPTIONAL_CHILD_INTERFACE(PointRange, IndexRange);
	CHILD_INTERFACE_WRITE_D(PointRange, IndexRange);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(PointList, IndexArray);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(FamilyName, std::string, std::string const &);
	
	CHILD_INTERFACE(InwardNormalIndex, Normal);
	OPTIONAL_CHILD_INTERFACE_WRITE_D(InwardNormalIndex, Normal);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(InwardNormalList, IndexArray);
	
	/** Creates this node from implementation data. */
	BC(Private::BC * iImpl);
}; // CGNS::BC


/** Represents a SIDS ZoneBC_t node
  */
class ZoneBC : public Stated {
public:
	N_CHILDS_INTERFACE_WRITE1(BC, BC, BC::BCType);

	/** Creates this node from implementation data. */
	ZoneBC(Private::ZoneBC * iImpl);
}; // CGNS::ZoneBC


/** Represents a SIDS BCDataSet_t node
  */
class BCDataSet : public Stated {
public:
	/** Gets the type of this boundary patch. */
	BC::BCType getBCType() const;
	
	OPTIONAL_CHILD_INTERFACE_WRITE(DirichletData, BCData);
	
	OPTIONAL_CHILD_INTERFACE_WRITE(NeumannData, BCData);

	/** Creates this node from implementation data. */
	BCDataSet(Private::BCDataSet * iImpl);
}; // CGNS::BCDataSet


/** Represents a SIDS BCData_t node
  */
class BCData : public Valued {
public:

	/** Creates this node from implementation data. */
	BCData(Private::BCData * iImpl);
}; // CGNS::BCData


/** Represents a SIDS GeometryReference_t node
  */
class GeometryReference : public Node {
public:
	enum GeometryFormat { NULL_FORMAT, NASA_IGES, SDRC, UNIGRAPHICS,
		PRO_ENGINEER, ICEM_CFD, USER_FORMAT };
	struct GeometryEntity {};
		
	GeometryFormat getGeometryFormat() const;
	
	std::string getGeometryFile() const;

	N_EMPTY_CHILDS_INTERFACE(GeometryEntity, GeometryEntity);
	
	std::string writeGeometryEntity(std::string const & iEntity);
	
	/** Creates this node from implementation data. */
	GeometryReference(Private::GeometryReference * iImpl);
}; // CGNS::GeometryReference


/** Represents a SIDS node holding a Family_t.
  */
class Family : public Node {
public:
	N_CHILDS_INTERFACE_WRITE2(GeometryReference, GeometryReference,
		std::string const & iFile, GeometryReference::GeometryFormat iFormat);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(FamilyBC, BC::BCType, BC::BCType);
	
	/** Creates this family from implementation data. */
	Family(Private::Family * iImpl);
}; // CGNS::Family


/** Represents a SIDS node holding a ReferenceState_t.
  */
class ReferenceState : public Valued {
public:
	OPTIONAL_CHILD_INTERFACE_WRITE1(ReferenceStateDescription, std::string, std::string const &);
		
	/** Creates this node from implementation data. */
	ReferenceState(Private::ReferenceState * iImpl);
}; // CGNS::ReferenceState


/** Represents a SIDS node holding a UserDefinedData_t.
  */
class UserDefinedData : public Arrayed {
public:
	
	/** Creates this user defined data from implementation data. */
	UserDefinedData(Private::UserDefinedData * iImpl);
}; // CGNS::UserDefinedData


/** Represents a SIDS node holding a BaseIterativeData_t.
  */
class BaseIterativeData : public Arrayed {
public:
	/** Returns the number of steps required. */
	int getNumberOfSteps() const;
	
	bool hasTimeValues() const;
	double getTimeValues(int iStep) const;
	DataArray getTimeValues() const;
	int appendTimeValues(double iTime);
	double writeTimeValues(int iStep, double iTime);
	void deleteTimeValues();
	
	bool hasIterationValues() const;
	int getIterationValues(int iStep) const;
	DataArray getIterationValues() const;
	int appendIterationValues(int iIteration);
	int writeIterationValues(int iStep, int iIteration);
	void deleteIterationValues();
	
	void appendTimeIterationValues(double iTime, int iIteration);
	
#define POINTERS_INTERFACE(name)                                  \
bool has##name##Pointers() const;                                 \
int getMaxNum##name() const;                                      \
int getNum##name(int iStep) const;                                \
std::string get##name##Name(int iStep, int iNum) const;           \
DataArray get##name##Pointers() const;                            \
name get##name(int iStep, int iNum) const;                        \
int append##name(int iStep, std::string const & iName);           \
int append##name(int iStep, name const & iName);                  \
void write##name(int iStep, std::string const & iName, int iNum); \
void write##name(int iStep, name const & iName, int iNum);        \
void delete##name##Pointers()

	bool hasNumberOfZones() const;
	int getNumberOfZones(int iStep) const;
	DataArray getNumberOfZones() const;
	int writeNumberOfZones(int iStep, int iNumZones);
	void deleteNumberOfZones();
	POINTERS_INTERFACE(Zone);
	
	bool hasNumberOfFamilies() const;
	int getNumberOfFamilies(int iStep) const;
	DataArray getNumberOfFamilies() const;
	int writeNumberOfFamilies(int iStep, int iNumFamilies);
	void deleteNumberOfFamilies();
	POINTERS_INTERFACE(Family);
#undef POINTERS_INTERFACE
	
	/** Creates this node from implementation data. */
	BaseIterativeData(Private::BaseIterativeData * iImpl);
}; // CGNS::BaseIterativeData

/** Represents a SIDS node holding a ZoneIterativeData_t.
  */
class ZoneIterativeData : public Arrayed {
public:
#define POINTERS_INTERFACE(name)                        \
bool has##name##Pointers() const;                       \
std::string get##name##Name(int iStep) const;           \
DataArray get##name##Pointers() const;                  \
name get##name(int iStep) const;                        \
void write##name(int iStep, std::string const & iName); \
void write##name(int iStep, name const & iName);        \
void delete##name##Pointers()

	POINTERS_INTERFACE(RigidGridMotion);
	
	POINTERS_INTERFACE(ArbitraryGridMotion);
	
	POINTERS_INTERFACE(GridCoordinates);
	
	POINTERS_INTERFACE(FlowSolution);
#undef POINTERS_INTERFACE
	
	/** Creates this node from implementation data. */
	ZoneIterativeData(Private::ZoneIterativeData * iImpl);
}; // CGNS::ZoneIterativeData


/** Represents a SIDS node holding a GridCoordinates_t.
  */
class GridCoordinates : public Gridded {
public:
//	virtual ~GridCoordinates();
	/** The possible coordinate types. */
	enum CoordinateType { NULL_COORDINATE=DataArray::NULL_DATA,
		COORDINATE_X=DataArray::COORDINATE_X,
		COORDINATE_Y=DataArray::COORDINATE_Y, COORDINATE_Z=DataArray::COORDINATE_Z,
		COORDINATE_R=DataArray::COORDINATE_R,
		COORDINATE_THETA=DataArray::COORDINATE_THETA,
		COORDINATE_PHI=DataArray::COORDINATE_PHI,
		COORDINATE_NORMAL=DataArray::COORDINATE_NORMAL,
		COORDINATE_TANGENTIAL=DataArray::COORDINATE_TANGENTIAL,
		COORDINATE_XI=DataArray::COORDINATE_XI,
		COORDINATE_ETA=DataArray::COORDINATE_ETA,
		COORDINATE_ZETA=DataArray::COORDINATE_ZETA,
		USER_COORDINATE
	};
	
	N_CHILDS_INTERFACE(Coordinate, DataArray);
	
	/** Asks if there is a subnode for the given CoordinateType. */
	bool hasCoordinate(CoordinateType iCoordinate) const;
	/** Gets the corresponding DataArray_t subnode. */
	DataArray getCoordinate(CoordinateType iCoordinate) const;
	
	/** Does the actual read of the coordinate entry. */
	void readData(CoordinateType iCoordinate, double * oCoordinates) const;
	void readData(CoordinateType iCoordinate, float * oCoordinates) const;

	/** Does the actual write of the coordinate data. */
	void writeData(CoordinateType iCoordinate, double const * iCoordinates);
	void writeData(CoordinateType iCoordinate, float const * iCoordinates);

    /** Adds the named coordinate entry to this node and returns it. */
    DataArray writeCoordinate(CoordinateType iCoordinate,
    	DataArray::DataType iType=DataArray::DOUBLE);

	/** Creates these grid coordinates from implementation data. */
	GridCoordinates(Private::GridCoordinates * iImpl);
}; // CGNS::GridCoordinates

/** Represents a SIDS node holding a FlowSolution_t.
  */
class FlowSolution : public Discreted {
public:
	/** Creates this solution from implementation data. */
	FlowSolution(Private::FlowSolution * iImpl);
}; // CGNS::FlowSolution

/** Represents a SIDS node holding a DiscreteData_t.
  */
class DiscreteData : public Discreted {
public:
	/** Creates this discrete data from implementation data. */
	DiscreteData(Private::DiscreteData * iImpl);
}; // CGNS::DiscreteData

/** Represents a SIDS node holding a ConvergenceHistory_t.
  */
class ConvergenceHistory : public Arrayed {
public:
	/** Returns the number of iterations. */
	int getIterations() const;

	OPTIONAL_CHILD_INTERFACE_WRITE1(NormDefinitions, std::string, std::string const &);
		
	/** Creates this convergence history from implementation data. */
	ConvergenceHistory(Private::ConvergenceHistory * iImpl);
}; // CGNS::ConvergenceHistory

/** Represents a SIDS node holding a GoverningEquations_t.
  */
class GoverningEquations : public Node {
public:
	enum GoverningEquationsType { NULL_EQUATION, FULL_POTENTIAL, EULER,
		NS_LAMINAR, NS_TURBULENT,
		NS_LAMINAR_INCOMPRESSIBLE, NS_TURBULENT_INCOMPRESSIBLE,
		USER_EQUATION };
		
	GoverningEquationsType getType() const;
		
	OPTIONAL_CHILD(DiffusionModel);
	bool getDiffusionTerm(DiffusionIndex iIndex) const;
	void writeDiffusionTerm(DiffusionIndex iIndex, bool iIsModeled);
	void writeThinLayerDiffusion(DiffusionIndex iIndex);
	void writeFullDiffusion();
	
	/** Creates these governing equations from implementation data. */
	GoverningEquations(Private::GoverningEquations * iImpl);
}; // CGNS::GoverningEquations

/** Represents a SIDS node holding a GasModel_t.
  */
class GasModel : public Valued {
public:
	enum GasModelType { NULL_GAS,
		IDEAL, VANDER_WAALS,
		// from version 2.1 on
		CALORICALLY_PERFECT, THERMALLY_PERFECT, CONSTANT_DENSITY, REDLICH_KWONG,		
		USER_GAS };
	
	GasModelType getType() const;
	
	/** Creates this gas model from implementation data. */
	GasModel(Private::GasModel * iImpl);
}; // CGNS::GasModel

/** Represents a SIDS node holding a ViscosityModel_t.
  */
class ViscosityModel : public Valued {
public:
	enum ViscosityModelType { NULL_VISCOSITY,
		CONSTANT, POWER_LAW, SUTHERLAND_LAW, USER_VISCOSITY };
	
	ViscosityModelType getType() const;
	
	/** Creates this viscosity model from implementation data. */
	ViscosityModel(Private::ViscosityModel * iImpl);
}; // CGNS::ViscosityModel

/** Represents a SIDS node holding a ThermalConductivity_t.
  */
class ThermalConductivityModel : public Valued {
public:
	enum ThermalConductivityModelType { NULL_TC,
		CONSTANT_PRANDTL, POWER_LAW, SUTHERLAND_LAW, USER_TC };
	
	ThermalConductivityModelType getType() const;
	
	/** Creates this thermal conductivity model from implementation data. */
	ThermalConductivityModel(Private::ThermalConductivityModel * iImpl);
}; // CGNS::ThermalConductivityModel

/** Represents a SIDS node holding a TurbulenceClosure_t.
  */
class TurbulenceClosure : public Valued {
public:
	enum TurbulenceClosureType { NULL_CLOSURE,
		EDDY_VISCOSITY, REYNOLDS_STRESS, REYNOLDS_STRESS_ALGEBRAIC, USER_CLOSURE };
	
	TurbulenceClosureType getType() const;
	
	/** Creates this turbulence closure from implementation data. */
	TurbulenceClosure(Private::TurbulenceClosure * iImpl);
}; // CGNS::TurbulenceClosure

/** Represents a SIDS node holding a TurbulenceModel_t.
  */
class TurbulenceModel : public Valued {
public:
	enum TurbulenceModelType { NULL_TURBULENCE,
		ALGEBRAIC_BALDWIN_LOMAX, ALGEBRAIC_CEBECI_SMITH,
		HALF_EQUATION_JOHNSON_KING,
		ONE_EQUATION_BALDWIN_BARTH, ONE_EQUATION_SPALART_ALLMARAS,
		TWO_EQUATION_JONES_LAUNDER, TWO_EQUATION_MENTER_SST, TWO_EQUATION_WILCOX,
		USER_TURBULENCE
	};
	
	TurbulenceModelType getType() const;
	
	OPTIONAL_CHILD(DiffusionModel);
	bool getDiffusionTerm(DiffusionIndex iIndex) const;
	void writeDiffusionTerm(DiffusionIndex iIndex, bool iIsModeled);
	void writeThinLayerDiffusion(DiffusionIndex iIndex);
	void writeFullDiffusion();
	
	/** Creates this turbulence model from implementation data. */
	TurbulenceModel(Private::TurbulenceModel * iImpl);
}; // CGNS::TurbulenceModel

/** Represents a SIDS node holding a ThermalRelaxationModel_t.
  */
class ThermalRelaxationModel : public Valued {
public:
	enum ThermalRelaxationModelType { NULL_RELAXATION,
		FROZEN, THERMAL_EQUILIB, THERMAL_NONEQUILIB,
		USER_RELAXATION };
	
	ThermalRelaxationModelType getType() const;
	
	/** Creates this relaxation model from implementation data. */
	ThermalRelaxationModel(Private::ThermalRelaxationModel * iImpl);
}; // CGNS::ThermalRelaxationModel

/** Represents a SIDS node holding a ChemicalKineticsModel_t.
  */
class ChemicalKineticsModel : public Valued {
public:
	enum ChemicalKineticsModelType { NULL_KINETICS,
		FROZEN, CHEMICAL_EQUILIB_CURVE_FIT, CHEMICAL_EQUILIB_MINIMIZATION,
		CHEMICAL_NONEQUILIB, USER_KINETICS };
	
	ChemicalKineticsModelType getType() const;
	
	/** Creates this kinetics model from implementation data. */
	ChemicalKineticsModel(Private::ChemicalKineticsModel * iImpl);
}; // CGNS::ChemicalKineticsModel

/** Represents a SIDS node holding a FlowEquationSet_t.
  */
class FlowEquationSet : public Dimensioned {
public:
	OPTIONAL_CHILD_INTERFACE_WRITE1(GoverningEquations, GoverningEquations,
		GoverningEquations::GoverningEquationsType);

	OPTIONAL_CHILD_INTERFACE_WRITE1(GasModel, GasModel, GasModel::GasModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(ViscosityModel, ViscosityModel,
		ViscosityModel::ViscosityModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(ThermalConductivityModel, ThermalConductivityModel,
		ThermalConductivityModel::ThermalConductivityModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(TurbulenceModel, TurbulenceModel,
		TurbulenceModel::TurbulenceModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(TurbulenceClosure, TurbulenceClosure,
		TurbulenceClosure::TurbulenceClosureType);

	OPTIONAL_CHILD_INTERFACE_WRITE1(ThermalRelaxationModel, ThermalRelaxationModel,
		ThermalRelaxationModel::ThermalRelaxationModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(ChemicalKineticsModel, ChemicalKineticsModel,
		ChemicalKineticsModel::ChemicalKineticsModelType);
	
	OPTIONAL_CHILD_INTERFACE_WRITE1(EquationDimension, int, int);
		
	/** Creates this equation set from implementation data. */
	FlowEquationSet(Private::FlowEquationSet * iImpl);
}; // CGNS::FlowEquationSet


// make sure nothing remains
#undef CHILD_INTERFACE_WRITE
#undef CHILD_INTERFACE_WRITE1
#undef CHILD_INTERFACE_WRITE2
#undef CHILD_INTERFACE_WRITE3
#undef CHILDS_INTERFACE_WRITE
#undef CHILDS_INTERFACE_WRITE1
#undef CHILDS_INTERFACE_WRITE2
#undef N_CHILDS_INTERFACE
#undef N_CHILDS_INTERFACE_WRITE
#undef N_CHILDS_INTERFACE_WRITE1
#undef N_CHILDS_INTERFACE_WRITE2
#undef OPTIONAL_CHILD_INTERFACE
#undef OPTIONAL_CHILD_INTERFACE_WRITE
#undef OPTIONAL_CHILD_INTERFACE_WRITE1
#undef OPTIONAL_CHILD_INTERFACE_WRITE2
#undef OPTIONAL_CHILD_INTERFACE_WRITE3

#undef DERIVE_VALUED

} // namespace CGNS


#endif
