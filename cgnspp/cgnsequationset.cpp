/***************************************************************************
                          cgnsequationset.cpp  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsequationset.h"

#include "cgnsconverged.h"

#include "cgnsmodel.h"

#include <assert.h>

using CGNS::FlowEquationSet;

GENERATE_D(FlowEquationSet);

COMMON_NODE_IMPLEMENTATION(FlowEquationSet);

INIT_IMPLEMENTATION6(FlowEquationSet, mEquations, mGas, mViscosity,
	mConductivity, mClosure, mTurbulenceModel);

void CGNS::Private::FlowEquationSet::Read(FlowEquationSet * me, std::auto_ptr<int> & cDest) {
	if (&cDest==&me->mOrdinal) {
		readChild(me->mNode, "Ordinal", "int", cDest);
		return;
	}
	if (&cDest==&me->mDimension) {
		readChild(me->mNode, "EquationDimension", "int", cDest);
		return;
	}
	assert(0);
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION_WRITE(EquationDimension, int,
	FlowEquationSet, D()->mDimension);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(GoverningEquations, GoverningEquations,
	FlowEquationSet, D()->mEquations, CGNS::GoverningEquations::GoverningEquationsType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(GasModel, GasModel,
	FlowEquationSet, D()->mGas, CGNS::GasModel::GasModelType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(ViscosityModel, ViscosityModel,
	FlowEquationSet, D()->mViscosity, CGNS::ViscosityModel::ViscosityModelType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(ThermalConductivityModel, ThermalConductivityModel,
	FlowEquationSet, D()->mConductivity, CGNS::ThermalConductivityModel::ThermalConductivityModelType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(TurbulenceClosure, TurbulenceClosure,
	FlowEquationSet, D()->mClosure, CGNS::TurbulenceClosure::TurbulenceClosureType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(TurbulenceModel, TurbulenceModel,
	FlowEquationSet, D()->mTurbulenceModel, CGNS::TurbulenceModel::TurbulenceModelType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(ThermalRelaxationModel, ThermalRelaxationModel,
	FlowEquationSet, D()->mThermalRelaxation, CGNS::ThermalRelaxationModel::ThermalRelaxationModelType);

OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(ChemicalKineticsModel, ChemicalKineticsModel,
	FlowEquationSet, D()->mChemicalKinetics, CGNS::ChemicalKineticsModel::ChemicalKineticsModelType);

