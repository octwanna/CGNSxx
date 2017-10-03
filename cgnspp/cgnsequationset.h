/***************************************************************************
                          cgnsequationset.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSEQUATIONSET_H
#define CGNSEQUATIONSET_H

#include "cgnsdimensioned.h"

#include "cgnsconverged.h"

#include "cgnsvalued.h"

/**Implementation data for FlowEquationSet_t subnodes
  */
struct CGNS::Private::FlowEquationSet : CGNS::Private::Dimensioned  {
COMMON_NODE_INTERFACE(FlowEquationSet, Converged, Dimensioned);
	std::auto_ptr<int> mDimension;
	refcnt_ptr<GoverningEquations> mEquations;
	refcnt_ptr<GasModel> mGas;
	refcnt_ptr<ViscosityModel> mViscosity;
	refcnt_ptr<ThermalConductivityModel> mConductivity;
	refcnt_ptr<TurbulenceClosure> mClosure;
	refcnt_ptr<TurbulenceModel> mTurbulenceModel;
	refcnt_ptr<ThermalRelaxationModel> mThermalRelaxation;
	refcnt_ptr<ChemicalKineticsModel> mChemicalKinetics;
	
	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	USING_READ(Dimensioned);
	static void Read(FlowEquationSet * me, std::auto_ptr<int> & cDest);
// int is twice, once for Ordinal_t and once for EquationDimension, so leave it out
DISTINCT_CHILDREN8(GoverningEquations, CGNS::GasModel, CGNS::ViscosityModel,
	CGNS::ThermalConductivityModel, CGNS::TurbulenceClosure, TurbulenceModel,
	CGNS::ThermalRelaxationModel, CGNS::ChemicalKineticsModel);
};

#endif
