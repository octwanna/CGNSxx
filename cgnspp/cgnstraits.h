/***************************************************************************
                          cgnstraits.h  -  description
                             -------------------
    begin                : Fri May 24 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSTRAITS_H
#define CGNSTRAITS_H

#include "cgns++.h"

namespace CGNS {
namespace Private {

template<typename T>
struct implementation_traits;// { typedef T Impl; };

#define DECLARE_NODE_TRAITS(node, id)                  \
template<>                                             \
struct implementation_traits<CGNS::node> {             \
	typedef CGNS::node Node;                           \
	typedef CGNS::Private::node Impl;                  \
	enum { ID=id, INIT_MASK=1u<<id };                  \
	static char const * Name() { return #node; }       \
	static char const * Label() { return #node "_t"; } \
};                                                     \
template<>                                             \
struct implementation_traits<CGNS::Private::node> {    \
	typedef CGNS::node Node;                           \
	typedef CGNS::Private::node Impl;                  \
	enum { ID=id, INIT_MASK=id ? 1u<<id : 0 };         \
	static char const * Name() { return #node; }       \
	static char const * Label() { return #node "_t"; } \
}

#define DECLARE_VALUED_TRAITS(node, id)                \
template<>                                             \
struct implementation_traits<CGNS::node> {             \
	typedef CGNS::node Node;                           \
	typedef CGNS::Private::Valued Impl;                \
	enum { ID=id, INIT_MASK=1u<<id };                  \
	static char const * Name() { return #node; }       \
	static char const * Label() { return #node "_t"; } \
}

#define DECLARE_MODEL_TRAITS(node, id)                 \
DECLARE_NODE_TRAITS(node, id)

#define DECLARE_CLASSED_NODE_TRAITS(class, node, id)   \
template<>                                             \
struct implementation_traits<CGNS::class::node> {      \
	typedef CGNS::class::node Node;                    \
	typedef CGNS::class::node Impl;                    \
	enum { ID=id, INIT_MASK=1u<<id };                  \
	static char const * Name() { return #node; }       \
	static char const * Label() { return #node "_t"; } \
}

#define DECLARE_BASIC_NODE_TRAITS(node, id)            \
template<>                                             \
struct implementation_traits<CGNS::node> {             \
	typedef CGNS::node Node;                           \
	typedef CGNS::node Impl;                           \
	enum { ID=id, INIT_MASK=1u<<id };                  \
	static char const * Name() { return #node; }       \
	static char const * Label() { return #node "_t"; } \
}

#define DECLARE_BUILTIN_NODE_TRAITS(name, node, id)    \
template<>                                             \
struct implementation_traits<node> {                   \
	enum { ID=id, INIT_MASK=1u<<id };                  \
	static char const * Name() { return #name; }       \
	static char const * Label() { return #name "_t"; } \
}

// BEWARE: The numbers are based on a fairly sophisticated system to assure
// no two subnodes with the same number can occur in the same parent
// while keeping the numbers below 32 to fit into an unsigned int
// NEVER fiddle around with these unless you know what you are doing!
enum { INIT_MASK_SELF=1, MASK_A=20, MASK_B, MASK_C, MASK_D, MASK_E, MASK_F,
	MASK_G, MASK_H, MASK_I };
DECLARE_NODE_TRAITS(Node, 0);
// this is basically DECLARE_NODE_TRAITS(Parented, 0);
template<>
struct implementation_traits<CGNS::Private::Parented> {
	typedef CGNS::Node Node;
	typedef CGNS::Private::Parented Impl;
	enum { ID=0, INIT_MASK=0 };
	static char const * Name() { return ""; }
	static char const * Label() { return ""; }
};
// this is basically DECLARE_NODE_TRAITS(IterativeData, 0);
template<>
struct implementation_traits<CGNS::Private::IterativeData> {
	typedef CGNS::Arrayed Node;
	typedef CGNS::Private::IterativeData Impl;
	enum { ID=0, INIT_MASK=0 };
	static char const * Name() { return ""; }
	static char const * Label() { return ""; }
};
DECLARE_NODE_TRAITS(Converged, 0);
DECLARE_NODE_TRAITS(Stated, 0);
DECLARE_NODE_TRAITS(Dimensioned, 0);
DECLARE_NODE_TRAITS(Arrayed, 0);
DECLARE_NODE_TRAITS(Valued, 0);
DECLARE_NODE_TRAITS(Discreted, 0);
DECLARE_NODE_TRAITS(Gridded, 0);
DECLARE_NODE_TRAITS(Base, 0);
DECLARE_NODE_TRAITS(Zone, MASK_A);
DECLARE_NODE_TRAITS(GridCoordinates, MASK_A);
DECLARE_NODE_TRAITS(Elements, MASK_B);
DECLARE_NODE_TRAITS(FlowSolution, MASK_C);
DECLARE_NODE_TRAITS(DiscreteData, MASK_D);
DECLARE_NODE_TRAITS(ZoneGridConnectivity, MASK_E);
DECLARE_NODE_TRAITS(GridConnectivity1to1, MASK_A);
DECLARE_NODE_TRAITS(GridConnectivity, MASK_B);
DECLARE_CLASSED_NODE_TRAITS(GridConnectivity, GridConnectivityType, MASK_A);
DECLARE_NODE_TRAITS(OversetHoles, MASK_C);
DECLARE_NODE_TRAITS(ZoneBC, MASK_F);
DECLARE_NODE_TRAITS(BC, MASK_A);
DECLARE_NODE_TRAITS(BCDataSet, MASK_A);
DECLARE_NODE_TRAITS(BCData, MASK_A);
DECLARE_CLASSED_NODE_TRAITS(BC, BCType, MASK_B);
DECLARE_NODE_TRAITS(FlowEquationSet, 1);
DECLARE_NODE_TRAITS(GoverningEquations, MASK_A);
DECLARE_MODEL_TRAITS(GasModel, MASK_B);
DECLARE_MODEL_TRAITS(ViscosityModel, MASK_C);
DECLARE_MODEL_TRAITS(ThermalConductivityModel, MASK_D);
DECLARE_MODEL_TRAITS(TurbulenceClosure, MASK_E);
DECLARE_NODE_TRAITS(TurbulenceModel, MASK_F);
DECLARE_NODE_TRAITS(ThermalRelaxationModel, MASK_G);
DECLARE_NODE_TRAITS(ChemicalKineticsModel, MASK_H);
DECLARE_NODE_TRAITS(ConvergenceHistory, 2);
DECLARE_VALUED_TRAITS(IntegralData, 3);
DECLARE_NODE_TRAITS(ReferenceState, 4);
DECLARE_NODE_TRAITS(DataArray, 5);
DECLARE_NODE_TRAITS(Family, MASK_B);
DECLARE_NODE_TRAITS(GeometryReference, MASK_A);
DECLARE_NODE_TRAITS(BaseIterativeData, MASK_C);
DECLARE_NODE_TRAITS(ZoneIterativeData, MASK_G);
DECLARE_NODE_TRAITS(RigidGridMotion, MASK_H);
DECLARE_NODE_TRAITS(ArbitraryGridMotion, MASK_I);
DECLARE_NODE_TRAITS(IndexArray, 6);
DECLARE_BASIC_NODE_TRAITS(Descriptor, 15);
DECLARE_NODE_TRAITS(UserDefinedData, 16);

DECLARE_CLASSED_NODE_TRAITS(Base, SimulationType, MASK_D);
DECLARE_BASIC_NODE_TRAITS(DimensionalUnits, 7);
DECLARE_BASIC_NODE_TRAITS(DataClass, 8);
DECLARE_BUILTIN_NODE_TRAITS(Ordinal, int, 9);
//DECLARE_BUILTIN_NODE_TRAITS(ZoneType, int, MASK_A); // not really required
DECLARE_BUILTIN_NODE_TRAITS(FamilyName, std::string, 10);
DECLARE_BUILTIN_NODE_TRAITS(DataArray, double, 17);
DECLARE_BASIC_NODE_TRAITS(Rind, 11);
DECLARE_BASIC_NODE_TRAITS(IndexRange1D, 12);
DECLARE_BASIC_NODE_TRAITS(IndexRange, 12);
//DECLARE_BASIC_NODE_TRAITS(DonorRange, 12);
//DECLARE_BASIC_NODE_TRAITS(_Range2D, 12);
//DECLARE_BASIC_NODE_TRAITS(_Range3D, 12);
DECLARE_BASIC_NODE_TRAITS(GridLocation, 13);
DECLARE_BASIC_NODE_TRAITS(Transform, 14);
DECLARE_BASIC_NODE_TRAITS(Normal, MASK_C);
//DECLARE_BUILTIN_NODE_TRAITS(int[], int, 14); // not really required
DECLARE_CLASSED_NODE_TRAITS(DataArray, DimensionalExponents, MASK_A);
DECLARE_CLASSED_NODE_TRAITS(DataArray, DataConversion, MASK_B);
DECLARE_CLASSED_NODE_TRAITS(Private, DiffusionModel_t, MASK_A);
//DECLARE_BUILTIN_NODE_TRAITS(GeometryReference, std::string, MASK_A);
DECLARE_CLASSED_NODE_TRAITS(GeometryReference, GeometryFormat, MASK_B);
DECLARE_CLASSED_NODE_TRAITS(GeometryReference, GeometryEntity, MASK_C);


#undef DECLARE_NODE_TRAITS
#undef DECLARE_BASIC_NODE_TRAITS
#undef DECLARE_CLASSED_NODE_TRAITS

} // namespace Private

} // namespace CGNS

#endif
